#ifndef _HACCESS_HDB_H_
#define _HACCESS_HDB_H_ // To make sure haccess_hdb.h is included only once
//------------------------------------------------------------------------------------------------//
// Description:
//   HACCESS_HDB template class to support both HACCESS & HDB API
//
// Revision History:
//   2024-10-01: Original [Murali Boddeti (or)  mkb]
//------------------------------------------------------------------------------------------------//

// Include Files: system header file
#include <map>
#include <cmath>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/case_conv.hpp>

// Include Files: external header file
#include "haccess.h"
#include "hdb.h"
#include "scf.h"
#include "appmgr.h"
#include "timedate.h"

// Include Files: Current program header file
#include "debug_manager.h"
#include "common_util.h"
#include "hdb_util.h"

//#define EMP_VERSION_2_6

#define HABITAT_INT_NULL -2139062144

/*
//#define HABITAT_FLOAT_NULL -1.1801e-38
#define HABITAT_FLOAT_NULL 1.18010406e-37
*/

// Class Definitions
class DBG_MGR;

class HACCESS_FIELD {
public:
    int Type;
    int Size;
    HaccessFieldBase *Buf;
    HaccessMask *Mask;
    HACCESS_FIELD () : Type(0), Size(0), Buf(NULL), Mask(NULL) {};
};

template <class T> class HACCESS_HDB {

public:
    HACCESS_HDB (DBG_MGR& iDm) : Db("UNKNOWN") {
        pDm  = &iDm;
        Init = false;
        Precision = 6; // Default
        //SetContext (); // Set default context values
    }
    ~HACCESS_HDB () {}

    std::string GetContext () { return Db + "." + App + "." + Fam; }
    std::string GetApp () { return App; }
    std::string GetFam () { return Fam; }
    std::string GetDb ()  { return Db;  }

    int Precision;
    void SetPrecision (int iPrecision)  { Precision = iPrecision; }

    T& Read (const std::string& iApp, const std::string& iFam) { DBG_MGR_LOG;
        App = iApp; // Set Context App
        Fam = iFam; // Set Context Fam

        if (Init) return Read(); // Initialized read and return Db pointer

        Haccess pHaccess;
        pMom = boost::shared_ptr <T> (new T(&pHaccess, iApp.c_str(), iFam.c_str()));
        //HACSERT(NULL == pMom); // Crash if MOM DB not created

        HaccessDbParams* lpParams = pMom->GetParams();
        if (lpParams != NULL) Db = lpParams->GetName(); // Set the Db Name

        if (pMom->GetHandle() == 0) {
            GetDm().Print (DML_W,DMF_Mom,"%s::Error in initializing (%s_%s_%s), Is HABITAT running? Is Clone exists?",
                __FUNCTION__, Db.c_str(), App.c_str(), Fam.c_str());
            return *pMom;
        }
        else {
            Init = true;
            pMom->DoAll(Haccess::readmx);
            pMom->DoAll(Haccess::alloc); // Since Object is closed, allocate again
            return Read();
        }
    }

    T& Read () { DBG_MGR_LOG;
        HaccessTransactionReadonly t;
        GetDm().Print (DML_1,DMF_Mom,"Reading DB(%s_%s_%s)", Db.c_str(), App.c_str(), Fam.c_str());
        pMom->DoAll(Haccess::readlv); // Read LV & MX
        pMom->DoAll(Haccess::read);   // Read DB
        return *pMom;
    }

    void Write() { DBG_MGR_LOG;
        GetDm().Print (DML_1,DMF_Mom,"Writing DB(%s_%s_%s)", Db.c_str(), App.c_str(), Fam.c_str());
        pMom->DoAll(Haccess::write);   // Write DB
    }

    inline T& GetMom() { DBG_MGR_LOG;
        return *pMom;
    }

    void Clear() { DBG_MGR_LOG;
        if (pMom != NULL) pMom->DoAll(Haccess::dealloc);
        Init = false;
    }

    int GetLv(const char *iRec) { DBG_MGR_LOG;
        int lRecLv = 0;
        if (UtilScfBad(HdbGetRecordLV (GetRecHdl(iRec), &lRecLv))) {
            GetDm().Print (DML_E,DMF_Mom,"%s::Error Cannot GetLv for(%s)", __FUNCTION__, iRec);
        }
        return lRecLv;
    }

    int GetMx(const char* iRec) { DBG_MGR_LOG;
        int lRecMx = 0;
        if (UtilScfBad(HdbGetRecordMX (GetRecHdl(iRec), &lRecMx))) {
            GetDm().Print (DML_E,DMF_Mom, "%s::Cannot GetLv for(%s) - (%s)", __FUNCTION__, iRec);
        }
        return lRecMx;
    }

    int AddOrDelRec (const char *iRec, unsigned int iSub, const int& iNum, const int& ipSub) { DBG_MGR_LOG;
        // Returns subscript where insert happened or subscript where deletion left off
        // This routine assumes that there is enough room available -- Need revision - XXXXXXXXXXXXX
        if ( iNum > 0 ) ExpRec(iRec, iSub, iNum, ipSub);
        else if ( iNum < 0 ) iSub = DelRec(iRec, iSub+1, -iNum); // Delete from iSub excluding iSub
        return iSub;
    }

    bool CheckSz (const char *iRec, const int& iNum) { DBG_MGR_LOG;
        int lCurMx = GetMx(iRec);
        if ( iNum > lCurMx ) {
            GetDm().Print (DML_E,"Cannot Expand(%s), to (%d) Mx(%d)", iRec, iNum, lCurMx);
            return false;
        }
        else return true;
    }

    bool ExpRec (const char *iRec, unsigned int& iSub, const int& iNum, const int& ipSub) { DBG_MGR_LOG;
        DBG_MGR& lDm = GetDm();

        if (iNum <=0) return false;

        // Check if record can be expanded to the requested number, commented to support circular record expansion
        // Enabled it again, this validation is needed but may need to do a correct solution
        int lCurLv = GetLv(iRec);
        int lCurMx = GetMx(iRec);
        int lNum = iNum;
        if (iNum > (lCurMx - lCurLv)) {
            lNum = lCurMx - lCurLv;
            lDm.Print (DML_1,"Cannot Expand(%s), Mx(%d), Lv(%d), Requested(%d) capping it to max", iRec, lCurMx, lCurLv, iNum);
        }

        SCF_STATUS lStatus = HdbExpand (GetRecHdl(iRec), iSub, lNum, ipSub, 0);
        if (UtilScfBad(lStatus)) {
            lDm.Print (DML_E,DMF_Mom,"%s::Cannot Expand(%s) at(%d), num(%d), par pos(%d)", __FUNCTION__, iRec, iSub, lNum, ipSub);
            return false;
        }
        else {
            lDm.Print (DML_1,DMF_Mom,"Expanded(%s) at(%d), num(%d), par pos(%d) in DB(%s_%s_%s)",
                iRec, iSub, lNum, ipSub, Db.c_str(), App.c_str(), Fam.c_str());
            return true;
        }
    }

    int DelRec (const char *iRec, int iSub, const int& iNum) { DBG_MGR_LOG;
        DBG_MGR& lDm = GetDm();
        if (iNum <=0) return iSub;
        SCF_STATUS lStatus = HdbDelete(GetRecHdl(iRec), iSub, iNum);
        if (UtilScfBad(lStatus)) {
            lDm.Print (DML_E,DMF_Mom, "%s::Error Cannot delete rows(%d) from(%s.%s.%s.%s)",
                __FUNCTION__, iNum, App.c_str(), Fam.c_str(), Db.c_str(), iRec);
        }
        else {
            lDm.Print (DML_1,DMF_Mom, "Deleted rows(%d) from(%s_%s_%s_%s)", iNum, iRec, Db.c_str(), App.c_str(), Fam.c_str());
            iSub = iSub+1; // Next records after delete
        }
        return iSub;
    }

    int ExpLastRec (const char *iRec, const int& iNum, const int& ipSub) { DBG_MGR_LOG;
        DBG_MGR& lDm = GetDm();
        if (iNum <=0) return false;

        int lSub = 0;
        SCF_STATUS lStatus = HdbExpandAtLast (GetRecHdl(iRec), iNum, ipSub, 0, &lSub);
        if (UtilScfBad(lStatus)) {
            lDm.Print (DML_E,DMF_Mom,"%s::Cannot Expand(%s) at last, num(%d), par pos(%d)", __FUNCTION__, iRec, iNum, ipSub);
            return 0;
        }
        else {
            lDm.Print (DML_1,DMF_Mom,"Expanded(%s) at last, num(%d), par pos(%d) in DB(%s_%s_%s)",
                iRec, iNum, ipSub, Db.c_str(), App.c_str(), Fam.c_str());
            return lSub;
        }
    }

    std::string Trim (std::string iStr) {
        boost::algorithm::trim(iStr);
        return iStr;
    }

    template <typename T1> bool IsEmpty (T1 iKey1) {
        if (GetString(iKey1).empty()) return true;
        return false;
    }
    template <typename T1> void CopyIfEmpty (std::string iSrc, T1 iDest) {
        if (IsEmpty(iDest)) iDest.CopyFromSz(iSrc.c_str());
    }
    template <typename T1> void Copy (std::string iSrc, T1 iDest) {
        iDest.CopyFromSz(iSrc.c_str());
    }
    template <typename T1, typename T2> void Copy (T1 iSrc, T2 iDest) {
        iDest.CopyFromSz(GetString(iSrc).c_str());
    }
    template <typename T1> std::string GetString (T1 iKey1) {
        if (iKey1.IsEmptyAfterTrim()) return "";
        if (iKey1.TestNull()) return "";
//        return iKey1.GetStringTrimedTrailBlanks();
        return iKey1.c_str_trim_trail_blanks();
    }

    template <typename T1>
    std::string HdbCompId (T1 iKey1) {
        return GetString(iKey1);
    }
    template <typename T1, typename T2>
    std::string HdbCompId (T1 iKey1, T2 iKey2) {
        return Trim(GetString(iKey1) + " " + GetString(iKey2));
    }
    template <typename T1, typename T2, typename T3>
    std::string HdbCompId (T1 iKey1, T2 iKey2, T3 iKey3) {
        return Trim(GetString(iKey1) + " " + GetString(iKey2) + " " + GetString(iKey3));
    }
    template <typename T1, typename T2, typename T3, typename T4>
    std::string HdbCompId (T1 iKey1, T2 iKey2, T3 iKey3, T4 iKey4) {
        return Trim(GetString(iKey1) + " " + GetString(iKey2) + " " + GetString(iKey3) + " " + GetString(iKey4));
    }
    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    std::string HdbCompId (T1 iKey1, T2 iKey2, T3 iKey3, T4 iKey4, T5 iKey5) {
        return Trim(GetString(iKey1) + " " + GetString(iKey2) + " " + GetString(iKey3) + " " + GetString(iKey4) + " " + GetString(iKey5));
    }
    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    std::string HdbCompId (T1 iKey1, T2 iKey2, T3 iKey3, T4 iKey4, T5 iKey5, T6 iKey6) {
        return Trim(GetString(iKey1) + " " + GetString(iKey2) + " " + GetString(iKey3) + " " +
               GetString(iKey4) + " " + GetString(iKey5) + " " + GetString(iKey6));
    }

    // Haccess hash ables
    template <typename T1, typename T2>
    void AddHierHash (T1& iHash, int iEnd, T2& iId) { DBG_MGR_LOG;
        iHash.clear();
        for ( int i=1; i<=iEnd; i++ ) {
            std::string lKey = HdbCompId (iId[i]);
            if (!lKey.empty()) iHash.insert(std::make_pair(lKey,i));
        }
    }
    template <typename T1, typename T2, typename T3, typename P>
    void AddHierHash (T1& iHash, int iEnd, T2& iId, T3& iPar1Id, P& iPar1Ptr) { DBG_MGR_LOG;
        iHash.clear();
        for ( int i=1; i<=iEnd; i++ ) {
            std::string lKey = HdbCompId (iPar1Id[iPar1Ptr[i]], iId[i]);
            if (!lKey.empty()) iHash.insert(std::make_pair(lKey,i));
        }
    }
    template <typename T1, typename T2, typename T3, typename T4, typename P>
    void AddHierHash (T1& iHash, int iEnd, T2& iId, T3& iPar1Id, T4& iPar2Id, P& iPar1Ptr, P& iPar2Ptr) { DBG_MGR_LOG;
        iHash.clear();
        for ( int i=1; i<=iEnd; i++ ) {
            std::string lKey = HdbCompId (iPar2Id[iPar2Ptr[iPar1Ptr[i]]], iPar1Id[iPar1Ptr[i]], iId[i]);
            if (!lKey.empty()) iHash.insert(std::make_pair(lKey,i));
        }
    }
    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename P>
    void AddHierHash (T1& iHash, int iEnd, T2& iId, T3& iPar1Id, T4& iPar2Id, T5& iPar3Id,
        P& iPar1Ptr, P& iPar2Ptr, P& iPar3Ptr) { DBG_MGR_LOG;
        iHash.clear();
        for ( int i=1; i<=iEnd; i++ ) {
            std::string lKey = HdbCompId (iPar3Id[iPar3Ptr[iPar2Ptr[iPar1Ptr[i]]]],
                iPar2Id[iPar2Ptr[iPar1Ptr[i]]], iPar1Id[iPar1Ptr[i]], iId[i]);
            if (!lKey.empty()) iHash.insert(std::make_pair(lKey,i));
        }
    }
    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename P>
    void AddHierHash (T1& iHash, int iEnd, T2& iId, T3& iPar1Id, T4& iPar2Id, T5& iPar3Id, T6& iPar4Id,
        P& iPar1Ptr, P& iPar2Ptr, P& iPar3Ptr, P& iPar4Ptr) { DBG_MGR_LOG;
        iHash.clear();
        for ( int i=1; i<=iEnd; i++ ) {
            std::string lKey = HdbCompId (iPar4Id[iPar4Ptr[iPar3Ptr[iPar2Ptr[iPar1Ptr[i]]]]],
                iPar3Id[iPar3Ptr[iPar2Ptr[iPar1Ptr[i]]]], iPar2Id[iPar2Ptr[iPar1Ptr[i]]], iPar1Id[iPar1Ptr[i]], iId[i]);
            if (!lKey.empty()) iHash.insert(std::make_pair(lKey,i));
        }
    }

    void CreateSavecase (std::string iApp, std::string iFam, std::string iCaseType, std::string iTitle, std::string iLoc) { DBG_MGR_LOG;
        DBG_MGR& lDm = GetDm();
        lDm.Print (DML_I, DMF_U, "Creating Savecase for(%s.%s) CaseType(%s) Title(%s) Location(%s)",
            iApp.c_str(), iFam.c_str(), iCaseType.c_str(), iTitle.c_str(), iLoc.c_str());
        if (UtilScfBad (CasemgrCreateSavecase (iApp.c_str(), iCaseType.c_str(), iTitle.c_str(), iFam.c_str(), iLoc.c_str(), CASEMGR_DEFAULT))) {
            lDm.Print (DML_E,DMF_Mom,"%s::Error for(%s_%s) CaseType(%s) Title(%s) Location(%s)",
                __FUNCTION__, iApp.c_str(), iFam.c_str(), iCaseType.c_str(), iTitle.c_str(), iLoc.c_str());
        }
    }

    void WriteField(std::string iField, std::string iRec, std::string iBuf) { DBG_MGR_LOG;
        for (int i=1; i<=GetLv(iRec.c_str()); i++) WriteField(iField, iRec, iBuf, i);
    }
    void WriteField(std::string iField, std::string iRec, std::string iBuf, int iSub) { DBG_MGR_LOG;
        DBG_MGR& lDm = GetDm();

        std::string lField = iField.substr(iField.rfind(".") + 1); // Ignore Db on the left if exists

        lDm.Print (DML_5,DMF_Mom, "%s:Writing Field(%s), Rec(%s) Buf(%s) Sub(%d)",
            __FUNCTION__, lField.c_str(), iRec.c_str(), iBuf.c_str(), iSub);

        auto& lUtil = COMMON_UTIL(lDm);

        void *lBuf = NULL;
        int   lInt = 0;
        float lFloat = 0.0;

        switch (GetFieldDataType (lField,iRec)) {

            case HDB_OBJECT_FIELD_INT :
            case HDB_OBJECT_FIELD_BOOL :
                lInt = lUtil.Convert <int> (iBuf);
                lBuf = &lInt;
                break;

            case HDB_OBJECT_FIELD_FLOAT :
                lFloat = lUtil.Convert <float> (iBuf);
                lBuf = &lFloat;
                break;

            case HDB_OBJECT_FIELD_MASK :
                if (boost::iequals(iBuf,"true")) lInt = 1;
                lBuf = &lInt;
                break;

            case HDB_OBJECT_FIELD_CHAR :
                lBuf = (void *)iBuf.c_str();
                break;

            case HDB_OBJECT_FIELD_DATE :
                lDm.Print (DML_E,DMF_Mom, "%s:Field(%s), Rec(%s) Buf(%s) Sub(%s) cannot handle Type(HDB_OBJECT_FIELD_DATE)",
                    __FUNCTION__, lField.c_str(), iRec.c_str(), iBuf.c_str(), iSub);
                return;

            case HDB_OBJECT_FIELD_TIME :
                lDm.Print (DML_E,DMF_Mom, "%s:Field(%s), Rec(%s) Buf(%s) Sub(%s) cannot handle Type(HDB_OBJECT_FIELD_TIME)",
                    __FUNCTION__, lField.c_str(), iRec.c_str(), iBuf.c_str(), iSub);
                return;

            case HDB_OBJECT_FIELD_BIT :
                lDm.Print (DML_E,DMF_Mom, "%s:Field(%s), Rec(%s) Buf(%s) Sub(%s) cannot handle Type(HDB_OBJECT_FIELD_BIT)",
                    __FUNCTION__, lField.c_str(), iRec.c_str(), iBuf.c_str(), iSub);
                return;

        }

        HdbPosition lPos;
        lPos.position = iSub;
        lPos.mode = HDB_MODE_RECORD;
        if (UtilScfBad(HdbWrite(GetFieldHandle(lField,iRec), lBuf, &lPos))) {
            lDm.Print (DML_E,DMF_Mom,"%s::Error in HdbWrite for(%s_%s)", __FUNCTION__, lField.c_str(), iRec.c_str());
        }
    }

    std::map <std::string,HACCESS_FIELD> FieldDtls;
    HACCESS_FIELD& GetFieldDtls(const std::string& iField, std::string& iRec) { DBG_MGR_LOG;
        if (FieldDtls.find(iField) != FieldDtls.end()) return FieldDtls[iField];
        auto& lFieldDtls = FieldDtls[iField];

        std::string lField = iField.substr(iField.rfind(".") + 1); // Ignore Db on the left if exists

        lFieldDtls.Type = GetFieldDataType (lField,iRec);
        lFieldDtls.Size = GetFieldDataSize (lField,iRec);
        lFieldDtls.Buf  = pMom->FindField(lField.c_str());
        lFieldDtls.Mask = pMom->FindMask(lField.c_str());

        return lFieldDtls;
    }

    std::string GetCompFieldValueExpr(const std::string& iField, std::string& iRec, int iSub) { DBG_MGR_LOG;
        size_t lpPos1 = 0;
        size_t lpPos2 = 0;
        std::string lField = iField;
        while ( (lpPos1 = lField.find('{', lpPos1)) != std::string::npos ) {
            std::string lExpr = Trim(lField.substr(lpPos2,lpPos1));
            if ( (lpPos2 = lField.find('}', lpPos1)) != std::string::npos ) { // Between lpPos1 and lpPos2 comp field exists
                std::string lCompField = lField.substr(lpPos1+1,lpPos2-1);
                lField.erase (lpPos1, lpPos2-lpPos1+1);
                lField.insert (lpPos1, GetCompFieldValue(lCompField, iRec, iSub));
            }
            lpPos1++; // To substitute any other environment variables
        }
        return lField;
    }
    std::string GetCompFieldValue(const std::string& iField, std::string& iRec, int iSub) { DBG_MGR_LOG;
        DBG_MGR& lDm = GetDm();
        auto& lUtil = COMMON_UTIL(lDm);

        std::vector <std::string> lFieldCols;
        boost::split(lFieldCols, iField, boost::is_any_of(" "));

        int lSub = iSub;
        std::string lRec = iRec;

        std::string lRetVal = "";
        BOOST_FOREACH (auto& it, lFieldCols) {
            std::string lField = it.substr(it.rfind(".") + 1);
            if (boost::iequals(lField,"$SUB") ||  boost::iequals(lField,"$SUB_"+iRec)) {
                lRetVal = lRetVal + " " + lUtil.Id(lSub);
                continue;
            }
            else if (lField.find("__") != std::string::npos || lField.find("$") != std::string::npos) { // Work through the pointers
                lSub = lUtil.Convert <int> (GetFieldValue (lField, lRec, lSub));
                continue;
            }
            else if (lField.at(0) == '+') {
                lSub += lUtil.Convert <int> (lField.substr(lField.find("+") + 1));
                continue;
            }
            else if (lField.at(0) == '-') {
                lSub -= lUtil.Convert <int> (lField.substr(lField.find("-") + 1));
                continue;
            }
            lRec = it.substr(it.rfind("_") + 1); // record will change from now on
            lRetVal = lRetVal + " " + GetFieldValue (lField, lRec, lSub);
        }

        lRetVal = Trim(lRetVal);
//        lDm.Print (DML_I,DMF_Mom, "%s:Field(%s), Rec(%s) Sub(%d) Value(%s)", __FUNCTION__, iField.c_str(), iRec.c_str(), iSub, lRetVal.c_str());

        return lRetVal;
    }

    std::string GetFieldValue(const std::string& iField, std::string& iRec, int iSub) { DBG_MGR_LOG;
        DBG_MGR& lDm = GetDm();
        auto& lUtil = COMMON_UTIL(lDm);
        auto& lHdbUtil = HDB_UTIL(lDm);

//        lDm.Print (DML_I,DMF_Mom, "%s:Field(%s), Rec(%s) Sub(%d)", __FUNCTION__, iField.c_str(), iRec.c_str(), iSub);

        if (iSub == 0) return "";
        if (boost::iequals(iField,"$SUB") || boost::iequals(iField,"$SUB_"+iRec)) return lUtil.Id(iSub);

        auto& lFieldDtls = GetFieldDtls(iField, iRec);

        switch (lFieldDtls.Type) {
            case HDB_OBJECT_FIELD_MASK:
                if (lFieldDtls.Mask->Test(iSub)) return "True";
                else return "False";

            case HDB_OBJECT_FIELD_DATE:
                switch (lFieldDtls.Size) {
                    case  2: return lUtil.Id((*dynamic_cast<HaccessFieldInt2*>(lFieldDtls.Buf))[iSub]); // Just short integer for now
                    default: return ""; break;
                }

            case HDB_OBJECT_FIELD_TIME:
                switch (lFieldDtls.Size) {
                    case  4: return lHdbUtil.GetHdbTimeStr((*dynamic_cast<HaccessFieldInt4*>(lFieldDtls.Buf))[iSub]);
                    case  8: return lHdbUtil.GetHdbTimeStr((*dynamic_cast<HaccessFieldInt8*>(lFieldDtls.Buf))[iSub]);
                    default: return ""; break;
                }

            case HDB_OBJECT_FIELD_INT:
            case HDB_OBJECT_FIELD_BOOL:
                switch (lFieldDtls.Size) {
                    case  2: return lUtil.Id((*dynamic_cast<HaccessFieldInt2*>(lFieldDtls.Buf))[iSub]);
                    case  4: return lUtil.Id((*dynamic_cast<HaccessFieldInt4*>(lFieldDtls.Buf))[iSub]);
                    case  8: return lUtil.Id((*dynamic_cast<HaccessFieldInt8*>(lFieldDtls.Buf))[iSub]);
                    default: return ""; break;
                }

            case HDB_OBJECT_FIELD_FLOAT:
                switch (lFieldDtls.Size) {
                    case  4: return FloatVal((*dynamic_cast<HaccessFieldReal4*>(lFieldDtls.Buf))[iSub]);
                    case  8: return FloatVal((*dynamic_cast<HaccessFieldReal8*>(lFieldDtls.Buf))[iSub]);
                    default: return "";
                }
            case HDB_OBJECT_FIELD_CHAR:
                switch (lFieldDtls.Size) {
                    case  1: return GetString((*(dynamic_cast<HaccessFieldStr< 1>*>(lFieldDtls.Buf)))[iSub]);
                    case  2: return GetString((*(dynamic_cast<HaccessFieldStr< 2>*>(lFieldDtls.Buf)))[iSub]);
                    case  3: return GetString((*(dynamic_cast<HaccessFieldStr< 3>*>(lFieldDtls.Buf)))[iSub]);
                    case  4: return GetString((*(dynamic_cast<HaccessFieldStr< 4>*>(lFieldDtls.Buf)))[iSub]);
                    case  5: return GetString((*(dynamic_cast<HaccessFieldStr< 5>*>(lFieldDtls.Buf)))[iSub]);
                    case  6: return GetString((*(dynamic_cast<HaccessFieldStr< 6>*>(lFieldDtls.Buf)))[iSub]);
                    case  7: return GetString((*(dynamic_cast<HaccessFieldStr< 7>*>(lFieldDtls.Buf)))[iSub]);
                    case  8: return GetString((*(dynamic_cast<HaccessFieldStr< 8>*>(lFieldDtls.Buf)))[iSub]);
                    case  9: return GetString((*(dynamic_cast<HaccessFieldStr< 9>*>(lFieldDtls.Buf)))[iSub]);

                    case 10: return GetString((*(dynamic_cast<HaccessFieldStr<10>*>(lFieldDtls.Buf)))[iSub]);
                    case 11: return GetString((*(dynamic_cast<HaccessFieldStr<11>*>(lFieldDtls.Buf)))[iSub]);
                    case 12: return GetString((*(dynamic_cast<HaccessFieldStr<12>*>(lFieldDtls.Buf)))[iSub]);
                    case 13: return GetString((*(dynamic_cast<HaccessFieldStr<13>*>(lFieldDtls.Buf)))[iSub]);
                    case 14: return GetString((*(dynamic_cast<HaccessFieldStr<14>*>(lFieldDtls.Buf)))[iSub]);
                    case 15: return GetString((*(dynamic_cast<HaccessFieldStr<15>*>(lFieldDtls.Buf)))[iSub]);
                    case 16: return GetString((*(dynamic_cast<HaccessFieldStr<16>*>(lFieldDtls.Buf)))[iSub]);
                    case 17: return GetString((*(dynamic_cast<HaccessFieldStr<17>*>(lFieldDtls.Buf)))[iSub]);
                    case 18: return GetString((*(dynamic_cast<HaccessFieldStr<18>*>(lFieldDtls.Buf)))[iSub]);
                    case 19: return GetString((*(dynamic_cast<HaccessFieldStr<19>*>(lFieldDtls.Buf)))[iSub]);

                    case 20: return GetString((*(dynamic_cast<HaccessFieldStr<20>*>(lFieldDtls.Buf)))[iSub]);
                    case 21: return GetString((*(dynamic_cast<HaccessFieldStr<21>*>(lFieldDtls.Buf)))[iSub]);
                    case 22: return GetString((*(dynamic_cast<HaccessFieldStr<22>*>(lFieldDtls.Buf)))[iSub]);
                    case 23: return GetString((*(dynamic_cast<HaccessFieldStr<23>*>(lFieldDtls.Buf)))[iSub]);
                    case 24: return GetString((*(dynamic_cast<HaccessFieldStr<24>*>(lFieldDtls.Buf)))[iSub]);
                    case 25: return GetString((*(dynamic_cast<HaccessFieldStr<25>*>(lFieldDtls.Buf)))[iSub]);
                    case 26: return GetString((*(dynamic_cast<HaccessFieldStr<26>*>(lFieldDtls.Buf)))[iSub]);
                    case 27: return GetString((*(dynamic_cast<HaccessFieldStr<27>*>(lFieldDtls.Buf)))[iSub]);
                    case 28: return GetString((*(dynamic_cast<HaccessFieldStr<28>*>(lFieldDtls.Buf)))[iSub]);
                    case 29: return GetString((*(dynamic_cast<HaccessFieldStr<29>*>(lFieldDtls.Buf)))[iSub]);

                    case 30: return GetString((*(dynamic_cast<HaccessFieldStr<30>*>(lFieldDtls.Buf)))[iSub]);
                    case 31: return GetString((*(dynamic_cast<HaccessFieldStr<31>*>(lFieldDtls.Buf)))[iSub]);
                    case 32: return GetString((*(dynamic_cast<HaccessFieldStr<32>*>(lFieldDtls.Buf)))[iSub]);
                    case 33: return GetString((*(dynamic_cast<HaccessFieldStr<33>*>(lFieldDtls.Buf)))[iSub]);
                    case 34: return GetString((*(dynamic_cast<HaccessFieldStr<34>*>(lFieldDtls.Buf)))[iSub]);
                    case 35: return GetString((*(dynamic_cast<HaccessFieldStr<35>*>(lFieldDtls.Buf)))[iSub]);
                    case 36: return GetString((*(dynamic_cast<HaccessFieldStr<36>*>(lFieldDtls.Buf)))[iSub]);
                    case 37: return GetString((*(dynamic_cast<HaccessFieldStr<37>*>(lFieldDtls.Buf)))[iSub]);
                    case 38: return GetString((*(dynamic_cast<HaccessFieldStr<38>*>(lFieldDtls.Buf)))[iSub]);
                    case 39: return GetString((*(dynamic_cast<HaccessFieldStr<39>*>(lFieldDtls.Buf)))[iSub]);

                    case 40: return GetString((*(dynamic_cast<HaccessFieldStr<40>*>(lFieldDtls.Buf)))[iSub]);
                    case 41: return GetString((*(dynamic_cast<HaccessFieldStr<41>*>(lFieldDtls.Buf)))[iSub]);
                    case 42: return GetString((*(dynamic_cast<HaccessFieldStr<42>*>(lFieldDtls.Buf)))[iSub]);
                    case 43: return GetString((*(dynamic_cast<HaccessFieldStr<43>*>(lFieldDtls.Buf)))[iSub]);
                    case 44: return GetString((*(dynamic_cast<HaccessFieldStr<44>*>(lFieldDtls.Buf)))[iSub]);
                    case 45: return GetString((*(dynamic_cast<HaccessFieldStr<45>*>(lFieldDtls.Buf)))[iSub]);
                    case 46: return GetString((*(dynamic_cast<HaccessFieldStr<46>*>(lFieldDtls.Buf)))[iSub]);
                    case 47: return GetString((*(dynamic_cast<HaccessFieldStr<47>*>(lFieldDtls.Buf)))[iSub]);
                    case 48: return GetString((*(dynamic_cast<HaccessFieldStr<48>*>(lFieldDtls.Buf)))[iSub]);
                    case 49: return GetString((*(dynamic_cast<HaccessFieldStr<49>*>(lFieldDtls.Buf)))[iSub]);

                    case 50: return GetString((*(dynamic_cast<HaccessFieldStr<50>*>(lFieldDtls.Buf)))[iSub]);
                    case 51: return GetString((*(dynamic_cast<HaccessFieldStr<51>*>(lFieldDtls.Buf)))[iSub]);
                    case 52: return GetString((*(dynamic_cast<HaccessFieldStr<52>*>(lFieldDtls.Buf)))[iSub]);
                    case 53: return GetString((*(dynamic_cast<HaccessFieldStr<53>*>(lFieldDtls.Buf)))[iSub]);
                    case 54: return GetString((*(dynamic_cast<HaccessFieldStr<54>*>(lFieldDtls.Buf)))[iSub]);
                    case 55: return GetString((*(dynamic_cast<HaccessFieldStr<55>*>(lFieldDtls.Buf)))[iSub]);
                    case 56: return GetString((*(dynamic_cast<HaccessFieldStr<56>*>(lFieldDtls.Buf)))[iSub]);
                    case 57: return GetString((*(dynamic_cast<HaccessFieldStr<57>*>(lFieldDtls.Buf)))[iSub]);
                    case 58: return GetString((*(dynamic_cast<HaccessFieldStr<58>*>(lFieldDtls.Buf)))[iSub]);
                    case 59: return GetString((*(dynamic_cast<HaccessFieldStr<59>*>(lFieldDtls.Buf)))[iSub]);

                    case 60: return GetString((*(dynamic_cast<HaccessFieldStr<60>*>(lFieldDtls.Buf)))[iSub]);
                    case 61: return GetString((*(dynamic_cast<HaccessFieldStr<61>*>(lFieldDtls.Buf)))[iSub]);
                    case 62: return GetString((*(dynamic_cast<HaccessFieldStr<62>*>(lFieldDtls.Buf)))[iSub]);
                    case 63: return GetString((*(dynamic_cast<HaccessFieldStr<63>*>(lFieldDtls.Buf)))[iSub]);
                    case 64: return GetString((*(dynamic_cast<HaccessFieldStr<64>*>(lFieldDtls.Buf)))[iSub]);
                    case 65: return GetString((*(dynamic_cast<HaccessFieldStr<65>*>(lFieldDtls.Buf)))[iSub]);
                    case 66: return GetString((*(dynamic_cast<HaccessFieldStr<66>*>(lFieldDtls.Buf)))[iSub]);
                    case 67: return GetString((*(dynamic_cast<HaccessFieldStr<67>*>(lFieldDtls.Buf)))[iSub]);
                    case 68: return GetString((*(dynamic_cast<HaccessFieldStr<68>*>(lFieldDtls.Buf)))[iSub]);
                    case 69: return GetString((*(dynamic_cast<HaccessFieldStr<69>*>(lFieldDtls.Buf)))[iSub]);

                    case 70: return GetString((*(dynamic_cast<HaccessFieldStr<70>*>(lFieldDtls.Buf)))[iSub]);
                    case 71: return GetString((*(dynamic_cast<HaccessFieldStr<71>*>(lFieldDtls.Buf)))[iSub]);
                    case 72: return GetString((*(dynamic_cast<HaccessFieldStr<72>*>(lFieldDtls.Buf)))[iSub]);
                    case 73: return GetString((*(dynamic_cast<HaccessFieldStr<73>*>(lFieldDtls.Buf)))[iSub]);
                    case 74: return GetString((*(dynamic_cast<HaccessFieldStr<74>*>(lFieldDtls.Buf)))[iSub]);
                    case 75: return GetString((*(dynamic_cast<HaccessFieldStr<75>*>(lFieldDtls.Buf)))[iSub]);
                    case 76: return GetString((*(dynamic_cast<HaccessFieldStr<76>*>(lFieldDtls.Buf)))[iSub]);
                    case 77: return GetString((*(dynamic_cast<HaccessFieldStr<77>*>(lFieldDtls.Buf)))[iSub]);
                    case 78: return GetString((*(dynamic_cast<HaccessFieldStr<78>*>(lFieldDtls.Buf)))[iSub]);
                    case 79: return GetString((*(dynamic_cast<HaccessFieldStr<79>*>(lFieldDtls.Buf)))[iSub]);

                    case 80: return GetString((*(dynamic_cast<HaccessFieldStr<80>*>(lFieldDtls.Buf)))[iSub]);
                    case 81: return GetString((*(dynamic_cast<HaccessFieldStr<81>*>(lFieldDtls.Buf)))[iSub]);
                    case 82: return GetString((*(dynamic_cast<HaccessFieldStr<82>*>(lFieldDtls.Buf)))[iSub]);
                    case 83: return GetString((*(dynamic_cast<HaccessFieldStr<83>*>(lFieldDtls.Buf)))[iSub]);
                    case 84: return GetString((*(dynamic_cast<HaccessFieldStr<84>*>(lFieldDtls.Buf)))[iSub]);
                    case 85: return GetString((*(dynamic_cast<HaccessFieldStr<85>*>(lFieldDtls.Buf)))[iSub]);
                    case 86: return GetString((*(dynamic_cast<HaccessFieldStr<86>*>(lFieldDtls.Buf)))[iSub]);
                    case 87: return GetString((*(dynamic_cast<HaccessFieldStr<87>*>(lFieldDtls.Buf)))[iSub]);
                    case 88: return GetString((*(dynamic_cast<HaccessFieldStr<88>*>(lFieldDtls.Buf)))[iSub]);
                    case 89: return GetString((*(dynamic_cast<HaccessFieldStr<89>*>(lFieldDtls.Buf)))[iSub]);

                    case 90: return GetString((*(dynamic_cast<HaccessFieldStr<90>*>(lFieldDtls.Buf)))[iSub]);
                    case 91: return GetString((*(dynamic_cast<HaccessFieldStr<91>*>(lFieldDtls.Buf)))[iSub]);
                    case 92: return GetString((*(dynamic_cast<HaccessFieldStr<92>*>(lFieldDtls.Buf)))[iSub]);
                    case 93: return GetString((*(dynamic_cast<HaccessFieldStr<93>*>(lFieldDtls.Buf)))[iSub]);
                    case 94: return GetString((*(dynamic_cast<HaccessFieldStr<94>*>(lFieldDtls.Buf)))[iSub]);
                    case 95: return GetString((*(dynamic_cast<HaccessFieldStr<95>*>(lFieldDtls.Buf)))[iSub]);
                    case 96: return GetString((*(dynamic_cast<HaccessFieldStr<96>*>(lFieldDtls.Buf)))[iSub]);
                    case 97: return GetString((*(dynamic_cast<HaccessFieldStr<97>*>(lFieldDtls.Buf)))[iSub]);
                    case 98: return GetString((*(dynamic_cast<HaccessFieldStr<98>*>(lFieldDtls.Buf)))[iSub]);
                    case 99: return GetString((*(dynamic_cast<HaccessFieldStr<99>*>(lFieldDtls.Buf)))[iSub]);

                    default: return "";
                }

            default:
                return ""; break;
        }
        return "";
    }

    bool SetFieldValue(const std::string& iField, std::string& iRec, int iSub, void *iBuf) { DBG_MGR_LOG;
        DBG_MGR& lDm = GetDm();
        auto& lUtil = COMMON_UTIL(lDm);
        auto& lHdbUtil = HDB_UTIL(lDm);

        bool lRetVal = true;

        std::string lField = iField;
        if (boost::iequals(iRec,"ITEMS") && (iField.find("_ITEMS") == std::string::npos)) lField += "_ITEMS";

        auto& lFieldDtls = GetFieldDtls(lField, iRec);

        switch (lFieldDtls.Type) {
            case HDB_OBJECT_FIELD_MASK:
                if (*(bool*) iBuf) lFieldDtls.Mask->Set(iSub);
                else lFieldDtls.Mask->Reset(iSub);

            case HDB_OBJECT_FIELD_DATE:
                switch (lFieldDtls.Size) {
                    case  2: (*dynamic_cast<HaccessFieldInt2*>(lFieldDtls.Buf))[iSub] = *(short*) iBuf;
                    default: lRetVal = false; break;
                }

            case HDB_OBJECT_FIELD_TIME:
                switch (lFieldDtls.Size) {
                    case  4: (*dynamic_cast<HaccessFieldInt4*>(lFieldDtls.Buf))[iSub] = *(int*) iBuf;
                    case  8: (*dynamic_cast<HaccessFieldInt8*>(lFieldDtls.Buf))[iSub] = *(long long int*) iBuf;
                    default: lRetVal = false; break;
                }

            case HDB_OBJECT_FIELD_INT:
            case HDB_OBJECT_FIELD_BOOL:
                switch (lFieldDtls.Size) {
                    case  2: (*dynamic_cast<HaccessFieldInt2*>(lFieldDtls.Buf))[iSub] = *(short int*) iBuf;
                    case  4: (*dynamic_cast<HaccessFieldInt4*>(lFieldDtls.Buf))[iSub] = *(int*) iBuf; 
                    case  8: (*dynamic_cast<HaccessFieldInt8*>(lFieldDtls.Buf))[iSub] = *(long long int*) iBuf;
                    default: lRetVal = false; break;
                }

            case HDB_OBJECT_FIELD_FLOAT:
                switch (lFieldDtls.Size) {
                    case  4: (*dynamic_cast<HaccessFieldReal4*>(lFieldDtls.Buf))[iSub] = *(float*) iBuf;
                    case  8: (*dynamic_cast<HaccessFieldReal8*>(lFieldDtls.Buf))[iSub] = *(double*) iBuf;
                    default: lRetVal = false; break;
                }
            case HDB_OBJECT_FIELD_CHAR:
                switch (lFieldDtls.Size) {
                    /*
                    case  1: return GetString((*(dynamic_cast<HaccessFieldStr< 1>*>(lFieldDtls.Buf)))[iSub]);
                    case  2: return GetString((*(dynamic_cast<HaccessFieldStr< 2>*>(lFieldDtls.Buf)))[iSub]);
                    case  3: return GetString((*(dynamic_cast<HaccessFieldStr< 3>*>(lFieldDtls.Buf)))[iSub]);
                    case  4: return GetString((*(dynamic_cast<HaccessFieldStr< 4>*>(lFieldDtls.Buf)))[iSub]);
                    case  5: return GetString((*(dynamic_cast<HaccessFieldStr< 5>*>(lFieldDtls.Buf)))[iSub]);
                    case  6: return GetString((*(dynamic_cast<HaccessFieldStr< 6>*>(lFieldDtls.Buf)))[iSub]);
                    case  7: return GetString((*(dynamic_cast<HaccessFieldStr< 7>*>(lFieldDtls.Buf)))[iSub]);
                    case  8: return GetString((*(dynamic_cast<HaccessFieldStr< 8>*>(lFieldDtls.Buf)))[iSub]);
                    case  9: return GetString((*(dynamic_cast<HaccessFieldStr< 9>*>(lFieldDtls.Buf)))[iSub]);

                    case 10: return GetString((*(dynamic_cast<HaccessFieldStr<10>*>(lFieldDtls.Buf)))[iSub]);
                    case 11: return GetString((*(dynamic_cast<HaccessFieldStr<11>*>(lFieldDtls.Buf)))[iSub]);
                    case 12: return GetString((*(dynamic_cast<HaccessFieldStr<12>*>(lFieldDtls.Buf)))[iSub]);
                    case 13: return GetString((*(dynamic_cast<HaccessFieldStr<13>*>(lFieldDtls.Buf)))[iSub]);
                    case 14: return GetString((*(dynamic_cast<HaccessFieldStr<14>*>(lFieldDtls.Buf)))[iSub]);
                    case 15: return GetString((*(dynamic_cast<HaccessFieldStr<15>*>(lFieldDtls.Buf)))[iSub]);
                    case 16: return GetString((*(dynamic_cast<HaccessFieldStr<16>*>(lFieldDtls.Buf)))[iSub]);
                    case 17: return GetString((*(dynamic_cast<HaccessFieldStr<17>*>(lFieldDtls.Buf)))[iSub]);
                    case 18: return GetString((*(dynamic_cast<HaccessFieldStr<18>*>(lFieldDtls.Buf)))[iSub]);
                    case 19: return GetString((*(dynamic_cast<HaccessFieldStr<19>*>(lFieldDtls.Buf)))[iSub]);

                    case 20: return GetString((*(dynamic_cast<HaccessFieldStr<20>*>(lFieldDtls.Buf)))[iSub]);
                    case 21: return GetString((*(dynamic_cast<HaccessFieldStr<21>*>(lFieldDtls.Buf)))[iSub]);
                    case 22: return GetString((*(dynamic_cast<HaccessFieldStr<22>*>(lFieldDtls.Buf)))[iSub]);
                    case 23: return GetString((*(dynamic_cast<HaccessFieldStr<23>*>(lFieldDtls.Buf)))[iSub]);
                    case 24: return GetString((*(dynamic_cast<HaccessFieldStr<24>*>(lFieldDtls.Buf)))[iSub]);
                    case 25: return GetString((*(dynamic_cast<HaccessFieldStr<25>*>(lFieldDtls.Buf)))[iSub]);
                    case 26: return GetString((*(dynamic_cast<HaccessFieldStr<26>*>(lFieldDtls.Buf)))[iSub]);
                    case 27: return GetString((*(dynamic_cast<HaccessFieldStr<27>*>(lFieldDtls.Buf)))[iSub]);
                    case 28: return GetString((*(dynamic_cast<HaccessFieldStr<28>*>(lFieldDtls.Buf)))[iSub]);
                    case 29: return GetString((*(dynamic_cast<HaccessFieldStr<29>*>(lFieldDtls.Buf)))[iSub]);

                    case 30: return GetString((*(dynamic_cast<HaccessFieldStr<30>*>(lFieldDtls.Buf)))[iSub]);
                    case 31: return GetString((*(dynamic_cast<HaccessFieldStr<31>*>(lFieldDtls.Buf)))[iSub]);
                    case 32: return GetString((*(dynamic_cast<HaccessFieldStr<32>*>(lFieldDtls.Buf)))[iSub]);
                    case 33: return GetString((*(dynamic_cast<HaccessFieldStr<33>*>(lFieldDtls.Buf)))[iSub]);
                    case 34: return GetString((*(dynamic_cast<HaccessFieldStr<34>*>(lFieldDtls.Buf)))[iSub]);
                    case 35: return GetString((*(dynamic_cast<HaccessFieldStr<35>*>(lFieldDtls.Buf)))[iSub]);
                    case 36: return GetString((*(dynamic_cast<HaccessFieldStr<36>*>(lFieldDtls.Buf)))[iSub]);
                    case 37: return GetString((*(dynamic_cast<HaccessFieldStr<37>*>(lFieldDtls.Buf)))[iSub]);
                    case 38: return GetString((*(dynamic_cast<HaccessFieldStr<38>*>(lFieldDtls.Buf)))[iSub]);
                    case 39: return GetString((*(dynamic_cast<HaccessFieldStr<39>*>(lFieldDtls.Buf)))[iSub]);

                    case 40: return GetString((*(dynamic_cast<HaccessFieldStr<40>*>(lFieldDtls.Buf)))[iSub]);
                    case 41: return GetString((*(dynamic_cast<HaccessFieldStr<41>*>(lFieldDtls.Buf)))[iSub]);
                    case 42: return GetString((*(dynamic_cast<HaccessFieldStr<42>*>(lFieldDtls.Buf)))[iSub]);
                    case 43: return GetString((*(dynamic_cast<HaccessFieldStr<43>*>(lFieldDtls.Buf)))[iSub]);
                    case 44: return GetString((*(dynamic_cast<HaccessFieldStr<44>*>(lFieldDtls.Buf)))[iSub]);
                    case 45: return GetString((*(dynamic_cast<HaccessFieldStr<45>*>(lFieldDtls.Buf)))[iSub]);
                    case 46: return GetString((*(dynamic_cast<HaccessFieldStr<46>*>(lFieldDtls.Buf)))[iSub]);
                    case 47: return GetString((*(dynamic_cast<HaccessFieldStr<47>*>(lFieldDtls.Buf)))[iSub]);
                    case 48: return GetString((*(dynamic_cast<HaccessFieldStr<48>*>(lFieldDtls.Buf)))[iSub]);
                    case 49: return GetString((*(dynamic_cast<HaccessFieldStr<49>*>(lFieldDtls.Buf)))[iSub]);

                    case 50: return GetString((*(dynamic_cast<HaccessFieldStr<50>*>(lFieldDtls.Buf)))[iSub]);
                    case 51: return GetString((*(dynamic_cast<HaccessFieldStr<51>*>(lFieldDtls.Buf)))[iSub]);
                    case 52: return GetString((*(dynamic_cast<HaccessFieldStr<52>*>(lFieldDtls.Buf)))[iSub]);
                    case 53: return GetString((*(dynamic_cast<HaccessFieldStr<53>*>(lFieldDtls.Buf)))[iSub]);
                    case 54: return GetString((*(dynamic_cast<HaccessFieldStr<54>*>(lFieldDtls.Buf)))[iSub]);
                    case 55: return GetString((*(dynamic_cast<HaccessFieldStr<55>*>(lFieldDtls.Buf)))[iSub]);
                    case 56: return GetString((*(dynamic_cast<HaccessFieldStr<56>*>(lFieldDtls.Buf)))[iSub]);
                    case 57: return GetString((*(dynamic_cast<HaccessFieldStr<57>*>(lFieldDtls.Buf)))[iSub]);
                    case 58: return GetString((*(dynamic_cast<HaccessFieldStr<58>*>(lFieldDtls.Buf)))[iSub]);
                    case 59: return GetString((*(dynamic_cast<HaccessFieldStr<59>*>(lFieldDtls.Buf)))[iSub]);

                    case 60: return GetString((*(dynamic_cast<HaccessFieldStr<60>*>(lFieldDtls.Buf)))[iSub]);
                    case 61: return GetString((*(dynamic_cast<HaccessFieldStr<61>*>(lFieldDtls.Buf)))[iSub]);
                    case 62: return GetString((*(dynamic_cast<HaccessFieldStr<62>*>(lFieldDtls.Buf)))[iSub]);
                    case 63: return GetString((*(dynamic_cast<HaccessFieldStr<63>*>(lFieldDtls.Buf)))[iSub]);
                    case 64: return GetString((*(dynamic_cast<HaccessFieldStr<64>*>(lFieldDtls.Buf)))[iSub]);
                    case 65: return GetString((*(dynamic_cast<HaccessFieldStr<65>*>(lFieldDtls.Buf)))[iSub]);
                    case 66: return GetString((*(dynamic_cast<HaccessFieldStr<66>*>(lFieldDtls.Buf)))[iSub]);
                    case 67: return GetString((*(dynamic_cast<HaccessFieldStr<67>*>(lFieldDtls.Buf)))[iSub]);
                    case 68: return GetString((*(dynamic_cast<HaccessFieldStr<68>*>(lFieldDtls.Buf)))[iSub]);
                    case 69: return GetString((*(dynamic_cast<HaccessFieldStr<69>*>(lFieldDtls.Buf)))[iSub]);

                    case 70: return GetString((*(dynamic_cast<HaccessFieldStr<70>*>(lFieldDtls.Buf)))[iSub]);
                    case 71: return GetString((*(dynamic_cast<HaccessFieldStr<71>*>(lFieldDtls.Buf)))[iSub]);
                    case 72: return GetString((*(dynamic_cast<HaccessFieldStr<72>*>(lFieldDtls.Buf)))[iSub]);
                    case 73: return GetString((*(dynamic_cast<HaccessFieldStr<73>*>(lFieldDtls.Buf)))[iSub]);
                    case 74: return GetString((*(dynamic_cast<HaccessFieldStr<74>*>(lFieldDtls.Buf)))[iSub]);
                    case 75: return GetString((*(dynamic_cast<HaccessFieldStr<75>*>(lFieldDtls.Buf)))[iSub]);
                    case 76: return GetString((*(dynamic_cast<HaccessFieldStr<76>*>(lFieldDtls.Buf)))[iSub]);
                    case 77: return GetString((*(dynamic_cast<HaccessFieldStr<77>*>(lFieldDtls.Buf)))[iSub]);
                    case 78: return GetString((*(dynamic_cast<HaccessFieldStr<78>*>(lFieldDtls.Buf)))[iSub]);
                    case 79: return GetString((*(dynamic_cast<HaccessFieldStr<79>*>(lFieldDtls.Buf)))[iSub]);

                    case 80: return GetString((*(dynamic_cast<HaccessFieldStr<80>*>(lFieldDtls.Buf)))[iSub]);
                    case 81: return GetString((*(dynamic_cast<HaccessFieldStr<81>*>(lFieldDtls.Buf)))[iSub]);
                    case 82: return GetString((*(dynamic_cast<HaccessFieldStr<82>*>(lFieldDtls.Buf)))[iSub]);
                    case 83: return GetString((*(dynamic_cast<HaccessFieldStr<83>*>(lFieldDtls.Buf)))[iSub]);
                    case 84: return GetString((*(dynamic_cast<HaccessFieldStr<84>*>(lFieldDtls.Buf)))[iSub]);
                    case 85: return GetString((*(dynamic_cast<HaccessFieldStr<85>*>(lFieldDtls.Buf)))[iSub]);
                    case 86: return GetString((*(dynamic_cast<HaccessFieldStr<86>*>(lFieldDtls.Buf)))[iSub]);
                    case 87: return GetString((*(dynamic_cast<HaccessFieldStr<87>*>(lFieldDtls.Buf)))[iSub]);
                    case 88: return GetString((*(dynamic_cast<HaccessFieldStr<88>*>(lFieldDtls.Buf)))[iSub]);
                    case 89: return GetString((*(dynamic_cast<HaccessFieldStr<89>*>(lFieldDtls.Buf)))[iSub]);

                    case 90: return GetString((*(dynamic_cast<HaccessFieldStr<90>*>(lFieldDtls.Buf)))[iSub]);
                    case 91: return GetString((*(dynamic_cast<HaccessFieldStr<91>*>(lFieldDtls.Buf)))[iSub]);
                    case 92: return GetString((*(dynamic_cast<HaccessFieldStr<92>*>(lFieldDtls.Buf)))[iSub]);
                    case 93: return GetString((*(dynamic_cast<HaccessFieldStr<93>*>(lFieldDtls.Buf)))[iSub]);
                    case 94: return GetString((*(dynamic_cast<HaccessFieldStr<94>*>(lFieldDtls.Buf)))[iSub]);
                    case 95: return GetString((*(dynamic_cast<HaccessFieldStr<95>*>(lFieldDtls.Buf)))[iSub]);
                    case 96: return GetString((*(dynamic_cast<HaccessFieldStr<96>*>(lFieldDtls.Buf)))[iSub]);
                    case 97: return GetString((*(dynamic_cast<HaccessFieldStr<97>*>(lFieldDtls.Buf)))[iSub]);
                    case 98: return GetString((*(dynamic_cast<HaccessFieldStr<98>*>(lFieldDtls.Buf)))[iSub]);
                    case 99: return GetString((*(dynamic_cast<HaccessFieldStr<99>*>(lFieldDtls.Buf)))[iSub]);
                    */

                    default: lRetVal = false; break;
                }

            default: lRetVal = false; break;
        }
        return lRetVal;
    }

    template <typename T1> std::string FloatVal(T1& iFloat) { DBG_MGR_LOG;
        std::stringstream lVal;
        lVal << std::fixed << std::setprecision(Precision) << iFloat;
        return lVal.str();
    }

protected:

private:

    DBG_MGR *pDm;
    DBG_MGR& GetDm() { return *pDm; }

    std::string App; // Context Application Name
    std::string Fam; // Context Family Name
    std::string Db;  // Database Name
    bool Init;       // True if MOM Db is initialized

    int SessionHdl;
    int DbHdl;

    boost::shared_ptr <T> pMom;

    int GetDbHdl() { DBG_MGR_LOG;
        HdbGetSessionHandle  (&SessionHdl);
        if (UtilScfBad(HdbGetDatabaseHandle (SessionHdl, Db.c_str(), &DbHdl, App.c_str(), Fam.c_str()))) {
            GetDm().Print (DML_E,DMF_Mom, "%s:Error Db(%s)", __FUNCTION__, Db.c_str());
        }
        return DbHdl;
    }

    int GetRecHdl(std::string iRec) { DBG_MGR_LOG;
        GetDm().Print (DML_7,DMF_Mom,"Getting Record Handled for(%s) in DB(%s_%s_%s)", iRec.c_str(), Db.c_str(), App.c_str(), Fam.c_str());
        int lRecHdl;
        if (UtilScfBad(HdbGetRecordHandle (GetDbHdl(), iRec.c_str(), &lRecHdl)))
            GetDm().Print (DML_E,DMF_Mom, "%s:Error Rec(%s)", __FUNCTION__, iRec.c_str());
        return lRecHdl;
    }

    int GetFieldHandle (std::string iField, std::string iRec) { DBG_MGR_LOG;
        int lFieldHdl;
        if (UtilScfBad(HdbGetFieldHandle (GetDbHdl(), iField.c_str(), &lFieldHdl)))
            GetDm().Print (DML_E,DMF_Mom, "%s:Error Field(%s), Rec(%s)", __FUNCTION__, iField.c_str(), iRec.c_str());
        return lFieldHdl;
    }

    int GetFieldDataType (std::string iField, std::string iRec) { DBG_MGR_LOG;
        int lDataType = 0;
        if (UtilScfBad(HdbGetDatatype(GetFieldHandle(iField,iRec), &lDataType))) // Get Field Data Type
            GetDm().Print (DML_E,DMF_Mom, "%s:Error in HdbGetDataType Field(%s), Rec(%s)", __FUNCTION__, iField.c_str(), iRec.c_str());
        return lDataType;
    }

    int GetFieldDataSize (std::string iField, std::string iRec) { DBG_MGR_LOG;
        int lDataSize = 0;
        if (UtilScfBad(HdbGetDatasize(GetFieldHandle(iField,iRec), &lDataSize))) // Get Field Data Type
            GetDm().Print (DML_E,DMF_Mom, "%s:Error in HdbGetDatasize Field(%s), Rec(%s)", __FUNCTION__, iField.c_str(), iRec.c_str());
        return lDataSize;
    }

    bool UtilScfBad (const SCF_STATUS iStatus) { // DBG_MGR_LOG; // Print respective text if iStatus is bad else return
        if (!ScfBad(iStatus)) return false;
        std::cout << UtilScfText(iStatus) << std::endl;
        //GetDm().Print(DML_E, DMF_U, "%s", UtilScfText(iStatus).c_str());
        return true;
    }

    std::string UtilScfText (const SCF_STATUS iStatus) { // DBG_MGR_LOG; // Return Text for the iStatus
        char lMsg[SCF_TEXT_MAXLEN] = {0};
        ScfGetText(iStatus, lMsg, sizeof(lMsg), SCF_TXT);
        return lMsg;
    }

};

template <typename T> std::string HdbId (T iId) {
    if (iId.IsEmptyAfterTrim()) return "";
    if (iId.TestNull()) return "";
    return iId.c_str_trim_trail_blanks();
}

template <typename T> std::string GetMrid(T iMrid) {
	char str[37] = {};
	sprintf(iMrid,
		"%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
		uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]
		);
	return iId.c_str_trim_trail_blanks();
}



#endif // _HACCESS_HDB_H_

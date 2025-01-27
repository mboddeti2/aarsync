#ifndef _COMMON_UTIL_H_
#define _COMMON_UTIL_H_
//------------------------------------------------------------------------------------------------//
// Description:
//   Common Utilities used in the program are coded here.
//
// Revision History:
//   2024-10-01: Original [Murali Boddeti (or)  mkb]
//------------------------------------------------------------------------------------------------//

// Include Files: system header file
#include <cctype>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp> // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp> // streaming operators etc.
#include <boost/algorithm/string.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <random>

// Include Files: Current program header file
#include "debug_manager.h"

#define EMS_INT_NULL -2139062144
#define EMS_INT_NULL_STR "-2139062144"
//#define EMS_FLOAT_NULL -1.1801e-38
#define EMS_FLOAT_NULL 1.18010406e-37
#define EMS_FLOAT_NULL_STR "-1.1801e-38"

class DBG_MGR;

class COMMON_UTIL {

public:
    COMMON_UTIL () {};
    COMMON_UTIL (DBG_MGR& iDm) {
        pDm  = &iDm;
    }
    ~COMMON_UTIL() {}

    template <typename T1, typename T2> std::string Id (T1 iPrefix, T2 iSuffix, int iFill) {
        std::stringstream lStr;
        lStr << iPrefix << std::setw(iFill) << std::setfill('0') << iSuffix;
        return lStr.str();
    }

    // Int or Float conversion to std::string
    template <typename T> std::string Id (T iValue) {
        std::ostringstream lStr;
        lStr << iValue;
        return lStr.str();
    }
    template <typename T> std::string HexId (T iValue) { // Returns hex decimal string Id of input value (int4 or int8)
        std::ostringstream lStr;
        lStr << std::hex << iValue;
        return lStr.str();
    }
    template <typename T> std::string TitleCase (T iStr) {
        std::string lStr(iStr);
        boost::algorithm::to_lower(lStr);
        lStr[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(lStr[0])));
        return lStr;
    }

    template <typename T> inline T Convert (std::string iStr) { // atoi or atof equivalent function that catches exception
        DBG_MGR& lDm = GetDm();
        std::istringstream lStr(iStr);
        T lObj;
        lStr >> std::ws >> lObj >> std::ws;
        if(!lStr.eof()) {
            lDm.Print (DML_W, DMF_U, "%s::Error in Converting String(%s)", __FUNCTION__, iStr.c_str());
            throw "COMMON_UTIL::Convert Error";
        }
        return lObj;
    }

    std::string Trim (std::string iStr) {
        boost::algorithm::trim(iStr);
        return iStr;
    }
    std::string TrimNew (std::string iStr) {
        boost::algorithm::trim(iStr);
        boost::algorithm::trim_left_if(iStr, [] (char iChar) {return iChar== '\n' || iChar == '\r';});
        return iStr;
    }

    // Composite Id
    template <typename T1>
    std::string CompId (T1 iKey1) {
        std::ostringstream lStr;
        lStr << iKey1;
        return Trim(lStr.str());
    }
    template <typename T1>
    std::string SpaceId (T1 iKey1) {
        std::string lKey1 = CompId(iKey1); 
        if (lKey1.empty()) return ""; 
        return lKey1 + " ";
    }
    template <typename T1,typename T2>
    std::string CompId (T1 iKey1,T2 iKey2) {
        return Trim(SpaceId(iKey1) + CompId(iKey2));
    }
    template <typename T1,typename T2,typename T3>
    std::string CompId (T1 iKey1,T2 iKey2,T3 iKey3) {
        return Trim(SpaceId(iKey1) + CompId(iKey2,iKey3));
    }
    template <typename T1,typename T2,typename T3,typename T4>
    std::string CompId (T1 iKey1,T2 iKey2,T3 iKey3,T4 iKey4) {
        return Trim(SpaceId(iKey1) + CompId(iKey2,iKey3,iKey4));
    }
    template <typename T1,typename T2,typename T3,typename T4,typename T5>
    std::string CompId (T1 iKey1,T2 iKey2,T3 iKey3,T4 iKey4,T5 iKey5) {
        return Trim(SpaceId(iKey1) + CompId(iKey2,iKey3,iKey4,iKey5));
    }
    template <typename T1,typename T2,typename T3,typename T4,typename T5, typename T6>
    std::string CompId (T1 iKey1,T2 iKey2,T3 iKey3,T4 iKey4,T5 iKey5,T6 iKey6) {
        return Trim(SpaceId(iKey1) + CompId(iKey2,iKey3,iKey4,iKey5,iKey6));
    }
    template <typename T1,typename T2,typename T3,typename T4,typename T5, typename T6, typename T7>
    std::string CompId (T1 iKey1,T2 iKey2,T3 iKey3,T4 iKey4,T5 iKey5,T6 iKey6, T7 iKey7) {
        return Trim(SpaceId(iKey1) + CompId(iKey2,iKey3,iKey4,iKey5,iKey6,iKey7));
    }
    template <typename T1,typename T2,typename T3,typename T4,typename T5, typename T6, typename T7, typename T8>
    std::string CompId (T1 iKey1,T2 iKey2,T3 iKey3,T4 iKey4,T5 iKey5,T6 iKey6, T7 iKey7, T8 iKey8) {
        return Trim(SpaceId(iKey1) + CompId(iKey2,iKey3,iKey4,iKey5,iKey6,iKey7,iKey8));
    }
    template <typename T1,typename T2,typename T3,typename T4,typename T5, typename T6, typename T7, typename T8, typename T9>
    std::string CompId (T1 iKey1,T2 iKey2,T3 iKey3,T4 iKey4,T5 iKey5,T6 iKey6, T7 iKey7, T8 iKey8, T9 iKey9) {
        return Trim(SpaceId(iKey1) + CompId(iKey2,iKey3,iKey4,iKey5,iKey6,iKey7,iKey8,iKey9));
    }

    std::string ReadFile (const std::string& iFileName) { DBG_MGR_LOG;
        std::ifstream lFile(iFileName);
        std::stringstream lFileBuf;
        lFileBuf << lFile.rdbuf();
        return lFileBuf.str();
    }

    std::string GetFileName (const std::string& iFileName) { // Routine to resolve Directory environment variables exists if any
        std::string lFileName = iFileName;
        size_t lpPos1 = 0;
        size_t lpPos2 = 0;
        while ( (lpPos1 = lFileName.find('{', lpPos1)) != std::string::npos ) {
            if ( (lpPos2 = lFileName.find('}', lpPos1)) != std::string::npos ) {
                // Between lpPos1 and lpPos2 we have Dir Environment Variable Name
                std::string lDirEnv = lFileName.substr(lpPos1+1,lpPos2-1);
                char *lDir = getenv(lDirEnv.c_str());
                if (lDir != NULL) {
                    lFileName.erase (lpPos1, lpPos2-lpPos1+1);
                    lFileName.insert (lpPos1, lDir);
                }
            }
            else {
                // No proper termination of the variable leave file name alone
            }
            lpPos1++; // To substitute any other environment variables
        }
        return lFileName;
    }

    bool CreateDir (std::string iDir) {
        DBG_MGR& lDm = GetDm();
        try {
            boost::filesystem::path lPath(GetFileName(iDir));
            if (boost::filesystem::is_directory(lPath)) {
                lDm.Print (DML_8, DMF_U, "Directory(%s) exists", iDir.c_str());
                return true;
            }
            else if (boost::filesystem::create_directories(lPath)) {
                lDm.Print (DML_I, DMF_U, "Directory(%s) successfully created", iDir.c_str());
                return true;
            }
            else {
                lDm.Print (DML_W, DMF_U, "Unable to create DirectoryPath(%s)", iDir.c_str());
                return false;
            }
        }
        catch (const std::exception& ex) {
            lDm.Print (DML_E, DMF_U, "Cannot create DirectoryPath(%s) error(%s)", iDir.c_str(), ex.what());
        }
        return false;
    }

    bool FileExists (std::string iFile) {
        std::ifstream lInFile(iFile);
        return lInFile.good();
    }

    double Round (double& iValue, int& iPrec) {
        return int((iValue + (0.5*std::pow(10.0,-iPrec))) * std::pow(10.0,iPrec)) / std::pow(10.0,iPrec);
    }

    std::string Uuid () {
        static boost::uuids::random_generator lGen; // here
        return boost::lexical_cast<std::string>(lGen());
    }
    std::string UuidUp () {
        return boost::to_upper_copy(Uuid());
    }
	std::string Uuid(const unsigned char iMrid[16]) {
		boost::uuids::uuid lUuid;
		std::copy(iMrid, iMrid + 16, lUuid.begin());
		return boost::uuids::to_string(lUuid);
	}

    template <typename T> std::string XmlTag (std::string iAtt, T& iVal, bool iDelta, bool iObjDelta) {
        std::stringstream lStr;
        lStr << "        <" << iAtt << ">" << iVal << "</" << iAtt << ">\n";
        return (lStr.str());
    }
    template <typename T> std::string XmlMapTag (std::string iAtt, T& iVal, bool iDelta, bool iObjDelta) {
        // When Delta is requested export only if Current object is delta or Referece obj is delta
        //if (!iDelta || iObjDelta || iRefObjDelta) return "";
        if (iDelta && !iObjDelta) return "";
        std::stringstream lStr;
        lStr << "        <" << iAtt << " rdf:resource=\"#" << iVal << "\"/>\n";
        return (lStr.str());
    }
    template <typename T> std::string XmlRefTag (std::string iAtt, T& iVal, bool iDelta, bool iReduced) {
        // When Delta is requested export only if Current object is delta or Referece obj is delta
        //if (!iDelta || iObjDelta || iRefObjDelta) return "";
        if (iVal.GetDeleted()) return ""; // This Ref object is deleted
        if (iReduced && iVal.GetReduced()) return "";
        if (iDelta && !iVal.GetDelta()) return "";
        std::stringstream lStr;
        lStr << "        <" << iAtt << " rdf:resource=\"#" << iVal.GetId() << "\"/>\n";
        return (lStr.str());
    }
    template <typename T> std::string XmlBoolTag (std::string iAtt, T& iVal, bool iDelta, bool iObjDelta) {
        std::stringstream lStr;
        if (iVal) lStr << "        <" << iAtt << ">" << "true" << "</" << iAtt << ">\n";
        else lStr << "        <" << iAtt << ">" << "false" << "</" << iAtt << ">\n";
        return (lStr.str());
    }
    std::string XmlEnumTag (std::string iSchema, std::string iClass, std::string iAtt, std::string iEnum, bool iDelta, bool iFldDelta) {
        if (iDelta && !iFldDelta) return ""; // Need to revisit this logic to make enum names space provide function
        if (iEnum.empty()) return ""; // Don't export Enums that are not initialized Default enum's into XML export
        std::string iRef = "http://iec.ch/TC57/2013/CIM-schema-cim16#";
        if (iSchema == "pse") iRef = "TODO#";
        else if (iSchema == "aarsync") iRef = "aarsync";
        return ("        <" + iSchema + ":" + iClass + "." + iAtt + " rdf:resource=\"" + iRef + iEnum + "\"/>\n");
    }

    template <typename T> std::string XmlEnumTag (T& iAttMap, std::string iSchema, std::string iClass, std::string iAtt, std::string iEnum, bool iDelta, bool iFldDelta) {
        if (iDelta && !iFldDelta) return ""; // Need to revisit this logic to make enum names space provide function
        if (iEnum.empty()) return ""; // Don't export Enums that are not initialized Default enum's into XML export
        std::string iRef = "http://iec.ch/TC57/2013/CIM-schema-cim16#";
        if (iSchema == "pse") iRef = "TODO#";
        else if (iSchema == "aarsync") iRef = "aarsync";
        return ("        <" + iSchema + ":" + iClass + "." + iAtt + " rdf:resource=\"" + iRef + iEnum + "\"/>\n");
    }
    template <typename T> std::string TypesXmlStrTag (std::string iAtt, T& iVal, bool iDelta, bool iObjDelta) {
        if (iDelta && !iVal.IsDelta()) return "";
        if (iVal.Get().empty()) return (""); // The idea is to redude xml file file size for empty strings
        return ("        <" + iAtt + ">" + iVal.GetXmlString() + "</" + iAtt + ">\n");
    }
    template <typename T> std::string TypesXmlTag (std::string iAtt, T& iVal, bool iDelta, bool iObjDelta) {
        if (!iVal.IsUsed()) return ""; // Dont export if not used during the run time of the process
        if (iDelta && !iVal.IsDelta()) return "";
        return ("        <" + iAtt + ">" + iVal.GetString() + "</" + iAtt + ">\n");
    }
    template <typename T> std::string TypesXmlRefTag (std::string iAtt, T& iVal, bool iDelta, bool iObjDelta) {
        if (iDelta && !iVal.IsDelta()) return "";
        return ("        <" + iAtt + " rdf:resource=\"#" + iVal.GetId() + "\"/>\n");
    }
    template <typename T> std::string TypesXmlBoolTag (std::string iAtt, T& iVal, bool iDelta, bool iObjDelta) {
        if (!iVal.IsUsed()) return ""; // Dont export if not used during the run time of the process
        if (iDelta && !iVal.IsDelta()) return "";
        std::stringstream lStr;
        lStr << "        <" << iAtt << ">";
        if (iVal.Get()) lStr << "true";
        else lStr << "false";
        lStr << "</" << iAtt << ">\n";
        return (lStr.str());
    }

    template <typename T1, typename T2, typename T3>
    void AddSortMap (T1& iHier, T2& iHierSort, T3& iHash, const std::string& iCompId, int& iSub) {
        if (iCompId.empty()) {
            std::cout << "AddSortMap::CompId cannot be empty... unfortunately debug and fix it" << std::endl;
            return;
        }
        auto it = iHash.find(iCompId);
        if (it != iHash.end()) iHierSort[it->second] = &iHier;
        else iHierSort[iSub--] = &iHier;
        if (iHier.Id.empty()) iHier.Id = iCompId;
    }

    bool IsNull (short int& iField) { // Used in excluding Habitat NULL values in the XML export
        if (iField != 0 && iField <= EMS_INT_NULL) return true;
        return false;
    }
    bool IsNull (int& iField) { // Used in excluding Habitat NULL values in the XML export
        if (iField != 0 && iField <= EMS_INT_NULL) return true;
        return false;
    }
    bool IsNull (float& iField) { // Used in excluding Habitat NULL values in the XML export
        if (iField != 0.0 && std::abs(iField) < EMS_FLOAT_NULL) return true;
        return false;
    }
    bool IsNull (double& iField) { // Used in excluding Habitat NULL values in the XML export
        if (iField != 0.0 && std::abs(iField) < EMS_FLOAT_NULL) return true;
        return false;
    }
    bool IsNull (std::string iField) { // Used in excluding Habitat NULL values in the XML export
        if (iField == EMS_INT_NULL_STR || iField == EMS_FLOAT_NULL_STR) return true;
        return false;
    }

    std::string ReplaceString (std::string iVal, std::string iSearchStr, std::string iRepStr) { // Without DBG_MGR
        //if (iVal.find(iSearchStr) == std::string::npos) return iVal;
        boost::xpressive::sregex lRegStr = boost::xpressive::sregex::compile(iSearchStr, boost::xpressive::icase);
        return boost::xpressive::regex_replace(iVal, lRegStr, iRepStr);
    }
    std::string AddXmlJunk (std::string iVal) { // Adds XML junk during export to XML - Without DBG_MGR
        iVal = ReplaceString (iVal, "&",  "&amp;");
        iVal = ReplaceString (iVal, "\"", "&quot;");
        iVal = ReplaceString (iVal, "\'", "&apos;");
        iVal = ReplaceString (iVal, "<",  "&lt;");
        iVal = ReplaceString (iVal, ">",  "&gt;");
        return (iVal);
    }
    std::string RemoveXmlJunk (std::string iVal) { // Removes XML junk during import to XML - Without DBG_MGR
        if ((iVal.find("&amp;" ) == std::string::npos) && (iVal.find("&lt;") == std::string::npos) &&
            (iVal.find("&apos;") == std::string::npos) && (iVal.find("&gt;") == std::string::npos) &&
            (iVal.find("&quot;") == std::string::npos)) return (iVal);
        iVal = ReplaceString (iVal, "&amp;",  "&");
        iVal = ReplaceString (iVal, "&quot;", "\"");
        iVal = ReplaceString (iVal, "&apos;", "\'");
        iVal = ReplaceString (iVal, "&lt;",   "<");
        iVal = ReplaceString (iVal, "&gt;",   ">");
        return (iVal);
    }
    std::string TrimLeftOf (std::string iVal, std::string iDelim) { DBG_MGR_LOG;
        std::size_t lFound = iVal.rfind(iDelim);
        if (lFound != std::string::npos) return iVal.substr(lFound+1);
        return iVal;
    }
    std::string TrimRightOf (std::string iVal, std::string iDelim) { DBG_MGR_LOG;
        std::size_t lFound = iVal.rfind(iDelim);
        if (lFound != std::string::npos) return iVal.substr(0,lFound);
        return iVal;
    }
    std::string TrimChar (std::string iVal, const char *iChar) { DBG_MGR_LOG;
        return TrimLeftChar(TrimRightChar(iVal,iChar),iChar);
    }
    std::string TrimLeftChar (std::string iVal, const char *iChar) { DBG_MGR_LOG;
        for (auto it = iVal.begin(); it != iVal.end(); it++) {
            if (*it == *iChar) it=iVal.erase(it);
            else break;
        }
        return iVal;
    }
    std::string TrimRightChar (std::string iVal, const char *iChar) { DBG_MGR_LOG;
        for (auto it = iVal.end(); it != iVal.begin(); it++) {
            if (*it == *iChar) it=iVal.erase(it);
            else break;
        }
        return iVal;
    }
    std::string TrimLeft (std::string iVal, unsigned int iSize) { DBG_MGR_LOG;
        if (iVal.size() > iSize) return iVal.substr(iVal.size()-iSize);
        return iVal;
    }
    std::string TrimRight (std::string iVal, unsigned int iSize) { DBG_MGR_LOG;
        if (iSize) return iVal.substr(0, iSize);
        return iVal;
    }
    std::string RemoveLastCharIfDot (std::string iVal) { DBG_MGR_LOG;
        if (*iVal.rbegin() == '.') return iVal.substr(0, iVal.size()-1);
        return iVal;
    }
    std::string TrimString (std::string iVal, std::string iBegin, std::string iEnd) { DBG_MGR_LOG; // Trim string between two delimiters
        std::string lVal = iVal;
        size_t lBegin = lVal.find(iBegin.c_str());
        if (lBegin == std::string::npos) return lVal;
        size_t lEnd = lVal.find(iEnd, lBegin);
        if (lEnd == std::string::npos) return lVal;
        lVal.erase(lBegin,lEnd-lBegin+1);
        return lVal;
    }
    std::string GetSubString (std::string iVal, std::string iBegin, std::string iEnd) { DBG_MGR_LOG;
        std::string lVal = iVal;
        size_t lBegin = lVal.find(iBegin.c_str());
        if (lBegin == std::string::npos) return lVal;
        size_t lEnd = lVal.find(iEnd, lBegin);
        if (lEnd == std::string::npos) return lVal;
        return lVal.substr(lBegin+1,lEnd-lBegin-1);
    }

protected:

private:
    DBG_MGR *pDm;
    DBG_MGR& GetDm () {
        if (!pDm) std::cout << "Error contact developer, COMMON_UTIL called without DBG_MGR, crashing..." << std::endl;
        return *pDm;
    }

};
#endif // _COMMON_UTIL_H_

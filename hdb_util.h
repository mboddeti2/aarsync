#ifndef _HDB_UTIL_H_
#define _HDB_UTIL_H_ // To make sure haccess_hdb.h is included only once
//------------------------------------------------------------------------------------------------//
// Description:
//   HDB_UTIL wrapper class on miscellaneous HABITAT API's
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

//#define EMP_VERSION_2_6

#define HABITAT_INT_NULL -2139062144

/*
//#define HABITAT_FLOAT_NULL -1.1801e-38
#define HABITAT_FLOAT_NULL 1.18010406e-37
*/

// Class Definitions
class DBG_MGR;

class HDB_UTIL {

public:
    HDB_UTIL () {};
    HDB_UTIL (DBG_MGR& iDm) {
        pDm  = &iDm;
    }
    ~HDB_UTIL() {}

    bool UtilScfBad (const SCF_STATUS iStatus) { // DBG_MGR_LOG; // Print respective text if iStatus is bad else return
        if (!ScfBad(iStatus)) return false;
        //std::cout << UtilScfText(iStatus) << std::endl;
        GetDm().Print(DML_E, DMF_U, "%s", UtilScfText(iStatus).c_str());
        return true;
    }
    std::string UtilScfText (const SCF_STATUS iStatus) { // DBG_MGR_LOG; // Return Text for the iStatus
        char lMsg[SCF_TEXT_MAXLEN] = {0};
        ScfGetText(iStatus, lMsg, sizeof(lMsg), SCF_TXT);
        return lMsg;
    }

    std::string GetApp () { // Get application name from task context
        char lApp[APPMGR_APPLICATION_MAXLEN+1];
        AppmgrGetApplicationName (lApp, sizeof(lApp)); // Get Application
        return lApp;
    }

    std::string GetFam () { // Get family name from task context
        char lFam[APPMGR_FAMILY_MAXLEN+1];
        AppmgrGetFamilyName (lFam, sizeof(lFam)); // Get Family name
        return lFam;
    }

    std::string GetAppFamily(std::string iDb, std::string iApp) { DBG_MGR_LOG;
        // return the app Family name based the installed clones either EMS or DTS else return based on the context
        if (CheckDb(iDb, iApp, "EMS")) return "EMS";
        else if (CheckDb(iDb, iApp, "DTS")) return "DTS";
        return GetFam ();
    }

    bool CheckDb (std::string iDb, std::string iApp, std::string iFam) { DBG_MGR_LOG; // Check if DB can be opened
        DBG_MGR& lDm = GetDm();
        int lSessionHdl;
        int lDbHdl;
        HdbGetSessionHandle (&lSessionHdl);
        HdbGetDatabaseHandle (lSessionHdl, iDb.c_str(), &lDbHdl, iApp.c_str(), iFam.c_str());
        SCF_STATUS lStatus = HdbOpen (lSessionHdl, iDb.c_str(), &lDbHdl, iApp.c_str(), iFam.c_str());         
        if (lStatus == HDB_W_DUOPEN) return true; // Open already not an error
        else if (!UtilScfBad(lStatus)) return true;
        lDm.Print (DML_E,DMF_Mom,"Error(%d) Opening(%s_%s_%s), Is Habitat running?", lStatus, iDb.c_str(), iApp.c_str(), iFam.c_str());
        return false;
    }

    TIMEDATE_HABTIME GetHabTime () {
        TIMEDATE_HABTIME lHabTime;
        TimedateGetHabtime (&lHabTime);
        return lHabTime;
    }
    TIMEDATE_HABTIME GetHabTime (std::string iTimeStr) {
        return GetHabTime(iTimeStr, "%Y/%m/%d %H:%M:%S");
    }
    TIMEDATE_HABTIME GetHabTime (std::string iTimeStr, std::string iFmt) {
        TIMEDATE_HABTIME lHabTime;
        HdbSetNull(lHabTime);
        if (iTimeStr.size() > 0) UtilScfBad (TimedateParseHabtime (&lHabTime, 0, iTimeStr.c_str(), iFmt.c_str()));
        return lHabTime;
    }
    TIMEDATE_HABTIME GetHabTime (std::string iTimeStr, std::string iFmt, std::string iClock) {
        if (iClock.empty()) return GetHabTime (iTimeStr, iFmt); // Current external clock by default
        TIMEDATE_CLOCK_HANDLE lCurClockHdl, lUtcClockHdl; // Current and UTC clock handles
        UtilScfBad (TimedateGetClockHandle(iClock.c_str(), &lUtcClockHdl));
        UtilScfBad (TimedateSetDefaultClock(lUtcClockHdl, &lCurClockHdl));
        TIMEDATE_HABTIME lHabTime = GetHabTime (iTimeStr);
        UtilScfBad (TimedateSetDefaultClock(lCurClockHdl, &lUtcClockHdl));
        return lHabTime;
    }
    TIMEDATE_HABTIME GetTodTime () {
        TIMEDATE_HABDATE lHabDate;
        TIMEDATE_HABTIME lHabTime = GetHabTime();
        lHabDate = HabtimeToHabdate(lHabTime);
        lHabTime = HabdateToHabtime(lHabDate);
        return lHabTime;
    }
    TIMEDATE_HABTIME GetTodTimeDaysAgo (const int iDaysAgo) {
        TIMEDATE_HABDATE lHabDate;
        TIMEDATE_HABTIME lHabTime = GetHabTime();
        lHabDate = HabtimeToHabdate(lHabTime);
        lHabTime = HabdateToHabtime(lHabDate - iDaysAgo);
        return lHabTime;
    }
    TIMEDATE_HABDATE HabtimeToHabdate(const TIMEDATE_HABTIME Habtime_I) {
        TIMEDATE_HABDATE lHabDate;
        TIMEDATE_GFORM_HANDLE gForm;
        gForm = TimedateAllocGform();
        UtilScfBad (TimedateCvtHabtimeToGform(Habtime_I, gForm));
        UtilScfBad (TimedateCvtGformToHabdate(gForm, &lHabDate));
        TimedateDeallocGform(gForm);
        return lHabDate;
    }
    TIMEDATE_HABTIME HabdateToHabtime(const TIMEDATE_HABDATE Habdate_I) {
        TIMEDATE_HABTIME lHabTime;
        TIMEDATE_GFORM_HANDLE gForm;
        gForm = TimedateAllocGform();
        TIMEDATE_HABDATE habDate = Habdate_I;
        TIMEDATE_CLOCK_ATTRSET ClockAttrset;
        //Obtain the current time offset value
        UtilScfBad (TimedateGetClockAttributes(TIMEDATE_DEFAULT_CLOCK_HANDLE, &ClockAttrset));
        if( (habDate < 2) && (ClockAttrset.utc_offset > 0) )
            habDate = 2;
        UtilScfBad (TimedateCvtHabdateToGform(habDate, gForm));
        UtilScfBad (TimedateCvtGformToHabtime(gForm, &lHabTime));
        TimedateDeallocGform(gForm);
        return lHabTime;
    }
    int GetHabHour(const TIMEDATE_HABTIME iHabtime) {
        int iHour;
        int iMin;
        TIMEDATE_GFORM_HANDLE gForm;
        TIMEDATE_GFORM_ELEMENTS gFormSet;
        gForm = TimedateAllocGform();
        UtilScfBad (TimedateCvtHabtimeToGform (iHabtime, gForm));
        UtilScfBad (TimedateDecomposeTime(gForm, &gFormSet));
        iHour = gFormSet.hour ;
        iMin  = gFormSet.minute ;
        UtilScfBad (TimedateDeallocGform(gForm));
        return iHour;
    }

    // Needs to work on Date field, time works for date does not TODO
    template <typename T> std::string GetHdbDateStr (T iHabDate) {
        return GetHdbDateStr(iHabDate,"%d-%b-%Y");
    }
    template <typename T> std::string GetHdbDateStr (T iHabTime, std::string iFmtStr) { DBG_MGR_LOG;
        // Get Custom format time string supported by Timedate API
        DBG_MGR& lDm = GetDm();
        char lTimeStr[30]={0};
        if (iHabTime <= 0) return lTimeStr; // Validation Check
        if (UtilScfBad(TimedateFormatHabtime(lTimeStr, sizeof(lTimeStr), iFmtStr.c_str(), iHabTime))) {
            lDm.Print (DML_E,DMF_Mom,"%s::Error HabTime(%d) Format String(%s)", __FUNCTION__, iHabTime, iFmtStr.c_str());
        }
        return lTimeStr;
    }
    template <typename T> std::string GetHdbTimeStr (T iHabTime) {
        return GetHdbTimeStr(iHabTime,"%d-%b-%Y %H:%M:%S"); // Is it External default time format?
    }
    template <typename T> std::string GetHdbTimeStr (T iHabTime, std::string iFmtStr) { DBG_MGR_LOG;
        // Get Custom format time string supported by Timedate API
        DBG_MGR& lDm = GetDm();
        char lTimeStr[30]={0};
        if (iHabTime <= 0) return lTimeStr; // Validation Check
        if (UtilScfBad(TimedateFormatHabtime(lTimeStr, sizeof(lTimeStr), iFmtStr.c_str(), iHabTime))) {
            lDm.Print (DML_E,DMF_Mom,"%s::Error HabTime(%d) Format String(%s)", __FUNCTION__, iHabTime, iFmtStr.c_str());
        }
        return lTimeStr;
    }

protected:

private:
    DBG_MGR *pDm; // Reference to global Debug Manager object
    DBG_MGR& GetDm () {
        return *this->pDm;
    }

};

/*
template <typename T> std::string HdbId (T iId) {
    if (iId.IsEmptyAfterTrim()) return "";
    if (iId.TestNull()) return "";
    return iId.c_str_trim_trail_blanks();
}
*/

#endif // _HDB_UTIL_H_

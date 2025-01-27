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
		static char lApp[APPMGR_APPLICATION_MAXLEN + 1] = {0};
		if (lApp[0] != '\0') return lApp;
		AppmgrGetApplicationName(lApp, sizeof(lApp)); // Get Application
        return lApp;
    }

    std::string GetFam () { // Get family name from task context
        static char lFam[APPMGR_FAMILY_MAXLEN+1];
		if (lFam[0] != '\0') return lFam;
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
        lDm.Print (DML_W,DMF_Mom,"Error(%d) Opening(%s_%s_%s), Is Habitat running?", lStatus, iDb.c_str(), iApp.c_str(), iFam.c_str());
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

	std::string GetGMTTime (std::string iFmtStr) { DBG_MGR_LOG;
        // Get the GMT time string in the format provided as input by manupulating the default clock
	    
	    TIMEDATE_CLOCK_HANDLE lGMTHdl;
		if (UtilScfBad(TimedateGetClockHandle("UTC", &lGMTHdl))) return "";

		TIMEDATE_CLOCK_HANDLE lPrevHdl;
		if (UtilScfBad(TimedateSetDefaultClock(lGMTHdl, &lPrevHdl))) return "";

		std::string lGMTTime = GetHdbTimeStr(GetHabTime(), iFmtStr);

		if (UtilScfBad(TimedateSetDefaultClock(lPrevHdl, &lPrevHdl))) return ""; // Set the previous clock again so other operations could go as normal
		
		return lGMTTime;
	}

protected:

private:
    DBG_MGR *pDm; // Reference to global Debug Manager object
    DBG_MGR& GetDm () {
        return *this->pDm;
    }

};

#endif // _HDB_UTIL_H_

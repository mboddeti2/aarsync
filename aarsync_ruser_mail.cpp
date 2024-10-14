//------------------------------------------------------------------------------------------------//
// Description:
//   Contains AARSYNC RUSER Mail command routines
//
// Revision History:
//   2024-10-01: Original [Murali Boddeti (or)  mkb]
//------------------------------------------------------------------------------------------------//

// Include Files: system header file

// Include Files: Habitat Headers
#include "ppm.h"
#include "cfgman.h"
#include <permit.h>
#include "ruser.h"
#include "scf_ruser.h"

// Include Files: Current Program header file
#include "debug_manager.h"
#include "hdb_util.h"
#include "haccess_hdb.h"
#include "haccess_aarsync_aarsdb.h"
#include "aarsync_main.h"

void AARSYNC_MAIN::RuserMailCallbk (const char* iCommand) { // Static function
    auto& lMain = AARSYNC_MAIN::GetMain();
    auto& lDm = lMain.GetDm();

    auto& lAarsdbHdb = lMain.GetAarsdbHdb ();
    auto& lAarsdb = lMain.GetAarsdb ();

    COMMON_UTIL lUtil(lDm);
    HDB_UTIL lHdbUtil(lDm);

    RUSER_CONTEXT lContext = {0};
    RUSER_RECORD  lRecord = {0};
    RUSER_CLIENT_INFO lClientInfo = {0};

    if (lHdbUtil.UtilScfBad(RuserEnumGetSelectRecord(0, &lContext, &lRecord))) { // Find out where command is coming from
        lDm.Print(DML_E, DMF_M, "%s::Error in RuserEnumGetSelectRecord, cannot get mail command context", __FUNCTION__);
        return;
    }

    if (lHdbUtil.UtilScfBad(RuserGetClientInfo(&lClientInfo))) { // Get client details
        lDm.Print(DML_E, DMF_M, "%s::Error in RuserGetClientInfo, cannot get mail command client details", __FUNCTION__);
        return;
    }

    std::string lReply = "";

    std::vector <std::string> lCols;
    boost::split(lCols, iCommand, boost::is_any_of("_"));
    std::string lCommand = lUtil.Trim(lCols[0]);

    lDm.Print(DML_I, DMF_M, "Received Mail Command(%s) from Console(%s), Viewport(%s)", lCommand.c_str(), lContext.console, lContext.viewport);

    bool lWrite = false;

    lMain.CurTime = lHdbUtil.GetHabTime();
    unsigned int lSub = lRecord.subscript;
    std::string lRecordName = lRecord.recordname;

    int lDbgLvl = DML_I;
    lReply = std::string(iCommand) + " - Completed";
    if (boost::iequals(lCommand, "IMPORT")) {
        std::string lFileName = HdbId(lAarsdb.DNVFILE_ITEMS[1]);
        if (lCols.size() > 1) lFileName = lCols[1];
        if (!lMain.Import(lFileName, true)) lReply = "Import Failed";
    }

    else {
        lDbgLvl = DML_E;
        lReply = "Unknown Mail Command - " + lCommand;
    }

    if (lWrite) {
        lAarsdbHdb.Write(); // Write now as some thing changed requiring us to write
    }

    if (!lReply.empty()) {
        lDm.Print(lDbgLvl, DMF_M, "%s::Reply Message (%s) to console(%s) viewport(%s)", __FUNCTION__, lReply.c_str(), lContext.console, lContext.viewport);
        lHdbUtil.UtilScfBad(RuserSendMessage(lContext.console, lContext.viewport, lReply.c_str(), true, true));
    }
}

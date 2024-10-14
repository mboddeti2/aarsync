//------------------------------------------------------------------------------------------------//
// Description:
//   AARSync - RUSER entry routines holder
//
// Design:
//   Handles RUSER Entry & Mail commands
//
// Revision History:
//   2024-10-01: Original [Murali Boddeti (or)  mkb]
//------------------------------------------------------------------------------------------------//

// Include Files: system header file

// Include Files: Habitat Headers
#include "ppm.h"
#include "cfgman.h"
#include "ruser.h"
#include "scf_ruser.h"

// Include Files: Current Program header file
#include "debug_manager.h"
#include "hdb_util.h"
#include "haccess_hdb.h"
#include "haccess_aarsync_aarsdb.h"
#include "aarsync_main.h"

void AARSYNC_MAIN::RuserEntryCallbk(const RUSER_CONTEXT* iContext, const char* iCommand) {
    auto& lMain = AARSYNC_MAIN::GetMain();
    auto& lDm = lMain.GetDm();
    HDB_UTIL lHdbUtil(lDm);

    if (lHdbUtil.UtilScfBad(RuserEdecProcessEntries(RuserEntryPredicateCallbk))) {
        lDm.Print(DML_E, DMF_M, "%s::Error in RuserEdecProcessEntries, cannot process entry commands", __FUNCTION__);
    }
}

SCF_STATUS AARSYNC_MAIN::RuserEntryPredicateCallbk (const RUSER_CONTEXT* iContext, const RUSER_FIELD* iField, const RUSER_VALUE* iValue) {
    auto& lMain = AARSYNC_MAIN::GetMain();
    auto& lDm = lMain.GetDm();
    HDB_UTIL lHdbUtil(lDm);

    lMain.CurTime = lHdbUtil.GetHabTime();

    auto& lAarsdbHdb = lMain.GetAarsdbHdb ();
    auto& lAarsdb = lMain.GetAarsync ();

    RUSER_CLIENT_INFO lClientInfo = {0};
    if (lHdbUtil.UtilScfBad(RuserGetClientInfo(&lClientInfo))) { // Get client details
        lDm.Print(DML_E, DMF_M, "%s::Error in RuserGetClientInfo, cannot get mail command client details", __FUNCTION__);
    }

    std::string lDb = iField->db;
    if (!boost::iequals(lDb, "AARSYNC")) {
        std::string lMsg = "Unknown Db(" + lDb + ") EDEC Entry rejected";
        lHdbUtil.UtilScfBad(RuserSendMessage(iContext->console, iContext->viewport, lMsg.c_str(), true, true));
        return RUSER_E_REJECT;
    }

    std::string lRec = iField->rec1.recordname;
    std::string lField = iField->fieldname;
    int lSub1 = iField->rec1.subscript;
    int lSub2 = iField->rec2.subscript;

    if (boost::iequals(lRec, "ITEMS")) {
        if      (boost::iequals(lField, "GENLSTSH" )) lAarsdb.GENLSTSH_ITEMS[lSub1] = *(float*) iValue->value;

        else if (boost::iequals(lField, "DNVFILE"  )) lAarsdb.CTGFILE_ITEMS[lSub1].CopyFromSz((char*) iValue->value);

        else if (boost::iequals(lField, "DBGFUN")) {
            lAarsdb.DBGFUN_ITEMS[lSub1] = *(int*) iValue->value;
            lDm.SetFunction(lAarsdb.DBGFUN_ITEMS[lSub1]);
        }
        else if (boost::iequals(lField, "DBGLVL")) {
            lAarsdb.DBGLVL_ITEMS[lSub1] = *(int*) iValue->value;
            lDm.SetLevel(lAarsdb.DBGLVL_ITEMS[lSub1]);
        }
        else if (boost::iequals(lField, "MAXMSG")) {
            lAarsdb.MAXMSG_ITEMS[lSub1] = *(int*) iValue->value;
            lDm.SetMaxMsg(lAarsdb.MAXMSG_ITEMS[lSub1]);
        }

        else {
            std::string lMsg = "Unknown Field(" + lField + "_" + lRec + ") EDEC Entry rejected";
            lHdbUtil.UtilScfBad(RuserSendMessage(iContext->console, iContext->viewport, lMsg.c_str(), true, true));
            return RUSER_E_REJECT;
        }
    }

    else {
        std::string lMsg = "Unknown Field(" + lField + "_" + lRec + ") EDEC Entry rejected";
        lHdbUtil.UtilScfBad(RuserSendMessage(iContext->console, iContext->viewport, lMsg.c_str(), true, true));
        return RUSER_E_REJECT;
    }

    lAarsdb.DoGroup("AARSPR",Haccess::write);

    return RUSER_S_ACCEPT;
}

template <typename T> void AARSYNC_MAIN::AssignMask (T& iField, int iSub, void *iValue) { DBG_MGR_LOG;
    if (*(bool*) iValue) iField.Set(iSub);
    else iField.Reset(iSub);
}

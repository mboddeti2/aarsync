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
    auto& lHdbUtil = HDB_UTIL(lDm);

    if (lHdbUtil.UtilScfBad(RuserEdecProcessEntries(RuserEntryPredicateCallbk))) {
        lDm.Print(DML_E, DMF_M, "%s::Error in RuserEdecProcessEntries, cannot process entry commands", __FUNCTION__);
    }
}

SCF_STATUS AARSYNC_MAIN::RuserEntryPredicateCallbk (const RUSER_CONTEXT* iContext, const RUSER_FIELD* iField, const RUSER_VALUE* iValue) {
    auto& lMain = AARSYNC_MAIN::GetMain();
    auto& lDm = lMain.GetDm();
    auto& lHdbUtil = HDB_UTIL(lDm);

    lMain.CurTime = lHdbUtil.GetHabTime();

    auto& lAarsdbHdb = lMain.GetAarsdbHdb ();
    auto& lAarsdb = lMain.GetAarsdb ();

    RUSER_CLIENT_INFO lClientInfo = {0};
    if (lHdbUtil.UtilScfBad(RuserGetClientInfo(&lClientInfo))) { // Get client details
        lDm.Print(DML_E, DMF_M, "%s::Error in RuserGetClientInfo, cannot get mail command client details", __FUNCTION__);
    }

    std::string lDb = iField->db;
    if (!boost::iequals(lDb, "AARSDB")) {
        std::string lMsg = "Unknown Db(" + lDb + ") EDEC Entry rejected";
        lHdbUtil.UtilScfBad(RuserSendMessage(iContext->console, iContext->viewport, lMsg.c_str(), true, true));
        return RUSER_E_REJECT;
    }

    std::string lRec = iField->rec1.recordname;
    std::string lField = iField->fieldname;
    int lSub1 = iField->rec1.subscript;
    int lSub2 = iField->rec2.subscript;

    if (boost::iequals(lRec, "ITEMS")) {

		if      (boost::iequals(lField, "DNVFILE")) lAarsdb.DNVFILE_ITEMS[lSub1].CopyFromSz((char*) iValue->value);
		else if (boost::iequals(lField,  "NETAPP")) lAarsdb.NETAPP_ITEMS[lSub1].CopyFromSz((char*)iValue->value);
		else if (boost::iequals(lField,  "NETFAM")) lAarsdb.NETFAM_ITEMS[lSub1].CopyFromSz((char*)iValue->value);
		else if (boost::iequals(lField,   "SCAPP")) lAarsdb.SCAPP_ITEMS[lSub1].CopyFromSz((char*)iValue->value);
		else if (boost::iequals(lField,   "SCFAM")) lAarsdb.SCFAM_ITEMS[lSub1].CopyFromSz((char*)iValue->value);

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
		else if (boost::iequals(lField, "STDOUT")) {
			lAarsdb.STDOUT_ITEMS.Test(lSub1) ? lAarsdb.STDOUT_ITEMS.Reset(lSub1) : lAarsdb.STDOUT_ITEMS.Set(lSub1); // Toggle
			lDm.SetStdout(lAarsdb.STDOUT_ITEMS.Test(lSub1));
		}

		else if (boost::iequals(lField, "FLOLDTH")) lAarsdb.FLOLDTH_ITEMS[lSub1] = *(int*)iValue->value;
		else if (boost::iequals(lField, "SPIURTE")) lAarsdb.SPIURTE_ITEMS[lSub1] = *(int*)iValue->value;
		else if (boost::iequals(lField, "PERTM")) lAarsdb.PERTM_ITEMS[lSub1] = *(int*)iValue->value;
		else if (boost::iequals(lField, "OFFTM")) lAarsdb.OFFTM_ITEMS[lSub1] = *(int*)iValue->value;

        else {
            std::string lMsg = "Unknown Field(" + lField + "_" + lRec + ") EDEC Entry rejected";
            lHdbUtil.UtilScfBad(RuserSendMessage(iContext->console, iContext->viewport, lMsg.c_str(), true, true));
            return RUSER_E_REJECT;
        }
    }

	else if (boost::iequals(lRec, "AARELE")) {
		if (boost::iequals(lField, "OPER")) {
			lAarsdb.USER_AARELE[lSub1].CopyFromSz(lClientInfo.clientname); // Note the user modified the rating
			lAarsdb.OPER_AARELE[lSub1] = *(float*)iValue->value;
		}
		else if (boost::iequals(lField, "NORM")) {
			lAarsdb.USER_AARELE[lSub1].CopyFromSz(lClientInfo.clientname); // Note the user modified the rating
			lAarsdb.NORM_AARELE[lSub1] = *(float*)iValue->value;
		}
		else if (boost::iequals(lField, "EMER")) {
			lAarsdb.USER_AARELE[lSub1].CopyFromSz(lClientInfo.clientname); // Note the user modified the rating
			lAarsdb.EMER_AARELE[lSub1] = *(float*)iValue->value;
		}
		
		else if (boost::iequals(lField, "MAN")) {
			if (lAarsdb.MAN_AARELE.Test(lSub1)) { // When asked to reset copy file ratings to actual
				lAarsdb.MAN_AARELE.Reset(lSub1);
				lAarsdb.OPER_AARELE[lSub1] = lAarsdb.FILEOPER_AARELE[lSub1];
				lAarsdb.NORM_AARELE[lSub1] = lAarsdb.FILENORM_AARELE[lSub1];
				lAarsdb.EMER_AARELE[lSub1] = lAarsdb.FILEEMER_AARELE[lSub1];
				lAarsdb.USER_AARELE[lSub1].CopyFromSz(""); // Since file ratings are copied from file mark user as empty
			}
			else {
				lAarsdb.MAN_AARELE.Set(lSub1); // Toggle
			}
		}
	}

    else {
        std::string lMsg = "Unknown Field(" + lField + "_" + lRec + ") EDEC Entry rejected";
        lHdbUtil.UtilScfBad(RuserSendMessage(iContext->console, iContext->viewport, lMsg.c_str(), true, true));
        return RUSER_E_REJECT;
    }

	// lAarsdbHdb.WriteField(lField, lRec, ) // Use WriteField instead
	lAarsdbHdb.Write();

    return RUSER_S_ACCEPT;
}

template <typename T> void AARSYNC_MAIN::AssignMask (T& iField, int iSub, void *iValue) { DBG_MGR_LOG;
    if (*(bool*) iValue) iField.Set(iSub);
    else iField.Reset(iSub);
}
//------------------------------------------------------------------------------------------------//
// Description:
//   Contains various AARSYNC SCAPI Routines
//
// Revision History:
//   2024-10-01: Original [Murali Boddeti (or)  mkb]
//------------------------------------------------------------------------------------------------//

// Include Files: system header file

// Include Files: Habitat Headers
#include "ppm.h"
#include "scapi.h"
#include "scf_scapi.h"

// Include Files: Current Program header file
#include "debug_manager.h"
#include "hdb_util.h"
#include "haccess_hdb.h"

#include "haccess_aarsync_aarsdb.h"
#include "aarsync_main.h"


void AARSYNC_MAIN::ScapiInitialize () {	DBG_MGR_LOG;
    auto& lDm = GetDm();

    ScapiVerifyFlag = false;
    
	if (!ScapiInitFlag) { // To make sure we do ScapiInit once for life time of application
		ScapiInit (Task.c_str(), SCAPI_VERSION); // Initialize SCAPI Interface
		lDm.Print (DML_I, DMF_S, "ScapiInit called");
		ScapiInitFlag = true;
	}

	if (!ScapiEnableFlag) {
		ScapiEnable (); // Enable SCAPI interface
		lDm.Print(DML_I, DMF_S, "ScapiEnable called");
		ScapiEnableFlag = true;
	}

    VerifyType = AARSYNC_SCAPI_REVERIFY;
	ScapiConnect ("", ScapiCommCallBk, NULL, ScapiVerifyCallBk, (void *)&VerifyType, NULL); // Connect SCAPI interface
    lDm.Print (DML_I, DMF_S,"ScapiConnect called");

	ScapiRegister (); // Register required analogs
}

void AARSYNC_MAIN::ScapiCommCallBk(SCF_STATUS iStatus, void *) {
    auto& lMain = AARSYNC_MAIN::GetMain();
	auto& lDm = lMain.GetDm();
    auto& lHdbUtil = HDB_UTIL(lDm);
	lDm.Print(DML_L, DMF_U, "Entering - %s", __FUNCTION__);

	lMain.ScapiVerifyFlag = false; // To ensure re-verification is done
    if (!lHdbUtil.UtilScfBad(iStatus)) lDm.Print(DML_I, DMF_S, lHdbUtil.UtilScfText(iStatus));
	
	lDm.Print(DML_L, DMF_S, "Returning - %s", __FUNCTION__);
}

void AARSYNC_MAIN::ScapiRegister() {	DBG_MGR_LOG;
    auto& lDm = GetDm();
    auto& lHdbUtil = HDB_UTIL(lDm);

    auto& lAarsdb = GetAarsdb();

    SCAPI_REG_REC RegRec;
	RegRec.update_rate = lAarsdb.SPIURTE_ITEMS[1];
	RegRec.access_modes = SCAPI_ACCESS_WRITE_DATA | SCAPI_ACCESS_READ_DATA; // Sets both WRITE and READ bits so we can read and/or write

	int lTotal = 0;
	for (int lAarele = 1; lAarele <= lAarsdb.AARELE.GetLv(); lAarele++) {
		std::string lCompId = HdbId(lAarsdb.COMPID_AARELE[lAarele]);

		if (!lAarsdb.SMAP_AARELE.Test(lAarele)) {
			lDm.Print(DML_2, DMF_S, "Cannot ScapiRegisterObject for AARELE(%d)(%s), Not mapped with SCADA", lAarele, lCompId.c_str());
			continue;
		}

		int lAarout = lAarsdb.I__AAROUT_AARELE[lAarele];
		std::string lAarOutCompId = HdbId(lAarsdb.COMPID_AAROUT[lAarout]);
		
		lDm.Print(DML_5, DMF_S, "Registering AARELE(%d)(%s) AAROUT(%d)(%s) with SCAPI, Handle(%d)", lAarele, lCompId.c_str(), lAarout, lAarOutCompId.c_str(), lAarsdb.HANDLE_AAROUT[lAarout]);
		ScapiRegisterObject(lAarsdb.KEY_AAROUT[lAarout], SCAPI_OBJTYP_ANALOG, lAarout, &RegRec, (SCAPI_OBJECT_HANDLE*) &lAarsdb.HANDLE_AAROUT[lAarout]);		
		lDm.Print(DML_1, DMF_S, "Registered AARELE(%d)(%s) AAROUT(%d)(%s) with SCAPI, Handle(%d)", lAarele, lCompId.c_str(), lAarout, lAarOutCompId.c_str(), lAarsdb.HANDLE_AAROUT[lAarout]);

		lTotal++;
	}

	lDm.Print(DML_I, DMF_S, "Registed(%d) AAROUT objects", lTotal);
	if (lTotal > 0) GetAarsdbHdb().Write(); // To preseve the Handles

	VerifyType = AARSYNC_SCAPI_VERIFY;
	if (!lHdbUtil.UtilScfBad(ScapiRegisterVerify(ScapiVerifyCallBk, (void*)&VerifyType))) { // Verify registered objects
        lDm.Print(DML_I, DMF_S, "ScapiRegisterVerify Called");
    }
}

void AARSYNC_MAIN::ScapiVerifyCallBk(SCF_STATUS iStatus, void *iVerifyType) {
    auto& lMain = AARSYNC_MAIN::GetMain();
    auto& lDm = lMain.GetDm();
	lDm.Print(DML_L, DMF_U, "Entering - %s", __FUNCTION__);
	auto& lHdbUtil = HDB_UTIL(lDm);
	auto& lAarsdb = lMain.GetAarsdb();

    int lVerifyType = *(int*)iVerifyType;

    if (lVerifyType == AARSYNC_SCAPI_VERIFY) lDm.Print (DML_I, DMF_S,"SCAPI Verification in Progress");
	else if (lVerifyType == AARSYNC_SCAPI_REVERIFY) lDm.Print(DML_I, DMF_S, "SCAPI Re-verification in Progress");
    else lDm.Print(DML_I, DMF_S, "%s: Unknown VerifyType(%d)", __FUNCTION__, lVerifyType);

    if (iStatus == SCAPI_S_NORMAL) lDm.Print(DML_I, DMF_S, "SCAPI Verification successful");
    else if (iStatus == SCAPI_W_EMPTYVERQ) lDm.Print(DML_W, DMF_S, "%s:No Objects registered to verify");
    else if (iStatus == SCAPI_E_NOVERIFY) {
        SCAPI_USER_HANDLE lObj = 0;   // Regitered object index
        SCF_STATUS        lStatus = 0;
        while (ScapiRegisterGetError(&lObj, &lStatus) != SCAPI_S_DONE) {
            if (lHdbUtil.UtilScfBad(lStatus)) {
                lDm.Print(DML_E, DMF_S, "Error verifying AAROUT(%d)(%s)", lObj, HdbId(lAarsdb.COMPID_AAROUT[lObj]).c_str());
                ScapiRegisterRemove(lAarsdb.HANDLE_AAROUT[lObj]);
                lAarsdb.HANDLE_AAROUT[lObj] = 0;
            }
        }
    }

	lMain.ScapiVerifyFlag = true; // SCAPI Interface is ready

	lDm.Print(DML_L, DMF_S, "Returning - %s", __FUNCTION__);
}

void AARSYNC_MAIN::WriteLimits() { DBG_MGR_LOG;
	auto& lDm = GetDm();
	auto& lHdbUtil = HDB_UTIL(lDm);
	auto& lAarsdb = GetAarsdb();
	auto& lUtil = COMMON_UTIL(lDm);

	if (!ScapiVerifyFlag) {
        lDm.Print (DML_W, DMF_S, "%s::SCAPI Verify not complete, cannot write limits to scada", __FUNCTION__);
        return;
    }

	bool lWriteSend = false;
	static std::map <int, std::string> sEleRtg; // Ratings has to remember the values or last write

	for (int lAarele = 1; lAarele <= lAarsdb.AARELE.GetLv(); lAarele++) {
		
		std::string lCompId = HdbId(lAarsdb.COMPID_AARELE[lAarele]);

		if (!lAarsdb.DNV_AARELE.Test(lAarele)) {
			lDm.Print(DML_2, DMF_S, "Cannot write limits for AARELE(%d)(%s), Not mapped with DNV", lAarele, lCompId.c_str());
			continue;
		}

		if (!lAarsdb.SMAP_AARELE.Test(lAarele)) {
			lDm.Print(DML_2, DMF_S, "Cannot write limits for AARELE(%d)(%s), Not mapped with SCADA", lAarele, lCompId.c_str());
			continue;
		}

		int lAarout = lAarsdb.I__AAROUT_AARELE[lAarele];
		std::string lAaroutCompId = HdbId(lAarsdb.COMPID_AAROUT[lAarout]);

		int lHandle = lAarsdb.HANDLE_AAROUT[lAarout];
		if (lHandle <= 0) {
			lDm.Print(DML_I, DMF_S, "Cannot write limits for AARELE(%d)(%s) AAROUT(%d)(%s), SCAPI Handle 0", lAarele, lCompId.c_str(), lAarout, lAaroutCompId.c_str());
			continue;
		}

		lDm.Print(DML_1, DMF_S, "Writing Limits for AARELE(%d)(%s) AAROUT(%d)(%s)", lAarele, lCompId.c_str(), lAarout, lAaroutCompId.c_str());

		float lNewOper = lAarsdb.OPER_AARELE[lAarele];
		float lNewNorm = lAarsdb.NORM_AARELE[lAarele];
		float lNewEmer = lAarsdb.EMER_AARELE[lAarele];

		std::string lRtgCompId = lUtil.CompId(lNewOper, lNewNorm, lNewEmer);
		auto& lScapiMessage = gScapiErr[lAarele];
		if (!lScapiMessage.empty() && sEleRtg[lAarele] == lRtgCompId) {
			lDm.Print(DML_1, DMF_S, "Writing Limits ignored for AARELE(%d)(%s) due to SCAPI Error", lAarele, lCompId.c_str());
			lAarsdb.SPIMSG_AARELE[lAarele].CopyFromSz(lScapiMessage.c_str());
			continue;
		}
		lScapiMessage = "";
		lAarsdb.SPIMSG_AARELE[lAarele].CopyFromSz(lScapiMessage.c_str());
		sEleRtg[lAarele] = lRtgCompId;

		if (lAarsdb.LN_AARELE.Test(lAarele)) { // Write OPER, NORM and EMER
			float lCurOper = ReadLimit(lHandle, 1);
			float lCurNorm = ReadLimit(lHandle, 2);
			float lCurEmer = ReadLimit(lHandle, 3);

			lDm.Print(DML_1, DMF_S, "Writing Limits(OPER,NORM,EMER) for AARELE(%d)(%s) Current(%5.1f,%5.1f,%5.1f) New(%5.1f,%5.1f,%5.1f)",
				lAarele, lCompId.c_str(), lCurOper, lCurNorm, lCurEmer, lNewOper, lNewNorm, lNewEmer);
			
			if (lNewEmer < lCurEmer) {
				if (lNewNorm < lCurNorm) {
					WriteLimit(lHandle, 2, lNewNorm, lCurNorm);
					WriteLimit(lHandle, 1, lNewOper, lCurOper);
				}
				else {
					WriteLimit(lHandle, 2, lNewNorm, lCurNorm);
					WriteLimit(lHandle, 1, lNewOper, lCurOper);
				}
				WriteLimit(lHandle, 3, lNewEmer, lCurEmer);
			}
			else {
				WriteLimit(lHandle, 3, lNewEmer, lCurEmer);
				if (lNewNorm < lCurNorm) {
					WriteLimit(lHandle, 2, lNewNorm, lCurNorm);
					WriteLimit(lHandle, 1, lNewOper, lCurOper);
				}
				else {
					WriteLimit(lHandle, 2, lNewNorm, lCurNorm);
					WriteLimit(lHandle, 1, lNewOper, lCurOper);
				}
			}
		}
			
		else if (lAarsdb.CB_AARELE.Test(lAarele) || lAarsdb.LINE_AARELE.Test(lAarele)) { // Write OPER & NORM
			float lCurOper = ReadLimit(lHandle, 1);
			float lCurNorm = ReadLimit(lHandle, 2);
			lDm.Print(DML_1, DMF_S, "Writing Limits(OPER,NORM) for AARELE(%d)(%s) Current(%5.1f,%5.1f) New(%5.1f,%5.1f)",
				lAarele, lCompId.c_str(), lCurOper, lCurNorm, lNewOper, lNewNorm);
			if (lNewOper < lCurOper) { // Make sure we write in order to avoid nexting issues
				WriteLimit(lHandle, 1, lNewOper, lCurOper);
				WriteLimit(lHandle, 2, lNewNorm, lCurNorm);
			}
			else {
				WriteLimit(lHandle, 2, lNewNorm, lCurNorm);
				WriteLimit(lHandle, 1, lNewOper, lCurOper);
			}
		}
		
		else if (lAarsdb.XF_AARELE.Test(lAarele)) { // Write OPER & EMER
			float lCurOper = ReadLimit(lHandle, 1);
			float lCurEmer = ReadLimit(lHandle, 2);
			lDm.Print(DML_1, DMF_S, "Writing Limits(OPER,EMER) for AARELE(%d)(%s) Current(%5.1f,%5.1f) New(%5.1f,%5.1f)",
				lAarele, lCompId.c_str(), lCurOper, lCurEmer, lNewOper, lNewEmer);
			if (lNewOper < lCurOper) { // Make sure we write in order to avoid nexting issues
				WriteLimit(lHandle, 1, lNewOper, lCurOper);
				WriteLimit(lHandle, 2, lNewEmer, lCurEmer);
			}
			else {
				WriteLimit(lHandle, 2, lNewEmer, lCurEmer);
				WriteLimit(lHandle, 1, lNewOper, lCurOper);
			}
		}

		else {
			lDm.Print(DML_W, DMF_S, "Unhandled AARELE(%d)(%s) Type", lAarele, lCompId.c_str());
			continue;
		}

		lWriteSend = true;
	}

	if (lWriteSend) lHdbUtil.UtilScfBad(ScapiWriteSend(ScapiWriteCallbk, (void*)0, SCAPI_IMMEDIATE_CONTROL));
	WriteStatus("SCAPI Write Complete");
}

float AARSYNC_MAIN::ReadLimit(int iHandle, int iLimNum) { DBG_MGR_LOG;
    auto& lDm = GetDm();
    auto& lHdbUtil = HDB_UTIL(lDm);

    SCAPI_LIMIT_READ_REC lReadRec;
    lHdbUtil.UtilScfBad(ScapiReadAnalogLimit(iHandle, iLimNum, &lReadRec));
    return lReadRec.high_value;
}

void AARSYNC_MAIN::WriteLimit(int iHandle, int iLimNum, float iNewVal, float iCurVal) { DBG_MGR_LOG;
    auto& lDm = GetDm();
    auto& lHdbUtil = HDB_UTIL(lDm);

	if (iNewVal == iCurVal) return; // Non need to write as they are same

    SCAPI_LIMIT_WRITE_REC lWriteRec;
	lWriteRec.high_value = iNewVal;
    lWriteRec.high_validity = SCAPI_VALIDITY_VALID;
    lWriteRec.low_value     = -1.0;
	//lWriteRec.low_value = -iLimit;
	lWriteRec.low_validity = SCAPI_VALIDITY_NOTVALID;
	lWriteRec.masks = SCAPI_MASKS_LIMIT_OVERRIDE || SCAPI_MASKS_NORMAL_EXTERNAL;
    lHdbUtil.UtilScfBad (ScapiWriteAnalogLimitEx(iHandle, iLimNum, &lWriteRec, Task.c_str()));
	lDm.Print(DML_1, DMF_S, "Writing Limits(%d) New Value(%5.1f) Current Value(%5.1f)", iLimNum, iNewVal, iCurVal);
}

void AARSYNC_MAIN::ScapiWriteCallbk(SCF_STATUS iStatus, void*) {
    auto& lMain = AARSYNC_MAIN::GetMain();
    auto& lDm = lMain.GetDm();
    lDm.Print(DML_L, DMF_U, "Entering - %s", __FUNCTION__);

	auto& lHdbUtil = HDB_UTIL(lDm);
	auto& lAarsdb = lMain.GetAarsdb();

    if (iStatus == SCAPI_S_NORMAL) lDm.Print(DML_I, DMF_S, "Scapi write successful");
    
    else if (iStatus == SCAPI_E_FAILED) { // SCAPI Write Failed for atleast one request, process the error
        int lStatus = SCAPI_S_MORE;

        while (lStatus == SCAPI_S_MORE) {
            unsigned int lSub     = 0; // Analog index, in the order of registration 
            unsigned int lRepStat = 0; // Reply status            
            lStatus = ScapiWriteGetError(&lSub, &lRepStat);
            if (lStatus != SCAPI_S_DONE) { // Issue an error message,
				std::string lErrMsg = lHdbUtil.UtilScfText(lRepStat);
				lDm.Print(DML_I, DMF_S, "%s", lErrMsg.c_str());
                lDm.Print (DML_I, DMF_S, "SCAPI write failed for AAROUT(%d)(%s)", lSub, HdbId(lAarsdb.COMPID_AAROUT[lSub]).c_str());
				int lAarele = lAarsdb.I__AARELE_AAROUT[lSub];
				if (lAarele > 0) lMain.gScapiErr[lAarele] = lErrMsg;
            }
            PpmYield();
        }
        lDm.Print(DML_I, DMF_S, "SCAPI write complete with errors");
    }

    else lDm.Print(DML_I, DMF_S, "SCAPI Write returned unknown Status");

	lDm.Print(DML_L, DMF_S, "Returning - %s", __FUNCTION__);
}
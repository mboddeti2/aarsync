//------------------------------------------------------------------------------------------------//
// Description:
//   Contains various AARSYNC_MAIN class utilities
//
// Revision History:
//   2024-10-01: Original [Murali Boddeti (or)  mkb]
//------------------------------------------------------------------------------------------------//

// Include Files: system header file
#include<boost/tokenizer.hpp>

// Include Files: Habitat Headers

// Include Files: Current Program header file
#include "debug_manager.h"
#include "hdb_util.h"
#include "haccess_hdb.h"
#include "aarsync_main.h"

#include "haccess_aarsync_aarsdb.h"
#include "haccess_aarsync_netmom.h"
#include "haccess_aarsync_scadamom.h"

void AARSYNC_MAIN::CreateHash () { DBG_MGR_LOG;
	auto& lDm = GetDm();
	COMMON_UTIL lUtil (lDm);

	auto& lAarsdb= GetAarsdb();
	auto& lNetmom = GetNetmom();
	auto& lScadamom = GetScadamom();

	for (int lAnalog = 1; lAnalog <= lScadamom.ANALOG.GetLv(); lAnalog++) {
		int lMeas = lScadamom.P__MEAS_ANALOG[lAnalog];
		int lDevice = lScadamom.P__DEVICE_MEAS[lMeas];
		int lDevtyp = lScadamom.P__DEVTYP_DEVICE[lDevice];
		int lSubstn = lScadamom.P__SUBSTN_DEVTYP[lDevtyp];

		std::string lDevtypId = HdbId(lScadamom.ID_DEVTYP[lDevtyp]);

		if (!boost::iequals(lDevtypId, "LINE") && !boost::iequals(lDevtypId, "LN") && !boost::iequals(lDevtypId, "ZBR") &&
			!boost::iequals(lDevtypId, "XFMR") && !boost::iequals(lDevtypId, "XF")) continue; // Include only branches

		Analog[lUtil.CompId(HdbId(lScadamom.ID_SUBSTN[lSubstn]), HdbId(lScadamom.ID_DEVICE[lDevice]))] = lAnalog;
	}

	lDm.Print(DML_1, DMF_U, "Craeted NETMOM and SCADAMOM Hash Tables");
}

void AARSYNC_MAIN::PopulateNetmomModel () { DBG_MGR_LOG;
	auto& lDm = GetDm();
	auto& lUtil = COMMON_UTIL(lDm);

	auto& lAarsdb = GetAarsdb();
	auto& lNetmom = GetNetmom();

	if (lAarsdb.TVALNMD_ITEMS[1] == lNetmom.TVALNMD_ITEMS[1]) {
		lDm.Print(DML_1, DMF_U, "NETMOM Model populated already");
		return;
	}

	lDm.Print(DML_I, DMF_U, "Populating Model data from NETMOM");

	std::vector <std::string> lCols;
	boost::split(lCols, HdbId(lAarsdb.COINC_ITEMS[1]), boost::is_any_of(","));
	BOOST_FOREACH(const auto& it, lCols) CoInc[it] = true;

	struct AARELE_DATA {
		HdbMRID Mrid;
		std::string CompId;
		std::string Id;
		std::string CoId;
		std::string FrSt;
		std::string ToSt;
		std::string SegId;
		int NetSub;
		std::string Type;
	};
	std::map <std::string, AARELE_DATA> lEleData;

	for (int lSub = 1; lSub <= lNetmom.LN.GetLv();   lSub++) PopulateModelLn  (lEleData, lSub);
	for (int lSub = 1; lSub <= lNetmom.ZBR.GetLv();  lSub++) PopulateModelZbr (lEleData, lSub);
	for (int lSub = 1; lSub <= lNetmom.XF.GetLv();   lSub++) PopulateModelXf  (lEleData, lSub);
	for (int lSub = 1; lSub <= lNetmom.CB.GetLv();   lSub++) PopulateModelCb  (lEleData, lSub);

	if (lEleData.empty()) {
		lDm.Print(DML_E, DMF_U, "Number of Facilities empty from NETMOM", lEleData.size());
		return;
	}

	auto& lAarsdbHdb = GetAarsdbHdb();
	lAarsdbHdb.Write();

	lAarsdbHdb.AddOrDelRec("AARELE", 0, (int)lEleData.size() - lAarsdb.AARELE.GetLv(), 0);
	lAarsdbHdb.Read();

	int lSub = 1;
	BOOST_FOREACH (const auto& it, lEleData) {
		if (lSub > lAarsdb.AARELE.GetLv()) {
			lDm.PrintUnique(DML_E, DMF_U, "Cannot populate AARELE LV(%d) MX(%d) cannot fit Number of Facilities(%d)",
				lAarsdb.AARELE.GetLv(), lAarsdb.AARELE.GetMx(), lEleData.size());
			continue;
		}
		lAarsdb.MRID_AARELE.Set(it.second.Mrid, lSub);
		lAarsdb.COMPID_AARELE[lSub].CopyFromSz(it.second.CompId.c_str()); // Facility Composite ID
		lAarsdb.ID_AARELE[lSub].CopyFromSz(it.second.Id.c_str());         // Facility ID
		lAarsdb.SEGID_AARELE[lSub].CopyFromSz(it.second.SegId.c_str());   // Segment ID
		lAarsdb.COID_AARELE[lSub].CopyFromSz(it.second.CoId.c_str());     // Company ID
		lAarsdb.FRST_AARELE[lSub].CopyFromSz(it.second.FrSt.c_str());     // Substation ID
		lAarsdb.TOST_AARELE[lSub].CopyFromSz(it.second.ToSt.c_str());     // Other end Substation ID

		lAarsdb.TYPE_AARELE[lSub] = 0;
		
		lAarsdb.SPIMSG_AARELE[lSub].CopyFromSz("");

		/*lAarsdb.LINE_AARELE.Reset(lSub);
		lAarsdb.LN_AARELE.Reset(lSub);
		lAarsdb.ZB_AARELE.Reset(lSub);
		lAarsdb.XF_AARELE.Reset(lSub);
		lAarsdb.CB_AARELE.Reset(lSub);*/
		//if (it.second.Type == "LINE") lAarsdb.LINE_AARELE.Set(lSub);
		if      (it.second.Type ==  "LN") lAarsdb.LN_AARELE.Set(lSub);
		else if (it.second.Type == "ZBR") lAarsdb.ZB_AARELE.Set(lSub);
		else if (it.second.Type ==  "XF") lAarsdb.XF_AARELE.Set(lSub);
		else if (it.second.Type ==  "CB") lAarsdb.CB_AARELE.Set(lSub);
		else {
			lDm.PrintUnique(DML_W, DMF_U, "Unknow AARELE(%s) Type(%s)", it.second.CompId.c_str(), it.second.Type.c_str());
		}

		lAarsdb.NETSUB_AARELE[lSub] = it.second.NetSub;

		lSub++;
	}

	lAarsdb.TVALNMD_ITEMS[1] = lNetmom.TVALNMD_ITEMS[1];

	lAarsdbHdb.Write();
	lDm.Print(DML_I, DMF_U, "Populated(%d) facilities from NETMOM", lEleData.size());
}

bool AARSYNC_MAIN::IsCoIncluded (std::string iType, std::string iCompId, std::string iCoId) { DBG_MGR_LOG;
	if (CoInc.find(iCoId) != CoInc.end()) return true;
	GetDm().Print(DML_1, DMF_U, "%s(%s) CO(%s) Ignored", iType.c_str(), iCompId.c_str(), iCoId.c_str());
	return false;
}

template <typename T> void AARSYNC_MAIN::PopulateModelLn (T& iEleData, int iSub) { DBG_MGR_LOG;
	auto& lDm = GetDm();
	auto& lUtil = COMMON_UTIL(lDm);

	auto& lNetmom = GetNetmom();

	int lLine = lNetmom.P__LINE_LN[iSub];
	int lDv   = lNetmom.P__DV_LINE[lLine];
	int lCo   = lNetmom.P__CO_DV[lDv];

	std::string lLineId = HdbId(lNetmom.ID_LINE[lLine]);
	std::string lLnId   = HdbId(lNetmom.ID_LN[iSub]);
	std::string lCompId = lUtil.CompId(lLineId, lLnId);

	std::string lCoId = HdbId(lNetmom.ID_CO[lCo]);
	
	if (!IsCoIncluded("LN", lCompId, lCoId)) return;
	
	// Then LN or Segment
	auto& lObj2  = iEleData[lUtil.CompId("2", lCompId)]; // Sorting order in AARSDB
	lObj2.Mrid   = lNetmom.MRID_LN.Get(iSub);
	lObj2.Type   = "LN";
	lObj2.CompId = lCompId;
	lObj2.Id     = lLineId;
	lObj2.SegId  = lLnId;
	lObj2.CoId   = lCoId;
	lObj2.FrSt   = HdbId(lNetmom.ST_LN[iSub]);
	lObj2.ToSt   = HdbId(lNetmom.ZST_LN[iSub]);
	lObj2.NetSub = iSub;

	// Add Hash table with both CompId and MRID
	Ln[lCompId] = iSub;
	Ln[lNetmom.MRID_LN.Get(iSub).canonicalString()] = iSub;
}

template <typename T> void AARSYNC_MAIN::PopulateModelZbr (T& iEleData, int iSub) { DBG_MGR_LOG;
	auto& lDm = GetDm();
	auto& lUtil = COMMON_UTIL(lDm);

	auto& lNetmom = GetNetmom();

	int lLine = lNetmom.P__LINE_ZBR[iSub];
	int lDv   = lNetmom.P__DV_LINE[lLine];
	int lCo   = lNetmom.P__CO_DV[lDv];

	std::string lLineId = HdbId(lNetmom.ID_LINE[lLine]);
	std::string lZbrId  = HdbId(lNetmom.ID_ZBR[iSub]);
	std::string lCompId = lUtil.CompId(lLineId, lZbrId);

	std::string lCoId = HdbId(lNetmom.ID_CO[lCo]);

	if (!IsCoIncluded("ZBR", lCompId, lCoId)) return;
	
	// Then ZBR or Segment	
	auto& lObj2  = iEleData[lUtil.CompId("3", lCompId)]; // Sorting order in AARSDB
	lObj2.Mrid   = lNetmom.MRID_ZBR.Get(iSub);
	lObj2.Type   = "ZBR";
	lObj2.CompId = lCompId;
	lObj2.Id     = lLineId;
	lObj2.SegId  = lZbrId;
	lObj2.CoId   = lCoId;
	lObj2.FrSt   = HdbId(lNetmom.ST_ZBR[iSub]);
	lObj2.ToSt   = HdbId(lNetmom.ZST_ZBR[iSub]);
	lObj2.NetSub = iSub;
	// Add Hash table with both CompId and MRID
	Zbr[lCompId] = iSub;
	Zbr[lNetmom.MRID_ZBR.Get(iSub).canonicalString()] = iSub;
}

template <typename T> void AARSYNC_MAIN::PopulateModelXf (T& iEleData, int iSub) { DBG_MGR_LOG;
	auto& lDm = GetDm();
	auto& lUtil = COMMON_UTIL(lDm);

	auto& lNetmom = GetNetmom();

	int lSt = lNetmom.P__ST_XF[iSub];
	int lDv = lNetmom.P__DV_ST[lSt];
	int lCo = lNetmom.P__CO_DV[lDv];

	std::string lXfId = HdbId(lNetmom.ID_XF[iSub]);
	std::string lStId = HdbId(lNetmom.ID_ST[lSt]);
	std::string lCompId = lUtil.CompId(lStId, lXfId);

	std::string lCoId = HdbId(lNetmom.ID_CO[lCo]);

	if (!IsCoIncluded("XF", lCompId, lCoId)) return;
	
	auto& lObj  = iEleData[lUtil.CompId("4", lCompId)]; // Sorting order in AARSDB
	lObj.Mrid   = lNetmom.MRID_XF.Get(iSub);
	lObj.Type   = "XF";
	lObj.CompId = lCompId;
	lObj.Id     = lXfId;
	lObj.SegId  = "";
	lObj.CoId   = lCoId;
	lObj.FrSt   = lStId;
	lObj.ToSt   = lStId;
	lObj.NetSub = iSub;

	// Add Hash table with both CompId and MRID
	Xf[lCompId] = iSub;
	Xf[lNetmom.MRID_XF.Get(iSub).canonicalString()] = iSub;
}

template <typename T> void AARSYNC_MAIN::PopulateModelCb (T& iEleData, int iSub) { DBG_MGR_LOG;
	auto& lDm = GetDm();
	auto& lUtil = COMMON_UTIL(lDm);

	auto& lNetmom = GetNetmom();

	int lSt = lNetmom.P__ST_KV[lNetmom.I__KV_CB[iSub]];
	int lDv = lNetmom.P__DV_ST[lSt];
	int lCo = lNetmom.P__CO_DV[lDv];

	std::string lCbId = HdbId(lNetmom.ID_CB[iSub]);
	std::string lStId = HdbId(lNetmom.ID_ST[lSt]);
	std::string lCompId = lUtil.CompId(lStId, lCbId);

	std::string lCoId = HdbId(lNetmom.ID_CO[lCo]);

	if (!IsCoIncluded("CB", lCompId, lCoId)) return;
	
	auto& lObj  = iEleData[lUtil.CompId("5", lCompId)]; // Sorting order in AARSDB
	lObj.Mrid   = lNetmom.MRID_CB.Get(iSub);
	lObj.Type   = "CB";
	lObj.CompId = lCompId;
	lObj.Id     = lCbId;
	lObj.SegId  = "";
	lObj.CoId   = lCoId;
	lObj.FrSt   = lStId;
	lObj.ToSt   = lStId;
	lObj.NetSub = iSub;

	// Add Hash table with both CompId and MRID
	Cb[lCompId] = iSub;
	Cb[lNetmom.MRID_CB.Get(iSub).canonicalString()] = iSub;
}

void AARSYNC_MAIN::CreateAareleHash () { DBG_MGR_LOG;
	auto& lDm = GetDm();
	auto& lUtil = COMMON_UTIL(lDm);

	auto& lAarsdb = GetAarsdb();

	for (int lSub = 1; lSub <= lAarsdb.AARELE.GetLv(); lSub++) {
		std::string lCompId = lAarsdb.MRID_AARELE.Get(lSub).canonicalString();
		Aarele[lCompId] = lSub;
		lCompId = lUtil.CompId(HdbId(lAarsdb.FRST_AARELE[lSub]), HdbId(lAarsdb.ID_AARELE[lSub]));
		Aarele[lCompId] = lSub;
	}
}

void AARSYNC_MAIN::PopulateScadamomModel() { DBG_MGR_LOG;
	auto& lDm = GetDm();
	auto& lUtil = COMMON_UTIL(lDm);
	auto& lHdbUtil = HDB_UTIL(lDm);

	auto& lAarsdb = GetAarsdb();
	auto& lScadamom = GetScadamom();

	if (lAarsdb.VFYMOM_ITEMS[1] == lScadamom.VFYMOM_ITEMS[1]) {
		lDm.Print(DML_1, DMF_U, "SCADAMOM Model populated already");
		return;
	}

	lDm.Print(DML_I, DMF_U, "Populating Model data from SCADAMOM");

	for (int lAarele = 1; lAarele <= lAarsdb.AARELE.GetLv(); lAarele++) {
		lAarsdb.SMAP_AARELE.Reset(lAarele);
		lAarsdb.I__AAROUT_AARELE[lAarele] = 0;
	}

	struct AAROUT_DATA {
		HdbMRID Mrid;
		std::string CoId;
		std::string Substn;
		std::string Devtyp;
		std::string Device;
		std::string Meas;
		std::string Analog;
		int Key;
		int Sub;
	};
	std::map <std::string, AAROUT_DATA> lOutData;

	for (int lAnalog = 1; lAnalog <= lScadamom.ANALOG.GetLv(); lAnalog++) {
		
		std::string lAnalogId = HdbId(lScadamom.ID_ANALOG[lAnalog]);
		if (!boost::iequals(lAnalogId, "MVA")) continue; // We only write to MVA ratings
		if ((lScadamom.P__LIMIT_ANALOG[lAnalog + 1] - lScadamom.P__LIMIT_ANALOG[lAnalog]) == 0) continue; // No limits to write
		
		int lMeas   = lScadamom.P__MEAS_ANALOG[lAnalog];
		int lDevice = lScadamom.P__DEVICE_MEAS[lMeas];
		int lDevtyp = lScadamom.P__DEVTYP_DEVICE[lDevice];
		int lSubstn = lScadamom.P__SUBSTN_DEVTYP[lDevtyp];

		std::string lCoId = HdbId(lScadamom.CO_SUBSTN[lSubstn]);
		std::string lSubstnId = HdbId(lScadamom.ID_SUBSTN[lSubstn]);
		std::string lDevtypId = HdbId(lScadamom.ID_DEVTYP[lDevtyp]);
		std::string lDeviceId = HdbId(lScadamom.ID_DEVICE[lDevice]);
		std::string lMeasId = HdbId(lScadamom.ID_MEAS[lMeas]);

		std::string lCompId = lUtil.CompId(lSubstnId, lDevtypId, lDeviceId, lAnalogId);

		if (!IsCoIncluded("ANALOG", lCompId, lCoId)) continue;

		auto& lObj  = lOutData[lCompId];
		lObj.Mrid   = lScadamom.MRID_ANALOG.Get(lAnalog);
		lObj.CoId   = lCoId;
		lObj.Substn = lSubstnId;
		lObj.Devtyp = lDevtypId;
		lObj.Device = lDeviceId;
		lObj.Analog = lMeasId;
		lObj.Analog = lAnalogId;
		lObj.Sub    = lAnalog;
		lObj.Key    = lScadamom.KEY_ANALOG[lAnalog];
	}

	if (lOutData.empty()) {
		lDm.Print(DML_E, DMF_U, "AAROUT record size empty from SCADAMOM");
		return;
	}

	auto& lAarsdbHdb = GetAarsdbHdb();
	lAarsdbHdb.Write();
	lAarsdbHdb.AddOrDelRec("AAROUT", 0, (int)lOutData.size() - lAarsdb.AAROUT.GetLv(), 0);
	lAarsdbHdb.Read();

	int lSub = 1;
	BOOST_FOREACH (const auto& it, lOutData) {
		if (lSub > lAarsdb.AAROUT.GetLv()) {
			lDm.Print (DML_E, DMF_U, "Cannot populate AAROUT LV(%d) MX(%d) cannot fit Number of Facilities(%d)",
				lAarsdb.AAROUT.GetLv(), lAarsdb.AAROUT.GetMx(), lOutData.size());
			continue;
		}
		lAarsdb.MRID_AAROUT.Set(it.second.Mrid, lSub);
		lAarsdb.COMPID_AAROUT[lSub].CopyFromSz(it.first.c_str()); // Analog Composite ID
		lAarsdb.COID_AAROUT[lSub].CopyFromSz(it.second.CoId.c_str());
		lAarsdb.SUBSTN_AAROUT[lSub].CopyFromSz(it.second.Analog.c_str());
		lAarsdb.DEVTYP_AAROUT[lSub].CopyFromSz(it.second.Devtyp.c_str());
		lAarsdb.DEVICE_AAROUT[lSub].CopyFromSz(it.second.Device.c_str());
		lAarsdb.MEAS_AAROUT[lSub].CopyFromSz(it.second.Meas.c_str());
		lAarsdb.ANALOG_AAROUT[lSub].CopyFromSz(it.second.Analog.c_str());

		lAarsdb.SUB_AAROUT[lSub] = it.second.Sub;
		lAarsdb.KEY_AAROUT[lSub] = it.second.Key;

		std::string lEleId = lUtil.CompId(it.second.Substn, it.second.Device);
		//if (it.second.Devtyp == "LINE" || it.second.Devtyp == "LN") lEleId = lUtil.CompId(it.second.Devtyp, it.second.Device);
		int lAareleSub = Aarele[lEleId];
		lAarsdb.I__AARELE_AAROUT[lSub] = lAareleSub;
		if (lAareleSub > 0)	{
			lAarsdb.I__AAROUT_AARELE[lAareleSub] = lSub;
			lAarsdb.SMAP_AARELE.Set(lAareleSub);
			if (it.second.Devtyp == "LINE") { // Only for LINE rest keep it NETMOM record type
				lAarsdb.TYPE_AARELE[lAareleSub] = 0;
				lAarsdb.LINE_AARELE.Set(lAareleSub);
			}
		}

		if (lAareleSub <= 0) lDm.Print(DML_1, DMF_U, "Cannot map AAROUT(%s) to AARELE", it.first.c_str());

		lSub++;
	}

	lAarsdb.VFYMOM_ITEMS[1] = lScadamom.VFYMOM_ITEMS[1];
	lAarsdb.SCADART_ITEMS[1] = lHdbUtil.GetHabTime();
	lAarsdbHdb.Write();

	lDm.Print(DML_I, DMF_U, "Populated(%d) AAROUT records from SCADAMOM", lOutData.size());

}

bool AARSYNC_MAIN::Import (std::string iFileName, bool iForce) { DBG_MGR_LOG;
    // Import file if forced to import like command from UI to import (or) When hour boundary changed (or) When file contents change

	auto& lDm = GetDm();
	auto& lUtil = COMMON_UTIL(lDm);
	auto& lHdbUtil = HDB_UTIL(lDm);

    auto& lAarsdbHdb = GetAarsdbHdb ();
    auto& lAarsdb = GetAarsdb ();

    lAarsdbHdb.Read();

    if (iFileName.empty()) iFileName = HdbId(lAarsdb.DNVFILE_ITEMS[1]);
    iFileName = lUtil.GetFileName(iFileName);

	std::string lCurTimeHr = lHdbUtil.GetHdbTimeStr(lHdbUtil.GetHabTime(), "%d-%b-%Y %H");
	static std::string sTimeHr = "";

    // Import DNV Ratings file if needed
    time_t lWriteTime;
    try {
        lWriteTime = boost::filesystem::last_write_time(iFileName);
        if (!iForce) { // If forced import don't do the timestamp check
			if ((lWriteTime == UpdateTime) && (lCurTimeHr == sTimeHr)) { // No change in file or no change in current hour, don't import
                lDm.Print (DML_1, DMF_U, "File(%s) imported already", iFileName.c_str());
                return false;
            }
        }
    }
    catch (...) {
        lDm.Print (DML_E, DMF_U, "Error in accessing file(%s)", iFileName.c_str());
        return false;
    }

	for (int lSub = 1; lSub <= lAarsdb.AARELE.GetLv(); lSub++) {
		lAarsdb.DNV_AARELE.Reset(lSub);
		lAarsdb.VALID_AARELE.Reset(lSub);
		if (iForce) gScapiErr.clear(); // When forced import ignore Scapi errors and retry again
	}

    std::ifstream lFile(iFileName);

    UpdateTime = lWriteTime; // Modify update time so file is not read next time
    lDm.PrintDiffTime (DML_1, DMF_U, "Reading CSV file(%s), size(%u)", iFileName.c_str(), boost::filesystem::file_size(iFileName));

	std::string lCurTime = lHdbUtil.GetGMTTime("%Y-%m-%d %H");

	int nImported = 0;
    std::string lLine;
	while (getline(lFile, lLine)) {
		lLine.erase(std::remove(lLine.begin(), lLine.end(), '\n'), lLine.end()); // Remove new line char if any
		lLine.erase(std::remove(lLine.begin(), lLine.end(), '\r'), lLine.end()); // Remove new line char if any

		if (lLine.at(0) == '#') continue; // Ignore comment lines

		typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
		Tokenizer lToken(lLine, boost::escaped_list_separator<char>('\\', ',', '\"'));

		std::vector <std::string> lColVec;
		lColVec.assign(lToken.begin(), lToken.end());

		if (lColVec.size() <= 8) {
			lDm.PrintDiffTime(DML_W, DMF_U, "Insufficient number of rows(%d) at line(%s)", lColVec.size(), lLine.c_str());
			continue;
		}

		std::string lMrId  = lColVec[0];
		std::string lEleId = lColVec[1];
		std::string lSegId = lColVec[2];
		std::string lStId  = lColVec[3];
		std::string lTime  = lColVec[4];

		if (!boost::iequals(lTime, "-") && !boost::iequals(lTime, lCurTime)) continue; // Ratings data is not for current hour

		auto it = Aarele.find(lMrId);
		if (it == Aarele.end()) it = Aarele.find(lUtil.CompId(lEleId, lSegId)); // For Lines
		if (it == Aarele.end()) it = Aarele.find(lUtil.CompId(lStId, lEleId)); // For Transformers Switches
		if (it == Aarele.end()) {
			lDm.Print (DML_W, DMF_U, "Ignoring Facility @Line(%s)", lLine.c_str());
			continue;
		}

		lDm.Print(DML_1, DMF_U, "Importing ratings @Line(%s)", lLine.c_str());

		lAarsdb.DNV_AARELE.Set(it->second);
		if ((lAarsdb.NETSUB_AARELE[it->second] > 0) && lAarsdb.SMAP_AARELE.Test(it->second)) lAarsdb.VALID_AARELE.Set(it->second);

		lAarsdb.FILENORM_AARELE[it->second] = lUtil.Convert <float>(lColVec[6]);
		lAarsdb.FILEEMER_AARELE[it->second] = lUtil.Convert <float>(lColVec[7]);
		if (lColVec[5].empty()) lAarsdb.FILEOPER_AARELE[it->second] = ((float) 0.95) * lAarsdb.FILENORM_AARELE[it->second];
		else lAarsdb.FILEOPER_AARELE[it->second] = lUtil.Convert <float>(lColVec[5]);
		
		nImported++;

		if (lAarsdb.MAN_AARELE.Test(it->second)) {
			lDm.Print(DML_1, DMF_U, "Ignoring DNV Ratings for AARELE(%d)(%s), as manually entered", it->second, HdbId(lAarsdb.COMPID_AARELE[it->second]).c_str());
			continue;
		}
		else { // Not manually overwritten copy to ratings in use.
			lAarsdb.OPER_AARELE[it->second] = lAarsdb.FILEOPER_AARELE[it->second];
			lAarsdb.NORM_AARELE[it->second] = lAarsdb.FILENORM_AARELE[it->second];
			lAarsdb.EMER_AARELE[it->second] = lAarsdb.FILEEMER_AARELE[it->second];
			lAarsdb.USER_AARELE[it->second].CopyFromSz(""); // Since file ratings are copied from file mark user as empty
		}
	}

	lAarsdb.FILERT_ITEMS[1] = lHdbUtil.GetHabTime();

	lDm.Print(DML_I, DMF_U, "Imported ratings for(%d) facilities for Current Hour(%s)", nImported, lCurTime.c_str());

	sTimeHr = lCurTimeHr;
	
	return true;
}

bool AARSYNC_MAIN::Export (std::string iFileName) { DBG_MGR_LOG;
	auto& lDm = GetDm();
	auto& lUtil = COMMON_UTIL(lDm);
	auto& lHdbUtil = HDB_UTIL(lDm);
	auto& lAarsdb = GetAarsdb();

	if (iFileName.empty()) iFileName = "{HABITAT_DATAROOT}/aarsync/aarele_data.csv";
	iFileName = lUtil.GetFileName(iFileName);

	std::ofstream lFile(iFileName);
	if (!lFile.is_open()) {
		lDm.Print(DML_E, DMF_U, "Unable to open export file(%s)", iFileName.c_str());
		return false;
	}

	lFile << "rdf:ID,Facility ID,Segment ID,Substation ID,Time,OPER,NORM,EMER,Comments" << "\n";
	for (int lSub = 1; lSub <= lAarsdb.AARELE.GetLv(); lSub++) {
		if (!lAarsdb.SMAP_AARELE.Test(lSub)) continue; // Only export element that are mapped to SCADA
		lFile << lAarsdb.MRID_AARELE.Get(lSub).canonicalString() << ","
			  << HdbId(lAarsdb.ID_AARELE[lSub])                  << ","
			  << HdbId(lAarsdb.SEGID_AARELE[lSub])               << ","
			  << HdbId(lAarsdb.FRST_AARELE[lSub])                << ","
			  << lHdbUtil.GetGMTTime("%Y-%m-%d %H")              << ","
			  << lAarsdb.OPER_AARELE[lSub]                       << ","
			  << lAarsdb.NORM_AARELE[lSub]                       << ","
			  << lAarsdb.EMER_AARELE[lSub]                       << ","
			  << "AAR Element Type - " << GetEleType(lSub)       << "\n";
	}
	lDm.Print(DML_I, DMF_U, "Exported ratings file(%s) for(%d) facilities", iFileName.c_str(), lAarsdb.AARELE.GetLv());
	
	WriteStatus("Export Complete");
	
	return true;
}

void AARSYNC_MAIN::SetFileOld () { DBG_MGR_LOG;
	auto& lDm = GetDm();
	auto& lAarsdb = GetAarsdb();

	std::time_t lTimeNow = std::time(0);
	struct tm* lTimeInfo = std::localtime(&lTimeNow);
	std::ostringstream lTimeInfo_oss;
	lTimeInfo_oss << std::put_time(lTimeInfo, "%Y-%m-%d %H:%M:%S");

	struct tm* lUpdateTime = std::localtime(&UpdateTime);
	std::ostringstream lUpdateTime_oss;
	lUpdateTime_oss << std::put_time(lUpdateTime, "%Y-%m-%d %H:%M:%S");

	if (lTimeNow - UpdateTime > lAarsdb.FLOLDTH_ITEMS[1]) {
		lDm.Print(DML_W, DMF_U, "DNV File old, Current Time(%s) Update Time(%s) Threshold(%d)",
			lTimeInfo_oss.str().c_str(), lUpdateTime_oss.str().c_str(), lAarsdb.FLOLDTH_ITEMS[1]);
		lAarsdb.FILEOLD_ITEMS.Set(1);
	}
	else lAarsdb.FILEOLD_ITEMS.Reset(1);
}

std::string AARSYNC_MAIN::GetEleType (int iSub) { DBG_MGR_LOG;
	auto& lAarsdb = GetAarsdb();
	if    (lAarsdb.LINE_AARELE.Test(iSub)) return "LINE";
	else if (lAarsdb.LN_AARELE.Test(iSub)) return "LN";
	else if (lAarsdb.ZB_AARELE.Test(iSub)) return "ZB";
	else if (lAarsdb.XF_AARELE.Test(iSub)) return "XF";
	else if (lAarsdb.CB_AARELE.Test(iSub)) return "CB";
	return "N/A";
}
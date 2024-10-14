//------------------------------------------------------------------------------------------------//
// Description:
//   Ambient Adjusted Ratigns Synchronization applciaton a.k.a AARSYNC transfers AAR ratigns
//   data from DNV applciation to EMS SCADA, RTNET and RTCA.
//
// Design:
//   This file includes routines to register with Habitat PPM, PROCMAN, CFGMAN.
//
// Revision History:
//   2024-10-01: Original [Murali Boddeti (or)  mkb]
//------------------------------------------------------------------------------------------------//

// Include Files: system header file

// Include Files: Habitat Headers
#include "ppm.h"
#include "osal.h"
#include "mlf.h"
#include "ruser.h"
#include "procman.h"
#include "cfgman.h"
#include "appmgr.h"
#include "hul.h"

// Include Files: Current Program header file
#include "debug_manager.h"
#include "hdb_util.h"
#include "haccess_hdb.h"

#include "haccess_aarsync_aarsdb.h"
#include "haccess_aarsync_netmom.h"
#include "haccess_aarsync_scadamom.h"
#include "aarsync_main.h"

AARSYNC_MAIN* AARSYNC_MAIN::g_pMain = NULL; // Singletron instance
int main (const int argc, const char* argv[]) { // Main function
    auto& lMain = AARSYNC_MAIN::GetMain();
    for (int i = 1; i < argc; i++) lMain.CmdArg.push_back(argv[i]);
    return lMain.DoWork ();
}

AARSYNC_MAIN& AARSYNC_MAIN::GetMain() {
    if (g_pMain) return *g_pMain;
    g_pMain = new AARSYNC_MAIN();
    return *g_pMain;
}

AARSYNC_MAIN::AARSYNC_MAIN () {
}

AARSYNC_MAIN::~AARSYNC_MAIN () {
    GetDm().Print(DML_I, DMF_M, "%s - Task complete", Task.c_str());
}

void AARSYNC_MAIN::Usage() {
    std::cout << "\n";
    std::cout << "    Usage: " << Task << " <Command> <Command Options>\n";
    std::cout << "        * Import <DNV Ratigns File Name - Optional>\n";
    std::cout << "        *     Default file name is from AARSYNC.DNVFILE_ITEMS\n";
    std::cout << "\n";
    std::cout << "        * Help (or) -h - Help command\n";
    std::cout << "\n";
}

DBG_MGR& AARSYNC_MAIN::GetDm () { // To match dbg file name similar to how HABITAT api does
    if (pDm) return *pDm;
    std::string lDbgFile = Task + "_" + GetApp() + "_" + GetFam();
    boost::algorithm::to_lower(lDbgFile);
    pDm = boost::shared_ptr <DBG_MGR> (new DBG_MGR(lDbgFile, "{HABITAT_LOGDIR}"));
    return *pDm;
}

HACCESS_HDB <HaccessAarsdb>& AARSYNC_MAIN::GetAarsdbHdb () { DBG_MGR_LOG;
    if (AarsdbHdb) return *AarsdbHdb;
    AarsdbHdb = boost::shared_ptr <HACCESS_HDB<HaccessAarsdb>> (new HACCESS_HDB<HaccessAarsdb>(GetDm()));
    AarsdbHdb->Read(GetApp(), GetFam());
    auto& lAarsdb = AarsdbHdb->GetMom();
    GetDm().Set(lAarsdb.DBGLVL_ITEMS[1], lAarsdb.DBGFUN_ITEMS[1], lAarsdb.MAXMSG_ITEMS[1], lAarsdb.STDOUT_ITEMS.Test(1));
    return *AarsdbHdb;
}
HaccessAarsdb& AARSYNC_MAIN::GetAarsdb () { DBG_MGR_LOG;
    return GetAarsdbHdb().GetMom();
}

HACCESS_HDB <AarsyncNetmom>& AARSYNC_MAIN::GetNetmomHdb () { DBG_MGR_LOG;
    if (NetmomHdb) return *NetmomHdb;
    NetmomHdb = boost::shared_ptr <HACCESS_HDB<AarsyncNetmom>> (new HACCESS_HDB<AarsyncNetmom>(GetDm()));
    NetmomHdb->Read("RTNET", GetFam()); // Default read from RTCA
    return *NetmomHdb;
}
AarsyncNetmom& AARSYNC_MAIN::GetNetmom () { DBG_MGR_LOG;
    return GetNetmomHdb().GetMom();
}

HACCESS_HDB <AarsyncScadamom>& AARSYNC_MAIN::GetScadamomHdb () { DBG_MGR_LOG;
    if (ScadamomHdb) return *ScadamomHdb;
    ScadamomHdb = boost::shared_ptr <HACCESS_HDB<AarsyncScadamom>> (new HACCESS_HDB<AarsyncScadamom>(GetDm()));
    ScadamomHdb->Read("RTCA", GetFam()); // Default read from RTCA
    return *ScadamomHdb;
}
AarsyncScadamom& AARSYNC_MAIN::GetScadamom () { DBG_MGR_LOG;
    return GetScadamomHdb().GetMom();
}

std::string AARSYNC_MAIN::GetApp () { // Intentionally DM is not used so this routing can be called before DM initialized
    static std::string lCurApp = HDB_UTIL().GetApp();
    return lCurApp;
}
std::string AARSYNC_MAIN::GetFam () { // Intentionally DM is not used so this routing can be called before DM initialized
    static std::string lCurFam = HDB_UTIL().GetFam();
    return lCurFam;
}
std::string AARSYNC_MAIN::GetProcmanFam () { DBG_MGR_LOG;
    static std::string lProcmanFam = HDB_UTIL(GetDm()).GetAppFamily("PROCMAN", "PROCMAN");
    return lProcmanFam;
}

bool AARSYNC_MAIN::IsRealTime () { DBG_MGR_LOG;
    if (GetFam() == GetProcmanFam()) return true;
    return false;
}

int AARSYNC_MAIN::DoWork () {

    if (!CmdArg.empty()) { // Validate Input arguments before proceeding
       if (!boost::iequals(CmdArg[0], "Import")) {
           Usage (); 
           return 0; // return since working in command mode
       }
    }

    PpmInit(); // Initialize with PPM

    AppmgrSyncAbort (Task.c_str()); // Make sure we do this before DM object is created to avoid dbg file backup

    ExitHandle = OsalRegisterExitHandler( (OSAL_EXIT_CALLBACK) AARSYNC_MAIN::ExitHandler, (void *)this); // Register OSAL exit handler

    auto& lDm = GetDm();
    HDB_UTIL lHdbUtil(lDm);

    lDm.PrintDiffTime(DML_L, DMF_M, "%s:: Entered", __FUNCTION__);
    lDm.Print(DML_I, DMF_M, "%s Version(%s) Build On(%s %s) Started", Task.c_str(), Version.c_str(), __DATE__, __TIME__);

    if (!lHdbUtil.CheckDb("AARSYNC", GetApp(), GetFam())) return 0;

    InitParameters ();

    if (!CmdArg.empty()) {
       std::string lFileName = ""; if (CmdArg.size() > 1) lFileName = CmdArg[1];
       std::string lApp = "";      if (CmdArg.size() > 1) lApp      = CmdArg[1];
       std::string lFam = "";      if (CmdArg.size() > 2) lFam      = CmdArg[2];
       std::string lOption = "";   if (CmdArg.size() > 3) lOption   = CmdArg[3];

       lDm.Print(DML_I, DMF_M, "Running(%s) in command mode", Task.c_str());
       if      (boost::iequals(CmdArg[0], "Import")) Import (lFileName);

       lDm.PrintDiffTime(DML_I, DMF_M, "%s Command complete", Task.c_str());
       return 0; // return since working in command mode
    }

    MlfSetDeflogToHablog (NULL, NULL, "AARSLG", "TEXT"); // Set MLF default logs to AARSLG

    HabitatApiInit ();

    return 0;
}

void AARSYNC_MAIN::HabitatApiInit () { DBG_MGR_LOG;
    auto& lDm = GetDm();
    auto& lAarsdb = GetAarsdb ();
    HDB_UTIL lHdbUtil(lDm);

    // Register periodic timer so work can be performed in both real time and study mode
    int lGForm = TimedateAllocGform();
    time_t lCTypeTime;
    int lStartTime = lHdbUtil.GetHabTime();
    lStartTime = lStartTime + lAarsdb.OFFTM_ITEMS[1] + lAarsdb.PERTM_ITEMS[1] - lStartTime%lAarsdb.PERTM_ITEMS[1];
    
    lHdbUtil.UtilScfBad(TimedateCvtHabtimeToGform(lStartTime, lGForm));
    lHdbUtil.UtilScfBad(TimedateCvtGformToCtype(lGForm, &lCTypeTime));
    lHdbUtil.UtilScfBad(TimedateDeallocGform(lGForm));

    PPM_TIMER_SPEC TimeBlk;
    TimeBlk.type = PPM_TIMER_PA;
    TimeBlk.period_seconds = lAarsdb.PERTM_ITEMS[1];
    TimeBlk.absolute_time = lCTypeTime;
    TimeBlk.period_nanoseconds = 0;
    TimeBlk.absolute_nanoseconds = 0;

    // Advantages and disadvantages of using PPM timers vs PROCMAN schedules, predominantly the control we get using simple
    // parameter change vs complex PROCMAN model update. Periodic timer is needed to check SOL logic irrespective of execution cycles.
    PeriodicTimerHdl = PpmScheduleTimer(&TimeBlk, (PPM_TIMER_CALLBACK) AARSYNC_MAIN::TimerCallbk, (void *)this);

    if (IsRealTime()) ProcmanRegisterTask(Task.c_str(), ProcmanRegisterCb, (void *)this); // Register Procman only in real time mode
    else CurRole = CFGMAN_ROLE_ENABLED; // Trick to run in study mode

    if (lHdbUtil.UtilScfBad(RuserDefineUser((GetApp()+"_"+GetFam()).c_str(), RUSER_VERSION, RuserMailCallbk, RuserEntryCallbk))) {
        lDm.Print(DML_E, DMF_M, "RuserDefineUser failed but continuing the task"); // Failed to Register with RUSER
    }

    PpmMainLoop();
}

void AARSYNC_MAIN::ExitHandler (SCF_STATUS iStatus, void* iUserArgs) { // Static function
    auto& lMain = static_cast<AARSYNC_MAIN*>(iUserArgs) -> GetMain();
    auto& lDm = lMain.GetDm();
    auto& lAarsdb = lMain.GetAarsdb ();
    HDB_UTIL lHdbUtil(lDm);

    if (lHdbUtil.UtilScfBad(iStatus)) {
        lAarsdb.STATUS_ITEMS[1].CopyFromSz("Abnormal Exit");
        lDm.Print(DML_E, DMF_M, "%s::Exiting Task(%s)", __FUNCTION__, lMain.Task.c_str());
    }
    else {
        lDm.Print(DML_I, DMF_M, "%s::Exiting Task(%s) Normally", __FUNCTION__, lMain.Task.c_str());
        lAarsdb.STATUS_ITEMS[1].CopyFromSz("Normal Exit");
    }
    lAarsdb.DoGroup("SOLRT", Haccess::write);
}

void AARSYNC_MAIN::TimerCallbk (PPM_TIMER_HANDLE iTimerHdl, void* iUserArgs) { // Static function
    auto& lMain = static_cast<AARSYNC_MAIN*>(iUserArgs) -> GetMain();
    auto& lAarsdb = lMain.GetAarsdb ();
    lMain.GetDm().Print(DML_1, DMF_M, "Timer callback received");
    lMain.Import("", false);
}

void AARSYNC_MAIN::ProcmanRegisterCb (SCF_STATUS iStatus, void * iUserArgs) {
    auto& lMain = static_cast<AARSYNC_MAIN*>(iUserArgs) -> GetMain();
    auto& lDm = lMain.GetDm();
    HDB_UTIL lHdbUtil(lDm);

    if (lHdbUtil.UtilScfBad(iStatus)) {
        lDm.Print(DML_E, DMF_M, "%s:: Cannot register to PROCMAN", __FUNCTION__);
        return;
    }

    // Perform PROCMAN Node and Flag registrations
    lHdbUtil.UtilScfBad(ProcmanRegisterNode ("*", ProcmanNodeCb, (void *)&lMain));

    if (HulInRealtimeEnvironment()) { // Register with CFGMAN in real time mode
        CCA_BOOL AcceptsCalls = TRUE;
        if (!lHdbUtil.UtilScfBad(CfgmanRegisterProcessPpm (lMain.GetApp().c_str(),
                AcceptsCalls, CFGMAN_AVAILABILITY_UNAVAILABLE, CfgmanRoleCallBk, (void *)&lMain))) {
            CfgmanReportProcessStatus(CFGMAN_AVAILABILITY_AVAILABLE); // Report the Status as available to CFGMAN
        }
    }
    else { // Perform necessary tasks for DTS
    }
}

void AARSYNC_MAIN::ProcmanNodeCb (const char iNodeName[], void *iUserArgs) {
    auto& lMain = static_cast <AARSYNC_MAIN*> (iUserArgs) -> GetMain();
    auto& lDm = lMain.GetDm();
    auto& lAarsdb = lMain.GetAarsdb ();
    lMain.SrcApp = lMain.ProcmanNode[iNodeName];
    if (boost::iequals(iNodeName, "START_AARSYNC")) lDm.Print(DML_1, DMF_M, "Ignoring PROCMAN Node(%s), no action required", iNodeName);
    else lDm.Print(DML_W, DMF_M, "Ignoring PROCMAN Node(%s)", iNodeName);
}

void AARSYNC_MAIN::CfgmanRoleCallBk (CFGMAN_ROLE iNewRole, void *iUserArgs) {
    auto& lMain = static_cast<AARSYNC_MAIN*>(iUserArgs) -> GetMain();
    auto& lDm = lMain.GetDm();
    HDB_UTIL lHdbUtil(lDm);

    if ( iNewRole == CFGMAN_ROLE_ENABLED ) {
        lDm.Print(DML_I, DMF_M, "Task(%s) status Enabled with CFGMAN", lMain.Task.c_str());
        lMain.Import("", false);
    }
    else if (lMain.CurRole == CFGMAN_ROLE_ENABLED) { // CFGMAN informed node new role is not ENABLED was ENABLE before
        lDm.Print(DML_I, DMF_M, "Task(%s) task status Not-Enabled with CFGMAN", lMain.Task.c_str());
        // Un Register with Alarm TODO - future need?
    }

    lMain.CurRole = iNewRole;
    CfgmanConfirmProcessRole(lMain.CurRole);

    // Register with Cfgman Site TODO
    static bool sInitConfigMan = false;
    if (!sInitConfigMan) {
        if (lHdbUtil.UtilScfBad(CfgmanRegisterToSiteState (lMain.GetApp().c_str(),
            CfgmanStateReportCallBk, CfgmanStateCommandCallBk, (void *)&lMain, (void *)&lMain))) {
            lDm.Print(DML_E, DMF_M, "CFGMAN Site Status cannot be determined for task(%s)", lMain.GetApp().c_str());
        }
        sInitConfigMan = true;
    }
}

void AARSYNC_MAIN::CfgmanStateReportCallBk (CFGMAN_SITESTATE iCurState, CCA_USERARG iUserArgs) {
    // TODO if we need to track the site status in AARSYNC database via some flag setting, for now not done
    auto& lMain = static_cast<AARSYNC_MAIN*>(iUserArgs) -> GetMain();
    auto& lDm = lMain.GetDm();
    switch (iCurState) {
        case CFGMAN_SITESTATE_INACTIVE :
            lDm.Print(DML_I, DMF_M, "SITE status INACTIVE");
            break;
        case CFGMAN_SITESTATE_ACTIVE :
            lDm.Print(DML_I, DMF_M, "SITE status ACTIVE");
            break;
        case CFGMAN_SITESTATE_PASSIVE :
            lDm.Print(DML_I, DMF_M, "SITE status PASSIVE");
            break;
        case CFGMAN_SITESTATE_TEST :
            lDm.Print(DML_I, DMF_M, "SITE status TEST");
            break;
        default :
            lDm.Print(DML_I, DMF_M, "SITE status(%d) UNKNOWN", iCurState);
    }
}

void AARSYNC_MAIN::CfgmanStateCommandCallBk (CFGMAN_SITESTATE iNewState, CCA_USERARG iUserArgs) {
    // TODO if we need to track the site status in AARSYNC database via some flag setting, for now not done
    auto& lMain = static_cast<AARSYNC_MAIN*>(iUserArgs) -> GetMain();
    auto& lDm = lMain.GetDm();
    HDB_UTIL lHdbUtil(lDm);
    if (lHdbUtil.UtilScfBad(CfgmanResponseSiteStateCommand(lMain.GetApp().c_str(), iNewState, CFGMAN_ACCEPT, 0, 0))) {
        lDm.Print(DML_E, DMF_M, "Task(%s) cannot confirm the current site to CFGMAN", lMain.GetApp().c_str()); 
        return;
    }

    switch (iNewState) {
        case CFGMAN_SITESTATE_INACTIVE :
            lDm.Print(DML_I, DMF_M, "SITE status INACTIVE");
            break;
        case CFGMAN_SITESTATE_ACTIVE :
            lDm.Print(DML_I, DMF_M, "SITE status ACTIVE");
            break;
        case CFGMAN_SITESTATE_PASSIVE :
            lDm.Print(DML_I, DMF_M, "SITE status PASSIVE");
            break;
        case CFGMAN_SITESTATE_TEST :
            lDm.Print(DML_I, DMF_M, "SITE status TEST");
            break;
        default :
            lDm.Print(DML_I, DMF_M, "SITE status(%d) UNKNOWN", iNewState);
    }
}

void AARSYNC_MAIN::InitParameters () { DBG_MGR_LOG;
    auto& lDm = GetDm();
    auto& lAarsdbHdb = GetAarsdbHdb ();
    auto& lAarsdb = GetAarsdb ();

    lDm.Print(DML_1, DMF_M, "Initializing Default Parameters");

    bool lWrite = false;

    lAarsdbHdb.CopyIfEmpty ("{HABITAT_DATAROOT}/dnv_aar_data.csv", lAarsdb.DNVFILE_ITEMS[1]);

    if (SetDefault (lAarsdb.FLOLDTH_ITEMS,  1, 2*60*60 )) lWrite = true; 
    if (SetDefault (lAarsdb.OFFTM_ITEMS,    1, 10      )) lWrite = true;
    if (SetDefault (lAarsdb.PERTM_ITEMS,    1, 5*60    )) lWrite = true;

    if (SetDefault (lAarsdb.DBGLVL_ITEMS,   1, 0       )) lWrite = true;
    if (SetDefault (lAarsdb.DBGFUN_ITEMS,   1, 0       )) lWrite = true;
    if (SetDefault (lAarsdb.MAXMSG_ITEMS,   1, 100000  )) lWrite = true;

    lDm.Set(lAarsdb.DBGLVL_ITEMS[1], lAarsdb.DBGFUN_ITEMS[1], lAarsdb.MAXMSG_ITEMS[1], lAarsdb.STDOUT_ITEMS.Test(1));

    if (lWrite) lAarsdb.DoGroup("AARSPR",Haccess::write);
}

template <typename T1, typename T2> bool AARSYNC_MAIN::SetDefault (T1& iField, int iSub, T2 iValue) { DBG_MGR_LOG;
    if (!HdbTestNull(iField[iSub])) return false;
    iField[iSub] = iValue;
    return true;
}

void AARSYNC_MAIN::WriteStatus (std::string iStatus) { DBG_MGR_LOG;
    auto& lAarsdb = GetAarsdb ();
    lAarsdb.STATUS_ITEMS[1].CopyFromSz(iStatus.c_str());
    lAarsdb.DoGroup("AARSMR", Haccess::write);
}
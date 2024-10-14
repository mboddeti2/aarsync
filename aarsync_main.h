#ifndef _AARSYNC_MAIN_H_
#define _AARSYNC_MAIN_H_
//------------------------------------------------------------------------------------------------//
// Description:
//   AARSYNC Main header
//
// Design:
//
// Revision History:
//   2024-10-01: Original [Murali Boddeti (or)  mkb]
//------------------------------------------------------------------------------------------------//

// Include Files: System header file

// Include Files: External header file

// Include Files: Current Program header file

// Class Definitions 
#define FLOAT_EPSILON 0.00001f

class HaccessAarsdb;
class AarsyncNetmom;
class AarsyncScadamom;
class C_XML;

class AARSYNC_MAIN {

public:
    AARSYNC_MAIN ();
    ~AARSYNC_MAIN();

    static AARSYNC_MAIN& GetMain();

    std::vector <std::string> CmdArg;
    int DoWork ();

protected:

private:
    std::string Task = "AARSYNC";
    std::string Version = "1.0";
    std::string SrcApp; // Data Source Applciation name
    std::string SrcFam; // Data Source Family name
    std::string GetApp ();
    std::string GetFam ();
    std::string GetProcmanFam ();

    int CurTime = 0;
    int CurHour = 0;

    static AARSYNC_MAIN* g_pMain;

    boost::shared_ptr <DBG_MGR> pDm;
    DBG_MGR& GetDm ();

    boost::shared_ptr <HACCESS_HDB<HaccessAarsdb>> AarsdbHdb;
    HACCESS_HDB <HaccessAarsdb>& GetAarsdbHdb ();
    HaccessAarsdb& GetAarsdb ();

    boost::shared_ptr <HACCESS_HDB<AarsyncNetmom>> NetmomHdb;
    HACCESS_HDB <AarsyncNetmom>& GetNetmomHdb ();
    AarsyncNetmom& GetNetmom ();

    boost::shared_ptr <HACCESS_HDB<AarsyncScadamom>> ScadamomHdb;
    HACCESS_HDB <AarsyncScadamom>& GetScadamomHdb ();
    AarsyncScadamom& GetScadamom ();

    // aarsync_main.cpp routines
    void Usage ();

    void HabitatApiInit ();

    OSAL_EXIT_HANDLE ExitHandle;
    static void ExitHandler (SCF_STATUS iStatus, void* iUserArg);

    PPM_TIMER_HANDLE PeriodicTimerHdl;
    static void TimerCallbk (PPM_TIMER_HANDLE RTNTimerHdl, void* iUserArgs);

    static void ProcmanRegisterCb (SCF_STATUS iStatus, void *);
    static void ProcmanNodeCb (const char iNodeName[], void *);

    CFGMAN_ROLE CurRole = CFGMAN_ROLE_UNKNOWN; // Application current role
    static void CfgmanRoleCallBk (CFGMAN_ROLE iNewRole, void *);
    static void CfgmanStateReportCallBk (CFGMAN_SITESTATE iCurState, CCA_USERARG iUserArgs);
    static void CfgmanStateCommandCallBk (CFGMAN_SITESTATE iNewState, CCA_USERARG iUserArgs);

    bool IsRealTime ();

    void InitParameters ();
    template <typename T1, typename T2> bool SetDefault (T1& iField, int iSub, T2 iValue);
    void WriteStatus (std::string iStatus);

    // aarsync_import.cpp routines
    bool Import (std::string iFileName, bool iForce);

    // aarsync_ruser_mail.cpp routines
    static void RuserMailCallbk (const char* iCommand);

    // aarsync_ruser_entry.cpp routines
    static void RuserEntryCallbk (const RUSER_CONTEXT* iContext, const char* iCommand);
    static SCF_STATUS RuserEntryPredicateCallbk (const RUSER_CONTEXT* iContext, const RUSER_FIELD* iField, const RUSER_VALUE* iValue);
    template <typename T> void AssignMask (T& iField, int iSub, void *iValue);

    // aarsync_hash.cpp routines
    typedef std::map <std::string,std::string> AARSYNC_STRING_HASH;
    typedef std::map <std::string,int> AARSYNC_INT_HASH;

    template <typename T1, typename T2> void CreateIntHash (AARSYNC_INT_HASH& iHash, T1& iRec, T2& iId);
    template <typename T1, typename T2, typename T3> void CreateStringHash (AARSYNC_STRING_HASH& iHash, T1& iRec, T2& iId, T3& iValue);

    AARSYNC_STRING_HASH ProcmanNode;
    AARSYNC_INT_HASH User;

};
#endif // _AARSYNC_MAIN_H_

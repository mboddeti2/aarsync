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

// Include Files: Habitat Headers
#include "ppm.h"
#include "cfgman.h"
#include "ruser.h"

// Include Files: Current Program header file

// Class Definitions 
#define AARSYNC_VERSION "1.0"
#define AARSYNC_SCAPI_VERIFY 1
#define AARSYNC_SCAPI_REVERIFY 2

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
	std::string Version = AARSYNC_VERSION;
	std::string App = "";
	std::string Fam = "";
    std::string GetProcmanFam ();

    int CurTime = 0;
    int CurHour = 0;
	int VerifyType = AARSYNC_SCAPI_VERIFY;

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
    std::string WriteStatus (std::string iStatus);

    // aarsync_util.cpp routines
	std::map <std::string, bool> CoInc;
	std::map <std::string, int> Line;
	std::map <std::string, int> Ln;
	std::map <std::string, int> Zbr;
	std::map <std::string, int> Xf;
	std::map <std::string, int> Cb;
	std::map <std::string, int> Aarele;
	std::map <std::string, int> Analog;
	void CreateHash ();

	void PopulateNetmomModel();
	bool IsCoIncluded (std::string iType, std::string iCompId, std::string iCoId);
	template <typename T> void PopulateModelLn (T& iEleData, int iSub);
	template <typename T> void PopulateModelZbr(T& iEleData, int iSub);
	template <typename T> void PopulateModelXf (T& iEleData, int iSub);
	template <typename T> void PopulateModelCb (T& iEleData, int iSub);
	void CreateAareleHash ();

	void PopulateScadamomModel();

    bool Import (std::string iFileName, bool iForce);
	bool Export (std::string iFileName);
	std::time_t UpdateTime = 0;
	void SetFileOld ();
	std::string GetEleType (int iSub);
	void WriteMessage(char iSev, std::string iMsg);

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

    AARSYNC_INT_HASH User;

	// aarsync_scapi.cpp routines
	bool ScapiInitFlag   = false;
	bool ScapiEnableFlag = false;
	bool ScapiVerifyFlag = false;

	void ScapiInitialize ();
	static void ScapiCommCallBk(SCF_STATUS iStatus, void *);
	void ScapiRegister();
	static void ScapiVerifyCallBk(SCF_STATUS iStatus, void *iVerifyType);
	float ReadLimit(int iHandle, int iLimNum);
	void WriteLimits();
	void WriteLimit(int iHandle, int iLimNum, float iNewVal, float iCurVal);

	std::map <int, std::string> gScapiErr;
	static void ScapiWriteCallbk(SCF_STATUS iStatus, void*);

};
#endif // _AARSYNC_MAIN_H_

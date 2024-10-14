#ifndef _DBG_MGR_H_
#define _DBG_MGR_H_
//------------------------------------------------------------------------------------------------//
// Description:
//   Hold AARSYNC Debug Manager Class Header and implementation
//
// Design:
//   * Single threaded debug message handler, can control message by debug level and function.
//
// Revision History:
//   2024-10-01: Original [Murali Boddeti (or)  mkb]
//------------------------------------------------------------------------------------------------//

// Include Files: system header file
#include <sstream>
#include <iostream>
#include <iomanip>  // setw, setfill to work
#include <stdarg.h> // for va_start
#include <ctime>    // Required in Windows build

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/unordered/unordered_map.hpp>

#if defined(_MSC_VER) && (_MSC_VER <= 1500)
#else
#include <boost/chrono/chrono.hpp> // High Definition Clock
#endif

#if defined(_MSC_VER)
#pragma warning(disable:4996)
#endif

// Include Files: External header file
#include "mlf_aarslg.h"

// Include Files: Current program header file

// Type Definitions
#define MSG_TEXT_MAX_LEN 50000 // Debug print message text max length, Avoid this limit TODO

// Declaration of literals
enum DML_Types { // Debug Manager Levels Types
    DML_F=-3, DML_E=-2, DML_W=-1, DML_I=0,
    DML_1=1,  DML_2,    DML_3,    DML_4,   DML_5,
    DML_6,    DML_7,    DML_8,    DML_9,   DML_10,
    DML_11,   DML_12,   DML_13,   DML_14,  DML_15,
    DML_16,   DML_17,   DML_18,   DML_19,  DML_20,

    DML_L = 100, // DML_L, reserved from Function entry and Exit

    // Below remove after cleanup
    DM_Fatal = DML_W,
    DM_Err   = DML_E,
    DM_Warn  = DML_W,
    DM_Info  = DML_I,
    DM_Func  = DML_L,
    DM_DbgLvl_1  = DML_1,  DM_DbgLvl_2  = DML_2, DM_DbgLvl_3 = DML_3, DM_DbgLvl_4 = DML_4, DM_DbgLvl_5  = DML_5,
    DM_DbgLvl_6  = DML_6,  DM_DbgLvl_7  = DML_7, DM_DbgLvl_8 = DML_8, DM_DbgLvl_9 = DML_9, DM_DbgLvl_10 = DML_10,
    DM_DbgLvl_11 = DML_11, DM_DbgLvl_15 = DML_15
};

enum DMF_Types { // Debug Manager Function Types
    DMF_All=0,

    DMF_1,  DMF_2,  DMF_3,  DMF_4,  DMF_5,
    DMF_6,  DMF_7,  DMF_8,  DMF_9,  DMF_10,
    DMF_11, DMF_12, DMF_13, DMF_14, DMF_15,

    DMF_M = 21,   // Main Function
    DMF_P,        // Parser Function
    DMF_Util, DMF_Hdb,
    DMF_T,        // XML to HDB Transformation Function
    DMF_R = 26,   // Oracle / MS-SQL Interface Class
    DMF_U,        // General Utility Class
    DMF_UI,       // UI Function
    DMF_I,        // Function for misc. Interface classes
    DMF_IO,       // Input/Output Function
    DMF_PSSE,     // PSSE file export function
    DMF_L = 40,   // Log Function

    DMF_Mom = 51,
    DMF_BULK,     // Bulk Import Function
    DMF_WEBTOOL,  // Webtool Project conversion program
    DM_Func_Main = DMF_M,
    DM_Func_Mom = DMF_Mom
};

class DBG_MGR {

public:
    DBG_MGR (const std::string& iTask, const std::string& iDir) :
        Task(iTask), Level(0), Function(DMF_All), MaxMsg(100000), Stdout(true), CurNumMsgs(0) {

        #if defined(_MSC_VER) && (_MSC_VER <= 1500)
            StartTime  = time(NULL);
            DiffTime   = StartTime;
        #else
            StartTime  = boost::chrono::high_resolution_clock::now();
            DiffTime   = StartTime;
        #endif

        // Setup List of Backup files
        std::string lDir = ResolveEnv(iDir);
        for ( unsigned int i=4; i>0; i-- ) { // Saves 5 backup files
            std::stringstream lFileStr;
            lFileStr << lDir << "/" << Task << ".dbg-bak" << i;
            BakFile.push_back(lFileStr.str());
        }
        BakFile.push_back(lDir+"/"+Task+".dbg");
    }

    int  GetLevel    () {return Level;   }
    int  GetMaxMsg   () {return MaxMsg;  }
    int  GetFunction () {return Function;}
    bool GetStdout   () {return Stdout;  }

    void Set (int iLevel, int iFunction, int iMaxMsg, bool iStdout) {
        SetLevel (iLevel);
        SetFunction (iFunction);
        SetMaxMsg (iMaxMsg);
        SetStdout (iStdout);
    }
    void SetLevel (int iLevel) {
        if (iLevel == Level) return;
        Level = iLevel;
        Print (DML_I, DMF_All, "Task(%s) Debug Level(%d) Assigned", Task.c_str(), iLevel);
    }
    void SetFunction (int iFunction) {
        if (iFunction == Function) return;
        Function = iFunction;
        Print (DML_I, DMF_All, "Task(%s) Function Level(%d) Assigned", Task.c_str(), iFunction);
    }
    void SetMaxMsg (int iMaxMsg) {
        if (iMaxMsg == MaxMsg) return;
        MaxMsg = iMaxMsg;
        if (MaxMsg <= 0) MaxMsg = 100000; // Make sure we have some reasonable number in there 
        Print (DML_I, DMF_All, "Task(%s) Max. Number Of Messages(%d) Assigned", Task.c_str(), MaxMsg);
    }
    void SetStdout (bool iStdout) {
        if (iStdout == Stdout) return;
        Stdout = iStdout;
        Print (DML_I, DMF_All, "Task(%s) Stdout(%s) Assigned", Task.c_str(), iStdout ? "true": "false");
    }

    bool Print (const int& iLevel, const char *Fmt, ...) { // Remove after cleanup TODO
        if (!ValidMsg(iLevel, DMF_All)) return false;

        char lInMsg[MSG_TEXT_MAX_LEN+1] = {0};
        va_list Args;
        va_start(Args,Fmt);
        vsnprintf(lInMsg, MSG_TEXT_MAX_LEN, Fmt, Args);
        va_end(Args);

        std::string lMsg = lInMsg;
        return Print (iLevel, DMF_All, lMsg);
    }

    bool Print (const int& iLevel, const int& iFunc, const char *Fmt, ...) {
        if (!ValidMsg(iLevel, iFunc)) return false;

        char lInMsg[MSG_TEXT_MAX_LEN+1] = {0};
        va_list Args;
        va_start(Args,Fmt);
        vsnprintf(lInMsg, MSG_TEXT_MAX_LEN, Fmt, Args);
        va_end(Args);

        std::string lMsg = lInMsg;
        return Print (iLevel, iFunc, lMsg);
    }

    std::map<std::string,boost::shared_ptr <std::ofstream>> LogFilePtr; // Specific log file messsage if needed
    std::ofstream& GetLogFile (std::string iFileName, const int& iLevel, const int& iFunc) {
        auto it = LogFilePtr.find(iFileName);
        if (it != LogFilePtr.end()) return *it->second;
        Print (DML_1, iFunc, "%s::Opening File(%s)", __FUNCTION__, iFileName.c_str());
        LogFilePtr[iFileName] = boost::shared_ptr <std::ofstream> (new std::ofstream(iFileName,std::ofstream::out));
        if (!LogFilePtr[iFileName] || !*LogFilePtr[iFileName]) {
            Print (DML_W, iFunc, "Unabled to open file - %s", iFileName.c_str());
            perror(iFileName.c_str());
        }
        return *LogFilePtr[iFileName];
    }

    boost::unordered_map <std::string, bool> UniqueMsg;
    bool PrintUniqueFile (std::string iFileName, const int& iLevel, const int& iFunc, const char *Fmt, ...) {
        // Print message to separate file by checking the uniqueness but issue debug message based on other checks
        char lInMsg[MSG_TEXT_MAX_LEN+1] = {0};
        va_list Args;
        va_start(Args,Fmt);
        vsnprintf(lInMsg, MSG_TEXT_MAX_LEN, Fmt, Args);
        va_end(Args);

        std::string lMsg = lInMsg;
        std::stringstream lMsgPrefix;
        if      (iLevel == DML_I) lMsgPrefix << "   I";
        else if (iLevel == DML_W) lMsgPrefix << "   W";
        else if (iLevel == DML_E) lMsgPrefix << "   E";
        else if (iLevel == DML_F) lMsgPrefix << "   F";
        else lMsgPrefix << " D" << std::setw(2) << std::setfill('0') << iLevel;

        if (Stdout && iLevel <= DML_I) {
            auto &lIssued = UniqueMsg[lMsg];
            if (lIssued) return false; // The message issued and marked to be uniq return
            lIssued = true; // Dont mark the message as issued here
            GetLogFile(iFileName, iLevel, iFunc) << lMsgPrefix.str() << ": " << lMsg << std::endl;
        }

        return true;
    }
    bool PrintUnique (const int& iLevel, const int& iFunc, const char *Fmt, ...) {
        // Prints message only once for life of the program, to filter repeated messages from filling the disk
        if (!ValidMsg(iLevel, iFunc)) return false;

        char lInMsg[MSG_TEXT_MAX_LEN+1] = {0};
        va_list Args;
        va_start(Args,Fmt);
        vsnprintf(lInMsg, MSG_TEXT_MAX_LEN, Fmt, Args);
        va_end(Args);

        std::string lMsg = lInMsg;

        auto &lIssued = UniqueMsg[lMsg];
        if (lIssued) return false; // The message issued and marked to be uniq return
        lIssued = true;
        return Print (iLevel, iFunc, lMsg);
    }

    bool Print (const int& iLevel, const int& iFunc, const std::string& iMsg) {
        if (!ValidMsg(iLevel, iFunc)) return false;

        boost::filesystem::ofstream& lDmFile = GetDmFile ();

        std::string lTimeStr = TimeStr(std::time_t());
        std::stringstream lMsgStr;
        lMsgStr << " F" << std::setw(3) << std::setfill('0') << iFunc;
        if      (iLevel == DML_I) lMsgStr << "   I";
        else if (iLevel == DML_W) lMsgStr << "   W";
        else if (iLevel == DML_E) lMsgStr << "   E";
        else if (iLevel == DML_F) lMsgStr << "   F";
        else lMsgStr << " D" << std::setw(2) << std::setfill('0') << iLevel;

        if (Stdout && iLevel <= DML_I) std::cout << lTimeStr << lMsgStr.str() << ": " << iMsg << std::endl;

        lDmFile << lTimeStr << lMsgStr.str() << ": " << iMsg << std::endl;

        if      (iLevel == DML_I) MlfLogAarsyncAa004(iMsg.c_str());
        else if (iLevel == DML_W) MlfLogAarsyncAa003(iMsg.c_str());
        else if (iLevel == DML_E) MlfLogAarsyncAa002(iMsg.c_str());
        else if (iLevel == DML_F) MlfLogAarsyncAa001(iMsg.c_str());

        CurNumMsgs ++; // Initialize the number of message

        if ( CurNumMsgs >= MaxMsg ) DeleteDmFile();
        return true; // Issued message successfully
    }

    bool PrintTime (const int& iLevel, const std::string& iMsg) {
        return PrintTime (iLevel, DMF_All, iMsg);
    }
    bool PrintTime (const int& iLevel, const int& iFunc, const std::string& iMsg) {
        std::string lMsg = iMsg + " %8.2f seconds";
        float  lSec = 0.0;

        #if defined(_MSC_VER) && (_MSC_VER <= 1500)
            lSec = (float) StartTime;
        #else
            boost::chrono::high_resolution_clock::time_point lCurTime = boost::chrono::high_resolution_clock::now();
            lSec = (float) ((boost::chrono::duration_cast <boost::chrono::milliseconds>(lCurTime - StartTime).count()) / 1000.0);
        #endif

        return Print (iLevel, iFunc, lMsg.c_str(), lSec);
    }

    bool PrintDiffTime (const int& iLevel, const int& iFunc, const char *Fmt, ...) {
        char lInMsg[MSG_TEXT_MAX_LEN+1] = {0};
        va_list Args;
        va_start(Args,Fmt);
        vsnprintf(lInMsg, MSG_TEXT_MAX_LEN, Fmt, Args);
        va_end(Args);

        std::string lMsg = lInMsg;
        lMsg += " %7.2f sec";
        float lSec = 0.0;

        #if defined(_MSC_VER) && (_MSC_VER <= 1500)
            time_t lCurTime = time(NULL);
            lSec = (float) difftime(lCurTime, DiffTime);
            DiffTime = lCurTime;
        #else
            boost::chrono::high_resolution_clock::time_point lCurTime = boost::chrono::high_resolution_clock::now();
            lSec = (float) ((boost::chrono::duration_cast <boost::chrono::milliseconds>(lCurTime - DiffTime).count()) / 1000.0);
            DiffTime = lCurTime;
        #endif

        return Print(iLevel, iFunc, lMsg.c_str(), lSec);
    }

    std::string TimeStr(std::time_t iTime) {
        char lBuf [25];
        std::time (&iTime);
        #if defined(_WIN32)
            struct tm lTimeInfo;
            localtime_s(&lTimeInfo, &iTime);
            std::strftime (lBuf,sizeof(lBuf),"%d-%b-%Y %H:%M:%S", &lTimeInfo);
        #else
            std::tm* lTimeInfo;
            lTimeInfo = std::localtime (&iTime);
            std::strftime(lBuf,sizeof(lBuf),"%d-%b-%Y %H:%M:%S", lTimeInfo);
        #endif
        return lBuf;
    }

protected:

private:
    std::string Task;       // Task Name
    int         Level;      // Debug Level
    int         Function;   // Debug Function number 
    int         MaxMsg;     // Debug Max Messages
    int         CurNumMsgs; // Current Number of messages
    bool        Stdout;     // if true all messages will show on terminal 

    std::vector <std::string> BakFile;

    #if defined(_MSC_VER) && (_MSC_VER <= 1500)
        // Cannot support hight resolution clock for VS2008 and less with current setup
        time_t StartTime; // Program Start Time
        time_t DiffTime;  // Time difference from previous PrintDiffTime call
    #else
        // High resolution clock to support VS 2010 and above, Linux
        boost::chrono::high_resolution_clock::time_point StartTime;  // Program start high resolution time 
        boost::chrono::high_resolution_clock::time_point DiffTime;   // Time difference from previous PrintDiffTime call
    #endif

    boost::shared_ptr <boost::filesystem::ofstream> pDmFile;
    boost::filesystem::ofstream& GetDmFile () {
        if (pDmFile) return *pDmFile; // Valid pointer exists return
        pDmFile = boost::shared_ptr <boost::filesystem::ofstream> (new boost::filesystem::ofstream());

        // Backup current file before writing
        for (unsigned int i=1; i<BakFile.size(); i++) {
            try {
                if (!boost::filesystem::exists(BakFile[i])) continue;
                boost::filesystem::rename(BakFile[i],BakFile[i-1]);
            }
            catch (...) {
                std::cout << "Cannot Rename File(" << BakFile[i] << ") To(" << BakFile[i-1] << ")\n";
                *pDmFile << "Cannot Rename File(" << BakFile[i] << ") To(" << BakFile[i-1] << ")\n";
            }
        }

        pDmFile->open(BakFile[BakFile.size()-1]);
        if (!pDmFile->is_open()) std::cout << "Unable to open DebugManager Output File - " << BakFile[BakFile.size()-1] << std::endl;

        *pDmFile << "**** Debug File created on(" << TimeStr(std::time_t()) << ") App(" << Task
            << ") Level (" << Level << ") Func(" << Function << ") MaxMsgs("
            << MaxMsg << ") Stdout(" << std::boolalpha << Stdout << ") ****\n";
        CurNumMsgs = 0; // Reset the counter to start counting message from 0
        return *pDmFile;
    }

    void DeleteDmFile () {
        if (pDmFile == NULL) return; // Nothing to do
        pDmFile.reset();
    }

    std::string ResolveEnv (const std::string& iName) { // Routine to resolve environment variables exists if any
        std::string lName = iName;
        size_t lpPos1 = 0, lpPos2 = 0;
        while ( (lpPos1 = lName.find('{', lpPos1)) != std::string::npos ) {
            if ( (lpPos2 = lName.find('}', lpPos1)) != std::string::npos ) {
                // Between lpPos1 and lpPos2 we have Dir Environment Variable Name
                std::string lDirEnv = lName.substr(lpPos1+1,lpPos2-1);
                char *lDir = getenv(lDirEnv.c_str());
                if (lDir != NULL) {
                    lName.erase (lpPos1, lpPos2-lpPos1+1);
                    lName.insert (lpPos1, lDir);
                }
            }
            lpPos1++; // To substitute any other environment variables
        }
        return lName;
    }

    bool ValidMsg (const int& iLevel, const int& iFunc) {
        if ( iLevel > Level ) return false; // Return as current level is > user defined
        if ( (Function != DMF_All) && (Function != iFunc) ) return false;
        return true;
    }

};

#define DBG_MGR_LOG DBG_MGR_TRACER _token(__FUNCTION__,GetDm())
struct DBG_MGR_TRACER {
    char const* lFun;
    DBG_MGR *pDm;
    DBG_MGR_TRACER (char const* iFun, DBG_MGR& iDm): lFun(iFun), pDm(&iDm) {
        pDm->Print (DML_L, DMF_U, "Entering - %s", lFun);
    }
    ~DBG_MGR_TRACER () {
        pDm->Print (DML_L, DMF_U, "Returning - %s", lFun);
    }
};

#define DBG_MGR_LOG_TIME DBG_MGR_TRACER_TIME _token(__FUNCTION__,GetDm())
struct DBG_MGR_TRACER_TIME {
    char const* lFun;
    DBG_MGR *pDm;
    boost::chrono::high_resolution_clock::time_point StartTime; // Program start high resolution time 

    DBG_MGR_TRACER_TIME (char const* iFun, DBG_MGR& iDm): lFun(iFun), pDm(&iDm) {
        GetPrefix (true);
        StartTime = boost::chrono::high_resolution_clock::now();
    }
    ~DBG_MGR_TRACER_TIME () {
        float lSec = (boost::chrono::duration_cast <boost::chrono::milliseconds>
            (boost::chrono::high_resolution_clock::now() - StartTime).count()) / 1000.0;
        pDm->Print (DML_I, DMF_U, "%s%s::Completed in - %7.2f seconds", GetPrefix(false).c_str(), lFun, lSec);
    }
    std::string GetPrefix (bool iSet) {
        static std::string Prefix = "";
        if (iSet) Prefix += " ";
        else if (!Prefix.empty()) Prefix.erase(Prefix.length()-1);
        return Prefix;
    }
};
#endif // _DBG_MGR_H_

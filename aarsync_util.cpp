//------------------------------------------------------------------------------------------------//
// Description:
//   Contains various AARSYNC_MAIN class utilities
//
// Revision History:
//   2024-10-01: Original [Murali Boddeti (or)  mkb]
//------------------------------------------------------------------------------------------------//

// Include Files: system header file

// Include Files: Habitat Headers

// Include Files: Current Program header file
#include "debug_manager.h"
#include "hdb_util.h"
#include "haccess_hdb.h"
#include "aarsync_main.h"

bool AARSYNC_MAIN::Import (std::string iFileName, bool iForce) { DBG_MGR_LOG;

    auto& lAarsdbHdb = GetAarsdbHdb ();
    auto& lAarsdb = GetAarsdb ();

    lAarsdbHdb.Read();

    auto& lDm = GetDm();
    COMMON_UTIL lUtil(lDm);
    HDB_UTIL lHdbUtil(lDm);

    if (iFileName.empty()) iFileName = HdbId(lAarsdb.DNVFILE_ITEMS[1]);
    iFileName = lUtil.GetFileName(iFileName);

    // Import DNV Ratings file if needed
    static time_t lUpdateTime = 0;
    time_t lWriteTime;
    try {
        lWriteTime = boost::filesystem::last_write_time(iFileName);
        if (!iForce) { // If forced import don't do the timestamp check
            if (lWriteTime == lUpdateTime) {
                lDm.Print (DML_2, DMF_P, "File(%s) imported already", iFileName.c_str());
                return false;
            }
        }
    }
    catch (...) {
        lDm.Print (DML_W, DMF_P, "Error in accessing file(%s)", iFileName.c_str());
        return false;
    }

    std::string lFileName = lUtil.GetFileName(iFileName);
    std::ifstream lFile(lFileName);

    lUpdateTime = lWriteTime; // Modify update time so file is not read next time
    lDm.PrintDiffTime (DML_1, DMF_P, "Reading CSV file(%s), size(%u)", lFileName.c_str(), boost::filesystem::file_size(lFileName));

    std::string lLine;
    std::vector <std::string> lHeader;
    std::vector <std::string> lColVec;

    unsigned int lRow = 0;
    while (getline(lFile,lLine)) {
        lLine.erase(std::remove(lLine.begin(), lLine.end(), '\n'), lLine.end()); // Remove new line char if any
        lLine.erase(std::remove(lLine.begin(), lLine.end(), '\r'), lLine.end()); // Remove new line char if any

        typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
        Tokenizer lToken(lLine, boost::escaped_list_separator<char>('\\', ',', '\"'));
        lColVec.assign (lToken.begin(), lToken.end());

        if (lColVec.empty()) continue; // No columns to read

        if (lColVec[0].at(0) == '#') { // This is a header row
            if (lHeader.empty()) { // Only assign header for the first time per file
                lHeader = lColVec;
                lHeader[0].erase(0, 1); // Basically remove first char # sign
            }
            continue;
        }
        if (lHeader.size() != lColVec.size()) {
            lDm.Print (DML_E, DMF_P, "Columns(%d) != Header Columns(%d) at Line(%s)", lColVec.size(), lHeader.size(), lLine.c_str());
            continue;
        }

        ObjId = lColVec[0];
        if (!boost::iequals(lHeader[0], "rdf:ID")) ObjId = lUtil.CompId(lRow,lLine); // Make the whole line as rdf:ID

        for (unsigned int i=0; i<lColVec.size(); i++ ) {
        }
        lRow++;
    }
}
//------------------------------------------------------------------------------------------------//
// Description:
//   Clone schema file - defining App Name, Databases and Savecases
//
// Design:
//   * TODO
//
// Revision History:
//   2024-10-01: Original [Murali Boddeti (or)  mkb]
//------------------------------------------------------------------------------------------------//

CREATE APPLICATION AARSYNC
    USE DATABASE AARSDB.PSE_AARSYNC
    USE DATABASE AARSLG.PSE_AARSYNC

    DEFINE CASE AARSDB AS AARSDB
    DEFINE CASE ENTIRE AS AARSDB, AARSLG
    DEFINE CASE PARAMS AS AARSPR.AARSDB;
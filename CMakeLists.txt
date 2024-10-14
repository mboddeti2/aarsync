cmake_minimum_required(VERSION 2.8.8)
#------------------------------------------------------------------------------------------------#
# Description:
#   CMakeLists.txt - Describes the code to build dependencies, libraries and executables
#
# Design:
#   * Builds the dbdef as pre build step and build aarsync libs, aarsync and aarsync_test exes
#
# Revision History: Add the signature whenever changing this file
#   2024-10-01: Original [Murali Boddeti (or)  mkb]
#------------------------------------------------------------------------------------------------#

PROJECT( aarsync_build )

include ($ENV{HABUSER_CFGDIR}/habuser.cmake)
init_habitat_libraries()

# Build aarsync library
add_sources_exclude_main ( SOURCES )
LIST(LENGTH SOURCES num)
if ( NOT ${num} STREQUAL "0" )
    hab_add_library (aarsync ${SOURCES})
endif()

# Build all executables
HAB_ADD_EXE (aarsync_exe aarsync_main.cpp aarsync)
HAB_EXE_SKIP_FORTRAN ( aarsync_exe 1 )
if (CMAKE_SYSTEM MATCHES Linux)
    hab_target_link_libraries (aarsync_exe
         aarsync
         ${LINK_HABITAT_CXX_LIBS}
         ${LINK_HACCESS_LIBS}
         ${LINK_SCADA_CXX_LIBS}
         ${Boost_FILESYSTEM_LIBRARY}
         ${Boost_CHRONO_LIBRARY}
        )
elseif (CMAKE_SYSTEM MATCHES Windows)
    hab_target_link_libraries (aarsync_exe
         aarsync
         ${LINK_HABITAT_CXX_LIBS}
         ${LINK_HACCESS_LIBS}
         ${LINK_SCADA_CXX_LIBS}
         ${Boost_FILESYSTEM_LIBRARY}
         ${Boost_CHRONO_LIBRARY}
        )
endif ()

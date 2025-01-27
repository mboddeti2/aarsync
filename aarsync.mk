#------------------------------------------------------------------------------------------------//
# Description:
#   .mk file to build necessary 
#
# Revision History:
#   2024-10-01: Original [Murali Boddeti (or)  mkb]
#------------------------------------------------------------------------------------------------//

CODESET aarsync

MESSAGE_FILE aarsync
MESSAGE_C_BINDING

CPP_FILE aarsync_ruser_entry
CPP_FILE aarsync_ruser_mail
CPP_FILE aarsync_scapi
CPP_FILE aarsync_util
CPP_FILE haccess_aarsync_aarsdb
CPP_FILE haccess_aarsync_aarslg
CPP_FILE haccess_aarsync_netmom
CPP_FILE haccess_aarsync_scadamom

EXE_CPP_TASK aarsync
EXE_CPP_MAIN aarsync_main

LIBS $(LIBDIR)/libaarsync \
     $(HABITAT_LIBS) 

LINK_LIBS \
    -laarsync \
    -lscadamdl \
	-lscadacalc \
    -lscada \
    -lscipc \
    -lscadautil   \
    -lscapi_cxx \
    $(LINK_HABITAT_CXX_LIBS) \
    -lboost_filesystem \
	-lboost_system
# end of file

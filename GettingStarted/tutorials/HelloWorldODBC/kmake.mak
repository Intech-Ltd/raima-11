# Auto generated please do not edit this file

#include $(RDM_HOME)$/sysmake$/$(KMAKESYS).mak

CC_DEFS = $DNEED_SETUP_DATABASE 
CC_INCL = $I../../../include 

#include $(RDM_HOME)$/kheader.mak
#include "HelloWorldODBC.dp"

$#RSQL{name=hello_worldODBC}


all: hello_world_ODBCTutorial 

hello_world_ODBCTutorial: odir $Ohello_world_ODBCTutorial$e

OBJS = $(GENERATED) \


EXE_LIBS = $(RDBC_LIB) $(RSQL_LIB) $(XML_LIB) $(RDM_LIB) $(RDM_LIB) $(DDL_LIB) $(TFS_LIB) $(HTTP_LIB) $(ENC_LIB) $(SBASE_LIB) $(BASE_LIB) $(TRANSPORT_LIB) $(TRANSPORT_LIB) $(PSP_LIB) $(MATH_LIB) 

$#MKEXE{prog=hello_world_ODBCTutorial, objs= $(OBJS) hello_world_ODBCTutorial_main$o, libs=$(EXE_LIBS)}

$#KEND{}
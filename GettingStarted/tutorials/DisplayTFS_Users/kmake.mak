# Auto generated please do not edit this file

#include $(RDM_HOME)$/sysmake$/$(KMAKESYS).mak

CC_DEFS = $DNEED_SETUP_DATABASE 
CC_INCL = $I../../../include 

#include $(RDM_HOME)$/kheader.mak
#include "DisplayTFS_Users.dp"

$#DATABASE{name=displayUsers_db, carray=1}


all: displayUsers_db.h displayTFS_UsersTutorial 

displayTFS_UsersTutorial: odir $OdisplayTFS_UsersTutorial$e

OBJS = $(GENERATED) \


EXE_LIBS = $(RDM_LIB) $(RDM_LIB) $(TFS_LIB) $(HTTP_LIB) $(ENC_LIB) $(BASE_LIB) $(TRANSPORT_LIB) $(TRANSPORT_LIB) $(PSP_LIB) $(MATH_LIB) 

$#MKEXE{prog=displayTFS_UsersTutorial, objs= $(OBJS) displayTFS_UsersTutorial_main$o, libs=$(EXE_LIBS)}

$#KEND{}
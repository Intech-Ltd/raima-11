# Auto generated please do not edit this file

#include $(RDM_HOME)$/sysmake$/$(KMAKESYS).mak

CC_DEFS = $DNEED_SETUP_DATABASE 
CC_INCL = $I../../../include 

#include $(RDM_HOME)$/kheader.mak
#include "OptionalKey.dp"

$#DATABASE{name=optional_key, carray=1, opts=-d}


all: optional_keyTutorial 

optional_keyTutorial: odir $Ooptional_keyTutorial$e

OBJS = $(GENERATED) \


EXE_LIBS = $(RDM_LIB) $(RDM_LIB) $(TFS_LIB) $(HTTP_LIB) $(ENC_LIB) $(BASE_LIB) $(TRANSPORT_LIB) $(TRANSPORT_LIB) $(PSP_LIB) $(MATH_LIB) 

$#MKEXE{prog=optional_keyTutorial, objs= $(OBJS) optional_keyTutorial_main$o, libs=$(EXE_LIBS)}

$#KEND{}
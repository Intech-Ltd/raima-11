# Auto generated please do not edit this file

#include $(RDM_HOME)$/sysmake$/$(KMAKESYS).mak

CC_DEFS = $DNEED_SETUP_DATABASE 
CC_INCL = $I../../../include 

#include $(RDM_HOME)$/kheader.mak
#include "DuplicateKey.dp"

$#DATABASE{name=duplicate_key, carray=1, opts=-d}


all: duplicate_keyTutorial 

duplicate_keyTutorial: odir $Oduplicate_keyTutorial$e

OBJS = $(GENERATED) \


EXE_LIBS = $(RDM_LIB) $(RDM_LIB) $(TFS_LIB) $(HTTP_LIB) $(ENC_LIB) $(BASE_LIB) $(TRANSPORT_LIB) $(TRANSPORT_LIB) $(PSP_LIB) $(MATH_LIB) 

$#MKEXE{prog=duplicate_keyTutorial, objs= $(OBJS) duplicate_keyTutorial_main$o, libs=$(EXE_LIBS)}

$#KEND{}
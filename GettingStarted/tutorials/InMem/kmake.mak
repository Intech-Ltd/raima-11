# Auto generated please do not edit this file

#include $(RDM_HOME)$/sysmake$/$(KMAKESYS).mak

CC_DEFS = $DNEED_SETUP_DATABASE 
CC_INCL = $I../../../include 

#include $(RDM_HOME)$/kheader.mak
#include "InMem.dp"

$#DATABASE{name=pure_in_memory, carray=1}


all: pure_in_memory.h pure_in_memoryTutorial 

pure_in_memoryTutorial: odir $Opure_in_memoryTutorial$e

OBJS = $(GENERATED) \


EXE_LIBS = $(RDM_LIB) $(RDM_LIB) $(TFS_LIB) $(HTTP_LIB) $(ENC_LIB) $(BASE_LIB) $(TRANSPORT_LIB) $(TRANSPORT_LIB) $(PSP_LIB) $(MATH_LIB) 

$#MKEXE{prog=pure_in_memoryTutorial, objs= $(OBJS) pure_in_memoryTutorial_main$o, libs=$(EXE_LIBS)}

$#KEND{}
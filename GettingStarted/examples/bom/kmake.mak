# Auto generated please do not edit this file

#include $(RDM_HOME)$/sysmake$/$(KMAKESYS).mak

CC_DEFS = $DNEED_SETUP_DATABASE 
CC_INCL = $I../../../include 

#include $(RDM_HOME)$/kheader.mak
#include "bom.dp"

$#DATABASE{name=bom, carray=1, opts=-d}


all: bomExample 

bomExample: odir $ObomExample$e

OBJS = $(GENERATED) \


EXE_LIBS = $(UTIL_LIB) $(RDM_LIB) $(RDM_LIB) $(DDL_LIB) $(TFS_LIB) $(HTTP_LIB) $(ENC_LIB) $(BASE_LIB) $(TRANSPORT_LIB) $(TRANSPORT_LIB) $(PSP_LIB) $(MATH_LIB) 

$#MKEXE{prog=bomExample, objs= $(OBJS) bomExample_main$o, libs=$(EXE_LIBS)}

$#KEND{}
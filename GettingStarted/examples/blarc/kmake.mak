# Auto generated please do not edit this file

#include $(RDM_HOME)$/sysmake$/$(KMAKESYS).mak
CC_INCL = $I../../../include 

#include $(RDM_HOME)$/kheader.mak
#include "blarc.dp"

$#DATABASE{name=blarc, carray=1, opts=-d}


all: blarcExample 

blarcExample: odir $OblarcExample$e

OBJS = $(GENERATED) \


EXE_LIBS = $(RDM_LIB) $(RDM_LIB) $(DDL_LIB) $(TFS_LIB) $(HTTP_LIB) $(ENC_LIB) $(BASE_LIB) $(TRANSPORT_LIB) $(TRANSPORT_LIB) $(PSP_LIB) $(MATH_LIB) 

$#MKEXE{prog=blarcExample, objs= $(OBJS) blarcExample_main$o, rc=blarcExample_main, libs=$(EXE_LIBS)}

$#KEND{}
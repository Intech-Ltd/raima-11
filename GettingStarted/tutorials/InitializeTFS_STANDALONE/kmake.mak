# Auto generated please do not edit this file

#include $(RDM_HOME)$/sysmake$/$(KMAKESYS).mak

CC_DEFS = $DNEED_SETUP_DATABASE 
CC_INCL = $I../../../include 

#include $(RDM_HOME)$/kheader.mak
#include "InitializeTFS_STANDALONE.dp"

$#DATABASE{name=initSTANDALONE_db, carray=1}


all: initSTANDALONE_db.h initSTANDALONE_Tutorial 

initSTANDALONE_Tutorial: odir $OinitSTANDALONE_Tutorial$e

OBJS = $(GENERATED) \


EXE_LIBS = $(RDM_LIB) $(RDM_LIB) $(TFS_LIB) $(HTTP_LIB) $(ENC_LIB) $(BASE_LIB) $(TRANSPORT_LIB) $(TRANSPORT_LIB) $(PSP_LIB) $(MATH_LIB) 

$#MKEXE{prog=initSTANDALONE_Tutorial, objs= $(OBJS) initSTANDALONE_Tutorial_main$o, libs=$(EXE_LIBS)}

$#KEND{}

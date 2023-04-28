# RDM
#
# Copyright (c) 2010 Raima Inc.,  All rights reserved.
#
# Use of this software, whether in source code format, or in executable,
# binary object code form, is governed by the Raima LICENSE which
# is fully described in the LICENSE.TXT file, included within this
# distribution of files.

# This file contains platform-independent definitions of things such as
# library names.  The idea is to defined a macro for a library name here,
# so that other makefiles can refer to it simply by name, rather than
# having to specify both a name and a location.

#ifndef RDM_HOME
#fatal Must set RDM_HOME in your environment
#endif

.SUFFIXES:
.SUFFIXES: .c .cpp .m $o $a .h .java .class

DBIN    = $(RDM_HOME)$/$(SYSDIR)$/bin$/
DLIB    = $(RDM_HOME)$/$(SYSDIR)$/lib$/

DEFDIR  = $(RDM_HOME)$/source$/def$/$(SYSDIR)$/
H       = $(RDM_HOME)$/include$/
O       = $(SYSDIR)$/

NOP     = @echo
DOCROOT = $(RDM_HOME)$/databases.$(SYSDIR)

#ifdef JAVA_HOME
#ifndef JDKPATH
JDKPATH = $(JAVA_HOME)$/bin$/
#endif
#endif

#ifdef HOSTDIR
U             = $(HOSTDIR)$/
RUN_TFSERVER  = echo "Not starting tfserver for hosted platforms"
STOP_TFSERVER = $(NOP)
#else
U             = $(SYSDIR)$/
RUN_TFSERVER  = $#IF_EXIST{file=$(DBIN)tfserver$e, do=$(DBIN)tfserver -start -d $(DOCROOT)}
STOP_TFSERVER = $#IF_EXIST{file=$(DBIN)tfserver$e, do=$(DBIN)tfserver -stop}
#endif

HBIN = $(RDM_HOME)$/$U$/bin$/

default: all ;

PREFIX = rdm
SUFFIX = 11

#let DLL $(LPFX)$(PREFIX)$(MODULE)$(SUFFIX)

#let DEF $(DEFDIR)$(PREFIX)$(MODULE)$(SUFFIX)

#let BASE        $(PREFIX)base$(SUFFIX)
#let CPPAPI      $(PREFIX)cppapi$(SUFFIX)
#let DBIMP       $(PREFIX)dbimp_tool$(SUFFIX)
#let DBEXP       $(PREFIX)dbexp$(SUFFIX)
#let DBGET       $(PREFIX)dbget_tool$(SUFFIX)
#let DATAMOVE    $(PREFIX)datamove$(SUFFIX)
#let MIRRORING   $(PREFIX)mirroring$(SUFFIX)
#let OBJCAPI     $(PREFIX)objcapi$(SUFFIX)
#let REPLICATION $(PREFIX)replication$(SUFFIX)
#let REPLICATIONSQL $(PREFIX)replicationsql$(SUFFIX)
#let DAL         $(PREFIX)dal_tool$(SUFFIX)
#let DDL         $(PREFIX)ddl$(SUFFIX)
#let E2SUTIL     $(PREFIX)e2sutil$(SUFFIX)
#let ENC         $(PREFIX)enc$(SUFFIX)
#let ENCKEY      $(PREFIX)enckey$(SUFFIX)
#let HA          $(PREFIX)ha$(SUFFIX)
#let HARPC       $(PREFIX)harpc$(SUFFIX)
#let HTTP        $(PREFIX)http$(SUFFIX)
#let HTTPMON     $(PREFIX)httpmon$(SUFFIX)
#let IDA         $(PREFIX)ida_tool$(SUFFIX)
#let REGODRV     $(PREFIX)regodrv_tool$(SUFFIX)
#let PSP         $(PREFIX)psp$(SUFFIX)
#let RDBC        $(PREFIX)rdbc$(SUFFIX)
#let RDM         $(PREFIX)rdm$(SUFFIX)
#let RSTEST      $(PREFIX)rstest$(SUFFIX)
#let RDMSQL_TOOL $(PREFIX)rdmsql_tool$(SUFFIX)
#let RDMSQLSERVER $(PREFIX)rdmsqlserver_tool$(SUFFIX)
#let REPLCLI     $(PREFIX)replcli$(SUFFIX)
#let SCHEMAXLATE $(PREFIX)schemaxlate_tool$(SUFFIX)
#let RSQL        $(PREFIX)rsql$(SUFFIX)
#let SBASE       $(PREFIX)sbase$(SUFFIX)
#let TFSADMIN    $(PREFIX)tfsadmin$(SUFFIX)
#let TFS         $(PREFIX)tfs$(SUFFIX)
#let TFSRVR      $(PREFIX)tfserver_tool$(SUFFIX)
#let TFSUSER     $(PREFIX)tfsuser_tool$(SUFFIX)
#let TRANSPORT   $(PREFIX)transport$(SUFFIX)
#let UTIL        $(PREFIX)util$(SUFFIX)
#let UTODBC      $(PREFIX)utodbc$(SUFFIX)
#let UTODRV      $(PREFIX)utodrv$(SUFFIX)
#let XML         $(PREFIX)xml$(SUFFIX)
#let QAFRAME     $(PREFIX)qaframe$(SUFFIX)
#let QAFRAMERSQL $(PREFIX)qaframersql$(SUFFIX)
#let QAFRAMERDBC $(PREFIX)qaframerdbc$(SUFFIX)
#let QAFRAMEODRV $(PREFIX)qaframeodrv$(SUFFIX)
#let QAFRAMEOBJC $(PREFIX)qaframeobjc$(SUFFIX)
#let SQLITE      sqlite3
#let INTERNAL_UTIL $(PREFIX)internal_util$(SUFFIX)

#ifdef USE_DASH_L
BASE_LIB        = -l$(BASE)
CPPAPI_LIB      = -l$(CPPAPI)
DBIMP_TOOL_LIB  = -l$(DBIMP)
DBEXP_LIB       = -l$(DBEXP)
DBGET_TOOL_LIB  = -l$(DBGET)
DATAMOVE_LIB    = -l$(DATAMOVE)
MIRRORING_LIB   = -l$(MIRRORING)
OBJCAPI_LIB     = -l$(OBJCAPI)
REPLICATION_LIB = -l$(REPLICATION)
REPLICATIONSQL_LIB = -l$(REPLICATIONSQL)
DAL_TOOL_LIB    = -l$(DAL)
DDL_LIB         = -l$(DDL)
E2SUTIL_LIB     = -l$(E2SUTIL)
ENC_LIB         = -l$(ENC)
ENCKEY_LIB      = -l$(ENCKEY)
HA_LIB          = -l$(HA)
HARPC_LIB       = -l$(HARPC)
HTTP_LIB        = -l$(HTTP)
HTTPMON_LIB     = -l$(HTTPMON)
IDA_TOOL_LIB    = -l$(IDA)
REGODRV_TOOL_LIB = -l$(REGODRV)
PSP_LIB         = -l$(PSP)
RDBC_LIB        = -l$(RDBC)
RDM_LIB         = -l$(RDM)
REPLCLI_LIB     = -l$(REPLCLI)
RSTEST_LIB      = -l$(RSTEST)
RDMSQL_TOOL_LIB = -l$(RDMSQL_TOOL)
RDMSQLSERVER_TOOL_LIB = -l$(RDMSQLSERVER)
SCHEMAXLATE_TOOL_LIB = -l$(SCHEMAXLATE)
RSQL_LIB        = -l$(RSQL)
SBASE_LIB       = -l$(SBASE)
TFSADMIN_LIB    = -l$(TFSADMIN)
TFS_LIB         = -l$(TFS)
TFSERVER_TOOL_LIB = -l$(TFSRVR)
TFSUSER_TOOL_LIB = -l$(TFSUSER)
TRANSPORT_LIB   = -l$(TRANSPORT)
UTIL_LIB        = -l$(UTIL)
UTODBC_LIB      = -l$(UTODBC)
UTODRV_LIB      = -l$(UTODRV)
XML_LIB         = -l$(XML)
QAFRAME_LIB     = -l$(QAFRAME)
QAFRAMERSQL_LIB = -l$(QAFRAMERSQL)
QAFRAMERDBC_LIB = -l$(QAFRAMERDBC)
QAFRAMEODRV_LIB = -l$(QAFRAMEODRV)
QAFRAMEOBJC_LIB = -l$(QAFRAMEOBJC)
SQLITE_LIB      = -l$(SQLITE)
INTERNAL_UTIL_LIB = -l$(INTERNAL_UTIL)
#else
BASE_LIB        = $(DLIB)$(LPFX)$(BASE)$l
CPPAPI_LIB      = $(DLIB)$(LPFX)$(CPPAPI)$l
DBIMP_TOOL_LIB  = $(DLIB)$(LPFX)$(DBIMP)$l
DBEXP_LIB       = $(DLIB)$(DBEXP)$l
DBGET_TOOL_LIB  = $(DLIB)$(DBGET)$l
DAL_TOOL_LIB    = $(DLIB)$(LPFX)$(DAL)$l
DDL_LIB         = $(DLIB)$(LPFX)$(DDL)$l
DATAMOVE_LIB    = $(DLIB)$(LPFX)$(DATAMOVE)$l
MIRRORING_LIB   = $(DLIB)$(LPFX)$(MIRRORING)$l
REPLICATION_LIB = $(DLIB)$(LPFX)$(REPLICATION)$l
REPLICATIONSQL_LIB = $(DLIB)$(LPFX)$(REPLICATIONSQL)$l
E2SUTIL_LIB     = $(DLIB)$(LPFX)$(E2SUTIL)$l
ENC_LIB         = $(DLIB)$(LPFX)$(ENC)$l
ENCKEY_LIB      = $(DLIB)$(LPFX)$(ENCKEY)$l
HA_LIB          = $(DLIB)$(LPFX)$(HA)$l
HARPC_LIB       = $(DLIB)$(LPFX)$(HARPC)$l
HTTP_LIB        = $(DLIB)$(LPFX)$(HTTP)$l
HTTPMON_LIB     = $(DLIB)$(LPFX)$(HTTPMON)$l
IDA_TOOL_LIB    = $(DLIB)$(LPFX)$(IDA)$l
REGODRV_TOOL_LIB = $(DLIB)$(LPFX)$(REGODRV)$l
PSP_LIB         = $(DLIB)$(LPFX)$(PSP)$l
RDBC_LIB        = $(DLIB)$(LPFX)$(RDBC)$l
RDM_LIB         = $(DLIB)$(LPFX)$(RDM)$l
REPLCLI_LIB     = $(DLIB)$(LPFX)$(REPLCLI)$l
RSTEST_LIB      = $(DLIB)$(LPFX)$(RSTEST)$l
RDMSQL_TOOL_LIB = $(DLIB)$(LPFX)$(RDMSQL_TOOL)$l
RDMSQLSERVER_TOOL_LIB = $(DLIB)$(LPFX)$(RDMSQLSERVER)$l
SCHEMAXLATE_TOOL_LIB = $(DLIB)$(LPFX)$(SCHEMAXLATE)$l
RSQL_LIB        = $(DLIB)$(LPFX)$(RSQL)$l
SBASE_LIB       = $(DLIB)$(LPFX)$(SBASE)$l
TFSADMIN_LIB    = $(DLIB)$(LPFX)$(TFSADMIN)$l
TFS_LIB         = $(DLIB)$(LPFX)$(TFS)$l
TFSERVER_TOOL_LIB = $(DLIB)$(LPFX)$(TFSRVR)$l
TFSUSER_TOOL_LIB = $(DLIB)$(LPFX)$(TFSUSER)$l
TRANSPORT_LIB   = $(DLIB)$(LPFX)$(TRANSPORT)$l
UTIL_LIB        = $(DLIB)$(LPFX)$(UTIL)$l
UTODBC_LIB      = $(DLIB)$(LPFX)$(UTODBC)$l
UTODRV_LIB      = $(DLIB)$(LPFX)$(UTODRV)$l
XML_LIB         = $(DLIB)$(LPFX)$(XML)$l
QAFRAME_LIB     = $(DLIB)$(LPFX)$(QAFRAME)$l
QAFRAMERSQL_LIB = $(DLIB)$(LPFX)$(QAFRAMERSQL)$l
QAFRAMERDBC_LIB = $(DLIB)$(LPFX)$(QAFRAMERDBC)$l
QAFRAMEODRV_LIB = $(DLIB)$(LPFX)$(QAFRAMEODRV)$l
QAFRAMEOBJC_LIB = $(DLIB)$(LPFX)$(QAFRAMEOBJC)$l
SQLITE_LIB      = $(DLIB)$(LPFX)$(SQLITE)$l
INTERNAL_UTIL_LIB = $(DLIB)$(LPFX)$(INTERNAL_UTIL)$l
#endif

JNI_JAR = $(DLIB)rdm.jni.jar

DDLP      = ddlp
SDDLP     = sddlp
INITDB    = initdb
IMPORT    = dbimp
EMBEDFILE = $(HBIN)embedfile
RDMSQL    = rdmsql

#ifdef DEBUG
LMODE  = $(LDEBUG)
CCMODE = $(CCNOOPT) $(CCSYMBOLS)
CSCMODE = $(CSCDEBUG)
#else
LMODE  =
CCMODE = $(CCOPT) $DRELEASE
CSCMODE = $(CSCOPTIMIZE) $(CSCSYMBOLS)
#endif

#ifdef RDM_NO_DBUNIONS
UNION_DEF = $DRDM_NO_DBUNIONS
#endif

COPTS = $(CCONLY) $(CCMODE) $(CCDEST) $(CCDLL) $(UNION_DEF) $(CC_DEFS) $(CC_INCL) $(OSDEFS) $D$(OS) $I$(RDM_HOME)$/include
LOPTS = $(LMODE) $(OSLIBS)
CSCOPTS = $(CSCFLAGS) $(CSCMODE) $(CSCPLAT)

## --------------------------------------------------------------------------
## Rename a file or add definitions or include subdirectories
##

#define CC_RENAME{src,dest,defs,inc}
$(dest)$o: $(src).c
    $(SYSCC) $(CFLAGS) $(COPTS) $(inc) $(defs) $(src).c $(CCEND)

#edef

#include $(RDM_HOME)$/kmacros$/kversion.mak
#include $(RDM_HOME)$/kmacros$/embed.mak
#include $(RDM_HOME)$/kmacros$/dbfiles.mak
#include $(RDM_HOME)$/kmacros$/database.mak
#include $(RDM_HOME)$/kmacros$/sqldatabase.mak

#define PREBUILD{target}
    $#IF_EXIST{file=kprebuild.mak, do=kmake -rf kprebuild.mak $(target)}
#edef

## --------------------------------------------------------------------------
## User-defined inference rules:
##

.c{$[d,$O]}$o:
    $(SYSCC) $(CFLAGS) $(COPTS) $< $(CCEND)

.cpp{$[d,$O]}$o:
    $(SYSCPP) $(CPPFLAGS) $(COPTS) $< $(CCEND)

.m{$[d,$O]}$o:
    $(SYSOBJC) $(OBJCFLAGS) $(COPTS) $< $(CCEND)

.java.class:
	"$(JDKPATH)javac" -Xlint -classpath $(RDM_HOME)$/source$/jdbc$:$(EXTRA_CP) $<

odir:
    $#MKDIR{dir=$(SYSDIR)}

ddir:
    $#MKDIR{dir=$(DOCROOT)}

local_clean:
	$(NOP)

#define KEND()
clean: $(AUTO_CLEAN_TARGETS)
	$#CLEAN_DIR{dir=$(SYSDIR)}

cleanall clean_all: clean local_clean $(AUTO_CLEANALL_TARGETS)

rebuild: cleanall all

embedfiles: $(EMBEDFILE_TARGETS)
    $(NOP)
#edef

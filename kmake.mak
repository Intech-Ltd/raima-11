# RDM
#
# Copyright (c) 2010 Raima Inc.,  All rights reserved.
#
# Use of this software, whether in source code format, or in executable,
# binary object code form, is governed by the Raima LICENSE which
# is fully described in the LICENSE.TXT file, included within this
# distribution of files.

#include $(RDM_HOME)$/sysmake$/$(KMAKESYS).mak
#include $(RDM_HOME)$/kheader.mak

all: krun Source BuildExamples

Source: headers
#ifexist $(RDM_HOME)$/source
    krun $(RDM_HOME)$/source                        kmake -rf kmake.mak
#endif

host: headers
#ifexist $(RDM_HOME)$/source
    krun $(RDM_HOME)$/source                        kmake -rf kmake.mak host
#endif

BuildExamples: ddir
    krun $(RDM_HOME)$/httpdocs                      kmake -rf kmake.mak

kmake: outdir krun
     krun $(RDM_HOME)$/kmake                        kmake -rf kmake.mak

headers: outdir
    $#PREBUILD{target=headers}

clean: krun
    -$(STOP_TFSERVER)
    krun $(RDM_HOME)$/examples                      kmake -rf kmake.mak $@
    krun $(RDM_HOME)$/httpdocs                      kmake -rf kmake.mak $@
#ifexist $(RDM_HOME)$/source
    krun $(RDM_HOME)$/source                        kmake -rf kmake.mak $@
#endif
#    krun $(RDM_HOME)$/lib                           kmake -rf kmake.mak $@
#    krun $(RDM_HOME)$/bin                           kmake -rf kmake.mak $@
    $#PREBUILD{target=$@}

cleanall clean_all: krun
    -$(STOP_TFSERVER)
    krun $(RDM_HOME)$/examples                      kmake -rf kmake.mak $@
    krun $(RDM_HOME)$/httpdocs                      kmake -rf kmake.mak $@
#ifexist $(RDM_HOME)$/source
    krun $(RDM_HOME)$/source                        kmake -rf kmake.mak $@
#endif
#    $#CLEAN_DIR{dir=$(DOCROOT)}
    $#PREBUILD{target=$@}

embedfiles: ddir outdir krun
    $#PREBUILD{target=$@}
    krun $(RDM_HOME)$/test                          kmake -rf kmake.mak $@

outdir:
    $#MKDIR{dir=$(SYSDIR)}
    $#MKDIR{dir=$(DBIN)}
    $#MKDIR{dir=$(DLIB)}

krun:
    $#SETEXECPERM{file=bin$/krun}


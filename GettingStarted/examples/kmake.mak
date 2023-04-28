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

all Examples:
    krun blarc               kmake -rf kmake.mak
    krun bom                 kmake -rf kmake.mak
    krun cppbom              kmake -rf kmake.mak
    krun market              kmake -rf kmake.mak
    krun poi                 kmake -rf kmake.mak
    krun rbom                kmake -rf kmake.mak
#ifexist replicationMaster
    krun replicationMaster   kmake -rf kmake.mak
#endif
#ifexist replicationSlave
    krun replicationSlave    kmake -rf kmake.mak
#endif
    krun sbomODBC            kmake -rf kmake.mak
    krun sbomRSQL            kmake -rf kmake.mak
    krun songs               kmake -rf kmake.mak
    krun tims                kmake -rf kmake.mak
#ifndef HOSTDIR
    tfsuser -docroot $(DOCROOT) -u admin -p secret
#endif

clean cleanall clean_all:
    krun blarc               kmake -rf kmake.mak $@
    krun cppbom              kmake -rf kmake.mak $@
    krun market              kmake -rf kmake.mak $@
    krun poi                 kmake -rf kmake.mak $@
    krun rbom                kmake -rf kmake.mak $@
#ifexist replicationMaster
    krun replicationMaster   kmake -rf kmake.mak $@
#endif
#ifexist replicationSlave
    krun replicationSlave    kmake -rf kmake.mak $@
#endif
    krun sbomODBC            kmake -rf kmake.mak $@
    krun sbomRSQL            kmake -rf kmake.mak $@
    krun songs               kmake -rf kmake.mak $@
    krun tims                kmake -rf kmake.mak $@

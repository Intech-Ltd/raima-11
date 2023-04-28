# Raima Database Manager
#
# Copyright (c) 2010 Raima Inc.,  All rights reserved.
#
# Use of this software, whether in source code format, or in executable,
# binary object code form, is governed by the Raima LICENSE which
# is fully described in the LICENSE.TXT file, included within this
# distribution of files.

#define RSQL{name}
#let name $(name)
#let GENERATED $(GENERATED) $(name)_cat$o $(name)_dbd$o

$(name).h $(name)_cat.h $(name)_cat.c $(name)_dbd.h $(name)_dbd.c: $(name).sdl
    $(RDMSQL) -B -c $(name).sdl

#let AUTO_CLEAN_TARGETS $(AUTO_CLEAN_TARGETS) clean_$(name)
#let AUTO_CLEANALL_TARGETS $(AUTO_CLEANALL_TARGETS) cleanall_$(name)
clean_$(name):
    $#DELFILE{file=$(name).h}
    $#DELFILE{file=$(name)_dbd.c}
    $#DELFILE{file=$(name)_dbd.h}
    $#DELFILE{file=$(name)_cat.c}
    $#DELFILE{file=$(name)_cat.h}

cleanall_$(name):
    $#CLEAN_DIR{dir=$(DOCROOT)$/$(name)}
#edef

#define RSQL_SSP{name}
#let name $(name)
#let GENERATED $(GENERATED) $(name)_cat$o $(name)_dbd$o $(name)_ssp$o

$(name).h $(name)_cat.h $(name)_cat.c $(name)_dbd.h $(name)_dbd.c $(name)_ssp.c $(name)_ssp.h: $(name).sdl
    $(RDMSQL) -B -c $(name).sdl

#let AUTO_CLEAN_TARGETS $(AUTO_CLEAN_TARGETS) clean_$(name)
#let AUTO_CLEANALL_TARGETS $(AUTO_CLEANALL_TARGETS) cleanall_$(name)
clean_$(name):
    $#DELFILE{file=$(name).h}
    $#DELFILE{file=$(name)_dbd.c}
    $#DELFILE{file=$(name)_dbd.h}
    $#DELFILE{file=$(name)_cat.c}
    $#DELFILE{file=$(name)_cat.h}
    $#DELFILE{file=$(name)_ssp.c}
    $#DELFILE{file=$(name)_ssp.h}

cleanall_$(name):
    $#CLEAN_DIR{dir=$(DOCROOT)$/$(name)}
#edef

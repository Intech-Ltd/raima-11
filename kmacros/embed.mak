# Raima Database Manager
#
# Copyright (c) 2010 Raima Inc.,  All rights reserved.
#
# Use of this software, whether in source code format, or in executable,
# binary object code form, is governed by the Raima LICENSE which
# is fully described in the LICENSE.TXT file, included within this
# distribution of files.

#define EMBED{name,files,opts}

#let GENERATED $(GENERATED) $(name)$o
#let AUTO_CLEAN_TARGETS $(AUTO_CLEAN_TARGETS) clean_$(name)
#let EMBEDFILE_TARGETS $(EMBEDFILE_TARGETS) embedfile_$(name)

#ifb $(files)
._l$(name)=$(name)
#else
._l$(name)=$(files)
#endif

#ifb $(opts)
._l$(opts)=$(opts)
#endif

$(name).c $(name).h: $(._l${name})
    $#IF_EXIST{file=$(EMBEDFILE)$e, do=$(EMBEDFILE) $(opts) -o $(name) $(._l${name})}
embedfile_$(name): $(name).c $(name).h
clean_$(name):
    $#DELFILE(file=$(name).c)
    $#DELFILE(file=$(name).h)

#edef


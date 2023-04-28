# Raima Database Manager
#
# Copyright (c) 2010 Raima Inc.,  All rights reserved.
#
# Use of this software, whether in source code format, or in executable,
# binary object code form, is governed by the Raima LICENSE which
# is fully described in the LICENSE.TXT file, included within this
# distribution of files.

#define DATABASE{name,carray,dbd,embed,opts,files,initdb,dbimp,dbimpxml,cpp,objc}
#let name $(name)

#ifdef HOSTDIR
dbd=
#endif

#ifnb $(embed)
#let GENERATED $(GENERATED) embed_$(name)_dbd$o
$#EMBED(name=$(name).ddl)
._emb$(name)=-e -nd
#else
# ._emb$(name)=
#endif

#ifnb $(cpp)
#let GENERATED $(GENERATED) $(name)_api$o
._cpp$(name)=-cpp
#else
# ._cpp$(name)=
#endif

#ifnb $(objc)
#let GENERATED $(GENERATED) $(name)_objc$o $(name)_dbd$o
._objc$(name)=-objc
#else
# ._objc$(name)=
#endif

#ifnb $(carray)           #
#let GENERATED $(GENERATED) $(name)_dbd$o

#ifnb $(dbd)                 #
._opt$(name)=-c
._dbd$(name)=$(DOCROOT)$/$(name)$/$(name).dbd
#else                        #
._opt$(name)=-cn
#endif                       #
#else                    #

#ifndef HOSTDIR              #
._dbd$(name)=$(DOCROOT)$/$(name)$/$(name).dbd
# ._opt$(name)=
#else                        #
#ifb $(embed)
._opt$(name)=-nd
#endif
#endif                       #

#endif                   #

$(name).h embed_$(name)_dbd.c embed_$(name)_dbd.h $(name)_dbd.c $(name)_dbd.h $(._dbd${name}) $(name)_api.cpp $(name)_api.h $(name)_objc.h $(name)_objc.m: $(name).ddl
    $(DDLP) $(._opt${name}) $(._cpp${name}) $(._objc${name}) $(._emb${name}) $(opts) $(name).ddl
#ifnb $(initdb)
    $(INITDB) -y $(name)
#endif
#ifnb $(dbimp)
    $(IMPORT) -q $(name).imp
#endif
#ifnb $(dbimpxml)
    $(IMPORT) -q -xml $(name).xml
#endif

#let AUTO_CLEAN_TARGETS $(AUTO_CLEAN_TARGETS) clean_$(name)
#let AUTO_CLEANALL_TARGETS $(AUTO_CLEANALL_TARGETS) cleanall_$(name)
clean_$(name):
    $#DELFILE{file=$(name).h}
    $#DELFILE{file=$(name)_dbd.c}
    $#DELFILE{file=$(name)_dbd.h}
    $#DELFILE{file=$(DOCROOT)$/$(name)$/$(name).synchronous}
#ifnb $(dbd)
    $#DELFILE{file=$(._dbd${name})}
#endif
#ifnb $(embed)
    $#DELFILE(file=embed_$(name)_dbd.c)
    $#DELFILE(file=embed_$(name)_dbd.h)
#endif
#ifnb $(cpp)
    $#DELFILE(file=$(name)_api.cpp)
    $#DELFILE(file=$(name)_api.h)
#endif
#ifnb $(objc)
    $#DELFILE(file=$(name)_objc.h)
    $#DELFILE(file=$(name)_objc.m)
#endif

#ifnb $(files)
$#DBFILES{name=$(name),files=$(files),dbimp=$(dbimp)}

#endif

cleanall_$(name):
    $#CLEAN_DIR{dir=$(DOCROOT)$/$(name)}

#edef

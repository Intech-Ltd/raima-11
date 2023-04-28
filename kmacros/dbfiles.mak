# Raima Database Manager
#
# Copyright (c) 2010 Raima Inc.,  All rights reserved.
#
# Use of this software, whether in source code format, or in executable,
# binary object code form, is governed by the Raima LICENSE which
# is fully described in the LICENSE.TXT file, included within this
# distribution of files.

#define DBFILES{name,files,dbimp}

#let name $(name)
._dbfiles1$(name)=$(DOCROOT)$/$(name)

#let $(name)_DB $(DOCROOT)$/$(name)

#ifdef RDM_TFSERVER

#let ._dbfiles2$(name) $(name)_FILES

#else

#for elem in $(files)

#let ._dbfiles2$(name) ${._dbfiles2$(name)} ${._dbfiles1$(name)}$/$(elem)

#efor

#endif

#ifndef HOSTDIR
#let BUILD_DATABASES $(BUILD_DATABASES) ${._dbfiles2$(name)}
#endif

${._dbfiles2$(name)}: ${._dbfiles1$(name)}$/$(name).dbd
	$(INITDB) -y $(name)
#ifnb $(dbimp)
    $(IMPORT) -q $(dbimp)
#endif

#edef

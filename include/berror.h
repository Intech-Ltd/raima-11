/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2009 Birdstep Technology Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Birdstep LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

#include "rdm.h"
#include "dberr.h"
const char * EXTERNAL_FCN b_errorstr(int32_t errnum);
const char * EXTERNAL_FCN b_errorname(int32_t errnum);
const char * EXTERNAL_FCN b_errorstrEx(int32_t errnum, int32_t num_errors, const RDM_ERROR[]);
const char * EXTERNAL_FCN b_errornameEx(int32_t errnum, int32_t num_errors, const RDM_ERROR[]);


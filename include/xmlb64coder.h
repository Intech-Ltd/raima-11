/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2010 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

#ifndef XMLB64CODER_H
#define XMLB64CODER_H

uint32_t EXTERNAL_FCN xml_encode(const uint8_t *, size_t, char *, size_t);
uint32_t EXTERNAL_FCN xml_decode(const char *, size_t, uint8_t *, size_t);
#endif


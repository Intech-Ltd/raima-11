/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2001-2008 Birdstep Technology Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Birdstep LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

#if !defined(__GETNAMES_H)
#define __GETNAMES_H

extern uint16_t getrec(const char *, const DB_ENTRY *);
extern uint16_t getset(const char *, const DB_ENTRY *);
extern uint32_t getfld(const char *, uint32_t, const DB_ENTRY *);
extern int32_t rec_const(uint16_t);
extern int32_t set_const(uint16_t);
extern int32_t fld_const(uint32_t, const DB_ENTRY *);

#endif  /* __GETNAMES_H */


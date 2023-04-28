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

#ifndef OPTIONS_H
#define OPTIONS_H

#define DEFAULT_OPTS        (DB_ULONG) (DCHAINUSE)

/*
    some options cannot be changed once a database has been opened
*/
#define OPEN_OPTS        (DB_ULONG) (IGNOREENV | NORECOVER | PREALLOC_CACHE)

#endif  /* OPTIONS_H */

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

#if !defined(ENCPRIV_H)
#define ENCPRIV_H

#include "psp.h"

#if defined(__cplusplus)
extern "C" {
#endif


#define ENC_FAST         1
#define SHA_DIGESTSIZE  20
#define CHARSPERKEY      8

#define INI_ACCOUNTS_SECTION    "Accounts"


typedef struct {
    PSP_MEMTAG tag;
    void     *aes;
    uint32_t  blocksize;
    uint8_t   key[CHARSPERKEY];
} ENCCB;

ENCCB   *EXTERNAL_FCN enc_init(int, const uint8_t *, PSP_MEMTAG);
void     EXTERNAL_FCN enc_encrypt(const ENCCB *, void *, uint32_t);
void     EXTERNAL_FCN enc_decrypt(const ENCCB *, void *, uint32_t);
void     EXTERNAL_FCN enc_term(const ENCCB *);

uint8_t *EXTERNAL_FCN enc_syskey(uint8_t, uint8_t *, uint32_t);

void     EXTERNAL_FCN sha_encrypt(const void *, uint32_t, void *);


#if defined(__cplusplus)
}
#endif

#endif /* ENCPRIV_H */


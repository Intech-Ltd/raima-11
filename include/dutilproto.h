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

#ifndef DUTILPROTO_H
#define DUTILPROTO_H

#include "base.h"
#include "utilities.h"

int32_t EXTERNAL_FCN dal_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN datdump_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbcheck_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbcluster_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbdefrag_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbget_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbgen_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbexp_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbimp_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbrev_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbswap_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dchain_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN ddlp_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN initdb_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbcollapse_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbchkpt_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN keybuild_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN keydump_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN keypack_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN lmclear_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN prdbd_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN rdmsql_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN tfsutil_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN tfserver_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN tfsuser_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbmir_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbrepair_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbrep_main(int32_t argc, const char *const *argv);
int32_t EXTERNAL_FCN dbrepsql_main(int32_t argc, const char *const *argv);

int32_t EXTERNAL_FCN d_datdump(const DATDUMP_PARAMS*, DB_TASK*, int32_t);
int32_t EXTERNAL_FCN d_dbcheck(const DBCHECK_PARAMS*, uint32_t *, uint32_t *, DB_TASK*, int32_t);
int32_t EXTERNAL_FCN d_dbdefrag(const DBDEFRAG_PARAMS*, DB_TASK*, int32_t);
int32_t EXTERNAL_FCN d_dbcluster(const DBCLUSTER_PARAMS*, DB_TASK*, int32_t);
int32_t EXTERNAL_FCN d_dbexpcsv(const DBEXPCSV_PARAMS*, DB_TASK*, int32_t);
int32_t EXTERNAL_FCN d_dbexpxml(const DBEXPXML_PARAMS*, DB_TASK*, int32_t);
int32_t EXTERNAL_FCN d_dbgen(const DBGEN_PARAMS*, DB_TASK*, int32_t);
int32_t EXTERNAL_FCN d_dbrepair(const DBREPAIR_PARAMS*, DB_TASK*, int32_t);
int32_t EXTERNAL_FCN d_dbrevise(const DBREVISE_PARAMS*, DB_TASK*);
int32_t EXTERNAL_FCN d_dbswap(const char *, const DBSWAP_PARAMS*);
int32_t EXTERNAL_FCN d_dchain(const DCHAIN_PARAMS*, DB_TASK*, int32_t);
int32_t EXTERNAL_FCN d_ddlp(const DDLP_PARAMS*, int32_t *num_errors, int32_t *num_warnings);
int32_t EXTERNAL_FCN d_keybuild(DB_TASK *, int32_t);
int32_t EXTERNAL_FCN d_keyfilebuild(DB_TASK *, int32_t, const char *);
int32_t EXTERNAL_FCN d_keybuildEx(const KEYBUILD_PARAMS*, DB_TASK*, int32_t);
int32_t EXTERNAL_FCN d_keydump(const KEYDUMP_PARAMS*, DB_TASK*, int32_t);

#if !defined(RDM_NO_UNICODE)
int32_t EXTERNAL_FCN d_datdumpW(const DATDUMP_PARAMSW *, DB_TASK *, int32_t);
int32_t EXTERNAL_FCN d_dbcheckW(const DBCHECK_PARAMSW *, uint32_t *, uint32_t *, DB_TASK *, int32_t);
int32_t EXTERNAL_FCN d_dbdefragW(const DBDEFRAG_PARAMSW *, DB_TASK *, int32_t);
int32_t EXTERNAL_FCN d_dbclusterW(const DBCLUSTER_PARAMSW *, DB_TASK *, int32_t);
int32_t EXTERNAL_FCN d_dbexpcsvW(const DBEXPCSV_PARAMSW *, DB_TASK *, int32_t);
int32_t EXTERNAL_FCN d_dbexpxmlW(const DBEXPXML_PARAMSW *, DB_TASK *, int32_t);
int32_t EXTERNAL_FCN d_dbgenW(const DBGEN_PARAMSW *, DB_TASK *, int32_t);
int32_t EXTERNAL_FCN d_dchainW(const DCHAIN_PARAMSW *, DB_TASK *, int32_t);
int32_t EXTERNAL_FCN d_dbreviseW(const DBREVISE_PARAMSW *, DB_TASK *);
int32_t EXTERNAL_FCN d_dbswapW(const wchar_t *, const DBSWAP_PARAMSW *);
int32_t EXTERNAL_FCN d_ddlpW(const DDLP_PARAMSW*, int32_t *num_errors, int32_t *num_warnings);
int32_t EXTERNAL_FCN d_keybuildExW(const KEYBUILD_PARAMSW *, DB_TASK *, int32_t);
int32_t EXTERNAL_FCN d_keydumpW(const KEYDUMP_PARAMSW *, DB_TASK *, int32_t);
int32_t EXTERNAL_FCN d_keyfilebuildW(DB_TASK *, int32_t, const wchar_t *);

int32_t EXTERNAL_FCN dal_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN datdump_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbcheck_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbcluster_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbdefrag_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbget_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbgen_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbexp_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbimp_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbrev_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbswap_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dchain_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN ddlp_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN initdb_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbcollapse_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbchkpt_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN keybuild_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN keydump_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN keypack_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN lmclear_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN prdbd_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN rdmsql_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN tfsutil_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN tfserver_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN tfsuser_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbmir_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbrepair_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbrep_mainW(int32_t argc, const wchar_t *const *argv);
int32_t EXTERNAL_FCN dbrepsql_mainW(int32_t argc, const wchar_t *const *argv);
#endif

#endif /* ! DUTILPROTO_H */

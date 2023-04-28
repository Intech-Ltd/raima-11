/*===========================================================================
    vtfcns.c -- Sample C-based virtual table access function in
                  RDM SQL.

    C-based functions needed to implement virtual tables.
    
    File vtabs.sql contains the DDL for the database used in this example
    containing the following declaration:
    
    create virtual table virtab(
        pkey integer primary key,
        name char(24),
        addr char(32),
        city char(24),
        state char(2),
        zip char(10)
    );

    To register this virtual table with RDM SQL call the following
    function BEFORE opening the vtabs database:

    RSQL_ERRCODE EXTERNAL_FCN rsqlRegisterVirtualTables(
        HCONN               hConn,        in:  connection handle
        const char         *dbname,       in:  name of db
        uint16_t            novts,        in:  # of virtual tables (0 => NULL terminated vtftab)
        const VTFLOADTABLE *vtftab,       in:  ptr to VTF load table
        void               *pRegCtx,      in:  ptr to user's registration context
        const size_t        szFetchCtx)   in:  size of fetch context space to be alloc'd
        
    e.g.,
        extern const VTFLOADTABLE vtFcnTable[];
            ...
        rsqlRegisterVirtualTables(hdbc, "vtabs", 1, vtFcnTable, NULL, vtFetchSz);
 
    Copyright � 2011 Raima Inc., All rights reserved.
==========================================================================*/
#include <stdio.h>
#include <string.h>
/*lint ++flb */
#include "rdmsql.h"
/*lint --flb */

/*--------------------------------------------------------------------------
    Virtual table context data structure
---------------------------------------------------------------------------*/
typedef struct vtab_ctx {
    uint64_t      rowcnt;       /* count of rows fetched */
    uint64_t      rowno;        /* number of next row to be fetched */
    RSQL_VALUE   *pkeyval;      /* ptr to primary key's value */
} VTAB_CTX;

struct virtab {
    int32_t    pkey;
    char       name[25];
    char       addr[33];
    char       city[24];
    char       state[3];
    char       zip[10];
    int8_t     is_null[6];
};

/*lint -save -e956 */
static PSP_SEM         vtsem = NO_PSP_SEM;
static const uint32_t  maxrows = 1000;
static struct virtab  *vtrows = NULL;
static uint32_t        norows = 0;
/*lint -restore */

/*--------------------------------------------------------------------------
    Function prototypes
---------------------------------------------------------------------------*/
/*lint -esym(403,vtab*) */
static VTINSERT      vtInsert;
static VTROWCOUNT    vtRowCount;
static VTSELECTCOUNT vtSelectCount;
static VTSELECTOPEN  vtSelectOpen;
static VTFETCH       vtFetch;

/*--------------------------------------------------------------------------
    Table of virtual tables function table entries
---------------------------------------------------------------------------*/
const VTFLOADTABLE vtFcnTable[] = {
    {"virtab", vtInsert, vtRowCount, vtSelectCount, vtSelectOpen, vtFetch, NULL},
    {""}
};
const size_t vtFetchSz = sizeof(VTAB_CTX);

/* ========================================================================
    Enter serialized access to vtrows data 
*/
static void vtEnter()
{
    if ( vtsem == NO_PSP_SEM ) {
        psp_enterCritSec();
        if ( vtsem == NO_PSP_SEM )
            vtsem = psp_syncCreate(PSP_MUTEX_SEM);
        psp_exitCritSec();
    }
    psp_syncEnterExcl(vtsem);
}

/* ========================================================================
    Exit serialized access to vtrows data 
*/
static void vtExit()
{
    psp_syncExitExcl(vtsem);
}

/* ========================================================================
    Virtual table row count function 
*/
static RSQL_ERRCODE EXTERNAL_FCN vtRowCount(  /* vtRowCount() */
    HSTMT            hstmt,   /* in:  statement handle */
    void            *pRegCtx, /* in:  unused */
    uint64_t        *pNoRows) /* out: ptr to row count value */
/*
    Called by SQL optimizer to retrieve (estimate of) number of rows in the table.
*/
{
    UNREF_PARM(hstmt)
    UNREF_PARM(pRegCtx)
    
    vtEnter();
    *pNoRows = (uint64_t)norows;
    vtExit();

    return errSUCCESS;
} /*lint !e715*/

/* ========================================================================
    Virtual table SELECT COUNT(*) function 
*/
static RSQL_ERRCODE EXTERNAL_FCN vtSelectCount(  /* vtSelectCount() */
    HSTMT            hstmt,   /* in:  statement handle */
    void            *pRegCtx, /* in:  unused */
    void            *pvCtx,   /* in:  context pointer */
    uint64_t        *pNoRows) /* out: ptr to row count value */
/*
    Called by SQL to process "SELECT COUNT(*) from virtab".
*/
{
    UNREF_PARM(pvCtx)
    
    return vtRowCount(hstmt, pRegCtx, pNoRows);
} /*lint !e715*/

/* ========================================================================
    Store column values in specified row (0 = first row)
*/
static RSQL_ERRCODE vtStoreRow(
    uint32_t         rowno,        /* in:  row number into which store col vals */
    uint16_t         nocols,       /* in:  no. of ref'd columns */
    const VCOL_INFO *colsvals)     /* in:  array of ref'd column value containers */
{
    uint16_t         cno;
    const VCOL_INFO *pCol;
    struct virtab   *pRow;
    
    if ( rowno >= maxrows )
        return errVTSPACE;
        
    pRow = &vtrows[rowno];
    
    for (pCol = colsvals, cno = 0; cno < nocols; ++cno, ++pCol ) {
        if ( *pCol->is_null )
            pRow->is_null[pCol->colno] = 1;
        else {
            pRow->is_null[pCol->colno] = 0;
            switch (pCol->colno) {
                case 0: memcpy(&pRow->pkey, pCol->data, sizeof(int32_t)); break;
                case 1: strncpy(pRow->name,  (char *)pCol->data, 24);     break;
                case 2: strncpy(pRow->addr,  (char *)pCol->data, 32);     break;
                case 3: strncpy(pRow->city,  (char *)pCol->data, 24);     break;
                case 4: strncpy(pRow->state, (char *)pCol->data, 2);      break;
                case 5: strncpy(pRow->zip,   (char *)pCol->data, 9);      break;
            } /*lint !e744 */
        }
    }
    return errSUCCESS;
}

/* ========================================================================
    Virtual table INSERT execution function 
*/
static RSQL_ERRCODE EXTERNAL_FCN vtInsert(  /* vtInsert() */
    HSTMT        hstmt,        /* in:  statement handle */
    uint16_t     nocols,       /* in:  no. of ref'd columns */
    VCOL_INFO   *colsvals,     /* in:  array of ref'd column value containers */
    void        *pRegCtx)      /* in:  unused */
/*  
    Called by SQL when INSERT statement is executed (rsqlExecute).
    
    The new column values are contained in colvals.

    The pRegCtx contains the pointer specified on the 
    rsqlRegisterVirtualTables call.
    
    Returns errSUCCESS or errDUPLICATE error code.  
*/
{
    RSQL_ERRCODE stat = errSUCCESS;
    RSQL_VALUE   pkey, *pkeyval = NULL;
    int16_t      pkno = -1;
    uint32_t     rowno;

    UNREF_PARM(hstmt)
    UNREF_PARM(pRegCtx)
    
    vtEnter();
    
    if ( !vtrows ) {
        /* allocate only on TAG0 */
        vtrows = psp_cGetMemory(maxrows*sizeof(struct virtab), TAG0);
    }
    /* locate specified primary key value, if any */
    for (pkno = 0; pkno < nocols; ++pkno) {
        if ( colsvals[pkno].colno == 0 ) {
            pkey.type = tINTEGER;
            memcpy(&pkey.vt.lv, colsvals[pkno].data, sizeof(int32_t));
            pkeyval = &pkey;
            break;
        }
    }
    if ( pkeyval ) {
        /* locate row with matching primary key */
        for ( rowno = 0; rowno < norows; ++rowno ) {
            if ( pkeyval->vt.lv == vtrows[rowno].pkey )
                break;
        }
        if ( rowno < norows )
            stat = errDUPLICATE;
    }
    if ( stat == errSUCCESS ) {
        stat = vtStoreRow(norows, nocols, colsvals); 
        if ( stat == errSUCCESS )
            ++norows;
    }

    vtExit();
    
    return stat;
} /*lint !e818 */

/* ========================================================================
    Virtual table SELECT execution function 
*/
static RSQL_ERRCODE EXTERNAL_FCN vtSelectOpen(  /* vtSelectOpen() */
    HSTMT        hstmt,        /* in:  statement handle */
    uint16_t     nocols,       /* in:  no. of ref'd columns */
    VCOL_INFO   *colsvals,     /* in:  array of ref'd column value containers */
    void        *pRegCtx,      /* in:  ptr to registration context */
    void        *pFCtx,        /* in:  ptr to fetch context */
    RSQL_VALUE  *pkeyval)      /* in:  ptr to primary key value */
/*  
    Called by SQL when SELECT statement is executed (rsqlExecute).
    
    The colvals contains the value containers for each ref'd row.

    The pRegCtx contains the pointer specified on the 
    rsqlRegisterVirtualTables call.
    
    The pFetchCtx points to an allocated data space of szFetchCtx (see
    rsqlRegisterVirtualTables) bytes long.  This is NULL is szFetchCtx
    was 0. 

    The pkeyval contains the value of the primary key if specified, else NULL.
    If specified then the select applies to that specific value only.
*/
{
    RSQL_ERRCODE stat = errSUCCESS;
    uint32_t     rowno;
    VTAB_CTX    *pCtx = (VTAB_CTX *)pFCtx;
    
    UNREF_PARM(hstmt)
    UNREF_PARM(pRegCtx)
    
    pCtx->rowcnt   = 0;
    pCtx->rowno    = rowno = 0;
    pCtx->pkeyval  = pkeyval;

    vtEnter();
    
    if ( !vtrows ) {
        /* allocate only on TAG0 */
        vtrows = psp_cGetMemory(maxrows*sizeof(struct virtab), TAG0);
    }
    if ( pkeyval ) {
        /* locate row with matching primary key */
        for ( rowno = 0; rowno < norows; ++rowno ) {
            if ( pkeyval->vt.lv == vtrows[rowno].pkey )
                break;
        }
        pCtx->rowno = rowno;
    }
    vtExit();
    
    return stat;
} /*lint !e818 */

/* ========================================================================
    Virtual table fetch function 
*/
static RSQL_ERRCODE EXTERNAL_FCN vtFetch(  /* vtFetch() */
    HSTMT        hstmt,        /* in:  statement handle */
    uint16_t     nocols,       /* in:  no. of ref'd columns */
    VCOL_INFO   *colsvals,     /* in:  array of ref'd column value containers */
    void        *pRegCtx,      /* in:  ptr to registration context */
    void        *pFCtx)        /* in:  ptr to fetch context */
/*
    Called by SQL to retrieve next row. Returns errSUCCESS until after the
    last row has been fetched then errNOMOREDATA.

    The colvals contains the value containers for each ref'd row.

    The pRegCtx contains the pointer specified on the 
    rsqlRegisterVirtualTables call.
    
    The pCtx points to an allocated data space of vtFetchSz bytes long
    (see rsqlRegisterVirtualTables). 
*/
{
    int16_t      cno;
    VTAB_CTX    *pCtx = (VTAB_CTX *)pFCtx;
    uint32_t     rno = (uint32_t)pCtx->rowno;
    
    vtEnter();
    
    if ( rno == norows || (pCtx->pkeyval && pCtx->rowcnt) ) {
        pCtx->rowno = 0;
        vtExit();
        return errNOMOREDATA;
    }
    for (cno = 0; cno < nocols; ++cno) {
        const VCOL_INFO *pCVal = &colsvals[cno];
        if ( vtrows[rno].is_null[pCVal->colno] )
            *pCVal->is_null = 1;
        else {
            *pCVal->is_null = 0;
            switch ( pCVal->colno ) {
                case 0: 
                    memcpy(pCVal->data, &vtrows[rno].pkey, sizeof(int32_t));
                    break;
                case 1:
                    strcpy(pCVal->data, vtrows[rno].name);
                    break;
                case 2:
                    strcpy(pCVal->data, vtrows[rno].addr);
                    break;
                case 3:
                    strcpy(pCVal->data, vtrows[rno].city);
                    break;
                case 4:
                    strcpy(pCVal->data, vtrows[rno].state);
                    break;
                case 5:
                    strcpy(pCVal->data, vtrows[rno].zip);
                    break;
            } /*lint !e744 */
        }
    }
    ++pCtx->rowcnt;
    ++pCtx->rowno;

    vtExit();
    
    return errSUCCESS;
}

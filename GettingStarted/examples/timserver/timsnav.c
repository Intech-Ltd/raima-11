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

/*
    This example demonstrates how to navigate a network-model database (timserver),
    using the d_recfrst, d_recnext and d_recprev function calls.

    This example also shows how to create an HTTP server that can build web
    pages to display your database.

    See also (in httpdocs\timserver):
        display.js
        index.html
*/

#include "timsnav.h"
#include "timserver.h"


/************************ structures ************************************/
/* Function mapping table */
typedef struct {
    const char *fcnName;
    int32_t   (*fcn)(PSP_OUTPUTBUFF, const URLARGS *, DB_TASK *);
} HTTPACTIONS;

typedef struct {
    const char *fcnName;
    int32_t     record_type;
    int32_t   (*fcn)(PSP_OUTPUTBUFF, DB_TASK *, int32_t);
} FIELDACTIONS;



/*********************** functions **************************************/

static int32_t     display_author(PSP_OUTPUTBUFF, DB_TASK *, int32_t);
static int32_t     display_info(PSP_OUTPUTBUFF, DB_TASK *, int32_t);

static int32_t     getAction(const URLARGS *, uint32_t *);
static int32_t     getField(const URLARGS *, uint32_t *);
static int32_t     getIndex(const URLARGS *, int32_t *);

static int32_t     recfrst_handler(PSP_OUTPUTBUFF, const URLARGS *, DB_TASK *);
static int32_t     recnext_handler(PSP_OUTPUTBUFF, const URLARGS *, DB_TASK *);
static int32_t     recprev_handler(PSP_OUTPUTBUFF, const URLARGS *, DB_TASK *);
static char       *simpleEscape(const char *, PSP_MEMTAG);
static void        my_dberr(int32_t, char *);

/*********************** defines ****************************************/


#define MAX_ACTION_FUNCTIONS    RLEN(httpActions)
#define MAX_TYPE_FUNCTIONS      RLEN(httpFields)



/*********************** globals ****************************************/
/* URL arguments contain an 'action=xxx' and a 'type=xxx' */
/* Function mapping table for URL 'action' argument */
static const HTTPACTIONS httpActions[] = {
    { "d_recfrst",    recfrst_handler },
    { "d_recnext",    recnext_handler },
    { "d_recprev",    recprev_handler },
};

/* Function mapping table for URL 'field' argument */
static const FIELDACTIONS httpFields[] = {
    { "info",       INFO,       display_info },
};



/* =======================================================================
    Get the action function id. Finds the index into httpActions from the
    value of the URL argument 'action'.
*/
static int32_t getAction(
    const URLARGS       *urlargs,    /* IN:  URL args to parse */
    uint32_t            *fcnid)      /* OUT: index into function table */
{
    const char  *action;
    uint32_t     ii;

    if (!fcnid || !urlargs)
        return S_INVNULL;

    action = http_getURLArgValue("action", urlargs);

    /* Find action function based on 'action' arg value */
    for (ii = 0; action && ii < MAX_ACTION_FUNCTIONS; ii++)
    {
        if (strcmp(action, httpActions[ii].fcnName) == 0)
        {
            *fcnid = ii;
            return S_OKAY;
        }
    }

    return PSP_FAILED;
}

/* =======================================================================
    Get the field function id. Finds the index into httpFields from the
    value of the URL argument 'field'.
*/
static int32_t getField(
    const URLARGS       *urlargs,    /* IN:  URL args to parse */
    uint32_t            *fcnid)      /* OUT: index into function table */
{
    const char  *type;
    uint32_t     ii;

    if (!fcnid || !urlargs)
        return S_INVNULL;

    type = http_getURLArgValue("field", urlargs);

    /* Find action function based on 'action' arg value */
    for (ii = 0; type && ii < MAX_TYPE_FUNCTIONS; ii++)
    {
        if (strcmp(type, httpFields[ii].fcnName) == 0)
        {
            *fcnid = ii;
            return S_OKAY;
        }
    }

    return PSP_FAILED;
}

/* =======================================================================
    Get the index function id. Finds the value of the URL argument 'index'.
*/
static int32_t getIndex(
    const URLARGS       *urlargs,    /* IN:  URL args to parse */
    int32_t             *index)      /* OUT: index  */
{
    const char  *type;

    if (!index || !urlargs)
        return S_INVNULL;

    type = http_getURLArgValue("index", urlargs);
    *index = type ? atoi(type) : 0;

    return S_OKAY;
}


/* =======================================================================
    Perform a simple escape of a string for example purposes.
    A more robust escape function may be required.
    Only replaces = with %3D and & with %26.
    Allocates a buffer for output, caller must free returned buffer.
*/
static char *simpleEscape(
    const char      *str,   /* IN: string to escape */
    PSP_MEMTAG       tag)   /* Memory tag for allocation */
{
    char    *outstr;
    char    *output;

    outstr = output = psp_getMemory((strlen(str) * 3) + 1, tag);

    if (outstr)
    {
        for (; *str; ++str)
        {
            if (*str == '=')
            {
                *outstr++ = '%';
                *outstr++ = '3';
                *outstr++ = 'D';
            }
            else if (*str == '&')
            {
                *outstr++ = '%';
                *outstr++ = '2';
                *outstr++ = '6';
            }
            else
                *outstr++ = *str;
        }
        *outstr = *str; /* null terminate */
    }

    return output;
}


/* =======================================================================
    Display info record. Reads the current record to get the info
    fields.  Builds an output string in a name=value pair format. This
    string is output to the browser, where the Javascript will parse
    the name=value pairs and display the results.
*/
static int32_t display_info(
    PSP_OUTPUTBUFF           outbuf, /* IN: HTTP output buffer */
    DB_TASK                 *task,
    int32_t                  index)
{
    int32_t          stat;
    const char      *outputfmt;
    char            *id_code;
    char            *info_title;
    char            *publisher;
    char            *pub_date;
    struct info      info_record;

    /* Output string is in the format: name=value&...
       The Javascript will parse for these names ('id_code', 'info_title',
       'publisher', 'pub_date', 'info_type', 'index').
       These are not display names.  See function ajaxInfoResponseHandler in
       display.js.

       The index is stored in the URL so the code know where to start when
       'next' or 'previous' are clicked.
     */
    outputfmt = "id_code=%s&info_title=%s&publisher=%s&pub_date=%s&info_type=%d&index=%d&";

    /* Lock record */
    stat = d_reclock(INFO, "r", task, CURR_DB);

    /* Read current record */
    if (stat == S_OKAY &&
            (stat=d_recread(&info_record, task, CURR_DB)) == S_OKAY)
    {
        d_recfree(INFO, task, CURR_DB);

        /* Escape user name so any '=' or '&' characters don't cause a problem
           when being parsed in the browser.
         */
        id_code = simpleEscape(info_record.id_code, TAG0);
        if (!id_code)
            return PSP_NOMEMORY;

        info_title = simpleEscape(info_record.info_title, TAG0);
        if (!info_title)
        {
            psp_freeMemory(id_code, TAG0);
            return PSP_NOMEMORY;
        }

        publisher = simpleEscape(info_record.publisher, TAG0);
        if (!publisher)
        {
            psp_freeMemory(id_code, TAG0);
            psp_freeMemory(info_title, TAG0);
            return PSP_NOMEMORY;
        }

        pub_date = simpleEscape(info_record.pub_date, TAG0);
        if (!pub_date)
        {
            psp_freeMemory(id_code, TAG0);
            psp_freeMemory(info_title, TAG0);
            psp_freeMemory(publisher, TAG0);
            return PSP_NOMEMORY;
        }

        stat = http_printf(outbuf, outputfmt, id_code, info_title,
                            publisher, pub_date, info_record.info_type, index);

        psp_freeMemory(id_code, TAG0);
        psp_freeMemory(info_title, TAG0);
        psp_freeMemory(publisher, TAG0);
        psp_freeMemory(pub_date, TAG0);
    }

    return stat;
}


/* =======================================================================
    Action is to retrieve previous record.
*/
static int32_t recprev_handler(
    PSP_OUTPUTBUFF           outbuf, /* IN: HTTP output buffer */
    const URLARGS           *urlargs,
    DB_TASK                 *task)
{
    int32_t          stat;
    uint32_t         field;
    int32_t          index;
    int32_t          ii;

    /* Get 'field' and 'index' arguments from the URL */
    if (getField(urlargs, &field) != S_OKAY)
        return PSP_FAILED;

    if (getIndex(urlargs, &index) != S_OKAY)
        return PSP_FAILED;

    /* HTTP sessions are not persistent, therefore we must get the first
       record and do recnext until we get the desired record. We use the 'index'
       to find the correct 'previous' record.
     */
    if (index <= -2)
    {
        /* -2 means to find the last record and the last record's index */
        ii = 0;
        for ((stat = d_recfrst(httpFields[field].record_type, task, CURR_DB));
                stat == S_OKAY;
                    stat = d_recnext(task, CURR_DB))
        {
                ++ii;
        }
        if ((stat = d_recprev(task, CURR_DB)) != S_OKAY)
            return stat;

        index = ii - 1;

        /* Call the display function for this field */
        return httpFields[field].fcn(outbuf, task, index);
    }
    else if (index == -1)
    {
        /* Display 'no data' between last record and first record. */
        return S_NOTFOUND;
    }

    if ((stat = d_recfrst(httpFields[field].record_type, task, CURR_DB))
                                                                  != S_OKAY)
    {
        /* TODO: Write an error handler */
        return stat;
    }

    /* Make the record at 'index' the current record */
    for (ii = 0; ii < index; ++ii)
    {
        if ((stat = d_recnext(task, CURR_DB)) == S_NOTFOUND)
        {
            if ((stat = d_recfrst(httpFields[field].record_type, task,
                                    CURR_DB)) != S_OKAY)
                return stat;

            index = 0;
        }
    }

    /* Call the display function for this field */
    return httpFields[field].fcn(outbuf, task, index);
}

/* =======================================================================
    Action is to retrieve next record.
*/
static int32_t recnext_handler(
    PSP_OUTPUTBUFF           outbuf, /* IN: HTTP output buffer */
    const URLARGS           *urlargs,
    DB_TASK                 *task)
{
    int32_t          stat;
    uint32_t         field;
    int32_t          index;
    int32_t          ii;

    /* Get 'field' and 'index' arguments from the URL */
    if (getField(urlargs, &field) != S_OKAY)
        return PSP_FAILED;

    if (getIndex(urlargs, &index) != S_OKAY)
        return PSP_FAILED;

    /* HTTP sessions are not persistent, therefore we must get the first
       record and do recnext until we get the desired record. We use the 'index'
       to find the correct 'previous' record.
     */
    if ((stat = d_recfrst(httpFields[field].record_type, task, CURR_DB))
                                                                  != S_OKAY)
    {
        /* TODO: Write an error handler */
        return stat;
    }

    /* Make the record at 'index' the current record */
    for (ii = 0; ii < index; ++ii)
    {
        if ((stat = d_recnext(task, CURR_DB)) == S_NOTFOUND)
            return stat;
    }

    /* Call the display function for this field */
    return httpFields[field].fcn(outbuf, task, index);
}

/* =======================================================================
    Action is to set the first record as the current record.
*/
static int32_t recfrst_handler(
    PSP_OUTPUTBUFF           outbuf, /* IN: HTTP output buffer */
    const URLARGS           *urlargs,
    DB_TASK                 *task)
{
    int32_t          stat;
    uint32_t         field;

    /* Get 'field' argument from the URL */
    if (getField(urlargs, &field) != S_OKAY)
        return PSP_FAILED;

    if ((stat = d_recfrst(httpFields[field].record_type, task, CURR_DB))
                                                                  != S_OKAY)
    {
        /* TODO: Write an error handler */
        return stat;
    }

    /* Call the display function for this field */
    return httpFields[field].fcn(outbuf, task, 0);
}

/* =======================================================================
    Main navigate handler function.  This function is setup in the
    TFS_XAPI_FCNMAP in timserver.c.  This function is called whan a URL
    with /navigate.rdm is requested by this HTTP server.
*/
int32_t navigate_handler(
    RHS_HANDLE               hRHS,
    PSP_OUTPUTBUFF           outbuf,   /* IN: HTTP output buffer */
    const char              *args,     /* IN: URL decoded args */
    const char              *raw_args, /* IN: URL encoded args */
    const char              *content)  /* IN: HTTP content from POST */
{
    int32_t         stat;
    uint32_t        action;
    const URLARGS  *urlargs;
    const char     *dbname;
    DB_TASK        *task = NULL;

    UNREF_PARM(content);
    UNREF_PARM(raw_args);
    
    /* Parse the URL arguments.  Use the result 'urlargs' to retrieve
       specific argument values.
     */
    if ((stat = http_initURLArgs(hRHS, args, &urlargs)) != S_OKAY)
        return stat;

    /* Get the action from the URL */
    if (getAction(urlargs, &action) != S_OKAY)
    {
        http_freeURLArgs(urlargs);
        return PSP_FAILED;
    }

    /* Open a task */
    if (d_opentask(&task) != S_OKAY)
    {
        http_freeURLArgs(urlargs);
        return PSP_FAILED;
    }

    if (d_set_dberr((ERRORPROC) my_dberr, task) != S_OKAY)
    {
        http_freeURLArgs(urlargs);
        d_closetask(task);
        return PSP_FAILED;
    }

    dbname = http_getURLArgValue("dbname", urlargs);
    if (!dbname)
    {
        http_freeURLArgs(urlargs);
        d_closetask(task);
        return PSP_FAILED;
    }

    /* Open the timserver database */
    if (d_open(dbname, "s", task) != S_OKAY)
    {
        http_freeURLArgs(urlargs);
        d_closetask(task);
        return PSP_FAILED;
    }

    /* Call the action function handler for this action */
    stat = httpActions[action].fcn(outbuf, urlargs, task);

    /* cleanup */
    d_close(task);
    d_closetask(task);
    http_freeURLArgs(urlargs);

    return stat;
}

static void my_dberr(
    int32_t err,
    char   *errmsg)
{
    /* This function does not display the error on stdout, as the default
       error handler does. We may get -905 errors if the user tries to read
       the database after removing it. We don't want these errors to be seen
       by the user.
    */

    UNREF_PARM(err)
    psp_log("%s", errmsg);
}

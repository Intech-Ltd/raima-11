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
 *    See section 4.5.3 of the User's Guide for database design and
 *    application requirements.
*/ /*lint -e957 */

#include <stdio.h>
#include "rdm.h"

#include "tims_dbd.h"

int32_t loan_book(DB_TASK *);
int32_t return_book(DB_TASK *);
int32_t list_loaners(DB_TASK *);
int32_t del_info(DB_TASK *);
int32_t ent_info(DB_TASK *);
void    by_key(DB_TASK *);
int32_t by_author(DB_TASK *);
int32_t list_authors(DB_TASK *);
int32_t list_keys(DB_TASK *);
char   *getstring(char *, size_t); /*lint -esym(534,getstring) */
static void EXTERNAL_FCN tims_dberr(int32_t, const char *, void *, DB_TASK *);

/* Define NEED_SETUP_DATABASE for creating an executable which is self
   contained with respect to needed source files for executing it.
   The purpose of this is to be able to run this example either from
   some non source directory or on some embedded platform.  In these
   environments we don't have access or it may be inconvenient to gain
   access the source files.
 */
#if defined(NEED_SETUP_DATABASE)
#include "tims_ddl.h"
#include "tims_imp.h"
#include "author_txt.h"
#include "info_txt.h"
#include "text_txt.h"
#include "key_word_txt.h"
#include "intersec_txt.h"

static int16_t writeFile(
    const char *name,
    const char *data)
{
    PSP_FH fh;

    fh = psp_fileOpen(name, O_CREAT|O_TRUNC|O_RDWR, 0);
    if (fh == NULL)
    {
        printf("Failed to create %s (errno = %d)\n", name, psp_errno());
        printf("Failed to set up database\n");
        fflush(stdout);
        return PSP_FAILED;
    }

    if (psp_fileWrite(fh, data, (uint32_t) strlen(data)) != strlen(data))
    {
        printf("Failed to write to file '%s'\n", name);
        fflush(stdout);
        return PSP_FAILED;
    }

    psp_fileClose(fh);

    return PSP_OKAY;
}

/* This function will write the schema definition and some data to a
   number of files, generate the dictionary, initialize the database,
   and finaly import the data into the database.

   These steps can instead be carried out from the command line as
   follows:

      $ cp ${CURRENT_SOURCE_DIR}/tims.ddl .
      $ cp ${CURRENT_SOURCE_DIR}/tims.imp .
      $ cp ${CURRENT_SOURCE_DIR}/?*.txt .
      $ ddlp -q -nh tims.ddl
      $ initdb -q -y tims
      $ dbimp -q tims.imp
 */
static int32_t setup_database(void)
{
    int32_t   stat;

    const char *const ddlp_tims_argv[] =
    {
        "ddlp",
        "-q",
        "-nh",
        "tims.ddl"
    };

    const char *const initdb_tims_argv[] =
    {
        "initdb",
        "-q",
        "-y",
        "tims"
    };

    const char *const dbimp_tims_argv[] =
    {
        "dbimp",
        "-q",
        "tims.imp"
    };


    if ((stat = writeFile("tims.ddl", tims_ddl)) != PSP_OKAY)
        return stat;

    if ((stat = writeFile("tims.imp", tims_imp)) != PSP_OKAY)
        return stat;

    if ((stat = writeFile("author.txt", author_txt)) != PSP_OKAY)
        return stat;

    if ((stat = writeFile("info.txt", info_txt)) != PSP_OKAY)
        return stat;

    if ((stat = writeFile("text.txt", text_txt)) != PSP_OKAY)
        return stat;

    if ((stat = writeFile("key_word.txt", key_word_txt)) != PSP_OKAY)
        return stat;

    if ((stat = writeFile("intersec.txt", intersec_txt)) != PSP_OKAY)
        return stat;

    stat = ddlp_main(RLEN(ddlp_tims_argv), ddlp_tims_argv);
    if (stat != PSP_OKAY) {
       printf("Failed to set up database, error = %d\n", stat);
       return stat;
    }

    stat = initdb_main(RLEN(initdb_tims_argv), initdb_tims_argv);
    if (stat != PSP_OKAY) {
       printf("Failed to set up database, error = %d\n", stat);
       return stat;
    }

    stat = dbimp_main(RLEN(dbimp_tims_argv), dbimp_tims_argv);
    if (stat != PSP_OKAY) {
       printf("Failed to set up database, error = %d\n", stat);
       return stat;
    }

    return PSP_OKAY;
}

#else /* NEED_SETUP_DATABASE */

/* This function will do nothing.  We assume that ddlp, initdb and
   dbimp already have been run from the currect working directory
   before executing this example.  The standard make files distributed
   with this example will run these commands as follows:

      $ ddlp -c tims.ddl
      $ initdb -y tims
      $ dbimp tims.imp
 */
static int32_t setup_database(void)
{
    return PSP_OKAY;
}
#endif /* NEED_SETUP_DATABASE */

/* Technical Information Management System
*/ /*lint -esym(765,tims_main) */
int32_t EXTERNAL_FCN tims_main(
    int32_t            argc,
    const char *const *argv)
{
    char         cmd[20];        /* command entry string */
    int32_t      status;
    int16_t      sCont = 1;
    DB_TASK     *task;

    UNREF_PARM(argc)
    UNREF_PARM(argv)

    if ((status = d_opentask(&task)) != S_OKAY)
    {
        printf("Failed to open a new task, error = %d\n", status);
        return (1);
    }

    if ((status = d_set_dberr_ex(tims_dberr, NULL, task)) != S_OKAY)
    {
        printf("Failed set dberror handler, error = %d\n", status);
        d_closetask(task);
        return (1);
    }

    setup_database(); /*lint !e522 !e534 */

    if ((status = d_open_ptr ("tims", "o", &tims_dbd, sizeof(tims_dbd), task)) != S_OKAY)
    {
        printf("Failed to open database, error = %d\n", status);
        d_closetask(task);
        return (1);
    }

    while (sCont)
    {
        /* display command menu */
        printf("\nTIMS Commands:\n");
        printf("   1 - Display list of key words\n");
        printf("   2 - Display list of authors\n");
        printf("   3 - List publications by key word\n");
        printf("   4 - List publications by author\n");
        printf("   5 - Enter technical information\n");
        printf("   6 - Delete technical information\n");
        printf("   7 - Loan book\n");
        printf("   8 - Return loaned book\n");
        printf("   9 - List borrowed books\n");
        printf("   q - Quit\n");
        printf("enter command: ");
        fflush (stdout);
        getstring(cmd, sizeof(cmd));

        switch (cmd[0])
        {
        case 'q':
        case 'Q':
            d_close(task);
            d_closetask(task);
            sCont = 0;
            break;

        case '1':   list_keys(task);      break; /*lint !e534 */
        case '2':   list_authors(task);   break; /*lint !e534 */
        case '3':   by_key(task);         break; /*lint !e534 */
        case '4':   by_author(task);      break; /*lint !e534 */
        case '5':   ent_info(task);       break; /*lint !e534 */
        case '6':   del_info(task);       break; /*lint !e534 */
        case '7':   loan_book(task);      break; /*lint !e534 */
        case '8':   return_book(task);    break; /*lint !e534 */
        case '9':   list_loaners(task);   break; /*lint !e534 */
        default:
            printf("*** incorrect command -- re-enter\n");
            break;
        }
    }
    return 0;
}


/* safe gets() */
char *getstring(char *s, size_t size)
{
    size_t len;

    if (fgets(s, (int32_t) (uint32_t) size, stdin) == NULL)
        return NULL;

    len = strlen(s);
    if (s[len-1] == '\n')
        s[len-1] = '\0';

    return s;
}

static void EXTERNAL_FCN tims_dberr(
    int32_t     errnum, 
    const char *errmsg,
    void       *data,
    DB_TASK    *task)
{ /*lint !e403 */
    UNREF_PARM(data)
    UNREF_PARM(task)

    DisplayErrorNow(FALSE, FALSE, "\n%s (errnum = %d)\n", errmsg, errnum);
} /*lint !e818 */

PSP_STARTUP_EXAMPLE(tims)

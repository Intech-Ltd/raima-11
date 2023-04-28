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
#ifndef REPAIR_H
#define REPAIR_H

#ifdef _DBREPAIR_TOOLKIT_
#define MOD_EDBR  "edbr"
/*
 * Velocis Database Repair (dbrepair) error codes
*/
#define S_DBRTIMEOUT (-6002)      /* Timeout waiting for DBREPAIR module */
#define S_BADTEMPDB  (-6005)      /* Unable to open temporary database */
#endif

#define DBCHECK     0
#define DBFIX       1
#define KEYBUILD    2
#define DCHAIN      3
#define DBDEFRAG    4


#define NUMFUNCTIONS 5

/* The table below is used to calculate how much of the processing time
   for the functions is used within each function. Consider that the
   entire process for this database takes 10 minutes. The table describes
   how much of that 100 minutes (in whole minutes) is used for DBCHECK,
   DBFIX, etc. It's an estimate that helps in the progress display only. */

#ifdef DBRLIB
int16_t FunctPctOfTotal[NUMFUNCTIONS]={
	  100,50,20,15,15 };  /* Total for last 4 should equal 100 */

char FunctNames[NUMFUNCTIONS][10] = {
    "DBCheck",
    "DBFix",
    "Keybuild",
    "DChain",
    "DBDefrag" };
#else
    int16_t GetFunctPctOfTotal(int16_t sNum);
    char* GetFunctNames(int16_t sNum);
#endif

#ifdef DBRLIB
char *StatusStrings[] = {
	  "Initializing...",
	  "Reading Dictionary...",
	  "Checking Key files...",
	  "Quick check of data files...",
	  "Full check of data files...",
	  "Done checking files. Reporting...",
	  "Searching for wasted records...",
	  "Verifying sets (owners)...",
	  "Verifying sets (members)...",
	  "Cleaning inconsistencies...",
	  "Apply fixes...",
	  "Done fixing data files.",
	  "Done fixing key files.",
	  "Initializing key files...",
	  "Rebuilding keys...",
	  "Done rebuilding keys. Reporting...",
	  "Rebuilding delete chain..."
};

#endif

#define U_SETCHECK      0x0001
#define U_KEYCHECK      0x0002
#define U_BLOBCHECK     0x0004
#define U_ALLCHECK      (U_SETCHECK | U_KEYCHECK | U_BLOBCHECK)

#define U_REPORT_NO_DETAILS    0x0010
#define U_SYSLOG_NO_DETAILS    0x0020

#define U_NO_EXECUTE     0x0100
#define U_IGNORE_DBA     0x0200

/* Default constants */
#define DBREPAIR_CACHEPAGES_DFLT        64
#define DEFAULT_LOG_NAME               "dbrepair.log"
#define INI_FILE_NAME                  "dbrepair.ini"

/*===================================================================
   Return codes. Based on the last RDM system status code.
===================================================================*/
#ifndef S_SYSTEM_LAST   /* no need to include rdm.h everywhere */
#define S_SYSTEM_LAST   -947              /* review this number if not using RDM 4.02 */
#pragma message("S_SYSTEM_LAST hard coded. Verify it's the correct number (" __FILE__ ")")
#endif /* S_SYSTEM_LAST */
#define S_CORRUPTED     S_SYSTEM_LAST - 1 /* Database is corrupted */
#define S_ERROR         S_SYSTEM_LAST - 2 /* General dbreapair error */
#define S_NORESULT      S_SYSTEM_LAST - 3 /* Log file couldn't be opened */
#define S_BADDB         S_SYSTEM_LAST - 4 /* No DBD file present */
#define S_ABORT         S_SYSTEM_LAST - 5 /* Process has been aborted */
#define S_BADTEMPDB     S_SYSTEM_LAST - 6 /* No DBD file for temporary DB present */

#ifdef WM_USER    /* Windows related constants, no need for them on the 
                     low level functions */
/*===================================================================
   Windows messaging
===================================================================*/
#define WM_USER_ABORT   WM_USER + 11  /* Message to abort the processing */
#define WM_USER_REPORT  WM_USER + 12  /* Message to report the progress status */
#define WM_USER_ERROR   WM_USER + 13  /* Message to indicate db_Repair error */

#define PROCESS_IDS_READ_PROFILE    62001
#define PROCESS_IDS_DO_REPAIR     62001
#define PROCESS_IDS_DB_CHECK        62003
#define PROCESS_IDS_DB_FIX          62004
#define PROCESS_IDS_KEYBUILD        62005
#define PROCESS_IDS_D_CHAIN         62006

#define PROCESS_IDS_STATUS          63000

#endif /* WM_USER */
/*===================================================================
   Error return codes
===================================================================*/
#define EXIT_CODE_NO_SYSTEM_INFO         -1
#define EXIT_CODE_INVALID_OPTION         -2
#define EXIT_CODE_INSUFFICIENT_RESOURCES -3
#define EXIT_CODE_TOO_MANY_REPAIRS       -4
#define EXIT_CODE_RUNTIME_ERROR          -5
#define EXIT_CODE_INTERRUPTED            -6

/*===================================================================
   DBRepair stages
===================================================================*/
#define WL_DBR_STAGE      -1L /* flag to indicate that the message 
                                 includes a stage constant */
#define WS_DBR_INITIAL     0  /* Initialization stage */
#define WS_DBR_DICTIONARY  1  /* Reading DB dictionary */
#define WS_DBR_KEYCHECK    2  /* Checking key files */
#define WS_DBR_QCKCHECK    3  /* Quick check on data files */
#define WS_DBR_FULLDATACHK 4  /* Full check on data files */
#define WS_DBR_DONECHK     5  /* Done checking files */
#define WS_DBR_WASTEREC    6  /* Searching for wasted records */
#define WS_DBR_CORRUPTSETS 7  /* Searching for corrupted sets (owners) */
#define WS_DBR_MEMBERCHNS  8  /* Searching for healthy member chains (members) */
#define WS_DBR_INCONSIST   9  /* Cleaning inconsistencies */
#define WS_DBR_FIX        10  /* Apply fixes */
#define WS_DBR_DONEDATA   11  /* Done fixing data files */
#define WS_DBR_DONEKEYS   12  /* Done fixing key files - LAST STAGE of DBFix */
#define WS_DBR_INITKEYFILE 13 /* Initializing key files */
#define WS_DBR_REBLDKEYS  14  /* Rebuilding key files */
#define WS_DBR_DONEREBLDKEYS 15 /* Done rebuilding key files - LAST STAGE of Keybuild */
#define WS_DBR_REBLDCHAIN 16  /* Rebuilding Delete chain */
#define WS_DBR_TCOUNT     -1  /* Message indicating that wP will be a total */
#define WS_DBR_COUNT      -2  /* Message indicating that wP is a counter */

/*===================================================================
   Function prototypes
===================================================================*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
int16_t EXTERNAL_FCN d_dbrdbcheck(
        DBREPAIR_PARAMS  *params,
        int16_t           optFlags, 
        char            **setNames,
        int16_t           nosets, 
        int16_t           nodbfiles);

int16_t EXTERNAL_FCN d_dbfix(
		DBREPAIR_PARAMS*, 
		int16_t optFlags);

int16_t EXTERNAL_FCN d_dbrkeybuild(
        DBREPAIR_PARAMS*,
		int16_t optFlags, 
        int16_t nodbfiles);

int16_t EXTERNAL_FCN dbr_dchain(
        DBREPAIR_PARAMS*, 
        int16_t, 
        int16_t);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*REPAIR_H */

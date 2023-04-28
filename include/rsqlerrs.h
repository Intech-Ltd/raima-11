/***************************************************************************
 * Raima Database Manager                                                  *
 *                                                                         *
 * Copyright (c) 2011 Raima Inc. All rights reserved.                      *
 *                                                                         *
 * Use of this software, whether in source code format, or in executable,  *
 * binary object code form, is governed by the Raima Inc. LICENSE          *
 * which is fully described in the LICENSE.TXT file, included within this  *
 * distribution of files.                                                  * 
 **************************************************************************/
/*----------------------------------------------------------------------------
   rsqlerrs.h:  RDM SQL internal error code assignments header file.
----------------------------------------------------------------------------*/

#if !defined(__RSQLERRS_H)
#define __RSQLERRS_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
    const char *errcode;
    char        sqlstate[6];
    const char *msg;
} RSQLERROR_DEFN;

#define RSQL_ERRCODE_OFFSET (-errINVHANDLE)

/* RDM SQL-specific error codes.
   Add new codes to end of table *before errSYSTEM.
*/
typedef enum {
    /* GENERRORBEGIN */
    errINVHANDLE = -5,
    errNEEDDATA = -4,
    errIGNORE = -3,
    errSQLERROR = -2,
    errNOMOREDATA = -1,
    errSUCCESS = 0,
    errTRUNCATE = 1,
    errPSPFAILURE,
    errSYNTAX,
    errNOCRDB,     
    errGENDDLFILE,  
    errBADREFDCOL,
    errPKNOTFOUND,  
    errFKPKMISMATCH,
    errBADFORKEYCOL,
    errDUPCOLNAME,
    errDUPTABNAME,
    errDUPCURSOR,
    errNODOMAIN,
    errTABNOTFOUND,
    errDATETIMEOVF,
    errBADKEYCOL,
    errGENFILE,
    errNOMEMORY,
    errBADFORMAT,
    errBADBINLITERAL,
    errBADLITERAL,
    errTOOLONG,
    errNODB,
    errCATNOTFOUND,
    errINITDB,
    errFILEIO,
    errNOTOPEN,
    errINVARG,
    errINVNULL,
    errOPENHDBCS,
    errBADDATALEN,
    errTYPEATTR,
    errCOLNUMBER,
    errPARNUMBER,
    errFCNSEQ,
    errTRACTIVE,
    errTRNOTACT,
    errRDMERROR,
    errCONNECTED,
    errDBTBO,
    errDBNOTOPEN,
    errINSERTVALS,
    errSELECTCOLS,
    errNOCOLUMN,
    errNUMPAR,
    errNOTNULL,
    errTABNOTLISTED,
    errCOLNOTFOUND,
    errFETCHTYPE,
    errROWVALUE,
    errCURPOS,
    errCURSTATE,
    errNOTSELECT,
    errRANGE,
    errDATALOST,
    errREFINTEGRITY,
    errCLOSEHDBC,
    errDRVMEMORY,
    errATTRVALUE,
    errOPTIONRANGE,
    errROWERROR,
    errOPTCHANGED,
    errTYPEMISMATCH,
    errESCAPECHAR,
    errINVSTATE,
    errWHERECALCS,
    errDIVBY0,
    errESCAPE,
    errDDLNOTEXEC,
    errINVPARAM,
    errDUPPROC,
    errNOPROC,
    errNUMARGS,
    errJOINCOLS,
    errKEYCOLLIMIT,
    errBADOUTERJOIN,
    errHDBCINUSE,
    errREGISTERED,
    errVIRTABPOS,
    errVIRTABCOLTYPE,
    errVIRTABERROR,
    errNOTREGISTERED,
    errUDF,       
    errUDFNOVAL,  
    errUDFDEFN,
    errUDFARG,
    errUDFNOARGS,
    errFKSETNULL,
    errINSERTCOLREF,
    errDUPLICATE,
    errNOSETNULL,
    errNOCASCADE,
    errRESTRICTED,
    errDUPJOINCOL,
    errNOJOINCOLMATCH,
    errSORTCOLREF,
    errFCNARG,
    errNUMFCNARGS,
    errREADONLY,
    errINVSTMT,
    errDISTINCT,
    errCANCELED,
    errGROUPBYAGGFN,
    errBADCTYPE,
    errBADSTYPE,
    errNOTCURSOR,
    errINVDESCIDX,
    errTXUNKNOWN,
    errNOTPREPARED,
    errINVOPNOW,
    errINVTXTYPE,
    errINVIRDMOD,
    errAUTODESCR,
    errBADDESCINFO,
    errDESCRANGE,
    errSCALE,
    errPARMTYPE,
    errTIMEOUT,
    errNOTCAPABLE,
    errNOTIMPLEMENTED,
    errINVCONVERT,
    errNOINDVAR,
    errNOTSUPPORTED,
    errINVTFSSPEC,
    errIEF,
    errTFSERROR,
    errREMOTEERROR,
    errCOMERROR,
    errROTRACT,
    errUNLOCKINTRANS,
    errNOTLOCKED,
    errLOCKSACTIVE,
    errNOPENDBS,
    errMULTIDBTRANS,
    errINVDATEFMT,
    errINVDATESEP,
    errINVOPENMODE,
    errEXCLUSIVE,
    errNOTUPDATEABLE,
    errBLOBDEFAULT,
    errBLOBEXPR,
    errDATAATEXEC,
    errBLOBPARSONLY,
    errBADBLOBPAR,
    errCONCATNULL,
    errFCNRANGE,
    errCOLRANGE,
    errSCOPERANGE,
    errNULLRANGE,
    errUNIQUERANGE,
    errACCURANGE,
    errSQLTYPE,
    errCIRCTABREF,
    errTFSFAILURE,
    errVARCHARLEN,
    errNOPGSANDROWS,
    errCIRTABDELETE,
    errMAXROWS,
    errDEFEREDBLOB,
    errDBEXISTS,
    errTFSINIT,
    errLOCKMODE,
    errNOUPDCOLS,
    errAUTOCOMMIT,
    errCURSORTABLE,
    errNOTINPROC,
    errRDONLYFLAG,
    errTABLEREF,
    errDBOPENMODE,
    errNESTEDAGGS,
    errCHARREQD,
    errCONNECT,
    errLOCKSFREED,
    errINVTRID,
    errUNIONOPEN,
    errDBUNAVAIL,
    errDBOPEN,
    errRDONLYDB,
    errNOBLOBSORT,
    errNOBLOBGROUP,
    errINVARGTYPE,
    errDIFFCONNS,
    errOUTERJOIN,
    errSHOWPLAN,
    errQUERYUDF,
    errVTACCESS,
    errVTSPACE,
    errNOWHEREBLOB,
    errINVTYPEMAP,
    errNAMEDPARAM,
    errUNUSED000,
    errDBACTIVE,
    errDBINUSE,
    errNOTINUDF,
    errCOREDBOPEN,
    errCATVERSION,
    errONCLAUSE,
    errGROUPRESULT,
    errDOCROOTUSED,
    errNOVTENTRY,
    errNOVTSINDB,
    errINVCURSOR,
    errCURSORTAB,
    errFORUPDCOL,
    errINVPROC,
    errNOTUDFAGG,
    errNODBTABLES,
    errDDLKEYWORD,
    errINVFILETYPE,
    errTBD,            /* leave as next to last entry */
    errSYSTEM          /* Must be last entry */
    /* GENERROREND */
} RSQL_ERRCODE;

#if defined(__cplusplus)
}
#endif

#endif  /* __RSQLERRS_H */

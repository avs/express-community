/********************************************************************************/
/*										*/
/*	Project  : P A P Y R U S  Toolkit					*/
/*	File     : PapyError3.h					        	*/
/*	Function : Error codes and macros for papyrus generated errors   	*/
/*	Authors  : Jean-Francois Vurlod						*/
/*		   Christian Girard						*/
/*	History  : 08.1992	version 1.2					*/
/*		   05.1993	version 2.0					*/
/*		   06.1994	version 3.0					*/
/*								   		*/
/*	(C) 1992, 1993, 1994, 1996 The University Hospital of Geneva		*/
/*		       All Rights Reserved					*/
/*										*/
/********************************************************************************/

#ifndef PapyError3H
#define PapyError3H

#ifdef NDEBUG
#define NODEBUG
#endif

typedef enum {
    papNoError		    	=  0,

    papErrorInPapyErrNo	    	= -1,

    papNotImplemented	    	= -2,

  /********************************************************/
  /* System Errors                                        */
  /********************************************************/

    papClib		    	= -3,	/* This is the code set for any unix
					   c-library call that returns a bad
					   value. The caller, if interested, 
					   can check the global variable
					   errno.*/

    papNoMemory		    	= -4,	/* Error in allocating heap memory. */

    papUIDUnknow		= -5,	/* PapyRead */
    papReadGroup		= -6,
    papWriteGroup	    	= -7,
    papTooMuchOverlays	    	= -8,	/* current overlay is > MAX_OVERLAY */
    papTooMuchUINOverlays	= -9,	/* current overlay is > MAX_UINOVERLAY */
    papElemOfTypeOneNotFilled  	= -10,
    papMissingGr8		= -11,
    papImageFileNameNotDefind  	= -12,
    papLengthIsNotEven	    	= -13,
    papProblemInValue	    	= -14,
    papProblemInAscii	    	= -15,
    papProblemInDef	    	= -16,
    papTooManyImages	    	= -17,
    papEnumGroup		= -18,
    papIndexBigger		= -19,
    papNbImagesIsZero	    	= -20,
    papMaxOpenFile		= -21,
    papFileAlreadyExist	    	= -22,
    papFileCreationFailed	= -23,
    papReadingOpenFile	    	= -24,
    papNotFound		    	= -29,
    papOpenFile		    	= -30,	/* error opening file */
    papCLOSE_FILE		= -31,	/* error closing file */
    papReadFile		    	= -32,	/* error reading file */
    papWriteFile		= -33,	/* error writing to file */
    papFileName		    	= -34,	/* wrong filename */
    papPositioning		= -35,	/* error positioning the file pointer */
    papElemNumber		= -36,	/* wrong element number */
    papElemSize		    	= -37,	/* wrong element size */
    papGroupErr		    	= -38,	/* error in group */
    papGroupNumber		= -39,	/* wrong group number */
    papBadArgument		= -40,
    papListError		= -41,	/* pointer error in the list */
    papSyntaxNotImplemented    	= -42,	/* DICOM transfert syntax not implemented */
    papUnknownModality	    	= -43,	/* Imaging modality unknown in DICOM */
    papMissingModule	    	= -44,	/* a mandatory module is missing */
    papNotPapyrusFile	    	= -45,	/* this is not a Papyrus file */
    papDeleteFile		= -46,
    papWrongValue		= -47,	/* a wrong value was found in the file */
    papWrongPapVersion		= -48,	/* file version newer than version of the toolkit */
    papLastErrorNb		= -49
} PapyError3;



extern PapyError3   PapyErrNo;
extern char	    *PapyErrorList [];
extern int	    PapyErrLigne [];
extern char	    *PapyErrFile [];
extern void 	    Papy3CheckError (int, char *, int);


#ifdef NODEBUG
#define RETURN(CodeErr)	    return(CodeErr);
#else
#define RETURN(CodeErr)\
{\
    Papy3CheckError(CodeErr,__FILE__,__LINE__);\
    return(CodeErr);\
}
#endif

#endif /* __UINError.h__ */

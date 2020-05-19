/******************************************************************************/
/*									      */
/*	Project  : P A P Y R U S  Toolkit				      */
/*	File     : PapyErrorMacros3.h					      */
/*	Function : contains the fatal, warning and debug printing messages    */
/*	Authors  : Jean-Francois Vurlod					      */
/*	History  : 04.1992	version 1.2				      */
/*		   05.1993	version 2.0				      */
/*								   	      */
/*	(C) 1992, 1993 The University Hospital of Geneva		      */
/*		       All Rights Reserved				      */
/*									      */
/******************************************************************************/

#ifndef __Papy3ErrorMacros__
#define __Papy3ErrorMacros__


#include <stdio.h>


/* Eventually there should be a "SetFatalErrorHandler" function. */

#define PAPY_FATAL_ERROR(code,str)\
{\
    fprintf(stderr,\
	  "\nFatal Error -- Code: %d\n\tMessage: %s\n\tFile: %s   Line: %d\n",\
	  code,\
	  str,\
	  __FILE__,\
	  __LINE__);\
	  if (ExitWhenError) exit((int)code);\
}

#define PAPY_WARNING(code,str)\
{\
    fprintf(stderr,\
	  "\nWARNING -- Code: %d\n\tMessage: %s\n\tFile: %s   Line: %d\n",\
	  code,\
	  str,\
	  __FILE__,\
	  __LINE__);\
}

#ifdef DEBUG
#define PAPY_DEBUG(str)\
{\
    fprintf(stderr,\
	  "\nDEBUG -- Message: %s\n\tFile: %s   Line: %d\n",\
	  str,\
	  __FILE__,\
	  __LINE__);\
}
#else
#define PAPY_DEBUG(str)
#endif



#endif /* __Papy3ErrorMacros__ */

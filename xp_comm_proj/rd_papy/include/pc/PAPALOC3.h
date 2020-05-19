/********************************************************************************/
/*										*/
/*	Project  : P A P Y R U S  Toolkit					*/
/*	File     : PapyEalloc3.h 						*/
/*	Function : declaration of the fct of emalloc3		 		*/
/*	Authors  : Matthieu Funk						*/
/*		   Christian Girard						*/
/*		   Jean-Francois Vurlod						*/
/*		   Marianne Logean					        */
/*										*/
/*	History  : 12.1990	version 1.0					*/
/*		   07.1992	version 1.2					*/
/*		   05.1993	version 2.0					*/
/*		   06.1993	version 3.0					*/
/*		   06.1995	version 3.1					*/
/*								   		*/
/* 	(C) 1990, 1991, 1992, 1993, 1994, 1995, 1996				*/
/*		 The University Hospital of Geneva				*/
/*		       All Rights Reserved					*/
/*										*/
/********************************************************************************/

#ifndef PapyEalloc3H
#define PapyEalloc3H

#include <stddef.h>
#ifdef UNIX
#include <malloc.h>
#endif

#ifndef FILENAME83		/* this is for the normal machines ... */

#ifndef PapyTypeDef3H
#include "PapyTypeDef3.h"
#endif

#else				/* FILENAME83 defined for the DOS machines */

#ifndef PapyTypeDef3H
#include "PAPYDEF3.h"
#endif

#endif 				/* FILENAME83 defined */


#ifdef _NO_PROTO
extern void *emalloc3	();
extern void *ecalloc3	();
extern void *erealloc3	();
extern void  efree3	();
#else
extern void *emalloc3  (PapyULong);
extern void *ecalloc3  (PapyULong, PapyULong);
extern void *erealloc3 (void *, PapyULong);
extern void  efree3    (void **);
#endif

#endif

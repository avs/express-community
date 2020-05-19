/******************************************************************************/
/*									      */
/*	Project  : P A P Y R U S  Toolkit				      */
/*	File     : PapyEnumGroups3.h					      */
/*	Function : contains the declarations of the groups names and of the   */
/*		   elements names					      */
/*	Authors  : Christian Girard					      */
/*	History  : 04.1994	version 3.0				      */
/*								   	      */
/*	(C) 1994-1996 The University Hospital of Geneva			      */
/*		       All Rights Reserved				      */
/*									      */
/******************************************************************************/


#ifndef PapyEnumGroups3H 
#define PapyEnumGroups3H

#ifndef FILENAME83		/* this is for the normal machines ... */

#ifndef   PapyEnumImageGroups3H
#include "PapyEnumImageGroups3.h"
#endif

#ifndef   PapyEnumMiscGroups3H
#include "PapyEnumMiscGroups3.h"
#endif

#else				/* FILENAME83 defined for the DOS machines */

#ifndef   PapyEnumImageGroups3H
#include "PAPEIG3.h"
#endif

#ifndef   PapyEnumMiscGroups3H
#include "PAPEMG3.h"
#endif

#endif 				/* FILENAME83 defined */

/* 	enumeration of the groups	*/

enum groups {
Group2,
Group8,
Group10,
Group18,
Group20,
Group28,
Group32,
Group38,
Group41,
Group88,
Group2000,
Group2010,
Group2020,
Group2030,
Group2040,
Group2100,
Group2110,
Group4008,
Group5000,
Group6000,
UINOVERLAY,
Group7FE0,
END_GROUP
};


#endif

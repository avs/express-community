/********************************************************************************/
/*			                                                        */
/*	Project  : P A P Y R U S  Toolkit                                       */
/*	File     : Papyrus3.h                                                   */
/*	Function : contains the declarations of types, enumerated types,        */
/*                 constants and global variables                               */
/*	Authors  : Matthieu Funk                                                */
/*                 Christian Girard                                             */
/*                 Jean-Francois Vurlod                                         */
/*                 Marianne Logean                                              */
/*                                                                              */
/*	History  : 12.1990	version 1.0                                     */
/*                 04.1991	version 1.1                                     */
/*                 12.1991	version 1.2                                     */
/*                 06.1993	version 2.0                                     */
/*                 06.1994	version 3.0                                     */
/*                 06.1995	version 3.1                                     */
/*                 02.1996	version 3.3                                     */
/*                 02.1999	version 3.6                                     */
/*                 04.2001	version 3.7                                     */
/*                 09.2001      version 3.7  on CVS                             */
/*                 10.2001      version 3.71 MAJ Dicom par CHG                  */
/*                                                                              */
/* 	(C) 1990-2001                                                           */
/*	The University Hospital of Geneva                                       */
/*	All Rights Reserved                                                     */
/*                                                                              */
/********************************************************************************/


#ifndef Papyrus3H 
#define Papyrus3H

#ifdef FILENAME83	
#undef FILENAME83	
#endif

#include <string.h>

/* BEWARE : Mac must be defined only if we are on a Mac architecture. */

#ifndef unix
#ifndef hpux
#ifndef __alpha
#ifndef _WINDOWS
#ifndef Mac
#define Mac
#endif
#endif
#endif
#endif
#endif


#ifndef _WINDOWS
#ifndef Mac
#ifdef hpux
#include <sys/unistd.h>
#else
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#endif
#endif
#endif


#ifdef  PAPY3_VAR
#define WHERE3
#else
#define WHERE3 extern
#endif



/* --- includes --- */

#ifndef FILENAME83		/* this is for the normal machines ... */

#ifndef PapyEnumGroups3H 	/* enumeration of the groups */
#include "PapyEnumGroups3.h"
#endif
#ifndef PapyEnumModulesH 	/* enumeration of the modules */
#include "PapyEnumModules3.h"
#endif
#ifndef PapyTypeDef3H		/* PAPYRUS type definition */
#include "PapyTypeDef3.h"
#endif
#ifndef PapyGlobalVar3H		/* PAPYRUS global variables */
#include "PapyGlobalVar3.h"
#endif
#ifndef PapyPrivFunctionDef3H	/* PAPYRUS private functions */
#include "PapyPrivFunctionDef3.h"
#endif
#ifndef PapyPubFunctionDef3H	/* PAPYRUS public functions */
#include "PapyPubFunctionDef3.h"
#endif
#ifndef PapyEalloc3H		/* interface to the PAPYRUS memory manager */
#include "PapyEalloc3.h"
#endif
#ifndef PapyFileSystem3H	/* interface to the PAPYRUS file manager */
#include "PapyFileSystem3.h"
#endif
#ifndef PapyError3H		/* interface to the PAPYRUS error manager */
#include "PapyError3.h"
#endif
#include "PapyWild3.h"
#ifndef DicomdirH	
#include "DicomDir.h"
#endif

#else				/* FILENAME83 defined for the DOS machines */

#ifndef PapyTypeDef3H		/* PAPYRUS type definition */
#include "PAPTD3.h"
#endif
#ifndef PapyGlobalVar3H		/* PAPYRUS global variables */
#include "PAPGLV3.h"
#endif
#ifndef PapyPrivFunctionDef3H	/* PAPYRUS private functions */
#include "PAPPRF3.h"
#endif
#ifndef PapyPubFunctionDef3H	/* PAPYRUS public functions */
#include "PAPPUF3.h"
#endif
#ifndef PapyEnumGroups3H 
#include "PAPEG3.h"
#endif
#ifndef PapyEnumModulesH 
#include "PAPEM3.h"
#endif
#ifndef PapyEalloc3H
#include "PAPALOC3.h"
#endif
#ifndef PapyFileSystem3H
#include "PAPFSYS3.h"
#endif
#ifndef PapyError3H
#include "PAPERR3.h"
#endif
#include "PAPWILD3.h"
#ifndef DicomdirH	
#include "DICOMDIR.h"
#endif
#endif 				/* FILENAME83 defined */



#endif	    /* Papyrus3H */


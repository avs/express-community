/********************************************************************************/
/*				                                                */
/*	Project  : P A P Y R U S  Toolkit                                       */
/*	File     : PapyWildexit3.c                                              */
/*	Function : contains all program exit points                             */
/*	Authors  : Matthieu Funk                                                */
/*                 Christian Girard                                             */
/*                 Jean-Francois Vurlod                                         */
/*                 Marianne Logean                                              */
/*                                                                              */
/*	History  : 12.1990      version 1.0                                     */
/*                 04.1991      version 1.1                                     */
/*                 12.1991      version 1.2                                     */
/*                 06.1993      version 2.0                                     */
/*                 06.1994      version 3.0                                     */
/*                 06.1995      version 3.1                                     */
/*                 02.1996      version 3.3                                     */
/*                 02.1999      version 3.6                                     */
/*                 04.2001      version 3.7                                     */
/*                 09.2001      version 3.7  on CVS                             */
/*                 10.2001      version 3.71 MAJ Dicom par CHG                  */
/*                                                                              */
/* 	(C) 1990-2001                                                           */
/*	The University Hospital of Geneva                                       */
/*	All Rights Reserved                                                     */
/*                                                                              */
/********************************************************************************/

/* ------------------------- includes ---------------------------------------*/

#include <stdio.h>
#include <stdlib.h>


#ifndef FILENAME83		/* this is for the normal machines ... */

#include "PapyWild3.h"

#else				/* FILENAME83 defined for the DOS machines */

#include "PAPWILD3.h"

#endif 				/* FILENAME83 defined */



#ifndef EXIT_FAILURE
#define EXIT_FAILURE	-1
#endif
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS	0
#endif

static void (*	EX_funcp) ();
static int  ExitEnabled = 1;


/*******************************************************************************/
/*									       */
/*	ActiveExitWhenError :						       */
/*									       */
/*******************************************************************************/

void
ActiveExitWhenError ()
{
  ExitEnabled = 1;
} /* endof ActiveExitWhenError */


/*******************************************************************************/
/*									       */
/*	UnactiveExitWhenError : 					       */
/*									       */
/*******************************************************************************/

void
UnactiveExitWhenError ()
{
  ExitEnabled = 0;
} /* endof UnactiveWhenError */


/*******************************************************************************/
/*									       */
/*	exset : 							       */
/*									       */
/*******************************************************************************/

void
exset (void (* funcp) ())
{
  EX_funcp = funcp;
} /* endof exset */


/*******************************************************************************/
/*									       */
/*	wild2exit : 							       */
/*									       */
/*******************************************************************************/

void
wild2exit (char *inString1P, char *inString2P)
{
	/*
	** We should try to arrange for functions registerd using atexit
	** to be called before the output we generate goes to stderr.
	** For now. . .
	*/
  if (EX_funcp != NULL)
    EX_funcp ();
  if (ExitEnabled)
  {
    wild3 (inString1P, inString2P);
#ifndef DLL
    exit (EXIT_FAILURE);
#else
    abort ();
#endif
  }
  /*NOTREACHED*/
	
} /* endof wild2exit */


/*******************************************************************************/
/*									       */
/*	wildexit : 							       */
/*									       */
/*******************************************************************************/

void
wildexit (char *inStringP)
{
  wild2exit (inStringP, (char *) NULL);
  /*NOTREACHED*/
	
} /* endof wildexit */


/*******************************************************************************/
/*									       */
/*	wildcexit : 							       */
/*									       */
/*******************************************************************************/

void
wildcexit (char *inStringP)
{
  wild2exit ("call of", inStringP);
  /*NOTREACHED*/
	
} /* endof wildcexit */


/*******************************************************************************/
/*									       */
/*	wildrexit : 							       */
/*									       */
/*******************************************************************************/

void
wildrexit (char *inStringP)
{
  wild2exit ("result from", inStringP);
  /*NOTREACHED*/
	
} /* endof wildrexit */


/*******************************************************************************/
/*									       */
/*	tameexit : 							       */
/*									       */
/*******************************************************************************/

void
tameexit ()
{
  if (EX_funcp != NULL)
    EX_funcp ();
  if (ExitEnabled) 
#ifndef DLL
    exit (EXIT_SUCCESS);
#else
    abort ();
#endif
  /*NOTREACHED*/

} /* endof tameexit */

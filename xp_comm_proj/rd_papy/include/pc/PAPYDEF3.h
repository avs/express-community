/********************************************************************************/
/*										*/
/*	Project  : P A P Y R U S  Toolkit					*/
/*	File     : PapyTypeDef3.h						*/
/*	Function : contains the declarations of the basic types used in the  	*/
/*		   PAPYRUS toolkit.						*/
/*	Authors  : Christian Girard						*/
/*								   		*/
/*	History  : 03.1996	creation	version 3.3			*/
/*								   		*/
/* 	(C) 1996								*/
/*	The University Hospital of Geneva					*/
/*	All Rights Reserved							*/
/*										*/
/********************************************************************************/

#ifndef PapyTypeDef3H
#define PapyTypeDef3H



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

/* 		-------- some constant needed by the file managers -------- */

#ifndef SEEK_END
#define SEEK_END    2
#endif
#ifndef SEEK_CUR
#define SEEK_CUR    1
#endif
#ifndef SEEK_SET
#define SEEK_SET    0
#endif

#ifdef unix
#define NULLFILE    NULL
#else
#define NULLFILE    -1
#endif

/*			-------- basic types definition --------	*/
/* some type definitions to ensure compatibility between the different architectures */

#ifdef _WIN16_
typedef unsigned char 	__huge	PapyUHChar;	
typedef char 		__huge	PapyHChar;	
typedef unsigned short 	__huge	PapyUHShort;	
typedef short 		__huge	PapyHShort;	
#else
typedef unsigned char   PapyUHChar;	
typedef char            PapyHChar;
typedef unsigned short  PapyUHShort;	
typedef short 		PapyHShort;	
#endif

typedef	short		PapyShort;		/* 16 bits short int */
typedef unsigned short	PapyUShort;		/* 16 bits unsigned short int */

#ifdef __alpha
typedef	int		PapyLong;		/* 32 bits long  int */
typedef	unsigned int	PapyULong;		/* 32 bits unsigned long  int */
#else
typedef	long		PapyLong;		/* 32 bits long  int */
typedef	unsigned long	PapyULong;		/* 32 bits unsigned long  int */
#endif

typedef float		PapyFloat;		/* 32 bits float */
typedef double		PapyFloatDouble;	/* 64 bits float double */


/* ------------------------- type definition -----------------------------------*/

#ifdef Mac
typedef PapyShort 	PAPY_FILE; 	     /* the pointer to a PAPYRUS file */
#else
#ifdef _WINDOWS
typedef int            	PAPY_FILE;           /* Windows file handle */
#else
typedef FILE *      	PAPY_FILE;         
#endif
#endif


#endif /* PapyTypeDef3H */


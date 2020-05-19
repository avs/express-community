/*
 * libaiff.c -- library routines for dealing with AIFF
 *
 * 1991-1992 Charles J Williams III (chas@ra.nrl.navy.mil)
 * Naval Research Lab/Research Computation Division Visualization Lab
 */
/*
 *			 NRL/RCD Visualization Lab
 *
 * Source code and documentation developed at NRL/RCD Visualization Lab
 * are in the public domain. Hence, no limitation exists on the right to use,
 * publish, or resale the source code and documentation.
 *
 * We ask, but do not require, that the following message be included in all
 * derived works
 *
 * THE NAVAL RESEARCH LABORATORY GIVES NO WARRANTY, EXPRESSED OR IMPLIED, FOR
 * THE SOFTWARE AND/OR DOCUMENTATION PROVIDED, INCLUDING, WITHOUT LIMITATION,
 * WARRANTY OF MERCHANTABILITY AND WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE
 *
 */


/*
 *       International AVS Centre
 *
 * Slight modifications made so that library could be used within ReadAiff project
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
  To display debugging statements uncomment the following line
*/
// #define DEBUG


#ifndef SEEK_SET
#define SEEK_SET 0
#endif

#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif

#define kFORM "FORM"
#define kAIFF "AIFF"
#define kCOMM "COMM"
#define kSSND "SSND"

typedef char ID[4];
typedef unsigned long ulong;



typedef struct {
	ID	ckID;
	long 	ckDataSize;
	char 	*ckData;
} Chunk;

typedef struct {
	ID	ckID;		/* "FORM" */
	long	ckDataSize;
	
	ID	formType;	/* "AIFF" for AIFF files */
} FormChunk;

typedef struct {
	ID	ckID;		/* "COMM" */
	long	ckDataSize;
	
	short 	numChannels;
	ulong	numSampleFrames;
	short	sampleSize;
	double	sampleRate;
} CommChunk;

typedef struct {
	ID	ckID;
	long	ckDataSize;
	
	ulong	offset;
	ulong	blockSize;
	char	*soundData;
} SsndChunk;

/* global variables */
char *AIFFerr;

char* getAIFFerr() {
  return AIFFerr;
}


/* function definitions */
int ReadChunk(FILE *file,Chunk *chunk );
void ReadFormChunk(FILE *file,FormChunk *chunk );
int ReadCommChunk(FILE *file,CommChunk *chunk );
int ReadSsndChunk(FILE *file,SsndChunk *chunk );
int ReadID(FILE *file,ID *ckID );
int ReadLong(FILE *file,long *val );
int ReadShort(FILE *file,short *val );
int ReadULong(FILE *file,ulong *val );
int ReadExtended(FILE *file,double *val );
double ConvertFromIeeeExtended(unsigned char *bytes);
int readCOMMandSSNDchunks(FILE *file ,CommChunk *commChunk,SsndChunk *ssndChunk);


int
readAIFFfile(FILE *audioFile ,int *nChannels,int *nSamples,int *sSize,double *sRate,char **sData)
{
	static FormChunk formChunk;
	static CommChunk commChunk;
	static SsndChunk ssndChunk;

	ReadFormChunk( audioFile, &formChunk );
	
	if ( strncmp( formChunk.ckID, kFORM, 4 )!=0 )
	{
		AIFFerr = "not a valid Interchange Format File";
		return -1;
	}

	if ( strncmp( formChunk.formType, kAIFF, 4)!=0 )
	{
		AIFFerr = "not a uncompressed AIFF audio file";
		return -1;
	}

	if ( readCOMMandSSNDchunks(audioFile, &commChunk, &ssndChunk) )
		return 0;

	*nChannels = commChunk.numChannels;
	*nSamples = commChunk.numSampleFrames;
	*sSize = commChunk.sampleSize;
	*sRate = commChunk.sampleRate;
	*sData = (char *) ssndChunk.soundData;
	return 1;
}

int
readCOMMandSSNDchunks(FILE *file ,CommChunk *commChunk,SsndChunk *ssndChunk)
{
	Chunk chunk;
	int haveCOMM = 0, haveSSND = 0;
	int i;

	AIFFerr = "missing SSND or COMM chunk";

	for(;;)
	{
		if ( ! ReadChunk( file, &chunk ) )
		{
			return(-1);
		}

		if ( strncmp( chunk.ckID, kCOMM, 4) == 0 )
		{
#ifdef DEBUG
			fprintf(stderr,"reading COMM Chunk...\n");
#endif
			fseek( file, -8L, SEEK_CUR );
			if ( ! ReadCommChunk( file, commChunk ) )
				return -1;
			haveCOMM = 1;
			AIFFerr = "missing SSND chunk";
		}
		else if ( strncmp( chunk.ckID, kSSND, 4 ) == 0 )
		{
#ifdef DEBUG
			fprintf(stderr,"reading SSND Chunk...\n");
#endif
			fseek( file, -8L, SEEK_CUR );
			if ( ! ReadSsndChunk( file, ssndChunk ) )
				return -1;
			haveSSND = 1;
			AIFFerr = "missing COMM Chunk";
		}
		else 
		{
#ifdef DEBUG
			fprintf(stderr,"skipping unknown Chunk '");
			for(i=0; i<4; ++i)
				fputc(chunk.ckID[i], stderr);
			fprintf(stderr,"'...\n");
#endif
			
			fseek( file, chunk.ckDataSize, SEEK_CUR );
			if ( chunk.ckDataSize % 2 == 1 )
				fseek( file, 1L, SEEK_CUR );
		}

		if ( haveCOMM && haveSSND )
		{
			AIFFerr = "no error";
			return(0);
		}

	}
}
		
int
ReadChunk(FILE *file,Chunk *chunk )
{
	if ( ! ReadID( file, &chunk->ckID ) ) return 0;
	if ( ! ReadLong( file, &(chunk->ckDataSize)) ) return 0;
	return 1;
}

void
ReadFormChunk(FILE *file,FormChunk *chunk )
{
	ReadID( file, &chunk->ckID );
	ReadLong( file, &(chunk->ckDataSize));
	ReadID( file, &chunk->formType );
}

int
ReadCommChunk(FILE *file,CommChunk *chunk )
{
	AIFFerr = "error reading COMM chunk";
	if ( ! ReadID(file, &chunk->ckID ) ) return 0;
	if ( ! ReadLong( file, &chunk->ckDataSize ) ) return 0;
	if ( ! ReadShort( file, &chunk->numChannels ) ) return 0;
	if ( ! ReadULong( file, &chunk->numSampleFrames ) ) return 0;
	if ( ! ReadShort( file, &chunk->sampleSize )) return 0;
	if ( ! ReadExtended( file, &chunk->sampleRate ) ) return 0;
	return 1;
}

int
ReadSsndChunk(FILE *file,SsndChunk *chunk )
{
	unsigned int rawsize;

  AIFFerr = "read error for SSND header data";

	if ( ! ReadID(file, &chunk->ckID ) ) return 0;
	if ( ! ReadLong( file, &chunk->ckDataSize ) ) return 0;
	if ( ! ReadULong( file, &chunk->offset ) ) return 0;
	if ( ! ReadULong( file, &chunk->blockSize ) ) return 0;

	rawsize = chunk->ckDataSize - chunk->offset - 8;
	chunk->soundData = (char *) malloc( rawsize );

	fseek( file, (long) chunk->offset, SEEK_CUR );
	if ( fread( chunk->soundData, sizeof(char), rawsize, file ) != rawsize)
	{
		AIFFerr = "read error for SSND sound data";
		return 0;
	}

	return 1;
}



int
ReadID(FILE *file,ID* ckID )
{
	if (fread( (void *) ckID, sizeof(char), 4, file ) != 4) return 0;
	return 1;
}

int
ReadLong(FILE *file,long *val )
{
	unsigned char b[4];
	int i;
	
	for( i=0; i<4; ++i)
		if (fread( (void *) &b[i], sizeof(char), 1, file ) != 1 )
			return 0;
	*val = b[0]*16777216 + b[1]*65536 + b[2]*256 + b[3];
	return 1;
}

int 
ReadShort(FILE *file,short *val )
{
	unsigned char b[2];
	int i;

	for ( i=0; i<2; ++i)
		if ( fread( (void *) &b[i], sizeof(char), 1, file) != 1 )
			return 0;
	*val = b[0]*256 + b[1];
	return 1;
}
	
int
ReadULong(FILE *file,ulong *val )
{
	unsigned char b[4];
	int i;
	
	for( i=0; i<4; ++i)
		if (fread( (void *) &b[i], sizeof(char), 1, file ) != 1 )
			return 0;
	*val = b[0]*16777216 + b[1]*65536 + b[2]*256 + b[3];
	return 1;
}

int
ReadExtended(FILE *file,double *val )
{
	unsigned char b[10];	/* ieee extended is 80 bits */
	
	if ( fread( &b[0], sizeof(char), 10, file ) != 10 ) return 0;
	*val = ConvertFromIeeeExtended(b);
	return 1;
}
	

/* IAC CODE CHANGE : #include <math.h> */
#include <avs/avs_math.h>

/*
 * C O N V E R T   F R O M   I E E E   E X T E N D E D  
 */

/* 
 * Copyright (C) 1988-1991 Apple Computer, Inc.
 * All rights reserved.
 *
 * Machine-independent I/O routines for IEEE floating-point numbers.
 *
 * NaN's and infinities are converted to HUGE_VAL or HUGE, which
 * happens to be infinity on IEEE machines.  Unfortunately, it is
 * impossible to preserve NaN's in a machine-independent way.
 * Infinities are, however, preserved on IEEE machines.
 *
 * These routines have been tested on the following machines:
 *    Apple Macintosh, MPW 3.1 C compiler
 *    Apple Macintosh, THINK C compiler
 *    Silicon Graphics IRIS, MIPS compiler
 *    Cray X/MP and Y/MP
 *    Digital Equipment VAX
 *
 *
 * Implemented by Malcolm Slaney and Ken Turkowski.
 *
 * Malcolm Slaney contributions during 1988-1990 include big- and little-
 * endian file I/O, conversion to and from Motorola's extended 80-bit
 * floating-point format, and conversions to and from IEEE single-
 * precision floating-point format.
 *
 * In 1991, Ken Turkowski implemented the conversions to and from
 * IEEE double-precision format, added more precision to the extended
 * conversions, and accommodated conversions involving +/- infinity,
 * NaN's, and denormalized numbers.
 */

#ifndef HUGE_VAL
# define HUGE_VAL HUGE
#endif /*HUGE_VAL*/

# define UnsignedToFloat(u)         (((double)((long)(u - 2147483647L - 1))) + 2147483648.0)

/****************************************************************
 * Extended precision IEEE floating-point conversion routine.
 ****************************************************************/

double ConvertFromIeeeExtended(unsigned char *bytes)
{
    double    f;
    int    expon;
    unsigned long hiMant, loMant;
    
    expon = ((bytes[0] & 0x7F) << 8) | (bytes[1] & 0xFF);
    hiMant    =    ((unsigned long)(bytes[2] & 0xFF) << 24)
            |    ((unsigned long)(bytes[3] & 0xFF) << 16)
            |    ((unsigned long)(bytes[4] & 0xFF) << 8)
            |    ((unsigned long)(bytes[5] & 0xFF));
    loMant    =    ((unsigned long)(bytes[6] & 0xFF) << 24)
            |    ((unsigned long)(bytes[7] & 0xFF) << 16)
            |    ((unsigned long)(bytes[8] & 0xFF) << 8)
            |    ((unsigned long)(bytes[9] & 0xFF));

    if (expon == 0 && hiMant == 0 && loMant == 0) {
        f = 0;
    }
    else {
        if (expon == 0x7FFF) {    /* Infinity or NaN */
            f = HUGE_VAL;
        }
        else {
            expon -= 16383;
            f  = ldexp(UnsignedToFloat(hiMant), expon-=31);
            f += ldexp(UnsignedToFloat(loMant), expon-=32);
        }
    }

    if (bytes[0] & 0x80)
        return -f;
    else
        return f;
}



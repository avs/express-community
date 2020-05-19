/*******************************************************************
**	Read Tecplot-Data-Files
**
**	Copyright (c) 1997 DLR
********************************************************************
**	Created:	01/97, 08/97, 11/97
**	by:		I.Hallstein
**
**	DLR, WT-DV
**	Linder Hoehe
**	D-51147 Cologne
**	Germany
*******************************************************************/

#ifndef	READ_TECPLOT_h
#define READ_TECPLOT_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* debug flags */
#define ECHO    0
#define ECHOX   0

/*#define DEBUG*/

#define	DIMS	3

typedef struct TecBlock
{
    char	*title;
    int		dim[DIMS];
    int		nVar;
    int		nVal;
    double	**val;
    struct TecBlock	*next;
} TecBlock;

typedef struct
{
    char	*title;
    int		nVar;
    char	**varNames;
    int		nBlocks;
    TecBlock	*blocks;
} TecMultiBlock;

TecMultiBlock	tecMb;
TecBlock	*lastTecBlock;


typedef enum
{
    NullToken,
    TITLEToken,
    VARIABLESToken,
    ZONEToken,
    GEOMETRYToken,
    CUSTOMLABELToken,
    USERRECToken,     
    TEXTToken,
    IdentToken,
    StringToken,
    ValueToken,
    EqualsToken,
    LeftParenToken,
    RightParenToken,
    EndofFileToken,
    EndofVarLineToken,
    DupConnectToken,
    UnknownToken
} Tokens;

typedef enum
{
    FieldDataType_Null,
    FieldDataType_Float,
    FieldDataType_Double,
    FieldDataType_LongInt,
    FieldDataType_ShortInt,
    FieldDataType_Byte,
    FieldDataType_Bit
} FieldDataType;

typedef enum
{
    TRIANGLE,
    QUADRILATERAL,
    TETRAHEDRON,
    BRICK,
    BADELEMENT = 99
} ElementType;

typedef enum
{
    LINE,
    RECTANGLE,
    SQUARE,
    CIRCLE,
    ELLIPSE,
    LINE3D
} GeomType;

typedef enum
{
    IJKBLOCK,
    IJKPOINT,
    FEBLOCK,
    FEPOINT
} FormatType;

typedef struct NodeVarStruct
{
    int	ii;
    int	val;
} NodeVarType;


typedef	char	*StringPtr;
typedef char	Boolean;

#define	ROUND2(x)	((x) >= 0 ? ((int)(x+0.49)) : ((int)(x-0.49)))

#define	FALSE	0
#define	TRUE	1

#define	NUM_CHARS_VAR_NAME	32
#define NUM_CHARS_ZONE_NAME	32
#define	MAX_TEXT_STRING	300
#define	MAX_LINE_INDEX	4023
#define	MAX_NUM_VAR	2048
#define	MAX_NUM_ZONE	5000
/*
#define LARGESTINDEX	2000000000
*/
#define LARGESTINDEX	2000000
#define	SET_SIZE	200


/*
int	ECHO;
int	ECHOX;
int	DEBUG;
*/

Tokens	curToken;
char	curData[MAX_TEXT_STRING];
double	curTValue;

Boolean	ok;
int	repeatLength;
double	repeatValue;

int	numVar;
int	numZone;

StringPtr	zoneName[MAX_NUM_ZONE];
/* StringPtr	varName[MAX_NUM_VAR+1]; */
StringPtr	vName[MAX_NUM_VAR];
FieldDataType	varDataType[MAX_NUM_VAR+1];
/*FormatType	zoneFormat[MAX_NUM_ZONE]; */

int	iStart[MAX_NUM_ZONE+1];
int	iEnd[MAX_NUM_ZONE+1];
int	iSkip[MAX_NUM_ZONE+1];
int	jStart[MAX_NUM_ZONE+1];
int	jEnd[MAX_NUM_ZONE+1];
int	jSkip[MAX_NUM_ZONE+1];
int	kStart[MAX_NUM_ZONE+1];
int	kEnd[MAX_NUM_ZONE+1];
int	kSkip[MAX_NUM_ZONE+1];

#ifdef _WIN64
unsigned __int64 zoneSet[SET_SIZE];
#else
unsigned long	zoneSet[SET_SIZE];
#endif

NodeVarType	*nodeVarList;
int		lastNodeInList;


ElementType        element;
ElementType        oldElement;
FormatType         format;
FormatType         oldFormat;

int	iMax, jMax, kMax;
int	oldIMax, oldJMax, oldKMax;
int	nodeVar;

int	numDupVar;
Boolean	dupVar[MAX_NUM_VAR+1];
Boolean	dupConnect;

int	linePos;
int	lineLen;
int	lineNumber;
char	line[MAX_LINE_INDEX+1];
char	textString[MAX_TEXT_STRING];
char	headerString[MAX_TEXT_STRING+1];

#define	NCOLOR	15
static char *tecplotColor[]	= { "BLACK", "RED", "GREEN", "BLUE",  "CYAN", "YELLOW",
			    "WHITE", "CUST1","CUST2", "CUST3", "CUST4", "CUST5"
			    "CUST6", "CUST7","CUST8" };

#endif

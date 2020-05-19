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

#include "gen.h"
#include "rdtecpl.h"

/******************************************************************/
/*
**	print errormessage
*/
void
Error (char *str)
{
    int i;

    ok	= FALSE;
/*    fprintf (stderr, "ERROR: %s\n", str);*/

    /* clean */
    for (i = 0; i < MAX_NUM_ZONE; i++)
	if (zoneName[i])
	    free (zoneName[i]);
    for (i = 0; i < MAX_NUM_VAR; i++)
	if (vName[i])
	    free (vName[i]);

/*
    for (i = 0; i < MAX_CUSTOM_ABEL ; i++)
	if (customLabel[i])
	    free (customLabel[i]);
	    */

    /* Speicher von TecMB freigeben */
    /* ... */
/*    exit (-1);*/
}

/******************************************************************/
/*
**
*/
TecBlock *NewTecBlock ()
{
    TecBlock    *block;
    int         i;
 
    block       = (TecBlock*) malloc (sizeof (TecBlock));
    block->title        = NULL;
    for (i = 0; i < DIMS; i++)
        block->dim[i]   = 0;
    block->nVar         = 0;
    block->nVal         = 0;
    block->val          = NULL;
    block->next = NULL;
 
    return (block);
}

/******************************************************************/
/*
**	
*/
Boolean
CheckTokenStr (char *str)
{
    char s[150];
    int i = -1;

    while (curData[++i] != '\0')
	if (islower (curData[i]))
	    s[i] = toupper (curData[i]);
	else
	    s[i] = curData[i];
    s[i] = '\0';
    return (!strcmp (s, str));
}

/******************************************************************/
/*
**	
*/
Boolean
CompareIdent (char *str, char *id)
{
    char *strPtr = str;
    char *idPtr = id;

    while (*strPtr) {
	if ((*strPtr & 223) != *idPtr)
	    return (FALSE);
	strPtr++;
	idPtr++;
    }
    return ((Boolean)(*idPtr == '\0'));
}

/******************************************************************/
/*
**	
*/
Boolean
GetALine (FILE *fptr)
{
#if defined OLDCODE
    short c;
#endif
    
    if (feof (fptr))
	return (FALSE);
    
    if (fgets (line, MAX_LINE_INDEX, fptr) == NULL)
	lineLen = 0;
    else
	lineLen = strlen (line) - 1;
    if (lineLen == MAX_LINE_INDEX)
	fprintf (stderr, "line too long");
    
#if defined OLDCODE
    lineLen = 0;
    while (((c = fgetc (fptr)) != EOF) && (c != '\n')) {
	line[lineLen++] = c;
	if (lineLen > MAX_LINE_INDEX)
	    fprintf (stderr, "line too long");
    }
#endif
 
    line[lineLen]	= '\0';
    if ((lineLen == 0) && feof (fptr))
	return (FALSE);
    
    return (TRUE);
}

/******************************************************************/
/*
**	get next line
*/
Boolean
GetNextLine (FILE *fptr)
{
    Boolean done = FALSE;

    curData[0]	= '\0';
 
if (ECHO) printf ("*** in GetNextLine\n");
    while (!done) {
	if (!GetALine (fptr))
	    return (FALSE);
	else
	    done = (*line && (*line != '#'));
	lineNumber++;
    }
 
    if (ECHOX) printf("==> Zeile=%5d:	line='%s'\n", lineNumber, line);
    linePos = -1;

    return (TRUE);
}

/******************************************************************/
/*
**	get next Token
*/
void
GetToken (Boolean treatParenAsAlpha, Boolean okToGetNextLine, FILE *fptr)
{
    int	i;
    int	tempR;
    char	s[40];
    double	dValue;
/*    double	absDValue;*/
    Boolean	decimalOrEFound;
    Boolean	eFound;
    short	c;
    
    if (repeatLength > 0) {
	curTValue	= repeatValue;
	curToken	= ValueToken;
	repeatLength--;
	return;
    }

    curData[0]	= '\0';
    curToken	= NullToken;

    /*linePos = 0;*/

if (ECHO) printf ("\n*** in GetToken\n");

    while ((line[linePos] == ' ')	||
	   (line[linePos] == ',')	||
	   (line[linePos] == '\r')	||
	   (line[linePos] == '\t')	||
	   (line[linePos] == '\\')	||
	   (linePos >= lineLen)) {

	if (linePos >= lineLen) {
	    if (okToGetNextLine) {
		if (!GetNextLine (fptr)) {
		    curToken	= EndofFileToken;
		    if (ECHO) printf ("..!GetNext...EndofFileToken\n");
		    return;
		}
	    }
	    else {
		curToken	= EndofVarLineToken;
		return;
	    }
	}
	linePos++;
    }
    if (line[linePos] == '=') {
	curToken	= EqualsToken;
	if (ECHO) printf (" '='	;linePos=%d\n", linePos);
	linePos++;
    }
    else if ((line[linePos] == '(') && !treatParenAsAlpha) {
	curToken	= LeftParenToken;
	if (ECHO) printf (" '('	;linePos=%d\n", linePos);
	linePos++;
    }
    else if ((line[linePos] == ')') && !treatParenAsAlpha) {
	curToken	= RightParenToken;
	if (ECHO) printf (" ')'	;linePos=%d\n", linePos);
	linePos++;
    }
    else if ((line[linePos] == '-') &&
	     !(isdigit (line[linePos+1]) || (line[linePos+1] == '.')))
	curToken	= UnknownToken;
    else if ((isdigit(line[linePos])) || (line[linePos] == '.')   ||
	     (line[linePos] == '+')   || (line[linePos] == '-')) {
	curToken	= ValueToken;
	if (ECHO) printf (" digit,'.','+','-'	;linePos=%d\n", linePos);
	decimalOrEFound	= FALSE;
	eFound	= FALSE;

	i	= 0;
	while ((i < 40) &&
	       (curToken == ValueToken) &&
	       (linePos < lineLen) &&
	       (isdigit (c = line[linePos]) || (c == 'E') || (c == 'D')
		|| (c == 'e') || (c == 'd') || (c == '.')
		|| (c == '+') || (c == '-'))) {

	    if (isalpha (c)) {
		eFound	= TRUE;
		c	= 'E';
	    }
	    s[i++] = c;
	    if ((c == '.') || (c == 'E'))
		decimalOrEFound = TRUE;
	    else if (c == '-') {
		if (eFound) {
		    if (!isalpha(line[linePos-1]))
			curToken	= UnknownToken;
		}
		else if (i > 1) {
		    eFound	= TRUE;
		    s[i-1]	= 'E';
		    s[i++]	= '-';
		}
	    }
	    else if (!eFound && (c == '+') && (i > 1)) {
		eFound	= TRUE;
		s[i-1]	= 'E';
		s[i++]	= '+';
	    }
	    linePos++;
	}
	
	s[i]	= '\0';
	if (decimalOrEFound) {
	    if (sscanf (s, "%le", &dValue) == EOF)
		curToken	= UnknownToken;
	    else
		curTValue	= dValue;
	}
	else {
	    if (sscanf (s, "%d", &i) == EOF)
		curToken	= UnknownToken;
	    else
		curTValue	= i;
	}
	if (line[linePos] == '*') { /* repeat */
	    tempR	= (int)(curTValue+0.01) - 1;
	    linePos++;
	    GetToken (FALSE, TRUE, fptr);
	    
	    if (curToken == ValueToken) {
		repeatValue	= curTValue;
		repeatLength	= tempR;
	    }
	    else
		fprintf (stderr, "bad repeat\n");
	}
    }
    else if (line[linePos] == '"') {	/* stringToken */
	curToken	= StringToken;
	if (ECHO) printf ("String; (linePos: von %d bis ", linePos);
	i	= 0;
	linePos++;
	while ((i < MAX_LINE_INDEX)	&& (i < MAX_TEXT_STRING) &&
	       (linePos < lineLen)	&& (line[linePos] != '"')) {
	    if ((line[linePos] == '\\') && ((linePos < lineLen-1)))
		linePos++;
	    if (!isprint (line[linePos]))
		curData[i++]	= ' ';
	    else
		curData[i++]	= line[linePos];
	    linePos++;
        }
	if ((linePos < lineLen) && (line[linePos] == '"')) {
	    linePos++;
	    curData[i]	= '\0';
	    if (ECHO) printf ("%d)	string= '%s'\n", linePos, curData);
	}
	else {
	    curData[0]	= '\0';
	    curToken	= UnknownToken;
	}
    }
    else {
	curToken	 = IdentToken;
	if (ECHO) printf ("else IdentToken	;linePos=%d\n", linePos);
	i	= 0;
	while ((i < MAX_LINE_INDEX)	&& (i < MAX_TEXT_STRING)	&&
	       (linePos < lineLen)     	&& (line[linePos] != ' ')	&&
	       (line[linePos] != ',')	&&
	       (treatParenAsAlpha	|| (line[linePos] != ')'))	&&
	       (line[linePos] != '\t')	&& (line[linePos] != '\r')	&&
	       (line[linePos] != '\\')	&& (line[linePos] != '=')) {
	    if (!isprint (line[linePos]))
		curData[i++]	= ' ';
            else
		curData[i++]	= line[linePos];
            linePos++;
	}
	curData[i] = '\0';
	if (ECHO) printf ("curData= '%s'	;linePos=%d\n", curData, linePos);
 
	if (CheckTokenStr ("ZONE")) {
	    curToken	= ZONEToken;
	    if (ECHO) printf ("ZONEToken ...\n");
	}
	else if (CheckTokenStr ("FECONNECT")) {
	    curToken	= DupConnectToken;
	    if (ECHO) printf ("DupConnectToken ...\n");
	}
	else if (CheckTokenStr ("TITLE")) {
	    curToken	= TITLEToken;
	    if (ECHO) printf ("TITLEToken ...\n");
	}
	else if (CheckTokenStr ("VARIABLES")) {
	    curToken	= VARIABLESToken;
	    if (ECHO) printf ("VARIABLESToken ...\n");
	}
	else if (CheckTokenStr ("GEOMETRY")) {
	    curToken	= GEOMETRYToken;
	    if (ECHO) printf ("GEOMETRYToken ...\n");
	}
	else if (CheckTokenStr ("TEXT")) {
	    curToken	= TEXTToken;
	    if (ECHO) printf ("TEXTToken ...\n");
	}
	else if (CheckTokenStr ("CUSTOMLABELS")) {
	    curToken	= CUSTOMLABELToken;
	    if (ECHO) printf ("CUSTOMLABELToken ...\n");
	}
	else if (CheckTokenStr ("USERREC")) {
	    curToken	= USERRECToken;
	    if (ECHO) printf ("USERRECToken ...\n");
	}
    }
}

/******************************************************************/
/*
**	
*/
void
StuffString (char **strPtr, char *str, int max)
{
    int lenUsed;
 
    if (*strPtr != NULL)
	free (*strPtr);
 
    lenUsed	= strlen (str) - 1;
    while (lenUsed > 0 && str[lenUsed] == ' ')
	lenUsed--;
    lenUsed++;
 
    if (lenUsed > max)
	lenUsed	= max;
 
    (*strPtr)	= (char *) malloc (lenUsed+1);
    if ((*strPtr) == NULL)
	Error ("Out of heap space for strings");
 
    strncpy (*strPtr, str, lenUsed);
    (*strPtr)[lenUsed] = '\0';
}

/******************************************************************/
/*
**
*/
void
StuffGenericVarName (char **strPtr, int ind)
{
    char str[10];
    sprintf (str,"V%-4d",ind+1);
    StuffString (strPtr, str, NUM_CHARS_VAR_NAME);
}

/******************************************************************/
/*
**	copy the current String
*/
void
CopyCurData (char *str, int max)
{
    int len;

    curData[max]	= '\0';
    strcpy (str, curData);
    len	= strlen (str) -1;
    while ((len > 0) && (str[len] == ' '))
	len--;
    str[len+1]	= '\0';
}

/******************************************************************/
/*
**	remove the Sign '='
*/
Boolean
RemoveEqualsSign (Boolean treatParenAsAlpha, FILE *fptr)
{
    if (ECHO) printf ("*** in RemoveEqualsSign\n");
    GetToken (treatParenAsAlpha, TRUE, fptr);
    if (curToken == EqualsToken) {
	GetToken (treatParenAsAlpha,TRUE, fptr);
	return (TRUE);
    }
    else
	return (FALSE);
}

/******************************************************************/
/*
**	
*/
void
RecogVarNames (FILE *fptr)
{
    numVar = 0;

    if (ECHO) printf ("*** in RecogVarNames\n");
    if (RemoveEqualsSign (TRUE, fptr)) {
	do {
	    if ((curToken == StringToken) || (curToken == IdentToken))	{
              if (numVar >= MAX_NUM_VAR)
		  Error ("Maximum number of variables exceeded");
              StuffString (&vName[numVar], curData, NUM_CHARS_VAR_NAME);
              if (ECHO) printf("var: %s\n", vName[numVar]);
              numVar++;
            }
	    else
		Error ("Bad variable name for variable");
	    GetToken (TRUE, TRUE, fptr);
        } while (ok && (curToken == StringToken || curToken == IdentToken));
    }
    else
	Error ("Missing Equals sign after 'VARIABLES' in header");
}

/******************************************************************/
/*
**	ZONE x, D=(duplist)
**		^^^^^^^^^^^
*/
void
GetDupInfo (FILE *fptr)
{
    if (RemoveEqualsSign (FALSE, fptr)) {
	if (curToken != LeftParenToken)
	    Error ("Expecting a left parenthesis");
 
        if (ok)
	    GetToken (FALSE, TRUE, fptr);

	if (ok && (curToken != ValueToken && curToken != DupConnectToken))
	    Error ("Expecting a variable number");
 
	while (ok && (curToken == ValueToken || curToken == DupConnectToken)) {
	    if (curToken == ValueToken) {
              if (curTValue >= 1.0 && curTValue <= (double)numVar) {
                  dupVar[(int)curTValue-1]	= TRUE;
                  numDupVar++;
                  GetToken (FALSE, TRUE, fptr);
	      }
              else
		  Error ("Invalid value for dup var Assignment");
            }
	    else {
		dupConnect	= TRUE;
		GetToken (FALSE, TRUE, fptr);
            }
        }
	if (ok && curToken != RightParenToken)
	    Error ("Expecting a value, FECONNECT, or a right parenthesis");
    }
    else
	Error ("Expecting equals sign");
}

/******************************************************************/
/*
**	get Tecplot color
**
**	... C="colorname"
*/
int
GetColor (FILE *fptr)
{
    int i;

    if (RemoveEqualsSign (FALSE, fptr)) {
	for (i = 0; i < NCOLOR; i++)
	    if (CheckTokenStr (tecplotColor[i]))
		return i;
	Error ("Invalid Color");
    }
    else
	Error ("Missing Equals Sign in Color Assignment");
    return 0; /* default color: black */
}

/******************************************************************/
/*
**	get an integer
**
**	...=interger-value
**	   ^^^^^^^^^^^^^^^
*/
int
GetInteger (FILE *fptr)
{
 
    if (RemoveEqualsSign (FALSE, fptr)) {
	if (curToken != ValueToken)
	    Error ("Expecting a value");

	if (ECHO) printf ("*** in GetInteger\n");
	if (abs (curTValue) < 200000000.0) {
	    return abs (curTValue);
	}
	else
	    Error ("Value out of range");
    }
    else
	Error ("Missing an Equals sign");
    return 0; /* default */
}

/******************************************************************/
/*
**	get element-type
**
**	ZONE x, ET=element-type
**		  ^^^^^^^^^^^^^
*/
void
GetElementType (FILE *fptr)
{
    if (RemoveEqualsSign (FALSE, fptr)) {
	if (CheckTokenStr ("TRIANGLE"))
	    element	= TRIANGLE;
	else if (CheckTokenStr ("QUADRILATERAL"))
	    element	= QUADRILATERAL;
	else if (CheckTokenStr ("TETRAHEDRON"))
	    element	= TETRAHEDRON;
	else if (CheckTokenStr ("BRICK"))
	    element	= BRICK;
	else if (CheckTokenStr ("CUBE")) {
	    Error ("Element type of CUBE will be phased out.  Use BRICK");
	    element	= BRICK;
        }
	else
	    Error ("Invalid Element type, Need TRIANGLE, QUADRILATERAL, TETRAHEDRON, or BRICK");
    }
    else
	Error ("Missing Equals sign for Format Specifier");
}

/******************************************************************/
/*
**	get data-types (for all variables)
**
**	ZONE x, DT=(datatypes)
**		  ^^^^^^^^^^^^
*/
void
GetDataType (FieldDataType *dataType, Boolean isVar, FILE *fptr)
{
    Boolean done	= FALSE;
    int     i	= 0;

    if (!RemoveEqualsSign (FALSE, fptr))
	Error ("Missing Equals Sign in Data Type Assignment");
 
    if (isVar) {
	if (curToken != LeftParenToken)
	    Error ("Expecting a left parenthesis");
	if (ok)
	    GetToken (FALSE, TRUE, fptr);
    }
 
    while (ok && !done && (i < MAX_NUM_VAR)) {
	if (CheckTokenStr ("SINGLE") || CheckTokenStr ("FLOAT"))
	    dataType[i]	= FieldDataType_Float;
	else if (CheckTokenStr ("DOUBLE"))
	    dataType[i] = FieldDataType_Double;
	else {
	    if (!isVar)
		Error ("Only allowed to use LONGINT, SHORTINT,\nBYTE and BIT data types with zone field data");
	    if (CheckTokenStr ("LONGINT"))
		dataType[i]	= FieldDataType_LongInt;
	    else if (CheckTokenStr ("SHORTINT"))
		dataType[i]	= FieldDataType_ShortInt;
	    else if (CheckTokenStr ("BYTE"))
		dataType[i]	= FieldDataType_Byte;
	    else if (CheckTokenStr ("BIT"))
		dataType[i]	= FieldDataType_Bit;
	    else
		Error ("Expecting a Data type or right parenthesis");
        }
	i++;
	if (isVar) {
	    GetToken (FALSE, TRUE, fptr);
	    if (curToken == RightParenToken)
		done 	= TRUE;
        }
	else
	    done = TRUE;
    }
}

/******************************************************************/
/*
**	get format
**
**	ZONE x, F=format
**		 ^^^^^^^
*/
void
GetFormat (FILE *fptr)
{
    if (RemoveEqualsSign (FALSE, fptr)) {
	if (CheckTokenStr ("POINT"))
	    format	= IJKPOINT;
	else if (CheckTokenStr ("BLOCK"))
	    format	= IJKBLOCK;
	else if (CheckTokenStr ("FEBLOCK"))
	    format	= FEBLOCK;
	else if (CheckTokenStr ("FEPOINT"))
	    format	= FEPOINT;
	else
	    Error ("Invalid Format, Need POINT, BLOCK, FEPOINT, or FEBLOCK");
    }
    else
	Error ("Missing Equals sign for Format Specifier");
}

/******************************************************************/
/*
**	test element in set
*/
Boolean
InSet (size_t i, size_t *set)
{
    size_t	word;
    size_t	mask;
 
    mask	= (size_t)1 << (i % 32);
    word	= i / 32;
    return ((Boolean)((mask & set[word]) != 0));
}
 
 /******************************************************************/
/*
**	test the set
*/
Boolean
IsSetNull (size_t *set, size_t n)
{
    int i;
    for (i = 0; i < n; i++)
	if ((long int) set[i] != (int)0)
	    return ((Boolean)0);
    return ((Boolean)1);
}

/******************************************************************/
/*
**	add element to set
*/
void
AddToSet (size_t i, size_t *set)
{
    size_t	word = i / 32;
    set[word]	= (set[word] | ((size_t)1 << (i % 32)));
}

/******************************************************************/
/*
**	init the set
*/
void
SetToNull (size_t *set, size_t n)
{
    int	i;
    for (i = 0; i < n; i++)
	set[i]	= 0;
}
 
/******************************************************************/
/*
**	copy the set
*/
void
CopySet (size_t *toSet, size_t *fromSet, size_t n)
{
    int	i;
    for (i = 0; i < n; i++)
	toSet[i]	= fromSet[i];
}

/******************************************************************/
/*
**	test zone
*/
Boolean
ZoneIsOk (int zone)
{
    if (IsSetNull ((size_t *)zoneSet, SET_SIZE))
	return (TRUE);
    else
	return (InSet ((size_t)zone, (size_t *)zoneSet));
}

/******************************************************************/
/*
**	reset the List
*/
void
ResetNodeList (int n)
{
    if (nodeVarList != NULL)
	free (nodeVarList);
 
    nodeVarList	= (NodeVarType *) malloc (n * sizeof (NodeVarType));
    if (nodeVarList == NULL )
	Error ("Cannot allocate temporary memory for node var values");
    lastNodeInList	= 0;
}

/******************************************************************/
/*
**	add element to list
*/
void
AddToNodeList (int ind, double value)
{
    int	p;
    int v = ROUND2 (value);
 
    lastNodeInList++; 
    p	= lastNodeInList;
    while (p > 1 && v < nodeVarList[p-1].val) {
	nodeVarList[p]	= nodeVarList[p-1];
	p--;
    }
    if (p == 1 || v > nodeVarList[p-1].val) {
	nodeVarList[p].ii	= ind;
	nodeVarList[p].val	= v;
    }
    else
	Error ("Node values repeated");
}

/******************************************************************/
/*
**	find element in list
*/ 
int
FindInNodeList (double value)
{
    int	hi = lastNodeInList;
    int	lo = 1;
    int	mid;
    int	val;
    int v = ROUND2 (value);
 
    while (hi >= lo) {
	mid = (hi + lo) / 2;
	val =  nodeVarList[mid].val;
	if (v < val)
	    hi = mid - 1;
	else if (v > val)
	    lo = mid + 1;
	else
	    return (nodeVarList[mid].ii);
    }
    return 0;
}

/******************************************************************/
/*
**	get block-data
**	(varNum -1 for geometries)
*/
void
GetBlock (FILE *fptr, Boolean WriteData, int iStart, int iEnd, int iSkip, int iMax,
	  int jStart, int jEnd, int jSkip, int jMax, int kStart, int kEnd,
	  int kSkip, int kMax, int varNum, FieldDataType type, double fac, double *tmpVar)
{
    int i, j, k;
    int n = 0;
    int	ind;

    if (ECHO) printf ("*** in GetBlock\n");

    if (ok) {
	if (iStart == 1 && iEnd == iMax && iSkip == 1 &&
	    jStart == 1 && jEnd == jMax && jSkip == 1 &&
	    kStart == 1 && kEnd == kMax && kSkip == 1) {
	    ind	= iMax * jMax * kMax;
	    if (ECHO) printf ("ind=%d (=prod von Max-Werte %d %d %d)\n", ind,
			      iMax, jMax, kMax);
	    for (i = 0; i < ind && ok; i++) {
		if (curToken != ValueToken) {
		    if (curToken == EndofFileToken)
			Error ("Unexpected End of file. Need more values IJK");
		    else
			Error ("Bad Value (1)");
                }
		if (ECHO)
		    printf("(%d) %G\n", i, curTValue);
		/* set value */
		tmpVar[i]	= curTValue;
		n++;
		if (varNum >=0 && varNum == nodeVar)
		    AddToNodeList (i+1, curTValue);
		GetToken (FALSE, TRUE, fptr);
            }
        }
	else {
	    for (k = 1; k <= kMax && ok; k++)
		for (j = 1; j <= jMax && ok; j++)
		    for (i = 1; i <= iMax && ok; i++) {
			if (curToken != ValueToken) {
			    if (curToken == EndofFileToken)
				Error ("Unexpected End of file. Need more values IJK");
			    else
				Error ("Bad Value (2)");
			}
			if (i >= iStart && i <= iEnd &&
			    j >= jStart && j <= jEnd &&
			    k >= kStart && k <= kEnd &&
			    (i == iEnd || (((i-iStart) % iSkip) == 0)) &&
			    (j == jEnd || (((j-jStart) % jSkip) == 0)) &&
			    (k == kEnd || (((k-kStart) % kSkip) == 0))) {
                  
			    if (ECHO)
				printf("(%d,%d,%d) %G, n=%d\n", i, j, k, curTValue, n);
			    tmpVar[n]	= curTValue;
			    n++;
			    if (varNum >= 0 && varNum == nodeVar)
				AddToNodeList (i, curTValue);
			}
			GetToken (FALSE, TRUE, fptr);
		    }
        }
    }
}

/******************************************************************/
/*
**	zone-header:
**
**	ZONE T="zonetitle", I=imax, J=jmax, K=kmax, C=color,
**		F=orderedformat, D=(duplist)
**
**	ZONE T="zonetitle", N=numnodes, E=numelements, ET=elementtype,
**		F=feformat, D=(feduplist)
**	
*/
void
RecogZoneHeader (FILE *fptr)
{
    int	i;
    Boolean feFlagsUsed	= FALSE;
    Boolean iSet       	= FALSE;
    Boolean jSet       	= FALSE;
    Boolean kSet       	= FALSE;
    Boolean nvSet      	= FALSE;
    int	zoneColor	= -1;

    numZone++;
    if (numZone > MAX_NUM_ZONE)
	Error ("Maximum number of zones exceeded");

    if (ECHO) printf ("*** in RecogZoneHeader\nZONE: %d\n", numZone);

    /* init */
    /* default name; example zoneName[0]: 'ZONE 001' */
    StuffString (&zoneName[numZone],"ZONE xxx", NUM_CHARS_ZONE_NAME);
    zoneName[numZone][5] = '0' + numZone / 100;
    zoneName[numZone][6] = '0' + (numZone % 100) / 10;
    zoneName[numZone][7] = '0' + numZone % 10;
    oldElement	= element;
    oldIMax	= iMax;
    oldJMax	= jMax;
    oldKMax	= kMax;
    kMax	= 1;
    nodeVar	= -1;
    oldFormat	= format;
    format	= IJKPOINT;
    element	= QUADRILATERAL;
    numDupVar	= 0;
    dupConnect	= FALSE;
    for (i = 0; i < MAX_NUM_VAR; i++) {
	dupVar[i]	= FALSE;
	varDataType[i]	= FieldDataType_Float;
    }

    if (curToken == ZONEToken)
	GetToken (FALSE, TRUE, fptr);

    while (ok && curToken == IdentToken) {

        /* statt CompareIdent evtl. strcmp() -Funktion nutzen ??? */
	if (CompareIdent (curData, "T")) {
	    if (RemoveEqualsSign (FALSE, fptr)) {
		if (curToken == StringToken || curToken == IdentToken)
		    StuffString (&zoneName[numZone], curData, NUM_CHARS_ZONE_NAME);
		else
		    Error ("Missing or Invalid character string");
            }
	    else
		Error ("Missing Equals sign in Zone Title Assignment");
        }
	else if (CompareIdent (curData,"D")) {
	    GetDupInfo (fptr);
	    if (ECHO) printf ("ZONE D= ... numDupVar=%d\n", numDupVar);
	}
	else if (CompareIdent (curData,"C")) {
	    zoneColor	= GetColor (fptr);
	    if (ECHO) printf ("Zone color= '%s'\n", tecplotColor[zoneColor]);
	}
	else if (CompareIdent (curData,"I")) { /* IMAX */
	    iMax	= GetInteger (fptr);
	    iSet	= TRUE;
	    if (ECHO) printf ("iMax= %d\n", iMax);
        }
	else if (CompareIdent (curData, "N")) {
	    iMax	= GetInteger(fptr);
	    feFlagsUsed	= TRUE;
	    iSet	= TRUE;
	    if (ECHO) printf ("iMax= %d\n", iMax);
        }
	else if (CompareIdent (curData, "J")) { /* JMAX */
	    jMax	= GetInteger (fptr);
	    jSet	= TRUE;
	    if (ECHO) printf ("jMax= %d\n", jMax);
        }
	else if (CompareIdent (curData, "E")) {
	    jMax	= GetInteger (fptr);
	    feFlagsUsed	= TRUE;
	    jSet	= TRUE;
	    if (ECHO) printf ("jMax= %d\n", jMax);
        }
	else if (CompareIdent (curData, "ET")) {
	    GetElementType (fptr);
	    feFlagsUsed	= TRUE;
	    if (ECHO) printf ("ET=...\n");
        }
	else if (CompareIdent (curData,"NV")) { /* ab Tecplot7 ??? */
	    nodeVar	= GetInteger (fptr);
	    nodeVar--;
	    nvSet	= TRUE;
	    feFlagsUsed	= TRUE;
	    if (ECHO) printf ("ET=...\n");
        }
	else if (CompareIdent (curData, "K")) { /* KMAX */
	    kMax	= GetInteger (fptr);
	    kSet	= TRUE;
	    if (ECHO) printf ("kMax= %d\n", kMax);
        }
	else if (CompareIdent (curData, "F")) {
	    GetFormat (fptr);
	    if (ECHO) printf ("F=...\n");
	}
	else if (CompareIdent (curData,	"DT")) { /* ab Tecplot7 ??? */
	    GetDataType (varDataType, TRUE, fptr);
	    if (ECHO) printf ("DT=...\n");
	}
	else
	    Error ("Bad Identifier in zone header");
	if (ok)
	    GetToken (FALSE, TRUE, fptr);
    }

    /* set iMax, jMax, kMax */
    if (format == IJKPOINT || format == FEPOINT) {
      for (i = 0; i < MAX_NUM_VAR; i++)
          if (varDataType[i] == FieldDataType_Bit)
	      Error ("Not allowed to use BIT data type when data format is POINT or FEPOINT");
    }
    if ((format == IJKBLOCK || format == IJKPOINT) && !jSet && numDupVar > 0) {
	jMax	= oldJMax;
	jSet	= TRUE;
    }
    if ((format == FEPOINT || format == FEBLOCK) && dupConnect) {
	if (!jSet) {
	    jSet	= TRUE;
	    jMax	= oldJMax;
        }
	if (!iSet) {
	    iSet	= TRUE;
	    iMax	= oldIMax;
        }
    }
    if (!jSet) {
	if (format == FEPOINT || format == FEBLOCK)
	    jMax	= LARGESTINDEX;
	else
	    jMax	= 1;
    }
    if ((format == FEBLOCK || format == FEPOINT) && nvSet && !iSet)
	Error ("Cannot specify NV without also specifying N");
    if ((format == IJKBLOCK || format == IJKPOINT) && !iSet && numDupVar > 0) {
	iMax	= oldIMax;
	iSet	= TRUE;
	Error ("Variables are being duplicated and I is not set");
    }
    else if (!iSet)
	iMax	= LARGESTINDEX;
 
    if (numDupVar > 0) {

	int oldNumPts;
	int numPts;

	if (numZone == 1)
	    Error ("Not allowed to dup variables or connect list for first zone");
	if (oldFormat == FEPOINT || oldFormat == FEBLOCK)
	    oldNumPts	= oldIMax;
	else
	    oldNumPts	= oldIMax * oldJMax * oldKMax;
	if (format == FEPOINT || format == FEBLOCK)
	    numPts	= iMax;
	else
	    numPts	= iMax * jMax * kMax;
	if (oldNumPts != numPts)
	    Error ("Cannot dup variables or if current zone and previous zone have different number of points");
    }
 
    if (feFlagsUsed && !(format == FEPOINT || format == FEBLOCK))
	Error ("N, E, NV, and ET flag used for non-FE type data");

    if (dupConnect && (element != oldElement))
	Error ("Previous zones must have same element type when using FECONNECT");
 
#ifdef DEBUG
	printf ("\n========================================\n");
	printf("\nZONE HEADER: T= %10s\n", zoneName[numZone]);
	printf("\tI= %d\n", iMax);
	printf("\tJ= %d\n", jMax);
	printf("\tK= %d\n", kMax);
	printf("\tF= ");
	if (format == IJKBLOCK)
	    printf("BLOCK\n");
	else if (format == IJKPOINT)
	    printf("POINT\n");
	else if (format == FEPOINT)
	    printf("FEPOINT\n");
	else
	    printf("FEBLOCK\n");
	if (format == FEPOINT || format == FEBLOCK) {
	    iStart[numZone]	= 1;
	    iEnd[numZone]	= iMax;
	    iSkip[numZone]	= 1;
	    jStart[numZone]	= 1;
	    jEnd[numZone]	= 1;
	    jSkip[numZone]	= 1;
        }
	printf ("\n========================================\n");
#endif

    if (!iSet && (format == IJKBLOCK))
	Error ("Zone must at least contain I identifier for BLOCK format");
    if (!iSet && (format == FEPOINT || format == FEBLOCK))
	Error ("Must Specify N (greater than 0)) for FE formats");
    if (nvSet && (nodeVar < 0))
	Error ("NV must be greater than 0 if specified");
    /* Allow iMax = jMax = kMax = 0 (Zombie zone) */
    if (iMax != 0 || jMax != 0 || kMax != 0) {
	if (iMax <= 0)
	    Error ("I must be greater than 0 if specified");
	if (jMax <= 0)
	    Error ("J must be greater than 0 if specified");
	if (kMax <= 0)
	    Error ("K must be greater than 0 if specified");
    }
    if (iSet && (iMax == 1) && ((jMax > 1) || (kMax > 1)))
	Error ("Cannot set iMax to 1");
    if (jSet && (jMax == 1) && kSet && (kMax > 1))
	Error ("Cannot set jMax to 1 when kMax > 1");
}

/******************************************************************/
/*
**	read (parse) ascii-datafile
*/
int
ReadTecplotAscii (FILE *fptr)
{
    Boolean	endOfHeader		= FALSE;
    Boolean	variablesIdentified	= FALSE;
    Boolean	zoneHeaderParsed	= FALSE;
    Boolean	indexOk;
    Boolean	done;
    int		i, j, k, n, l;/* ii; */
    int		ind, lastInd;
    int		jjMax;

    /*
    **	parse the Tecplot-Header-Lines
    */
    GetNextLine (fptr);
    linePos	= 0;
    GetToken (FALSE, TRUE, fptr);

    if (ok) {
	do {

	    switch (curToken) {
	    case TITLEToken		:
		if (ECHOX) printf ("TITLEToken\n");
		
		if (RemoveEqualsSign (FALSE, fptr)) {
		    if ((curToken == StringToken) || (curToken == IdentToken)) {
			CopyCurData (headerString, 80);
			GetToken (FALSE, TRUE, fptr);
		    }
		    else
			Error ("Expecting a string");
		}
		else
		    Error ("Missing Equals Sign after 'TITLE' in header");
		break;

	    case VARIABLESToken	:
		if (ECHOX) printf ("VARIABLESToken\n");
		
		RecogVarNames (fptr);
		variablesIdentified	= TRUE;
		break;

	    case TEXTToken		:
	    case GEOMETRYToken	:
	    case CUSTOMLABELToken	:
	    case ZONEToken		:
	    case USERRECToken	:
	    case ValueToken		:
		if (ECHOX) printf ("\tTEXTToken or GEOMETRYToken or CUSTOMLABELToken or ZONEToken or USERRECToken or ValueToken\n");

		endOfHeader	= TRUE;
		break;
		
	    case EndofFileToken	:
		if (ECHOX) printf ("EndofFileToken\n");
		
		Error ("Unexpected End of File in Datafile header");
		break;

	    default			:
		if (ECHO) printf ("default\n");

		Error ("Expecting Record Marker or Data. Skipping Current token");
		GetToken (FALSE, TRUE, fptr);
		break;
	    }

	    if (ECHO) printf ("\n#################### next do ##################\n\n");

	} while (!endOfHeader && ok);
    } 

    /*
    **	parse the next Tecplot-File-Blocks: ZONE, TEXT, GEOMETRY, CUSTOMLABELS
    */
	
    /*	create variable-names */
    if (!variablesIdentified) {
/*	char	tvar[8];*/
	
	if (curToken == ZONEToken) {
	    RecogZoneHeader (fptr);
	    if (format != IJKPOINT && format != FEPOINT)
		Error ("VARIABLES= statement must be used if first\nzone format is not POINT");
	    zoneHeaderParsed = TRUE;
	}
	if (curToken == ValueToken) {
	    numVar	= 0;
	    while (curToken == ValueToken) {
		GetToken (FALSE, FALSE, fptr);
		numVar++;
		if (numVar >= MAX_NUM_VAR)
		    Error ("Maximum number of variables exceeded");
	    }
	    if (curToken != EndofVarLineToken)
		Error ("Bad value (3)");
	    linePos = 0;
	    GetToken (FALSE, TRUE, fptr);
	}
	else
	    numVar = 2;
	for (i = 0; i < numVar; i++)
	    StuffGenericVarName (&vName[i], i);
    }

    if (nodeVar >= numVar)
	Error ("NV is set greater than the number of variables");
    
#ifdef DEBUG    
	printf ("\n========================================\n");
	printf("\nFILE HEADER:\n");
	printf ("\tTITLE= '%s'\n", headerString);
	printf ("\tVARIABLES (%d)= ", numVar);
	for (i = 0; i < numVar; i++)
	    printf (" '%s'", vName[i]);
	printf ("\n========================================\n\n");
#endif

    /* set tecplot-multiblock-Data */
    tecMb.title	= (char*) malloc (sizeof (char) * (strlen (headerString) + 1));
    strcpy (tecMb.title, headerString);
    tecMb.nVar	= numVar;
    tecMb.varNames	= (char**) malloc (numVar * sizeof (char*));
    for (i = 0; i < numVar; i++) {
	tecMb.varNames[i]	= (char*) malloc ((strlen (vName[i]) + 1)
						  * sizeof (char));
	strcpy (tecMb.varNames[i], vName[i]);
    }

    /*
    **	while not EndofFileToken
    */
    /* init numZone */
    numZone	= 0;
    while (ok && curToken != EndofFileToken) {

	Boolean	zoneOk = ZoneIsOk (numZone);
	
	/*
	**	ZONEToken
	*/
	if (curToken == ZONEToken || (numZone == 0 && curToken == ValueToken)) {

	    double	**tmpVar;
	    int		num;

#ifdef DEBUG
		printf ("curToken==ZONEToken || (numZone==0 && curToken==ValueToken)\n");
#endif
	    if (zoneHeaderParsed)
		numZone++;
	    else
		RecogZoneHeader (fptr);
	    zoneHeaderParsed = FALSE;
 
	    if (format == FEBLOCK || format == FEPOINT)
		jjMax	= 1;
	    else
		jjMax	= jMax;

	    if (nodeVar >= 0)
		if (!dupVar[nodeVar])
		    ResetNodeList (iMax+1);
 
	    if (ok && (iMax != 0 || jMax != 0 || kMax != 0)) {
		if (iEnd[numZone] == 0)
		    iEnd[numZone]	= iMax;
		if (jEnd[numZone] == 0)
		    jEnd[numZone]	= jMax;
		if (kEnd[numZone] == 0)
		    kEnd[numZone]	= kMax;

		if (ECHO) {
		    printf ("IStart=%5d, IEnd=%5d, ISkip=%5d, IMax=%d\n",	
			   iStart[numZone], iEnd[numZone], iSkip[numZone], iMax);
		    printf ("JStart=%5d, JEnd=%5d, JSkip=%5d, JMax=%d\n",	
			   jStart[numZone], jEnd[numZone], jSkip[numZone], jjMax);
		    printf ("KStart=%5d, KEnd=%5d, KSkip=%5d, KMax=%d\n",	
			   kStart[numZone], kEnd[numZone], kSkip[numZone], kMax);
		}

		tmpVar	= (double**) malloc (numVar * sizeof (double));
		for (i = 0; i < numVar; i++) {
		    size_t	size;
		    size	= iEnd[numZone] * jEnd[numZone] * kEnd[numZone] *
			          sizeof (double);
		    tmpVar[i]	= (double*) malloc (size);
		}

		if (format == IJKBLOCK || format == FEBLOCK) {

		    for (i = 0; (i < numVar) && ok; i++) {
			if (!dupVar[i]) {
			    GetBlock (fptr, zoneOk, iStart[numZone], iEnd[numZone],
				      iSkip[numZone], iMax, jStart[numZone],
				      jEnd[numZone], jSkip[numZone], jjMax,
				      kStart[numZone], kEnd[numZone], kSkip[numZone],
				      kMax, i, varDataType[i], 1.0, tmpVar[i]);
			}
		    }
		}
		else if (iEnd[numZone] != 0 && jEnd[numZone] != 0 &&
			 kEnd[numZone] != 0) {
		    ind	= 0;
		    done	= FALSE;
		    /* hier besser for-Schleifen !! */
		    if (ECHO) printf ("3-do-loops\n");
		    num	= 0;
		    k	= 0;
		    do {
			k++;
			j	= 0;
			do {
			    j++;
			    i	= 0;
			    do
			    {
				i++;
				indexOk = (int)(i >= iStart[numZone] && i <= iEnd[numZone] &&
					   j >= jStart[numZone] && j <= jEnd[numZone] &&
					   k >= kStart[numZone] && k <= kEnd[numZone] &&
					   (i == iEnd[numZone] || (((i-iStart[numZone]) %
								    iSkip[numZone]) == 0)) &&
					   (j == jEnd[numZone] || (((j-jStart[numZone]) %
								    jSkip[numZone]) == 0)) &&
					   (k == kEnd[numZone] || (((k-kStart[numZone]) %
								    kSkip[numZone]) == 0)));
				if (indexOk)
				    lastInd	= i;
				for (n = 0; (n < numVar) && ok && !done; n++) {
				    if (!dupVar[n]) {
					ok	= (curToken == ValueToken);
					if (indexOk) {
					    if (ECHOX)
						printf("(%d,%d,%d)n=%d : %G ,num=%d\n",
						       i, j, k, n, curTValue, num);
				            /* save value */
					    tmpVar[n][num] = (double) curTValue;
					    if (n == nodeVar)
						AddToNodeList (i, curTValue);		
					}
					GetToken(FALSE, TRUE, fptr);
					if (format == IJKPOINT)
					    done	= (curToken != ValueToken);
				    }
				}
				num++;
				if (ECHOX)
				    printf ("\n");
				if (format == FEPOINT)
				    done	= (i == iMax);
				if (zoneOk && ((++ind % 250) == 0)) {
					if (ECHO){
				    	putchar('.');
				    	if ((ind % 10000) == 0)
							printf("%ld\n", i);
				    	fflush (stdout);
					}
				}
			    } while (!done && (iMax == LARGESTINDEX || i < iMax) && ok);
			} while (!done && j < jjMax && ok);
		    } while (!done && k < kMax && ok);
 
		    if (n != numVar)
			ok	= FALSE;
                              
		    if (ok) { /* Check to make sure that the correct number of values */
			      /* were read in */
			if (format == IJKPOINT) {
			    if (iMax != LARGESTINDEX)
				ok	= (i == iMax && j == jjMax && k == kMax);
			    else {
				ok	= (j == jMax && k == kMax);
				if (ok)
				    if (i < iEnd[numZone])
					iEnd[numZone]	= lastInd;
			    }	
			}	
		    }

		    if (!ok) {
			if (curToken == EndofFileToken) {
			    char str[120];
			    sprintf (str, "Unexpected end of file, IJKN = %d,%d,%d,%d",
				     i, j, k, n-1);
			    Error (str);
			}
			else
			    Error ("Expecting a value");
		    }
 
		    if (ok && iMax == 1 && (format == IJKPOINT || format == IJKBLOCK))
			Error ("Non-Standard input: IMax = 1");
		}
		if (format == FEBLOCK || format == FEPOINT) {

		    if (!dupConnect) { 
		    
			switch (element) {
			case TRIANGLE      : l = 3; break;
			case QUADRILATERAL : l = 4; break;
			case TETRAHEDRON   : l = 4; break;
			case BRICK         : l = 8; break;
			}
			j = 0;
			while (curToken == ValueToken && j < jMax) {
			    j++;
			    for (i = 0; ok && (i < l); i++) {
				char str[80];
				if (curToken != ValueToken) {
				    sprintf (str, "Err: Bad or Missing FE Cell Indicies for Cell %ld\n",j);
				    Error (str);
				}
			    
				if (ok && nodeVar >= 0) {
				    int	n;
				    n	= FindInNodeList (curTValue);
				    if (n == 0) {
					sprintf (str, "Err: FE Cell %d has index of %ld not found in Node Variable\n",j,(long)(curTValue));
					Error (str);
				    }
				}
				else if (ok) {
				    if (curTValue < 1 || curTValue > iMax) {
					sprintf (str, "FE Cell %d has bad index of %ld\n",
						 j, (long)(curTValue));
					Error (str);
				    }
				}
				GetToken (FALSE, TRUE, fptr);
			    }
			}
			if (jMax == LARGESTINDEX)
			    jMax = j;
			else if (j != jMax)
			    Error ("Not enough elements in connectivity list");	
		    }
		}
	    }
	    if (zoneOk) {
		printf("\n");
		if (iMax != 0 || jMax != 0 || kMax != 0) {
		    iMax	= 1 + (iEnd[numZone] - iStart[numZone]) / iSkip[numZone];
		    if (((iEnd[numZone] - iStart[numZone]) % iSkip[numZone]) != 0)
			iMax++;
		    if (format == FEBLOCK || format == FEPOINT) {
			kMax	= (int)element;
		    }
		    else {
			jMax	= 1 + (jEnd[numZone] - jStart[numZone]) / jSkip[numZone];
			if (((jEnd[numZone] - jStart[numZone]) % jSkip[numZone]) != 0)
			    jMax++;
			kMax	= 1 + (kEnd[numZone] - kStart[numZone]) / kSkip[numZone];
			if (((kEnd[numZone] - kStart[numZone]) % kSkip[numZone]) != 0)
			    kMax++;
		    }
		}

		if (ECHO) {
		    int	num	= iMax * jMax * kMax;
		    int	ii;
		    printf ("\n----------------------------------------------\n\n");
		    printf ("Dimensionen= %d %d %d\n\n", iMax, jMax, kMax);
		    printf ("numVar= %d\n", numVar);
		    printf ("Var-Werte:\n");
		    for (ii = 0; ii < numVar; ii++)
			    printf ("\t%s ", vName[ii]);
			printf ("\n");
		    for (i = 0; i < num; i++) {
			for (ii = 0; ii < numVar; ii++)
			    printf ("\t%G ", tmpVar[ii][i]);
			printf ("\n");
		    }
		    printf ("\n\n----------------------------------------------\n\n");
		}
	    }

	    /*
	    **	save blockinfo (zoneinfo)
	    */
	    tecMb.nBlocks++;
	    if (tecMb.blocks == NULL) {
		tecMb.blocks	= NewTecBlock ();
		lastTecBlock	= tecMb.blocks;
	    }
	    else {
		TecBlock	*block;
		int		n = 0;
		block	= tecMb.blocks;
		while (block->next != NULL) {
		    if (ECHO) printf ("block->next != NULL\n");
		    block	= block->next;
		    n++;
		}
		block->next	= NewTecBlock ();
		lastTecBlock	= block->next;
	    }
	    if (zoneName[numZone] != NULL) {
		lastTecBlock->title	= (char*) malloc ((strlen (zoneName[numZone])
							   + 1) * sizeof (char));
		strcpy (lastTecBlock->title, zoneName[numZone]);
	    }
	    lastTecBlock->dim[0]	= iMax;
	    lastTecBlock->dim[1]	= jMax;
	    lastTecBlock->dim[2]	= kMax;
	    lastTecBlock->nVar		= tecMb.nVar;
	    lastTecBlock->nVal		= iMax * jMax * kMax;
 	    lastTecBlock->val		= (double**) malloc (numVar * sizeof (double*));
	    for (n = 0; n < numVar; n++) {
		lastTecBlock->val[n]	= (double*) malloc (lastTecBlock->nVal *
							    sizeof (double));
		for (i = 0; i < lastTecBlock->nVal; i++)
		    lastTecBlock->val[n][i]	= tmpVar[n][i];
	    }

	    for (n = 0; n < numVar; n++)
		if (tmpVar[n] != NULL)
		    free (tmpVar[n]);
	    free (tmpVar);
	}
	/*
	**	GEOMETRYToken, TEXTToken, CUSTOMLABELToken, USERRECTToken unused
	*/
	else {
#ifdef DEBUG
		if (curToken == GEOMETRYToken)
		    printf ("GEOMETRYToken ...\n");
		else if (curToken == TEXTToken)
		    printf ("TEXTToken ...\n");
		else if (curToken == CUSTOMLABELToken)
		    printf ("CUSTOMLABELToken ...\n");
		else if (curToken == USERRECToken)
		    printf ("USERRECTToken ...\n");
#endif		
	    GetNextLine (fptr);
	    linePos	= 0;
	    GetToken (FALSE, TRUE, fptr);
	}
    }

    return (int)ok;
}


/******************************************************************/
/*
**	read next string (bin)
*/
void
GetNextString (FILE *fptr, int version)
{
    unsigned int	c;
    int			i;
    float		fval;

    i	= 0;
    if (version	== 6) {
	fread (&fval, sizeof (float), 1, fptr);
	c	= (unsigned int) fval;
	while (c != 0 && c != EOF && i < MAX_TEXT_STRING) {
	    /* printf ("\tc=%d, c='%c'\n", c, c); */
	    curData[i++]	= c;
	    fread (&fval, sizeof (float), 1, fptr);
	    c	= (unsigned int) fval;
	}
    }
    else {	/* version == 7 */
	fread (&c, sizeof (unsigned int), 1, fptr);
	while (c != 0 && c != EOF && i < MAX_TEXT_STRING) {
	    /* printf ("\tc=%d, c='%c'\n", c, c); */
	    curData[i++]	= c;
	    fread (&c, sizeof (unsigned int), 1, fptr);
	}
    }
    curData[i]	= '\0';
}

/******************************************************************/
/*
**	read next n chars (bin)
*/
void
GetNextNChars (FILE *fptr, int len)
{
/*    unsigned int	c;*/
    int			i;
    float		fval;

    i	= 0;
    if (len > 0) {
	if (len > MAX_TEXT_STRING)
	    len	= MAX_TEXT_STRING - 1;
	while (i < len) {
	    fread (&fval, sizeof (float), 1, fptr);
	    if ((unsigned int) fval == EOF)
		break;
	    curData[i++]	= (unsigned int) fval;
	}
    }
    curData[i]	= '\0';
}

/******************************************************************/
/*
**	read geometry (bin)
**
**	data unused
*/
int
ReadGeomBin (FILE *fptr, int version)
{
    float		fval;
/*    int			ival;
    double		dval;
*/    GeomType		geomType;
    FieldDataType	dataType;
    int			nPoly;
    int			nPoints;
    int			i;/*, j;*/
    int			size;

    if (version == 6) {

	size	= 9 * sizeof (float);
	fseek (fptr, (long) size, SEEK_CUR);
	fread (&fval, sizeof (float), 1, fptr);
	geomType	= (int) fval;
	size	= sizeof (float);
	fseek (fptr, (long) size, SEEK_CUR);
#if 0
	fread (&fval, sizeof (float), 1, fptr);		/* mode */
	fread (&fval, sizeof (float), 1, fptr);		/* group */
	for (i = 0; i < 3; i++)
	    fread (&fval, sizeof (float), 1, fptr);	/* x, y, z starting location */
	fread (&fval, sizeof (float), 1, fptr);		/* zone */
	fread (&fval, sizeof (float), 1, fptr);		/* color */
	fread (&fval, sizeof (float), 1, fptr);		/* fillColor */
	fread (&fval, sizeof (float), 1, fptr);		/* isfilled */
	fread (&fval, sizeof (float), 1, fptr);
	geomType	= (int) fval;			/* geomType */
	fread (&fval, sizeof (float), 1, fptr);		/* lineType */
#endif

#ifdef DEBUG
	    printf ("geomType=%d\n", geomType);
#endif

	switch (geomType) {

	case LINE:
	case LINE3D:		   
	    fread (&fval, sizeof (float), 1, fptr);	/* number of polyline */
	    nPoly	= (int) fval;

	    for (i = 0; i < nPoly; i++) {
		fread (&fval, sizeof (float), 1, fptr);	/* number of geometry points */
		nPoints	= (int) fval;

#ifdef DEBUG
		    printf ("nPoints= %d\n", nPoints);
#endif
	
		if (geomType == LINE3D)
		    size	= nPoints * 3 * sizeof (float);
		else
		    size	= nPoints * 2 * sizeof (float);
		fseek (fptr, (long) size, SEEK_CUR);
	    }
	    break;

	case RECTANGLE:
	    fseek (fptr, (long)(2 * sizeof (float)), SEEK_CUR);	/* x, y offset */
	    break;

	case CIRCLE:
	    fseek (fptr, (long)(sizeof (float)), SEEK_CUR);	/* radius */
	    break;

	case ELLIPSE:
	    fseek (fptr, (long)(2 * sizeof (float)), SEEK_CUR);	/* x, y radius */
	    break;

	default:
	    Error ("read geometry error");
	    return 0;
	}
    }
    else {

	size	= (6 * sizeof (int)) + (3 * sizeof (double));
	fseek (fptr, (long) size, SEEK_CUR);
	fread (&geomType, sizeof (int), 1, fptr);	/* geomType */
	size	= (4 * sizeof (int)) + (4 * sizeof (double));
	fseek (fptr, (long) size, SEEK_CUR);
	fread (&dataType, sizeof (int), 1, fptr);	/* polyline field data Type */

#ifdef DEBUG
	    printf ("geomType=%d, dataType=%d\n", geomType, dataType);
#endif

	switch (geomType) {

	case LINE:
	case LINE3D:		   
	    fread (&nPoly, sizeof (int), 1, fptr);	/* number of polyline */

	    if (dataType == FieldDataType_Float) {

		for (i = 0; i < nPoly; i++) {
		    fread (&nPoints, sizeof (int), 1, fptr);	/* number of geometry points */

#ifdef DEBUG
			printf ("nPoints= %d\n", nPoints);
#endif
		    if (geomType == LINE3D)
			size	= nPoints * 3 * sizeof (float);
		    else
			size	= nPoints * 2 * sizeof (float);
		    fseek (fptr, (long) size, SEEK_CUR);
		}
	    }
	    else if (dataType == FieldDataType_Double) {

		for (i = 0; i < nPoly; i++) {
		    fread (&nPoints, sizeof (int), 1, fptr);	/* number of geometry points */
		    
#ifdef DEBUG
			printf ("nPoints= %d\n", nPoints);
#endif
		    if (geomType == LINE3D)
			size	= nPoints * 3 * sizeof (double);
		    else
			size	= nPoints * 2 * sizeof (double);
		    fseek (fptr, (long) size, SEEK_CUR);
		}
	    }
	    break;

	case RECTANGLE:
	    fseek (fptr, (long)(2 * sizeof (double)), SEEK_CUR);	/* x, y offset */
	    break;

	case CIRCLE:
	    fseek (fptr, (long)(sizeof (double)), SEEK_CUR);		/* radius */
	    break;

	case ELLIPSE:
	    fseek (fptr, (long)(2 * sizeof (double)), SEEK_CUR);	/* x, y radius */
	    break;

	default:
	    Error ("read geometry error");
	    return 0;
	}
    }
    return 1;
}

/******************************************************************/
/*
**	read text (bin)
*/
int
ReadTextBin (FILE *fptr, int version)
{
    float	fval;
    double	dval;
    int		ival;
/*    int		i;*/
    int		size;

    if (version == 6) {
	size	= 13 * sizeof (float);
	fseek (fptr, (long) size, SEEK_CUR);
	fread (&fval, sizeof (float), 1, fptr);
	printf ("len=%d\n", (int) fval);
	GetNextNChars (fptr, (int) fval);
#if 0
	fread (&fval, sizeof (float), 1, fptr);		/* mode */
	fread (&fval, sizeof (float), 1, fptr);		/* group */
	for (i = 0; i < 2; i++)
	    fread (&fval, sizeof (float), 1, fptr);	/* x, y starting location */
	for (i = 0; i < 2; i++)
	    fread (&fval, sizeof (float), 1, fptr);	/* width, height of characters */
	fread (&fval, sizeof (float), 1, fptr);		/* textBox type */
	fread (&fval, sizeof (float), 1, fptr);		/* textBox margin */
	fread (&fval, sizeof (float), 1, fptr);		/* textBox outlineColor */
	fread (&fval, sizeof (float), 1, fptr);		/* TextBox fillColor */
	fread (&fval, sizeof (float), 1, fptr);		/* angle */
	fread (&fval, sizeof (float), 1, fptr);		/* zone */
	fread (&fval, sizeof (float), 1, fptr);		/* color */
	fread (&fval, sizeof (float), 1, fptr);		/* textLength */
	GetNextNChars (fptr, (int) fval);
#endif

#ifdef DEBUG
	    printf ("text=\t'%s'\n", curData);
#endif

    }
    else {	/* version == 7 */
#if 0
	size	= (10 * sizeof (int)) + (7 * sizeof (double));
	fseek (fptr, (long) size, SEEK_CUR);
#endif
	fread (&ival, sizeof (int), 1, fptr);		/* coordsys position */
	fread (&ival, sizeof (int), 1, fptr);		/* scope */
	fread (&dval, sizeof (double), 1, fptr);	/* x starting location */
	fread (&dval, sizeof (double), 1, fptr);	/* y starting location */
	fread (&ival, sizeof (int), 1, fptr);		/* fontType */
	fread (&ival, sizeof (int), 1, fptr);		/* character height units */
	fread (&dval, sizeof (double), 1, fptr);	/* character height */
	fread (&ival, sizeof (int), 1, fptr);		/* textBox type */
	fread (&dval, sizeof (double), 1, fptr);	/* textBox margin */
	fread (&dval, sizeof (double), 1, fptr);	/* textBox margin linewidth */
	fread (&ival, sizeof (int), 1, fptr);		/* textBox outlineColor */
	fread (&ival, sizeof (int), 1, fptr);		/* textBox fillColor */
	fread (&dval, sizeof (double), 1, fptr);	/* angle */
	fread (&dval, sizeof (double), 1, fptr);	/* line spacing */
	fread (&ival, sizeof (int), 1, fptr);		/* text anchor */
	fread (&ival, sizeof (int), 1, fptr);		/* zone */
	fread (&ival, sizeof (int), 1, fptr);		/* color */
	GetNextString (fptr, version);

#ifdef DEBUG
	    printf ("text=\t'%s'\n", curData);
#endif	

	/** spaeter mit fseek**/

    }

    return 1;
}

/******************************************************************/
/*
**	read customLabel (bin)
**
**	data unused
*/
int
ReadCustomLabelBin (FILE *fptr, int version)
{
    float	fval;
    int		i, n;

    if (version == 6) {
	fread (&fval, sizeof (float), 1, fptr);		/* number of labels */
	n	= (int) fval;
    }
    else {	/* version == 7 */
	fread (&n, sizeof (int), 1, fptr);		/* number of labels */
    }
    for (i = 0; i < n; i++) {
	GetNextString (fptr, version);
#ifdef DEBUG
	    printf ("label%d=\t'%s'\n", i, curData);
#endif
    }

    return 1;
}

/******************************************************************/
/*
**	read Tecplot data (bin)
*/
int
ReadTecplotBin (FILE *fptr, int version)
{
    float	fval;
    int		ival;
    double	dval;
/*    char	ch;*/
    int		i, j, k, n;
    int		ind;
/*    int		ichar;*/
/*    int      	version;	*//* Tecplot6, Tecplot7 */
    int		nZone;
    TecBlock	*block;

    /*FormatType	*zoneFormat; */
    FormatType zoneFormat[MAX_NUM_ZONE];
    
#ifdef DEBUG
	printf ("*** in ReadTecplotBin\n");
#endif

    /*
    **	read Tecplot-headerlines
    */
/* zum Test in Kommentar setzen ?! */
#if 0
    curData[0]	='\0';
    fread (curData, sizeof (char), 8, fptr);
#ifdef DEBUG
	printf ("curData='%s'\n", curData);
#endif
    if (!strncmp (curData, "#!TDV71", 6)) {
	version	= 7;
	fread (&ival, sizeof (int), 1, fptr);
    }
    else {
	rewind (fptr);
	fread (&fval, sizeof (float), 1, fptr);	/* read version */
	printf ("fval = version = %G\n", fval);
	version	= (int) fval;
	printf ("version = %d\n", version);
	if (version != 6) {
	    /* ascii-Variante !? */
	    Error ("unknown Tecplot-Version - check Ascii");
	    return 0;
	}
    }
#endif

#ifdef DEBUG
	printf ("read Tecplot-Version %d\n", version);
#endif
    
    /*	title	*/
    GetNextString (fptr, version);
    tecMb.title	= (char*) malloc (sizeof (char) * (strlen (curData) + 1));
    strcpy (tecMb.title, curData);

    if (version == 6) {
	fread (&fval, sizeof (float), 1, fptr);
	tecMb.nVar	= (int) fval;
    }
    else {
	fread (&ival, sizeof (int), 1, fptr);
	tecMb.nVar	= ival;
    }

    if (tecMb.nVar >= MAX_NUM_VAR) {
	Error ("out of range");
	return 0;
    }

    tecMb.varNames	= (char**) malloc (tecMb.nVar * sizeof (char*));
    for (i=0; i < tecMb.nVar; i++) {
	GetNextString (fptr, version);
	tecMb.varNames[i]	= (char*) malloc ((strlen (curData) + 1)
						  * sizeof (char));
	strcpy (tecMb.varNames[i], curData);
    }

#ifdef DEBUG
	printf ("\n========================================\n");
	printf("\nFILE HEADER:\n");
	printf ("\tTITLE= '%s'\n", tecMb.title);
	printf ("\tVARIABLES (%d)= ", tecMb.nVar);
	for (i = 0; i < tecMb.nVar; i++)
	    printf (" '%s'", tecMb.varNames[i]);
	printf ("\n========================================\n\n");
#endif


    /*
    **	check marker 
    **
    **		299.0	->	zones
    **		399.0	->	geometry
    **		499.0	->	text
    **		599.0	->	customLabel
    */
    while (fread (&fval, sizeof (float), 1, fptr) && fval != 357) {
	
#ifdef DEBUG
	    printf ("\n-->marker=\t%G\n", fval);
#endif

	if (fval == 299) {		/* read zone header */

#ifdef DEBUG
		printf ("read zone-header\n");
#endif
	    /* init new block */
	    tecMb.nBlocks++;
	    if (tecMb.blocks == NULL) {
		tecMb.blocks	= NewTecBlock ();
		lastTecBlock	= tecMb.blocks;
	    }
	    else {
		n = 0;
		block	= tecMb.blocks;
		while (block->next != NULL) {
		    block	= block->next;
		    n++;
		}
		block->next	= NewTecBlock ();
		lastTecBlock	= block->next;
	    }
	    lastTecBlock->nVar	= tecMb.nVar;

	    /* title */
	    GetNextString (fptr, version);
#ifdef DEBUG
		printf ("zoneName=\t'%s'\n", curData);
#endif
	    if (curData != NULL) {
		lastTecBlock->title	= (char*) malloc ((strlen (curData) + 1)
							  * sizeof (char));
		strcpy (lastTecBlock->title, curData);
	    }
	    
	    if (version == 6) {
		fread (&fval, sizeof (float), 1, fptr);
		zoneFormat[tecMb.nBlocks-1]	= (int) fval;
		fread (&fval, sizeof (float), 1, fptr);	/* color, unused */
		for (i = 0; i < 3; i++) {
		    fread (&fval, sizeof (float), 1, fptr);
		    lastTecBlock->dim[i]	= (int) fval;
		}
	    }
	    else {
		fread (&ival, sizeof (int), 1, fptr);
		zoneFormat[tecMb.nBlocks-1]	= ival;
		fread (&ival, sizeof (int), 1, fptr);	/* color, unused */
		for (i = 0; i < 3; i++) {
		    fread (&ival, sizeof (int), 1, fptr);
		    lastTecBlock->dim[i]	= ival;
		}
	    }
#ifdef DEBUG
		printf ("format=\tfval=\t%d\n", zoneFormat[tecMb.nBlocks-1]);
		printf ("dims=\t%d\t%d\t%d\n", lastTecBlock->dim[0],
			lastTecBlock->dim[1], lastTecBlock->dim[2]);
#endif

	    lastTecBlock->nVar		= tecMb.nVar;
	    lastTecBlock->nVal		= lastTecBlock->dim[0] * lastTecBlock->dim[1] *
		                          lastTecBlock->dim[2];
	    lastTecBlock->val		= (double**) malloc (lastTecBlock->nVar *
							     sizeof (double));
	    for (i = 0; i < lastTecBlock->nVar; i++)
		lastTecBlock->val[i]	= (double*) malloc (lastTecBlock->nVal *
							    sizeof (double));

	}
	else if (fval == 399) {	/* read geometry, unused*/

#ifdef DEBUG
	    printf ("read geometry\n");
#endif
	    printf ("return readGeomBin=%d\n", ReadGeomBin (fptr, version));

	}
	else if (fval == 499) {	/* read text */

#ifdef DEBUG
	    printf ("read text\n");
#endif
	    printf ("return-ReadTextBin= %d\n", ReadTextBin (fptr, version));

	}
	else if (fval == 599) {	/* read customLabel */
	    
#ifdef DEBUG
		printf ("-> read customLabel\n");
#endif

	    printf ("return-ReadCustomLabelBin= %d\n", ReadCustomLabelBin (fptr, version));

	}
	else {
	    /* bei Version 7 gibt es noch UserRec ??? */
	    Error ("read error");
	    return 0;
	}
    }
#ifdef DEBUG
	printf ("\nend read header\n");
#endif

    /*
    **	read data
    */
    nZone	= -1;
    block	= NULL;
    while (fread (&fval, sizeof (float), 1, fptr)) {
	
#ifdef DEBUG
	    printf ("\n-->marker=\t%G (read data)\n", fval);
#endif

	if (fval == 299) {		/* read zone */

	    FieldDataType	*type;

	    nZone++;
#ifdef DEBUG
		printf ("nZone= %d\n", nZone);
#endif

	    if (nZone > tecMb.nBlocks) {
		Error ("read error");
		return 0;
	    }
	    type	= (FieldDataType*) malloc (tecMb.nVar * sizeof (FieldDataType));
	    if (block == NULL)
		block	= tecMb.blocks;
	    else
		block	= block->next;
		
	    block->nVal	= block->dim[0] * block->dim[1] * block->dim[2];
	    block->val	= (double**) malloc (block->nVar * sizeof (double*));
	    for (i = 0; i < block->nVar; i++)
		block->val[i]	= (double*) malloc (block->nVal * sizeof (double));

	    if (version == 6) {

		int	rep;

		fread (&fval, sizeof (float), 1, fptr);
#ifdef DEBUG
		    printf ("nVar=\t%G\n", fval);
#endif
		rep	= (int) fval;
		for (i = 0; i < rep; i++) {
		    fread (&fval, sizeof (float), 1, fptr);
#ifdef DEBUG
			printf ("v%d::variable-numbers-of-repeat=\tival=\t%G\n", i, fval);
#endif
		}
		for (i = 0; i < tecMb.nVar; i++) {
		    type[i]	= FieldDataType_Float;
		}
	    }
	    else {

		int	rep;

		fread (&ival, sizeof (int), 1, fptr);
#ifdef DEBUG
		    printf ("repeat-vars=\t\tival=\t%d\n", ival);
#endif
		rep	= ival;
		for (i = 0; i < rep; i++) {
		    fread (&ival, sizeof (int), 1, fptr);
#ifdef DEBUG
			printf ("v%d::variable-numbers-of-repeat=\tival=\t%d\n", i, ival);
#endif
		}
		for (i = 0; i < tecMb.nVar; i++) {
		    fread (&ival, sizeof (int), 1, fptr);
#ifdef DEBUG
			printf ("var-data-format=\t%d\n", ival);
#endif
		    type[i]	= ival;
		}
	    }

	    if (zoneFormat[nZone] == IJKBLOCK || zoneFormat[nZone] == FEBLOCK) {

#ifdef DEBUG
		    printf ("format: IJKBLOCK or FEBLOCK\n");
#endif

		for (n = 0; n < block->nVar; n++) {
		    ind	= 0;
		    if (type[n] == FieldDataType_Float) {
			float	*f; 
			f	= (float*) malloc (block->nVal * sizeof (float));
			fread (f, sizeof (float), block->nVal, fptr);
			for (i = 0; i < block->nVal; i++) {
			    block->val[n][i]	= (double) f[i];
			}
			free (f);
		    }
		    else if (type[n] == FieldDataType_Double) {
			fread (block->val[n], sizeof (double), block->nVal, fptr);
		    }
		}

	    }
	    else {	/* IJKPOINT, FEPOINT */

#ifdef DEBUG
		    printf ("format: IJKPOINT or FEPOINT\n");
#endif

		ind	= 0;
		for (i = 0; i < block->dim[0]; i++)
		    for (j = 0; j < block->dim[1]; j++)
			for (k = 0; k < block->dim[2]; k++) {
			    for (n = 0; n < block->nVar; n++) {
				if (type[n] == FieldDataType_Float) {
				    fread (&fval, sizeof (float), 1, fptr);
				    block->val[n][ind]	= (double) fval;
				}
				else if (type[n] == FieldDataType_Double) {
				    fread (&dval, sizeof (double), 1, fptr);
				    block->val[n][ind]	= dval;
				}
				else {
				    Error ("read error");
				    return 0;
				}
			    }
			    ind++;
			}
	    }
	}
	else {
	    Error ("read data error");
	    return 0;
	}
    }
    return 1;
}

/******************************************************************/
/*
**	read the Tecplot-datafile
*/
int
ReadTecplot (OMobj_id Read_Tecplot_id, char *fname)
{
    FILE	*fptr;
    int		i, j;/*, n;*/
/*    Boolean	formatted	= FALSE;*/
    char	*format	= NULL;

    int		version;
    float	fval;
    int		ival;

    /* init global variables */
    SetToNull ((size_t *)zoneSet, SET_SIZE);
    ok			= TRUE;
    numVar		= 0;
    numZone		= 0;
    element		= BADELEMENT;
    nodeVar		= -1;
    repeatLength	= 0;
    nodeVarList		= NULL;
    lastNodeInList	= 0;
    for (i = 0; i < MAX_NUM_VAR; i++)
	vName[i] = NULL;
    for (i = 0; i <= MAX_NUM_ZONE; i++) {
	zoneName[i] = NULL;
	iStart[i]	= 1;
	iEnd[i]		= 0;
	iSkip[i]       	= 1;
	jStart[i]	= 1;
	jEnd[i]		= 0;
	jSkip[i]	= 1;
	kStart[i]	= 1;
	kEnd[i]		= 0;
	kSkip[i]	= 1;
    }

/***hier schon untersuchen, welches Fileformat vorliegt****/
#if 0
    curData[0]	='\0';
    fread (curData, sizeof (char), 8, fptr);
#ifdef DEBUG
	printf ("curData='%s'\n", curData);
#endif
    if (!strncmp (curData, "#!TDV71", 6)) {
	version	= 7;
	fread (&ival, sizeof (int), 1, fptr);
    }
    else {
	rewind (fptr);
	fread (&fval, sizeof (float), 1, fptr);	/* read version */
	printf ("fval = version = %G\n", fval);
	version	= (int) fval;
/*
	if (version != 6) {
	    Error ("unknown Tecplot-Version -- check Ascii");
	    return 0;
	}
	*/
	if (version != 6)
	    version	= 0;	/* ASCII */
    }
#ifdef DEBUG
    printf ("read Tecplot-Version %d\n", version);
#endif
#endif
/*******/


    /* check file-format */
#if 0
    if (OMget_name_str_val (Read_Tecplot_id, OMstr_to_name ("file_format"), &format, 0) != 1)
	format = NULL;
#ifdef DEBUG
		printf ("fileformat='%s'\n", format);
#endif
    if (!strcmp (format, "ascii"))
	formatted	= TRUE;
    else	/* binaer, default */
	formatted	= FALSE;

    /* open datafile */
    if (formatted) {
	if (!(fptr = fopen (fname, "r"))) {
	    char	s[150];
	    sprintf (s, "Can't open datafile %s", fname);
	    Error (s);
	    return 0;
	}
    }
    else {
	if (!(fptr = fopen (fname, "rb"))) {
	    char	s[150];
	    sprintf (s, "Can't open datafile %s", fname);
	    Error (s);
	    return 0;
	}
    }
#endif

    /*
    **	start parsing the datafile
    */

    textString[0]	= '\0';
    headerString[0]	= '\0';

    /* init tecplot-multi-block */
    tecMb.title		= NULL;
    tecMb.nBlocks	= 0;
    tecMb.blocks	= NULL;

#if 0
    if (formatted == TRUE) {
	ReadTecplotAscii (fptr);
    }
    else {
	ReadTecplotBin (fptr, version);
    }
#endif

/******/
    /* open datafile and check format */
    if (!(fptr = fopen (fname, "rb"))) {
	    char	s[150];
	    sprintf (s, "Can't open datafile %s", fname);
	    Error (s);
	    return 0;
    }
    curData[0]	='\0';
    fread (curData, sizeof (char), 8, fptr);
#ifdef DEBUG
    printf ("curData='%s'\n", curData);
#endif
    if (!strncmp (curData, "#!TDV71", 6)) {
	version	= 7;
	fread (&ival, sizeof (int), 1, fptr);
    }
    else {
	rewind (fptr);
	fread (&fval, sizeof (float), 1, fptr);	/* read version */
	version	= (int) fval;
	if (version != 6) {
	    fclose (fptr);
	    if (!(fptr = fopen (fname, "r"))) {
		char	s[150];
		sprintf (s, "Can't open datafile %s", fname);
		Error (s);
		return 0;
	    }
	    version	= 0;	/* ASCII */
	}
    }
#ifdef DEBUG
	printf ("read Tecplot-Version %d\n", version);
#endif
    if (version == 6 || version == 7) {
	ReadTecplotBin (fptr, version);
    }
    else {
#ifdef DEBUG
	    printf ("ReadTecplotAscii\n");
#endif
	ReadTecplotAscii (fptr);
    }

/******/


    if (ok) {
#ifdef DEBUG
	    TecBlock	*block;
	    printf ("\n----------------------------------------------\n");
	    printf ("--- TECPLOT DATA\n");
	    printf ("\nnBlocks=\t%d\n", tecMb.nBlocks);
	    if (tecMb.title != NULL)
		printf ("title=\t\t'%s'\n", tecMb.title);
	    printf ("Variables= (%d)", tecMb.nVar);
	    for (i = 0; i < tecMb.nVar; i++)
		if (tecMb.varNames[i] != NULL)
		    printf ("\t'%s'", tecMb.varNames[i]);
	    printf ("\nnBlocks=\t%d\n", tecMb.nBlocks);
	    block	= tecMb.blocks;
	    j = 0;
	    while (block != NULL) {
		j++;
		printf ("\n--- block Nr.%d ---\n", j);
		if (block->title != NULL)
		    printf ("title=\t'%s'\n", block->title);
		printf ("dim=\t%d \t%d\t%d\n", block->dim[0], block->dim[1],
			block->dim[2]);
		printf ("nVar=\t%d\n", block->nVar);
		printf ("nVal=\t%d\n", block->nVal);
		/*
		for (i = 0; i < block->nVal; i++) {
		    for (n = 0; n < block->nVar; n++)
			printf ("\t%G ", block->val[n][i]);
		    printf ("\n");
		}
		*/
		block	= block->next;
	    }
#endif	
    }

    /* clean */
    for (i = 0; i < MAX_NUM_ZONE; i++)
	if (zoneName[i])
	    free (zoneName[i]);
    for (i = 0; i < MAX_NUM_VAR; i++)
	if (vName[i])
	    free (vName[i]);

    fclose (fptr);
    return (int)ok;
}

/******************************************************************/
/*
**	free TecMultiBlock-structure
*/
int
CleanTecMultiBlock (TecMultiBlock *m)
{
	int		i;
	TecBlock *tmp, *block;

#ifdef DEBUG
     printf ("*** in CleanTecMultiBlock\n");
#endif

     if (m->title != NULL) {
         free (m->title);
         m->title = NULL;
     }

     if (m->varNames != NULL) {
         for (i = 0; i < m->nVar; ++i) {
             if (m->varNames[i]) free( m->varNames[i]);
         }
         free (m->varNames);
         m->varNames = NULL;
     }
     m->nVar     = 0;

     m->nBlocks  = 0;
     if (m->blocks != NULL) {
         block = m->blocks;
         while (block != NULL) {
             tmp = block->next;
             if (block->val != NULL) {
                 for (i = 0; i < block->nVar; ++i) {
                     if (block->val[i]) free(block->val[i]);
                 }
                 free(block->val);
             }
             if (block->title) free(block->title);
             free (block);
             block = tmp;
         }

         m->blocks = NULL;
     }

     return 1;
}


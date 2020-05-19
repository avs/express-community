/*************************************************************************
*                                                                        *
* Copyright (c) 1993, Scientific Visualization Associates, Inc.,         *
*                                                                        *
* All rights reserved.                                                   *
* This is licensed software of Scientific Visualization Associates, Inc. *
* This material contains trade secret information of Scientific          *
* Visualization Associates, Inc.  Use, repreoduction, or disclosure is   *
* prohibited, except under a written license by Scientific Visualization *
* Associates, Inc.  Use by the Federal Government is governed by         *
* FAR 52.227-19(c), Commercial Computer Software, or for Department of   *
* Defense Users, by DFAR 252.227-7013.                                   *
*                                                                        *
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xp_comm_proj/rd_fidap/fid2FLD.h"


/*++**********************************************************************
*
*   Scientific Vizualization Associates   Concord MA.
*
*   Title: read_line().
*
*   Author: Ralph J. Folz
*
*   Date Created: Mon Nov 08 10:06:58 1993
*
*   Revision History:
*
*   Description: This function will read a line from a file.  The
*      new-line will be included in the return string.
*
*   Inputs:
*
*      fp	-> file pointer of file to read from.
*
*   Outputs:
*
*      status is returned.  0 = OK  -1 = error
*
*      line	-> string is returned in this location.
*
**--*********************************************************************/
#ifdef _NO_PROTO
int read_line(fp, line)
FILE	*fp;
char	*line;
#else
int read_line(FILE *fp, char *line)
#endif
{
   line = fgets(line, MAX_LINE, fp);
   if(line == NULL)
   {
      fprintf(stdout, "Error reading line from file.  Exiting...\n");
      return(-1);
   }
   else
      return(0); 
}


/*++**********************************************************************
*
*   Scientific Vizualization Associates   Concord MA.
*
*   Title: read_string().
*
*   Author: Ralph J. Folz
*
*   Date Created: Mon Nov 08 10:03:12 1993
*
*   Revision History:
*
*   Description: This function will read a string of size "length" from
*      the file passed in.  
*
*   Inputs:
*
*      fp	-> File pointer of file to read from
*      length	-> Length of string to read.
*
*   Outputs:
*
*      status is returned   0 = ok   -1 = error
* 
*      string	-> string is returned.  Note that the caller should
*                  be passing in a string which has enough space to
*                  hold length + 1 characters since a null will be added.
*
**--*********************************************************************/
#ifdef _NO_PROTO
int read_string(fp, string, length)
FILE    *fp;
char    *string;
int     length;
#else
int read_string(FILE *fp, char *string, int length)
#endif
{
   /* fgets stops when it reads n-1 characters, so pass in length + 1	*/
   /* also, I will add a null to the end of the string passed in.	*/
   string = fgets(string, length + 1, fp);
   if(string == NULL)
   {
      fprintf(stdout, "Error reading string from file.  Exiting...\n");
      return(-1);
   }
   else
   {
      return(0);
   }
}

/* Same as above except it won't print out the error.			*/
#ifdef _NO_PROTO
int read_string_no_err(fp, string, length)
FILE    *fp;
char    *string;
int     length;
#else
int read_string_no_err(FILE *fp, char *string, int length)
#endif
{
   /* fgets stops when it reads n-1 characters, so pass in length + 1   */
   /* also, I will add a null to the end of the string passed in.       */
   string = fgets(string, length + 1, fp);
   if(string == NULL)
      return(-1);
   else
      return(0);
}



/*++**********************************************************************
*
*   Scientific Vizualization Associates   Concord MA.
*
*   Title: read_past_newline().
*
*   Author: Ralph J. Folz
*
*   Date Created: Mon Nov 08 11:04:06 1993
*
*   Revision History:
*
*   Description: This function will read past the next new-line and
*      position the file pointer at the beginning of the next line.
*
*   Inputs:
*
*      fp	-> file pointer.
*
*   Outputs: none.
*
**--*********************************************************************/
#ifdef _NO_PROTO
void read_past_newline(fp)
FILE	*fp;
#else
void read_past_newline(FILE *fp)
#endif
{
   char		temp_string[MAX_LINE];

   fgets(temp_string, MAX_LINE, fp);
}


/*++**********************************************************************
*
*   Scientific Vizualization Associates   Concord MA.
*
*   Title: read_variable_float().
*
*   Author: Ralph J. Folz
*
*   Date Created: Mon Nov 08 11:06:37 1993
*
*   Revision History:
*
*   Description: This function will read the next floating point value
*      from the given file pointer and put the result in float_value.
*
*   Inputs:
*
*      fp	-> file pointer to read from.
*
*   Outputs:
*
*      float_value	-> result placed here.
*
*      status is returned   0 = OK   -1 = error.
*
**--*********************************************************************/
#ifdef _NO_PROTO
int read_variable_float(fp, float_value)
FILE	*fp;
float	*float_value;
#else
int read_variable_float(FILE *fp, float *float_value)
#endif
{
   int		status;

   status = fscanf(fp, "%f", float_value);
   if(status < 0)
   {
      fprintf(stdout, "Error reading float value from file\n");
      return(-1);
   }
   else
      return(0);
}



/*++**********************************************************************
*
*   Scientific Vizualization Associates   Concord MA.
*
*   Title: read_int().
*
*   Author: Ralph J. Folz
*
*   Date Created: Mon Nov 08 11:35:20 1993
*
*   Revision History:
*
*   Description: This function will read a "length" character integer.
*
*   Inputs:
*
*      fp	-> File to read from
*      length	-> Length of characters representing integer.
*
*   Outputs:
*
*      status is returned 0 = OK  -1 = Error.
*
*      integer read is returned in int_value.
*
**--*********************************************************************/
#ifdef _NO_PROTO
int read_int(fp, int_value, length)
FILE	*fp;
int	*int_value;
int	length;
#else
int read_int(FILE *fp, int *int_value, int length)
#endif
{
   char		string_value[128];
   int		status;

   status = read_string(fp, string_value, length);
   if(status != 0)
      return(-1);

   *int_value = atoi(string_value);
   return(0);
}



/*++**********************************************************************
*
*   Scientific Vizualization Associates   Concord MA.
*
*   Title: read_float().
*
*   Author: Ralph J. Folz
*
*   Date Created: Mon Nov 08 13:17:31 1993
*
*   Revision History:
*
*   Description: This function will read a floating pointer number
*      which is represented by "length" number of characters.
*
*   Inputs:
*
*      fp	-> File Pointer.
*      length	-> Length to read.
*
*   Outputs:
*
*      float_value	-> Floating point number returned.
*
*      status returned 0 = OK  -1 = Error.
*
**--*********************************************************************/
#ifdef _NO_PROTO
int read_float(fp, float_value, length)
FILE    *fp;
float  	*float_value;
int     length;
#else
int read_float(FILE *fp, float *float_value, int length)
#endif
{
   char         string_value[128];
   int          status;

   status = read_string(fp, string_value, length);
   if(status != 0)
      return(-1);

   *float_value = atof(string_value);
   return(0);
}


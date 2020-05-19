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
#include <string.h>
#include "xp_comm_proj/rd_fidap/fid2FLD.h"

/*++**********************************************************************
*
*   Scientific Vizualization Associates   Concord MA.
*
*   Title: remove_trailing_blanks().
*
*   Author: Ralph J. Folz
*
*   Date Created: Mon Nov 08 15:19:39 1993
*
*   Revision History:
*
*   Description: This function will remove the trailing blanks from
*      the passed in string.
*
*   Inputs:
*
*      string	-> string to remove trailing blanks from.
*
*   Outputs: 
*
*      string	-> string is updated to remove trailing blanks.
*
**--*********************************************************************/
#ifdef _NO_PROTO
void remove_trailing_blanks(string)
char	*string;
#else
void remove_trailing_blanks(char *string)
#endif
{
   int		string_length;
   char		single_char;

   string_length = strlen(string);
   if(string_length == 0)
      return;

   single_char = string[string_length - 1];
   while(single_char == ' ')
   {
      string[string_length - 1] = '\0';
      if(string_length > 0)
         string_length--;
      else
         return;
   
      single_char = string[string_length - 1];
   }
}

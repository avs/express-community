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
*   Title: read_neutral_header().
*
*   Author: Ralph J. Folz
*
*   Date Created: Mon Nov 08 09:39:55 1993
*
*   Revision History:
*
*   Description: This function will read the fidap neutral header.
*
*   Inputs:
*
*      fp	-> File Pointer to open Fidap Neutral File.
*
*   Outputs:
*
*      header structure (Neutral_Header) is returned.
*
*      status is returned.   0 = OK  -1 = Error.
*
**--*********************************************************************/
#ifdef _NO_PROTO
int read_neutral_header(fp, header_struct)
FILE		*fp;
Neutral_Header	*header_struct;
#else
int read_neutral_header(FILE *fp, Neutral_Header *header_struct)
#endif
{
   char		line[MAX_LINE];
   int		status;
   int		string_length;
   char		single_char;
   char		temp_string[MAX_LINE];

   /*********************************************************************/
   /* Read in the first line of the Neutral file.  This line should 	*/
   /* contain "** FIDAP NEUTRAL FILE" as the first string.  If this is	*/
   /* not the case, it is probably not a fidap neutral file.		*/
   /*********************************************************************/
   status = read_line(fp, line);
   if(status != 0)
      return(-1);

   if(strncmp(line, "** FIDAP NEUTRAL FILE", 21) != 0)
   {
      fprintf(stdout, "First Line is not \"** FIDAP NEUTRAL FILE\".\n");
      fprintf(stdout, "The fid2meta program works only with ASCII Fidap Neutral Files.\n");
      fprintf(stdout, "Exiting...\n");
      return(-1);
   }

   /*********************************************************************/
   /* Now read in the user defined title.				*/
   /*********************************************************************/
   status = read_string(fp, header_struct->title, 80);
   if(status != 0)
      return(-1); 

   string_length = strlen(header_struct->title);
   /* First check if the last character is new-line.  If it is not,	*/
   /* I will want to position the file pointer at the next line.	*/
   if(header_struct->title[string_length - 1] != '\n')
      read_past_newline(fp);

   /* I am now going to remove trailing blanks and new-lines from the 	*/
   /* title.								*/
   if(string_length > 0)
   {
      single_char = header_struct->title[string_length - 1];
      while((single_char == ' ') || (single_char == '\n'))
      {
         header_struct->title[string_length - 1] = '\0';
         if(string_length == 0)
            break;
         else
            string_length--;
         single_char = header_struct->title[string_length - 1];
      }
   }

   /*********************************************************************/
   /* Now read in the revision level.					*/
   /*********************************************************************/
   status = read_string(fp, temp_string, 10);
   if(status != 0)
      return(-1);

   if(strncmp(temp_string, "VERSION   ", 10) != 0)
   {
      fprintf(stdout, "Error.  Expected \"VERSION   \".  Read \"%s\".\n", 
         temp_string);
   }

   status = read_variable_float(fp, &header_struct->revision_level);
   if(status != 0)
      return(-1);

   if(header_struct->revision_level < 7.0)
   {
      fprintf(stdout, "Revision %f detected.  Only Version 7.0 and higher supported by this reader.\n",
         header_struct->revision_level);
      return(-1);
   }

/*
   if(header_struct->revision_level < 7.0)
      header_struct->int_version = 6;
   else
      header_struct->int_version = 7;*/

   header_struct->int_version = 7;

   read_past_newline(fp);

   /*********************************************************************/
   /* Now read in the date and time records.				*/
   /*********************************************************************/
   read_past_newline(fp);

   /*********************************************************************/
   /* Now read in the NUMNP, NELEM, NGRPS, NDFCD, and NDFVL variables.	*/
   /*********************************************************************/
   read_past_newline(fp);
   status = read_int(fp, &header_struct->numnp, 15);
   if(status != 0)
      return(-1);

   status = read_int(fp, &header_struct->nelem, 15);
   if(status != 0)
      return(-1);

   status = read_int(fp, &header_struct->ngrps, 15);
   if(status != 0)
      return(-1);

   status = read_int(fp, &header_struct->ndfcd, 15);
   if(status != 0)
      return(-1);

   status = read_int(fp, &header_struct->ndfvl, 15);
   if(status != 0)
      return(-1);

   read_past_newline(fp);

   /*********************************************************************/
   /* Now read in the ITIM, ITURB, IFREE, and ICOMPR variables.		*/
   /*********************************************************************/
   read_past_newline(fp);
   status = read_int(fp, &header_struct->itim, 15);
   if(status != 0)
      return(-1);

   status = read_int(fp, &header_struct->iturb, 15);
   if(status != 0)
      return(-1);

   status = read_int(fp, &header_struct->ifree, 15);
   if(status != 0)
      return(-1);

   status = read_int(fp, &header_struct->icompr, 15);
   if(status != 0)
      return(-1);

   read_past_newline(fp);

   /*********************************************************************/
   /* Now read in the Transport Equation Solution Flags			*/
   /*********************************************************************/
   read_past_newline(fp);

   /* RJF for now pass up the Transport Equation Solution Flags.	*/
   read_past_newline(fp);

   /*********************************************************************/
   /* Now read in the Pressure Solution Flags IDCTS, IPENY, and MPDF	*/
   /*********************************************************************/
   read_past_newline(fp);
   status = read_int(fp, &header_struct->idcts, 10);
   if(status != 0)
      return(-1);

   status = read_int(fp, &header_struct->ipeny, 10);
   if(status != 0)
      return(-1);

   status = read_int(fp, &header_struct->mpdf, 10);
   if(status != 0)
      return(-1);

   read_past_newline(fp);

#ifdef DEBUG
   fprintf(stdout, "\n");
   fprintf(stdout, "Statistics:\n");
   fprintf(stdout, "===========\n");
   fprintf(stdout, "Problem Title:        %s\n", header_struct->title);
   fprintf(stdout, "Fidap Version:        %f\n", header_struct->revision_level);
   fprintf(stdout, "Num Nodes:            %d\n", header_struct->numnp);
   fprintf(stdout, "Num Elem:             %d\n", header_struct->nelem);
   fprintf(stdout, "Num Groups:           %d\n", header_struct->ngrps);
   fprintf(stdout, "Num Coord Directions: %d\n", header_struct->ndfcd);
   fprintf(stdout, "Num Velocity Comp:    %d\n", header_struct->ndfvl);
   if(header_struct->itim == 0)
      fprintf(stdout, "-- Steady State.\n");
   else if(header_struct->itim == 1)
      fprintf(stdout, "-- Transient.\n");
   else
      fprintf(stdout, "Error... Unknown ITIM setting: %d\n", header_struct->itim);
   if(header_struct->iturb == 0)
      fprintf(stdout, "-- Laminar.\n");
   else if(header_struct->iturb == 2)
      fprintf(stdout, "-- K-E Turbulence.\n");
   else
      fprintf(stdout, "Error... Unknown ITURB setting: %d\n", header_struct->iturb);
   if(header_struct->ifree == 0)
      fprintf(stdout, "-- Fixed.\n");
   else if(header_struct->ifree == 1)
      fprintf(stdout, "-- Free Surface.\n");
   else
      fprintf(stdout, "Error... Unknown IFREE setting: %d\n", header_struct->ifree);
   if(header_struct->icompr == 0)
      fprintf(stdout, "-- Incompressible.\n");
   else if(header_struct->icompr == 2)
      fprintf(stdout, "-- Compressible.\n");
   else
      fprintf(stdout, "Error... Unknown ICOMPR setting: %d\n", header_struct->icompr);
   if(header_struct->idcts == 0)
      fprintf(stdout, "-- Continuous Pressure.\n");
   else if(header_struct->idcts == 1)
      fprintf(stdout, "-- Discontinuous Pressure.\n");
   else
      fprintf(stdout, "Error... Unknown IDCTS setting: %d\n", header_struct->idcts);
   if(header_struct->ipeny == 0)
      fprintf(stdout, "-- Penalty.\n");
   else if(header_struct->ipeny == 1)
      fprintf(stdout, "-- Mixed.\n");
   else
      fprintf(stdout, "Error... Unknown IPENY setting: %d\n", header_struct->ipeny);
   fprintf(stdout, "Max Pres. DOF / Elem: %d\n", header_struct->mpdf);
#endif
   return(0);
}

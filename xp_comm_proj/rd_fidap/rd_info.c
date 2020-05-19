
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xp_comm_proj/rd_fidap/fid2FLD.h"


int scan_neutral_file(FILE *fp, char* label, int rewind)
{
   char  line[MAX_LINE];
   int   status;
  
   line[0] = '\0';
   while(strncmp(line, label, strlen(label))) {
     status = read_string_no_err(fp, line, strlen(label));
     if(status != 0)
       return(-1);
     read_past_newline(fp);
   }

   return(0);
}


int read_element_groupsInfo(FILE *fp, Neutral_Header neutral_header, 
   Element_Group *elem_groups)
{
  int			i, j, k, dummy;
  char			temp_string[MAX_LINE];
  int			status;
  int			col_count;

   /*********************************************************************/
   /* Loop once for each of the element groups.				*/
   /* Read in the Element Group Control Information.			*/
   /*********************************************************************/
   for(i = 0; i < neutral_header.ngrps; i++)
   {
      /******************************************************************/
      /* Read in Element Group Number (NGP).				*/
      /******************************************************************/
      status = read_string(fp, temp_string, 10);
      if(status != 0)
         return(-1);

      if(strncmp(temp_string, "GROUP:    ", 10) != 0)
      {
         fprintf(stdout, "Error... Expected \"GROUP:    \" read in ***%s***\n", 
            temp_string);
         return(-1);
      }

      status = read_int(fp, &elem_groups[i].ngp, 5);
      if(status != 0)
         return(-1);

      /******************************************************************/
      /* Read in Number of Elements in Group (NELGP)			*/
      /******************************************************************/
      status = read_string(fp, temp_string, 10);
      if(status != 0)
         return(-1);

      if(strncmp(temp_string, " ELEMENTS:", 10) != 0)
      {
         fprintf(stdout, "Error... Expected \" ELEMENTS:\" read in ***%s***\n", 
            temp_string);
         return(-1);
      }

      status = read_int(fp, &elem_groups[i].nelgp, 10);
      if(status != 0)
         return(-1);

      /******************************************************************/
      /* Read in Number of Nodes per Element in Group. (NDP)		*/
      /******************************************************************/
      status = read_string(fp, temp_string, 10);
      if(status != 0)
         return(-1);

      if(strncmp(temp_string, " NODES:   ", 10) != 0)
      {
         fprintf(stdout, "Error... Expected \" NODES:   \" read in ***%s***\n",
            temp_string);
         return(-1);
      }

      status = read_int(fp, &elem_groups[i].ndp, 10);
      if(status != 0)
         return(-1);

      /******************************************************************/
      /* Read in Element Geometry Type (IGEOM).				*/
      /******************************************************************/
      status = read_string(fp, temp_string, 10);
      if(status != 0)
         return(-1);

      if(strncmp(temp_string, " GEOMETRY:", 10) != 0)
      {
         fprintf(stdout, "Error... Expected \" GEOMETRY:\" read in ***%s***\n",
            temp_string);
         return(-1);
      }

      status = read_int(fp, &elem_groups[i].igeom, 5);
      if(status != 0)
         return(-1);

      /******************************************************************/
      /* Read in FIDAP Element Type (NFTYP).                            */
      /******************************************************************/
      status = read_string(fp, temp_string, 6);
      if(status != 0)
         return(-1);

      if(strncmp(temp_string, " TYPE:", 6) != 0)
      {
         fprintf(stdout, "Error... Expected \" TYPE:\" read in ***%s***\n",
            temp_string);
         return(-1);
      }

      status = read_int(fp, &elem_groups[i].nftyp, 4);
      if(status != 0)
         return(-1);

      read_past_newline(fp);

      if((elem_groups[i].nftyp >= 1) && (elem_groups[i].nftyp <= 5))
         elem_groups[i].boundary_flag = 0;
      else
         elem_groups[i].boundary_flag = 1;

      /******************************************************************/
      /* Now read in the Entity Name (ELMMAT)				*/
      /******************************************************************/
      status = read_string(fp, temp_string, 15);
      if(status != 0)
         return(-1);

      if(strncmp(temp_string, "ENTITY NAME:   ", 15) != 0)
      {
         fprintf(stdout, "Error... Expected \" TYPE:\" read in ***%s***\n",
            temp_string);
         return(-1);
      }

      status = read_string(fp, elem_groups[i].elmmat, 20);
      if(status != 0)
         return(-1);

      remove_trailing_blanks(elem_groups[i].elmmat);
      read_past_newline(fp);

      for(j = 0; j < elem_groups[i].nelgp; j++)
      {

         col_count = 8;
         status = read_int(fp, &dummy, 8);
         if(status != 0)
            return(-1);

         for(k = 0; k < elem_groups[i].ndp; k++)
         {
            col_count = col_count + 8;
            if(col_count > 80)
            {
               read_past_newline(fp);
               status = read_string(fp, temp_string, 8);
               if(status != 0)
                  return(-1);
               col_count = 16;
            }

            status = read_int(fp, &dummy, 8);
            if(status != 0)
               return(-1);

         }
         read_past_newline(fp);
      }
   }

#ifdef DEBUG
   fprintf(stdout, "\n");
   fprintf(stdout, "Element Group Information:\n");
   fprintf(stdout, "==========================\n");
   for(i = 0; i < neutral_header.ngrps; i++)
   {
      fprintf(stdout, "Element Group Number:      %d\n",
         elem_groups[i].ngp);
      fprintf(stdout, "Number Elements In Group:  %d\n",
         elem_groups[i].nelgp);
      fprintf(stdout, "Number Of Nodes / Element: %d\n",
         elem_groups[i].ndp);
      fprintf(stdout, "Element Geometry Type:     %d\n",
         elem_groups[i].igeom);
      fprintf(stdout, "Fidap Element Type:        %d\n",
         elem_groups[i].nftyp);
      fprintf(stdout, "Entity Name:               ***%s***\n",
         elem_groups[i].elmmat);
      fprintf(stdout, "\n");
   }
#endif
   return(0);
}


int read_nodal_resultsInfo(FILE *fp, Neutral_Header neutral_header, 
   char *labels[], int *num_labels, int* num_timesteps)
{
  char			temp_string[MAX_LINE];
  int			status, done;
  int			num_lines;
  int			i, j;
  int			loop_total;
  int			mod_value;
  char			convert[10];

  *num_labels = 0;
  *num_timesteps = 0;

  mod_value = neutral_header.numnp % 5;
  num_lines = neutral_header.numnp / 5;
  if(mod_value != 0)
    num_lines++;

  status = read_string_no_err(fp, temp_string, 10);
  if(status != 0) {
    if(feof(fp)) {
      return(0);
    }
    else
      return(-1);
  }
  else {
    if(strncmp(temp_string, "TIMESTEP: ", 10) != 0) {
      fprintf(stdout, "Error.  Expecting \"TIMESTEP: \", read in \"%s\"",
	      temp_string);
      return(-1);
    }
  }

  *num_timesteps = 1;
  read_past_newline(fp);

  done = 0;

  while(!done) {
    status = read_string(fp, temp_string, 13);
    if(status != 0)
      return(-1);

    if (strncmp(temp_string, "VELOCITY     ", 13) == 0) {
      read_past_newline(fp);

      temp_string[8] = '\0';
      labels[*num_labels] = (char *)malloc(9);
      strcpy(labels[(*num_labels)++],temp_string);

      loop_total = neutral_header.numnp * neutral_header.ndfvl;
      if((loop_total % 5) != 0)
	loop_total = (loop_total / 5) + 1;
      else
	loop_total = (loop_total / 5);
      for(i = 0; i < loop_total; i++)
	read_past_newline(fp);
    }
    else if (strncmp(temp_string, "PRESSURE     ", 13) == 0) {
      read_past_newline(fp);

      temp_string[8] = '\0';
      labels[*num_labels] = (char *)malloc(9);
      strncpy(labels[(*num_labels)++],temp_string,9);
      

      for(i = 0; i < num_lines; i++)
	read_past_newline(fp);
    }
    else if (strncmp(temp_string, "TEMPERATURE  ", 13) == 0) {
      read_past_newline(fp);

      temp_string[11] = '\0';
      labels[*num_labels] = (char *)malloc(12);
      strncpy(labels[(*num_labels)++],temp_string,12);

      for(i = 0; i < num_lines; i++)
	read_past_newline(fp);
    }
    else if (strncmp(temp_string, "COORDINATE   ", 13) == 0) {
      read_past_newline(fp);

      temp_string[10] = '\0';

      for(i = 0; i < neutral_header.ndfcd; i++) {
      labels[*num_labels] = (char *)malloc(11);
      strncpy(labels[(*num_labels)++],temp_string,11);
      for(j = 0; j < num_lines; j++)
	read_past_newline(fp);
      /* If it is not the last one, read in header	*/
      if(i != (neutral_header.ndfcd - 1))
	read_past_newline(fp);
      }
    }
    else if (strncmp(temp_string, "TURBULENT K.E", 13) == 0) {
      read_past_newline(fp);

      temp_string[13] = '\0';
      labels[*num_labels] = (char *)malloc(14);
      strncpy(labels[(*num_labels)++],temp_string,14);

      for(i = 0; i < num_lines; i++)
	read_past_newline(fp);
    }
    else if (strncmp(temp_string, "TURBULENT DIS", 13) == 0) {
      read_past_newline(fp);

      temp_string[13] = '\0';
      labels[*num_labels] = (char *)malloc(14);
      strncpy(labels[(*num_labels)++],temp_string,14);

      for(i = 0; i < num_lines; i++)
	read_past_newline(fp);
    }
    else if (strncmp(temp_string, "SPECIES", 7) == 0) {
      read_past_newline(fp);

      temp_string[10] = '\0';
      labels[*num_labels] = (char *)malloc(11);
      strncpy(labels[(*num_labels)++],temp_string,11);

      for(i = 0; i < num_lines; i++)
	read_past_newline(fp);
    }
    else if (strncmp(temp_string, "DENSITY      ", 13) == 0) {
      read_past_newline(fp);

      temp_string[7] = '\0';
      labels[*num_labels] = (char *)malloc(8);
      strncpy(labels[(*num_labels)++],temp_string,8);

      for(i = 0; i < num_lines; i++)
	read_past_newline(fp);
    }
    else if(strncmp(temp_string, "ENDOFTIMESTEP", 13) == 0) {
      read_past_newline(fp);
      while(1) {
	status = scan_neutral_file(fp, "TIMESTEP: ", 0);
	if (status != 0)
	  break;
	(*num_timesteps)++;
      }
      done = 1;
    }
    else {
      fprintf(stdout, "Unknown TYPE ***%s***\n", temp_string);
      read_past_newline(fp);
    }
  }

  return(0);
}

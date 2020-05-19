/*
	Create a file name for a particular time step

	Written by:     Ken Flurchick
	Date:           04/23/98

	Adapted from the animate file name module from AVS5
*/

#include "express.h"

int
animFile(OMobj_id animFile_id, OMevent_mask event_mask, int seq_num)
{
/***********************/
/*  Declare variables  */
/***********************/
	char  	*Root_Name = NULL;
	int		num;
   	int  	time_step;
   	char  	*FileName = NULL;
	char 	tmp[5], *t;
	char 	fname[80], *f;
/***********************/
/*  Get input values   */
/***********************/
   	/* Get Root_Name's value */
   	if (OMget_name_str_val(animFile_id, OMstr_to_name("Root_Name"),
		&Root_Name, 0) != 1) Root_Name = NULL;

   	/* Get num's value */
   	if (OMget_name_int_val(animFile_id, OMstr_to_name("num"),
		&num) != 1) num = 0;

   	/* Get time_step's value */
   	if (OMget_name_int_val(animFile_id, OMstr_to_name("time_step"),
		&time_step) != 1) time_step = 0;

	printf("Time step %d and root name %s\n", time_step, Root_Name);
/***********************/
/* Function's Body     */
/***********************/
   	/*  ERRverror("",ERR_NO_HEADER | ERR_INFO,
		"I'm in function: animFile.\n");
	*/
	t = &tmp[0];
	f = &fname[0];
	strcpy (f, Root_Name);
	if(num <= 0 || num > 5) {
		printf("Select a number from 1 to 5\n");
		return(0);
	}
	if(num == 1) sprintf (t, ".%01d", time_step);
	if(num == 2) sprintf (t, ".%02d", time_step);
	if(num == 3) sprintf (t, ".%03d", time_step);
	if(num == 4) sprintf (t, ".%04d", time_step);
	if(num == 5) sprintf (t, ".%05d", time_step);
	strcat (f, t);

/***********************/
/*  Set output values  */
/***********************/
   	/* Set FileName's value */
   	OMset_name_str_val(animFile_id, OMstr_to_name("FileName"), f);

/*************************/
/*  Free input variables */
/*************************/
   	if (Root_Name) free(Root_Name);
   	return(1);
}

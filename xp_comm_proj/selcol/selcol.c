
#include "gen.h"

/* #define DEBUG */

int
SelectColumnCore(OMobj_id SelectColumnCore_id, OMevent_mask event_mask, int seq_num)
{
	/***********************/
	/*	Declare variables  */
	/***********************/
	int  selected_size = 0;
	int *selected = NULL; 
	char	*label;
	OMobj_id labels_id;
	int labels_size = 0, labels_count;
	int index;
	OMobj_id sel_id;
	OMobj_id Columns_In_id;
	OMobj_id Columns_Out_id;
	char objname [512];
	int i, in_size;

	/***********************/
	/*	 Get input values	  */
	/***********************/

	Columns_In_id = OMfind_subobj(SelectColumnCore_id, OMstr_to_name("Columns_In"), OM_OBJ_RW); 

	selected = (int *)OMret_name_array_ptr(SelectColumnCore_id, OMstr_to_name("selected"), OM_GET_ARRAY_RD,
			&selected_size, NULL);


	Columns_In_id = OMfind_subobj(SelectColumnCore_id, OMstr_to_name("Columns_In"), OM_OBJ_RW); 
	Columns_Out_id = OMfind_subobj(SelectColumnCore_id, OMstr_to_name("Columns_Out"), OM_OBJ_RW);

	if (OMget_array_size(Columns_In_id, &in_size) != 1) 
		in_size = 0;


 	OMset_array_size(Columns_Out_id,0);
	if (!selected_size) return 0;
		
	for (i=0;i<selected_size;i++)
	{
		
#ifdef DEBUG
		fprintf(stderr,"selected[%i]: %i\n",i,selected[i]);
#endif
		if (selected[i] >= in_size) {
			ERRverror("",ERR_NO_HEADER | ERR_INFO,"ColumnSelector: seledted index %i out of range\n",selected[i]);		
			continue;
		};
			
		if (OMget_array_val(Columns_In_id, selected[i], &sel_id, OM_OBJ_RD) != 1) 
			 continue;			 

#ifdef DEBUG
		fprintf(stderr,"sel_id: %i\n",sel_id);
#endif
		OMset_array_val(Columns_Out_id,OM_ARRAY_APPEND,sel_id);
	}



	/***********************/
	/* Function's Body	  */
	/***********************/

#ifdef DEBUG
	ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: SelectColumnCore generated from method: ColumnSelectorCore.update\n");
	fprintf(stderr,"I'm in function: SelectColumnCore generated from method: ColumnSelectorCore.update\n");fflush(stderr);
#endif

	/***********************/
	/*	 Set output values  */
	/***********************/
	/* Set labels array */ 
	labels_id = OMfind_subobj(SelectColumnCore_id, OMstr_to_name("labels"), OM_OBJ_RW);
	labels_size=in_size;
	OMset_array_size(labels_id, labels_size); /* Set the number of strings */
	for (labels_count = 0; labels_count < labels_size;	 labels_count++) {
		if (OMget_array_val(Columns_In_id, labels_count, &sel_id, OM_OBJ_RD) != 1) 
			 continue;
		OMget_name_str_val(sel_id,OMstr_to_name("labels"),&label,0);
		OMset_str_array_val(labels_id, labels_count, label);
	}


	/*************************/
	/*	 Free input variables */
	/*************************/
	if (selected != NULL) 
		ARRfree(selected);

	return(1);
}

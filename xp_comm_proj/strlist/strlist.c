
#include "gen.h"

/*#define DEBUG /**/

int
StringListUpdate(OMobj_id StringListCore_id, OMevent_mask event_mask, int seq_num)
{
	/***********************/
	/*	 Declare variables  */
	/***********************/
	char **list = NULL;
	OMobj_id list_id;
	OMobj_id sel_id;
	int  list_size = 0, list_count;
	char *item = NULL;
	int  index;
	int  append;
	int  offset;
	int  insert;
	int  delete_;
	char *output = NULL;
	OMobj_id output_id;
	int  output_size = 0, output_count;

	/***********************/
	/*	 Get input values	  */
	/***********************/
	/* Get string array list */
	list_id = OMfind_subobj(StringListCore_id, OMstr_to_name("list"), OM_OBJ_RW);

	/* Get the number of strings */
	if (OMget_array_size(list_id, &list_size) != 1 || list_size == 0)
		list = NULL;
	else 
		list = (char **)malloc(list_size * sizeof(char *));
	for (list_count = 0; list_count < list_size; list_count++) {
		if (OMget_str_array_val(list_id, list_count, &(list[list_count]), 0) != 1)
			list[list_count] = NULL;
	}


	/* Get item's value */
	if (OMget_name_str_val(StringListCore_id, OMstr_to_name("item"), &item, 0) != 1)
		item = NULL;

	/* Get index's value */ 
	if (OMget_name_int_val(StringListCore_id, OMstr_to_name("index"), &index) != 1) 
		index = -2;

	/* Get append's value */ 
	if (OMget_name_int_val(StringListCore_id, OMstr_to_name("append"), &append) != 1) 
		append = 0;

	/* Get insert's value */ 
	if (OMget_name_int_val(StringListCore_id, OMstr_to_name("insert"), &insert) != 1) 
		insert = 0;

	/* Get delete_'s value */ 
	if (OMget_name_int_val(StringListCore_id, OMstr_to_name("delete"), &delete_) != 1) 
		delete_ = 0;


	/***********************/
	/* Function's Body	  */
	/***********************/

#ifdef DEBUG
	ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: StringListUpdate generated from method: StringListCore.Update\n");
	fprintf(stderr,"\nI'm in function: StringListUpdate generated from method: StringListCore.Update\n");
	fprintf(stderr,"\nlist_size: %i\n",list_size);
#endif

	do
	{
		if (append || insert)
		{
			output_size = list_size+1;
			break;
		}
		if (delete_)
		{
			if ((index>= 0) && (index<list_size))
			{
				output_size = list_size-1;
				break;	
			}
		}
		output_size = list_size;
	}
	while(0);	

	output_id = OMfind_subobj(StringListCore_id, OMstr_to_name("list"), OM_OBJ_RW);
	OMset_array_size(output_id, output_size); 
	offset=0;
	for (output_count = 0; output_count < output_size; output_count++) {
		if (delete_)
		{
			if (index==output_count)
				offset++;
			output=list[output_count+offset];
		}
		if (append)
			if (output_count == output_size-1)
				output=item;
			else
				output=list[output_count];
		if (insert) /* Array should be sorted or have a given index */
		{
			if (!offset)
				if ((index>= 0) && (index<list_size))
				{
					if (index==output_count)
					{
						output=item;
						offset=-1;
					} else 
						output=list[output_count+offset];
				} else {
#ifdef DEBUG
fprintf(stderr,"before strcmp");
#endif
					if ((!list_size) || (list_size==output_count+offset))
						output=item;
					else 
						if (strcmp(item,list[output_count+offset])<0)
						{
#ifdef DEBUG
fprintf(stderr,"if strcmp");
#endif
							output=item;
							offset=-1;
						} else { 
#ifdef DEBUG
fprintf(stderr,"else strcmp");
#endif
							output=list[output_count+offset];
						}
				}
			else
				output=list[output_count+offset];					
		}				
#ifdef DEBUG
		fprintf(stderr,"%s\n",output);
#endif		
		OMset_str_array_val(output_id, output_count, output);
	}
	if (!output_size)
	{
		OMset_obj_val(output_id,OMnull_obj,0);
	}
#ifdef DEBUG
	fprintf(stderr,"\n",output);
#endif		

		
	/***********************/
	/*	 Set output values  */
	/***********************/
	/* Set append's value */  
	OMset_name_int_val(StringListCore_id, OMstr_to_name("append"), 0);
	/* Set insert's value */  
	OMset_name_int_val(StringListCore_id, OMstr_to_name("insert"), 0);
	/* Set delete_'s value */	
	OMset_name_int_val(StringListCore_id, OMstr_to_name("delete"), 0);
	/* Set output array */ 
	if ((output_size)&&(index>=output_size))
		OMset_name_int_val(StringListCore_id, OMstr_to_name("index"), output_size-1);


	/*************************/
	/*	 Free input variables */
	/*************************/
	if (list != NULL) {
		for (list_count = 0; list_count < list_size; list_count++) {
			if (list[list_count] != NULL)
				free((char *)list[list_count]);
		}
		free((char *)list);
	}

	if (item)
		free(item);

	return(1);
}

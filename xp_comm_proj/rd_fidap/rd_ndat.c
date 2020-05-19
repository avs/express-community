
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <avs/om.h>
#include <avs/port.h>
#include <avs/err.h>
#include <avs/gd_def.h>


#include "xp_comm_proj/rd_fidap/fid2FLD.h"


int read_nodal_results(FILE *fp, Neutral_Header neutral_header,
		       int *load_items, int size, OMobj_id ndat_id)
{
  char          temp_string[MAX_LINE];
  int		status;
  int		i, j, index, cur_item, item;
  int	       	num_lines;
  int	       	loop_total;
  int	       	mod_value;

  int    out_ndat_data_type, out_ndat_data_size;
  char   *out_ndat_data;
  float  *data_ptr;

  cur_item = 0;

  mod_value = neutral_header.numnp % 5;
  num_lines = neutral_header.numnp / 5;
  if(mod_value != 0)
    num_lines++;

  for (index=0; index<size; index++) {

    item = load_items[index];

    for (j=cur_item; j<item; j++) {
      status = read_line(fp,temp_string);
      /*
      printf("skipping %s\n",temp_string);
      */
      if (strncmp(temp_string, "VELOCITY     ", 13) == 0) {
	loop_total = neutral_header.numnp * neutral_header.ndfvl;
	if((loop_total % 5) != 0)
	  loop_total = (loop_total / 5) + 1;
	else
	  loop_total = (loop_total / 5);
	for(i = 0; i < loop_total; i++)
	  read_past_newline(fp);
      }
      else
	for(i = 0; i < num_lines; i++)
	  read_past_newline(fp);
    }

    status = read_string(fp, temp_string, 13);
    /*
    printf("%d: reading %s to %d\n",item,temp_string,data_ptr);
    */

    out_ndat_data_type = DTYPE_FLOAT;

    status = FLDget_node_data (ndat_id,
			       index,
			       &out_ndat_data_type,
			       &out_ndat_data,
			       &out_ndat_data_size,
			       OM_GET_ARRAY_WR);

    /*
    printf("status for node data values: %d\n",status);
    printf("data type found: %d\n",out_ndat_data_type);
    */

    data_ptr = (float *)out_ndat_data;

    if (strncmp(temp_string, "VELOCITY     ", 13) == 0)
      loop_total = neutral_header.numnp * neutral_header.ndfvl;
    else
      loop_total = neutral_header.numnp;
    /*
    printf("sizes: %d,  %d\n",loop_total,out_ndat_data_size);
    */

    for(i = 0; i < loop_total; i++) {
      /*      printf("i = %d\n",i);*/
      if ((i%5) == 0)
	read_past_newline(fp);
      status = read_float(fp, data_ptr, 16);
      if (strncmp(temp_string, "TEMPERATURE  ", 13) == 0)
	*data_ptr = *data_ptr - 273.15;
      data_ptr++;
    }

    if (out_ndat_data)
      ARRfree(out_ndat_data);

    read_past_newline(fp);
    cur_item = item + 1;
  }

  return(1);
}

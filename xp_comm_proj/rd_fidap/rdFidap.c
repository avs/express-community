
#include <avs/om.h>
#include <avs/port.h>
#include <avs/err.h>
#include <avs/gd_def.h>
#include <math.h>


#include "xp_comm_proj/rd_fidap/fid2FLD.h"

int scan_neutral_file(FILE*, char*, int);
int read_element_groupsInfo(FILE*, Neutral_Header, Element_Group *);
int read_nodal_resultsInfo(FILE*, Neutral_Header, char**, int*, int*);
int read_nodal_coordinates(FILE*, Neutral_Header, int**, OMobj_id);
int read_element_groups(FILE*, Neutral_Header, int*, OMobj_id);
int read_nodal_results(FILE*, Neutral_Header, int*, int, OMobj_id);

int
rdFidapHeader_func(OMobj_id IVrdFidapResult_id, OMevent_mask event_mask, int seq_num)
{
  /***********************/
  /*  Declare variables  */
  /***********************/
  char  *filename = NULL;
  OMobj_id header_id;

  int			status;
  FILE			*fp;
  Neutral_Header	neutral_header;
  int			fdneut_opened = 0;

  /***********************/
  /*  Get input values   */
  /***********************/
  /* Get filename's value */
  if (OMget_name_str_val(IVrdFidapResult_id,
			 OMstr_to_name("filename"),
			 &filename, 0) != 1)
    return(0);

  /***********************/
  /* Function's Body     */
  /***********************/

  fp = open_neutral_file(filename);
  if(fp == NULL)
    return(0);

  fdneut_opened = 1;

  status = read_neutral_header(fp, &neutral_header);
  if(status != 0)
    return(0);

  header_id = OMfind_subobj(IVrdFidapResult_id,
			    OMstr_to_name("header"),
			    OM_OBJ_RW);

  status = OMset_name_str_val(header_id,
			      OMstr_to_name("title"),
			      neutral_header.title);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("revision_level"),
			      neutral_header.revision_level);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("int_version"),
			      neutral_header.int_version);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("numnp"),
			      neutral_header.numnp);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("nelem"),
			      neutral_header.nelem);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("ngrps"),
			      neutral_header.ngrps);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("ndfcd"),
			      neutral_header.ndfcd);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("ndfvl"),
			      neutral_header.ndfvl);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("itim"),
			      neutral_header.itim);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("iturb"),
			      neutral_header.iturb);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("ifree"),
			      neutral_header.ifree);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("icompr"),
			      neutral_header.icompr);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("idcts"),
			      neutral_header.idcts);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("ipeny"),
			      neutral_header.ipeny);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("mpdf"),
			      neutral_header.mpdf);


  /*************************/
  /*  Free input variables */
  /*************************/
  if (filename)
    free(filename);

  fclose(fp);

  return(1);
}


int
rdFidapInfo_func(OMobj_id IVrdFidapResult_id, OMevent_mask event_mask, int seq_num)
{
  /***********************/
  /*  Declare variables  */
  /***********************/
  char  *filename = NULL;
  OMobj_id header_id;
  OMobj_id out_mesh_id;
  int out_mesh_nspace, out_mesh_nnodes, out_mesh_size;
  int  out_mesh_nsets, out_mesh_set_count, out_mesh_ncells;
  OMobj_id out_mesh_cell_set;
  OMobj_id tmp_id;
  int *time_control, time_control_size;

  char *cell_names[5] = {"Quad","Tri","Hex","Prism","Tet"};
  char label[21];
  char *labels[30];
  int num_labels, num_timesteps;

  int			status, i, j;
  FILE			*fp;
  Neutral_Header	neutral_header;
  Element_Group         *elem_groups;
  int			fdneut_opened = 0;

  /***********************/
  /*  Get input values   */
  /***********************/

  /* Get filename's value */
  if (OMget_name_str_val(IVrdFidapResult_id,
			 OMstr_to_name("filename"),
			 &filename, 0) != 1)
    return(0);


  /***********************/
  /* Function's Body     */
  /***********************/

  fp = open_neutral_file(filename);
  if(fp == NULL)
    return(0);

  fdneut_opened = 1;

  status = read_neutral_header(fp, &neutral_header);
  if(status != 0)
    return(0);

  header_id = OMfind_subobj(IVrdFidapResult_id,
			    OMstr_to_name("header"),
			    OM_OBJ_RW);

  status = OMset_name_str_val(header_id,
			      OMstr_to_name("title"),
			      neutral_header.title);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("revision_level"),
			      neutral_header.revision_level);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("int_version"),
			      neutral_header.int_version);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("numnp"),
			      neutral_header.numnp);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("nelem"),
			      neutral_header.nelem);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("ngrps"),
			      neutral_header.ngrps);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("ndfcd"),
			      neutral_header.ndfcd);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("ndfvl"),
			      neutral_header.ndfvl);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("itim"),
			      neutral_header.itim);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("iturb"),
			      neutral_header.iturb);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("ifree"),
			      neutral_header.ifree);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("icompr"),
			      neutral_header.icompr);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("idcts"),
			      neutral_header.idcts);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("ipeny"),
			      neutral_header.ipeny);
  status = OMset_name_int_val(header_id,
			      OMstr_to_name("mpdf"),
			      neutral_header.mpdf);
  /*
  printf("\nstored header in group structure\n");
  */


  /***********************/
  /*  Set output values  */
  /***********************/

  /* set  out_mesh mesh */

  /*  Get mesh id */
  out_mesh_id = OMfind_subobj(IVrdFidapResult_id,
			      OMstr_to_name("out_mesh"),
			      OM_OBJ_RW);

  elem_groups = (Element_Group *)malloc(sizeof(Element_Group) *
					neutral_header.ngrps);
  if(elem_groups == NULL) {
    fprintf(stdout, "Error allocating memory.  Exiting...\n");
    return(0);
  }

  status = scan_neutral_file(fp,"ELEMENT GROUPS",0);
  status = read_element_groupsInfo(fp,
				   neutral_header,
				   elem_groups);
  /*
  printf("returned from read_element_groupsInfo with status = %d\n",status);
  */
  if(status != 0)
    return(0);

  out_mesh_nsets = 0;
  FLDset_ncell_sets(out_mesh_id, out_mesh_nsets);

  for(i = 0; i < neutral_header.ngrps; i++) {
    if (elem_groups[i].nftyp <= 5) {
      status = 0;
      for(j = 0; j < out_mesh_nsets; j++) {
	FLDget_cell_set(out_mesh_id, j, &out_mesh_cell_set);
        FLDget_cell_set_user_name(out_mesh_cell_set, label, 21);
	if (strncmp(elem_groups[i].elmmat, label, strlen(label)))
	  continue;
	if (strlen(elem_groups[i].elmmat) != strlen(label))
	  continue;
	FLDget_cell_set_name(out_mesh_cell_set, label, 21);
	if (strncmp(cell_names[elem_groups[i].nftyp-1], label, strlen(label)))
	  continue;
	status = 1;
        break;
      }
      if (!status) {
	FLDadd_cell_set(out_mesh_id, cell_names[elem_groups[i].nftyp-1]);
	FLDget_cell_set(out_mesh_id, out_mesh_nsets++, &out_mesh_cell_set);
	FLDset_cell_set_user_name(out_mesh_cell_set, elem_groups[i].elmmat);
      }
    }
  }

  for(j=0; j < out_mesh_nsets; j++) {
     out_mesh_ncells = 0;
     FLDget_cell_set(out_mesh_id, j, &out_mesh_cell_set);
     FLDget_cell_set_user_name(out_mesh_cell_set, label, 21);
     for(i = 0; i < neutral_header.ngrps; i++) {
       if (strncmp(elem_groups[i].elmmat, label, strlen(label)))
	 continue;
       if (strlen(label) != strlen(elem_groups[i].elmmat))
	 continue;
       out_mesh_ncells += elem_groups[i].nelgp;
     }
     FLDset_ncells(out_mesh_cell_set, out_mesh_ncells);
  }

  status = read_nodal_resultsInfo(fp,
				  neutral_header,
				  labels,
                                  &num_labels,
				  &num_timesteps);
  /*
  printf("returned from read_nodal_resultsInfo with status = %d\n",status);
  printf("got num_labels = %d,  num_timesteps = %d\n",
	 num_labels,
	 num_timesteps);
  */

  status = OMset_name_int_val(IVrdFidapResult_id,
			      OMstr_to_name("nresults"),
			      num_labels);

  status = OMset_name_int_val(IVrdFidapResult_id,
			      OMstr_to_name("ntime_steps"),
			      num_timesteps);

  time_control = (int *)OMret_name_array_ptr(IVrdFidapResult_id,
					     OMstr_to_name("time_control"),
					     OM_GET_ARRAY_WR,
					     &time_control_size,
					     NULL);
  time_control[0] = 0;
  time_control[1] = num_timesteps - 1;
  time_control[2] = 1;         /*num_timesteps*/;

  if (num_timesteps) {
    tmp_id = OMfind_subobj(IVrdFidapResult_id,
			   OMstr_to_name("result_labels"),
			   OM_OBJ_RW);

    for (i=0; i < num_labels; i++)
      OMset_str_array_val(tmp_id, i, labels[i]);
  }


  /*************************/
  /*  Free input variables */
  /*************************/
  if (filename)
    free(filename);
  if (elem_groups)
    free(elem_groups);
  for (i=0; i < num_labels; i++)
    if(labels[i])
      free(labels[i]);

  if (time_control)
    ARRfree(time_control);

  fclose(fp);

  return(1);
}


int
rdFidapMesh_func(OMobj_id IVrdFidapResult_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   char  *filename = NULL;
   int  read_mesh;
   OMobj_id out_mesh_id;
   int out_mesh_nspace, out_mesh_nnodes, out_mesh_size;
   int  out_mesh_nsets, out_mesh_set_count, out_mesh_ncells;
   int  out_mesh_cell_nnodes, *out_mesh_node_connect;
   OMobj_id out_mesh_cell_set;
   float *out_mesh_coord;
   int *node_id_table = NULL;

   int			status;
   FILE			*fp;
   Neutral_Header	neutral_header;
   int			fdneut_opened = 0;
   char			line[MAX_LINE];


   /***********************/
   /*  Get input values   */
   /***********************/

   /* Get read_mesh's value */ 
   if (OMget_name_int_val(IVrdFidapResult_id,
                          OMstr_to_name("read_mesh"),
                          &read_mesh) != 1) 
      read_mesh = 0;

   /* Get filename's value */
   if (OMget_name_str_val(IVrdFidapResult_id,
                          OMstr_to_name("filename"),
                          &filename, 0) != 1)
      filename = NULL;


   /***********************/
   /* Function's Body     */
   /***********************/

   fp = open_neutral_file(filename);
   if(fp == NULL)
      return(0);

   fdneut_opened = 1;

   status = read_neutral_header(fp, &neutral_header);
   if(status != 0)
      return(0);


   /***********************/
   /*  Set output values  */
   /***********************/

   /* set  out_mesh mesh */

   /*  Get mesh id */
   out_mesh_id = OMfind_subobj(IVrdFidapResult_id,
                               OMstr_to_name("out_mesh"),
                               OM_OBJ_RW);

   status = read_nodal_coordinates (fp,
				    neutral_header,
				    &node_id_table,
				    out_mesh_id);
   /*
   printf("returned from read_nodal_coordinates with status = %d\n",status);
   */

   if(neutral_header.int_version == 6)
   {
     line[0] = '\0';
     while(strncmp(line, "ELEMENT INFORMATION", 19)) {
       status = read_line(fp, line);
       if(status != 0)
	 return(0);
     }
   }
   else {
     line[0] = '\0';
     while(strncmp(line, "ELEMENT GROUPS", 14)) {
       status = read_line(fp, line);
       if(status != 0)
	 return(0);
     }
   }

   status = read_element_groups(fp,
				neutral_header,
			        node_id_table,
				out_mesh_id);
   /*
   printf("returned from read_element_groups with status = %d\n",status);
   */


   /*************************/
   /*  Free input variables */
   /*************************/
   if (filename)
     free(filename);
   if (node_id_table)
     free (node_id_table);

   fclose(fp);

   return(1);
}


int
rdFidapNdat_func(OMobj_id IVrdFidapResult_id, OMevent_mask event_mask, int seq_num)
{
  /***********************/
  /*  Declare variables  */
  /***********************/
  char  *filename = NULL;
  int  ntime_steps;
  int  read_ndat;
  int  time_control_size = 0;
  int *time_control = NULL; 
  int    time_table_size = 0;
  float *time_table = NULL; 
  int  load_items_size = 0;
  int *load_items = NULL; 
  OMobj_id results_label_id;
  OMobj_id out_ndat_id;
  int out_ndat_ncomp, out_ndat_comp_count, out_ndat_veclen;
  int    out_ndat_data_type, out_ndat_ndata;
  OMobj_id val_id, tmp_id;
  char  *out_ndat_data;
  int out_ndat_dim, max_step, i, j;
  char *label;

  int			status;
  FILE			*fp;
  Neutral_Header	neutral_header;
  int			fdneut_opened = 0;
  char			line[MAX_LINE];


  /***********************/
  /*  Get input values   */
  /***********************/

  /* Get read_ndat's value */ 
  if (OMget_name_int_val(IVrdFidapResult_id,
			 OMstr_to_name("read_ndat"),
			 &read_ndat) != 1) 
    read_ndat = 0;

  /* Get filename's value */
  if (OMget_name_str_val(IVrdFidapResult_id,
			 OMstr_to_name("filename"),
			 &filename, 0) != 1)
    return(0);

  /* Get ntime_steps's value */ 
  if (OMget_name_int_val(IVrdFidapResult_id,
			 OMstr_to_name("ntime_steps"),
			 &ntime_steps) != 1) 
    ntime_steps = 0;

  time_control = (int *)OMret_name_array_ptr(IVrdFidapResult_id,
					     OMstr_to_name("time_control"),
					     OM_GET_ARRAY_RD,
					     &time_control_size,
					     NULL);

  load_items = (int *)OMret_name_array_ptr(IVrdFidapResult_id,
					   OMstr_to_name("load_items"),
					   OM_GET_ARRAY_RD,
					   &load_items_size,
					   NULL);

  results_label_id = OMfind_subobj(IVrdFidapResult_id,
				   OMstr_to_name("result_labels"),
				   OM_OBJ_RD);


  /***********************/
  /* Function's Body     */
  /***********************/

  fp = open_neutral_file(filename);
  if(fp == NULL)
    return(0);

  fdneut_opened = 1;

  status = read_neutral_header(fp, &neutral_header);
  if(status != 0)
    return(0);

  out_ndat_ncomp = load_items_size;

  time_control[0] = 0 > time_control[0] ? 0 : time_control[0];
  time_control[1] = 0 > time_control[1] ? 0 : time_control[1];
  time_control[0] =
    ntime_steps < time_control[0] ? ntime_steps : time_control[0];
  time_control[1] =
    ntime_steps < time_control[1] ? ntime_steps : time_control[1];

  out_ndat_dim = 1 + ((time_control[1]-time_control[0]) / time_control[2]);
  out_ndat_dim = 0 > out_ndat_dim ? 0 : out_ndat_dim;
  /*
  printf("out_ndat_dim = %d\n",out_ndat_dim);
  */

  /***********************/
  /*  Set output values  */
  /***********************/

  tmp_id = OMfind_subobj(IVrdFidapResult_id,
			 OMstr_to_name("time_index"),
			 OM_OBJ_RW);
  if(out_ndat_dim == 0)
    status = OMset_obj_val(tmp_id,OMnull_obj,0);
  else
    status = OMset_int_val(tmp_id,0);

  OMset_name_int_val(IVrdFidapResult_id,
		     OMstr_to_name("out_ndat_dim"),
		     out_ndat_dim);

  /* set  out_ndat node data */

  /* Get field id */
  out_ndat_id = OMfind_subobj(IVrdFidapResult_id,
			      OMstr_to_name("out_ndat_set"),
			      OM_OBJ_RW);

  OMset_array_size(out_ndat_id,
		   out_ndat_dim);

  time_table = (float *)OMret_name_array_ptr(IVrdFidapResult_id,
					     OMstr_to_name("time_table"),
					     OM_GET_ARRAY_RW,
					     &time_table_size,
					     NULL);

  for (j=0; j<time_control[0]; j++)
    status = scan_neutral_file(fp,"TIMESTEP: ",0);

  status = read_string_no_err(fp, line, 10);
  if(status != 0)
    return(-1);
  while(strncmp(line, "TIMESTEP: ", 10)) {
    read_past_newline(fp);
    status = read_string_no_err(fp, line, 10);
    if(status != 0)
      return(-1);
  }

  /*
  printf("now entering timestep loop\n");
  */
  for (i=0; i<out_ndat_dim; i++) {
    /*
    printf("i = %d: ",i);
    */

    status = read_string(fp, line, 11);
    status = read_float(fp, &time_table[i], 20);
    read_past_newline(fp);

    OMget_array_val(out_ndat_id,i,&val_id,OM_OBJ_RW);

    FLDset_nnodes(val_id,neutral_header.numnp);

    /* Set number of node data components */
    FLDset_node_data_ncomp (val_id, out_ndat_ncomp);

    /* For each node data component set veclen, type and data arry itself */
    for (out_ndat_comp_count=0;
	 out_ndat_comp_count < out_ndat_ncomp;
	 out_ndat_comp_count++) {

      j = load_items[out_ndat_comp_count];
      label = NULL;
      OMget_str_array_val(results_label_id,j,&label,0);
      
      out_ndat_veclen = 1;
      if (strcmp(label, "VELOCITY") == 0)
	out_ndat_veclen = neutral_header.ndfvl;

      /* Set veclen, assign out_ndat_veclen before next call */
      FLDset_node_data_comp (val_id,
			     out_ndat_comp_count,
			     out_ndat_veclen,
			     label,
			     "");
      free(label);

      FLDset_node_data_type (val_id,
			     out_ndat_comp_count,
			     DTYPE_FLOAT);

    }

    /*
    printf("filling node data\n");
    */
    status = read_nodal_results(fp,
				neutral_header,
				load_items,
				load_items_size,
				val_id);
    /*
    printf("returned from read_nodal_results with status = %d\n",status);
    */

    for (j=0; j<time_control[2]-1; j++)
      status = scan_neutral_file(fp,"TIMESTEP: ",0);

    if (i < out_ndat_dim-1) {
      status = read_string_no_err(fp, line, 10);
      if(status != 0)
	return(-1);
      while(strncmp(line, "TIMESTEP: ", 10)) {
	read_past_newline(fp);
	status = read_string_no_err(fp, line, 10);
	if(status != 0)
	  return(-1);
      }
    }

  }
   

  /*************************/
  /*  Free input variables */
  /*************************/
  if (filename)
    free(filename);

  if (time_control != NULL) 
    ARRfree((char *)time_control);

  if (time_table != NULL) 
    ARRfree((char *)time_table);

  if (load_items != NULL) 
    ARRfree((char *)load_items);

  fclose(fp);

  return(1);
}







#ifdef _NO_PROTO
FILE *open_neutral_file(filename)
char	*filename;
#else
FILE *open_neutral_file(char *filename)
#endif
{
   FILE		*ret_fp;
   char		new_filename[256];

   /* First just try to open the file as is.				*/
   ret_fp = fopen(filename, "r");
   if(ret_fp == NULL)
   {
      /* If I could not open the file, try adding a FDNEUT extention	*/
      /* before giving up.						*/
      strcpy(new_filename, filename);
      strcat(new_filename, ".FDNEUT");
      ret_fp = fopen(new_filename, "r");
      if(ret_fp == NULL)
      {
         fprintf(stdout, "Error.  Could not find ***%s*** Fidap Neutral File.\n", filename);
         fprintf(stdout, "Check location and protection.   Exiting...\n");
         return(NULL);
      }
   }
   return(ret_fp);
}

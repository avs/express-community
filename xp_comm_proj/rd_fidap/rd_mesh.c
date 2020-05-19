
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#include <avs/om.h>
#include <avs/port.h>
#include <avs/err.h>
#include <avs/gd_def.h>


#include "xp_comm_proj/rd_fidap/fid2FLD.h"

int read_nodal_coordinates(FILE *fp, Neutral_Header neutral_header, 
			   int **node_id_table, OMobj_id out_mesh_id)
{
  char          temp_string[MAX_LINE];
  int		status;
  int		i,node,max_id;

  int out_mesh_nspace, out_mesh_nnodes, out_mesh_size;
  float *out_mesh_coord = NULL;
  float *coord_ptr;
  int *tmp_table = NULL, *iptr;


  /* Read the next line.  It should say "NODAL COORDINATES"		*/
  status = read_line(fp, temp_string);
  if(status != 0)
    return(-1);

  if(strncmp(temp_string, "NODAL COORDINATES", 17) != 0) {
    fprintf(stdout,
            "Error.  Expected \"NODAL COORDINATES\" read ***%s*** instead\n",
	    temp_string);
      /* RJF I could improve this to start from top of file and scan	*/
      /* for NODAL COORDINATES header.					*/
      return(-1);
  }

  tmp_table = malloc(sizeof(int)*neutral_header.numnp);

  /* Set mesh nnodes */
  FLDset_nnodes (out_mesh_id, neutral_header.numnp);

  /* Set mesh nspace */
  FLDset_nspace (out_mesh_id, neutral_header.ndfcd);

  /* Set mesh coordinates */
  status = FLDget_coord(out_mesh_id,
                        &out_mesh_coord,
                        &out_mesh_size,
                        OM_GET_ARRAY_WR);

  coord_ptr = out_mesh_coord;

  for (node=0; node<neutral_header.numnp; node++) {
    status = read_int(fp, &tmp_table[node], 10);
    if(status != 0)
      return(-1);
    for (i=0; i<neutral_header.ndfcd; i++) {
      status = read_float(fp, coord_ptr, 20);
      if(status != 0)
	return(-1);
      coord_ptr++;
    }
    read_past_newline(fp);
  }
  /*
  printf("out_mesh_size = %d,   check = %d\n",
	 out_mesh_size, coord_ptr-out_mesh_coord);
  */
  ARRfree((char *)out_mesh_coord);

  max_id = tmp_table[neutral_header.numnp-1];
  *node_id_table = malloc(sizeof(int)*max_id);
  iptr = *node_id_table;

  for (node=0; node<neutral_header.numnp; node++) {
    i = tmp_table[node] - 1;
    iptr[i] = node;
  }

  free(tmp_table);

  return(0);
}
   


int read_element_groups(FILE *fp, Neutral_Header neutral_header,
			int *node_id_table, OMobj_id out_mesh_id)
{
  int			i, j, k;
  char			temp_string[MAX_LINE];
  int			status;
  int			col_count, ngp, igeom, nftyp, size, ne;
  char		        elmmat[21], label[21];

  int  out_mesh_nsets, out_mesh_set_count, out_mesh_ncells;
  int  out_mesh_cell_nnodes, *out_mesh_node_connect;
  OMobj_id out_mesh_cell_set;
  int *connect_ptr, node;
  int **ptrlist;
  char *cell_names[5] = {"Quad","Tri","Hex","Prism","Tet"};

  /*
  printf("read_element_groups\n");
  */
  FLDget_ncell_sets(out_mesh_id, &out_mesh_nsets);
  /*
  printf("got %d cell sets\n",out_mesh_nsets);
  */

  ptrlist = malloc(out_mesh_nsets*sizeof(int *));
  for (i=0; i<out_mesh_nsets; i++)
    ptrlist[i] = NULL;
  /*
  printf("allocated ptrlist\n");
  */

  /* For each cell set read node_connect_list */
  for(i = 0; i < neutral_header.ngrps; i++) {
    /*
    printf("processing group %d\n",i);
    */

    status = read_string(fp, temp_string, 10);
    if(status != 0)
      return(-1);

    if(strncmp(temp_string, "GROUP:    ", 10) != 0) {
      fprintf(stdout, "Error... Expected \"GROUP:    \" read in ***%s***\n", 
	      temp_string);
      return(-1);
    }

    status = read_int(fp, &ngp, 5);
    if(status != 0)
      return(-1);

    status = read_string(fp, temp_string, 10);
    if(status != 0)
      return(-1);

    if(strncmp(temp_string, " ELEMENTS:", 10) != 0) {
      fprintf(stdout, "Error... Expected \" ELEMENTS:\" read in ***%s***\n", 
	      temp_string);
      return(-1);
    }

    status = read_int(fp, &out_mesh_ncells, 10);
    if(status != 0)
      return(-1);

    status = read_string(fp, temp_string, 10);
    if(status != 0)
      return(-1);

    if(strncmp(temp_string, " NODES:   ", 10) != 0) {
      fprintf(stdout, "Error... Expected \" NODES:   \" read in ***%s***\n",
	      temp_string);
      return(-1);
    }

    status = read_int(fp, &out_mesh_cell_nnodes, 10);
    if(status != 0)
      return(-1);

    status = read_string(fp, temp_string, 10);
    if(status != 0)
      return(-1);

    if(strncmp(temp_string, " GEOMETRY:", 10) != 0) {
      fprintf(stdout, "Error... Expected \" GEOMETRY:\" read in ***%s***\n",
	      temp_string);
      return(-1);
    }

    status = read_int(fp, &igeom, 5);
    if(status != 0)
      return(-1);

    status = read_string(fp, temp_string, 6);
    if(status != 0)
      return(-1);

    if(strncmp(temp_string, " TYPE:", 6) != 0) {
      fprintf(stdout, "Error... Expected \" TYPE:\" read in ***%s***\n",
	      temp_string);
      return(-1);
    }

    status = read_int(fp, &nftyp, 4);
    if(status != 0)
      return(-1);

    read_past_newline(fp);

    status = read_string(fp, temp_string, 15);
    if(status != 0)
      return(-1);

    if(strncmp(temp_string, "ENTITY NAME:   ", 15) != 0) {
      fprintf(stdout, "Error... Expected \" TYPE:\" read in ***%s***\n",
	      temp_string);
      return(-1);
    }

    status = read_string(fp, elmmat, 20);
    if(status != 0)
      return(-1);

    remove_trailing_blanks(elmmat);
    read_past_newline(fp);

    out_mesh_node_connect = NULL;

    if (nftyp <= 5) {
      /*
      printf("group %d: ",i);
      */

      out_mesh_set_count = -1;
      for (j=0; j<out_mesh_nsets; j++) {
	FLDget_cell_set(out_mesh_id, j, &out_mesh_cell_set);
	FLDget_cell_set_user_name(out_mesh_cell_set, label, 21);
	if (strncmp(elmmat, label, strlen(elmmat)))
	  continue;
	if (strlen(elmmat) != strlen(label))
	  continue;
	FLDget_cell_set_name(out_mesh_cell_set, label, 21);
	if (strncmp(cell_names[nftyp-1], label, strlen(label)))
	  continue;
	out_mesh_set_count = j;
	break;
      }
      /*
      printf("got set number %d\n",out_mesh_set_count);
      */

      /* Set node connectivity list */
      FLDget_node_connect(out_mesh_cell_set,
			  &out_mesh_node_connect, 
			  &size,
			  OM_GET_ARRAY_WR);

    }

    connect_ptr = NULL;
    if (out_mesh_node_connect) {
      connect_ptr = ptrlist[out_mesh_set_count];
      if (!connect_ptr)
	connect_ptr = out_mesh_node_connect;
    }
    /*
    printf("size = %d,  check = %d\n",size,connect_ptr-out_mesh_node_connect);
    */

    for(j = 0; j < out_mesh_ncells; j++) {
      col_count = 8;
      status = read_int(fp, &ne, 8);
      if(status != 0)
	return(-1);
      for(k = 0; k < out_mesh_cell_nnodes; k++) {
	col_count = col_count + 8;
	if(col_count > 80) {
	    read_past_newline(fp);
	    status = read_string(fp, temp_string, 8);
	    if(status != 0)
	      return(-1);
	    col_count = 16;
	}
	status = read_int(fp, &node, 8);
	if(status != 0)
	  return(-1);
	if (connect_ptr)
	  *connect_ptr++ = node_id_table[node-1];
      }
      read_past_newline(fp);
    }

    if (connect_ptr)
      ptrlist[out_mesh_set_count] = connect_ptr;

    if (out_mesh_node_connect)
      ARRfree((char *)out_mesh_node_connect);

  }

  if (ptrlist)
    free(ptrlist);

  return(0);
}


#include "gen.h"

#define max_label_size 100
#define max_units_size 100

int
DVdownsize_scat_update(OMobj_id DVdownsize_scat_id, OMevent_mask event_mask, int
seq_num)
{
	/***********************/
	/*	 Declare variables  */
	/***********************/
	OMobj_id in_id;
	int in_nspace, in_nnodes, in_size;
	int  in_nsets, in_set_count, in_ncells;
	int  in_cell_nnodes, *in_node_connect;
	OMobj_id in_cell_set;
	float *in_coord;
	int in_ncomp, in_comp_count, in_veclen;
	int	 in_data_type, in_ndata;
	char	*in_data;
	int  factor;
	OMobj_id out_id;
	int out_nspace, out_nnodes, out_size;
	int  out_nsets, out_set_count, out_ncells;
	int  out_cell_nnodes, *out_node_connect;
	OMobj_id out_cell_set;
	float *out_coord;
	int out_ncomp, out_comp_count, out_veclen;
	int	 out_data_type, out_ndata;
	char	 *out_data;
	float	 *out_float_data;
	short	 *out_short_data;
	int	 *out_int_data;
	double *out_double_data;
	float	 *in_float_data;
	short	 *in_short_data;
	int	 *in_int_data;
	double *in_double_data;
	int node_data_id;
	char out_label[max_label_size];
	char out_units[max_units_size];

	int II;
	int JJ;
	int res;

	/***********************/
	/*	 Get input values	  */
	/***********************/
	/* Get in mesh */

	/* Get mesh id */
	in_id = OMfind_subobj(DVdownsize_scat_id, OMstr_to_name("in"), OM_OBJ_RD);

	/* Get mesh number of nodes */
	FLDget_nnodes (in_id, &in_nnodes);

	/* Get mesh nspace */
	FLDget_nspace (in_id, &in_nspace);

	/* Get mesh coordinates */
	FLDget_coord (in_id, &in_coord, &in_size, OM_GET_ARRAY_RD);

	/* Get_number of cell sets */
	FLDget_ncell_sets(in_id, &in_nsets);

	/* For each cell set get information about cells */
	for (in_set_count=0; in_set_count<in_nsets; in_set_count++) {
		/* Get cell set id */
		FLDget_cell_set(in_id, in_set_count, &in_cell_set);

		/* Get number of cells */
		FLDget_ncells(in_cell_set, &in_ncells);

		/* Get number of nodes in each cell */
		FLDget_cell_set_nnodes(in_cell_set, &in_cell_nnodes);

		/* Get node connectivity list */
		FLDget_node_connect(in_cell_set, &in_node_connect, 
						 &in_size, OM_GET_ARRAY_RD);
		/*
		 * NOTE: for POLY cell_sets such as Polyline, Polytri and Polhedron,
		 *		if you want to get non-tesselated cells 
		 *		use:
		 * FLDget_npolys ()			 instead of			FLDget_ncells ()
		 * FLDget_poly_connect ()	 instead of			FLDget_node_connect ()
		 *
		 *		To check if a cell set is POLY type 
		 *		use:
		 *	 FLDget_poly_flag(in_cell_set, &poly_flag)
		 */
		if (in_node_connect)
			ARRfree((char *)in_node_connect);

	}
		

	/* Get field id */
	in_id = OMfind_subobj(DVdownsize_scat_id, OMstr_to_name("in"), OM_OBJ_RD);

	/* Get number of node data components */
	FLDget_node_data_ncomp (in_id, &in_ncomp);

	/* Get factor's value */
	if (OMget_name_int_val(DVdownsize_scat_id, OMstr_to_name("one_factor"), &factor) != 1)
		factor = 0;


	/***********************/
	/* Function's Body	  */
	/***********************/

	if (factor<1) {
		ERRverror("",ERR_NO_HEADER | ERR_INFO,"Factor < 1\n");
		if (in_coord)
			ARRfree((char *)in_coord);
		return(1);
	}

	out_nnodes = in_nnodes/factor;
	out_nspace = in_nspace;

	/***********************/
	/*	 Set output values  */
	/***********************/
	/* set  out mesh */

	/*	 Get mesh id */
	out_id = OMfind_subobj(DVdownsize_scat_id, OMstr_to_name("out"), OM_OBJ_RW);

	/* Set mesh nnodes */
	FLDset_nnodes (out_id, out_nnodes);

	/* Set mesh nspace */
	FLDset_nspace (out_id, out_nspace);

	/* Set mesh coordinates */
	/* First allocate out_coord array */
	
	out_coord = (float *)ARRalloc(NULL, DTYPE_FLOAT, 
										out_nspace*out_nnodes, NULL);

	/*** Fill in out_coord array with X[,Y,Z] values at each node ***/

	for (II=0;II<out_nnodes;II++)
		for(JJ=0; JJ<out_nspace;JJ++) 
			out_coord[II*out_nspace+JJ] = in_coord[II*factor*out_nspace+JJ];

	FLDset_coord (out_id, out_coord, out_nspace*out_nnodes, 
					  OM_SET_ARRAY_FREE);


/*		ERRverror("",ERR_NO_HEADER | ERR_INFO,"Coordinates Done !\n"); */

	out_nsets = 0;

	/* Set_number of cell sets to 0 initially, the real number of
	   cell_sets should be assigned to out_nsets variable */

	FLDset_ncell_sets(out_id, 0);

	/* For each cell set set information about cells */
	for (out_set_count=0; out_set_count<out_nsets; out_set_count++) {
		/* add cell set, the name can be one of the following:
			Point, Line, Polyline, Tri, Polytri, Polhedron, Tet, Hex,
			Prism, Pyr,
		*/
		FLDadd_cell_set(out_id, "Line");

		/* Get cell set id */
		FLDget_cell_set(out_id, out_set_count, &out_cell_set);

		/* Set number of cells */
		FLDset_ncells(out_cell_set, out_ncells);

		/* Set node connectivity list */
		/* First allocate out_node_connect */

		FLDget_cell_set_nnodes(out_cell_set,  &out_cell_nnodes);
		out_node_connect = (int *)ARRalloc(NULL, DTYPE_INT, 
			 					 out_ncells*out_cell_nnodes, NULL);

		/*** fill in  out_node_connect array with node indecies for each cell ***/

		FLDset_node_connect(out_cell_set, out_node_connect, 
							  out_ncells*out_cell_nnodes, OM_SET_ARRAY_FREE);


		/***	NOTE: for POLY cell_sets such as Polyline, Polytri and Polhedron 
				use:
	FLDset_npolys()			 instead of			FLDset_ncells()
		  FLDset_poly_connect()		instead of		  FLDset_node_connect()
		***/

	}
	
	/* Get/Set node data */

	/* set  out node data */
	out_ncomp = in_ncomp;
	out_veclen = in_veclen;

	/* Get out field id */
	out_id = OMfind_subobj(DVdownsize_scat_id, OMstr_to_name("out"), OM_OBJ_RW);

	/* Set number of node data components */
	FLDset_node_data_ncomp (out_id, out_ncomp);

		/* For each node data component get veclen, type and data arry itself */
	for (in_comp_count=0; in_comp_count < in_ncomp; in_comp_count++) {

		/* Get veclen */
		FLDget_node_data_veclen (in_id, in_comp_count, &in_veclen);

		out_veclen = in_veclen;
		out_comp_count = in_comp_count;
		/* Set veclen, assign out_veclen before next call */
		FLDset_node_data_veclen (out_id,out_comp_count,out_veclen);


		/* Get data array and data_type which is one of the following: 
			DTYPE_BYTE, DTYPE_CHAR, DTYPE_SHORT, 
			DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE */

		FLDget_node_data (in_id, in_comp_count, &in_data_type, &in_data,
								&in_ndata, OM_GET_ARRAY_RD);

		/* Set data array */
		/* data_type should be set to one of the following: 
		  DTYPE_BYTE, DTYPE_CHAR, DTYPE_SHORT, 
			DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE) */

		out_data_type = in_data_type;
		out_ndata = in_ndata/out_veclen/factor;

		/* allocate out_data array first */
		/* assume float array and out_ndata is set to number of nodes */

		out_data = (char *)ARRalloc(NULL, out_data_type, 
											out_veclen*out_ndata, NULL);
	
		out_float_data = (float*) out_data;
		out_int_data = (int*)out_data;
		out_double_data = (double*)out_data;
		out_short_data = (short*)out_data;

		in_float_data = (float*)in_data;
		in_int_data = (int*)in_data;
		in_double_data = (double*)in_data;
		in_short_data = (short*)in_data;

		for (II=0;II<out_ndata;II++) {
			switch (out_data_type) {
				case DTYPE_INT: 
					for (JJ=0;JJ<out_veclen;JJ++)
						out_int_data[II*out_veclen+JJ] = in_int_data[II*factor*out_veclen+JJ];
					break;
				case DTYPE_FLOAT: 
					for (JJ=0;JJ<out_veclen;JJ++)
						out_float_data[II*out_veclen+JJ] = in_float_data[II*factor*out_veclen+JJ];
					break;
				case DTYPE_DOUBLE:	
					for (JJ=0;JJ<out_veclen;JJ++)
						out_double_data[II*out_veclen+JJ] = in_double_data[II*factor*out_veclen+JJ];
					break;
				case DTYPE_SHORT: 
					for (JJ=0;JJ<out_veclen;JJ++)
						out_short_data[II*out_veclen+JJ] = in_short_data[II*factor*out_veclen+JJ]; 
					break;
				default: 
					for (JJ=0;JJ<out_veclen;JJ++)
					out_data[II*out_veclen+JJ] = in_data[II*factor*out_veclen+JJ];
			}
		}

		/* Now out_data is filled */
		FLDset_node_data (out_id, out_comp_count, out_data, out_data_type,
		out_ndata*out_veclen, OM_SET_ARRAY_FREE);


		res = FLDget_node_data_id(out_id,out_comp_count,&node_data_id);
		if (res==1)
		FLDset_node_data_id(out_id,out_comp_count,node_data_id);

		res = FLDget_node_data_label(in_id,out_comp_count,out_label,max_label_size);
		if (res!=1) strcpy(out_label,"NO_NAME!");
		res = FLDget_node_data_units(in_id,out_comp_count,out_units,max_units_size);
		if (res!=1) strcpy(out_units,"NO_UNITS!");
		FLDset_node_data_comp(out_id,out_comp_count,out_veclen,out_label,out_units);
/*				FLDget_node_data_minmax() */

		/*	 Other useful calls:

		*/
		if (in_data)
			ARRfree((char *)in_data);
	}
		

	/*************************/
	/*	 Free input variables */
	/*************************/
	if (in_coord)
		ARRfree((char *)in_coord);

	return(1);
}

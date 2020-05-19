
#include "gen.h"

//#define DEBUG

#ifndef XP_HAS_LONG
#define XP_HAS_LONG
typedef int xp_long;
typedef unsigned int xp_ulong;
#endif
#ifndef XP_HAS_BYTE
#define XP_HAS_BYTE
typedef unsigned char xp_byte;
#endif


#define ERR_RETURN(A) ERRerror("fnc.cxx",1,ERR_ORIG,A); return(1);
#define FAIL -1

int FilterNodeConnect_FilterNodeMods_FilterNodeConnectCore::update(OMevent_mask event_mask, int seq_num)
{
	//Field &in
	//int active
	//Field out
	
	if (!(int)active) return (1);
	
	float *in_coordinates, *out_coordinates;
	
	char *c_node_data_out,*c_node_data_in;
	short *s_node_data_out,*s_node_data_in;
	int *i_node_data_out,*i_node_data_in;
	xp_long *l_node_data_out,*l_node_data_in;
	float *f_node_data_out,*f_node_data_in;
	double *d_node_data_out,*d_node_data_in;
	
	char *out_node_data, *in_node_data;
	
	xp_long *in_node_connect_list, *out_node_connect_list; 
	int data_type, in_data_size, out_data_size, data_comp; 
	xp_long in_nodes, count_new_nodes, n;
	int set,  cell_nodes; 
	xp_long ncells;

	//int set,size,order,cell_nodes,ncells;	
 
	/***********************/
	/* Function's Body	  */
	/***********************/

#ifdef DEBUG
	ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: FilterNodeConnect_FilterNodeMods_filter_node_connect::update\n");
#endif

	in_nodes=(xp_long)in.nnodes;
	// TEMP	allocated array
	xp_long *temp = new xp_long[in_nodes];

 
	//I use temp array so I could ged rid of nodes that are not involved in node_connect_list
	// -1...-1 (27x)
	for (n = 0; n < in_nodes; n++)
	{
		temp[n]=-1;
	}
	// GO THROUGH NODE CONNECT LIST AND CERATE NEW REDUCED IN DATA		
	for (set=0; set < in.ncell_sets; set++) {

		ncells=in.cell_set[set].ncells;
		cell_nodes=in.cell_set[set].cell_nnodes;

		in_node_connect_list = (xp_long*)in.cell_set[set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);

		// ncells*cell_nodes	ncels is no. of cels and cell_nodes is no. of corners
		//START TEMP
		// -10, -1, -1, -10 ... (27x)
		// node_connect[{2}{4}{6}{20}]I set them to -10 

		for (n=0;n<ncells*cell_nodes;n++)
		{	  
			temp[in_node_connect_list[n]]=-10; 
		}	 
		//END TEMP	 
		if (in_node_connect_list)
			ARRfree(in_node_connect_list);
 	}
	count_new_nodes=0;
	for (n = 0; n < in_nodes; n++)
	{
		if (temp[n]==-10)
		{
			temp[n]=count_new_nodes; // used as output index in out_coord	
				count_new_nodes++;
		}
	}


	// START WRITING FILTERED OUTPUT
	out.nspace = in.nspace; 
	out.nnodes = count_new_nodes;
	
	data_type=FAIL;
	out_data_size= count_new_nodes; 
	out.nnode_data= in.nnode_data;

	// COORDINATES IN OUT
	in_coordinates = (float *)in.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
	out_coordinates = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
	//coordinates LOOP
	for (n=0;n<in_nodes;n++)
	{
		if(temp[n]!=-1)
		{
			out_coordinates[temp[n]*in.nspace]=in_coordinates[n*in.nspace]; //x
			out_coordinates[temp[n]*in.nspace+1]=in_coordinates[n*in.nspace+1]; //y
			if(in.nspace!=2)
				out_coordinates[temp[n]*in.nspace+2]=in_coordinates[n*in.nspace+2]; //z
		}
	}

	int vec_length,vec;
	
	// NODE DATA IN OUT 

	for (data_comp = 0; data_comp < (int)in.nnode_data; data_comp++) 
	{ 
		vec_length=(int)in.node_data[data_comp].veclen;
		out.node_data[data_comp].veclen=in.node_data[data_comp].veclen ;
		out.node_data[data_comp].labels=in.node_data[data_comp].labels ;
		out.node_data[data_comp].units=in.node_data[data_comp].units ;

		in_node_data = (char *)in.node_data[data_comp].values.ret_array_ptr(OM_GET_ARRAY_RD, &in_data_size, &data_type);
		if(data_type != FAIL)
		{
			// set pointer to output array
			out_node_data = (char *)out.node_data[data_comp].values.ret_typed_array_ptr(OM_GET_ARRAY_WR,data_type ,&out_data_size);
			switch(data_type)
			{
				 // now cast the pointer to the array to the datatype pointer for both the input and output

				case DTYPE_BYTE || DTYPE_CHAR:
									c_node_data_in=(char *)in_node_data;
									c_node_data_out=(char *)out_node_data;
									break;
				case DTYPE_SHORT: 	s_node_data_in=(short *)in_node_data;
									s_node_data_out=(short *)out_node_data;
									break;
				case DTYPE_INT:		i_node_data_in=(int *)in_node_data;
									i_node_data_out=(int *)out_node_data;
									break;
#ifdef OM_TYPE_LONG
				case DTYPE_LONG:	l_node_data_in=(xp_long *)in_node_data;
									l_node_data_out=(xp_long *)out_node_data;
									break;
#endif
				case DTYPE_FLOAT: 	f_node_data_in=(float *)in_node_data;
									f_node_data_out=(float *)out_node_data;
									break;
				case DTYPE_DOUBLE:	d_node_data_in=(double *)in_node_data;
									d_node_data_out=(double *)out_node_data;
									break;
			}
			//now put a for loop to copy the data across
			switch(data_type)
			{
				case DTYPE_BYTE|| DTYPE_CHAR:
					for(n=0;n<in_nodes;n++)
					{
						if(temp[n]!=-1)
						{
							for(vec=0;vec<vec_length;vec++)
							{	
								c_node_data_out[temp[n]*vec_length+vec]=c_node_data_in[n*vec_length+vec];
							}
						}
					}
					break;
				case DTYPE_SHORT:
					for(n=0;n<in_nodes;n++)
					{
						if(temp[n]!=-1)
						{
							for(vec=0;vec<vec_length;vec++)
							{	
								s_node_data_out[temp[n]*vec_length+vec]=s_node_data_in[n*vec_length+vec];
							}
						}
					}
					break;
				case DTYPE_INT:	
					for(n=0;n<in_nodes;n++)
					{
						if(temp[n]!=-1)
						{
							for(vec=0;vec<vec_length;vec++)
							{	
								i_node_data_out[temp[n]*vec_length+vec]=i_node_data_in[n*vec_length+vec];
							}
						}
					}
					break;
#ifdef OM_TYPE_LONG
				case DTYPE_LONG:	
					for(n=0;n<in_nodes;n++)
					{
						if(temp[n]!=-1)
						{
							for(vec=0;vec<vec_length;vec++)
							{	
								l_node_data_out[temp[n]*vec_length+vec]=l_node_data_in[n*vec_length+vec];
							}
						}
					}
					break;
#endif
				case DTYPE_FLOAT:
					for(n=0;n<in_nodes;n++)
					{
						if(temp[n]!=-1)
						{
							for(vec=0;vec<vec_length;vec++)
							{	
								f_node_data_out[temp[n]*vec_length+vec]=f_node_data_in[n*vec_length+vec];
							}
						}
					}
					break;
				case DTYPE_DOUBLE:
					for(n=0;n<in_nodes;n++)
					{
						if(temp[n]!=-1)
						{
							for(vec=0;vec<vec_length;vec++)
							{	
								d_node_data_out[temp[n]*vec_length+vec]=d_node_data_in[n*vec_length+vec];
							}
						}
					}
					break;
			}
		}

	
		//now free the out node data
		if (in_node_data)
			ARRfree(in_node_data);
		if (out_node_data)
			ARRfree(out_node_data);
	}
	
	// CELL SET IN OUT
 
	out.ncell_sets=in.ncell_sets;	//this must be done before anythin else with sell_sets
	int out_ncell_sets = in.ncell_sets;

	for (set=0; set < out_ncell_sets; set++) {
		ncells=in.cell_set[set].ncells;
		out.cell_set[set].ncells=ncells;
		out.cell_set[set].cell_ndim=in.cell_set[set].cell_ndim;
		cell_nodes=in.cell_set[set].cell_nnodes;
		out.cell_set[set].cell_nnodes=cell_nodes;
		out.cell_set[set].cell_order=in.cell_set[set].cell_order;
		out.cell_set[set].poly_flag=in.cell_set[set].poly_flag;
		in_node_connect_list = (xp_long*)in.cell_set[set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);
		out_node_connect_list = (xp_long*)out.cell_set[set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
		// ncells*cell_nodes	ncels is no. of cels and cell_nodes is no. of corners
		for(n=0;n<ncells*cell_nodes;n++)
		{
			out_node_connect_list[n]=temp[in_node_connect_list[n]];
		}
		if (in_node_connect_list)
			ARRfree(in_node_connect_list);
		if (out_node_connect_list)
			ARRfree(out_node_connect_list);
	}


	if (in_coordinates)
		ARRfree(in_coordinates);
	if (out_coordinates)
	  ARRfree(out_coordinates);

	delete[] temp;
	temp=NULL;
	return(1);
}

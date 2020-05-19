
#include "gen.h"

void* my_realloc(void *ptr, size_t size, size_t oldsize)
{
#ifdef MS_DOS
	/* fix for missing realloc in MS VC6.0 */
	void *new;
	if (new=(void*)malloc(size))
	{
		memcpy(new,ptr,oldsize);
		free(ptr);
		ptr=new;
	}
	return new;
#else
	return realloc(ptr,size);
#endif
}

int
readPolygonCore(OMobj_id ReadPolygonCore_id, OMevent_mask event_mask, int seq_num)
{
	/***********************/
	/*  Declare variables  */
	/***********************/
	char  *filename = NULL;
	OMobj_id out_id;
	int out_nspace, out_nnodes, out_ncells, out_cell_nnodes, *out_node_connect,
		out_veclen, out_data_type, out_ndata, *out_data;
	OMobj_id out_cell_set;
	float *out_coord;

	FILE *file;
	int red, green, blue, i, a, b, c, connections, count,type;
	float x, y, z;
	char current_line[1024];	
	float * temp_coords = (float *)malloc(sizeof(float));
	int * temp_node_data = (int *)malloc(sizeof(int));
	float * temp_coords_ptr;
	int * temp_node_data_ptr;
	long total_vertices = 0, vertices = 0, coord_index;


	/***********************/
	/*  Get input values   */
	/***********************/
	/* Get filename's value */
	if (OMget_name_str_val(ReadPolygonCore_id, OMstr_to_name("filename"), &filename, 0) != 1)
		filename = NULL;

	/* Get id of the out mesh */
	out_id = OMfind_subobj(ReadPolygonCore_id, OMstr_to_name("out"), OM_OBJ_RW);

	/***********************/
	/* Function's Body     */
	/***********************/

	/*Open the file*/
	if ( (file = fopen(filename,"r"))==NULL) {
		ERRverror("",ERR_ERROR,"The file %s could not be opened, or does not exist",filename);
		if (filename)
    		free(filename);
		return(0);
	} else {	
		/*Get the first line of the file*/
		fgets(current_line, 1023, file); 
		FLDset_ncell_sets(out_id, 0);
		count=0;
		while(strcmp(current_line,"\n")!=0) {
			/*Get color*/
			sscanf(current_line, "%i %i %i", &red, &green, &blue);

			/*Get number of co-ordinate vertices*/
			fgets(current_line, 1023, file);
			sscanf(current_line, "%li", &vertices);
			
			/*re-Allocate the arrays to the new size*/
			temp_coords_ptr = (float*)my_realloc(temp_coords, 3*(total_vertices+vertices)*sizeof(float),3*(total_vertices)*sizeof(float));
			if (!temp_coords_ptr) {
				ERRverror("",ERR_ERROR,"Not enough memory could be allocated to complete the operation");
				if (filename)
					free(filename);
				return(0);			
			}
			temp_coords = temp_coords_ptr;
			temp_node_data_ptr = (int*)my_realloc(temp_node_data, 3*(total_vertices+vertices)*sizeof(int), 3*(total_vertices)*sizeof(int));
			if (!temp_node_data) {
				ERRverror("",ERR_ERROR,"Not enough memory could be allocated to complete the operation");
				if (filename)
					free(filename);
				return(0);						
			}
			temp_node_data = temp_node_data_ptr;
			

			for(i=0;i<vertices;i++) {
				fgets(current_line, 1023, file);				
				sscanf(current_line, "%f %f %f", &x, &y, &z);
				coord_index=(total_vertices+i)*3;
				temp_coords[coord_index]=x;
				temp_node_data[coord_index]=red;
				temp_coords[coord_index+1]=y;
				temp_node_data[coord_index+1]=green;
				temp_coords[coord_index+2]=z;
				temp_node_data[coord_index+2]=blue;
			}

			/*Add a new cell set*/
			FLDadd_cell_set(out_id, "Tri");

			/* Get cell set id */
			FLDget_cell_set(out_id, count, &out_cell_set);

			/*Read the total nuber of faces from the file*/
			fgets(current_line, 6, file);
			sscanf(current_line, "%i", &connections);

			/* Set number of cells */
			out_ncells = connections;
			FLDset_ncells(out_cell_set, out_ncells);

			/* Set node connectivity list */
			FLDget_cell_set_nnodes(out_cell_set,  &out_cell_nnodes);
			out_node_connect = (int *)ARRalloc(NULL, DTYPE_INT, 
                    			   out_ncells*out_cell_nnodes, NULL);

			/*** Fill connections array for this cell set ***/
			for(i=0;i<connections;i++) {
				fgets(current_line, 1023, file);
				sscanf(current_line, "%i %i %i %i", &type, &a, &b, &c);

				out_node_connect[3*i] = total_vertices+a-1;
				out_node_connect[3*i+1] = total_vertices+b-1;
				out_node_connect[3*i+2] = total_vertices+c-1;		
			}
			total_vertices += vertices;
			FLDset_node_connect(out_cell_set, out_node_connect, 
                			  out_ncells*out_cell_nnodes, OM_SET_ARRAY_FREE);

			/*Get end of shape char*/		
			fgets(current_line, 3, file);

			/*Get next line (will always be blank)*/
			fgets(current_line, 3, file);
			
			/*Get the next line (blank if end of file else next RGB color)*/
			fgets(current_line, 1023, file); 
			count++;
		}
	}

	/***********************/
	/*  Set output values  */
	/***********************/

	/* Set nnodes (number of nodes) and ndata (number of data componenets)*/
	out_nnodes = total_vertices;
	out_ndata = total_vertices;
	FLDset_nnodes (out_id, out_nnodes);

	/*Set the number of data components*/
	FLDset_node_data_ncomp (out_id, 1);
	
	/* Set nspace (Number of dimensions)*/
	out_nspace = 3;
	FLDset_nspace (out_id, out_nspace);

	/*Set number of components for the data (Red, Green, Blue - 3)*/
	out_veclen=3;
	FLDset_node_data_veclen (out_id,0,out_veclen);

	/*Set type of the data*/
	out_data_type = DTYPE_INT;

	/*Allocate arrays (Co-ordinate and data arrays)*/
	out_coord = (float *)ARRalloc(NULL, DTYPE_FLOAT, 
                        	  out_nspace*out_nnodes, NULL);
	out_data = (int *)ARRalloc(NULL, out_data_type, 
                        	 out_veclen*out_ndata, NULL);

	/*Transfer temporary arrays to final arrays*/
	for (i=0;i<total_vertices*3;i++) {
		out_coord[i]=temp_coords[i];
		out_data[i]=temp_node_data[i];
	}

	/*Set node and data arrays*/
	FLDset_coord (out_id, out_coord, out_nspace*out_nnodes, 
			OM_SET_ARRAY_FREE);

	FLDset_node_data (out_id, 0, (char *)out_data, out_data_type,
        	out_ndata*out_veclen, OM_SET_ARRAY_FREE);

	/*Set the node data ID (note:667 is the ID for RGB color value)*/
	FLDset_node_data_id(out_id, 0,667);


	/*************************/
	/*  Free variables       */
	/*************************/

	fclose(file);
	if (temp_coords)
		free(temp_coords);
	if (temp_node_data)
		free(temp_node_data);
	if (filename)
		free(filename);

	return(1);
}

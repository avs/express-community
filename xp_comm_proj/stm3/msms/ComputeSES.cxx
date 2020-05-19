//
//	Computed the Solvent Excluded Surface with the msms program
//	(see: http://www.scripps.edu/pub/olson-web/people/sanner/html/msms_home.html)
//	The input are a list of atoms coordinates and a list of atoms radii
//	The output is the computed surface.
//
//	The msms executable is pointed by the MSMSSERVER environment variable
//

#include "ComputeSES_gen.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#include <fcntl.h>
int mkstemp(char *s)
{
      char *s1 = mktemp(s);
      return open(s1, _O_CREAT | _O_EXCL | _O_WRONLY);
}
#endif
#include <avs/gd_def.h>

#define STARTING_SIZE  1024
#define INCREMENT_SIZE 1024
#define LINE_LENGHT    1024

#include "../lib/stm3.h"
#include "../base/mol_type.h"

/*
**	Windows version of system() library call
**	Does not create a DOS window.
**	Works only if the first token in cmd is an executable file
**	and not a DOS internal command (like DIR) and does not accepts
**	redirections.
*/
#ifdef WIN32

#include <stdio.h>
#include <string.h>
#include <process.h>
#define MAX_ARGS	20

int system(char *cmd)
{
	char *token, *p;
	int ntokens;
	char seps[] = " \t\n";
	char *argv[MAX_ARGS+1];
	int i;

	/*
	**	Check if the path is quoted to mask blanks
	*/
	if(cmd[0] == '\"')
	{
		for(i=1; cmd[i] != '\"' && cmd[i]; i++);
		p = (cmd[i] == '\"') ? cmd + i + 1 : cmd + i;
		cmd[i] = '\0';
		argv[0] = cmd+1;
		ntokens = 1;
	}
	else
	{
		p = cmd;
		ntokens = 0;
	}

	/*
	**	Tokenize the non quoted part of the string
	*/
	token = strtok(p, seps);
	while(token != NULL)
	{
		if(ntokens >= MAX_ARGS) return -1;
		argv[ntokens] = token;
		ntokens++;

		token = strtok(NULL, seps);
	}
	argv[ntokens] = NULL;

	/*
	**	Execute the command
	*/
	_flushall();
	return _spawnvp(_P_WAIT, argv[0], argv);
}
#endif

int
STM3_MSMS_ComputeSES::ComputeSES(OMevent_mask event_mask, int seq_num)
{
   // xyz_lst (OMXfloat_array read req notify)
   int xyz_lst_size;
   float *xyz_lst_arr;
   // color (OMXint read notify)
   // fld (Field write)
	// filename (OMXstr read req notify)
	// fld (Mesh write)
	char line[LINE_LENGHT];
	FILE *fp;
	int i;

	// reset the status message
	status = "";
	
	// If only the kind of coloring ha changed, don't reexecute MSMS
	if(color.changed(seq_num) && last_atoms_idx.ret_array_size() > 0)
	{
		// access the saved data
		int   *last_atoms_idx_arr = (int *)last_atoms_idx.ret_array_ptr(OM_GET_ARRAY_RD);
		float *last_normals_arr = (float *)last_normals.ret_array_ptr(OM_GET_ARRAY_RD);
		int n_points = last_atoms_idx.ret_array_size();
	
		if(last_atoms_idx_arr && last_normals_arr)
		{
			// if requested color the vertex by the closest sphere number
			if((int)color == ATOM_TYPE)
			{
				fld.nnode_data = 2;
				fld.node_data[0].id = GD_COLOR_DATA_ID;
				fld.node_data[0].veclen = 3;
				fld.node_data[0].labels = "Nearest atom color";
				float *fld_data1 = (float *)fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
				if(!fld_data1)
				{
					ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
					ARRfree(last_atoms_idx_arr);
					ARRfree(last_normals_arr);
					return 0;
				}
   				int *atom_z_arr = (int *)atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
   				if(!atom_z_arr)
				{
					ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing atom_z array.");
					ARRfree(last_atoms_idx_arr);
					ARRfree(last_normals_arr);
					ARRfree(fld_data1);
					return 0;
				}
				for(i=0; i < n_points; ++i)
				{
					fld_data1[3*i+0] = atom_properties[atom_z_arr[last_atoms_idx_arr[i]]].color[0];
					fld_data1[3*i+1] = atom_properties[atom_z_arr[last_atoms_idx_arr[i]]].color[1];
					fld_data1[3*i+2] = atom_properties[atom_z_arr[last_atoms_idx_arr[i]]].color[2];
				}
				ARRfree(fld_data1);
				ARRfree(atom_z_arr);

				fld.node_data[1].veclen = 3;
				fld.node_data[1].labels = "Normals";
				fld.node_data[1].id = GD_NORMAL_DATA_ID;
				float *fld_data0 = (float *)fld.node_data[1].values.set_array(DTYPE_FLOAT, last_normals_arr, n_points*3, OM_SET_ARRAY_COPY);
				if(!fld_data0)
				{
					ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
					ARRfree(last_atoms_idx_arr);
					ARRfree(last_normals_arr);
					return 0;
				}
			}
			else if((int)color == ATOM_CHARGE)
			{
				fld.nnode_data = 2;
				fld.node_data[0].id.set_obj_ref(OMnull_obj, 0);
				fld.node_data[0].veclen = 1;
				fld.node_data[0].labels = "Charge";
				float *fld_data1 = (float *)fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
				if(!fld_data1)
				{
					ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
					ARRfree(last_atoms_idx_arr);
					ARRfree(last_normals_arr);
					return 0;
				}
				float *charge_arr = (float *)charge.ret_array_ptr(OM_GET_ARRAY_RD);
				if(!charge_arr)
				{
					ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing charge array.");
					ARRfree(last_atoms_idx_arr);
					ARRfree(last_normals_arr);
					ARRfree(fld_data1);
					return 0;
				}
				for(i=0; i < n_points; ++i)
				{
					fld_data1[i] = charge_arr[last_atoms_idx_arr[i]];
				}
				ARRfree(fld_data1);
				ARRfree(charge_arr);

				fld.node_data[1].veclen = 3;
				fld.node_data[1].labels = "Normals";
				fld.node_data[1].id = GD_NORMAL_DATA_ID;
				float *fld_data0 = (float *)fld.node_data[1].values.set_array(DTYPE_FLOAT, last_normals_arr, n_points*3, OM_SET_ARRAY_COPY);
				if(!fld_data0)
				{
					ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
					ARRfree(last_atoms_idx_arr);
					ARRfree(last_normals_arr);
					return 0;
				}
			}
			else if((int)color == ATOM_Z)
			{
				fld.nnode_data = 2;
				fld.node_data[0].id.set_obj_ref(OMnull_obj, 0);
				fld.node_data[0].veclen = 1;
				fld.node_data[0].labels = "Z";
				float *fld_data1 = (float *)fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
				if(!fld_data1)
				{
					ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
					ARRfree(last_atoms_idx_arr);
					ARRfree(last_normals_arr);
					return 0;
				}
   				int *atom_z_arr = (int *)atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
   				if(!atom_z_arr)
				{
					ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing atom_z array.");
					ARRfree(last_atoms_idx_arr);
					ARRfree(last_normals_arr);
					ARRfree(fld_data1);
					return 0;
				}
				for(i=0; i < n_points; ++i)
				{
					fld_data1[i] = atom_z_arr[last_atoms_idx_arr[i]];
				}
				ARRfree(fld_data1);
				ARRfree(atom_z_arr);

				fld.node_data[1].veclen = 3;
				fld.node_data[1].labels = "Normals";
				fld.node_data[1].id = GD_NORMAL_DATA_ID;
				float *fld_data0 = (float *)fld.node_data[1].values.set_array(DTYPE_FLOAT, last_normals_arr, n_points*3, OM_SET_ARRAY_COPY);
				if(!fld_data0)
				{
					ARRfree(last_atoms_idx_arr);
					ARRfree(last_normals_arr);
					ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
					return 0;
				}
			}
			else
			{
				fld.nnode_data = 1;
				fld.node_data[0].veclen = 3;
				fld.node_data[0].labels = "Normals";
				fld.node_data[0].id = GD_NORMAL_DATA_ID;
				float *fld_data0 = (float *)fld.node_data[0].values.set_array(DTYPE_FLOAT, last_normals_arr, n_points*3, OM_SET_ARRAY_COPY);
				if(!fld_data0)
				{
					ARRfree(last_atoms_idx_arr);
					ARRfree(last_normals_arr);
					ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
					return 0;
				}
			}

			ARRfree(last_atoms_idx_arr);
			ARRfree(last_normals_arr);
			return 1;
		}
		else
		{
			if(last_atoms_idx_arr) ARRfree(last_atoms_idx_arr);
			if(last_normals_arr) ARRfree(last_normals_arr);
		}
	}
	
	// read the input data
    xyz_lst_arr = (float *)xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD, &xyz_lst_size);
    if(!xyz_lst_arr)
	{
      	ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error reading XYZ input.");
		return 0;
    }
	if(xyz_lst_size < 3)
	{
		ARRfree(xyz_lst_arr);
		return 0;
	}

   	int *atom_z_arr = (int *)atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
   	if(!atom_z_arr)
	{
      	ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error reading Atom Z input.");
      	ARRfree(xyz_lst_arr);
		return 0;
   	}
		
	// build the input file
	char input_file[1024];
	char *t = getenv("TMP");
	if(!t) t = getenv("TEMP");
	if(!t) t = "/tmp";
	sprintf(input_file, "%s/stm3msmsXXXXXX", t);
	int fd = mkstemp(input_file);
	if(fd < 0)
	{
		ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error creating input file.");
		return 0;
	}
	fp = fdopen(fd, "w");
	
	for(i=0; i < xyz_lst_size/3; ++i)
	{
		float radius = atom_properties[atom_z_arr[i]].rvdw;
		fprintf(fp, "%f %f %f %f\n", xyz_lst_arr[i*3+0], xyz_lst_arr[i*3+1], xyz_lst_arr[i*3+2], radius);
	}
	
	fclose(fp);
    ARRfree(xyz_lst_arr);
	ARRfree(atom_z_arr);

	// invoke the MSMS command
	char *msms_cmd = getenv("MSMSSERVER");
	if(!msms_cmd)
	{
		remove(input_file);
		ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Cannot execute msms. Environment variable MSMSSERVER not defined");
		status = "MSMSSERVER not defined";
		return 0;
	}
	sprintf(line, "%s -if %s -of %s -no_header -probe_radius %f -density %f", msms_cmd, input_file, input_file, (float)probe_radius, (float)density);

	printf("\n\n");
	int sts = system(line);
	remove(input_file);
	if(sts)
	{
		ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Cannot execute msms. Status returned %d", sts);
		status = "Cannot execute msms";
		return 0;
	}

	// prepare the output filenames
	int len = strlen(input_file);
	char *vert_filename = new char[len+6];
	char *face_filename = new char[len+6];
	strcpy(vert_filename, input_file);
	strcpy(face_filename, input_file);
	strcpy(vert_filename+len, ".vert");
	strcpy(face_filename+len, ".face");

	// open the vertex coordinates file
	fp = fopen(vert_filename, "r");
	if(fp == NULL)
	{
		ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Cannot open %s", vert_filename);
   		delete [] vert_filename;
   		delete [] face_filename;
		return 0;
	}
	
   	// initialize the extensible coordinates array
	int allocated_points = STARTING_SIZE;
	int n_points = 0;
 
	float *coords_buffer = (float *) ARRalloc(NULL, DTYPE_FLOAT, allocated_points*3, NULL); 
	if(coords_buffer == NULL)
	{
		ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error allocating coords array.");
   		delete [] vert_filename;
   		delete [] face_filename;
		fclose(fp);
		return 0;
	}

	// initialize the vertex normals array
	float *normals_buffer = (float *) ARRalloc(NULL, DTYPE_FLOAT, allocated_points*3, NULL); 
	if(normals_buffer == NULL)
	{
		ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error allocating normals array.");
   		delete [] vert_filename;
   		delete [] face_filename;
		ARRfree(coords_buffer);
		fclose(fp);
		return 0;
	}
	
	// initialize the atom names
	int *atoms_buffer = (int *) ARRalloc(NULL, DTYPE_INT, allocated_points, NULL); 
	if(atoms_buffer == NULL)
	{
		ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error allocating atoms array.");
   		delete [] vert_filename;
   		delete [] face_filename;
		ARRfree(coords_buffer);
		fclose(fp);
		return 0;
	}

	// read the vert file
	while(fgets(line, sizeof(line)-2, fp))
	{
		if(n_points >= allocated_points)
		{
			allocated_points += INCREMENT_SIZE;
			coords_buffer = (float *)ARRrealloc(coords_buffer, DTYPE_FLOAT, allocated_points*3, NULL); 
			if(coords_buffer == NULL)
			{
				ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error reallocating coords array.");
   				delete [] vert_filename;
   				delete [] face_filename;
				fclose(fp);
				return 0; 
			}
			normals_buffer = (float *)ARRrealloc(normals_buffer, DTYPE_FLOAT, allocated_points*3, NULL); 
			if(normals_buffer == NULL)
			{
				ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error reallocating normals array.");
   				delete [] vert_filename;
   				delete [] face_filename;
				ARRfree(coords_buffer);
				fclose(fp);
				return 0;
			}
			atoms_buffer = (int *)ARRrealloc(atoms_buffer, DTYPE_INT, allocated_points, NULL); 
			if(atoms_buffer == NULL)
			{
				ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error reallocating atoms array.");
   				delete [] vert_filename;
   				delete [] face_filename;
				ARRfree(coords_buffer);
				fclose(fp);
				return 0;
			}
		}

		int dummy, atom;
		sscanf(line, "%f%f%f%f%f%f%d%d", &coords_buffer[n_points*3+0], &coords_buffer[n_points*3+1], &coords_buffer[n_points*3+2],
		                             &normals_buffer[n_points*3+0], &normals_buffer[n_points*3+1], &normals_buffer[n_points*3+2],
									 &dummy, &atom);
		atoms_buffer[n_points] = atom - 1;
		
		++n_points;
	}
	fclose(fp);
	
	// Save the atom indices
	last_atoms_idx.set_array_size(n_points);
	int *last_atoms_idx_arr = (int *)last_atoms_idx.ret_array_ptr(OM_GET_ARRAY_WR);
	if(last_atoms_idx_arr)
	{
		memcpy(last_atoms_idx_arr, atoms_buffer, n_points*sizeof(int));
		ARRfree(last_atoms_idx_arr);
	}
	else
	{
		last_atoms_idx.set_array_size(0);
	}
	
	// Save the normals
	last_normals.set_array_size(n_points*3);
	float *last_normals_arr = (float *)last_normals.ret_array_ptr(OM_GET_ARRAY_WR);
	if(last_normals_arr)
	{
		memcpy(last_normals_arr, normals_buffer, 3*n_points*sizeof(float));
		ARRfree(last_normals_arr);
	}
	
	// set nnodes
	fld.nnodes = n_points;
	
	float *fld_coordinates = (float *)fld.coordinates.values.set_array(DTYPE_FLOAT, coords_buffer, n_points*3, OM_SET_ARRAY_FREE);
	if(!fld_coordinates)
	{
		ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing coords array.");
   		delete [] vert_filename;
   		delete [] face_filename;
		ARRfree(coords_buffer);
		ARRfree(normals_buffer);
		return 0;
	}

	// if requested color the vertex by the closest sphere number
	if((int)color == ATOM_TYPE)
	{
		fld.nnode_data = 2;
		fld.node_data[0].id = GD_COLOR_DATA_ID;
		fld.node_data[0].veclen = 3;
		fld.node_data[0].labels = "Nearest atom color";
		float *fld_data1 = (float *)fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!fld_data1)
		{
			ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
   			delete [] vert_filename;
   			delete [] face_filename;
			ARRfree(coords_buffer);
			ARRfree(normals_buffer);
			ARRfree(atoms_buffer);
			return 0;
		}
   		int *atom_z_arr = (int *)atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
   		if(!atom_z_arr)
		{
			ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing atom_z array.");
   			delete [] vert_filename;
   			delete [] face_filename;
			ARRfree(coords_buffer);
			ARRfree(normals_buffer);
			ARRfree(atoms_buffer);
			ARRfree(fld_data1);
			return 0;
		}
		for(i=0; i < n_points; ++i)
		{
			fld_data1[3*i+0] = atom_properties[atom_z_arr[atoms_buffer[i]]].color[0];
			fld_data1[3*i+1] = atom_properties[atom_z_arr[atoms_buffer[i]]].color[1];
			fld_data1[3*i+2] = atom_properties[atom_z_arr[atoms_buffer[i]]].color[2];
		}
		ARRfree(fld_data1);
		ARRfree(atom_z_arr);

		fld.node_data[1].veclen = 3;
		fld.node_data[1].labels = "Normals";
		fld.node_data[1].id = GD_NORMAL_DATA_ID;
		float *fld_data0 = (float *)fld.node_data[1].values.set_array(DTYPE_FLOAT, last_normals_arr, n_points*3, OM_SET_ARRAY_COPY);
		if(!fld_data0)
		{
			ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
   			delete [] vert_filename;
   			delete [] face_filename;
			ARRfree(coords_buffer);
			ARRfree(normals_buffer);
			ARRfree(atoms_buffer);
			return 0;
		}

	}
	else if((int)color == ATOM_CHARGE)
	{
		fld.nnode_data = 2;
		fld.node_data[0].id.set_obj_ref(OMnull_obj, 0);
		fld.node_data[0].veclen = 1;
		fld.node_data[0].labels = "Charge";
		float *fld_data1 = (float *)fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!fld_data1)
		{
			ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
   			delete [] vert_filename;
   			delete [] face_filename;
			ARRfree(coords_buffer);
			ARRfree(normals_buffer);
			ARRfree(atoms_buffer);
			return 0;
		}
		float *charge_arr = (float *)charge.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!charge_arr)
		{
			ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing charge array.");
   			delete [] vert_filename;
   			delete [] face_filename;
			ARRfree(coords_buffer);
			ARRfree(normals_buffer);
			ARRfree(fld_data1);
			ARRfree(atoms_buffer);
			return 0;
		}
		for(int i=0; i < n_points; ++i)
		{
			fld_data1[i] = charge_arr[atoms_buffer[i]];
		}
		ARRfree(atoms_buffer);
		ARRfree(fld_data1);
		ARRfree(charge_arr);
		
		fld.node_data[1].veclen = 3;
		fld.node_data[1].labels = "Normals";
		fld.node_data[1].id = GD_NORMAL_DATA_ID;
		float *fld_data0 = (float *)fld.node_data[1].values.set_array(DTYPE_FLOAT, normals_buffer, n_points*3, OM_SET_ARRAY_FREE);
		if(!fld_data0)
		{
			ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
   			delete [] vert_filename;
   			delete [] face_filename;
			ARRfree(coords_buffer);
			ARRfree(normals_buffer);
			return 0;
		}
	}
	else if((int)color == ATOM_Z)
	{
		fld.nnode_data = 2;
		fld.node_data[0].id.set_obj_ref(OMnull_obj, 0);
		fld.node_data[0].veclen = 1;
		fld.node_data[0].labels = "Z";
		float *fld_data1 = (float *)fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!fld_data1)
		{
			ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
   			delete [] vert_filename;
   			delete [] face_filename;
			ARRfree(coords_buffer);
			ARRfree(normals_buffer);
			ARRfree(atoms_buffer);
			return 0;
		}
   		atom_z_arr = (int *)atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
   		if(!atom_z_arr)
		{
			ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing atom_z array.");
   			delete [] vert_filename;
   			delete [] face_filename;
			ARRfree(coords_buffer);
			ARRfree(normals_buffer);
			ARRfree(fld_data1);
			ARRfree(atoms_buffer);
			return 0;
		}
		for(int i=0; i < n_points; ++i)
		{
			fld_data1[i] = atom_z_arr[atoms_buffer[i]];
		}
		ARRfree(atoms_buffer);
		ARRfree(fld_data1);
		ARRfree(atom_z_arr);
		
		fld.node_data[1].veclen = 3;
		fld.node_data[1].labels = "Normals";
		fld.node_data[1].id = GD_NORMAL_DATA_ID;
		float *fld_data0 = (float *)fld.node_data[1].values.set_array(DTYPE_FLOAT, normals_buffer, n_points*3, OM_SET_ARRAY_FREE);
		if(!fld_data0)
		{
			ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
   			delete [] vert_filename;
   			delete [] face_filename;
			ARRfree(coords_buffer);
			ARRfree(normals_buffer);
			return 0;
		}
	}
	else
	{
		fld.nnode_data = 1;
		fld.node_data[0].veclen = 3;
		fld.node_data[0].labels = "Normals";
		fld.node_data[0].id = GD_NORMAL_DATA_ID;
		float *fld_data0 = (float *)fld.node_data[0].values.set_array(DTYPE_FLOAT, normals_buffer, n_points*3, OM_SET_ARRAY_FREE);
		if(!fld_data0)
		{
			ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing normals array.");
   			delete [] vert_filename;
   			delete [] face_filename;
			ARRfree(coords_buffer);
			ARRfree(normals_buffer);
			return 0;
		}
		ARRfree(atoms_buffer);
	}

	// open the face connectivity file
	fp = fopen(face_filename, "r");
	if(fp == NULL)
	{
		ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Cannot open %s", face_filename);
   		delete [] vert_filename;
   		delete [] face_filename;
		return 0;
	}

	// initialize the extensible connectivity array
	int *conn_buffer;
	int allocated_cells = STARTING_SIZE;
	int n_cells = 0;
 
	conn_buffer = (int *) ARRalloc(NULL, DTYPE_INT, allocated_cells*3, NULL); 
	if(conn_buffer == NULL)
	{
		ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error allocating conn array.");
   		delete [] vert_filename;
   		delete [] face_filename;
		fclose(fp);
		return 0;
	}
	
	while(fgets(line, sizeof(line)-2, fp))
	{
		if(n_cells >= allocated_cells)
		{
			allocated_cells += INCREMENT_SIZE;
			conn_buffer = (int *)ARRrealloc(conn_buffer, DTYPE_INT, allocated_cells*3, NULL); 
			if(conn_buffer == NULL)
			{
				ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error reallocating conn array.");
   				delete [] vert_filename;
   				delete [] face_filename;
				fclose(fp);
				return 0;
			}
		}
		sscanf(line, "%d%d%d", &conn_buffer[n_cells*3+0], &conn_buffer[n_cells*3+1], &conn_buffer[n_cells*3+2]);
		--conn_buffer[n_cells*3+0];
		--conn_buffer[n_cells*3+1];
		--conn_buffer[n_cells*3+2];
		
		++n_cells;
	}
	fclose(fp);
	
	fld.cell_set[0].ncells = n_cells;
   	int *fld_node_connect_list = (int *)fld.cell_set[0].node_connect_list.set_array(DTYPE_INT, conn_buffer, n_cells*3, OM_SET_ARRAY_FREE);
	if(!fld_node_connect_list)
	{
		ERRverror("ReadMSMS", ERR_NO_HEADER | ERR_ERROR, "Error accessing conn array.");
   		delete [] vert_filename;
   		delete [] face_filename;
		ARRfree(conn_buffer);
		return 0;
	}

   	// return 1 for success
	remove(vert_filename);
	remove(face_filename);
   	delete [] vert_filename;
   	delete [] face_filename;
   	return 1;
}

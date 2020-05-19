//
//
//	Crop Box for unstructured fields
//	Crop the field (1D, 2D or 3D) inside the entered coordinates.
//	If a coordinate is unspecified, no crop will happens along this side
//
//	Limitations:
//		It works only with unstructured fields
//		It works only with non polygonal cell sets
//
//	Author:		Mario Valle - AVS Italy
//	Date:		15-JAN-2002
//	Version:	1.0
//
//
#include "gen.h"
#include "cropbox.h"
#include <memory.h>

// Max size of data labels and units
#define TXT_SIZE	200

int
Crop3D_Crop3DCore::Crop3D(OMevent_mask event_mask, int seq_num)
{
	int sts = 0;
	float _xmin, _xmax;
	float _ymin, _ymax;
	float _zmin, _zmax;
	char txt[TXT_SIZE+1];
	char txt2[TXT_SIZE+1];
	int in_data_size, in_data_type, id, nprops;

	//
	//	Pass values to the output mesh
	//
	out.nspace     = in.nspace;
	out.ncell_sets = 0;
	out.nnode_data = in.nnode_data;
	out.nnodes     = 0;

	//
	//	At least one point needed
	//
	if(!in.nnodes) return 1;

	//
	//	Access coordinates and field bounding box
	//
	float *in_coordinates = (float *)in.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!in_coordinates) return 0;

	float *minv = (float *)in.coordinates.min_vec.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!minv)
	{
		ARRfree(in_coordinates);
		return 0;
	}
	float *maxv = (float *)in.coordinates.max_vec.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!maxv)
	{
		ARRfree(in_coordinates);
		ARRfree(minv);
		return 0;
	}

	//
	//	Check coordinates against the defined box
	//
	CropBox box;
	switch(in.nspace)
	{
	case 1:
		_xmin = xmin.valid_obj() ? xmin : minv[0];
		_xmax = xmax.valid_obj() ? xmax : maxv[0];
		if(_xmax < _xmin) break;
		if(_xmax < minv[0]) break;

		out.nnodes = box.CheckCoords1D(in_coordinates, in.nnodes, _xmin, _xmax);
		sts = 1;
		break;

	case 2:
		_xmin = xmin.valid_obj() ? xmin : minv[0];
		_xmax = xmax.valid_obj() ? xmax : maxv[0];
		_ymin = ymin.valid_obj() ? ymin : minv[1];
		_ymax = ymax.valid_obj() ? ymax : maxv[1];
		if(_xmax < _xmin) break;
		if(_xmax < minv[0]) break;
		if(_ymax < _ymin) break;
		if(_ymax < minv[1]) break;

		out.nnodes = box.CheckCoords2D(in_coordinates, in.nnodes, _xmin, _xmax, _ymin, _ymax);
		sts = 1;
		break;

	case 3:
		_xmin = xmin.valid_obj() ? xmin : minv[0];
		_xmax = xmax.valid_obj() ? xmax : maxv[0];
		_ymin = ymin.valid_obj() ? ymin : minv[1];
		_ymax = ymax.valid_obj() ? ymax : maxv[1];
		_zmin = zmin.valid_obj() ? zmin : minv[2];
		_zmax = zmax.valid_obj() ? zmax : maxv[2];
		if(_xmax < _xmin) break;
		if(_xmax < minv[0]) break;
		if(_ymax < _ymin) break;
		if(_ymax < minv[1]) break;
		if(_zmax < _zmin) break;
		if(_zmax < minv[2]) break;

		out.nnodes = box.CheckCoords3D(in_coordinates, in.nnodes, _xmin, _xmax, _ymin, _ymax, _zmin, _zmax);
		sts = 1;
		break;

	default:
		ERRverror("Crop3D",ERR_ERROR,"Unsupported nspace %d", in.nspace);
		break;
	};
	
	//	Min/max not needed anymore
	ARRfree(minv);
	ARRfree(maxv);

	//	No points selected (or error detected)
	if(!out.nnodes)
	{
		ARRfree(in_coordinates);
		return sts;
	}

	//
	//	Copy coordinates of points inside the crop box
	//
	float *out_coordinates = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!out_coordinates)
	{
		ERRverror("Crop3D",ERR_ERROR,"Cannot create output coordinates");
		return 0;
	}
	box.ValidPoints(in_coordinates, out_coordinates);

	ARRfree(out_coordinates);
	ARRfree(in_coordinates);

	//
	//	Copy node data
	//
	for(int in_data_comp = 0; in_data_comp < in.nnode_data; in_data_comp++)
	{ 
		char *in_node_data = (char *)in.node_data[in_data_comp].values.ret_array_ptr(OM_GET_ARRAY_RD, &in_data_size, &in_data_type);
        if(!in_node_data)
		{
			ERRverror("Crop3D",ERR_ERROR,"Cannot access input node data %d", in_data_comp);
			return 0;
		}

		// Set the characteristics of output node data
		FLDset_node_data_type(out.obj_id(OM_OBJ_RW), in_data_comp, in_data_type);
		if(FLDget_node_data_label(in.obj_id(OM_OBJ_RW), in_data_comp, txt,  TXT_SIZE) != 1) txt[0]  = '\0';
		if(FLDget_node_data_units(in.obj_id(OM_OBJ_RW), in_data_comp, txt2, TXT_SIZE) != 1) txt2[0] = '\0';
		FLDset_node_data_comp(out.obj_id(OM_OBJ_RW), in_data_comp, in.node_data[in_data_comp].veclen, txt, txt2);
		if(FLDget_node_data_id(in.obj_id(OM_OBJ_RW), in_data_comp, &id) == 1)
			FLDset_node_data_id(out.obj_id(OM_OBJ_RW), in_data_comp, id);
		int node_null_flag;
		double node_null_value; // large enough to hold any prim
        if(FLDget_node_null_data(in.obj_id(OM_OBJ_RW), in_data_comp, &node_null_flag, (char *)&node_null_value) == 1)
		{
           FLDset_node_null_flag(out.obj_id(OM_OBJ_RW), in_data_comp, node_null_flag);
           if(node_null_flag)
		   {
              FLDset_node_null_data(out.obj_id(OM_OBJ_RW), in_data_comp, (char *)&node_null_value, in_data_type);
           }
        }

		char *out_node_data = (char *)out.node_data[in_data_comp].values.ret_array_ptr(OM_GET_ARRAY_WR);
        if(!out_node_data)
		{
			ERRverror("Crop3D",ERR_ERROR,"Cannot create output node data %d", in_data_comp);
			ARRfree(in_node_data);
			return 0;
		}

		box.CopyNodeData(in_node_data, out_node_data, in.node_data[in_data_comp].veclen, in_data_type);

		ARRfree(out_node_data);
		ARRfree(in_node_data);
	}

	//
	//	For each cell set
	//
	int out_set = -1;
	for(int in_set=0; in_set < in.ncell_sets; in_set++) 
	{	
		int *in_node_connect_list = (int *)in.cell_set[in_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!in_node_connect_list)
		{
			ERRverror("Crop3D",ERR_ERROR,"Cannot read connection list from cell set %d", in_set);
			return 0;
		}

		// Prepare a list of all the cells with all the vertex included in the box
		int ncells = box.PrepareCells(in_node_connect_list, in.cell_set[in_set].ncells, in.cell_set[in_set].cell_nnodes);
		ARRfree(in_node_connect_list);

		// If at least one cell, create the cell set
		if(ncells < 1) continue;

		// Create the output cell set
		FLDget_cell_set_name(in.cell_set[in_set].obj_id(OM_OBJ_RW), txt, TXT_SIZE);
		FLDadd_cell_set(out.obj_id(OM_OBJ_RW), txt);
		out_set++;

		// Set cell set name and number of cells
		if(FLDget_cell_set_user_name(in.cell_set[in_set].obj_id(OM_OBJ_RW), txt, TXT_SIZE) != 1) txt[0] = '\0';
		FLDset_cell_set_user_name(out.cell_set[out_set].obj_id(OM_OBJ_RW), txt);
		out.cell_set[out_set].ncells = ncells;
		if(FLDget_cell_nprops(in.cell_set[in_set].obj_id(OM_OBJ_RW), &nprops) == 1)
		{
			FLDset_cell_nprops(out.cell_set[out_set].obj_id(OM_OBJ_RW), nprops);
			if(nprops)
			{
				int size;
				float *in_props, *out_props;
				FLDget_cell_props(in.cell_set[in_set].obj_id(OM_OBJ_RW), &in_props, &size, OM_GET_ARRAY_RD);
				if(in_props)
				{
					FLDget_cell_props(out.cell_set[out_set].obj_id(OM_OBJ_RW), &out_props, &size, OM_GET_ARRAY_WR);
					if(out_props)
					{
						memcpy(out_props, in_props, size * sizeof(float));
						ARRfree(out_props);
					}
					ARRfree(in_props);
				}
			}
		}

		// Copy the connectivity list
		int *out_node_connect_list = (int *)out.cell_set[out_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!out_node_connect_list)
		{
			ERRverror("Crop3D",ERR_ERROR,"Cannot create connection list for cell set %d", out_set);
			return 0;
		}
		box.CopyCells(out_node_connect_list, ncells, in.cell_set[in_set].cell_nnodes);
		ARRfree(out_node_connect_list);

		// Copy the cell data
		out.cell_set[out_set].ncell_data = in.cell_set[in_set].ncell_data;
		for(int cdata=0; cdata < in.cell_set[in_set].ncell_data; cdata++)
		{
			// Set added variable general characteristics
			if(FLDget_cell_data_label(in.cell_set[in_set].obj_id(OM_OBJ_RW), cdata, txt,  TXT_SIZE) != 1) txt[0]  = '\0';
			if(FLDget_cell_data_units(in.cell_set[in_set].obj_id(OM_OBJ_RW), cdata, txt2, TXT_SIZE) != 1) txt2[0] = '\0';
			FLDset_cell_data_comp(out.cell_set[out_set].obj_id(OM_OBJ_RW), cdata,
								  in.cell_set[in_set].cell_data[cdata].veclen, txt, txt2);
			if(FLDget_cell_data_id(in.cell_set[in_set].obj_id(OM_OBJ_RW), cdata, &id) == 1)
				FLDset_cell_data_id(out.cell_set[out_set].obj_id(OM_OBJ_RW), cdata, id);

			char *in_values = (char *)in.cell_set[in_set].cell_data[cdata].values.ret_array_ptr(OM_GET_ARRAY_RD, &in_data_size, &in_data_type);
			if(!in_values)
			{
				ERRverror("Crop3D",ERR_ERROR,"Cannot access cell data component %d for cell set %d", cdata, in_set);
				return 0;
			}

			// Set new variable type and null flag
			FLDset_cell_data_type(out.cell_set[out_set].obj_id(OM_OBJ_RW), cdata, in_data_type);
			int cell_null_flag;
			double cell_null_value; // large enough to hold any prim
			if(FLDget_cell_null_data(in.cell_set[in_set].obj_id(OM_OBJ_RW), cdata, &cell_null_flag, (char *)&cell_null_value) == 1)
			{
			   FLDset_cell_null_flag(out.cell_set[out_set].obj_id(OM_OBJ_RW), cdata, cell_null_flag);
			   if(cell_null_flag)
			   {
				  FLDset_cell_null_data(out.cell_set[out_set].obj_id(OM_OBJ_RW), cdata, (char *)&cell_null_value, in_data_type);
			   }
			}

			char *out_values = (char *)out.cell_set[out_set].cell_data[cdata].values.ret_array_ptr(OM_GET_ARRAY_WR);
			if(!out_values)
			{
				ERRverror("Crop3D",ERR_ERROR,"Cannot access cell data component %d for cell set %d", cdata, out_set);
				ARRfree(in_values);
				return 0;
			}

			box.CopyCellData(in_values, out_values, in.cell_set[in_set].cell_data[cdata].veclen, in_data_type);

			ARRfree(out_values);
			ARRfree(in_values);
		}
	}

	// return 1 for success
	return 1;
}

#include "dv_cgns.h"
#include <avs/fld.h>

/* read coordinate data, fill-in FLD */
/* returns 0 if all OK, 1 otherwise */
/* sets cg_err_str to error string if cgns read error */
static int
add_str_coords(OMobj_id fld_id, int base, int zone, int index_dim, int phys_dim,
	int *vertex_size, int *rind, int *XYZ, int f_indx, char **cg_err_str)
{
	float *coords, *tmp;
	int	i_min[3], i_max[3];
	int	i, n, size;
	int	indx;
	char	coord1_name[12], coord2_name[12], coord3_name[12];
	char	stat_str[100];
	int	stop;

	*cg_err_str = NULL;
	if(FLDset_nspace(fld_id, phys_dim) != 1)
	{
		ERRverror("DVread_cgns:add_str_coords",ERR_INFO,
			"can't set nspace\n");
		return (1);
	}
	if(FLDset_ndim(fld_id, index_dim) != 1)
	{
		ERRverror("DVread_cgns:add_str_coords",ERR_INFO,
			"can't set ndim\n");
		return (1);
	}
	if(FLDset_dims(fld_id, vertex_size) != 1)
	{
		ERRverror("DVread_cgns:add_str_coords",ERR_INFO,
			"can't set dims\n");
		return (1);
	}
	n = vertex_size[0];
	i_min[0] = 1 + rind[0];
	i_max[0] = vertex_size[0] + rind[0];
	if(index_dim > 1)
	{
		n *= vertex_size[1];
		i_min[1] = 1 + rind[2];
		i_max[1] = vertex_size[1] + rind[2];
	}
	if(index_dim > 2)
	{
		n *= vertex_size[2];
		i_min[2] = 1 + rind[4];
		i_max[2] = vertex_size[2] + rind[4];
	}
	/* set-up cgns coordinate names used in file */
	switch(phys_dim)
	{
		case 3:
			sprintf(coord1_name,"CoordinateX");
			sprintf(coord2_name,"CoordinateY");
			sprintf(coord3_name,"CoordinateZ");
			break;
		case 2:
			if(XYZ[0])
			{
				sprintf(coord1_name,"CoordinateX");
				if(XYZ[1])
					sprintf(coord2_name,"CoordinateY");
				else
					sprintf(coord2_name,"CoordinateZ");
			}
			else
			{
				sprintf(coord1_name,"CoordinateY");
				sprintf(coord2_name,"CoordinateZ");
			}
			break;
		case 1:
			if(XYZ[0])
				sprintf(coord1_name,"CoordinateX");
			else
			if(XYZ[1])
				sprintf(coord1_name,"CoordinateY");
			else
				sprintf(coord1_name,"CoordinateZ");
			break;
	}

	sprintf(stat_str,"rd_cgns: zone %d\ncoord X",zone);
	OMstatus_check(0.1,stat_str,&stop);
	if(stop) return (1);

	/* alloc temp array */
	tmp = (float *)ARRalloc(NULL, DTYPE_FLOAT, n, NULL);
	if(tmp == NULL)
	{
		ERRverror("DVread_cgns:add_str_coords",ERR_INFO,
			"failed to alloc mem for tmp\n");
		return (1);
	}
	if(FLDget_coord(fld_id, &coords, &size, OM_GET_ARRAY_WR) != 1)
	{
		ARRfree(tmp);
		ERRverror("DVread_cgns:add_str_coords",ERR_INFO,
			"failed to get coords\n");
		return (1);
	}
	/* read 1st coordinate - fill-out FLD */
	if(cg_coord_read(f_indx, base, zone, coord1_name, RealSingle,
		i_min, i_max, tmp))
	{
		ARRfree(tmp);
		ARRfree(coords);
		*cg_err_str = cg_get_error();
		return (1);
	}
	for(i=0, indx=0; i<n; ++i, indx+=phys_dim)
		coords[indx] = tmp[i];

	sprintf(stat_str,"rd_cgns: zone %d\ncoord Y",zone);
	OMstatus_check(33.3,stat_str,&stop);
	if(stop) return (1);

	/* if 2D read 2nd coordinate */
	if(phys_dim > 1)
	{
		if(cg_coord_read(f_indx, base, zone, coord2_name, RealSingle,
			i_min, i_max, tmp))
		{
			ARRfree(tmp);
			ARRfree(coords);
			*cg_err_str = cg_get_error();
			return (1);
		}
		for(i=0, indx=1; i<n; ++i, indx+=phys_dim)
			coords[indx] = tmp[i];
	}

	sprintf(stat_str,"rd_cgns: zone %d\ncoord Z",zone);
	OMstatus_check(66.6,stat_str,&stop);
	if(stop) return (1);

	/* if 3D read 3rd coordinate */
	if(phys_dim > 2)
	{
		if(cg_coord_read(f_indx, base, zone, coord3_name, RealSingle,
			i_min, i_max, tmp))
		{
			ARRfree(tmp);
			ARRfree(coords);
			*cg_err_str = cg_get_error();
			return (1);
		}
		for(i=0, indx=2; i<n; ++i, indx+=phys_dim)
			coords[indx] = tmp[i];
	}
	/* do I need this ? -TODO */
/*	if(FLDset_coord(fld_id, coords, size, OM_SET_ARRAY_COPY) != 1)
	{
		ARRfree(coords);
		ERRverror("DVread_cgns:add_str_coords",ERR_INFO,
			"failed to set coords\n");
		return (1);
	}*/
	ARRfree(tmp);
	ARRfree(coords);

	sprintf(stat_str,"rd_cgns: zone %d\ncoord Z",zone);
	OMstatus_check(100.0,stat_str,&stop);
	if(stop) return (1);

	return (0);
}

/* read cell data, fill-in FLD */
/* returns 0 if all OK, 1 otherwise */
/* sets cg_err_str to error string if cgns read error */
static int
add_str_cell_data(OMobj_id fld_id, int base, int zone, int index_dim,
	int *vertex_size, int *rind, int solution, int f_indx, char **cg_err_str)
{
	OMobj_id	cell_id, cell_data_id, cell_data_el_id, values_id;
	int		i, nFields;
	DataType_t	type;
	char		name[CGNS_NAME_MAX_LEN];
	int		i_min[3], i_max[3];
	int		size, junk;
	int		*i_arr;
	float		*f_arr;
	char		stat_str[100];
	int		stop;


	*cg_err_str = NULL;
	if(FLDget_cell_set(fld_id, 0, &cell_id) != 1)
	{
		ERRverror("DVread_cgns:add_str_cell_data",ERR_INFO,
			"failed to get cell_id\n");
		return (1);
	}
	if(cg_nfields(f_indx, base, zone, solution, &nFields))
	{
		*cg_err_str = cg_get_error();
		return (1);
	}
	if(OMset_name_int_val(cell_id, OMstr_to_name("ncell_data"), nFields) !=
		OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_str_cell_data",ERR_INFO,
			"failed to set ncell_data\n");
		return (1);
	}
	/* find the cell_data array object */
	cell_data_id = OMfind_subobj(cell_id, OMstr_to_name("cell_data"),
		OM_OBJ_RW);
	if(OMis_null_obj(cell_data_id))
	{
		ERRverror("DVread_cgns:add_str_cell_data",ERR_INFO,
			"can't locate cell_data_id object\n");
		return (1);
	}
	for(i=0; i<nFields; ++i)
	{
		sprintf(stat_str,"rd_cgns: zone %d data\nfield %d",zone,i+1);
		OMstatus_check(i*100/nFields+1,stat_str,&stop);
		if(stop) return (1);
	
		/* get field info */
		if(cg_field_info(f_indx, base, zone, solution, i+1, &type, name))
		{
			*cg_err_str = cg_get_error();
			return (1);
		}
		/* get cell_data array subobject id*/
		if(OMget_array_val(cell_data_id, i, &cell_data_el_id, OM_OBJ_RW)
			!= OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_str_cell_data",ERR_INFO,
				"can't get cell_data_el_id element object\n");
			return (0);
		}
		if(OMis_null_obj(cell_data_el_id))
		{
			ERRverror("DVread_cgns:add_str_cell_data",ERR_INFO,
				"can't locate cell_data_el_id object\n");
			return (1);
		}
		/* set veclen */
		if(OMset_name_int_val(cell_data_el_id, OMstr_to_name("veclen"), 1) !=
			OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_str_cell_data",ERR_INFO,
				"failed to set veclen\n");
			return (1);
		}
		/* set label */
		if(OMset_name_str_val(cell_data_el_id, OMstr_to_name("labels"), name) !=
			OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_str_cell_data",ERR_INFO,
				"failed to set labels string\n");
			return (1);
		}
		/* prepare the selection range */
		i_min[0] = 1 + rind[0];
		i_max[0] = vertex_size[0] + rind[0] - 1;
		if(index_dim > 1)
		{
			i_min[1] = 1 + rind[2];
			i_max[1] = vertex_size[1] + rind[2] - 1;
		}
		if(index_dim > 2)
		{
			i_min[2] = 1 + rind[4];
			i_max[2] = vertex_size[2] + rind[4] - 1;
		}
		/* find the values array object */
		values_id = OMfind_subobj(cell_data_el_id, OMstr_to_name("values"),
			OM_OBJ_RW);
		if(OMis_null_obj(values_id))
		{
			ERRverror("DVread_cgns:add_str_cell_data",ERR_INFO,
				"can't locate values_id object\n");
			return (1);
		}
		/* set values data type */
		switch(type)
		{
			case Integer:
				if(OMset_data_type(values_id, OM_TYPE_INT) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:add_str_cell_data",
						ERR_INFO,
						"failed to set data type for values\n");
					return (1);
				}
				/* get values array */
				i_arr = OMret_array_ptr(values_id, OM_GET_ARRAY_RW, &size, &junk);
				if(i_arr == NULL)
				{
					ERRverror("DVread_cgns:add_str_cell_data",
						ERR_INFO,
						"failed to get values array\n");
					return (1);
				}
				if(cg_field_read(f_indx, base, zone, solution, name, Integer,
					i_min, i_max, (void *)i_arr))
				{
					*cg_err_str = cg_get_error();
					ARRfree(i_arr);
					return (1);
				}
				ARRfree(i_arr);
				break;
			case RealSingle:
			case RealDouble:
				if(OMset_data_type(values_id, OM_TYPE_FLOAT) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:add_str_cell_data",
						ERR_INFO,
						"failed to set data type for values\n");
					return (1);
				}
				/* get values array */
				f_arr = OMret_array_ptr(values_id, OM_GET_ARRAY_RW, &size, &junk);
				if(f_arr == NULL)
				{
					ERRverror("DVread_cgns:add_str_cell_data",
						ERR_INFO,
						"failed to get values array\n");
					return (1);
				}
				if(cg_field_read(f_indx, base, zone, solution, name, RealSingle,
					i_min, i_max, (void *)f_arr))
				{
					*cg_err_str = cg_get_error();
					ARRfree(f_arr);
					return (1);
				}

				ARRfree(f_arr);
				break;
		}
	}
	
	return (0);
}

/* read vertex data, fill-in FLD */
/* returns 0 if all OK, 1 otherwise */
/* sets cg_err_str to error string if cgns read error */
static int
add_str_vrtx_data(OMobj_id fld_id, int base, int zone, int index_dim,
	int *vertex_size, int *rind, int solution, int f_indx, char **cg_err_str)
{
	OMobj_id	node_data_id, node_data_el_id, values_id;
	int		i, nFields;
	DataType_t	type;
	char		name[CGNS_NAME_MAX_LEN];
	int		i_min[3], i_max[3];
	int		size, junk;
	int		*i_arr;
	float		*f_arr;
	char		stat_str[100];
	int		stop;

	*cg_err_str = NULL;
	if(cg_nfields(f_indx, base, zone, solution, &nFields))
	{
		*cg_err_str = cg_get_error();
		return (1);
	}
	if(OMset_name_int_val(fld_id, OMstr_to_name("nnode_data"), nFields) !=
		OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_str_vrtx_data",ERR_INFO,
			"failed to set nnode_data\n");
		return (1);
	}
	/* find the node_data array object */
	node_data_id = OMfind_subobj(fld_id, OMstr_to_name("node_data"),
		OM_OBJ_RW);
	if(OMis_null_obj(node_data_id))
	{
		ERRverror("DVread_cgns:add_str_vrtx_data",ERR_INFO,
			"can't locate node_data_id object\n");
		return (1);
	}
	for(i=0; i<nFields; ++i)
	{
		sprintf(stat_str,"rd_cgns: zone %d data\nfield %d",zone,i+1);
		OMstatus_check(i*100/nFields+1,stat_str,&stop);
		if(stop) return (1);
	
		/* get field info */
		if(cg_field_info(f_indx, base, zone, solution, i+1, &type, name))
		{
			*cg_err_str = cg_get_error();
			return (1);
		}
		/* get node_data array subobject id*/
		if(OMget_array_val(node_data_id, i, &node_data_el_id, OM_OBJ_RW)
			!= OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_str_vrtx_data",ERR_INFO,
				"can't get node_data_el_id element object\n");
			return (0);
		}
		if(OMis_null_obj(node_data_el_id))
		{
			ERRverror("DVread_cgns:add_str_vrtx_data",ERR_INFO,
				"can't locate node_data_el_id object\n");
			return (1);
		}
		/* set veclen */
		if(OMset_name_int_val(node_data_el_id, OMstr_to_name("veclen"), 1) !=
			OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_str_vrtx_data",ERR_INFO,
				"failed to set veclen\n");
			return (1);
		}
		/* set label */
		if(OMset_name_str_val(node_data_el_id, OMstr_to_name("labels"), name) !=
			OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_str_vrtx_data",ERR_INFO,
				"failed to set labels string\n");
			return (1);
		}
		/* prepare the selection range */
		i_min[0] = 1 + rind[0];
		i_max[0] = vertex_size[0] + rind[0];
		if(index_dim > 1)
		{
			i_min[1] = 1 + rind[2];
			i_max[1] = vertex_size[1] + rind[2];
		}
		if(index_dim > 2)
		{
			i_min[2] = 1 + rind[4];
			i_max[2] = vertex_size[2] + rind[4];
		}
		/* find the values array object */
		values_id = OMfind_subobj(node_data_el_id, OMstr_to_name("values"),
			OM_OBJ_RW);
		if(OMis_null_obj(values_id))
		{
			ERRverror("DVread_cgns:add_str_vrtx_data",ERR_INFO,
				"can't locate values_id object\n");
			return (1);
		}
		/* set values data type */
		switch(type)
		{
			case Integer:
				if(OMset_data_type(values_id, OM_TYPE_INT) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:add_str_vrtx_data",
						ERR_INFO,
						"failed to set data type for values\n");
					return (1);
				}
				/* get values array */
				i_arr = OMret_array_ptr(values_id, OM_GET_ARRAY_RW, &junk, &junk);
				if(i_arr == NULL)
				{
					ERRverror("DVread_cgns:add_str_vrtx_data",
						ERR_INFO,
						"failed to get values array\n");
					return (1);
				}
				if(cg_field_read(f_indx, base, zone, solution, name, Integer,
					i_min, i_max, (void *)i_arr))
				{
					*cg_err_str = cg_get_error();
					ARRfree(i_arr);
					return (1);
				}
				ARRfree(i_arr);
				break;
			case RealSingle:
			case RealDouble:
				if(OMset_data_type(values_id, OM_TYPE_FLOAT) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:add_str_vrtx_data",
						ERR_INFO,
						"failed to set data type for values\n");
					return (1);
				}
				/* get values array */
				f_arr = OMret_array_ptr(values_id, OM_GET_ARRAY_RW, &size, &junk);
				if(f_arr == NULL)
				{
					ERRverror("DVread_cgns:add_str_vrtx_data",
						ERR_INFO,
						"failed to get values array\n");
					return (1);
				}
				if(cg_field_read(f_indx, base, zone, solution, name, RealSingle,
					i_min, i_max, (void *)f_arr))
				{
					*cg_err_str = cg_get_error();
					ARRfree(f_arr);
					return (1);
				}

				ARRfree(f_arr);
				break;
		}
	}
	
	return (0);
}

/* reads coordinate data for a structured filed, and appends */
/* to the multi-block structure, returns 0 if all OK, 1 otherwise */
int
add_structured_coords(OMobj_id DVread_cgns_id, char *file_name,
	int base, int zone, int index_dim, int phys_dim, int *vertex_size,
	int *rind, int *XYZ)
{
	int	f_indx;
	OMobj_id	mb_id, fld_id, fld_el_id;
	int	nblocks;
	int	stat;
	char	*cg_err_str;

	/* find the Multi_Block object */
	mb_id = OMfind_subobj(DVread_cgns_id, OMstr_to_name("Multi_Block"),
		OM_OBJ_RW);
	if(OMis_null_obj(mb_id))
	{
		ERRverror("DVread_cgns:add_structured_coords",ERR_INFO,
			"can't locate Multi_Block object\n");
		return (1);
	}
	/* get nblocks value */
	if(OMget_name_int_val(mb_id, OMstr_to_name("nblocks"),
		&nblocks) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_structured_coords",ERR_INFO,
			"can't get nblocks value\n");
		return (1);
	}
	/* add another block to Multi_Block */
	if(OMset_name_int_val(mb_id, OMstr_to_name("nblocks"),
		nblocks+1) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_structured_coords",ERR_INFO,
			"can't set nblocks value\n");
		return (1);
	}
	/* find the fields object */
	fld_id = OMfind_subobj(mb_id, OMstr_to_name("fields"),
		OM_OBJ_RW);
	if(OMis_null_obj(fld_id))
	{
		ERRverror("DVread_cgns:add_structured_coords",ERR_INFO,
			"can't locate fields object\n");
		return (1);
	}
	if(OMget_array_val(fld_id, nblocks, &fld_el_id, OM_OBJ_RW)
		!= OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_structured_coords",ERR_INFO,
			"can't get fld_el_id element object\n");
		return (0);
	}
	/* CGNS open file */
	if(cg_open(file_name, MODE_READ, &f_indx))
	{
		OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
			cg_get_error());
		return (1);
	}
	stat = add_str_coords(fld_el_id, base, zone, index_dim, phys_dim,
		vertex_size, rind, XYZ, f_indx, &cg_err_str);
	if(stat)
	{
		if(cg_err_str != NULL)
			OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"), cg_err_str);
		cg_close(f_indx);
		return (1);
	}
	cg_close(f_indx);
	return (0);
}

/* reads vertex data for a structured field, and sets */
/* the multi-block structure, returns 0 if all OK, 1 otherwise */
int
add_structured_vrtx_data(OMobj_id DVread_cgns_id, char *file_name,
	int base, int zone, int fld_el_indx, int index_dim, int *vertex_size,
	int *rind, int solution)
{
	int	f_indx;
	OMobj_id	mb_id, fld_id, fld_el_id;
	int	stat;
	char	*cg_err_str;

	/* find the Multi_Block object */
	mb_id = OMfind_subobj(DVread_cgns_id, OMstr_to_name("Multi_Block"),
		OM_OBJ_RW);
	if(OMis_null_obj(mb_id))
	{
		ERRverror("DVread_cgns:add_structured_vrtx_data",ERR_INFO,
			"can't locate Multi_Block object\n");
		return (1);
	}
	/* find the fields object */
	fld_id = OMfind_subobj(mb_id, OMstr_to_name("fields"),
		OM_OBJ_RW);
	if(OMis_null_obj(fld_id))
	{
		ERRverror("DVread_cgns:add_structured_cell_data",ERR_INFO,
			"can't locate fields object\n");
		return (1);
	}
	if(OMget_array_val(fld_id, fld_el_indx, &fld_el_id, OM_OBJ_RW)
		!= OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_structured_vrtx_data",ERR_INFO,
			"can't get fld_el_id element object\n");
		return (0);
	}
	/* CGNS open file */
	if(cg_open(file_name, MODE_READ, &f_indx))
	{
		OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
			cg_get_error());
		return (1);
	}
	stat = add_str_vrtx_data(fld_el_id, base, zone, index_dim, vertex_size,
		rind, solution, f_indx, &cg_err_str);
	if(stat)
	{
		if(cg_err_str != NULL)
			OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"), cg_err_str);
		cg_close(f_indx);
		return (1);
	}
	cg_close(f_indx);
	return (0);
}

/* reads cell data for a structured field, and sets */
/* the multi-block structure, returns 0 if all OK, 1 otherwise */
int
add_structured_cell_data(OMobj_id DVread_cgns_id, char *file_name,
	int base, int zone, int fld_el_indx, int index_dim, int *vertex_size,
	int *rind, int solution)
{
	int	f_indx;
	OMobj_id	mb_id, fld_id, fld_el_id;
	int	stat;
	char	*cg_err_str;

	/* find the Multi_Block object */
	mb_id = OMfind_subobj(DVread_cgns_id, OMstr_to_name("Multi_Block"),
		OM_OBJ_RW);
	if(OMis_null_obj(mb_id))
	{
		ERRverror("DVread_cgns:add_structured_cell_data",ERR_INFO,
			"can't locate Multi_Block object\n");
		return (1);
	}
	/* find the fields object */
	fld_id = OMfind_subobj(mb_id, OMstr_to_name("fields"),
		OM_OBJ_RW);
	if(OMis_null_obj(fld_id))
	{
		ERRverror("DVread_cgns:add_structured_cell_data",ERR_INFO,
			"can't locate fields object\n");
		return (1);
	}
	if(OMget_array_val(fld_id, fld_el_indx, &fld_el_id, OM_OBJ_RW)
		!= OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_structured_cell_data",ERR_INFO,
			"can't get fld_el_id element object\n");
		return (0);
	}
	/* CGNS open file */
	if(cg_open(file_name, MODE_READ, &f_indx))
	{
		OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
			cg_get_error());
		return (1);
	}
	stat = add_str_cell_data(fld_el_id, base, zone, index_dim, vertex_size,
		rind, solution, f_indx, &cg_err_str);
	if(stat)
	{
		if(cg_err_str != NULL)
			OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"), cg_err_str);
		cg_close(f_indx);
		return (1);
	}
	cg_close(f_indx);
	return (0);
}


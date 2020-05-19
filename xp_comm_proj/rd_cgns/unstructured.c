#include "dv_cgns.h"
#include <avs/fld.h>

/* read coordinate data, fill-in FLD */
/* returns 0 if all OK, 1 otherwise */
/* sets cg_err_str to error string if cgns read error */
static int
add_unstr_coords(OMobj_id fld_id, int base, int zone, int phys_dim,
	int *vertex_size, int *XYZ, int f_indx, char **cg_err_str)
{
	float *coords, *tmp;
	int	i_min, i_max;
	int	i, n, size;
	int	indx;
	char	coord1_name[12], coord2_name[12], coord3_name[12];
	char	stat_str[100];
	int	stop;

	*cg_err_str = NULL;
	if(FLDset_nspace(fld_id, phys_dim) != 1)
	{
		ERRverror("DVread_cgns:add_unstr_coords",ERR_INFO,
			"can't set nspace\n");
		return (1);
	}
	n = vertex_size[0];
	i_min = 1;
	i_max = n;
	/* we add an additional dummy first node in FLD */
	/* this way when no reordering of connection is required for a cell */
	/* we use the CGNS list starting from 1, AVS starts from 0 */
	if(FLDset_nnodes(fld_id, n+1) != 1)
	{
		ERRverror("DVread_cgns:add_unstr_coords",ERR_INFO,
			"can't set nnodes\n");
		return (1);
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
		ERRverror("DVread_cgns:add_unstr_coords",ERR_INFO,
			"failed to alloc mem for tmp\n");
		return (1);
	}
	if(FLDget_coord(fld_id, &coords, &size, OM_GET_ARRAY_WR) != 1)
	{
		ARRfree(tmp);
		ERRverror("DVread_cgns:add_unstr_coords",ERR_INFO,
			"failed to get coords\n");
		return (1);
	}
	/* read 1st coordinate - fill-out FLD */
	if(cg_coord_read(f_indx, base, zone, coord1_name, RealSingle,
		&i_min, &i_max, tmp))
	{
		ARRfree(tmp);
		ARRfree(coords);
		*cg_err_str = cg_get_error();
		return (1);
	}
	/* copy to first node coords (our dummy node) the coords of first CGNS node */
	/* this will ensure that min and max of coord array is correct */
	coords[0] = tmp[0];
	for(i=0, indx=phys_dim; i<n; ++i, indx+=phys_dim)
		coords[indx] = tmp[i];
	
	sprintf(stat_str,"rd_cgns: zone %d\ncoord Y",zone);
	OMstatus_check(33.3,stat_str,&stop);
	if(stop) return (1);

	/* if 2D read 2nd coordinate */
	if(phys_dim > 1)
	{
		if(cg_coord_read(f_indx, base, zone, coord2_name, RealSingle,
			&i_min, &i_max, tmp))
		{
			ARRfree(tmp);
			ARRfree(coords);
			*cg_err_str = cg_get_error();
			return (1);
		}
		coords[1] = tmp[0];
		for(i=0, indx=phys_dim+1; i<n; ++i, indx+=phys_dim)
			coords[indx] = tmp[i];
	}
	
	sprintf(stat_str,"rd_cgns: zone %d\ncoord Z",zone);
	OMstatus_check(66.6,stat_str,&stop);
	if(stop) return (1);

	/* if 3D read 3rd coordinate */
	if(phys_dim > 2)
	{
		if(cg_coord_read(f_indx, base, zone, coord3_name, RealSingle,
			&i_min, &i_max, tmp))
		{
			ARRfree(tmp);
			ARRfree(coords);
			*cg_err_str = cg_get_error();
			return (1);
		}
		coords[2] = tmp[0];
		for(i=0, indx=phys_dim+2; i<n; ++i, indx+=phys_dim)
			coords[indx] = tmp[i];
	}
	/* do I need this ? -TODO */
/*	if(FLDset_coord(fld_id, coords, size, OM_SET_ARRAY_COPY) != 1)
	{
		ARRfree(coords);
		ERRverror("DVread_cgns:add_unstr_coords",ERR_INFO,
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


/* reads coordinate data for a structured filed, and appends */
/* to the multi-block structure, returns 0 if all OK, 1 otherwise */
int
add_unstructured_coords(OMobj_id DVread_cgns_id, char *file_name,
	int base, int zone, int phys_dim, int *vertex_size,
	int *XYZ)
{
	int	f_indx;
	OMobj_id	mb_id, fld_id, fld_el_id;
	int	nblocks;
	int	stat;
	char	*cg_err_str;

	/* find the Multi_Block_Unstr object */
	mb_id = OMfind_subobj(DVread_cgns_id, OMstr_to_name("Multi_Block_Unstr"),
		OM_OBJ_RW);
	if(OMis_null_obj(mb_id))
	{
		ERRverror("DVread_cgns:add_unstructured_coords",ERR_INFO,
			"can't locate Multi_Block_Unstr object\n");
		return (1);
	}
	/* get nblocks value */
	if(OMget_name_int_val(mb_id, OMstr_to_name("nblocks"),
		&nblocks) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_unstructured_coords",ERR_INFO,
			"can't get nblocks value\n");
		return (1);
	}
	/* add another block to Multi_Block */
	if(OMset_name_int_val(mb_id, OMstr_to_name("nblocks"),
		nblocks+1) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_unstructured_coords",ERR_INFO,
			"can't set nblocks value\n");
		return (1);
	}
	/* find the fields object */
	fld_id = OMfind_subobj(mb_id, OMstr_to_name("fields"),
		OM_OBJ_RW);
	if(OMis_null_obj(fld_id))
	{
		ERRverror("DVread_cgns:add_unstructured_coords",ERR_INFO,
			"can't locate fields object\n");
		return (1);
	}
	if(OMget_array_val(fld_id, nblocks, &fld_el_id, OM_OBJ_RW)
		!= OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_unstructured_coords",ERR_INFO,
			"can't get fld_el_id element object\n");
		return (1);
	}
	/* CGNS open file */
	if(cg_open(file_name, MODE_READ, &f_indx))
	{
		OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
			cg_get_error());
		return (1);
	}
	stat = add_unstr_coords(fld_el_id, base, zone, phys_dim,
		vertex_size, XYZ, f_indx, &cg_err_str);
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

/* set cell_set data - type, ncells, node_connect_list */
static int
add_elem(OMobj_id cell_set_id, int ncells, int *conn, int n, char *el_name)
{
	OMobj_id	conn_id;

	if(FLDset_cell_set(cell_set_id, el_name) != 1)
	{
		ERRverror("DVread_cgns:add_elem",ERR_INFO,
			"can't set FLD cell_set to '%s'\n",el_name);
		return (1);
	}
	if(FLDset_ncells(cell_set_id, ncells) != 1)
	{
		ERRverror("DVread_cgns:add_elem",ERR_INFO,
			"can't set FLD ncells\n");
		return (1);
	}
	conn_id = OMfind_subobj(cell_set_id, OMstr_to_name("node_connect_list"),
		OM_OBJ_RW);
	if(OMis_null_obj(conn_id))
	{
		ERRverror("DVread_cgns:add_elem",ERR_INFO,
			"can't locate node_connect_list object\n");
		return (1);
	}
	if(OMset_array(conn_id, DTYPE_INT, conn, n, OM_SET_ARRAY_COPY) !=
		OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_elem",ERR_INFO,
			"failed to set node_connect_list array\n");
		return (1);
	}
	return (0);
}

/* set cell_set data for Polyhedron - type, npolys, poly_connect_list, poly_nnodes */
static int
add_ngon_elem(OMobj_id cell_set_id, int ncells, int *conn, int el_nnodes)
{
	OMobj_id	conn_id;
	int		*nnodes, i;

	if(FLDset_cell_set(cell_set_id, "Polyhedron") != 1)
	{
		ERRverror("DVread_cgns:add_ngon_elem",ERR_INFO,
			"can't set FLD cell_set to 'Polyhedron'\n");
		return (1);
	}
	if(FLDset_npolys(cell_set_id, ncells) != 1)
	{
		ERRverror("DVread_cgns:add_ngon_elem",ERR_INFO,
			"can't set FLD npolys\n");
		return (1);
	}
	conn_id = OMfind_subobj(cell_set_id, OMstr_to_name("poly_connect_list"),
		OM_OBJ_RW);
	if(OMis_null_obj(conn_id))
	{
		ERRverror("DVread_cgns:add_ngon_elem",ERR_INFO,
			"can't locate poly_connect_list object\n");
		return (1);
	}
	if(OMset_array(conn_id, DTYPE_INT, conn, ncells*el_nnodes, OM_SET_ARRAY_COPY) !=
		OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_ngon_elem",ERR_INFO,
			"failed to set poly_connect_list array\n");
		return (1);
	}
	if(FLDget_poly_nnodes(cell_set_id, &nnodes, &i, OM_GET_ARRAY_RW) !=
		OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_ngon_elem",ERR_INFO,
			"failed to get poly_nnodes array\n");
		return (1);
	}
	for(i=0;i<ncells;++i)
		nnodes[i] = el_nnodes;
	ARRfree(nnodes);
	return (0);
}

/* adjust CGNS node ordering to AVS one - reorder/remove nodes */
/* returns number of nodes placed in out_order */
static int
adjust_node_order(ElementType_t type, int *in_order, int *out_order,
	int *avs_type)
{
	int i, indx;

	indx = 0;
	switch(type)
	{
		case NODE:
			out_order[indx++] = in_order[0];
			*avs_type = Point;
			break;
		case BAR_2:
			out_order[indx++] = in_order[0];
			out_order[indx++] = in_order[1];
			*avs_type = Line;
			break;
		case BAR_3:
			out_order[indx++] = in_order[0];
			out_order[indx++] = in_order[1];
			out_order[indx++] = in_order[2];
			*avs_type = Line2;
			break;
		case TRI_3:
			out_order[indx++] = in_order[0];
			out_order[indx++] = in_order[1];
			out_order[indx++] = in_order[2];
			*avs_type = Tri;
			break;
		case TRI_6:
			out_order[indx++] = in_order[0];
			out_order[indx++] = in_order[1];
			out_order[indx++] = in_order[2];
			out_order[indx++] = in_order[3];
			out_order[indx++] = in_order[4];
			out_order[indx++] = in_order[5];
			*avs_type = Tri2;
			break;
		case QUAD_4:
			out_order[indx++] = in_order[0];
			out_order[indx++] = in_order[1];
			out_order[indx++] = in_order[2];
			out_order[indx++] = in_order[3];
			*avs_type = Quad;
			break;
		case QUAD_8:
		case QUAD_9:
			out_order[indx++] = in_order[0];
			out_order[indx++] = in_order[1];
			out_order[indx++] = in_order[2];
			out_order[indx++] = in_order[3];
			out_order[indx++] = in_order[4];
			out_order[indx++] = in_order[5];
			out_order[indx++] = in_order[6];
			out_order[indx++] = in_order[7];
			*avs_type = Quad2;
			break;
		case TETRA_4:
			out_order[indx++] = in_order[3];
			out_order[indx++] = in_order[0];
			out_order[indx++] = in_order[1];
			out_order[indx++] = in_order[2];
			*avs_type = Tet;
			break;
		case TETRA_10:
			out_order[indx++] = in_order[3];
			out_order[indx++] = in_order[0];
			out_order[indx++] = in_order[1];
			out_order[indx++] = in_order[2];
			out_order[indx++] = in_order[7];
			out_order[indx++] = in_order[8];
			out_order[indx++] = in_order[9];
			out_order[indx++] = in_order[4];
			out_order[indx++] = in_order[5];
			out_order[indx++] = in_order[6];
			*avs_type = Tet2;
			break;
		case PYRA_5:
			out_order[indx++] = in_order[4];
			out_order[indx++] = in_order[3];
			out_order[indx++] = in_order[0];
			out_order[indx++] = in_order[1];
			out_order[indx++] = in_order[2];
			*avs_type = Pyr;
			break;
		case PYRA_14:
			out_order[indx++] = in_order[4];
			out_order[indx++] = in_order[3];
			out_order[indx++] = in_order[0];
			out_order[indx++] = in_order[1];
			out_order[indx++] = in_order[2];
			out_order[indx++] = in_order[12];
			out_order[indx++] = in_order[9];
			out_order[indx++] = in_order[10];
			out_order[indx++] = in_order[11];
			out_order[indx++] = in_order[8];
			out_order[indx++] = in_order[5];
			out_order[indx++] = in_order[6];
			out_order[indx++] = in_order[7];
			*avs_type = Pyr2;
			break;
		case PENTA_6:
			out_order[indx++] = in_order[4];
			out_order[indx++] = in_order[5];
			out_order[indx++] = in_order[3];
			out_order[indx++] = in_order[1];
			out_order[indx++] = in_order[2];
			out_order[indx++] = in_order[0];
			*avs_type = Prism;
			break;
		case PENTA_15:
		case PENTA_18:
			out_order[indx++] = in_order[4];
			out_order[indx++] = in_order[5];
			out_order[indx++] = in_order[3];
			out_order[indx++] = in_order[1];
			out_order[indx++] = in_order[2];
			out_order[indx++] = in_order[0];
			out_order[indx++] = in_order[13];
			out_order[indx++] = in_order[14];
			out_order[indx++] = in_order[12];
			out_order[indx++] = in_order[7];
			out_order[indx++] = in_order[8];
			out_order[indx++] = in_order[6];
			out_order[indx++] = in_order[11];
			out_order[indx++] = in_order[10];
			out_order[indx++] = in_order[9];
			*avs_type = Prism2;
			break;
		case HEXA_8:
			out_order[indx++] = in_order[6];
			out_order[indx++] = in_order[7];
			out_order[indx++] = in_order[4];
			out_order[indx++] = in_order[5];
			out_order[indx++] = in_order[2];
			out_order[indx++] = in_order[3];
			out_order[indx++] = in_order[0];
			out_order[indx++] = in_order[1];
			*avs_type = Hex;
			break;
		case HEXA_20:
		case HEXA_27:
			out_order[indx++] = in_order[6];
			out_order[indx++] = in_order[7];
			out_order[indx++] = in_order[4];
			out_order[indx++] = in_order[5];
			out_order[indx++] = in_order[2];
			out_order[indx++] = in_order[3];
			out_order[indx++] = in_order[0];
			out_order[indx++] = in_order[1];
			out_order[indx++] = in_order[18];
			out_order[indx++] = in_order[19];
			out_order[indx++] = in_order[16];
			out_order[indx++] = in_order[17];
			out_order[indx++] = in_order[10];
			out_order[indx++] = in_order[11];
			out_order[indx++] = in_order[8];
			out_order[indx++] = in_order[9];
			out_order[indx++] = in_order[14];
			out_order[indx++] = in_order[15];
			out_order[indx++] = in_order[12];
			out_order[indx++] = in_order[13];
			*avs_type = Hex2;
			break;
		default: /* NGON_n */
			for(i=0;i<type - NGON_n;++i)
				out_order[indx++] = in_order[i];
			*avs_type = PolyH;
			break;
	}
	return indx;
}

static void
avs_cg_npe(ElementType_t type, int *npe)
{
	switch(type)
	{
		case NODE:
			*npe = 1;
			break;
		case BAR_2:
			*npe = 2;
			break;
		case BAR_3:
			*npe = 3;
			break;
		case TRI_3:
			*npe = 3;
			break;
		case TRI_6:
			*npe = 6;
			break;
		case QUAD_4:
			*npe = 4;
			break;
		case QUAD_8:
		case QUAD_9:
			*npe = 8;
			break;
		case TETRA_4:
			*npe = 4;
			break;
		case TETRA_10:
			*npe = 10;
			break;
		case PYRA_5:
			*npe = 5;
			break;
		case PYRA_14:
			*npe = 13;
			break;
		case PENTA_6:
			*npe = 6;
			break;
		case PENTA_15:
		case PENTA_18:
			*npe = 15;
			break;
		case HEXA_8:
			*npe = 8;
			break;
		case HEXA_20:
		case HEXA_27:
			*npe = 20;
			break;
		default: /* NGON_n */
			*npe = type - NGON_n;
			break;
	}
}

static int
add_unstr_section(OMobj_id fld_el_id, int base, int zone, int section,
	ElementType_t el_type, int start, int end, int parent_flag, int f_indx,
	char **cg_err_str)
{
	int		ncell_sets;
	OMobj_id	cell_set_id, cell_set_el_id;
	int		el_cnt;
	int		j, indx;
	int		avs_type;
	int		stat;
	int		el_data_size, *elements, *elements_tmp;
	int		npe, avs_npe;
	static char avs_el_name[21][11] = {
		"", "Point", "Line", "", "Tri", "Quad",
		"Tet", "Hex", "Prism", "Pyr", "",
		"", "Line2", "", "Tri2", "Quad2",
		"Tet2", "Hex2", "Prism2", "Pyr2", "Polyhedron" };

	*cg_err_str = NULL;

	if(cg_ElementDataSize(f_indx, base, zone, section, &el_data_size))
	{
		*cg_err_str = cg_get_error();
		ERRverror("DVread_cgns:add_unstr_section",ERR_INFO,
			"failed to get CGNS ElementDataSize\n");
		return (1);
	}
	elements = ARRalloc(NULL, DTYPE_INT, el_data_size, NULL);
	if(elements == NULL)
	{
		ERRverror("DVread_cgns:add_unstr_section",ERR_INFO,
			"failed alloc memory for Elements\n");
		return (1);
	}
	if(cg_elements_read(f_indx, base, zone, section, elements, NULL))
	{
		*cg_err_str = cg_get_error();
		ERRverror("DVread_cgns:add_unstr_section",ERR_INFO,
			"failed to read CGNS Elements\n");
		ARRfree(elements);
		return (1);
	}
	el_cnt = end - start + 1;
	/* alloc mem for temp elements connection array */
	cg_npe(el_type, &npe);
	avs_cg_npe(el_type, &avs_npe);
	elements_tmp =ARRalloc(NULL, DTYPE_INT, el_cnt*avs_npe, NULL);
	if(elements_tmp == NULL)
	{
		ERRverror("DVread_cgns:add_unstr_section",ERR_INFO,
			"failed alloc memory for Elements temp\n");
		ARRfree(elements);
		return (1);
	}
	/* collect connections for this element type and reorder them */
	/* suitably for AVS */
	indx = 0;
	for(j=0;j<el_data_size;)
	{
		indx += adjust_node_order(el_type, elements+j, elements_tmp+indx,
			&avs_type);
		j += npe;
	}
	ARRfree(elements);

	/* get current ncell_set */
	if(OMget_name_int_val(fld_el_id, OMstr_to_name("ncell_sets"), &ncell_sets) !=
		OM_STAT_SUCCESS)
	{
		ARRfree(elements_tmp);
		ERRverror("DVread_cgns:add_unstr_section",ERR_INFO,
			"can't get ncell_sets value\n");
		return (1);
	}
	/* add one more cell_set */
	if(OMset_name_int_val(fld_el_id, OMstr_to_name("ncell_sets"), ncell_sets+1) !=
		OM_STAT_SUCCESS)
	{
		ARRfree(elements_tmp);
		ERRverror("DVread_cgns:add_unstr_section",ERR_INFO,
			"can't set ncell_sets value\n");
		return (1);
	}
	/* get cell_set object id */
	cell_set_id = OMlookup_subobj(fld_el_id, OMstr_to_name("cell_set"),
		OM_OBJ_RW);
	if(OMis_null_obj(cell_set_id))
	{
		ARRfree(elements_tmp);
		ERRverror("DVread_cgns:add_unstr_section",ERR_INFO,
			"can't get cell_set object\n");
		return (1);
	}
	/* get cell_set element ncell_sets id */
	if(OMget_array_val(cell_set_id, ncell_sets, &cell_set_el_id, OM_OBJ_RW) !=
		OM_STAT_SUCCESS)
	{
		ARRfree(elements_tmp);
		ERRverror("DVread_cgns:add_unstr_section",ERR_INFO,
			"can't get cell_set_el element object\n");
		return (1);
	}
	if(el_type < NGON_n)
		stat = add_elem(cell_set_el_id, el_cnt, elements_tmp,
			el_cnt*avs_npe, avs_el_name[avs_type]);
	else
		stat = add_ngon_elem(cell_set_el_id, el_cnt, elements_tmp,
			avs_npe);
	ARRfree(elements_tmp);
	if(stat)
		return (1);
	return (0);
}

static int
add_unstr_mixed_section(OMobj_id fld_el_id, int base, int zone, int section,
	int parent_flag, int f_indx, char **cg_err_str)
{
	int		ncell_sets;
	OMobj_id	cell_set_id, cell_set_el_id;
	int		el_cnt[NGON_MAX];
	int		i, j, indx;
	int		avs_type;
	int		stat;
	int		el_data_size, *elements, *elements_tmp;
	int		npe, avs_npe;
	int		err;
	static char avs_el_name[21][11] = {
		"", "Point", "Line", "", "Tri", "Quad",
		"Tet", "Hex", "Prism", "Pyr", "",
		"", "Line2", "", "Tri2", "Quad2",
		"Tet2", "Hex2", "Prism2", "Pyr2", "Polyhedron" };


	*cg_err_str = NULL;

	if(cg_ElementDataSize(f_indx, base, zone, section, &el_data_size))
	{
		*cg_err_str = cg_get_error();
		ERRverror("DVread_cgns:add_unstr_mixed_section",ERR_INFO,
			"failed to get CGNS ElementDataSize\n");
		return (1);
	}
	elements = ARRalloc(NULL, DTYPE_INT, el_data_size, NULL);
	if(elements == NULL)
	{
		ERRverror("DVread_cgns:add_unstr_mixed_section",ERR_INFO,
			"failed alloc memory for Elements\n");
		return (1);
	}
	if(cg_elements_read(f_indx, base, zone, section, elements, NULL))
	{
		*cg_err_str = cg_get_error();
		ERRverror("DVread_cgns:add_unstr_mixed_section",ERR_INFO,
			"failed to read CGNS Elements\n");
		ARRfree(elements);
		return (1);
	}
	/* get current ncell_set */
	if(OMget_name_int_val(fld_el_id, OMstr_to_name("ncell_sets"), &ncell_sets) !=
		OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_unstr_mixed_section",ERR_INFO,
			"can't get ncell_sets value\n");
		ARRfree(elements);
		return (1);
	}
	for(i=NODE;i<NGON_MAX;++i)
		el_cnt[i] = 0;
	i = 0;
	/* count occurring element types, save count for each type in el_cnt */
	/* check if exists an element (NGON) not handeled - set err */
	err = 0;
	do
	{
		if(elements[i] < NGON_MAX)
			el_cnt[elements[i]]++;
		else
			err = elements[i];
		/* skip element connection data */
		cg_npe((ElementType_t)elements[i], &npe);
		i += npe+1;
	}
	while(i < el_data_size);
	if(err)
		ERRverror("DVread_cgns:add_unstr_mixed_section",ERR_INFO,
			"Section %d: found elements (like NGON_%d) not supported (only up to NGON_%d)\n",
			section+1, err-NGON_n, NGON_MAX-NGON_n-1);

	for(i=NODE;i<NGON_MAX;++i)  /* loop through all served types */
	{
		if(el_cnt[i])
		{
			/* alloc mem for temp elements connection array */
			avs_cg_npe(i, &avs_npe);
			elements_tmp =ARRalloc(NULL, DTYPE_INT, el_cnt[i]*avs_npe, NULL);
			if(elements_tmp == NULL)
			{
				ERRverror("DVread_cgns:add_unstr_mixed_section",ERR_INFO,
					"failed alloc memory for Elements temp\n");
				ARRfree(elements);
				return (1);
			}
			/* collect connections for this element type and reorder them */
			/* suitably for AVS */
			indx = 0;
			for(j=0;j<el_data_size;)
			{
				cg_npe((ElementType_t)elements[j], &npe);
				if(elements[j++] == i)
				{
					indx += adjust_node_order((ElementType_t)i, elements+j,
						elements_tmp+indx, &avs_type);
				}
				j += npe;
			}
			/* add one more cell_set */
			if(OMset_name_int_val(fld_el_id, OMstr_to_name("ncell_sets"),
					ncell_sets+1) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:add_unstr_section",ERR_INFO,
					"can't set ncell_sets value\n");
				ARRfree(elements);
				ARRfree(elements_tmp);
				return (1);
			}
			/* get cell_set object id */
			cell_set_id = OMlookup_subobj(fld_el_id, OMstr_to_name("cell_set"),
				OM_OBJ_RW);
			if(OMis_null_obj(cell_set_id))
			{
				ERRverror("DVread_cgns:add_unstr_section",ERR_INFO,
					"can't get cell_set object\n");
				ARRfree(elements);
				ARRfree(elements_tmp);
				return (1);
			}
			/* get cell_set element ncell_sets id */
			if(OMget_array_val(cell_set_id, ncell_sets, &cell_set_el_id,
					OM_OBJ_RW) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:add_unstr_section",ERR_INFO,
					"can't get cell_set_el element object\n");
				ARRfree(elements);
				ARRfree(elements_tmp);
				return (1);
			}
			++ncell_sets;
			if(i < NGON_n)
				stat = add_elem(cell_set_el_id, el_cnt[i], elements_tmp,
					el_cnt[i]*avs_npe, avs_el_name[avs_type]);
			else
				stat = add_ngon_elem(cell_set_el_id, el_cnt[i], elements_tmp,
					avs_npe);
			if(stat)
			{
				ARRfree(elements);
				ARRfree(elements_tmp);
				return (1);
			}
			ARRfree(elements_tmp);
		}
   }
	ARRfree(elements);
	return (0);
}

int
add_unstructured_sections(OMobj_id DVread_cgns_id, OMobj_id zone_id,
	char *file_name, int base, int zone)
{
	int	f_indx;
	OMobj_id	sect_info_id, sect_info_el_id;
	int	i, nSections;
	OMobj_id	mb_id, fld_id, fld_el_id;
	int	nblocks;
	ElementType_t el_type;
	int	start, end, parent_flag;
	int	stat;
	char	*cg_err_str;
	char	stat_str[100];
	int	stop;

	/* find the Multi_Block_Unstr object */
	mb_id = OMfind_subobj(DVread_cgns_id, OMstr_to_name("Multi_Block_Unstr"),
		OM_OBJ_RW);
	if(OMis_null_obj(mb_id))
	{
		ERRverror("DVread_cgns:add_unstructured_sections",ERR_INFO,
			"can't locate Multi_Block_Unstr object\n");
		return (1);
	}
	/* get nblocks value */
	if(OMget_name_int_val(mb_id, OMstr_to_name("nblocks"),
		&nblocks) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_unstructured_sections",ERR_INFO,
			"can't get nblocks value\n");
		return (1);
	}
	/* find the fields object */
	fld_id = OMfind_subobj(mb_id, OMstr_to_name("fields"),
		OM_OBJ_RW);
	if(OMis_null_obj(fld_id))
	{
		ERRverror("DVread_cgns:add_unstructured_sections",ERR_INFO,
			"can't locate fields object\n");
		return (1);
	}
	if(OMget_array_val(fld_id, nblocks-1, &fld_el_id, OM_OBJ_RW)
		!= OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_unstructured_sections",ERR_INFO,
			"can't get fld_el_id element object\n");
		return (0);
	}
	/* get nSection value */
	if(OMget_name_int_val(zone_id, OMstr_to_name("nSection"),
		&nSections) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_unstructured_sections",ERR_INFO,
			"can't get nSection value\n");
		return (1);
	}
	/* find the sect_info object */
	sect_info_id = OMfind_subobj(zone_id, OMstr_to_name("sect_info"),
		OM_OBJ_RW);
	if(OMis_null_obj(sect_info_id))
	{
		ERRverror("DVread_cgns:add_unstructured_sections",ERR_INFO,
			"can't locate sect_info object\n");
		return (1);
	}
	/* CGNS open file */
	if(cg_open(file_name, MODE_READ, &f_indx))
	{
		OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
			cg_get_error());
		return (1);
	}
	for(i=0;i<nSections;++i)
	{

		sprintf(stat_str,"rd_cgns: zone %d cells\nsection: %d",zone,i+1);
		OMstatus_check(i*100/nSections+1,stat_str,&stop);
		if(stop) return (1);
	
		/* get obj_id of section element of sect_info object */
		if(OMget_array_val(sect_info_id, i, &sect_info_el_id, OM_OBJ_RD)
			!= OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_unstructured_sections",ERR_INFO,
				"can't get sect_info element object\n");
			cg_close(f_indx);
			return (0);
		}
		/* get ElementType value */
		if(OMget_name_int_val(sect_info_el_id, OMstr_to_name("ElementType"),
			&el_type) != OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_unstructured_sections",ERR_INFO,
				"can't get ElementType value\n");
			cg_close(f_indx);
			return (0);
		}
		/* get ElementRangeStart value */
		if(OMget_name_int_val(sect_info_el_id, OMstr_to_name("ElementRangeStart"),
			&start) != OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_unstructured_sections",ERR_INFO,
				"can't get ElementRangeStart value\n");
			cg_close(f_indx);
			return (0);
		}
		/* get ElementRangeEnd value */
		if(OMget_name_int_val(sect_info_el_id, OMstr_to_name("ElementRangeEnd"),
			&end) != OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_unstructured_sections",ERR_INFO,
				"can't get ElementRangeEnd value\n");
			cg_close(f_indx);
			return (0);
		}
		/* get ParentFlag value */
		if(OMget_name_int_val(sect_info_el_id, OMstr_to_name("ParentFlag"),
			&parent_flag) != OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_unstructured_sections",ERR_INFO,
				"can't get ParentFlag value\n");
			cg_close(f_indx);
			return (0);
		}
		if(el_type == MIXED) /* MIXED need special treatment */
		{
			stat = add_unstr_mixed_section(fld_el_id, base, zone, i+1,
				parent_flag, f_indx, &cg_err_str);
		}
		else
		if(el_type == ElementTypeNull || el_type == ElementTypeUserDefined)
		{
			ERRverror("DVread_cgns:add_unstructured_sections",ERR_INFO,
				"Section %d: element type Null/UserDefined\n",i+1);
			stat = 0;
		}
		else
		if(el_type >= NGON_MAX) /* this NGON_n not supported */
		{
			ERRverror("DVread_cgns:add_unstructured_sections",ERR_INFO,
				"Section %d: element type NGON_%d not supported (only up to NGON_%d)\n",
				i+1, el_type-NGON_n, NGON_MAX-NGON_n-1);
			stat = 0;
		}
		else
		{
			stat = add_unstr_section(fld_el_id, base, zone, i+1,
				el_type, start, end, parent_flag, f_indx, &cg_err_str);
		}
		if(stat)
		{
			if(cg_err_str != NULL)
				OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"), cg_err_str);
			cg_close(f_indx);
			return (1);
		}
	}

	sprintf(stat_str,"rd_cgns: zone %d cells\nsection: %d",zone,i);
	OMstatus_check(100.1,stat_str,&stop);
	if(stop) return (1);
	
	cg_close(f_indx);
	return (0);
}

/* read vertex data, fill-in FLD */
/* returns 0 if all OK, 1 otherwise */
/* sets cg_err_str to error string if cgns read error */
static int
add_unstr_vrtx_data(OMobj_id fld_id, int base, int zone, int index_dim,
	int *vertex_size, int solution, int f_indx, char **cg_err_str)
{
	OMobj_id	node_data_id, node_data_el_id, values_id;
	int		i, nFields;
	DataType_t	type;
	char		name[CGNS_NAME_MAX_LEN];
	int		i_min, i_max;
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
		ERRverror("DVread_cgns:add_unstr_vrtx_data",ERR_INFO,
			"failed to set nnode_data\n");
		return (1);
	}
	/* find the node_data array object */
	node_data_id = OMfind_subobj(fld_id, OMstr_to_name("node_data"),
		OM_OBJ_RW);
	if(OMis_null_obj(node_data_id))
	{
		ERRverror("DVread_cgns:add_unstr_vrtx_data",ERR_INFO,
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
			ERRverror("DVread_cgns:add_unstr_vrtx_data",ERR_INFO,
				"can't get node_data_el_id element object\n");
			return (0);
		}
		if(OMis_null_obj(node_data_el_id))
		{
			ERRverror("DVread_cgns:add_unstr_vrtx_data",ERR_INFO,
				"can't locate node_data_el_id object\n");
			return (1);
		}
		/* set veclen */
		if(OMset_name_int_val(node_data_el_id, OMstr_to_name("veclen"), 1) !=
			OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_unstr_vrtx_data",ERR_INFO,
				"failed to set veclen\n");
			return (1);
		}
		/* set label */
		if(OMset_name_str_val(node_data_el_id, OMstr_to_name("labels"), name) !=
			OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_unstr_vrtx_data",ERR_INFO,
				"failed to set labels string\n");
			return (1);
		}
		/* prepare the selection range */
		i_min = 1;
		i_max = vertex_size[0];
		/* find the values array object */
		values_id = OMfind_subobj(node_data_el_id, OMstr_to_name("values"),
			OM_OBJ_RW);
		if(OMis_null_obj(values_id))
		{
			ERRverror("DVread_cgns:add_unstr_vrtx_data",ERR_INFO,
				"can't locate values_id object\n");
			return (1);
		}
		/* set values data type */
		switch(type)
		{
			case Integer:
				if(OMset_data_type(values_id, OM_TYPE_INT) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:add_unstr_vrtx_data",
						ERR_INFO,
						"failed to set data type for values\n");
					return (1);
				}
				/* get values array */
				i_arr = (int *)OMret_array_ptr(values_id, OM_GET_ARRAY_RW, &junk,
					&junk);
				if(i_arr == NULL)
				{
					ERRverror("DVread_cgns:add_unstr_vrtx_data",
						ERR_INFO,
						"failed to get values array\n");
					return (1);
				}
				if(cg_field_read(f_indx, base, zone, solution, name, Integer,
					&i_min, &i_max, (void *)(i_arr+1)))
				{
					*cg_err_str = cg_get_error();
					ARRfree(i_arr);
					return (1);
				}
				/* copy to dummy vertex value of 1st vertex */
				i_arr[0] = i_arr[1];
				ARRfree(i_arr);
				break;
			case RealSingle:
			case RealDouble:
				if(OMset_data_type(values_id, OM_TYPE_FLOAT) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:add_unstr_vrtx_data",
						ERR_INFO,
						"failed to set data type for values\n");
					return (1);
				}
				/* get values array */
				f_arr = (float *)OMret_array_ptr(values_id, OM_GET_ARRAY_RW, &size,
					&junk);
				if(f_arr == NULL)
				{
					ERRverror("DVread_cgns:add_unstr_vrtx_data",
						ERR_INFO,
						"failed to get values array\n");
					return (1);
				}
				if(cg_field_read(f_indx, base, zone, solution, name, RealSingle,
					&i_min, &i_max, (void *)(f_arr+1)))
				{
					*cg_err_str = cg_get_error();
					ARRfree(f_arr);
					return (1);
				}
				/* copy to dummy vertex value of 1st vertex */
				f_arr[0] = f_arr[1];
				ARRfree(f_arr);
				break;
		}
	}
	
	return (0);
}

/* reads vertex data for an unstructured field, and sets */
/* the multi-block-unstr structure, returns 0 if all OK, 1 otherwise */
int
add_unstructured_vrtx_data(OMobj_id DVread_cgns_id, char *file_name,
	int base, int zone, int fld_el_indx, int index_dim, int *vertex_size,
	int solution)
{
	int	f_indx;
	OMobj_id	mb_id, fld_id, fld_el_id;
	int	stat;
	char	*cg_err_str;

	/* find the Multi_Block_Unstr object */
	mb_id = OMfind_subobj(DVread_cgns_id, OMstr_to_name("Multi_Block_Unstr"),
		OM_OBJ_RW);
	if(OMis_null_obj(mb_id))
	{
		ERRverror("DVread_cgns:add_unstructured_vrtx_data",ERR_INFO,
			"can't locate Multi_Block_Unstr object\n");
		return (1);
	}
	/* find the fields object */
	fld_id = OMfind_subobj(mb_id, OMstr_to_name("fields"),
		OM_OBJ_RW);
	if(OMis_null_obj(fld_id))
	{
		ERRverror("DVread_cgns:add_unstructured_vrtx_data",ERR_INFO,
			"can't locate fields object\n");
		return (1);
	}
	if(OMget_array_val(fld_id, fld_el_indx, &fld_el_id, OM_OBJ_RW)
		!= OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_unstructured_vrtx_data",ERR_INFO,
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
	stat = add_unstr_vrtx_data(fld_el_id, base, zone, index_dim, vertex_size,
		solution, f_indx, &cg_err_str);
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

static int
add_unstr_mixed_section_cell_data(OMobj_id fld_id, int ncell_data,
	int *current_cell_set, int *elements, int el_data_size,
	DataType_t data_type, void *data)
{
	int		i, j, k, indx;
	int		el_cnt[NGON_MAX];
	int		npe;
	OMobj_id	cell_set_id, cell_set_el_id;
	OMobj_id	cell_data_id, cell_data_el_id;
	OMobj_id	values_id;
	int		junk;
	int		*i_arr;
	float		*f_arr;

	for(i=NODE;i<NGON_MAX;++i)
		el_cnt[i] = 0;
	i = j = 0;
	/* count occurring element types, save count for each type in el_cnt */
	/* if an element (NGON_n higher than allowed) appears set err=1 */
	do
	{
		if(elements[i] < NGON_MAX)
			el_cnt[elements[i]]++;
		/* skip element connection data */
		cg_npe((ElementType_t)elements[i], &npe);
		i += npe+1;
	}
	while(i < el_data_size);
	/* for each elemenet type find the cell_set and fill it with cell data */
	for(i=NODE; i<NGON_MAX; ++i)
	{
		if(el_cnt[i])
		{
			/* find cell_set object */
			cell_set_id = OMfind_subobj(fld_id, OMstr_to_name("cell_set"),
				OM_OBJ_RW);
			if(OMis_null_obj(cell_set_id))
			{
				ERRverror("DVread_cgns:add_unstr_mixed_section_cell_data",ERR_INFO,
					"can't find cell_set object\n");
				return (1);
			}
			/* get the current_cell_set array element object */
			if(OMget_array_val(cell_set_id, *current_cell_set, &cell_set_el_id,
				OM_OBJ_RW) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:add_unstr_mixed_section_cell_data",ERR_INFO,
					"can't get cell_set array element object\n");
				return (1);
			}
			/* find cell_data object */
			cell_data_id = OMfind_subobj(cell_set_el_id, OMstr_to_name("cell_data"),
				OM_OBJ_RW);
			if(OMis_null_obj(cell_data_id))
			{
				ERRverror("DVread_cgns:add_unstr_mixed_section_cell_data",ERR_INFO,
					"can't find cell_data object\n");
				return (1);
			}
			/* get the ncell_data array element object */
			if(OMget_array_val(cell_data_id, ncell_data, &cell_data_el_id,
				OM_OBJ_RW) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:add_unstr_mixed_section_cell_data",ERR_INFO,
					"can't get cell_data array element object\n");
				return (1);
			}
			/* find values object */
			values_id = OMfind_subobj(cell_data_el_id, OMstr_to_name("values"),
				OM_OBJ_RW);
			if(OMis_null_obj(values_id))
			{
				ERRverror("DVread_cgns:add_unstr_mixed_section_cell_data",ERR_INFO,
					"can't find values object\n");
				return (1);
			}
			/* get values array */
			if(data_type == Integer)
			{
				i_arr = (int *)OMret_array_ptr(values_id, OM_GET_ARRAY_RW, &junk,
					&junk);
				indx = 0;
				k = 0;
				for(j=0;j<el_data_size;)
				{
					cg_npe((ElementType_t)elements[j], &npe);
					if((int)elements[j] == i)
					{
						i_arr[indx++] = ((int *)data)[k];
					}
					/* skip element connection data */
					j += npe+1;
					++k;
				}
				ARRfree(i_arr);
			}
			else
			{
				f_arr = (float *)OMret_array_ptr(values_id, OM_GET_ARRAY_RW, &junk,
					&junk);
				indx = 0;
				k = 0;
				for(j=0;j<el_data_size;)
				{
					cg_npe((ElementType_t)elements[j], &npe);
					if((int)elements[j] == i)
					{
						f_arr[indx++] = ((float *)data)[k];
					}
					/* skip element connection data */
					j += npe+1;
					++k;
				}
				ARRfree(f_arr);
			}
			++(*current_cell_set);
		}
	}

	return (0);
}

static int
add_unstr_section_cell_data(OMobj_id fld_id, int ncell_data,
	int current_cell_set, ElementType_t el_type, int *elements, int el_data_size,
	DataType_t data_type, void *data)
{
	int		j, indx, npe;
	OMobj_id	cell_set_id, cell_set_el_id;
	OMobj_id	cell_data_id, cell_data_el_id;
	OMobj_id	values_id;
	int		junk;
	int		*i_arr;
	float		*f_arr;

	/* find cell_set object */
	cell_set_id = OMfind_subobj(fld_id, OMstr_to_name("cell_set"),
		OM_OBJ_RW);
	if(OMis_null_obj(cell_set_id))
	{
		ERRverror("DVread_cgns:add_unstr_section_cell_data",ERR_INFO,
			"can't find cell_set object\n");
		return (1);
	}
	/* get the current_cell_set array element object */
	if(OMget_array_val(cell_set_id, current_cell_set, &cell_set_el_id,
		OM_OBJ_RW) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_unstr_section_cell_data",ERR_INFO,
			"can't get cell_set array element object\n");
		return (1);
	}
	/* find cell_data object */
	cell_data_id = OMfind_subobj(cell_set_el_id, OMstr_to_name("cell_data"),
		OM_OBJ_RW);
	if(OMis_null_obj(cell_data_id))
	{
		ERRverror("DVread_cgns:add_unstr_section_cell_data",ERR_INFO,
			"can't find cell_data object\n");
		return (1);
	}
	/* get the ncell_data array element object */
	if(OMget_array_val(cell_data_id, ncell_data, &cell_data_el_id,
		OM_OBJ_RW) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_unstr_section_cell_data",ERR_INFO,
			"can't get cell_data array element object\n");
		return (1);
	}
	/* find values object */
	values_id = OMfind_subobj(cell_data_el_id, OMstr_to_name("values"),
		OM_OBJ_RW);
	if(OMis_null_obj(values_id))
	{
		ERRverror("DVread_cgns:add_unstr_section_cell_data",ERR_INFO,
			"can't find values object\n");
		return (1);
	}
	cg_npe(el_type, &npe); /* no of connection data for this elemenet type */
	/* get values array */
	if(data_type == Integer)
	{
		i_arr = (int *)OMret_array_ptr(values_id, OM_GET_ARRAY_RW, &junk,
			&junk);
		indx = 0;
		for(j=0;j<el_data_size;)
		{
			i_arr[indx] = ((int *)data)[indx];
			++indx;
			j += npe+1;
		}
		ARRfree(i_arr);
	}
	else
	{
		f_arr = (float *)OMret_array_ptr(values_id, OM_GET_ARRAY_RW, &junk,
			&junk);
		indx = 0;
		for(j=0;j<el_data_size;)
		{
			f_arr[indx] = ((float *)data)[indx];
			++indx;
			j += npe+1;
		}
		ARRfree(f_arr);
	}
	return (0);
}

/* read cell data, fill-in FLD */
/* returns 0 if all OK, 1 otherwise */
/* sets cg_err_str to error string if cgns read error */
static int
add_unstr_cell_data(OMobj_id sect_info_id, int nSections, OMobj_id fld_id,
	int base, int zone, int *vertex_size, int solution,
	int f_indx, char **cg_err_str)
{
	int		current_cell_set;
	int		ncell_sets;
	OMobj_id	cell_set_id, cell_set_el_id, cell_data_id, cell_data_el_id;
	OMobj_id	sect_info_el_id;
	OMobj_id	values_id;
	int		i, nFields;
	int		j;
	int		stat;
	DataType_t	data_type;
	char		name[CGNS_NAME_MAX_LEN];
	ElementType_t el_type;
	int		i_min, i_max;
	void		*data;
	int		el_data_size;
	int		*elements;
	char		stat_str[100];
	int		stop;

	*cg_err_str = NULL;
	if(cg_nfields(f_indx, base, zone, solution, &nFields))
	{
		*cg_err_str = cg_get_error();
		return (1);
	}
	/* each cell_set has nFields cell_data */
	if(OMget_name_int_val(fld_id, OMstr_to_name("ncell_sets"), &ncell_sets) !=
		OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_unstr_cell_data",ERR_INFO,
			"can't get ncell_sets value\n");
		return (1);
	}
	/* find the cell_set object */
	cell_set_id = OMfind_subobj(fld_id, OMstr_to_name("cell_set"),
		OM_OBJ_RW);
	if(OMis_null_obj(cell_set_id))
	{
		ERRverror("DVread_cgns:add_unst_cell_data",ERR_INFO,
			"can't locate cell_set object\n");
		return (1);
	}
	
	/* each cell_set has nFields ncell_data values */
	for(i=0; i<ncell_sets; ++i)
	{
		/* get cell_set array subobject id*/
		if(OMget_array_val(cell_set_id, i, &cell_set_el_id, OM_OBJ_RW)
			!= OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_unst_cell_data",ERR_INFO,
				"can't locate cell_set array element object\n");
			return (1);
		}
		/* set ncell_data */
		if(OMset_name_int_val(cell_set_el_id, OMstr_to_name("ncell_data"),
			nFields) != OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:add_unst_cell_data",ERR_INFO,
				"can't set ncell_data\n");
			return (1);
		}
		cell_data_id = OMfind_subobj(cell_set_el_id, OMstr_to_name("cell_data"),
			OM_OBJ_RW);
		if(OMis_null_obj(cell_data_id))
		{
			ERRverror("DVread_cgns:add_unst_cell_data",ERR_INFO,
				"can't locate cell_data object\n");
			return (1);
		}
		for(j=0; j<nFields; ++j)
		{
			/* get field info */
			if(cg_field_info(f_indx, base, zone, solution, j+1, &data_type, name))
			{
				*cg_err_str = cg_get_error();
				return (1);
			}
			if(data_type != Integer && data_type !=RealSingle)
				continue;
			if(OMget_array_val(cell_data_id, j, &cell_data_el_id, OM_OBJ_RW)
				!= OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:add_unst_cell_data",ERR_INFO,
					"can't locate cell_data array element object\n");
				return (1);
			}
			/* set cell_data element's veclen and label */
			if(OMset_name_int_val(cell_data_el_id, OMstr_to_name("veclen"), 1) !=
				OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:add_unst_cell_data",ERR_INFO,
					"can't set veclen value\n");
				return (1);
			}
			if(OMset_name_str_val(cell_data_el_id, OMstr_to_name("labels"), name) !=
				OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:add_unst_cell_data",ERR_INFO,
					"can't set labels value\n");
				return (1);
			}
			if(data_type == RealDouble)
				data_type = RealSingle;
			/* find values object */
			values_id = OMfind_subobj(cell_data_el_id, OMstr_to_name("values"),
				OM_OBJ_RW);
			if(OMis_null_obj(values_id))
			{
				ERRverror("DVread_cgns:add_unst_cell_data",ERR_INFO,
					"can't find values object\n");
				return (1);
			}
			/* set type of values */
			if(data_type == Integer)
			{
				if(OMset_data_type(values_id, OM_TYPE_INT) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:add_unst_cell_data",ERR_INFO,
						"can't set data type for values object\n");
					return (1);
				}
			}
			else
			{
				if(OMset_data_type(values_id, OM_TYPE_FLOAT) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:add_unst_cell_data",ERR_INFO,
						"can't set data type for values object\n");
					return (1);
				}
			}
		}
	}
	for(i=0; i<nFields; ++i)
	{
		/* get field info */
		if(cg_field_info(f_indx, base, zone, solution, i+1, &data_type, name))
		{
			*cg_err_str = cg_get_error();
			return (1);
		}
		if(data_type == RealDouble)
			data_type = RealSingle;
		if(data_type != Integer && data_type != RealSingle)
			continue;
		current_cell_set = 0;

		for(j=0; j<nSections; ++j)
		{
		
			sprintf(stat_str,"rd_cgns: zone %d data\nfield %d:section %d",zone,i+1,j+1);
			OMstatus_check(i*100/nFields+1,stat_str,&stop);
			if(stop) return (1);
	
			/* find sect_info array element id */
			if(OMget_array_val(sect_info_id, j, &sect_info_el_id, OM_OBJ_RD)
				!= OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:add_unstr_cell_data",ERR_INFO,
					"can't get sect_info array element object\n");
				return (1);
			}
			/* get element type */
			if(OMget_name_int_val(sect_info_el_id, OMstr_to_name("ElementType"),
				&el_type) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:add_unstr_cell_data",ERR_INFO,
					"can't get ElementType value\n");
				return (1);
			}
			/* elements of interest only */
			if(el_type < NODE || el_type >= NGON_MAX)
				continue;
			/* get element range */
			if(OMget_name_int_val(sect_info_el_id, OMstr_to_name("ElementRangeStart"),
				&i_min) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:add_unstr_cell_data",ERR_INFO,
					"can't get ElementRangeStart value\n");
				return (1);
			}
			if(OMget_name_int_val(sect_info_el_id, OMstr_to_name("ElementRangeEnd"),
				&i_max) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:add_unstr_cell_data",ERR_INFO,
					"can't get ElementRangeEnd value\n");
				return (1);
			}
			/* alloc mem for cell data in this range */
			if(data_type == Integer)
				data = (void *)ARRalloc(NULL, DTYPE_INT, i_max-i_min+1, NULL);
			else
				data = (void *)ARRalloc(NULL, DTYPE_FLOAT, i_max-i_min+1, NULL);
			if(data == NULL)
			{
				ERRverror("DVread_cgns:add_unstr_cell_data",ERR_INFO,
					"can't allocate memeory for cell data\n");
				return (1);
			}
			/* read cell data in range */
			if(cg_field_read(f_indx, base, zone, solution, name, data_type,
				&i_min, &i_max, data))
			{
				*cg_err_str = cg_get_error();
				ARRfree(data);
				return (1);
			}
			/* get element info of section */
			if(cg_ElementDataSize(f_indx, base, zone, j+1, &el_data_size))
			{
				*cg_err_str = cg_get_error();
				ERRverror("DVread_cgns:add_unstr_cell_data",ERR_INFO,
					"failed to get CGNS ElementDataSize\n");
				ARRfree(data);
				return (1);
			}
			elements = ARRalloc(NULL, DTYPE_INT, el_data_size, NULL);
			if(elements == NULL)
			{
				ERRverror("DVread_cgns:add_unstr_cell_data",ERR_INFO,
					"failed alloc memory for Elements\n");
				ARRfree(data);
				return (1);
			}
			if(cg_elements_read(f_indx, base, zone, j+1, elements, NULL))
			{
				*cg_err_str = cg_get_error();
				ERRverror("DVread_cgns:add_unstr_cell_data",ERR_INFO,
					"failed to read CGNS Elements\n");
				ARRfree(elements);
				ARRfree(data);
				return (1);
			}
			/* MIXED el type needs special handling */
			if(el_type == MIXED)
			{
				stat = add_unstr_mixed_section_cell_data(fld_id, i,
					&current_cell_set, elements, el_data_size, data_type, data);
			}
			else
			{
				stat = add_unstr_section_cell_data(fld_id, i,
					current_cell_set, el_type, elements, el_data_size, data_type, data);
				++current_cell_set;
			}
			ARRfree(data);
			ARRfree(elements);
			if(stat)
				return (1);
		}
	}
	
	return (0);
}

/* reads cell data for an unstructured field, and sets */
/* the multi-block-unstr structure, returns 0 if all OK, 1 otherwise */
int
add_unstructured_cell_data(OMobj_id DVread_cgns_id, OMobj_id zone_id,
	char *file_name, int base, int zone, int fld_el_indx, int *vertex_size,
	int solution)
{
	int	f_indx;
	OMobj_id	mb_id, fld_id, fld_el_id;
	OMobj_id	sect_info_id;
	int	nSections;
	int	stat;
	char	*cg_err_str;

	/* find the Multi_Block_Unstr object */
	mb_id = OMfind_subobj(DVread_cgns_id, OMstr_to_name("Multi_Block_Unstr"),
		OM_OBJ_RW);
	if(OMis_null_obj(mb_id))
	{
		ERRverror("DVread_cgns:add_unstructured_cell_data",ERR_INFO,
			"can't locate Multi_Block_Unstr object\n");
		return (1);
	}
	/* find the fields object */
	fld_id = OMfind_subobj(mb_id, OMstr_to_name("fields"),
		OM_OBJ_RW);
	if(OMis_null_obj(fld_id))
	{
		ERRverror("DVread_cgns:add_unstructured_cell_data",ERR_INFO,
			"can't locate fields object\n");
		return (1);
	}
	if(OMget_array_val(fld_id, fld_el_indx, &fld_el_id, OM_OBJ_RW)
		!= OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_unstructured_cell_data",ERR_INFO,
			"can't get fld_el_id element object\n");
		return (0);
	}
	/* get nSections */
	if(OMget_name_int_val(zone_id, OMstr_to_name("nSection"), &nSections) !=
		OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:add_unstructured_cell_data",ERR_INFO,
			"can't get nSection value\n");
		return (0);
	}
	/* get sect_info object id */
	sect_info_id = OMfind_subobj(zone_id, OMstr_to_name("sect_info"), OM_OBJ_RD);
	if(OMis_null_obj(sect_info_id))
	{
		ERRverror("DVread_cgns:add_unstructured_cell_data",ERR_INFO,
			"can't locate sect_info object\n");
		return (1);
	}
	/* CGNS open file */
	if(cg_open(file_name, MODE_READ, &f_indx))
	{
		OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
			cg_get_error());
		return (1);
	}
	stat = add_unstr_cell_data(sect_info_id, nSections, fld_el_id, base, zone,
		vertex_size, solution, f_indx, &cg_err_str);
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


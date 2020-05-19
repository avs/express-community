#include "dv_cgns.h"
#include <string.h>

/* return 1 if all OK, 0 otherwise */
/* scan the Bases info in the f_indx CGNS file, fillin DVread_cgns_id */
static int
scan_bases(OMobj_id DVread_cgns_id, int f_indx)
{
	OMobj_id	nBase_id, base_info_id, base_info_el_id;
	int	nBases;
	int	i;
	char	name[CGNS_NAME_MAX_LEN];
	int	cell_dim, phys_dim;

	/* CGNS get number of Bases */
	if(cg_nbases(f_indx, &nBases))
	{
		OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
			cg_get_error());
		return (0);
	}
	if(!nBases) /* if 0 Bases - possible ? */
	{
		return (0);
	}
	/* find obj_id of the nBase */
	nBase_id = OMlookup_subobj(DVread_cgns_id, OMstr_to_name("nBase"),
		OM_OBJ_RW);
	if(OMis_null_obj(nBase_id))
	{
		ERRverror("DVread_cgns:scan_bases",ERR_INFO,
			"can't locate nBase subobject\n");
		return (0);
	}
	/* reset value of nBase object */
	if(OMset_int_val(nBase_id, 0) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:scan_bases",ERR_INFO,
			"can't set nBase object\n");
		return (0);
	}
	/* find obj_id of the base_info */
	base_info_id = OMlookup_subobj(DVread_cgns_id, OMstr_to_name("base_info"),
		OM_OBJ_RW);
	if(OMis_null_obj(base_info_id))
	{
		ERRverror("DVread_cgns:scan_bases",ERR_INFO,
			"can't locate base_info subobject\n");
		return (0);
	}
	/* change dimentionality of base_info object */
	if(OMset_array_size(base_info_id, nBases) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:scan_bases",ERR_INFO,
			"can't change dimentionality of base_info object\n");
		return (0);
	}
	/* CGNS get Bases names - fill out the base_info object */
	for(i=0;i<nBases;++i)
	{
		/* CGNS get Base no i+1 name */
		if(cg_base_read(f_indx, i+1, name, &cell_dim, &phys_dim))
		{
			OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
				cg_get_error());
			return (0);
		}
		/* get obj_id of array element from base_options array */
		if(OMget_array_val(base_info_id, i, &base_info_el_id, OM_OBJ_RW) !=
			OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:scan_bases",ERR_INFO,
				"can't access array element of base_info object\n");
			return (0);
		}
		/* set call and phys dims in base_options subarray element */
		if(OMset_name_int_val(base_info_el_id, OMstr_to_name("cell_dim"),
			cell_dim) != OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:scan_bases",ERR_INFO,
				"can't set cell_dim of base_info array object\n");
			return (0);
		}
		if(OMset_name_int_val(base_info_el_id, OMstr_to_name("phys_dim"),
			phys_dim) != OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:scan_bases",ERR_INFO,
				"can't set phys_dim of base_info array object\n");
			return (0);
		}
		/* set label to name in base_options subarray element */
		if(OMset_name_str_val(base_info_el_id, OMstr_to_name("base_name"), name)
			!= OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:scan_bases",ERR_INFO,
				"can't set name of base_info array object\n");
			return (0);
		}
	}
	/* set correct value of nBase object */
	if(OMset_int_val(nBase_id, nBases) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:scan_bases",ERR_INFO,
			"can't set nBase object\n");
		return (0);
	}
	return (1);
}

static int
scan_zones(OMobj_id DVread_cgns_id, int f_indx, int current_base)
{
	OMobj_id	nZone_id, zone_info_id, zone_info_el_id;
	OMobj_id	sol_info_id, sol_info_el_id;
	int	nZones, nSols, nCoords;
	int	i,j;
	char	name[CGNS_NAME_MAX_LEN];
	char	zone_name[CGNS_NAME_MAX_LEN];
	int	index_dim;
	int	size_data[9];
	int	rind[6];
	ZoneType_t	zone_type;
	char	zone_label[CGNS_NAME_MAX_LEN+3+18];
	char	zone_info_label[100];
	GridLocation_t grid_loc_tmp;
	
	/* gather all info regarding zones */
	/* CGNS get number of Zones */
	if(cg_nzones(f_indx, current_base, &nZones))
	{
		OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
			cg_get_error());
		return (0);
	}
	if(!nZones) /* if 0 Zones - possible ? */
	{
		ERRverror("DVread_cgns:scan_zones",ERR_INFO,
			"cg_nzones(f_indx,%d,&nZones) reports 0 zones!\n",
			current_base);
		return (0);
	}
	/* find obj_id of the nZone */
	nZone_id = OMlookup_subobj(DVread_cgns_id, OMstr_to_name("nZone"),
		OM_OBJ_RW);
	if(OMis_null_obj(nZone_id))
	{
		ERRverror("DVread_cgns:scan_zones",ERR_INFO,
			"can't locate nZone subobject\n");
		return (0);
	}
	/* reset value of nZone object */
	if(OMset_int_val(nZone_id, 0) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:scan_zones",ERR_INFO,
			"can't set nZone object\n");
		return (0);
	}
	/* find obj_id of the zone_info */
	zone_info_id = OMlookup_subobj(DVread_cgns_id, OMstr_to_name("zone_info"),
		OM_OBJ_RW);
	if(OMis_null_obj(zone_info_id))
	{
		ERRverror("DVread_cgns:scan_zones",ERR_INFO,
			"can't locate zone_info subobject\n");
		return (0);
	}
	/* change dimentionality of zone_info object */
	if(OMset_array_size(zone_info_id, nZones) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:scan_zones",ERR_INFO,
			"can't change dimentionality of zone_info object\n");
		return (0);
	}
	/* CGNS get Zone names - fill out the zone_info object */
	for(i=0;i<nZones;++i)
	{
		/* CGNS get Zone no i+1 type */
		if(cg_zone_type(f_indx, current_base, i+1, &zone_type))
		{
			OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
				cg_get_error());
			return (0);
		}
		/* get obj_id of array element from zone_info array */
		if(OMget_array_val(zone_info_id, i, &zone_info_el_id, OM_OBJ_RW) !=
			OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:scan_zones",ERR_INFO,
				"can't access array element of zone_info object\n");
			return (0);
		}

		/* CGNS get Zone no i+1 name */
		if(cg_zone_read(f_indx, current_base, i+1, zone_name, size_data))
		{
			OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
				cg_get_error());
			return (0);
		}
		/* set label to name in zone_options subarray element */
		if(OMset_name_str_val(zone_info_el_id, OMstr_to_name("zone_name"), zone_name)
			!= OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:scan_zones",ERR_INFO,
				"can't set name of zone_info array object\n");
			return (0);
		}
		/* set zone_type in zone_options subarray element */
		if(OMset_name_int_val(zone_info_el_id, OMstr_to_name("zone_type"), zone_type) !=
			OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:scan_zones",ERR_INFO,
				"can't set zone_type of zone_info array object\n");
			return (0);
		}
		/* set index dimention */
		{
			OMobj_id cell_dim_id;
			char path[100];
			int cell_dim;

			switch(zone_type)
			{
				case Structured:
					/* index_dim for structured == cell_dim */
					sprintf(path,"BaseInfo.base_info[%d].cell_dim",current_base-1);
					cell_dim_id = OMfind_str_subobj(DVread_cgns_id, path, OM_OBJ_RW);
					if(OMis_null_obj(cell_dim_id))
					{
						ERRverror("DVread_cgns:scan_zones",ERR_INFO,
							"can't get value of cell_dim object\n");
						return (0);
					}
					if(OMget_int_val(cell_dim_id, &cell_dim) != OM_STAT_SUCCESS)
					{
						ERRverror("DVread_cgns:scan_zones",ERR_INFO,
							"can't get value of cell_dim object\n");
						return (0);
					}
					index_dim = cell_dim;
					sprintf(zone_label,"%s : Structured",zone_name);
					break;
				case Unstructured:
					index_dim = 1;
					sprintf(zone_label,"%s : Unstructured",zone_name);
					break;
				case ZoneTypeUserDefined:
					index_dim = 0;
					sprintf(zone_label,"%s : UserDefined",zone_name);
					break;
				case ZoneTypeNull:
					index_dim = 0;
					sprintf(zone_label,"%s : Null",zone_name);
					break;
			}
		}
		/* set index_dim */
		if(OMset_name_int_val(zone_info_el_id, OMstr_to_name("index_dim"),
				index_dim)
			!= OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:scan_zones",ERR_INFO,
				"can't set value of index_dim object\n");
			return (0);
		}
		/* if zone_type is Null or UserDefined, set cartesian to 0 */
		if(index_dim == 0)
		{
			if(OMset_name_int_val(zone_info_el_id, OMstr_to_name("cartesian"),
					0) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't set value of cartesian object\n");
				return (0);
			}
			continue; /* continue with other zones */
		}
		/* check if Cartesian coordinates */
		{
			int nCoords;
			DataType_t data_type;
			int n, xyz_cnt, cartesian, *XYZ;
			int size, type;
			OMobj_id phys_dim_id;
			char path[100];
			int phys_dim;

			if(cg_ncoords(f_indx, current_base, i+1, &nCoords))
			{
				OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
					cg_get_error());
				return (0);
			}
			if(nCoords < 1) /* if 0 Coordinates - possible ? */
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"cg_ncoords(f_indx,%d,%d,&nCoords) reports 0 coordinates!\n",
					current_base, i+1);
				return (0);
			}
			/* get XYZ array object */
			XYZ = (int *)OMret_name_array_ptr(zone_info_el_id,
				OMstr_to_name("XYZ"), OM_GET_ARRAY_WR, &size, &type);
			if(XYZ == NULL)
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't get array ptr of XYZ array object\n");
				return (0);
			}
			XYZ[0] = XYZ[1] = XYZ[2] = 0;
			xyz_cnt = 0;
			for(n=1; n<=nCoords; ++n)
			{
				/* get name of n coordinate */
				if(cg_coord_info(f_indx, current_base, i+1, n, &data_type, name))
				{
					OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
						cg_get_error());
					ARRfree(XYZ);
					return (0);
				}
				if(!strcmp(name,"CoordinateX"))
				{
					XYZ[0] = 1;
					++xyz_cnt;
				}
				else
				if(!strcmp(name,"CoordinateY"))
				{
					XYZ[1] = 1;
					++xyz_cnt;
				}
				else
				if(!strcmp(name,"CoordinateZ"))
				{
					XYZ[2] = 1;
					++xyz_cnt;
				}
			}
			{
				int tmp;

				tmp = XYZ[0]*100 + XYZ[1]*10 + XYZ[2];
				switch(tmp)
				{
					case 1: strcat(zone_label," [Z]"); break;
					case 10: strcat(zone_label," [Y]"); break;
					case 11: strcat(zone_label," [YZ]"); break;
					case 100: strcat(zone_label," [X]"); break;
					case 101: strcat(zone_label," [XZ]"); break;
					case 110: strcat(zone_label," [XY]"); break;
					case 111: strcat(zone_label," [XYZ]"); break;
				}
			}
			ARRfree(XYZ);
			/* get phys_dim */
			sprintf(path,"BaseInfo.base_info[%d].phys_dim",current_base-1);
			phys_dim_id = OMfind_str_subobj(DVread_cgns_id, path, OM_OBJ_RW);
			if(OMis_null_obj(phys_dim_id))
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't get value of phys_dim object\n");
				return (0);
			}
			if(OMget_int_val(phys_dim_id, &phys_dim) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't get value of phys_dim object\n");
				return (0);
			}
			/* did we get coordinate labels XYZ equal to phys_dim ? */
			if(xyz_cnt == phys_dim)
				cartesian = 1;
			else
			{
				sprintf(zone_label,"%s : [Not Cartesian]\n",zone_name);
				cartesian = 0;
			}

			if(OMset_name_int_val(zone_info_el_id, OMstr_to_name("cartesian"),
					cartesian) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't set value of cartesian object\n");
				return (0);
			}
			/* set zone_label */
			if(OMset_name_str_val(zone_info_el_id, OMstr_to_name("zone_label"),
				zone_label) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't set string value of zone_label object\n");
				return (0);
			}
		}
		/* get Rind information for zone i+1 grid */
		{
			int go_stat;

			go_stat = cg_goto(f_indx, current_base, "Zone_t", i+1,
				"GridCoordinates_t", 1, "end");
			switch(go_stat)
			{
				case ERROR:
					OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
						cg_get_error());
					return (0);
				case NODE_NOT_FOUND:
					OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
						"NODE NOT FOUND: while seeking\nfor Rind in GridCoordinates");
					return (0);
				case INCORRECT_PATH:
					OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
						"INCORRECT PATH: while seeking\nfor Rind in GridCoordinates");
					return (0);
			}
			if(cg_rind_read(rind))
			{
				OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
					cg_get_error());
				return (0);
			}
		}

		/* set VertexSize/CellSize/VertexSizeBoundary and Rind objects */
		{
			int *VertexSize, *CellSize, *VertexSizeBoundary, *Rind;
			int size, type;

			VertexSize = (int *)OMret_name_array_ptr(zone_info_el_id,
				OMstr_to_name("VertexSize"), OM_GET_ARRAY_WR, &size, &type);
			if(VertexSize == NULL)
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't get array ptr of VertexSize array object\n");
				return (0);
			}
			CellSize = (int *)OMret_name_array_ptr(zone_info_el_id,
				OMstr_to_name("CellSize"), OM_GET_ARRAY_WR, &size, &type);
			if(CellSize == NULL)
			{
				ARRfree(VertexSize);
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't get array ptr of CellSize array object\n");
				return (0);
			}
			VertexSizeBoundary = (int *)OMret_name_array_ptr(zone_info_el_id,
				OMstr_to_name("VertexSizeBoundary"), OM_GET_ARRAY_WR, &size, &type);
			if(VertexSizeBoundary == NULL)
			{
				ARRfree(VertexSize);
				ARRfree(CellSize);
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't get array ptr of VertexSizeBoundary array object\n");
				return (0);
			}
			Rind = (int *)OMret_name_array_ptr(zone_info_el_id,
				OMstr_to_name("Rind"), OM_GET_ARRAY_WR, &size, &type);
			if(VertexSizeBoundary == NULL)
			{
				ARRfree(VertexSize);
				ARRfree(CellSize);
				ARRfree(VertexSizeBoundary);
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't get array ptr of Rind array object\n");
				return (0);
			}
			switch(index_dim)
			{
				case 3:
					VertexSize[0] = size_data[0];
					VertexSize[1] = size_data[1];
					VertexSize[2] = size_data[2];
					CellSize[0] = size_data[3];
					CellSize[1] = size_data[4];
					CellSize[2] = size_data[5];
					VertexSizeBoundary[0] = size_data[6];
					VertexSizeBoundary[1] = size_data[7];
					VertexSizeBoundary[2] = size_data[8];
					Rind[0] = rind[0];
					Rind[1] = rind[1];
					Rind[2] = rind[2];
					Rind[3] = rind[3];
					Rind[4] = rind[4];
					Rind[5] = rind[5];
					break;
				case 2:
					VertexSize[0] = size_data[0];
					VertexSize[1] = size_data[1];
					CellSize[0] = size_data[2];
					CellSize[1] = size_data[3];
					VertexSizeBoundary[0] = size_data[4];
					VertexSizeBoundary[1] = size_data[5];
					Rind[0] = rind[0];
					Rind[1] = rind[1];
					Rind[2] = rind[2];
					Rind[3] = rind[3];
					break;
				case 1:
					VertexSize[0] = size_data[0];
					CellSize[0] = size_data[1];
					VertexSizeBoundary[0] = size_data[2];
					Rind[0] = rind[0];
					Rind[1] = rind[1];
					break;
			}
			/* set zone info label for Structured (Unstructured will be set with Sections) */
			if(zone_type == Structured)
			{
				switch(index_dim)
				{
					case 3:
						sprintf(zone_info_label, "Vrxt [%d %d %d] Rind [%d %d %d %d %d %d]",
							size_data[0], size_data[1], size_data[2], rind[0], rind[1],
							rind[2], rind[3], rind[4], rind[5]);
						break;
					case 2:
						sprintf(zone_info_label, "Vrxt [%d %d] Rind [%d %d %d %d]",
							size_data[0], size_data[1], rind[0], rind[1], rind[2], rind[3]);
						break;
					case 1:
						sprintf(zone_info_label, "Vrxt [%d] Rind [%d %d]",
							size_data[0], rind[0], rind[1]);
						break;
				}
			}
			ARRfree(VertexSize);
			ARRfree(CellSize);
			ARRfree(VertexSizeBoundary);
			ARRfree(Rind);
		}

		

		/* CGNS get Zone no i+1 number of solutions */
		if(cg_nsols(f_indx, current_base, i+1, &nSols))
		{
			OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
				cg_get_error());
			return (0);
		}
		/* reset value of nSolution */
		if(OMset_name_int_val(zone_info_el_id, OMstr_to_name("nSolution"), 0)
			!= OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:scan_zones",ERR_INFO,
				"can't set value of nSolution object\n");
			return (0);
		}
		/* find obj_id of the sol_info */
		sol_info_id = OMlookup_subobj(zone_info_el_id, OMstr_to_name("sol_info"),
			OM_OBJ_RW);
		if(OMis_null_obj(sol_info_id))
		{
			ERRverror("DVread_cgns:scan_zones",ERR_INFO,
				"can't locate sol_info subobject\n");
			return (0);
		}
		/* change dimentionality of sol_info object */
		if(OMset_array_size(sol_info_id, nSols) != OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:scan_zones",ERR_INFO,
				"can't change dimentionality of sol_info object\n");
			return (0);
		}
		for(j=0;j<nSols;++j)
		{
			/* CGNS get solution name */
			if(cg_sol_info(f_indx, current_base, i+1, j+1, name, &grid_loc_tmp))
			{
				OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
					cg_get_error());
				return (0);
			}
			/* get obj_id of array element from sol_info array */
			if(OMget_array_val(sol_info_id, j, &sol_info_el_id, OM_OBJ_RW) !=
				OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't access array element of sol_info object\n");
				return (0);
			}
			/* set label to zone_type in zone_options subarray element */
			if(OMset_name_str_val(sol_info_el_id, OMstr_to_name("sol_name"), name)
				!= OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't set sol_name of sol_info array object\n");
				return (0);
			}
			/* set GridLocation object */
			if(OMset_name_int_val(sol_info_el_id, OMstr_to_name("GridLocation"),
				grid_loc_tmp) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't set GridLocation of sol_info array object\n");
				return (0);
			}
			/* get Rind information for solution j+1 */
			{
				int go_stat;

				go_stat = cg_goto(f_indx, current_base, "Zone_t", i+1,
					"FlowSolution_t", j+1, "end");
				switch(go_stat)
				{
					case ERROR:
						OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
							cg_get_error());
						return (0);
					case NODE_NOT_FOUND:
						OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
							"NODE NOT FOUND: while seeking\nfor Rind in FlowSolution");
						return (0);
					case INCORRECT_PATH:
						OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
							"INCORRECT PATH: while seeking\nfor Rind in FlowSolution");
						return (0);
				}
				if(cg_rind_read(rind))
				{
					OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
						cg_get_error());
					return (0);
				}
			}
			/* set Rind object for solution j+1 */
			{
				int *Rind;
				int size, type;

				Rind = (int *)OMret_name_array_ptr(sol_info_el_id,
					OMstr_to_name("Rind"), OM_GET_ARRAY_WR, &size, &type);
				if(Rind == NULL)
				{
					ERRverror("DVread_cgns:scan_zones",ERR_INFO,
						"can't get array ptr of Rind array object\n");
					return (0);
				}
				switch(index_dim)
				{
					case 3:
						Rind[0] = rind[0];
						Rind[1] = rind[1];
						Rind[2] = rind[2];
						Rind[3] = rind[3];
						Rind[4] = rind[4];
						Rind[5] = rind[5];
						break;
					case 2:
						Rind[0] = rind[0];
						Rind[1] = rind[1];
						Rind[2] = rind[2];
						Rind[3] = rind[3];
						break;
					case 1:
						Rind[0] = rind[0];
						Rind[1] = rind[1];
						break;
				}
				ARRfree(Rind);
			}
		}

		/* if unstructured, get info about sections */
		if(zone_type == Unstructured)
		{
			int	nSections;
			OMobj_id	sect_info_id, sect_info_el_id;
			ElementType_t	el_type;
			int	start, end, nbndry, parent_flag;

			if(cg_nsections(f_indx, current_base, i+1, &nSections))
			{
				OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
					cg_get_error());
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't read nSections from CGNS\n");
				return (0);
			}
			if(OMset_name_int_val(zone_info_el_id, OMstr_to_name("nSection"),
				nSections) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't set value of nSection object\n");
				return (0);
			}
			/* find obj_id of the sect_info */
			sect_info_id = OMlookup_subobj(zone_info_el_id, OMstr_to_name("sect_info"),
				OM_OBJ_RW);
			if(OMis_null_obj(sect_info_id))
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't locate sect_info subobject\n");
				return (0);
			}
			/* change dimentionality of sect_info object */
			if(OMset_array_size(sect_info_id, nSections) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:scan_zones",ERR_INFO,
					"can't change dimentionality of sect_info object\n");
				return (0);
			}
			for(j=0;j<nSections;++j)
			{
				/* get obj_id of array element from sect_info array */
				if(OMget_array_val(sect_info_id, j, &sect_info_el_id, OM_OBJ_RW) !=
					OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:scan_zones",ERR_INFO,
						"can't access array element of sect_info object\n");
					return (0);
				}
				if(cg_section_read(f_indx, current_base, i+1, j+1, name, &el_type,
					&start, &end, &nbndry, &parent_flag))
				{
					OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"),
						cg_get_error());
					ERRverror("DVread_cgns:scan_zones",ERR_INFO,
						"can't read Section info from CGNS\n");
					return (0);
				}
				/* set label of sect_name in sect_info subarray element */
				if(OMset_name_str_val(sect_info_el_id, OMstr_to_name("sect_name"), name)
					!= OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:scan_zones",ERR_INFO,
						"can't set sect_name of sect_info array object\n");
					return (0);
				}
				/* set ElementType object */
				if(OMset_name_int_val(sect_info_el_id, OMstr_to_name("ElementType"),
					el_type) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:scan_zones",ERR_INFO,
						"can't set value of ElementType object\n");
					return (0);
				}
				/* set ElementRangeStart object */
				if(OMset_name_int_val(sect_info_el_id, OMstr_to_name("ElementRangeStart"),
					start) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:scan_zones",ERR_INFO,
						"can't set value of ElementRangeStart object\n");
					return (0);
				}
				/* set ElementRangeEnd object */
				if(OMset_name_int_val(sect_info_el_id, OMstr_to_name("ElementRangeEnd"),
					end) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:scan_zones",ERR_INFO,
						"can't set value of ElementRangeEnd object\n");
					return (0);
				}
				/* set ElementSizeBoundary object */
				if(OMset_name_int_val(sect_info_el_id, OMstr_to_name("ElementSizeBoundary"),
					nbndry) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:scan_zones",ERR_INFO,
						"can't set value of ElementSizeBoundary object\n");
					return (0);
				}
				/* set ParentFlag object */
				if(OMset_name_int_val(sect_info_el_id, OMstr_to_name("ParentFlag"),
					parent_flag) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:scan_zones",ERR_INFO,
						"can't set value of ParentFlag object\n");
					return (0);
				}
			}
			/* set zone info label for Unstructured */
			sprintf(zone_info_label, "Vrxt [%d] Cell [%d] Sctn [%d]",
				size_data[0], size_data[1], nSections);
		}
		/* set zone_info_label object*/
		if(OMset_name_str_val(zone_info_el_id, OMstr_to_name("zone_info"),
			zone_info_label) != OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:scan_zones",ERR_INFO,
				"can't set string value of zone_info object\n");
			return (0);
		}
	
		/* set true value of nSolution */
		if(OMset_name_int_val(zone_info_el_id, OMstr_to_name("nSolution"),
			nSols) != OM_STAT_SUCCESS)
		{
			ERRverror("DVread_cgns:scan_zones",ERR_INFO,
				"can't set value of nSolution object\n");
			return (0);
		}
	}
	/* set true value of nZone object */
	if(OMset_int_val(nZone_id, nZones) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:scan_zones",ERR_INFO,
			"can't set nZone object\n");
		return (0);
	}
   return(1);
}

static void
reset_zones(OMobj_id DVread_cgns_id)
{
	OMobj_id	nZone_id;

	/* find obj_id of the nZone */
	nZone_id = OMlookup_subobj(DVread_cgns_id, OMstr_to_name("nZone"),
		OM_OBJ_RW);
	if(OMis_null_obj(nZone_id))
	{
		ERRverror("DVread_cgns:reset_zones",ERR_INFO,
			"can't locate nZone subobject\n");
		return;
	}
	/* set value of nZone object */
	if(OMset_int_val(nZone_id, 0) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:reset_zones",ERR_INFO,
			"can't set nZone object\n");
		return;
	}
}

static void
reset_bases(OMobj_id DVread_cgns_id)
{
	OMobj_id	nBase_id;

	/* find obj_id of the nZone */
	nBase_id = OMlookup_subobj(DVread_cgns_id, OMstr_to_name("nBase"),
		OM_OBJ_RW);
	if(OMis_null_obj(nBase_id))
	{
		ERRverror("DVread_cgns:reset_bases",ERR_INFO,
			"can't locate nZone subobject\n");
		return;
	}
	/* set value of nBase object */
	if(OMset_int_val(nBase_id, 0) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:reset_bases",ERR_INFO,
			"can't set nBase object\n");
		return;
	}
}

void
reset_output_FLDs(OMobj_id DVread_cgns_id)
{
	OMobj_id	mb_id, nblocks_id;

	/* structured */
	mb_id = OMfind_subobj(DVread_cgns_id, OMstr_to_name("Multi_Block"),
		OM_OBJ_RW);
	if(OMis_null_obj(mb_id))
	{
		ERRverror("DVread_cgns:reset_output_FLDs",ERR_INFO,
			"can't locate Multi_Block subobject\n");
		return;
	}
	/* find obj_id of the nblocks */
	nblocks_id = OMlookup_subobj(mb_id, OMstr_to_name("nblocks"),
		OM_OBJ_RW);
	if(OMis_null_obj(nblocks_id))
	{
		ERRverror("DVread_cgns:reset_output_FLDs",ERR_INFO,
			"can't locate nblocks subobject\n");
		return;
	}
	/* set value of nblocks object */
	if(OMset_int_val(nblocks_id, 0) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:reset_output_FLDs",ERR_INFO,
			"can't set nblocks object\n");
		return;
	}
	/* unstructured */
	mb_id = OMfind_subobj(DVread_cgns_id, OMstr_to_name("Multi_Block_Unstr"),
		OM_OBJ_RW);
	if(OMis_null_obj(mb_id))
	{
		ERRverror("DVread_cgns:reset_output_FLDs",ERR_INFO,
			"can't locate Multi_Block_Unstr subobject\n");
		return;
	}
	nblocks_id = OMlookup_subobj(mb_id, OMstr_to_name("nblocks"),
		OM_OBJ_RW);
	if(OMis_null_obj(nblocks_id))
	{
		ERRverror("DVread_cgns:reset_output_FLDs",ERR_INFO,
			"can't locate nblocks subobject\n");
		return;
	}
	/* set value of nblocks object */
	if(OMset_int_val(nblocks_id, 0) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:reset_output_FLDs",ERR_INFO,
			"can't set nblocks object\n");
		return;
	}
	/* unstructured */
}

int
base_change_cgns(OMobj_id DVread_cgns_id, OMevent_mask event_mask, int seq_num)
{
	char  *file_name = NULL;
   char  file_name_buf[AVS_PATH_MAX];
	int	f_indx;
	int	current_base;

	reset_output_FLDs(DVread_cgns_id);
   /* Get file_name's value */
   if(OMget_name_str_val(DVread_cgns_id, OMstr_to_name("file_name"), &file_name, OM_OBJ_RD) != OM_STAT_SUCCESS)
	{
      file_name = NULL;
		ERRverror("DVread_cgns:base_change_cgns",ERR_INFO, "can't get filename object\n");
		return (0);
	}
	/* get current_base */
	if(OMget_name_int_val(DVread_cgns_id, OMstr_to_name("CurrentBase"), &current_base) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:base_change_cgns",ERR_INFO, "can't get CurrentBase object\n");
      if (file_name!=NULL) free(file_name);
		return (0);
	}

   /* Use AVS/Express function to map enviroment variables */
   FILEmap_variables(file_name, file_name_buf);

	/* CGNS open file */
	if(cg_open(file_name_buf, MODE_READ, &f_indx))
	{
		OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"), cg_get_error());
      if (file_name!=NULL) free(file_name);
		return (0);
	}
	if(!scan_zones(DVread_cgns_id, f_indx, current_base))
	{
		cg_close(f_indx);
		reset_zones(DVread_cgns_id);
      if (file_name!=NULL) free(file_name);
		return (0);
	}

	/* CGNS file processing went OK */
	OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"), "no CGNS error reported");
	cg_close(f_indx);

   if (file_name!=NULL) free(file_name);

   return(1);
}

int
scan_cgns(OMobj_id DVread_cgns_id, OMevent_mask event_mask, int seq_num)
{
   char  *file_name = NULL;
   char  file_name_buf[AVS_PATH_MAX];
	int	f_indx;

	reset_output_FLDs(DVread_cgns_id);
	reset_bases(DVread_cgns_id);
	reset_zones(DVread_cgns_id);

   /* Get file_name's value */
   if(OMget_name_str_val(DVread_cgns_id, OMstr_to_name("file_name"), &file_name, OM_OBJ_RD) != OM_STAT_SUCCESS)
	{
      file_name = NULL;
		return (0);
	}

   /* Use AVS/Express function to map enviroment variables */
   FILEmap_variables(file_name, file_name_buf);

	/* CGNS open file */
	if(cg_open(file_name_buf, MODE_READ, &f_indx))
	{
		OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"), cg_get_error());
      if (file_name!=NULL) free(file_name);
		return (0);
	}
	if(!scan_bases(DVread_cgns_id, f_indx))
	{
		cg_close(f_indx);
      if (file_name!=NULL) free(file_name);
		return (0);
	}
	if(!scan_zones(DVread_cgns_id, f_indx, 1))
	{
		reset_bases(DVread_cgns_id);
		cg_close(f_indx);
      if (file_name!=NULL) free(file_name);
		return (0);
	}

	/* CGNS file processing went OK */
	OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"), "no CGNS error reported");
	cg_close(f_indx);

   if (file_name!=NULL) free(file_name);
	return (1);
}


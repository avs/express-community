#include "dv_cgns.h"

int
load_cgns(OMobj_id DVread_cgns_id, OMevent_mask event_mask, int seq_num)
{
	char  *file_name = NULL;
   char  file_name_buf[AVS_PATH_MAX];
	int	base;
	int	*curr_zones, nZones;
	int	junk;
	int	zone;
	int	fld_el_indx_str, fld_el_indx_unstr;
	OMobj_id	base_arr_id, base_id;
	int	phys_dim;
	OMobj_id	zone_arr_id, zone_id;
	int	zone_type;
	int	index_dim, *vertex_size;
	int	*Rind, *XYZ;
	int	*curr_solutions, nSolutions;
	OMobj_id	sol_arr_id, sol_id;

	reset_output_FLDs(DVread_cgns_id);
   /* Get file_name's value */
   if(OMget_name_str_val(DVread_cgns_id, OMstr_to_name("file_name"),
		&file_name, OM_OBJ_RD) != OM_STAT_SUCCESS)
	{
      file_name = NULL;
		return (0);
	}

   /* Use AVS/Express function to map enviroment variables */
   FILEmap_variables(file_name, file_name_buf);

	/* get current Base */
	if(OMget_name_int_val(DVread_cgns_id, OMstr_to_name("CurrentBase"),
		&base) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:load_cgns",ERR_INFO,
			"can't get CurrentBase value\n");
      if (file_name!=NULL) free(file_name);
		return (0);
	}
	/* get obj_id of base_info object */
	base_arr_id = OMlookup_subobj(DVread_cgns_id,
		OMstr_to_name("base_info"), OM_OBJ_RD);
	if(OMis_null_obj(base_arr_id))
	{
		ERRverror("DVread_cgns:load_cgns",ERR_INFO,
			"can't locate base_info subobject\n");
      if (file_name!=NULL) free(file_name);
		return (0);
	}
	/* get obj_id of base element of base_info object */
	if(OMget_array_val(base_arr_id, base-1, &base_id, OM_OBJ_RD)
		!= OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:load_cgns",ERR_INFO,
			"can't get base_info element object\n");
      if (file_name!=NULL) free(file_name);
		return (0);
	}
	/* get phys_dim value */
	if(OMget_name_int_val(base_id, OMstr_to_name("phys_dim"),
		&phys_dim) != OM_STAT_SUCCESS)
	{
		ERRverror("DVread_cgns:load_cgns",ERR_INFO,
			"can't get phys_dim value\n");
      if (file_name!=NULL) free(file_name);
		return (0);
	}

	/* get CurrentActiveZones */
	curr_zones = OMret_name_array_ptr(DVread_cgns_id,
		OMstr_to_name("CurrentActiveZones"), OM_GET_ARRAY_RD, &nZones,
		&junk);
	if(curr_zones == NULL)
	{
		ERRverror("DVread_cgns:load_cgns",ERR_INFO,
			"can't get CurrentActiveZones array\n");
      if (file_name!=NULL) free(file_name);
		return (0);
	}
	/* get obj_id of zone_info object */
	zone_arr_id = OMlookup_subobj(DVread_cgns_id,
		OMstr_to_name("zone_info"), OM_OBJ_RD);
	if(OMis_null_obj(zone_arr_id))
	{
		ERRverror("DVread_cgns:load_cgns",ERR_INFO,
			"can't locate zone_info subobject\n");
		ARRfree(curr_zones);
      if (file_name!=NULL) free(file_name);
		return (0);
	}
	fld_el_indx_str = fld_el_indx_unstr = 0;
	/* add each active zone to appropiate Field */
	for(zone=0;zone<nZones;++zone)
	{
		if(curr_zones[zone]) /* is zone active ? */
		{
			/* get obj_id of zone element of zone_info object */
			if(OMget_array_val(zone_arr_id, zone, &zone_id, OM_OBJ_RD)
				!= OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:load_cgns",ERR_INFO,
					"can't get zone_info element object\n");
				ARRfree(curr_zones);
            if (file_name!=NULL) free(file_name);
				return (0);
			}
			/* get type of zone */
			if(OMget_name_int_val(zone_id, OMstr_to_name("zone_type"),
				&zone_type) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:load_cgns",ERR_INFO,
					"can't get zone_type value\n");
				ARRfree(curr_zones);
            if (file_name!=NULL) free(file_name);
				return (0);
			}
			/* get index_dim */
			if(OMget_name_int_val(zone_id, OMstr_to_name("index_dim"),
				&index_dim) != OM_STAT_SUCCESS)
			{
				ERRverror("DVread_cgns:load_cgns",ERR_INFO,
					"can't get index_dim value\n");
				ARRfree(curr_zones);
            if (file_name!=NULL) free(file_name);
				return (0);
			}
			/* get VertexSize array */
			vertex_size = OMret_name_array_ptr(zone_id,
				OMstr_to_name("VertexSize"), OM_GET_ARRAY_RD, &junk, &junk);
			if(vertex_size == NULL)
			{
				ERRverror("DVread_cgns:load_cgns",ERR_INFO,
					"can't get VertexSize array\n");
				ARRfree(curr_zones);
            if (file_name!=NULL) free(file_name);
				return (0);
			}
			/* status bar range for each zone */
			OMpush_status_range(zone*100/nZones,(zone+1)*100/nZones);

			/* depending if structured on unstructured */
			if(zone_type == Unstructured)
			{
				/* unstructured */
				/* get XYZ */
				XYZ = OMret_name_array_ptr(zone_id,
					OMstr_to_name("XYZ"), OM_GET_ARRAY_RD, &junk, &junk);
				if(XYZ == NULL)
				{
					ERRverror("DVread_cgns:load_cgns",ERR_INFO,
						"can't get XYZ array\n");
					ARRfree(curr_zones);
					ARRfree(vertex_size);
               if (file_name!=NULL) free(file_name);
					OMpop_status_range();
					return (0);
				}
				/* status bar range for coordinates */
				OMpush_status_range(0.0,30.0);
				if(add_unstructured_coords(DVread_cgns_id, file_name_buf,
						base, zone+1, phys_dim, vertex_size, XYZ))
				{
					ARRfree(curr_zones);
					ARRfree(vertex_size);
					ARRfree(XYZ);
               if (file_name!=NULL) free(file_name);
					reset_output_FLDs(DVread_cgns_id);
					OMpop_status_range();
					OMpop_status_range();
					return (0);
				}
				OMpop_status_range();
				ARRfree(XYZ);
				/* status bar range for sections */
				OMpush_status_range(30.0,45.0);
				/* create zone cell_sets basing on sections */
				if(add_unstructured_sections(DVread_cgns_id, zone_id, file_name_buf,
					base, zone+1))
				{
					ARRfree(curr_zones);
					ARRfree(vertex_size);
               if (file_name!=NULL) free(file_name);
					reset_output_FLDs(DVread_cgns_id);
					OMpop_status_range();
					OMpop_status_range();
					return (0);
				}
				OMpop_status_range();
				/* now set solution data */
				if(OMget_name_int_val(zone_id, OMstr_to_name("nSolution"),
					&nSolutions) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:load_cgns",ERR_INFO,
						"can't get nSolution value\n");
					ARRfree(curr_zones);
					ARRfree(vertex_size);
               if (file_name!=NULL) free(file_name);
					OMpop_status_range();
					return (0);
				}
				if(nSolutions)
				{
					/* get CurrentSolutions */
					curr_solutions = OMret_name_array_ptr(DVread_cgns_id,
						OMstr_to_name("CurrentSolutions"), OM_GET_ARRAY_RD, &junk,
						&junk);
					if(curr_solutions == NULL)
					{
						ERRverror("DVread_cgns:load_cgns",ERR_INFO,
							"can't get CurrentSolutions array\n");
						ARRfree(curr_zones);
						ARRfree(vertex_size);
                  if (file_name!=NULL) free(file_name);
						OMpop_status_range();
						return (0);
					}
					/* get obj_id of sol_info object */
					sol_arr_id = OMlookup_subobj(zone_id,
						OMstr_to_name("sol_info"), OM_OBJ_RD);
					if(OMis_null_obj(sol_arr_id))
					{
						ERRverror("DVread_cgns:load_cgns",ERR_INFO,
							"can't locate sol_info subobject\n");
						ARRfree(curr_zones);
						ARRfree(vertex_size);
						ARRfree(curr_solutions);
                  if (file_name!=NULL) free(file_name);
						OMpop_status_range();
						return (0);
					}
					/* get obj_id of sol element of sol_info object */
					if(OMget_array_val(sol_arr_id, curr_solutions[zone]-1, &sol_id,
						OM_OBJ_RD) != OM_STAT_SUCCESS)
					{
						ERRverror("DVread_cgns:load_cgns",ERR_INFO,
							"can't get sol_info element object\n");
						ARRfree(curr_zones);
						ARRfree(vertex_size);
						ARRfree(curr_solutions);
                  if (file_name!=NULL) free(file_name);
						OMpop_status_range();
						return (0);
					}
					{
						int	GridLocation;

						/* get GridLocation */
						if(OMget_name_int_val(sol_id, OMstr_to_name("GridLocation"),
							&GridLocation) != OM_STAT_SUCCESS)
						{
							ERRverror("DVread_cgns:load_cgns",ERR_INFO,
								"can't get GridLocation value\n");
							ARRfree(curr_zones);
							ARRfree(vertex_size);
							ARRfree(curr_solutions);
                     if (file_name!=NULL) free(file_name);
							OMpop_status_range();
							return (0);
						}
						/* status bar range for solution */
						OMpush_status_range(45.0,100.0);
						switch(GridLocation)
						{
							case Vertex:
								if(add_unstructured_vrtx_data(DVread_cgns_id, file_name_buf,
										base, zone+1, fld_el_indx_unstr, index_dim, vertex_size,
										curr_solutions[zone]))
								{
									ARRfree(curr_zones);
									ARRfree(vertex_size);
									ARRfree(curr_solutions);
									reset_output_FLDs(DVread_cgns_id);
                           if (file_name!=NULL) free(file_name);
									OMpop_status_range();
									OMpop_status_range();
									return (0);
								}
								break;
							case CellCenter:
								if(add_unstructured_cell_data(DVread_cgns_id, zone_id,
									file_name_buf, base, zone+1, fld_el_indx_unstr, vertex_size,
										curr_solutions[zone]))
								{
									ARRfree(curr_zones);
									ARRfree(vertex_size);
									ARRfree(curr_solutions);
									reset_output_FLDs(DVread_cgns_id);
                           if (file_name!=NULL) free(file_name);
									OMpop_status_range();
									OMpop_status_range();
									return (0);
								}
								break;
							default:
								{
									char zone_name[CGNS_NAME_MAX_LEN];

									OMret_name_str_val(zone_id, OMstr_to_name("zone_name"),
										zone_name, sizeof(zone_name));
									ERRverror("DVread_cgns:load_cgns",ERR_INFO,
										"Zone '%s': only Vertex or CellCentered GridLocation is supported\n",
										zone_name);
								}
						}
						OMpop_status_range();
					}
					ARRfree(curr_solutions);
				}
				++fld_el_indx_unstr;
			}
			else
			if(zone_type == Structured)
			{
				/* structured */
				/* get rind and XYZ */
				Rind = OMret_name_array_ptr(zone_id,
					OMstr_to_name("Rind"), OM_GET_ARRAY_RD, &junk, &junk);
				if(Rind == NULL)
				{
					ERRverror("DVread_cgns:load_cgns",ERR_INFO,
						"can't get Rind array\n");
					ARRfree(curr_zones);
					ARRfree(vertex_size);
               if (file_name!=NULL) free(file_name);
					OMpop_status_range();
					return (0);
				}
				XYZ = OMret_name_array_ptr(zone_id,
					OMstr_to_name("XYZ"), OM_GET_ARRAY_RD, &junk, &junk);
				if(XYZ == NULL)
				{
					ERRverror("DVread_cgns:load_cgns",ERR_INFO,
						"can't get XYZ array\n");
					ARRfree(curr_zones);
					ARRfree(vertex_size);
					ARRfree(Rind);
               if (file_name!=NULL) free(file_name);
					OMpop_status_range();
					return (0);
				}
				/* status bar range for coordinates */
				OMpush_status_range(0.0,30.0);
				if(add_structured_coords(DVread_cgns_id, file_name_buf,
						base, zone+1, index_dim, phys_dim, vertex_size,
						Rind, XYZ))
				{
					ARRfree(curr_zones);
					ARRfree(vertex_size);
					ARRfree(Rind);
					ARRfree(XYZ);
					reset_output_FLDs(DVread_cgns_id);
               if (file_name!=NULL) free(file_name);
					OMpop_status_range();
					OMpop_status_range();
					return (0);
				}
				OMpop_status_range();
				ARRfree(Rind);
				ARRfree(XYZ);
				/* now set solution data */
				if(OMget_name_int_val(zone_id, OMstr_to_name("nSolution"),
					&nSolutions) != OM_STAT_SUCCESS)
				{
					ERRverror("DVread_cgns:load_cgns",ERR_INFO,
						"can't get nSolution value\n");
					ARRfree(curr_zones);
					ARRfree(vertex_size);
               if (file_name!=NULL) free(file_name);
					OMpop_status_range();
					return (0);
				}
				if(nSolutions)
				{
					/* get CurrentSolutions */
					curr_solutions = OMret_name_array_ptr(DVread_cgns_id,
						OMstr_to_name("CurrentSolutions"), OM_GET_ARRAY_RD, &junk,
						&junk);
					if(curr_solutions == NULL)
					{
						ERRverror("DVread_cgns:load_cgns",ERR_INFO,
							"can't get CurrentSolutions array\n");
						ARRfree(curr_zones);
						ARRfree(vertex_size);
                  if (file_name!=NULL) free(file_name);
						OMpop_status_range();
						return (0);
					}
					/* get obj_id of sol_info object */
					sol_arr_id = OMlookup_subobj(zone_id,
						OMstr_to_name("sol_info"), OM_OBJ_RD);
					if(OMis_null_obj(sol_arr_id))
					{
						ERRverror("DVread_cgns:load_cgns",ERR_INFO,
							"can't locate sol_info subobject\n");
						ARRfree(curr_zones);
						ARRfree(vertex_size);
						ARRfree(curr_solutions);
                  if (file_name!=NULL) free(file_name);
						OMpop_status_range();
						return (0);
					}
					/* get obj_id of sol element of sol_info object */
					if(OMget_array_val(sol_arr_id, curr_solutions[zone]-1, &sol_id,
						OM_OBJ_RD) != OM_STAT_SUCCESS)
					{
						ERRverror("DVread_cgns:load_cgns",ERR_INFO,
							"can't get sol_info element object\n");
						ARRfree(curr_zones);
						ARRfree(vertex_size);
						ARRfree(curr_solutions);
                  if (file_name!=NULL) free(file_name);
						OMpop_status_range();
						return (0);
					}
					{
						int	GridLocation;

						/* get GridLocation */
						if(OMget_name_int_val(sol_id, OMstr_to_name("GridLocation"),
							&GridLocation) != OM_STAT_SUCCESS)
						{
							ERRverror("DVread_cgns:load_cgns",ERR_INFO,
								"can't get GridLocation value\n");
							ARRfree(curr_zones);
							ARRfree(vertex_size);
							ARRfree(curr_solutions);
                     if (file_name!=NULL) free(file_name);
							OMpop_status_range();
							return (0);
						}
						/* get Rind for solution */
						Rind = OMret_name_array_ptr(sol_id,
							OMstr_to_name("Rind"), OM_GET_ARRAY_RD, &junk, &junk);
						if(Rind == NULL)
						{
							ERRverror("DVread_cgns:load_cgns",ERR_INFO,
								"can't get Rind array\n");
							ARRfree(curr_zones);
							ARRfree(vertex_size);
							ARRfree(curr_solutions);
                     if (file_name!=NULL) free(file_name);
							OMpop_status_range();
							return (0);
						}
						/* status bar range for solution */
						OMpush_status_range(30.0,100.0);
						switch(GridLocation)
						{
							case Vertex:
								if(add_structured_vrtx_data(DVread_cgns_id, file_name_buf,
										base, zone+1, fld_el_indx_str, index_dim, vertex_size,
										Rind, curr_solutions[zone]))
								{
									ARRfree(curr_zones);
									ARRfree(vertex_size);
									ARRfree(curr_solutions);
									ARRfree(Rind);
									reset_output_FLDs(DVread_cgns_id);
                           if (file_name!=NULL) free(file_name);
									OMpop_status_range();
									OMpop_status_range();
									return (0);
								}
								break;
							case CellCenter:
								if(add_structured_cell_data(DVread_cgns_id, file_name_buf,
										base, zone+1, fld_el_indx_str, index_dim, vertex_size,
										Rind, curr_solutions[zone]))
								{
									ARRfree(curr_zones);
									ARRfree(vertex_size);
									ARRfree(curr_solutions);
									ARRfree(Rind);
									reset_output_FLDs(DVread_cgns_id);
                           if (file_name!=NULL) free(file_name);
									OMpop_status_range();
									OMpop_status_range();
									return (0);
								}
								break;
							default:
								{
									char zone_name[CGNS_NAME_MAX_LEN];

									OMret_name_str_val(zone_id, OMstr_to_name("zone_name"),
										zone_name, sizeof(zone_name));
									ERRverror("DVread_cgns:load_cgns",ERR_INFO,
										"Zone '%s': only Vertex or CellCentered GridLocation is supported\n",
										zone_name);
								}
						}
						ARRfree(Rind);
						OMpop_status_range();
					}
					ARRfree(curr_solutions);
				}
				++fld_el_indx_str;
			}
			else
			{
				/* not structured nor unstructured */
				char zone_name[CGNS_NAME_MAX_LEN];

				OMret_name_str_val(zone_id, OMstr_to_name("zone_name"),
					zone_name, sizeof(zone_name));
				ERRverror("DVread_cgns:load_cgns",ERR_INFO,
					"Zone '%s': only Structured or Unstructured GridType is supported\n",
					zone_name);
				reset_output_FLDs(DVread_cgns_id);
			}
			OMpop_status_range();

         ARRfree(vertex_size);
         vertex_size = NULL;
		}
	}

	ARRfree(curr_zones);
   if (file_name!=NULL) free(file_name);

	/* CGNS file processing went OK */
	OMset_name_str_val(DVread_cgns_id, OMstr_to_name("status"), "no CGNS error reported");

	return (1);
}


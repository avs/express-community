
#include "ccs_gen.h"
#include <iostream.h>

// #define DEBUG

#define NCELL_TYPES 17

// <<<<<<<<<<<<< Local Data Types >>>>>>>>>>>>>>

  enum CellType {point, line, line2, polyline, tri, tri2, polytri, _quad, quad2,
	         tet, tet2, _hex, _hex2, pyr, pyr2, prism, prism2};

  struct CellTypeInfo
  {
    int cs;           // which out cell set are 'celltype' to be put in
    int ncells;       // total number of cells in this cell set
    int curr_con_id;  // curr posn in connection list have been added so far

    int nnodes;  // express cell information
    int ncorners;
    int dim;
    int order;
    int poly_flag;
    int type;
    char *name;
  };

int
CatCSMods_CatCellSets::update(OMevent_mask event_mask, int seq_num)
{

// <<<<<<<<<<<<< Local Variables >>>>>>>>>>>>>>
  int i;
  int cell_nnodes, curr_con_id;
  int mesh_in_set;
  int mesh_in_ncell_sets = mesh_in.ncell_sets;
  int mesh_out_ncell_sets=0, mesh_out_set;
  int *mesh_in_con_list, *mesh_out_con_list;

  CellType cell_type, *mesh_in_ctype;
  CellTypeInfo cell_type_info[NCELL_TYPES]={
	{-1, 0, 0, 1, 1, 0, 1, 0, 1, "Point"},
	{-1, 0, 0, 2, 2, 1, 1, 0, 2, "Line"},
	{-1, 0, 0, 3, 2, 1, 2, 0, 12, "Line2"},
	{-1, 0, 0, 2, 2, 1, 1, 1, 3, "Polyline"}, // ncorners ?
	{-1, 0, 0, 3, 3, 2, 1, 0, 4, "Tri"},
	{-1, 0, 0, 6, 3, 2, 2, 0, 14, "Tri2"},
	{-1, 0, 0, 2, 2, 2, 1, 1, 10, "Polytri"},
	{-1, 0, 0, 4, 4, 2, 1, 0, 5, "Quad"},
	{-1, 0, 0, 8, 4, 2, 2, 0, 15, "Quad2"},
	{-1, 0, 0, 4, 4, 3, 1, 0, 6, "Tet"},
	{-1, 0, 0, 10, 4, 3, 2, 0, 16, "Tet2"},
	{-1, 0, 0, 8, 8, 3, 1, 0, 7, "_Hex"},
	{-1, 0, 0, 20, 8, 3, 2, 0, 17, "_Hex2"},
	{-1, 0, 0, 5, 5, 3, 1, 0, 9, "Pyr"},
	{-1, 0, 0, 13, 5, 3, 2, 0, 19, "Pyr2"},
	{-1, 0, 0, 6, 6, 3, 1, 0, 8, "Prism"},
	{-1, 0, 0, 15, 6, 3, 2, 0, 18, "Prism2"}};

// <<<<<<<<<<<<< Functional Part >>>>>>>>>>>>>>

  // ---------- parse input mesh -----------
     
  mesh_in_ctype = new CellType [mesh_in_ncell_sets];

#ifdef DEBUG
  cout << "Number of Cell Sets in = " << mesh_in_ncell_sets << endl;
#endif

  for (mesh_in_set=0; mesh_in_set < mesh_in_ncell_sets; mesh_in_set++)
  {
    // --------- identify each in cell set ---------
    switch(mesh_in.cell_set[mesh_in_set].cell_ndim)
    {
    case 0: // must be a point
      cell_type=point;
      break;

    case 1:  // line, line2, polyline,
      if(!mesh_in.cell_set[mesh_in_set].poly_flag) // if not polyXXX cell

	if(mesh_in.cell_set[mesh_in_set].cell_order==1) // if line
	  cell_type=line;
	else // line2
	  cell_type=line2;

      else // polyline
 	  cell_type=polyline;
	
      break;

    case 2:  // tri, tri2, polytri, quad, quad2
      if(!mesh_in.cell_set[mesh_in_set].poly_flag) // if not polyXXX cell
      
	switch (mesh_in.cell_set[mesh_in_set].cell_nnodes)
	{
	case 3: // tri  
	  cell_type=tri;
	  break;
	case 4: // quad
	  cell_type=_quad;
	  break;
        case 6: // tri2
      	  cell_type=tri2;
	  break;
	case 8:  // quad2
	  cell_type=quad2;
	  break;
	default: // error invalid cell type
	  break;
	}

      else // polytri
	cell_type=polytri;
      break;

    case 3:  // tet, tet2, _hex, _hex2, pyr, pyr2, prism, prism2
      if(mesh_in.cell_set[mesh_in_set].cell_order==1) // if vertex nodes only
      {
	switch (mesh_in.cell_set[mesh_in_set].cell_nnodes)
	{
	case 4:  // tet
	  cell_type=tet;
	  break;
	case 5:  // pyr
	  cell_type=pyr;
	  break;
	case 6:  // prism
	  cell_type=prism;
	  break;
	case 8:  // _hex
	  cell_type=_hex;
	  break;
	default: // error invalid cell type
	  break;
	}
      }else{
	switch (mesh_in.cell_set[mesh_in_set].cell_nnodes)
	{
	case 10:  // tet2
	  cell_type=tet2;
	  break;
	case 13:  // pyr2
	  cell_type=pyr2;
	  break;
	case 15:  // prism2
	  cell_type=prism2;
	  break;
	case 20:  // _hex2
	  cell_type=_hex2;
	  break;

	default: // error invalid cell type
	  break;
	}
      }
      break;
    default: // error invalid cell dimension
      break;
    }

    // --------- store information ---------
    mesh_in_ctype[mesh_in_set]=cell_type;

    if(cell_type_info[cell_type].cs == -1)
    {
      cell_type_info[cell_type].cs=mesh_out_ncell_sets;
      mesh_out_ncell_sets++;
    }

    cell_type_info[cell_type].ncells+=mesh_in.cell_set[mesh_in_set].ncells;

  }

#ifdef DEBUG
  cout << "Number of Cell Sets out = " << mesh_out_ncell_sets << endl;
#endif

  // --------- setup mesh out cell sets --------
  mesh_out.ncell_sets=mesh_out_ncell_sets; // NB there should never be >NCELL_TYPES cell_sets

  for(i=0; i<NCELL_TYPES; i++)
  {
    if(cell_type_info[i].cs > -1)
    {  
      mesh_out.cell_set[cell_type_info[i].cs].ncells=cell_type_info[i].ncells;
      mesh_out.cell_set[cell_type_info[i].cs].cell_nnodes=cell_type_info[i].nnodes;
      mesh_out.cell_set[cell_type_info[i].cs].cell_ndim=cell_type_info[i].dim;
      mesh_out.cell_set[cell_type_info[i].cs].cell_order=cell_type_info[i].order;
      mesh_out.cell_set[cell_type_info[i].cs].poly_flag=cell_type_info[i].poly_flag;
    }
  }

  // --------- catenate cell sets ---------
  for (mesh_in_set=0; mesh_in_set < mesh_in_ncell_sets; mesh_in_set++)
  {
    cell_type=mesh_in_ctype[mesh_in_set]; // cell_type fot curr mesh_in cell set
    cell_nnodes=mesh_in.cell_set[mesh_in_set].cell_nnodes;
    curr_con_id=cell_type_info[cell_type].curr_con_id;
    mesh_out_set= cell_type_info[cell_type].cs;
    mesh_in_con_list = (int *)mesh_in.cell_set[mesh_in_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);
    if (!mesh_in_con_list)
    {
	cout << "NULL mesh_in_con_list" << endl;
	return 0;
    };
    mesh_out_con_list = (int *)mesh_out.cell_set[mesh_out_set].node_connect_list.
      ret_array_ptr(OM_GET_ARRAY_WR, &cell_type_info[mesh_out_set].nnodes);
    if (!mesh_out_con_list)
    {
	cout << "NULL mesh_out_con_list" << endl;
	return 0;
    };

    // --------- copy connectivity ---------
    // for every cell in cell_set
    for(i = 0; i < mesh_in.cell_set[mesh_in_set].ncells * cell_nnodes; i++)
      mesh_out_con_list[i+curr_con_id] = mesh_in_con_list[i];

    // update cell_type curr_con_id
    cell_type_info[cell_type].curr_con_id+=mesh_in.cell_set[mesh_in_set].ncells * cell_nnodes;

    // --------- tidy up ---------
    if (mesh_in_con_list)
      ARRfree((char *)mesh_in_con_list);
    if (mesh_out_con_list)
      ARRfree((char *)mesh_out_con_list);
  }

  delete [] mesh_in_ctype;

  // return 1 for success
  return(1);
}





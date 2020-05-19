#include <avs/util.h>
#include <avs/err.h>
#include <avs/math.h>
#include <avs/om.h>
#include <avs/fld.h>
#include <avs/arr.h>
#include "fld_util.h"

#define MAX_NAME_SIZE 1024
#define ERR_RETURN(A) ERRerror("advec",1,ERR_ORIG, A); return(0);

void FUset_null_mesh(OMobj_id elem_id) 
{
  int dims[3];
  
  dims[0] = 0; dims[1] = 0; dims[2] = 0;
  if (!OMis_null_obj(elem_id)) {
    FLDset_nnodes(elem_id, 0); /* no coords */
    FLDset_dims(elem_id, dims);
    FLDset_node_data_ncomp(elem_id, 0); /* no node data */
    FLDset_ncell_sets(elem_id, 0); /* no cell sets */
  }
}

int FUcopy_field(OMobj_id inMesh_id, OMobj_id outMesh_id)
{
  OMobj_id inCellSet_id, outCellSet_id;
  int nnodes, nspace, ncomps;
  int in_coord_size, out_coord_size;
  float *in_coord_array, *out_coord_array;
  int in_cell_data_size, out_cell_data_size;
  char *in_cell_data, *out_cell_data;
  int ncell_sets, cell_type, ncells, i, j;
  int in_conn_size;
  int *in_conn;
  int in_nnode_size;
  int *in_nnodes;
  char cellSetName[128], user_name[128];
  int cell_ncomps, cell_veclen, cell_id, cell_data_type;
  int data_type,veclen,size;
  char *node_data;
  char units[MAX_NAME_SIZE], label[MAX_NAME_SIZE];
  
  /* Get the information from the input mesh. */
  if (FLDget_nnodes(inMesh_id, &nnodes) != 1) {
    FUset_null_mesh(outMesh_id);
    return(0);
  }
  if (FLDget_nspace(inMesh_id, &nspace) != 1) {
    FUset_null_mesh(outMesh_id);
    return(0);
  }
  if (FLDget_ncell_sets(inMesh_id, &ncell_sets) != 1) {
    FUset_null_mesh(outMesh_id);
    return(0);
  }
  /* If we have an empty input field, null the output. */
  if (nnodes == 0 || ncell_sets == 0) {
    FUset_null_mesh(outMesh_id);
    return(0);
  }
  
  if (FLDget_coord(inMesh_id, (float **)&in_coord_array, &in_coord_size,
    OM_GET_ARRAY_RD) != 1) {
    FUset_null_mesh(outMesh_id);
    return(0);
  }
  
  /* Write the information from to the output mesh. */
  if (FLDset_nnodes(outMesh_id, nnodes) != 1) {
    ARRfree((char *)in_coord_array);
    FUset_null_mesh(outMesh_id);
    return(0);
  }
  if (FLDset_nspace(outMesh_id, nspace) != 1) {
    ARRfree((char *)in_coord_array);
    FUset_null_mesh(outMesh_id);
    return(0);
  }
  if (FLDset_ncell_sets(outMesh_id, ncell_sets) != 1) {
    ARRfree((char *)in_coord_array);
    FUset_null_mesh(outMesh_id);
    return(0);
  }
  if (FLDget_coord(outMesh_id, (float **)&out_coord_array, &out_coord_size,
    OM_GET_ARRAY_WR) != 1) {
    ARRfree((char *)in_coord_array);
    FUset_null_mesh(outMesh_id);
    return(0);
  }
  memcpy(out_coord_array, in_coord_array, in_coord_size* sizeof(float));
  ARRfree((char *)in_coord_array);
  ARRfree((char *)out_coord_array);
  
  /* Clear all output cell sets so when we do the add cell
  sets they aren't appended to the field.
  */
  if (FLDset_ncell_sets(outMesh_id, 0) != 1) {
    FUset_null_mesh(outMesh_id);
    return(0);
  }
  
  /* Deal with each sell set individually. */
  for (i=0; i<ncell_sets; i++) {
    /* Get the input cell set name. */
    if (FLDget_cell_set(inMesh_id, i, &inCellSet_id) != 1) {
      FUset_null_mesh(outMesh_id);
      return(0);
    }
    if (FLDget_cell_type(inCellSet_id, &cell_type) != 1) {
      FUset_null_mesh(outMesh_id);
      return(0);
    }
    if (FLDget_cell_set_name(inCellSet_id, cellSetName, 128) != 1) {
      FUset_null_mesh(outMesh_id);
      return(0);
    }
    
    /* Add the cell set to the output mesh and get its id. */
    if (FLDadd_cell_set(outMesh_id, cellSetName) != 1) {
      FUset_null_mesh(outMesh_id);
      return(0);
    }
    if (FLDget_cell_set(outMesh_id, i, &outCellSet_id) != 1) {
      FUset_null_mesh(outMesh_id);
      return(0);
    }
    /* If we have an input user name, write it to the output. */
    if (FLDget_cell_set_user_name(inCellSet_id, user_name, 128) == 1) {
      FLDset_cell_set_user_name(outCellSet_id, user_name);
    }
    
    /* We need to deal with certain cell sets differently -
    polyline, polytri and polyhedron.
    */
    if (cell_type == 3 || cell_type == 10 || cell_type == 19) {
    /* Get the number of cells in the cell set and
    make that number in the output mesh.
      */
      if (FLDget_npolys(inCellSet_id, &ncells) != 1) {
        FUset_null_mesh(outMesh_id);
        return(0);
      }
      if (FLDset_npolys(outCellSet_id, ncells) != 1) {
        FUset_null_mesh(outMesh_id);
        return(0);
      }
      
      /* If we have a polyhedron cell set, we also need to deal
      with the nnodes array.
      */
      if (cell_type == 19) {
        if (FLDget_poly_nnodes(inCellSet_id, &in_nnodes, &in_nnode_size, 
          OM_GET_ARRAY_RD) != 1) {
          FUset_null_mesh(outMesh_id);
          return(0);
        }
        if (FLDset_poly_nnodes(outCellSet_id, in_nnodes, in_nnode_size, 
          OM_SET_ARRAY_COPY) != 1) {
          ARRfree((char *)in_nnodes);
          FUset_null_mesh(outMesh_id);
          return(0);
        }
        ARRfree((char *)in_nnodes);
      }
      
      /* Get the input cell set connectivity and copy it to
      the output cell set.
      */
      if (FLDget_poly_connect(inCellSet_id, &in_conn, &in_conn_size, 
        OM_GET_ARRAY_RD) != 1) {
        FUset_null_mesh(outMesh_id);
        return(0);
      }
      if (FLDset_poly_connect(outCellSet_id, in_conn, in_conn_size, 
        OM_SET_ARRAY_COPY) != 1) {
        ARRfree((char *)in_conn);
        FUset_null_mesh(outMesh_id);
        return(0);
      }
      ARRfree((char *)in_conn);
    }
/*    else if (cell_type == 2) {
      int out[2];
      
      if (FLDset_cell_set(outCellSet_id, "Polyline") != 1) {
        FUset_null_mesh(outMesh_id);
        return(0);
      }
      if (FLDget_ncells(inCellSet_id, &ncells) != 1) {
        FUset_null_mesh(outMesh_id);
        return(0);
      }
      if (FLDset_npolys(outCellSet_id, 1) != 1) {
        FUset_null_mesh(outMesh_id);
        return(0);
      }
      
      if (FLDget_node_connect(inCellSet_id, &in_conn, &in_conn_size, 
        OM_GET_ARRAY_RD) != 1) {
        FUset_null_mesh(outMesh_id);
        return(0);
      }
      out[0]=in_conn[0];
      out[1]=in_conn[in_conn_size-1];
      if (FLDset_poly_connect(outCellSet_id, out, 2, OM_SET_ARRAY_COPY) != 1) {
        ARRfree((char *)in_conn);
        FUset_null_mesh(outMesh_id);
        return(0);
      }
      ARRfree((char *)in_conn);
    }
*/  else {
      /* Get the number of cells in the cell set and
         make that number in the output mesh.
      */
      if (FLDget_ncells(inCellSet_id, &ncells) != 1) {
        FUset_null_mesh(outMesh_id);
        return(0);
      }
      if (FLDset_ncells(outCellSet_id, ncells) != 1) {
        FUset_null_mesh(outMesh_id);
        return(0);
      }
      
      /* Get the input cell set connectivity and copy it to
         the output cell set.
      */
      if (FLDget_node_connect(inCellSet_id, &in_conn, &in_conn_size, 
        OM_GET_ARRAY_RD) != 1) {
        FUset_null_mesh(outMesh_id);
        return(0);
      }
      if (FLDset_node_connect(outCellSet_id, in_conn, in_conn_size, 
        OM_SET_ARRAY_COPY) != 1) {
        ARRfree((char *)in_conn);
        FUset_null_mesh(outMesh_id);
        return(0);
      }
      ARRfree((char *)in_conn);
    }
    
    /* Deal with the cell data if there is any. */
    if (FLDget_cell_data_ncomp(inCellSet_id, &cell_ncomps) == 1) {
      if (FLDset_cell_data_ncomp(outCellSet_id, cell_ncomps) == 1) {
        for (j=0; j<cell_ncomps; j++) {
          /* cell data veclen */
          if (FLDget_cell_data_veclen(inCellSet_id, j, &cell_veclen) == 1)
            FLDset_cell_data_veclen(outCellSet_id, j, cell_veclen);
          /* cell data id */
          if (FLDget_cell_data_id(inCellSet_id, j, &cell_id) == 1)
            FLDset_cell_data_id(outCellSet_id, j, cell_id);
          /* actual cell data */
          if (FLDget_cell_data(inCellSet_id, j, &cell_data_type, (char **)&in_cell_data, 
            &in_cell_data_size, OM_GET_ARRAY_RD) == 1) {
            if (FLDget_cell_data(outCellSet_id, j, &cell_data_type, (char **)&out_cell_data, 
              &out_cell_data_size, OM_GET_ARRAY_WR) == 1) {
              switch (cell_data_type) {
              case OM_TYPE_DOUBLE:
                in_cell_data_size *= sizeof(double);
                break;
              case OM_TYPE_FLOAT:
                in_cell_data_size *= sizeof(float);
                break;
              case OM_TYPE_INT:
                in_cell_data_size *= sizeof(int);
                break;
              case OM_TYPE_SHORT:
                in_cell_data_size *= sizeof(short);
                break;
              case OM_TYPE_CHAR:
              case OM_TYPE_BYTE:
              default:
                break;
              }
              memcpy(out_cell_data, in_cell_data, in_cell_data_size);
              ARRfree((char *)in_cell_data);
              ARRfree((char *)out_cell_data);
            }
            else ARRfree((char *)in_coord_array);
          }
        }
      }
    }
  }
  
  /* Copying node data */
  if (FLDget_node_data_ncomp(inMesh_id, &ncomps) != 1) {
    ERR_RETURN("Error getting ncomp");
  }
  if (FLDset_node_data_ncomp (outMesh_id, ncomps) != 1) {
    ERR_RETURN("Error setting nnode_data");
  }
  
  for(i=0 ; i<ncomps ; i++) {
    if (FLDget_node_data_type(inMesh_id, i, &data_type) !=1) {
      ERR_RETURN("Error getting data type");
    }
    if (FLDget_node_data_veclen(inMesh_id, i, &veclen) !=1) {
      ERR_RETURN("Error getting veclen");
    }
    if (FLDget_node_data(inMesh_id, i, &data_type, (char **)(&node_data), &size, OM_GET_ARRAY_RD) != 1) {
      ERR_RETURN("cannot get node data");
    }
    if (FLDget_node_data_units(inMesh_id, i, units, MAX_NAME_SIZE) != 1) {
      strcpy(units, "");
    }
    if (FLDget_node_data_label(inMesh_id, i, label, MAX_NAME_SIZE) != 1) {
      strcpy(label, "");
    }
    if (FLDset_node_data_comp (outMesh_id, i, veclen, label, units) != 1) {
      ERR_RETURN("Error setting node component");
    }
    if (nnodes) {
      if (FLDset_node_data(outMesh_id, i, (char *)node_data, data_type, 
        veclen*nnodes, OM_SET_ARRAY_COPY) != 1) {
        ERR_RETURN("Error setting node data");
      }
      if (FLDcopy_node_minmax(inMesh_id, outMesh_id, i, i) != 1) {
        ERR_RETURN("Error copying node minmax");
      }		
      if (FLDcopy_node_minmax_vec(inMesh_id, outMesh_id, i, i) != 1) {
        ERR_RETURN("Error copying node minmax");
      }
    }
  }
  
  if (node_data) ARRfree((char *)node_data);
  
  return(1);
}

static int find_point(float x, float y, float z, float *coord, int from, int to)
{
  int i;
  
  for(i=from;i<=to;i++) {
    if (x==coord[3*i] && y==coord[3*i+1] && z==coord[3*i+2]) {
      return(i);
    }
  }
  
  return(-1);
}

int FUmerge_isolines(OMobj_id in, OMobj_id out, OMobj_id points)
{
  int      index,cell_in,cell_out,nb_new_points;
  int      nnodes, out_nnodes, tmp_nnodes, points_nnodes;
  int      data_type, size, veclen;
  int      *node_list, nsets, cs, ncells;
  int      *out_node_list, out_ncells;
  int      *tmp_node_list, tmp_ncells;
  int      nspace;
  float    *coord, *node_data;
  float    *out_coord, *out_node_data;
  float    *tmp_coord, *tmp_node_data;
  float    *points_coord;
  OMobj_id cell_set,out_cell_set;
  
  /*printf("FUmerge_isolines\n");*/
  if (FLDget_nnodes(out, &out_nnodes) != 1) {
    out_nnodes=0;
  }
  
  if (out_nnodes == 0) { /* Simple copy */
    /*    printf(" -> COPY\n");*/
    FUcopy_field(in, out);
  }
  else {
    /* Add */
    /*    printf(" -> ADD ");*/
    if (FLDget_nnodes(in, &nnodes) != 1) {
      ERR_RETURN("cannot get nnodes");
    }
    /*    printf(" -> In nnodes=%d",nnodes);*/
    if (FLDget_nnodes(points, &points_nnodes) != 1) {
      ERR_RETURN("cannot get nnodes");
    }
    /*    printf(" -> Points nnodes=%d",points_nnodes);*/
    if (FLDget_coord(in, &coord, &size, OM_GET_ARRAY_RD) != 1) {
      ERR_RETURN("cannot get coordinates");
    }
    if (FLDget_coord(points, &points_coord, &size, OM_GET_ARRAY_RD) != 1) {
      ERR_RETURN("cannot get coordinates");
    }
    /*    printf(" (%f)",points_coord[0]);*/
    
    if (nnodes<1 || points_nnodes<1) {
      ERR_RETURN("too few points");
    }
    
    if (!(coord[0]==points_coord[0] && coord[1]==points_coord[1] &&
      coord[2]==points_coord[2]))
    {
      /*printf("Nothing to ADD\n");*/
      if (coord) ARRfree((char *)coord);
      if (points_coord) ARRfree((char *)points_coord);
      return(1);
    }
    
    if (FLDget_nnodes(out, &out_nnodes) != 1) {
      ERR_RETURN("cannot get nnodes");
    }
    /*    printf(" -> Out nnodes=%d",out_nnodes);*/
    if (FLDget_coord(out, &out_coord, &size, OM_GET_ARRAY_RW) != 1) {
      ERR_RETURN("cannot get coordinates");
    }
    if (FLDget_node_data_veclen(in, 0, &veclen) !=1) {
      ERR_RETURN("Error getting veclen");
    }
    data_type = DTYPE_FLOAT;
    if (FLDget_node_data(in, 0, &data_type, (char **)(&node_data), &size, OM_GET_ARRAY_RD) != 1) {
      ERR_RETURN("cannot get node data");
    }
    if (FLDget_nspace(in, &nspace) != 1) {
      ERR_RETURN("cannot get nspace");
    }
    if (FLDget_node_data(out, 0, &data_type, (char **)(&out_node_data), &size, OM_GET_ARRAY_RW) != 1) {
      ERR_RETURN("cannot get node data");
    }
    if (FLDget_ncell_sets(in, &nsets) != 1) {
      ERR_RETURN("cannot get nsets");
    }
    
    tmp_nnodes=0;
    tmp_coord=(float *)malloc(sizeof(float));
    tmp_node_data=(float *)malloc(sizeof(float));
    
    for (cs=0; cs<nsets; cs++) {
      tmp_ncells=0;
      tmp_node_list=(int *)malloc(sizeof(float));
      if (FLDget_cell_set(in, cs, &cell_set) != 1) {
        ERR_RETURN("cannot get cell set");
      }
      if (FLDget_npolys(cell_set, &ncells) != 1) {
        ERR_RETURN("cannot get ncells");
      }
      if (FLDget_cell_set(out, cs, &out_cell_set) != 1) {
        ERR_RETURN("cannot get cell set");
      }
      if (FLDget_npolys(out_cell_set, &out_ncells) != 1) {
        ERR_RETURN("cannot get ncells");
      }
      if (FLDget_poly_connect(cell_set, &node_list, 
        &size, OM_GET_ARRAY_RD) != 1) {
        ERR_RETURN("cannot get cell connectivity");
      }
      if (FLDget_poly_connect(out_cell_set, &out_node_list, 
        &size, OM_GET_ARRAY_RD) != 1) {
        ERR_RETURN("cannot get cell connectivity");
      }
      
      for(cell_in=cell_out=0;cell_in<ncells;cell_out++) {
        if (node_list[2*cell_in] == node_list[2*cell_in+1]) {
          /* Null streamline, could take it off */
          cell_in++;
          continue;
        }
        
        index=find_point(points_coord[3*cell_in],points_coord[3*cell_in+1],
          points_coord[3*cell_in+2],
          out_coord,out_node_list[2*cell_out],out_node_list[2*cell_out+1]);
        if (index<0) {
          /* Streamline doesn't exist no more */
          continue;
        }
        
        nb_new_points=index-out_node_list[2*cell_out]+1 +
          node_list[2*cell_in+1]-node_list[2*cell_in];
        tmp_coord=(float *)realloc(tmp_coord,
          3*(tmp_nnodes + nb_new_points)*sizeof(float));
        tmp_node_data=(float *)realloc(tmp_node_data,
          veclen*(tmp_nnodes + nb_new_points)*sizeof(float));
        tmp_node_list=(int *)realloc(tmp_node_list,
          2*(tmp_ncells + 1)*sizeof(int));
        
        memcpy(tmp_coord+3*tmp_nnodes,
          out_coord+3*out_node_list[2*cell_out],
          3*sizeof(float)*(index - out_node_list[2*cell_out] + 1));
        memcpy(tmp_node_data+veclen*tmp_nnodes,
          out_node_data+veclen*out_node_list[2*cell_out],
          veclen*sizeof(float)*(index - out_node_list[2*cell_out] + 1));
        memcpy(tmp_coord+3*(tmp_nnodes+index-out_node_list[2*cell_out]+1),
          coord+3*(node_list[2*cell_in]+1),
          3*sizeof(float)*(node_list[2*cell_in+1]-node_list[2*cell_in]));
        memcpy(tmp_node_data+veclen*(tmp_nnodes+index-out_node_list[2*cell_out]+1),
          node_data+veclen*(node_list[2*cell_in]+1),
          veclen*sizeof(float)*(node_list[2*cell_in+1]-node_list[2*cell_in]));
        tmp_node_list[2*tmp_ncells]=tmp_nnodes;
        tmp_node_list[2*tmp_ncells+1]=tmp_nnodes + nb_new_points -1;
        
        tmp_nnodes = tmp_nnodes + nb_new_points;
        tmp_ncells++;
        cell_in++;
      }
      
      if (node_list) ARRfree((char *)node_list);
      if (out_node_list) ARRfree((char *)out_node_list);
    }
    
    if (coord) ARRfree((char *)coord);
    if (out_coord) ARRfree((char *)out_coord);
    if (points_coord) ARRfree((char *)points_coord);
    if (node_data) ARRfree((char *)node_data);
    if (out_node_data) ARRfree((char *)out_node_data);
    
    /*    printf(" -> New isolines nnodes = %d\n",tmp_nnodes);*/
    if (FLDset_nnodes(out, tmp_nnodes) != 1) {
      ERR_RETURN("cannot set nnodes");
    }
    if (FLDset_coord(out, tmp_coord, 3*tmp_nnodes, OM_SET_ARRAY_FREE) != 1) {
      ERR_RETURN("cannot set coord");
    }
    if (FLDset_node_data(out, 0, (char *)tmp_node_data, DTYPE_FLOAT, 
      veclen*tmp_nnodes, OM_SET_ARRAY_FREE)!= 1) {
      ERR_RETURN("cannot set node data");
    }
    if (FLDcopy_node_minmax(in, out, 0, 0) != 1) {
      ERR_RETURN("Error copying node minmax");
    }		
    if (FLDcopy_node_minmax_vec(in, out, 0, 0) != 1) {
      ERR_RETURN("Error copying node minmax");
    }
    if (FLDset_ncell_sets(out, 0) != 1) {
      ERR_RETURN("cannot set nsets");
    }
    if (FLDset_ncell_sets(out, 1) != 1) {
      ERR_RETURN("cannot set nsets");
    }
    if (FLDget_cell_set(out, 0, &out_cell_set) != 1) {
      ERR_RETURN("cannot get cell set");
    }
    if (FLDset_cell_set(out_cell_set, "Polyline") != 1) {
      ERR_RETURN("cannot set cell type");
    }
    if (FLDset_npolys(out_cell_set,tmp_ncells) != 1) {
      ERR_RETURN("cannot set npolys");
    }
    if (FLDset_poly_connect(out_cell_set, tmp_node_list, tmp_ncells*2, OM_SET_ARRAY_FREE) != 1) {
      ERR_RETURN("cannot set poly connect");
    }
  }
  
  /*printf("END FUmerge_isolines\n");*/
  return(1);
}

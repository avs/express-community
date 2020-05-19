#ifdef SCCS
static char sccsid[]="@(#)$RCSfile: advect.c,v $ $Revision: 4.4 $ $AVS$ $Date: 1997/10/16 18:51:00 $";
#endif
/*
Copyright (c) 1994 by
Advanced Visual Systems Inc.
All Rights Reserved

  This software comprises unpublished confidential information of
  Advanced Visual Systems Inc. and may not be used, copied or made
  available to anyone, except in accordance with the license
  under which it is furnished.
  
    This file is under sccs control at AVS in:
    $Source: /cvs/repository/express/modules/advect.c,v $
*/
#include <avs/util.h>
#include <avs/err.h>
#include <avs/math.h>
#include <avs/om.h>
#include <avs/fld.h>
#include <avs/arr.h>
#include "fld_util.h"

#define CAP   0
#define CYCLE 1
#define ADD   2

#define MAX(x,y)      ((x>y)?(x):(y))
#define MIN(x,y)      ((x<y)?(x):(y))
#define ERR_RETURN(A) ERRerror("advec",1,ERR_ORIG, A); return(0);
#define MAX_NAME_SIZE 1024
#define ALLOC_SIZE    256

static int tsn = 0;

static int get_tracer_min_node(int glyph_node, int stream_min_node,int stream_max_node,
                               int tracer_len, int tracer_style)
{
  switch(tracer_style) {
  case CAP:
    return(stream_min_node);
  case CYCLE:
    return(MAX(stream_min_node,glyph_node-tracer_len));
  case ADD:
  default:
    return(stream_min_node);
  }
}

static int get_tracer_max_node(int glyph_node, int stream_min_node,int stream_max_node,
                               int tracer_len, int tracer_style)
{
  switch(tracer_style) {
  case CAP:
    return(MIN(glyph_node,stream_min_node+tracer_len));
  case CYCLE:
    return(glyph_node);
  case ADD:
  default:
    return(glyph_node);
  }
}

static int FUNCadvec (OMobj_id in, double time, double start_time, double release_interval,
                      double time_step_interval,
                      int dir, int tracer_len, int tracer_style, OMobj_id out,
                      OMobj_id out_tracer)
{
  int   i, cell, node, nnodes, out_nnodes, rdir;
  int   data_type, size, veclen, ncomp;
  int   *node_list, *out_node_list, nsets, cs, ncells;
  int   nspace;
  char  units[MAX_NAME_SIZE], label[MAX_NAME_SIZE];
  float vel[3], vel1[3], vel2[3], del[3], dt, rt, t, mag, len, *coord,
    *out_coord, *node_data, *out_data;
  OMobj_id   cell_set,out_cell_set;
  int    ntimes, tidx, out_size, out_node_list_size;
  double *times, rel_time;
  
  
  /*printf("FUNCadvec(time=%lf, start_time=%lf, release_time=%lf, time_step_interval=%lf, dir=%d, tracer_len=%d, tracer_style=%d)\n",
   time,start_time,release_interval,time_step_interval,dir,tracer_len,tracer_style);*/
  
  /************************************************************/
  /*  if streamlines are 0-length clean the output and return */
  /************************************************************/
  if (FLDget_nnodes(in, &nnodes) != 1) {
    ERR_RETURN("cannot get nnodes");
  }
  
  if (nnodes == 0) {
    FUset_null_mesh(out);
    FUset_null_mesh(out_tracer);
    
    return(1);
  }
  
  if (dir)
    rdir = 1;
  else
    rdir = -1;
  
  /*****************************************/
  /* get streamlines coordinates and data  */
  /*****************************************/
  if (FLDget_node_data_ncomp(in, &ncomp) != 1) {
    ERR_RETURN("Error getting ncomp");
  }
  if (ncomp == 0) {
    ERR_RETURN("advec: no time  component in the input field");
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
  if (FLDget_coord(in, &coord, &size, OM_GET_ARRAY_RD) != 1) {
    ERR_RETURN("cannot get coordinates");
  }
  
  /********************************/
  /*   Free pre-allocated arrays  */
  /********************************/
  /* OUT */
  out_data = out_coord = NULL;
  FUset_null_mesh(out);
  if (FLDset_coord(out, out_coord, 0, OM_SET_ARRAY_FREE) != 1) {
    ERR_RETURN("Error setting coord");
  }
  
  /* OUT TRACER */
  FUset_null_mesh(out_tracer);
  if (FLDset_coord(out_tracer, 0, 0, OM_SET_ARRAY_FREE) != 1) {
    ERR_RETURN("Error setting coord");
  }
  out_node_list = NULL;
  
  /**********************************/
  /*   Pre-allocated output arrays  */
  /**********************************/
  out_size = ALLOC_SIZE;
  out_coord = (float *)ARRalloc(NULL, DTYPE_FLOAT, nspace*out_size, NULL);
  if (out_coord == NULL) {
    ERR_RETURN("cannot allocate coordinates");
  }
  out_data = (float *)ARRalloc(NULL, DTYPE_FLOAT, veclen*out_size, NULL);
  if (out_coord == NULL) {
    ERR_RETURN("cannot allocate node data");
  }
  out_nnodes=0;
  
  /**********************************************************/
  /* compute number of times at which to release particles  */
  /**********************************************************/
  if (release_interval != 0.0) {
    ntimes = (int)((time - start_time) / release_interval + 1.0001);
    if (ntimes < 1)
      return(0);
  }
  else
    ntimes = 1;


  /********************************************************/
  /* compute array of times at which to release particles */
  /********************************************************/
  times = (double *)ARRalloc(NULL, DTYPE_DOUBLE, ntimes, NULL);
  if (times == NULL) {
    ERR_RETURN("cannot allocate time array");
  }
  for( rel_time = time, i = ntimes; i > 0; rel_time -= release_interval, i-- ) {
    if (rel_time < 0.0)
      rel_time = 0.0;
    times[i-1] = rel_time;
  }
  
  
  /********************************************/
  /*** Copying Stream Field to Tracer Field ***/
  /********************************************/
  FUcopy_field(in, out_tracer);
  
  /**************************************************************/
  /* for each streamline (polyline) compute particles locations */
  /**************************************************************/
  if (FLDget_ncell_sets(in, &nsets) != 1) {
    ERR_RETURN("cannot get nsets");
  }
  if (tracer_len>0) {
    if (FLDset_ncell_sets(out_tracer, 1) != 1) {
      ERR_RETURN("cannot set nsets");
    }
  }
  for (cs=0; cs<nsets; cs++) {
    if (FLDget_cell_set(in, cs, &cell_set) != 1) {
      ERR_RETURN("cannot get cell set");
    }
    if (FLDget_npolys(cell_set, &ncells) != 1) {
      ERR_RETURN("cannot get ncells");
    }
    if (FLDget_poly_connect(cell_set, &node_list, 
      &size, OM_GET_ARRAY_RD) != 1) {
      ERR_RETURN("cannot get cell connectivity");
    }
    
    if (tracer_len>0) {
      out_node_list_size=ALLOC_SIZE;
      out_node_list = (int *)ARRalloc(NULL, DTYPE_INT, 2*out_node_list_size, NULL);
      if (out_node_list == NULL) {
        ERR_RETURN("cannot allocate tracer node list");
      }
    }
    else {
      out_node_list=NULL;
    }
    
    for (cell=0; cell<ncells; cell++) {
      for(t=0.0, tidx=0, node=node_list[cell*2]; node<node_list[cell*2+1]; node++) {
        vel1[0]=vel1[1]=vel1[2]=0.0;
        vel2[0]=vel2[1]=vel2[2]=0.0;
        memcpy(vel1, node_data+node*veclen, 
          veclen*sizeof(float));
        memcpy(vel2, node_data+(node+1)*veclen, 
          veclen*sizeof(float));
        
        /******************************************************************/
        /* compute average velocity along line segment, and mag(velocity) */
        /******************************************************************/
        for (i=0; i<3; i++) 
          vel[i] = (float)(0.5*(vel1[i]+vel2[i]));
        for (mag=0.0, i=0; i<veclen; i++) 
          mag += vel[i]*vel[i];
        mag = (float)sqrt((double)mag);
        
        /**************************************/
        /* compute the length of line segment */
        /**************************************/
        for (i=0; i<nspace; i++) 
          del[i] = coord[node*nspace+i]-
          coord[(node+1)*nspace+i];
        for (len=0.0, i=0; i<nspace; i++) 
          len += del[i]*del[i];
        len = (float)sqrt((double)len);
        
        /**************************************************/
        /* compute delta-time represented by line segment */
        /**************************************************/
        if (mag != 0.0)
          dt =len/mag;
        else
          dt = 0;
        t += dt;
        
        /*******************************************************/
        /* keep walking streamline until t > next release time */
        /*******************************************************/
        if (t < times[tidx])
          continue;
        
        while( (tidx < ntimes) && (t >= times[tidx]) ) {
          /**********************************************/
          /* compute particle location for each release */
          /**********************************************/
          rt = (float)(rdir*(dt-t+times[tidx]));
          if (out_nnodes >= out_size) {
            out_size += ALLOC_SIZE;
            out_coord = (float *)ARRrealloc(out_coord, DTYPE_FLOAT, nspace*out_size, NULL);
            out_data =  (float *)ARRrealloc(out_data,  DTYPE_FLOAT, veclen*out_size, NULL);
            if (!out_coord || ! out_data) {
              ERR_RETURN("Memory allocation error");
            }
          }
          /*/printf("Adding point tidx=%d, ntimes=%d, times[tidx]=%lf, out_nnodes=%d, node=%d\n",
          tidx,ntimes,times[tidx],out_nnodes,node); */
          for (i=0; i<nspace; i++) {
            out_coord[nspace*out_nnodes+i] =
              coord[nspace*node+i]/*+ rt*vel[i]*/;
          }
          for (i=0; i<veclen; i++) {
            out_data[veclen*out_nnodes+i] = vel[i];
          }
          if (out_node_list) {
            if (out_nnodes >= out_node_list_size) {
              out_node_list_size += ALLOC_SIZE;
              out_node_list = (int *)ARRrealloc(out_node_list, DTYPE_INT, 2*out_node_list_size, NULL);
            }
            out_node_list[out_nnodes*2]=get_tracer_min_node(
              node,node_list[cell*2],node_list[cell*2+1],
              tracer_len,tracer_style);
            out_node_list[out_nnodes*2+1]=get_tracer_max_node(
              node,node_list[cell*2],node_list[cell*2+1],
              tracer_len,tracer_style);
          }
          out_nnodes++;
          
          /* next release */
          tidx++;
        }
      }
    }
    
    if (node_list)
      ARRfree((char *)node_list);
  }
  
  /***************************/
  /*** TRACER OUTPUT Field ***/
  /***************************/
  if (tracer_len>0) {
    if (FLDget_cell_set(out_tracer, 0, &out_cell_set) != 1) {
      ERR_RETURN("cannot get cell set");
    }
    if (FLDset_cell_set(out_cell_set, "Polyline") != 1) {
      ERR_RETURN("cannot set cell type");
    }
    if (FLDset_npolys(out_cell_set, out_nnodes) != 1) {
      ERR_RETURN("cannot set npolys");
    }
    if (FLDset_poly_connect(out_cell_set, out_node_list, 
      2*out_nnodes, OM_SET_ARRAY_FREE) != 1) {
      ERR_RETURN("cannot set cell connectivity");
    }
  }
  
  /********************/
  /*** OUTPUT Field ***/
  /********************/
  if (FLDget_coord_units(in, units, MAX_NAME_SIZE) == 1) {
    if (FLDset_coord_units (out, units) != 1) {
      ERR_RETURN("Error setting units");
    }
  }
  if (FLDset_nspace (out, nspace) != 1) {
    ERR_RETURN("Error setting nspace");
  }
  if (FLDset_nnodes (out, out_nnodes) != 1) {
    ERR_RETURN("Error setting nnodes");
  }
  if (out_nnodes) {
    if (FLDset_coord(out, out_coord, nspace*out_nnodes, OM_SET_ARRAY_FREE) != 1) {
      ERR_RETURN("Error setting coord");
    }
  }
  else if (out_coord != NULL) 
    ARRfree(out_coord);
  
  /*********************************/
  /*  put VELOCITY into Node_Data  */
  /*********************************/
  if (FLDset_node_data_ncomp (out, 1) != 1) {
    ERR_RETURN("Error setting nnode_data");
  }
  if (FLDget_node_data_units(in, 0, units, MAX_NAME_SIZE) != 1) {
    strcpy(units, "");
  }
  if (FLDget_node_data_label(in, 0, label, MAX_NAME_SIZE) != 1) {
    strcpy(label, "");
  }
  if (FLDset_node_data_comp (out, 0, veclen, label, units) != 1) {
    ERR_RETURN("Error setting node component");
  }
  if (out_nnodes) {
    if (FLDset_node_data(out, 0, (char *)out_data, DTYPE_FLOAT, 
      veclen*out_nnodes, OM_SET_ARRAY_FREE) != 1) {
      ERR_RETURN("Error setting node data");
    }
    if (FLDcopy_node_minmax(in, out, 0, 0) != 1) {
      ERR_RETURN("Error copying node minmax");
    }		
    if (FLDcopy_node_minmax_vec(in, out, 0, 0) != 1) {
      ERR_RETURN("Error copying node minmax");
    }
  }
  else if (out_data != NULL)
    ARRfree(out_data);
  
  /*---------------*/
  /*   OUT CELLS   */
  /*---------------*/
  if (FLDset_ncell_sets(out, 1) != 1) {
    ERR_RETURN("cannot set nsets");
  }
  if (FLDget_cell_set(out, 0, &cell_set) != 1) {
    ERR_RETURN("Error getting cell set");
  }
  
  if (FLDset_cell_set(cell_set, "Point") != 1) {
    ERR_RETURN("Error setting cell type");
  }
  
  if (FLDset_ncells(cell_set, out_nnodes) != 1) {
    ERR_RETURN("Error setting ncells");
  }
  if (out_nnodes) {
    if (FLDget_node_connect(cell_set, &out_node_list, 
      &size, OM_GET_ARRAY_WR) != 1) {
      ERR_RETURN("cannot get cell connectivity");
    }
    for (i=0; i<out_nnodes; i++)
      out_node_list[i] = i;
    if (out_node_list)
      ARRfree((char *)out_node_list);
  }
  
  if (times)
    ARRfree((char *)times);
  if (node_data)
    ARRfree((char *)node_data);
  if (coord)
    ARRfree((char *)coord);
  
  /*/printf("END FUNCadvec\n"); */
  
  return(1);
}


static int reset(OMobj_id out, OMobj_id out_tracer, OMobj_id isolines)
{
/*  printf("RESET\n",tsn);*/

  FUset_null_mesh(out);
  FUset_null_mesh(out_tracer);
  FUset_null_mesh(isolines);

  return(1);
}

int tailed_advect_update(OMobj_id mod_id, OMevent_mask event_mask, int seq_num)
{
  OMobj_id in, out, out_tracer, isolines, new_probe;
  double   time, start_time, release_interval, time_step_interval;
  int      dir, tracer_len, tracer_style;
  int      ret;
  int      time_step_number;
  
  in = OMfind_subobj(mod_id, OMstr_to_name("in"), OM_OBJ_RD);
  out = OMfind_subobj(mod_id, OMstr_to_name("out"), OM_OBJ_RW);
  out_tracer = OMfind_subobj(mod_id, OMstr_to_name("out_tracer"), OM_OBJ_RW);
  isolines = OMfind_subobj(mod_id, OMstr_to_name("isolines"), OM_OBJ_RW);
  new_probe = OMfind_subobj(mod_id, OMstr_to_name("new_probe"), OM_OBJ_RW);
  
  if (OMget_name_real_val(mod_id, OMstr_to_name("time"), &time) != OM_STAT_SUCCESS)
    return(0);
  if (OMget_name_real_val(mod_id, OMstr_to_name("start_time"), &start_time) != OM_STAT_SUCCESS)
    start_time = 0.0;
  if (OMget_name_real_val(mod_id, OMstr_to_name("release_interval"), &release_interval) != OM_STAT_SUCCESS)
    release_interval = 0.0;
  if (OMget_name_real_val(mod_id, OMstr_to_name("time_step_interval"), &time_step_interval) != OM_STAT_SUCCESS)
    time_step_interval = 0.0;
  if (OMget_name_int_val(mod_id, OMstr_to_name("direction"), &dir) != OM_STAT_SUCCESS)
    return(0);
  if (OMget_name_int_val(mod_id, OMstr_to_name("tracer_len"), &tracer_len) != OM_STAT_SUCCESS)
    tracer_len = 0;
  if (OMget_name_int_val(mod_id, OMstr_to_name("tracer_style"), &tracer_style) != OM_STAT_SUCCESS)
    tracer_style = CAP;
  if (OMget_name_int_val(mod_id, OMstr_to_name("time_step_number"), &time_step_number) != OM_STAT_SUCCESS)
      time_step_number=0;

  if (release_interval != 0.) {
    ERRerror("advec",1,ERR_ORIG, "Warning : release_interval not taken into account !");
    release_interval = 0.0; /* Not implemented */
  }

  /*printf("tailed_advect_update : time = %f\n",(float)time);*/
  if (time == start_time) {
    reset(out, out_tracer, isolines);
    FUcopy_field(in, isolines);
  }
  else if (OMchanged(in, seq_num)) {
      if (time_step_interval==0 || time_step_number==0)
          FUmerge_isolines(in, isolines, out);
      else
          FUmerge_isolines(in, isolines, new_probe);
  }
  
  ret=FUNCadvec(isolines, time, start_time, release_interval, time_step_interval, dir,
                tracer_len, tracer_style, out, out_tracer);

  if (time_step_interval!=0) {
    /* time_step_interval/100 : because of rounding effect with floats */
    tsn=(int)((time+(time_step_interval/100))/time_step_interval);

    if (time_step_number != tsn) {
        /*printf("TSN = %d\n",tsn);*/
      OMset_name_int_val(mod_id, OMstr_to_name("time_step_number"), tsn);
      FUcopy_field(out, new_probe);
    }
  }

  return(ret);
}


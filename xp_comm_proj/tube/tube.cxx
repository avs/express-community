/*
   This module takes a field with line or polyline cell_sets,
   and generates tubes for the lines, which may be coloured using another
   dataset. Each line cell is turned into a tube. All other cell types
   are ignored.

   The radii are controlled by the data. The radii may be cell- or node-based.
   Cell-based radii (i.e. from the cell_data[]) ensure each tube has a
   constant radius. Node-based (i.e. from the node_data[]) allows the radius
   of the tube to vary across the cell. If there is no data, normalise is set
   and the scale value used.

   Similarly, the colours are based on cell- or node-data. Colour data is
   optional.

   The 2D mode is designed for use with output from modules like isoline. I
   calculate the tubes on the XY plane (by default) and then use the input
   field's xform to xform the tubes to the correct orientation in 3D space.

   All data must be scalar!

   Limitations:
      Does not handle NULL data
      Data must be scalar
      Data is type cast to float

   Inputs:
      in: Mesh 3D/2D with either line or polyline cell_sets. May have node
         and/or cell data to define radii and colours.
      axis: axis to project 2D data (like the output from isoline)
         perpendicular to (default is 2 = Z).
      xform: toggle whether to copy in xform to out or not. This is useful
         for 2D datasets like isoline which are calculated on the XY plane
         and then use the xform to transform them to the correct position in
         3D space.
      zscale: some 2D modules (like slice_plane) don't scale Z with X and Y.
         This allows the user to scale Z manually
      resolution: number of facets for each tube.
      normalise: toggle for normalising
         0 - no normalising - uses data for radii.
         1 - all radii set to scale - does not use data.
      scale: scale factor for radii.
      node_cell: toggle
         0 - use node data defined by node_data_comp
         1 - use cell data defined by cell_data_comp
      node_data_comp:
         the data to use if node_cell == 0
      cell_data_comp:
         the data to use if node_cell == 1
      col_data_comp:
         the node data to use for colouring. If node_cell == 0, we use node
         data, else we use cell_data.
   Outputs:
      out: Field 3D Quad cell set
      err: error toggle
      err_str: error string
   Written by:
      25/08/97
      Jeremy Maccelari
      visuals@solo.pipex.co.za
   Revisions:
      added 2D support, 26/08/97, JM
      added multiple cell_set support, 26/08/97, JM
      added xform for 2D, 26/08/97, JM
      fixed up checks for invalid cell and node data, 26/08/97, JM
      fixed up data limits, 26/07/97, JM
      changed from (OMobj_id) to .obj_id(), 19/07/99, AWD
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fld/Xfld.h>
#include "xp_comm_proj/tube/gen.hxx"

#define PI 3.1415927
#define VISMINVAL 1.0e-6
#define TUBE_DEBUG
#undef TUBE_DEBUG 

void  VISline_2_tube(float in[2][3], float radii[2], int res, float *out); 
float VISdist3D(float *p1, float *p2);
void  VISgen_rot_matrix3D(float *begin,float *end,float mat[4][4]);
void  VISgen_identity4x4(float mat[4][4]);
void  VISmult_4x4(float mat1[4][4],float mat2[4][4],float mat[4][4]);
void  VISdo_rot_point3D(float pt[3],float mat[4][4]);

int
Tube_TubeMods_tube::upd(OMevent_mask event_mask, int seq_num)
{
   // in (Field read req notify)
   // axis (OMXint read req notify)
   // xform (OMXint read req notify)
   // zscale (OMXfloat read req notify)
   // resolution (OMXint read req notify)
   // normalise (OMXint read req notify)
   // scale (OMXfloat read req notify)
   // node_cell (OMXint read req notify)
   // node_data_comp (OMXint read req notify)
   // cell_data_comp (OMXint read req notify)
   // col_data_comp (OMXint read req notify)
   // out (Field write)
   // err (OMXint write)
   // err_str (OMXstr write)
   OMevent_mask em;
   OMobj_id id;
   int i, j, k, l, m, o;
   int ax, b, ccs, cd, cdc, colour, cs, di, e, inc, inn, is, lcs, nc, ncd, ncs, ndc, nnd, norm, res, tl, twoD, xf, zs;
   int *in_ncl, *out_ncl;
   char  errstr[128];
   float *df, *in_coords, *in_cdata, *in_rdata, *out_coords, *out_data, sc;
   float data[2], pt[2][3], min, max, radii[2], *pts;

   /***********************/
   /* Function's Body     */
   /***********************/
   // get rid of compile warnings on some systems
   em = event_mask;
   i = seq_num;

   // initialise these arrays, since they might not be set later on
   out_data = in_rdata = in_cdata = NULL;

   // clear error stuff
   err = 0;
   err_str = "";
   // clear output field
   out.nnodes = 0;

   // check inputs
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"checking\n");
#endif
   // handle two exceptions immediately
   if (in.nnodes == 0) return(1);
   if ((int)in.ncell_sets <= 0)
   {
      out.nnodes = 0;
      err = 1;
      err_str = "tube: no cell sets in input.";
      return(1);
   }

   // get parameters
   res = (int)resolution;
   ax = (int)axis;
   if ((ax < 0) || (ax > 2))
   {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: axis out of range - setting to 2.");
      err = 1;
      err_str = "tube: axis out of range - setting to 2.";
      axis = ax = 2;
   }
   xf = !((int)xform == 0); // force all non-zero values to 1
   zs = (float)zscale;
   norm = !((int)normalise == 0); // force all non-zero values to 1
   sc = (float)scale;
   nc = (int)node_cell; // checked later
   cd = (int)col_data_comp;
   if (cd < 0)
   {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: colour data component is invalid - setting to 0.");
      err = 1;
      err_str = "tube: colour data component is invalid - setting to 0.";
      col_data_comp = cd = 0;
   }
   cdc = (int)cell_data_comp;
   if (cdc < 0)
   {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: cell data component is invalid - setting to 0.");
      err = 1;
      err_str = "tube: cell data component is invalid - setting to 0.";
      cell_data_comp = cdc = 0;
   }
   ndc = (int)node_data_comp;
   if (ndc < 0)
   {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: node data component is invalid - setting to 0.");
      err = 1;
      err_str = "tube: node data component is invalid - setting to 0.";
      node_data_comp = ndc = 0;
   }
   
   colour = 1; // flag colour on

   // check input dataset
   if ((int)in.ncell_sets <= 0)
   {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: input data has no cell_sets.");
      err = 1;
      err_str = "tube: input data has no cell_sets";
      return(1);
   }
   // check cell_sets
   lcs = tl = 0;
   ncs = (int)in.ncell_sets;
   for (i = 0; i < ncs; i++)
   {
      // count all the line cell sets and the total number of lines
      if (((int)in.cell_set[i].cell_ndim == 1) &&
          ((int)in.cell_set[i].cell_nnodes == 2) &&
          ((int)in.cell_set[i].cell_order == 1))
      {
         tl += (int)in.cell_set[i].ncells;
         lcs++;
      }
   }
   if (lcs == 0)
   {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: input data is not line or polyline.");
      err = 1;
      err_str = "tube: input data is not line or polyline.";
      return(1);
   }
   twoD = 0;
   if ((int)in.nspace != 3)
   {
      if ((int)in.nspace == 2)
         twoD = 1;
      else
      {
         ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: input data is not 2- or 3D.");
         err = 1;
         err_str = "tube: input data is not 2- or 3D.";
         return(1);
      }
   }
   // node_cell
   if ((nc != 0) && (nc != 1))
   {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: node_cell invalid - defaulting to 0.");
      err = 1;
      err_str = "tube: node_cell invalid - defaulting to 0.";
      node_cell = nc = 0;
   }

   // check node data
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"checking in data\n");
#endif
   if (nc == 0)
   {
      // check node data
      id = OMfind_str_subobj(in.obj_id(), "nnode_data", OM_OBJ_RD);
      if (OMis_null_obj(id))
         nnd = 0;
      else
         nnd = (int)in.nnode_data;
      // check there is some data
      if ((norm == 0) && (nnd <= 0))
      {
         ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: no node data found - normalising.");
         err = 1;
         err_str = "tube: no node data found - normalising.";
         normalise = norm = 1;
      }
      // check the radius data is in range
      if ((norm == 0) && ((ndc < 0)||(ndc >= nnd)))
      {
         ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: radius node data is out of range - normalising.");
         err = 1;
         err_str = "tube: radius node data is out of range - normalising.";
         normalise = norm = 1;
      }
      // check the radius data is scalar
      if ((norm == 0) && ((int)in.node_data[ndc].veclen != 1))
      {
         ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: radius node data is not scalar - normalising.");
         err = 1;
         err_str = "tube: radius node data is not scalar - normalising.";
         normalise = norm = 1;
      }
      // check the colour data is in range
      if ((nnd == 0) && (colour == 1))
      {
         sprintf(errstr,"tube: no node data for colour.\n");
         ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr);
         err = 1;
         err_str = errstr;
         colour = 0; // flag colour off
      }
      if ((cd < 0)||(cd >= nnd))
      {
         ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: colour node data is out of range.");
         err = 1;
         err_str = "tube: colour node data is out of range.";
         colour = 0; // flag colour off
      }
      else
      {
         // check the colour data is scalar
         
         if ((colour == 1) && (in.node_data[cd].veclen != 1))
         {
            ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: colour node data is not scalar.");
            err = 1;
            err_str = "tube: colour node data is not scalar.";
            colour = 0; // flag colour off
         }
      }
   }
   else
   {
      // check cell data for each cell_set
      for (i = 0; i < ncs; i++)
      {
         // check for line cell_set only
         if (((int)in.cell_set[i].cell_ndim == 1) &&
             ((int)in.cell_set[i].cell_nnodes == 2) &&
             ((int)in.cell_set[i].cell_order == 1))
         {
            id = OMfind_str_subobj(in.cell_set[i].obj_id(), "ncell_data", OM_OBJ_RD);
            if (!OMis_null_obj(id) && (int)in.cell_set[i].ncell_data >= 1)
               ncd = (int)in.cell_set[i].ncell_data;
            else
            {
               ncd = 0;
               if (norm == 0)
               {
                  sprintf(errstr,"tube: cell data selected, but no cell[%d] data - normalising.",i);
                  ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr); 
                  err = 1;
                  err_str = errstr;
                  normalise = norm = 1;
               }
            }
            // check there is some data
            if ((norm == 0) && (ncd <= 0))
            {
               sprintf(errstr,"tube: no cell[%d] data found - normalising.",i);
               ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr);
               err = 1;
               err_str = errstr;
               normalise = norm = 1;
            }
            // check the radii cell data is in range
            if ((norm == 0) &&((cdc < 0)||(cdc >= ncd)))
            {
               sprintf(errstr,"tube: radius cell[%d] data is out of range - normalising.",i);
               ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr);
               err = 1;
               err_str = errstr;
               normalise = norm = 1;
            }
            // check the radii data is scalar
            if ((norm == 0) &&((int)in.cell_set[i].cell_data[cdc].veclen != 1))
            {
               sprintf(errstr,"tube: radius cell[%d] data is not scalar - normalising.",i);
               ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr);
               err = 1;
               err_str = errstr;
               normalise = norm = 1;
            }
            // check the colour data is in range
            if ((ncd == 0) && (colour == 1))
            {
               sprintf(errstr,"tube: no cell data for colour.\n");
               ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr);
               err = 1;
               err_str = errstr;
               colour = 0; // flag colour off
            }
            if ((colour == 1) && ((cd < 0)||(cd >= ncd))) // this also handles ncd == 0
            {
               sprintf(errstr,"tube: colour cell[%d] data is out of range.",i);
               ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr);
               err = 1;
               err_str = errstr;
               colour = 0; // flag colour off
            }
            else
            {
               // check the colour data is scalar
               if (colour == 1)
               {
                  if (in.cell_set[i].cell_data[cd].veclen != 1)
                  {
                     sprintf(errstr,"tube: colour cell[%d] data is not scalar.",i);
                     ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr);
                     err = 1;
                     err_str = errstr;
                     colour = 0; // flag colour off
                  }
               }
            } 
         } // end of line cell_set if
      } // end of checking cell_data loop
   } // end of checking cell_data
   // check resolution
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"checking resolution\n");
#endif
   if (res < 3)
   {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: resolution must be at least 3.");
      err = 1;
      err_str = "tube: resolution must be at least 3.";
      resolution = res = 3;
   }
   
   // get input coords
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"getting input coords\n");
#endif
   inn = (int)in.nnodes;
   if ((in_coords = (float*)in.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD)) == NULL)
   {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: could not get in coords.");
      err = 1;
      err_str = "tube: could not get in coords.";
      return(1);
   }

   // do it!!
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"doing data\n");
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tot lines = %d, line cs = %d\n",tl,lcs);
#endif
   out.nspace = 3;
   out.nnodes = tl*res*2; // two nodes per res per total_cells
   out.ncell_sets = lcs;
   out.nnode_data = 1;
   out.node_data[0].veclen = 1;
   // get radius data
   if ((norm == 0) && (nc == 0))
   {
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"getting node radii data\n");
#endif
      in_rdata = (float*)in.node_data[ndc].values.ret_typed_array_ptr(OM_GET_ARRAY_RD,OM_TYPE_FLOAT);
      if (in_rdata == NULL)
      {
         sprintf(errstr,"tube: could not get in node radius data - normalising.");
         ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr);
         err = 1;
         err_str = errstr;
         normalise = norm = 1;
      }
   }
   // get node colour data, if required
   if ((colour == 1) && (nc == 0))
   {
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"getting node colour data\n");
#endif
      // get input data
      in_cdata = (float*)in.node_data[cd].values.ret_typed_array_ptr(OM_GET_ARRAY_RD,OM_TYPE_FLOAT);
      if (in_cdata == NULL)
      {
         sprintf(errstr,"tube: could not get in node colour data.");
         ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr);
         err = 1;
         err_str = errstr;
         out.nnode_data = colour = 0;
         out_data = NULL;
      }
   }

   // get output data
   if (colour == 1)
   {
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"getting out data for colour\n");
#endif
      if ((out_data = (float*)out.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_WR,OM_TYPE_FLOAT)) == NULL)
      {
         ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: could not get out data.");
         err = 1;
         err_str = "tube: could not get out data.";
         out.nnode_data = colour = 0;
         if (out_data) ARRfree((char*)out_data);
         out_data = NULL;
      }
   }
   else
      out.nnode_data = 0;

   // initialise min/max for out node_data
   if ((nc == 0) && (colour == 1))
   {
      min = (float)in.node_data[cd].min;
      max = (float)in.node_data[cd].max;
   }
   else
   {
      min = 9.99e16;
      max = -9.99e16;
   }

    // get out coords
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"getting out coords\n");
#endif
   if ((out_coords = (float*)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR)) == NULL)
   {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"tube: could not get out coords.");
      out.nnodes = 0;
      if (in_coords) ARRfree((char*)in_coords);
      if (in_cdata) ARRfree((char*)in_cdata);
      if (out_data) ARRfree((char*)out_data);
      err = 1;
      err_str = "tube: could not get out coords.";
      return(1);
   }
   // loop over cell sets
   ccs = k = l = m = 0; // output cell, output ncl, output node and output node data counters
   for (cs = 0; cs < ncs; cs++)
   {
      // do only line cell sets
      if (((int)in.cell_set[cs].cell_ndim == 1) &&
          ((int)in.cell_set[cs].cell_nnodes == 2) &&
          ((int)in.cell_set[cs].cell_order == 1))
      {
         // get node connect list for this cell set
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"getting in ncl\n");
#endif
         if ((in_ncl = (int*)in.cell_set[cs].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD)) == NULL)
         {
            sprintf(errstr,"tube: could not get in ncl[%d].",i);
            ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr);
            if (in_coords) ARRfree((char*)in_coords);
            err = 1;
            err_str = errstr;
            return(1);
         }
         // get radius data, if required
         if ((norm == 0) && (nc == 1))
         {
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"getting radii data\n");
#endif
            in_rdata = (float*)in.cell_set[cs].cell_data[cdc].values.ret_typed_array_ptr(OM_GET_ARRAY_RD,OM_TYPE_FLOAT);
            if (in_rdata == NULL)
            {
               sprintf(errstr,"tube: could not get in radius data[%d] - normalising.",i);
               ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr);
               err = 1;
               err_str = errstr;
               normalise = norm = 1;
            }
         }
         // get cell colour data, if required
         if ((colour == 1) && (nc == 1))
         {
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"getting colour data\n");
#endif
            in_cdata = (float*)in.cell_set[cs].cell_data[cd].values.ret_typed_array_ptr(OM_GET_ARRAY_RD,OM_TYPE_FLOAT);
            if (in_cdata == NULL)
            {
               sprintf(errstr,"tube: could not get in colour data[%d].",i);
               ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr);
               err = 1;
               err_str = errstr;
               out.nnode_data = colour = 0;
               if (out_data) ARRfree((char*)out_data);
               out_data = NULL;
            }
            if (colour == 1)
            {
               if (in.cell_set[cs].cell_data[cd].min < min)
                  min = in.cell_set[cs].cell_data[cd].min;
               if (in.cell_set[cs].cell_data[cd].max > max)
                  max = in.cell_set[cs].cell_data[cd].max;
            }
         }
         //create output cell_set[current cell set] - quads
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"setting cell set[%d] header\n",ccs);
#endif
         out.cell_set[ccs].ncells = (int)in.cell_set[cs].ncells*res;
         out.cell_set[ccs].cell_nnodes = 4;
         out.cell_set[ccs].cell_ndim = 2;
         out.cell_set[ccs].cell_order = 1;
         out.cell_set[ccs].poly_flag = 0;
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"getting out ncl\n");
#endif
         if ((out_ncl = (int*)out.cell_set[ccs].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR)) == NULL)
         {
            sprintf(errstr,"tube: could not get out ncl[%d].",i);
            ERRverror("",ERR_PRINT|ERR_NO_HEADER,"%s",errstr);
            out.nnodes = 0;
            if (in_coords) ARRfree((char*)in_coords);
            if (in_ncl) ARRfree((char*)in_ncl);
            if (in_cdata) ARRfree((char*)in_cdata);
            if (in_rdata) ARRfree((char*)in_rdata);
            if (out_data) ARRfree((char*)out_data);
            if (out_coords) ARRfree((char*)out_coords);
            err = 1;
            err_str = errstr;
            return(1);
         }
         // create revolved points
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"creating points\n");
#endif
         pts = (float*)malloc(6*res*sizeof(float));
         // loop through each line segment, creating tubes
         o = 0; // this ncl counter
         inc = (int)in.cell_set[cs].ncells;
         for (i = 0; i < inc; i++)
         {
            // get the node numbers for the two current points
            b = in_ncl[i*2];
            e = in_ncl[i*2+1];
            // get the coords for these two nodes
            if (twoD == 0)
            {
               pt[0][0] = in_coords[b*3+0];
               pt[0][1] = in_coords[b*3+1];
               pt[0][2] = in_coords[b*3+2];
               pt[1][0] = in_coords[e*3+0];
               pt[1][1] = in_coords[e*3+1];
               pt[1][2] = in_coords[e*3+2];
            }
            else
            {
               switch (axis)
               {
                  case 0:
                     pt[0][0] = (float)0.;
                     pt[0][1] = in_coords[b*2+0];
                     pt[0][2] = in_coords[b*2+1];
                     pt[1][0] = (float)0.;
                     pt[1][1] = in_coords[e*2+0];
                     pt[1][2] = in_coords[e*2+1];
                  break;
                  case 1:
                     pt[0][0] = in_coords[b*2+0];
                     pt[0][1] = (float)0.;
                     pt[0][2] = in_coords[b*2+1];
                     pt[1][0] = in_coords[e*2+0];
                     pt[1][1] = (float)0.;
                     pt[1][2] = in_coords[e*2+1];
                  break;
                  case 2:
                     pt[0][0] = in_coords[b*2+0];
                     pt[0][1] = in_coords[b*2+1];
                     pt[0][2] = (float)0.;
                     pt[1][0] = in_coords[e*2+0];
                     pt[1][1] = in_coords[e*2+1];
                     pt[1][2] = (float)0.;
                  break;
               }
            }
            // get the radius and colour data for these two points
            if (norm == 0)
            {
               if (nc == 0)
               {
                  radii[0] = in_rdata[b]*sc;
                  radii[1] = in_rdata[e]*sc;
                  if (colour == 1)
                  {
                     data[0] = in_cdata[b];
                     data[1] = in_cdata[e];
                  }
                  else
                     data[0] = data[1] = 0.;
               }
               else
               {
                  radii[0] = radii[1] = in_rdata[i]*sc;
                  if (colour == 1)
                     data[0] = data[1] = in_cdata[i];
                  else
                     data[0] = data[1] = 0.;
               }
            }
            else
            {
               if (nc == 0)
               {
                  radii[0] = sc;
                  radii[1] = sc;
                  if (colour == 1)
                  {
                     data[0] = in_cdata[b];
                     data[1] = in_cdata[e];
                  }
                  else
                     data[0] = data[1] = 0.;
               }
               else
               {
                  radii[0] = radii[1] = sc;
                  if (colour == 1)
                     data[0] = data[1] = in_cdata[i];
                  else
                     data[0] = data[1] = 0.;
               }
            }
   
            // generate the tube from the two current points
            VISline_2_tube(pt,radii,res,pts); 
            // set coords
            for (j = 0; j < (res*2); j++)
            {
               out_coords[k] = pts[j*3+0]; k++;
               out_coords[k] = pts[j*3+1]; k++;
               out_coords[k] = pts[j*3+2]; k++;
            }
            // set node connect list
            for (j = 0; j < res; j++)
            {
               out_ncl[o] = l/2+0; o++; l++;
               out_ncl[o] = l/2+1; o++; l++;
               if (j != (res-1))
               {
                  if (j == 0) b = o-2;
                  out_ncl[o] = out_ncl[o-1]+2; o++; l++;
                  out_ncl[o] = out_ncl[o-1]-1; o++; l++;
               }
               else
               {
                  out_ncl[o] = out_ncl[b+1]; o++; l++;
                  out_ncl[o] = out_ncl[b]; o++; l++;
               }
            }
            // set data
            if (colour == 1)
               for (j = 0; j < res; j++)
               {
                  out_data[m] = data[0]; m++;
                  out_data[m] = data[1]; m++;
               }
         } // end of points loop
         if (pts) free((char*)pts);
         // free for this cell set
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"freeing cell memory\n");
#endif
         if (in_ncl) ARRfree((char*)in_ncl);
         if (out_ncl) ARRfree((char*)out_ncl);
         // only free data if colour == 0, or cell data
         if ((colour == 0) || (nc == 1))
         {
            if (in_cdata) ARRfree((char*)in_cdata);
            if (in_rdata) ARRfree((char*)in_rdata);
         }
         // increment current cell set
         ccs++;
      } // end of line cell set if
   } // end of cell_sets loop
   // free memory
#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"freeing out memory\n");
#endif
   if (in_coords) ARRfree((char*)in_coords);
   if (out_coords) ARRfree((char*)out_coords);
   if (out_data) ARRfree((char*)out_data);

   // set colour min/max
   if (colour == 1)
   {
      OMset_data_type(out.node_data[0].min.obj_id(), OM_TYPE_FLOAT);
      OMset_data_type(out.node_data[0].max.obj_id(), OM_TYPE_FLOAT);
      out.node_data[0].min = min;
      out.node_data[0].max = max;
   }
   
   // set xform for 2D objects, if necessary
   if ((twoD == 1) && (xf == 1))
   {
      // check in.xform is OK
      id = OMfind_str_subobj(in.obj_id(),"xform",OM_OBJ_RD);
      if (!OMis_null_obj(id))
      {
         df = (float*)in.xform.mat.ret_typed_array_ptr(OM_GET_ARRAY_RD,OM_TYPE_FLOAT,&di);
         out.xform.mat.set_array(OM_TYPE_FLOAT,(char*)df,16);
         if (df) ARRfree((char*)df);
         // do Z scaling
         df = (float*)out.xform.mat.ret_typed_array_ptr(OM_GET_ARRAY_RD,OM_TYPE_FLOAT,&di);
         df[8] *= zs;
         df[9] *= zs;
         df[10] *= zs;
         out.xform.mat.set_array(OM_TYPE_FLOAT,(char*)df,16);
         if (df) ARRfree((char*)df);
         
         df = (float*)in.xform.xlate.ret_typed_array_ptr(OM_GET_ARRAY_RD,OM_TYPE_FLOAT,&di);
         out.xform.xlate.set_array(OM_TYPE_FLOAT,(char*)df,3);
         if (df) ARRfree((char*)df);
         df = (float*)in.xform.center.ret_typed_array_ptr(OM_GET_ARRAY_RD,OM_TYPE_FLOAT,&di);
         out.xform.center.set_array(OM_TYPE_FLOAT,(char*)df,3);
         if (df) ARRfree((char*)df);
      }
      else
      {
         // set out.xform to default
         df = (float*)malloc(16*sizeof(float));
         for (i = 0; i < 16; i++)
         {
            if ((i%5) != 0)
               df[i] = (float)0.;
            else
               df[i] = (float)1.;
         }
         out.xform.mat.set_array(OM_TYPE_FLOAT,(char*)df,16);
         if (df) free((char*)df);
         df = (float*)malloc(3*sizeof(float));
         for (i = 0; i < 3; i++) df[i] = (float)0.;
         out.xform.xlate.set_array(OM_TYPE_FLOAT,(char*)df,3);
         out.xform.center.set_array(OM_TYPE_FLOAT,(char*)df,3);
         if (df) free((char*)df);
      }
   }
   else
   {
      // set out.xform to default
      df = (float*)malloc(16*sizeof(float));
      for (i = 0; i < 16; i++)
      {
         if ((i%5) != 0)
            df[i] = (float)0.;
         else
            df[i] = (float)1.;
      }
      out.xform.mat.set_array(OM_TYPE_FLOAT,(char*)df,16);
      if (df) free((char*)df);
      df = (float*)malloc(3*sizeof(float));
      for (i = 0; i < 3; i++) df[i] = (float)0.;
      out.xform.xlate.set_array(OM_TYPE_FLOAT,(char*)df,3);
      out.xform.center.set_array(OM_TYPE_FLOAT,(char*)df,3);
      if (df) free((char*)df);
   }

#ifdef TUBE_DEBUG
ERRverror("",ERR_PRINT|ERR_NO_HEADER,"done.\n");
#endif
      // return 1 for success
   return(1);
}

/* ===========================================================================
   Generates a tube around the line defined by two points.
   Inputs:
      in[2][3] : (float) begin and end points
      radii[2] : (float) the radii at the begin and end points
      res      : (int) the number of segments in the tube
   Outputs:
      out[res*2][3] : (float) the nodes defining the tube
   Return value:
      none
*/
void VISline_2_tube(float in[2][3], float radii[2], int res, float *out)
{
   int k, r;
   float dp1[3], dp2[3], len, mat[4][4]; 

   // calculate length of this segment
   dp1[0] = in[0][0];
   dp1[1] = in[0][1];
   dp1[2] = in[0][2];
   dp2[0] = in[1][0];
   dp2[1] = in[1][1];
   dp2[2] = in[1][2];
   len = VISdist3D(dp1,dp2);

   // generate rotation matrix
   VISgen_rot_matrix3D(dp1,dp2,mat);
   // do revolution, rotation & translation
   k = 0;
   for (r = 0; r < res; r++)
   {
      dp1[0] = 0.;
      dp1[1] = radii[0]*sin(2*PI*(float)r/(float)res);
      dp1[2] = radii[0]*cos(2*PI*(float)r/(float)res);
      dp2[0] = len;
      dp2[1] = radii[1]*sin(2*PI*(float)r/(float)res);
      dp2[2] = radii[1]*cos(2*PI*(float)r/(float)res);
      // rotate points
      VISdo_rot_point3D(dp1,mat);
      VISdo_rot_point3D(dp2,mat);
      // translate points
      dp1[0] += in[0][0];
      dp1[1] += in[0][1];
      dp1[2] += in[0][2];
      dp2[0] += in[0][0];
      dp2[1] += in[0][1];
      dp2[2] += in[0][2];
      // set out 
      out[k] = dp1[0]; k++;
      out[k] = dp1[1]; k++;
      out[k] = dp1[2]; k++;
      out[k] = dp2[0]; k++;
      out[k] = dp2[1]; k++;
      out[k] = dp2[2]; k++;
   }
}

/* ===========================================================================
   Calculates the 3D cartesian distance
   Inputs:
      p0[3] : (float) begin point
      p1[3] : (float) end point
   Outputs:
   Return value:
      (float) cartesian distance
*/
float VISdist3D(float *p1, float *p2)
{
   return(sqrt(pow((p1[0]-p2[0]),2)+pow((p1[1]-p2[1]),2)+pow((p1[2]-p2[2]),2)));
}

/* ===========================================================================
   Calculates the matrix to rotate a line to the same orientation as two points
   in 3D space
   Inputs:
      begin[3]  : (float*) the begin point (X,Y,Z)
      end[3]    : (float*) the end point (X,Y,Z)
   Outputs:
      mat[4][4] : (float*) the rotation matrix (quaternion)
   Return value:
      none
*/
void VISgen_rot_matrix3D(float *begin,float *end,float mat[4][4])
{
   float cy, cz, dx, dy, dz, hxy, hxyz, ry[4][4], rz[4][4], sy, sz;
   float dp1[3], dp2[3];

   // fill ry and rz
   // create identity matrices
   VISgen_identity4x4(ry);
   VISgen_identity4x4(rz);

   // set origin
   dp1[0] = dp1[1] = dp1[2] = 0.;
   // set point
   dx = end[0] - begin[0];
   dy = end[1] - begin[1];
   dz = end[2] - begin[2];

   // calculate lengths
   dp2[0] = dx;
   dp2[1] = dy;
   dp2[2] = 0.;
   hxy = VISdist3D(dp1,dp2);
   dp2[2] = dz;
   hxyz = VISdist3D(dp1,dp2);

   // Y rotation first
   if (hxyz > VISMINVAL)
   {
      sy =  dz/hxyz;
      cy = hxy/hxyz;
   }
   else
   {
      sy = 0.;
      cy = 1.;
   }
   // then Z rotation
   if (hxy > VISMINVAL)
   {
      sz = dy/hxy;
      cz = dx/hxy;
   }
   else
   {
      sz = 0.;
      if (dx > 0)
         cz =  1.;
      else
         cz = -1.;
   }
   // fill ry - i runs from LHS to RHS, j from T to B
   ry[0][0] =  cy;
   ry[2][0] =  sy; //  sy
   ry[0][2] = -sy; // -sy
   ry[2][2] =  cy;
   // fill rz - i runs from LHS to RHS, j from T to B
   rz[0][0] =  cz;
   rz[1][0] =  sz;
   rz[0][1] = -sz;
   rz[1][1] =  cz;

   // now concatenate the matrices
   VISmult_4x4(ry,rz,mat);
}

/* ===========================================================================
   Sets the matrix to the identity
   Inputs:
      mat[4][4] : (float*) the matrix
   Outputs:
      mat[4][4] : (float*) the matrix set to the identity
   Return value:
      none
*/
void  VISgen_identity4x4(float mat[4][4])
{
   int i, j;

   for (j = 0; j < 4; j++)
   {
     for (i = 0; i < 4; i++)
      {
         if (i != j)
            mat[i][j] = 0.;
         else
            mat[i][i] = 1.;
      }
   }
}

/* ===========================================================================
   Performs the matrix multiplcation MAT = MAT1*MAT2
   Inputs:
      mat1[4][4] : (float*) the first matrix
      mat1[4][4] : (float*) the second matrix
   Outputs:
      mat[4][4] : (float*) the matrix product
   Return value:
      none
*/
void  VISmult_4x4(float mat1[4][4],float mat2[4][4],float mat[4][4])
{
   float df;
   int i, j, k;

   for (i = 0; i < 4; i++)
   {
      for (j = 0; j < 4; j++)
      {
         df = 0.;
         for (k = 0; k < 4; k++)
           df += mat2[i][k]*mat1[k][j];
         mat[i][j] = df;
      }
   }
}

/* ===========================================================================
   Rotates the point in 3D space using the matrix
   Inputs:
      pt[3]     : (float*) the point (X,Y,Z) to be rotated
      mat[4][4] : (float*) the rotation matrix (quaternion)
   Outputs:
      pt[3] : the rotated point
   Return value:
      none
*/
void VISdo_rot_point3D(float pt[3],float mat[4][4])
{
   float rpt[4], tpt[4], df;
   int i, j;

   // generate the point in homogeneous coords
   tpt[0] = pt[0];
   tpt[1] = pt[1];
   tpt[2] = pt[2];
   tpt[3] = 1.;

   for (i = 0; i < 4; i++)
   {
      df = 0.;
      for (j = 0; j < 4; j++)
         df += mat[i][j]*tpt[j];
      rpt[i] = df;
   }
   for (i = 0; i < 3; i++)
      pt[i] = rpt[i];
}
#ifdef TUBE_DEBUG
#undef TUBE_DEBUG
#endif

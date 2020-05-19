// by James S Perrin  Time-stamp: <Friday 06/09/02 14:46:48 zzcgujp>
// developed as part of the Sir Jules Thorn Charitable Trust funded project CAMRAS
// dicom_orthoslice_unif.cxx
// assumes veclen=1

#include "gen.h"

int DICOMOrtho_DICOM_orthoslice_unif::orthoslice(OMevent_mask event_mask, int seq_num)
{
  // in (Mesh_Unif+Node_Data read notify)
  // axis (OMXint read notify)
  // plane (OMXint read notify)
  // out (Mesh_Unif+Node_Data write)
  
  int *in_dims = (int *)in.dims.ret_array_ptr(OM_GET_ARRAY_RD);
  float *in_points = (float *)in.points.ret_array_ptr(OM_GET_ARRAY_RD);
  int  in_data_size, in_data_type;
  int *out_dims;
  float *out_points;
  int  comp;
  int  out_data_size, out_data_type;
  int a, p, d;  // local axis, plane and  total depth
  float pr;  //plane in real coords
  int w, h;  // slice width and height 
  
  // check for correct values
  if(!in.nnodes.valid_obj() || (int)in.nnodes == 0)
    return 1;
  if((int)in.ndim!=3) {
    ERRverror("",ERR_NO_HEADER | ERR_PRINT,"ESP_MPR_orthoslice_unif: in must has ndim = 3\n");
    if (in_dims) ARRfree(in_dims);
    if (in_points) ARRfree(in_points);
    return 1;
  }
  if((int)axis>2 || (int)axis<0) {
    ERRverror("",ERR_NO_HEADER | ERR_PRINT,"ESP_MPR_orthoslice_unif: axis must 0, 1 or 2\n");
    if (in_dims) ARRfree(in_dims);
    if (in_points) ARRfree(in_points);
    return 1;
  }
  if((int)plane<0 || (int)plane>in_dims[(int)axis]) {
    // be quite and do nothing
    return 1;
  }
  
  a = axis; p = plane;  
  
  // initialise output from input
  out.ndim = 2;
  out.nspace = 2;
  
  if(out_dims = (int *)out.dims.ret_array_ptr(OM_GET_ARRAY_WR)) {
    switch (a) {
    case 0:
      w = out_dims[0] = in_dims[1];
      h = out_dims[1] = in_dims[2];
      break;
    case 1:
      w = out_dims[0] = in_dims[0];
      h = out_dims[1] = in_dims[2];
      break;
    case 2:
      w = out_dims[0] = in_dims[0];
      h = out_dims[1] = in_dims[1];
      break;
    default:
      break;
    }
    
    d = in_dims[a];
    ARRfree(out_dims);
  } else
    return 1;
  
  if(out_points = (float *)out.points.ret_array_ptr(OM_GET_ARRAY_WR)) {
    switch (a) {
    case 0:
      out_points[0] = in_points[1];
      out_points[1] = in_points[2];
      out_points[2] = in_points[4];
      out_points[3] = in_points[5];
      pr = (float)(p * (in_points[3]-in_points[0])/(d-1));
      break;
    case 1:
      out_points[0] = in_points[0];
      out_points[1] = in_points[2];
      out_points[2] = in_points[3];
      out_points[3] = in_points[5];
      pr = (float)(p * (in_points[4]-in_points[1])/(d-1));
     break;
    case 2:
      out_points[0] = in_points[0];
      out_points[1] = in_points[1];
      out_points[2] = in_points[3];
      out_points[3] = in_points[4];
      pr = (float)(p * (in_points[5]-in_points[2])/(d-1));
      break;
      
    }
    ARRfree(out_points);
  } else 
    return 1;

  if (in_dims) ARRfree(in_dims);
  if (in_points) ARRfree(in_points);

  out.nnode_data = (int)in.nnode_data;
  
  for(comp = 0; comp < in.nnode_data; comp++) { 
    char *in_data, *out_data;
    unsigned char *in_byte_data, *out_byte_data;
    int *in_int_data, *out_int_data;
    short *in_short_data, *out_short_data;
    float *in_float_data, *out_float_data;
    double *in_double_data, *out_double_data;
    int i, j;
    int in_offset, out_offset, row_offset;
    
    in_data = (char *)in.node_data[comp].values.ret_array_ptr(OM_GET_ARRAY_RD, &in_data_size, &in_data_type);
    out.node_data[comp].veclen = in.node_data[comp].veclen;
    
    switch (a) {
    case 0:
      in_offset = p;
      out_offset = 0;
      row_offset = w*d;
      
      switch (in_data_type) {
      case OM_TYPE_CHAR:
      case OM_TYPE_BYTE:
	out_data = (char *)malloc(w*h);
	out_byte_data = (unsigned char *)out_data;
	in_byte_data = (unsigned char *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_byte_data[out_offset+i] = in_byte_data[in_offset + i*d];
	  
	  in_offset+=row_offset;
	  out_offset+=w;
	}
	break;
      case OM_TYPE_SHORT:
	out_data = (char *)malloc(w*h*sizeof(short));
	out_short_data = (short *)out_data;
	in_short_data = (short *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_short_data[out_offset+i] = in_short_data[in_offset + i*d];
	  
	  in_offset+=row_offset;
	  out_offset+=w;
	}
	break;
      case OM_TYPE_INT:
	out_data = (char *)malloc(w*h*sizeof(int));
	out_int_data = (int *)out_data;
	in_int_data = (int *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_int_data[out_offset+i] = in_int_data[in_offset + i*d];
	  
	  in_offset+=row_offset;
	  out_offset+=w;
	}
	break;
      case OM_TYPE_FLOAT:
	out_data = (char *)malloc(w*h*sizeof(float));
	out_float_data = (float *)out_data;
	in_float_data = (float *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_float_data[out_offset+i] = in_float_data[in_offset + i*d];
	  
	  in_offset+=row_offset;
	  out_offset+=w;
	}
	break;
      case OM_TYPE_DOUBLE:
	out_data = (char *)malloc(w*h*sizeof(double));
	out_double_data = (double *)out_data;
	in_double_data = (double *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_double_data[out_offset+i] = in_double_data[in_offset + i*d];
	  
	  in_offset+=row_offset;
	  out_offset+=w;
	}
	break;
      default:
	break;
      }
      break;
      
    case 1:
      in_offset = w*p;
      out_offset = w-1;
      row_offset = w*d;
      
      switch (in_data_type) {
      case OM_TYPE_CHAR:
      case OM_TYPE_BYTE:
	out_data = (char *)malloc(w*h);
	out_byte_data = (unsigned char *)out_data;
	in_byte_data = (unsigned char *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_byte_data[out_offset-i] = in_byte_data[in_offset + i];
	  
	  in_offset+=row_offset;
	  out_offset+=w;
	}
	break;
      case OM_TYPE_SHORT:
	out_data = (char *)malloc(w*h*sizeof(short));
	out_short_data = (short *)out_data;
	in_short_data = (short *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_short_data[out_offset-i] = in_short_data[in_offset + i];
	  
	  in_offset+=row_offset;
	  out_offset+=w;
	}
	break;
      case OM_TYPE_INT:
	out_data = (char *)malloc(w*h*sizeof(int));
	out_int_data = (int *)out_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_int_data[out_offset-i] = in_int_data[in_offset + i];
	  
	  in_offset+=row_offset;
	  out_offset+=w;
	}
	break;
      case OM_TYPE_FLOAT:
	out_data = (char *)malloc(w*h*sizeof(float));
	out_float_data = (float *)out_data;
	in_float_data = (float *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_float_data[out_offset-i] = in_float_data[in_offset + i];
	  
	  in_offset+=row_offset;
	  out_offset+=w;
	}
	break;
      case OM_TYPE_DOUBLE:
	out_data = (char *)malloc(w*h*sizeof(double));
	out_double_data = (double *)out_data;
	in_double_data = (double *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_double_data[out_offset-i] = in_double_data[in_offset + i];
	  
	  in_offset+=row_offset;
	  out_offset+=w;
	}
	break;
      default:
	break;
      }
      break;
      
    case 2:
      in_offset = w*h*p;
      out_offset = w-1;
      
      switch (in_data_type) {
      case OM_TYPE_CHAR:
      case OM_TYPE_BYTE:
	out_data = (char *)malloc(w*h);
	out_byte_data = (unsigned char *)out_data;
	in_byte_data = (unsigned char *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_byte_data[out_offset-i] = in_byte_data[in_offset + i];
	  
	  in_offset+=w;
	  out_offset+=w;
	}
	break;
      case OM_TYPE_SHORT:
	out_data = (char *)malloc(w*h*sizeof(short));
	out_short_data = (short *)out_data;
	in_short_data = (short *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_short_data[out_offset-i] = in_short_data[in_offset + i];
	  
	  in_offset+=w;
	  out_offset+=w;
	}
	break;
      case OM_TYPE_INT:
	out_data = (char *)malloc(w*h*sizeof(int));
	out_int_data = (int *)out_data;
	in_int_data = (int *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_int_data[out_offset-i] = in_int_data[in_offset + i];
	  
	  in_offset+=w;
	  out_offset+=w;
	}
	break;
      case OM_TYPE_FLOAT:
	out_data = (char *)malloc(w*h*sizeof(float));
	out_float_data = (float *)out_data;
	in_float_data = (float *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_float_data[out_offset-i] = in_float_data[in_offset + i];
	  
	  in_offset+=w;
	  out_offset+=w;
	}
	break;
      case OM_TYPE_DOUBLE:
	out_data = (char *)malloc(w*h*sizeof(double));
	out_double_data = (double *)out_data;
	in_double_data = (double *)in_data;
	for(j=0; j<h; j++)
	{
	  for(i=0; i<w; i++)
	    out_float_data[out_offset-i] = in_float_data[in_offset + i];
	  
	  in_offset+=w;
	  out_offset+=w;
	}
	break;
      default:
	break;
      }
      break;
    default:
      break;
    };

    out.node_data[comp].values.set_array(in_data_type, out_data, w*h, OM_SET_ARRAY_FREE);
    if (in_data) ARRfree(in_data);

    // set min/max
    out.node_data[comp].min.set_obj_ref(in.node_data[comp].min.obj_id());
    out.node_data[comp].max.set_obj_ref(in.node_data[comp].max.obj_id());
  }

  //   ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: ESP_MPR_orthoslice_unif::orthoslice\n");

  // do xforms
  float mats[3][16] = {
    {0.,1.,0.,0.,0.,0.,1.,0.,1.,0.,0.,0.,0.,0.,0.,1.},
    {1.,0.,0.,0.,0.,0.,1.,0.,0.,-1.,0.,0.,0.,0.,0.,1.},
    {1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.}};
  float xlates[3][3] = {{pr, 0, 0},{0,pr, 0},{0, 0, pr}};
  
  out.xform.mat.set_array(OM_TYPE_FLOAT, mats[a], 16, OM_SET_ARRAY_COPY);
  out.xform.xlate.set_array(OM_TYPE_FLOAT, xlates[a], 3, OM_SET_ARRAY_COPY);
  
  return 1;
}

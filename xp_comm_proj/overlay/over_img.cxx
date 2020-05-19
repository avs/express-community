// over_img.cxx
// James S Perrin Jan 1999

// overlay image2 on image1 from bottom left corner
// mask specifies parts of image2 that are transparent and allow image 1 to show through.
// if image2 is larger than image1 in any dimension pad out with bkgnd colour

#include <iostream.h>

#include "fld/Xfld.h"
#include "xp_comm_proj/overlay/gen.h"

int Overlay_OverlayMods_OverlayNodeData::overlay(OMevent_mask event_mask, int seq_num)
{
  // check images are valid objs
  if( !(image1.nnodes && image2.nnodes))
    return 1;

  // mask (OMXint_array read req notify)
  int mask_size;
  unsigned char *mask_arr;

  // fill (OMXint_array read req notify)
  int bkgnd_size;
  unsigned char *bkgnd_arr;

  // image1 (Field_Unif read req notify)
  int *image1_dims = (int *)image1.dims.ret_array_ptr(OM_GET_ARRAY_RD);
  int  image1_data_size, image1_data_type;
  unsigned char *image1_node_data =
    (unsigned char *)image1.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RD,
							      &image1_data_size,
							      &image1_data_type);
  if (!image1_node_data)
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"overlay: can't access image1 node_data\n");
    return 1;
  }

  // image2 (Field_Unif read req notify)
  int *image2_dims = (int *)image2.dims.ret_array_ptr(OM_GET_ARRAY_RD);
  int  image2_data_size, image2_data_type;
  unsigned char *image2_node_data =
    (unsigned char *)image2.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RD,
							      &image2_data_size,
							      &image2_data_type);
  if (!image2_node_data)
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"overlay: can't access image2 node_data\n");
    ARRfree((char *)image1_node_data);
    return 1;
  }

  // check images are both argb
  if( image1.node_data[0].id!=669 || image2.node_data[0].id!=669  )
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"overlay: images must be argb\n");
    return 1;
  }

  // Compare sizes of images
  int IMGSEQ=0;  // images are the same size
  int IMG2GX=0;  // image2 greater than image1 in X
  int IMG2GY=0;  // image2 greater than image1 in Y
  
  if( image1_dims[0]==image2_dims[0] && image1_dims[1]==image2_dims[1] )
    IMGSEQ=1;
  else
  {
    if(image2_dims[0] > image1_dims[0])
      IMG2GX=1;
    if(image2_dims[1] > image1_dims[1])
      IMG2GY=1;
  }
  
  // compos (Field_Unif write)
  compos.ndim=2;
  int *compos_dims = (int *)compos.dims.ret_array_ptr(OM_GET_ARRAY_WR);
  compos.nspace=2;
  compos.npoints=2;
  float *compos_points = (float *)compos.points.ret_array_ptr(OM_GET_ARRAY_WR);
  int  compos_data_size;

  // set compos dims to contain both images positioned upper left 
  compos_dims[0]=image1_dims[0]>image2_dims[0]?image1_dims[0]:image2_dims[0];
  compos_dims[1]=image1_dims[1]>image2_dims[1]?image1_dims[1]:image2_dims[1];
  compos.nnodes=compos_dims[0]*compos_dims[1];

  // set points
  compos_points[0]=0;
  compos_points[1]=0;
  compos_points[2]=compos_dims[0]-1;
  compos_points[3]=compos_dims[1]-1;
  ARRfree((char *)compos_points);

  // set single node_data
  compos.nnode_data=1;
  compos.node_data[0].id=669; // argb data
  compos.node_data[0].veclen=4;
  // need to use ret_typed_array_ptr as the values array is instanced as a prim
  unsigned char *compos_node_data = (unsigned char *)compos.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_WR, OM_TYPE_BYTE, &compos_data_size);

  if(!compos_node_data)
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"overlay: can't access compos node_data\n");
    ARRfree((char *)image1_node_data);
    ARRfree((char *)image2_node_data);
    return 1;
  }

  mask_arr = (unsigned char *)mask.ret_array_ptr(OM_GET_ARRAY_RD,&mask_size);
  bkgnd_arr = (unsigned char *)bkgnd.ret_array_ptr(OM_GET_ARRAY_RD,&bkgnd_size);
  
  if(!(mask_arr && bkgnd_arr))
  {
    ERRverror("",ERR_NO_HEADER | ERR_INFO,"overlay: can't access mask or background\n");
    return 1 ;
  }

  int i,j,k;
  int jwidth1, jwidth2, index1, index2; // loop constants
  
  // <<<< Pass 1: Write Image1 to output >>>>
  // loops work on images dims
  for(j=0; j<image1_dims[1]; j++)
  {
    jwidth1=j*compos_dims[0];
    jwidth2=j*image1_dims[0];
    
    for(i=0; i<image1_dims[0]; i++)
    {
      index1=(jwidth1+i)*4;
      index2=(jwidth2+i)*4;
      
      for(k=0; k<4; k++)
	compos_node_data[index1+k]=image1_node_data[index2+k];
    }
  }
  
  // <<<< Pass 2: Write Background >>>>
  if(!IMGSEQ)
  {
    // fill background above image1 in output
    if(IMG2GY)
      for(j=image1_dims[1]; j<compos_dims[1]; j++)
      {
	jwidth1=j*compos_dims[0];
	
	for(i=0; i<compos_dims[0]; i++)
	{
	  index1=(jwidth1+i)*4;
	  
	  for(k=0; k<4; k++)
	    compos_node_data[index1+k]=bkgnd_arr[k];
	}
      }
    
    // fill background right of image1 in output
    if(IMG2GX)
      for(j=0; j<image1_dims[1]; j++)
      {
	jwidth1=j*compos_dims[0];

	for(i=image1_dims[0]; i<compos_dims[0]; i++)
	{
	  index1=(jwidth1+i)*4;
		  
	  for(k=0; k<4; k++)
	    compos_node_data[index1+k]=bkgnd_arr[k];
	}
      }
  }
  
  // <<<< Pass 3: Write Image2 to output >>>>
  for(j=0; j<image2_dims[1]; j++)
  {
    jwidth1=j*compos_dims[0];
    jwidth2=j*image2_dims[0];
    
    for(i=0; i<image2_dims[0]; i++)
    {
      index1=(jwidth1+i)*4;
      index2=(jwidth2+i)*4;

      // test if pixel is ARGB = mask
      for(k=0; k<4; k++)
	if(image2_node_data[index2+k]!=mask_arr[k])
	  break;
	   
      // if not mask write image2 pixel
      if(k<4)
	for(k=0; k<4; k++)
	  compos_node_data[index1+k]=image2_node_data[index2+k];
    }
  }
  
  // free all the arrays
  ARRfree((char *)mask_arr);
  ARRfree((char *)bkgnd_arr);
  ARRfree((char *)image1_dims);
  ARRfree((char *)image2_dims);
  ARRfree((char *)compos_dims);
  ARRfree((char *)image1_node_data);
  ARRfree((char *)image2_node_data);
  ARRfree((char *)compos_node_data);

  // return 1 for success
  return 1;
}


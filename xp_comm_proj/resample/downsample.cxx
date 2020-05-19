#include <math.h>
#include "gen.h"

//#define DEBUG

#ifdef DEBUG
#include <iostream.h>
#endif

#define MIN(a,b) (a)<(b)?(a):(b)
#define MAX(a,b) (a)>(b)?(a):(b)

#define lookupTable(a,b,c) (c*Size[0]*Size[1]+b*Size[0]+a)
#define lookupTableOut(a,b,c) (c*out_field_dims[0]*out_field_dims[1]+b*out_field_dims[0]+a)

int
DownSample_downsample::resample(OMevent_mask event_mask, int seq_num)
{
	int nnodes, ndim, veclen;
	if(in_field.nnodes==0)
		 return 1;
		 
#ifdef DEBUG
fprintf(stderr,"start downsample\n");
#endif
	int *in_field_dims = (int *)in_field.dims.ret_array_ptr(OM_GET_ARRAY_RD);
	float *in_field_points = (float *)in_field.points.ret_array_ptr(OM_GET_ARRAY_RD);
	int	 in_field_data_size, in_field_data_type;
	char *in_field_node_data = (char *)in_field.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RD, &in_field_data_size, &in_field_data_type);

	out_field.nspace = (int)in_field.nspace;
	ndim = out_field.ndim = (int)in_field.ndim;
	int *out_field_dims= (int *)out_field.dims.ret_array_ptr(OM_GET_ARRAY_WR);
	float *out_field_points = (float *)out_field.points.ret_array_ptr(OM_GET_ARRAY_WR);
	int	 out_field_data_size, out_field_data_type;
	out_field.nnode_data = 1;
	veclen = out_field.node_data[0].veclen = (int)in_field.node_data[0].veclen;
	if(in_field.node_data[0].id.valid_obj())
	{
		out_field.node_data[0].id = (int)in_field.node_data[0].id;
	}
	else
	{
		OMset_obj_val (out_field.node_data[0].id,  OMnull_obj,0);
	  //out_field.node_data[0].id = 0;
	}
	
	// ----------------- Resample Volume ------------------
	int dim, i, j;
	//float res; // actual resolution to be used
	int Size[3]; // spatial extent of volume, origin at 0,0,0
	//float Delta[3]; // voxel separation in each dimension

	int * oldIntVol;
	unsigned char *oldByteVol;
	char *oldCharVol;
	short *oldShortVol;
	int * opMeshSize;
	float * oldFloatVol;

	// ----------- initialise -----------
	oldByteVol = (unsigned char *)in_field_node_data;
	oldCharVol = (char *)in_field_node_data;
	oldShortVol = (short *)in_field_node_data;
	oldIntVol= (int *)in_field_node_data;
	oldFloatVol=(float*)in_field_node_data;
	opMeshSize=(int *)output_mesh_size.ret_typed_array_ptr(OM_GET_ARRAY_RW,OM_TYPE_INT,NULL);
	if(ndim==3 && (opMeshSize[0]<2 || opMeshSize[1]<2 || opMeshSize[2]<2))
	{
		if (in_field_dims)
			ARRfree(in_field_dims);
		if (in_field_points)
			ARRfree(in_field_points);
		
		return(1);
	}
	
#ifdef DEBUG
fprintf(stderr,"after init\n");
#endif
	
	if(in_field_points)
	{
		for(i=0; i<(2*ndim); i++)
			out_field_points[i]=in_field_points[i];
	}
	if(ndim == 3)
	{
		Size[0]=in_field_dims[0];
		Size[1]=in_field_dims[1];
		Size[2]=in_field_dims[2];
#ifdef DEBUG
fprintf(stderr,"size %i %i %i\n",Size[0],Size[1], Size[2]);
#endif
		// no upsampling; keeping org size;
		for (dim=0; dim<2; ++dim)
		{
		  if(opMeshSize[dim]>in_field_dims[dim])
			opMeshSize[dim]=in_field_dims[dim];
		}
	} else {
		Size[0]=in_field_dims[0];
		Size[1]=in_field_dims[1];
		Size[2] = 1;

		// no upsampling; keeping org size;
		for (dim=0; dim<2; ++dim)
		{
		  if(opMeshSize[dim]>in_field_dims[dim])
			opMeshSize[dim]=in_field_dims[dim];
		}
		opMeshSize[2]=0;
	}
#ifdef DEBUG
fprintf(stderr,"Size %i %i %i\n", Size[0],Size[1], Size[2]);
#endif
	for (dim=0; dim<ndim; ++dim )
	{
		out_field_dims[dim]=opMeshSize[dim];
		//Delta[dim]=res;
	}

	// keep original slice direction imformation
	if(ndim == 3)
	{
		nnodes = out_field.nnodes = out_field_dims[0]*out_field_dims[1]*out_field_dims[2];
	} else
		nnodes = out_field.nnodes = out_field_dims[0]*out_field_dims[1];


	// -------- resample in_field_node_data into temp ----------
	float *temp, *tempValue, value;
 
	int x, y, z, v,offset;
	int pX, pY, pZ;

	int slice, sliceSize, rowSize, nSlices;
	float areaXY;
	float nrInValues[3];
	int currValue[3]={0,0,0};
	int count,sum;
	float nrInValue[3];
	int temp1, temp2, temp3;
	
	temp = new float[nnodes*veclen];
	tempValue = new float[veclen];
	nrInValues[0]=(float)(in_field_dims[0])/(float)(out_field_dims[0]);
	nrInValues[1]=(float)(in_field_dims[1])/(float)(out_field_dims[1]);

#ifdef DEBUG
fprintf(stderr,"nrInValues %f %f\n",nrInValues[0],nrInValues[1]);
#endif
	if(ndim == 3)
	{
		nSlices = out_field_dims[2];
		nrInValues[2]=(float)(in_field_dims[2])/(float)(out_field_dims[2]);
	}
	else
	{
		nSlices = 1;
		nrInValues[2]=1;
	}

#ifdef DEBUG
fprintf(stderr,"inFieldDataType %i\n",in_field_data_type);
#endif
	for ( offset = slice = 0; slice<(ndim==3?out_field_dims[2]:1); slice++ )
	{
		for ( y=0; y<out_field_dims[1]; y++ )
		{
			for ( x=0; x<out_field_dims[0]; x++, offset+=veclen )
			{
				for(v=0; v<veclen; v++) tempValue[v]=0;
				count=0;
#ifdef DEBUG
fprintf(stderr,"X %i Y %i Z %i\n",x,y,slice);
fprintf(stderr,"Z from %i to %i\n",(int)(slice*nrInValues[2]),(int)((slice+1)*nrInValues[2]));
fprintf(stderr,"Y from %i to %i\n",(int)(y*nrInValues[1]),(int)((y+1)*nrInValues[1]));
fprintf(stderr,"X from %i to %i\n",(int)(x*nrInValues[0]),(int)((x+1)*nrInValues[0]));
#endif
				//calculating the mean
				
				for(pZ=(slice*nrInValues[2]);pZ<(int)((slice+1)*nrInValues[2]);pZ++)
				{
#ifdef DEBUG
ERRverror(stderr,"",ERR_NO_HEADER | ERR_PRINT,"pZ %i\n",pZ);
#endif
					
					for(pY=(y*nrInValues[1]);pY<(int)((y+1)*nrInValues[1]);pY++)
					{
						
#ifdef DEBUG
fprintf(stderr,"pX %i, x %i nrInValues[0] %f\n",pX,x,nrInValues[0]);
#endif
						for(pX=(int)(x*nrInValues[0]);pX<(int)((x+1)*nrInValues[0]);pX++)
						{
#ifdef DEBUG
fprintf(stderr,"pX %i pY %i pZ %i\n",pX,pY,pZ);
fprintf(stderr,"in %i ",lookupTable(pX,pY,pZ)*veclen);
#endif
							switch(in_field_data_type)
							{
								case OM_TYPE_BYTE:
									for(v=0;v<veclen;v++)
									{
									    tempValue[v]+=oldByteVol[(int)(lookupTable(pX,pY,pZ)*veclen+v)];
#ifdef DEBUG
fprintf(stderr,"byte %i \n",oldByteVol[(int)(lookupTable(pX,pY,pZ)*veclen+v)]);
#endif
									}
									break;
								case OM_TYPE_CHAR:
									for(v=0;v<veclen;v++)
									{
									    tempValue[v]+=oldCharVol[(int)(lookupTable(pX,pY,pZ)*veclen+v)];
#ifdef DEBUG
fprintf(stderr,"byte %i \n",oldCharVol[(int)(lookupTable(pX,pY,pZ)*veclen+v)]);
#endif
									}
									break;

								case OM_TYPE_SHORT:
									for(v=0;v<veclen;v++)
									{
									  tempValue[v]+=oldShortVol[(int)(lookupTable(pX,pY,pZ)*veclen+v)];
#ifdef DEBUG
fprintf(stderr,"short %i \n",oldShortVol[(int)(lookupTable(pX,pY,pZ)*veclen+v)]);
#endif
									}
									break;
								case OM_TYPE_INT:
									for(v=0;v<veclen;v++)
									{
									  tempValue[v]+=oldIntVol[(int)(lookupTable(pX,pY,pZ)*veclen+v)];
#ifdef DEBUG
fprintf(stderr,"int %i \n",oldIntVol[(int)(lookupTable(pX,pY,pZ)*veclen+v)]);
#endif
									}
									break;
								case OM_TYPE_FLOAT:
									for(v=0;v<veclen;v++)
									{
									  tempValue[v]+=oldFloatVol[(int)(lookupTable(pX,pY,pZ)*veclen+v)];
#ifdef DEBUG
	fprintf(stderr,"float %i \n",oldFloatVol[(int)(lookupTable(pX,pY,pZ)*veclen+v)]);
#endif
									}
									break;
								case OM_TYPE_DOUBLE:
									break;
							}
							count++;
#ifdef DEBUG
fprintf(stderr,"pX %i pY %i pZ %i\n",pX,pY,pZ);
fprintf(stderr,"lookupTable %i\n",(int)(lookupTable(pX,pY,pZ)));
fprintf(stderr,"tempValue %f count %i\n",tempValue[0],count);
#endif
						}
					}
					
				}

				for(v=0; v<veclen; v++)
				{
#ifdef DEBUG
fprintf(stderr,"mean %f\n",(float)tempValue[v]/count);
#endif
					temp[(lookupTableOut(x,y,slice)*veclen)+v]=(float)tempValue[v]/count;
#ifdef DEBUG
fprintf(stderr,"  out %i",(lookupTableOut(x,y,slice)*veclen));
#endif
				}
#ifdef DEBUG
fprintf(stderr,"output %f\n",temp[(lookupTableOut(x,y,slice)*veclen)]);
#endif
#ifdef DEBUG
fprintf(stderr,"offset %i , count %i\n",offset,count);
#endif
#ifdef DEBUG
fprintf(stderr,"pX %i, pY %i, pZ %i\n",pX,pY,pZ);
#endif

			}
		}
	}

#ifdef DEBUG
ERRverror(stderr,"",ERR_NO_HEADER | ERR_PRINT,"after big loop\n");
#endif
	// copy temp (floats) to correct data type
	switch(in_field_data_type)
	{
	    case OM_TYPE_BYTE:
		{
		  int outsize=nnodes * veclen;
		  int outdatatype=in_field_data_type;
#ifdef DEBUG
fprintf(stderr,"outsize %i %i\n",outsize,outdatatype);
#endif
		  unsigned char *byteResult = //(unsigned char*)out_field.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RW, &outsize, &outdatatype);
			  (unsigned char *)malloc(sizeof(unsigned char) * nnodes * veclen);
		  if(byteResult)
		    for(i=0; i<nnodes*veclen; i++)
			{
			  byteResult[i]=(unsigned char)temp[i];
#ifdef DEBUG
fprintf(stderr,"bytes %i\n",byteResult[i]);
#endif
			}
		  //if(byteResult) ARRfree(byteResult);
		  out_field.node_data[0].values.set_array(in_field_data_type, (char *)byteResult, nnodes*veclen, OM_SET_ARRAY_FREE);
		}
		break;
		case OM_TYPE_CHAR:
		{
		  char *byteResult = (char *)malloc(sizeof(char) * nnodes * veclen);
		
		  for(i=0; i<nnodes*veclen; i++)
		  {
			byteResult[i]=(char)temp[i];
		  }
		
		  out_field.node_data[0].values.set_array(in_field_data_type, (char *)byteResult, nnodes*veclen, OM_SET_ARRAY_FREE);
          
		}
		break;
		case OM_TYPE_SHORT:
		{
		  short *shortResult = (short *)malloc(sizeof(short) * out_field.nnodes * veclen);
		
		  for(i=0; i<nnodes*veclen; i++)
		  {
			shortResult[i]=(short)temp[i];
		  }
		
		  out_field.node_data[0].values.set_array(in_field_data_type, (char *)shortResult, nnodes*veclen, OM_SET_ARRAY_FREE);
		}
		break;
		case OM_TYPE_INT:
		{
		  int *intResult = (int *)malloc(sizeof(int) * out_field.nnodes * veclen);
		
		  for(i=0; i<nnodes*veclen; i++)
		  {
			intResult[i]=(int)temp[i];
		  } 
		
		  out_field.node_data[0].values.set_array(in_field_data_type, (char *)intResult, nnodes*veclen, OM_SET_ARRAY_FREE);
		}
		break;
		case OM_TYPE_FLOAT:
		{
		  float *intResult = (float *)malloc(sizeof(float) * out_field.nnodes * veclen);
		
		  for(i=0; i<nnodes*veclen; i++)
		  {
				intResult[i]=(float)temp[i];
		  } 
		
		  out_field.node_data[0].values.set_array(in_field_data_type, (char *)intResult, nnodes*veclen, OM_SET_ARRAY_FREE);
		}
		break;
		case OM_TYPE_DOUBLE:
		{
		  double *intResult = (double *)malloc(sizeof(double) * out_field.nnodes * veclen);
		
		  for(i=0; i<nnodes*veclen; i++)
		  {
			intResult[i]=(double)temp[i];
		  } 
		
		  out_field.node_data[0].values.set_array(in_field_data_type, (char *)intResult, nnodes*veclen, OM_SET_ARRAY_FREE);
		}
		break;

	}
	delete [] temp;
	delete [] tempValue;


#ifdef DEBUG
ERRverror(stderr,"",ERR_NO_HEADER | ERR_PRINT,"after write data\n");
#endif

	FLDcopy_node_minmax(in_field.obj_id(), out_field.obj_id(), 0, 0);
	FLDcopy_node_minmax_vec(in_field.obj_id(), out_field.obj_id(), 0, 0);

	if (in_field_dims)
		ARRfree(in_field_dims);
	if (in_field_points)
		ARRfree(in_field_points);


	if (out_field_dims)
		ARRfree(out_field_dims);
	if (out_field_points)
		ARRfree(out_field_points);

	// return 1 for success
	return(1);
}

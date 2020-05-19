
#include "gen.h"
#ifndef XP_HAS_LONG
#define XP_HAS_LONG
typedef int xp_long;
typedef unsigned int xp_ulong;
#endif
#ifndef XP_HAS_BYTE
#define XP_HAS_BYTE
typedef unsigned char xp_byte;
#endif

#define ADD_SLICE(T) \
{ \
	T oval; \
	T *idata, *odata; \
 \
	idata = (T*) In_Slice_data; \
	odata = (T*) Out_Volume_data; \
 \
	switch (axis){ \
		case 0:	\
		{ \
			if (use_value){ \
				for (j=0;j<Ref_Volume_dims[1];j++) \
					for (k=0;k<Ref_Volume_dims[2];k++){ \
						 oval = (idata[k*In_Slice_dims[0]+j]!=(T)Null_Value)?(T)value:(T)Null_Value; \
						 odata[((k * Ref_Volume_dims[0])+j)* Ref_Volume_dims[1]+Slice]=oval; \
				} \
			} else { \
				for (j=0;j<Ref_Volume_dims[1];j++) \
					for (k=0;k<Ref_Volume_dims[2];k++){ \
						odata[((k * Ref_Volume_dims[0])+j)* Ref_Volume_dims[1]+Slice]=idata[k*In_Slice_dims[0]+j]; \
				} \
			} \
		};break; \
		case 1: \
		{ \
			if (use_value){ \
				for (i=0;i<Ref_Volume_dims[0];i++)	\
					for (k=0;k<Ref_Volume_dims[2];k++) { \
						oval = (idata[k*In_Slice_dims[0]+i]!=(T)Null_Value)?(T)value:(T)Null_Value; \
						odata[((k * Ref_Volume_dims[0])+Slice)* Ref_Volume_dims[1]+i]=oval; \
					} \
			} else { \
				for (i=0;i<Ref_Volume_dims[0];i++)	\
					for (k=0;k<Ref_Volume_dims[2];k++){ \
						odata[((k * Ref_Volume_dims[0])+Slice)* Ref_Volume_dims[1]+i]=idata[k*In_Slice_dims[0]+i]; \
				} \
			};  \
		};break; \
		case 2: \
		{ \
			if (use_value){ \
				for (i=0;i<Ref_Volume_dims[0];i++)	\
					for (j=0;j<Ref_Volume_dims[1];j++){ \
						oval = (idata[j*In_Slice_dims[0]+i]!=(T)Null_Value)?(T)value:(T)Null_Value; \
						odata[((Slice * Ref_Volume_dims[0])+j)* Ref_Volume_dims[1]+i]=oval; \
				}; \
			} else { \
				size = In_Slice_dims[0] * In_Slice_dims[1] * sizeof(T); \
				memcpy( &odata[Slice * size], idata, size ); \
			}; \
		};break; \
	}; \
}


int Slice_to_Volume(OMobj_id Slice_to_Volume_id, 
						  OMevent_mask event_mask, int seq_num)
{
	OMobj_id In_Slice_id;
	int In_Slice_ndim;
	xp_long *In_Slice_dims; 
	int In_Slice_nspace;
	xp_long In_Slice_npoints;
	float *In_Slice_points;
	int In_Slice_veclen;
	int In_Slice_data_type; 
	int In_Slice_ndata;
	char *In_Slice_data;

	OMobj_id Ref_Volume_id;
	int Ref_Volume_ndim; 
	xp_long *Ref_Volume_dims; 
	int Ref_Volume_nspace;
	xp_long Ref_Volume_npoints;
	float *Ref_Volume_points;
	xp_long Slice;
	OMobj_id Out_Volume_id;

	int Out_Volume_ndim; 
	xp_long *Out_Volume_dims;
	int Out_Volume_nspace;
	xp_long Out_Volume_npoints;
	float *Out_Volume_points;
	int Out_Volume_veclen;
	int Out_Volume_data_type; 
	int Out_Volume_ndata;
	char *Out_Volume_data;
	int axis;
	int value;
	int use_value;
	int Null_Value;
	int null_flag;
	int out_value;

	int status;
	xp_long size;
	xp_long x,y,i,j,k;

	/* Get input slice */
	
	In_Slice_id = OMfind_subobj( Slice_to_Volume_id,
								 OMstr_to_name("In_Slice"), OM_OBJ_RD );

	status = !FLDget_dims( In_Slice_id, &In_Slice_dims, &In_Slice_ndim );
	status |= !FLDget_node_data_veclen( In_Slice_id, 0, &In_Slice_veclen );
	status |= !FLDget_node_data_type( In_Slice_id, 0, &In_Slice_data_type );
	status |= !FLDget_node_data( In_Slice_id, 0, &In_Slice_data_type, 
										  &In_Slice_data, &In_Slice_ndata, 
										  OM_GET_ARRAY_RD);
	
	if( status != 0 )
	{
		fprintf( stderr, "Unable to read input slice\n" );fflush(stderr);

		if( In_Slice_dims ) {
			ARRfree( (char *)In_Slice_dims );
			In_Slice_dims = NULL;
		};
		if( In_Slice_data ) {
			ARRfree( (char *)In_Slice_data );
			In_Slice_data = NULL;
		};
		return 0;
	}

	status = FLDget_node_null_data (In_Slice_id,0, &null_flag, (char*)&Null_Value);

#ifdef DEBUG
	fprintf(stderr, "status: %i, null_flag: %i, null_value: %f\n",status,null_flag,Null_Value);
#endif
	/* Get reference volume */
	
	Ref_Volume_id = OMfind_subobj( Slice_to_Volume_id, 
								   OMstr_to_name("Ref_Volume"), OM_OBJ_RD );

	FLDget_dims( Ref_Volume_id, &Ref_Volume_dims, &Ref_Volume_ndim );

	/* Get slice to output and validate */
	
	if( OMget_name_int_val( Slice_to_Volume_id, 
							OMstr_to_name("axis"), &axis) != 1 )
	{
		if( In_Slice_dims ) {
			ARRfree( (char *)In_Slice_dims );
			In_Slice_dims = NULL;
		};
		if( Ref_Volume_dims ) {
			ARRfree( (char *)Ref_Volume_dims );
			Ref_Volume_dims = NULL;
		};
		if( In_Slice_data ) {
			ARRfree( (char *)In_Slice_data );
			In_Slice_data = NULL;
		};
		return 0;
	};
	if( (axis < 0) || (axis >= Ref_Volume_ndim) )
	{
		if( In_Slice_dims ) {
			ARRfree( (char *)In_Slice_dims );
			In_Slice_dims = NULL;
		};
		if( Ref_Volume_dims ) {
			ARRfree( (char *)Ref_Volume_dims );
			Ref_Volume_dims = NULL;
		};
		if( In_Slice_data ) {
			ARRfree( (char *)In_Slice_data );
			In_Slice_data = NULL;
		};
		return 0;
	};

#ifndef OM_TYPE_LONG
	if( OMget_name_int_val( Slice_to_Volume_id, 
							OMstr_to_name("Slice"), &Slice) != 1 )
#else
	if( OMget_name_long_val( Slice_to_Volume_id, 
							OMstr_to_name("Slice"), &Slice) != 1 )
#endif
	{
		if( In_Slice_dims ) {
			ARRfree( (char *)In_Slice_dims );
			In_Slice_dims = NULL;
		};
		if( Ref_Volume_dims ) {
			ARRfree( (char *)Ref_Volume_dims );
			Ref_Volume_dims = NULL;
		};
		if( In_Slice_data ) {
			ARRfree( (char *)In_Slice_data );
			In_Slice_data = NULL;
		};
		return 0;
	};
	
	if( (Slice < 0) || (Slice >= Ref_Volume_dims[axis]) )
	{
		if( In_Slice_dims ) {
			ARRfree( (char *)In_Slice_dims );
			In_Slice_dims = NULL;
		};
		if( Ref_Volume_dims ) {
			ARRfree( (char *)Ref_Volume_dims );
			Ref_Volume_dims = NULL;
		};
		if( In_Slice_data ) {
			ARRfree( (char *)In_Slice_data );
			In_Slice_data = NULL;
		};
		return 0;
	};

	switch (axis)
	{
		case 0:{
			if ((Ref_Volume_dims[1] != In_Slice_dims[0])||
				(Ref_Volume_dims[2] != In_Slice_dims[1]))
			{
				if( In_Slice_dims ) {
					ARRfree( (char *)In_Slice_dims );
					In_Slice_dims = NULL;
				};
				if( Ref_Volume_dims ) {
					ARRfree( (char *)Ref_Volume_dims );
					Ref_Volume_dims = NULL;
				};
				if( In_Slice_data ) {
					ARRfree( (char *)In_Slice_data );
					In_Slice_data = NULL;
				};
				return 0; 
			}
		};break;
		case 1:{
			if ((Ref_Volume_dims[0] != In_Slice_dims[0])||
				(Ref_Volume_dims[2] != In_Slice_dims[1]))
			{
				if( In_Slice_dims ) {
					ARRfree( (char *)In_Slice_dims );
					In_Slice_dims = NULL;
				};
				if( Ref_Volume_dims ) {
					ARRfree( (char *)Ref_Volume_dims );
					Ref_Volume_dims = NULL;
				};
				if( In_Slice_data ) {
					ARRfree( (char *)In_Slice_data );
					In_Slice_data = NULL;
				};
				return 0; 
			}
		};break;
		case 2:{
			if ((Ref_Volume_dims[0] != In_Slice_dims[0])||
				(Ref_Volume_dims[1] != In_Slice_dims[1]))
			{
				if( In_Slice_dims ) {
					ARRfree( (char *)In_Slice_dims );
					In_Slice_dims = NULL;
				};
				if( Ref_Volume_dims ) {
					ARRfree( (char *)Ref_Volume_dims );
					Ref_Volume_dims = NULL;
				};
				if( In_Slice_data ) {
					ARRfree( (char *)In_Slice_data );
					In_Slice_data = NULL;
				};
				return 0; 
			}
		};break;
	}
	
	
	if( OMget_name_int_val( Slice_to_Volume_id, 
							OMstr_to_name("value"), &value) != 1 )
		value = 1;
	  
	if( OMget_name_int_val( Slice_to_Volume_id, 
							OMstr_to_name("use_value"), &use_value) != 1 )
		use_value = 0;
 
	if(use_value){
	
	}

#ifdef DEBUG
	fprintf(stderr," use_value: %i, value %i\n",use_value,value);
#endif

	/*	 Set output */
	
	Out_Volume_id = OMfind_subobj( Slice_to_Volume_id, 
								   OMstr_to_name("Out_Volume"), OM_OBJ_RW );

	FLDset_node_data_type( Out_Volume_id, 0, In_Slice_data_type );

	FLDget_node_data( Out_Volume_id, 0, &Out_Volume_data_type, 
					  &Out_Volume_data, &Out_Volume_ndata, OM_GET_ARRAY_RW );

	switch (In_Slice_data_type) 
    {
		case OM_TYPE_BYTE:
			ADD_SLICE(xp_byte);
		break;
		case OM_TYPE_CHAR:
			ADD_SLICE(char);
		break;
		case OM_TYPE_SHORT:
			ADD_SLICE(unsigned short);
		break;
		case OM_TYPE_INT:
			ADD_SLICE(int);
		break;
#ifdef OM_TYPE_LONG
		case OM_TYPE_LONG:
			ADD_SLICE(xp_long);
		break;
#endif
		case OM_TYPE_FLOAT:
			ADD_SLICE(float);
		break;
		case OM_TYPE_DOUBLE:
			ADD_SLICE(double);
		break;	
	} 

	/* Clean up and return */

	if( In_Slice_dims ) {
		ARRfree( (char *)In_Slice_dims );
		In_Slice_dims = NULL;
	};
	if( Ref_Volume_dims ) {
		ARRfree( (char *)Ref_Volume_dims );
		Ref_Volume_dims = NULL;
	};
	if( In_Slice_data ) {
		ARRfree( (char *)In_Slice_data );
		In_Slice_data = NULL;
	};
	if( Out_Volume_data ) {
		ARRfree( (char *)Out_Volume_data );
		Out_Volume_data = NULL;
	};
	return 1;
}

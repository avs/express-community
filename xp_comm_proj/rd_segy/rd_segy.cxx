#include "gen.hxx"

//#define DEBUG		// this toggle dumps debug information to stdout
// #define FILEDEBUG	// this toggle dumps vast amounts of information to stderr

#ifdef MSDOS
// turn off warning about symbols too long for debugger
#pragma warning (disable : 4786)
#endif

#include "rd_segy.h"

#include <numeric>
#include <functional>
#include <vector>
#include <iterator>
#include <math.h>

#include <ctype.h>

#include <avs/f_utils.h>

void * SEGY_realloc (void* ptr, size_t size, size_t oldsize)
{
#ifdef MS_DOS
	/* fix for missing realloc in MS VC6.0 */
	void *new;
	if (new=(void*)malloc(size))
	{
		memcpy(new,ptr,oldsize);
		free(ptr);
		ptr=new;
	}
	return new;
#else
	return realloc(ptr,size);
#endif
}

//using namespace std;

// STL Vector Collection Classes
// ShotPoint
/////////////////////////////////////////////////////////////////////////////

class ShotPoint
{
public:
    int X;
    int Y;
	int Trace;
    ShotPoint() : X(0), Y(0), Trace(0) {}
    ShotPoint(int NewX, int NewY, int NewTrace) : X(NewX), Y(NewY), Trace(NewTrace) {}
};

// In this example, an ShotPoint is equivalent only if both X and Y match.
bool operator==(const ShotPoint& a, const ShotPoint& b)
{
    return (a.X == b.X) && (a.Y == b.Y);
}

// ShotPoint will be sorted by X and Y.
bool operator<(const ShotPoint& a, const ShotPoint& b)
{
	if (a.X == b.X) return (a.Y < b.Y);
    return a.X < b.X;
}

typedef vector<ShotPoint> ShotPointArray;

// NEW readsegy CODE IS BELOW
////////////////////////////////////////////////////////////////////////////////
//  Byte Swap routines used to read/write/convert data from LittleEndian
//  (PC) machines to BigEndian (other) machines;  Words (32 bits), Nybbles
//  (16 bits), and potentially Double Words (64 bits) must be converted -
//  characters (8 bits) do not;
////////////////////////////////////////////////////////////////////////////////

#ifdef AVS_LITTLEENDIAN
#define NEED_TO_SWAP_BYTES
#endif

//----- This routine swaps bytes to convert LittleEndian (Intel standard)
//----- words (32 bit values) to BigEndian (ACRES Binary standard)

static void SwapWordByteOrder (char * data_array, int word_count)
{
  int  i, j;
  char tmpchar;
  for (i=0; i<word_count; i++)  {
      j = (i*4);
      tmpchar         = data_array[j+0];
      data_array[j+0] = data_array[j+3];
      data_array[j+3] = tmpchar;
      tmpchar         = data_array[j+2];
      data_array[j+2] = data_array[j+1];
      data_array[j+1] = tmpchar;
  }
}

////////////////////////////////////////////////////////////////////////////////
//----- This routine swaps bytes to convert LittleEndian (Intel standard)
//----- nybbles (16 bit values) to BigEndian (ACRES Binary standard)

static void SwapNybbleByteOrder (char * data_array, int nybble_count)
{
  int  i, j;
  char tmpchar;
  for (i=0; i<nybble_count; i++)  {
      j = (i*2);
      tmpchar         = data_array[j+0];
      data_array[j+0] = data_array[j+1];
      data_array[j+1] = tmpchar;
  }
}

////////////////////////////////////////////////////////////////////////////////

//----- This routine transforms ACRES_BINARY into machine-specific
//----- numeric data format and vice-versa;  

static int ConvertBinary (void * data, int size, int count)
{
#ifdef NEED_TO_SWAP_BYTES
    if (size == sizeof (short))
	SwapNybbleByteOrder ((char *) data, count);
    else if (size != sizeof (char))
	SwapWordByteOrder ((char *) data, count);
#endif
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////

int ReadSegy_ReadSegyMods_ReadSegyCore::update(OMevent_mask event_mask, int seq_num)
{
	// FileName (OMXstr read req notify)
	int maxvals;
	
    // Declare a dynamically allocated vector of IDs.
	ShotPointArray spArray;

    // Iterator is used to loop through the vector.
    ShotPointArray::iterator spIterator;

	/***********************/
	/* Function's Body     */
	/***********************/

	#ifdef DEBUG
		ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: ReadSegyCore::update\n");
		fprintf(stdout,"I'm in method: ReadSegyCore::update\n\n");	
	#endif
	
	char filename_buf[AVS_PATH_MAX];

	// Use AVS/Express function to map enviroment variables
	FILEmap_variables((char *)FileName, filename_buf);

	FILE *infile;
	struct segy_ebcdic_hdr ebcdic_hdr;
	struct segy_reel_hdr reel_hdr;
	struct segy_trace_hdr trace_hdr;
	float *trace_vals;
	float *vals,*vals1;

	int trace_count = 0;
	int index;
	float minx, miny, maxx, maxy;
	float intervall=0.0;

	minx = miny = maxx = maxy = 0.0;

	if (!strlen(filename_buf)) return (1);

	if ((infile = fopen(filename_buf, "rb")) == NULL) {
		fprintf(stdout,"ReadSegy: Cannot open segy file - %s\n",filename_buf);
		ERRverror("",ERR_NO_HEADER | ERR_PRINT,"ReadSegy: Cannot open segy file - %s\n",filename_buf);
		return FALSE;
	}

	fread(&ebcdic_hdr, SEGY_EBCDIC_HDR_SIZE, 1, infile);
	fread(&reel_hdr, SEGY_REEL_HDR_SIZE, 1, infile);
	ConvertBinary(&reel_hdr.job_id_number, sizeof(int32_t), 1);
	ConvertBinary(&reel_hdr.line_number, sizeof(int32_t), 1);
	ConvertBinary(&reel_hdr.reel_number, sizeof(int32_t), 1);
	ConvertBinary(&reel_hdr.traces_per_record, sizeof(int16_t), 1);
	ConvertBinary(&reel_hdr.aux_traces_per_record, sizeof(int16_t), 1);
	ConvertBinary(&reel_hdr.sample_data_interval_ms, sizeof(int16_t), 1);
	ConvertBinary(&reel_hdr.original_data_interval_ms, sizeof(int16_t), 1);
	ConvertBinary(&reel_hdr.samples_per_trace, sizeof(int16_t), 1);
	ConvertBinary(&reel_hdr.original_samples_per_trace, sizeof(int16_t), 1);
	ConvertBinary(&reel_hdr.data_sample_format_code, sizeof(int16_t), 1);
	#ifdef FILEDEBUG
		DUMP_reel_hdr (&reel_hdr);
	#endif
	
	int XDIM = 1;
	int YDIM = reel_hdr.traces_per_record;
	int ZDIM = reel_hdr.samples_per_trace;

	// we might have to juggle the coordiates a bit 

	if (ZDIM<=0) 
	{	
		fprintf(stdout,"ReadSegy: Cannot read segy file - %s\n",filename_buf);
		ERRverror("",ERR_NO_HEADER | ERR_PRINT,"ReadSegy: Cannot read segy file - %s\n",filename_buf);
		return(0);
	};
	
	trace_vals = (float*) malloc (ZDIM*sizeof(float));

	maxvals = 200;
	vals = (float*) malloc (maxvals*ZDIM*sizeof(float));
	
	#ifdef DEBUG
		fprintf(stdout,"YDIM: %i\n",YDIM);	
		fprintf(stdout,"ZDIM: %i\n",ZDIM);
	#endif

	int first=1;
	while (!feof(infile)) {
		if (!fread(&trace_hdr, SEGY_TRACE_HDR_SIZE, 1, infile)) break;

		ConvertBinary(&trace_hdr.trace_sequence_number_within_line, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.trace_sequence_number_within_reel, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.original_field_record_number, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.trace_sequence_number_within_original_field_record, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.energy_source_point_number, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.cdp_ensemble_number, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.trace_sequence_number_within_cdp_ensemble, sizeof(int32_t), 1);
		//_4_*_2_=_8_bytes
		ConvertBinary(&trace_hdr.trace_identification_code, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.number_of_vertically_summed_traces_yielding_this_trace, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.number_of_horizontally_stacked_traced_yielding_this_trace, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.data_use, sizeof(int16_t), 1);
		//_location,_geometry_info_
		//_8_*_4_=_32_bytes
		ConvertBinary(&trace_hdr.distance_from_source_point_to_receiver_group, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.receiver_group_elevation, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.surface_elevation_at_source, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.source_depth_below_surface, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.datum_elevation_at_receiver_group, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.datum_elevation_at_source, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.water_depth_at_source, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.water_depth_at_receiver_group, sizeof(int32_t), 1);
		//_2_*_2_=_4_bytes
		ConvertBinary(&trace_hdr. scalar_for_elevations_and_depths, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr. scalar_for_coordinates, sizeof(int16_t), 1);
		//_4_*_4_=_16_bytes
		ConvertBinary(&trace_hdr.x_source_coordinate, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.y_source_coordinate, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.x_receiver_group_coordinate, sizeof(int32_t), 1);
		ConvertBinary(&trace_hdr.y_receiver_group_coordinate, sizeof(int32_t), 1);
		// 2 bytes
		ConvertBinary(&trace_hdr.coordinate_units, sizeof(int16_t), 1);

		ConvertBinary(&trace_hdr.weathering_velocity, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.subweathering_velocity, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.uphole_time_at_source, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.uphole_time_at_group, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.source_static_correction, sizeof(int16_t), 1);

		ConvertBinary(&trace_hdr.group_static_correction, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.total_static_applied, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.lag_time_a, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.lag_time_b, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.delay_according_time, sizeof(int16_t), 1);

		ConvertBinary(&trace_hdr.brute_time_start, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.mute_time_end, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.samples_in_this_trace, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.sample_intervall, sizeof(int16_t), 1);
		ConvertBinary(&trace_hdr.gain_type_instruments, sizeof(int16_t), 1);

		// print trace_hdr info
		#ifdef FILEDEBUG
			DUMP_trace_hdr (&trace_hdr);
		#endif

		if (first) {
			first=0;
			minx = trace_hdr.x_source_coordinate;
			miny = trace_hdr.y_source_coordinate;
			maxx = trace_hdr.x_source_coordinate;
			maxy = trace_hdr.y_source_coordinate;
		}
		if (minx > trace_hdr.x_source_coordinate) {
			minx = trace_hdr.x_source_coordinate;
		}
		if (miny > trace_hdr.y_source_coordinate) {
			miny = trace_hdr.y_source_coordinate;
		}
		if (maxx < trace_hdr.x_source_coordinate) {
			maxx = trace_hdr.x_source_coordinate;
		}
		if (maxy < trace_hdr.y_source_coordinate) {
			maxy = trace_hdr.y_source_coordinate;
		}
		
		intervall=trace_hdr.sample_intervall;

		if (!fread(trace_vals, sizeof(float), ZDIM, infile))
			break;

		ConvertBinary(trace_vals, sizeof(float), ZDIM);

		if (trace_count>=maxvals)
		{
			vals1 = (float*) SEGY_realloc (vals, (maxvals+200)*ZDIM*sizeof(float), (maxvals+200)*ZDIM*sizeof(float)); 
			if (!vals1) {
				// not enough memory
				fprintf(stderr,"ReadSegy: Not Enough Memory to read %s\n",filename_buf);fflush(stderr);			
				ERRverror("",ERR_NO_HEADER | ERR_PRINT,"ReadSegy: Not Enough Memory to read %s\n",filename_buf);
				if (vals)
				{
					free(vals);
					vals=NULL;
				}
				if (trace_vals)
				{
					free(trace_vals);
					trace_vals=NULL;
				}
				return(0);
			};
			vals=vals1;
			maxvals+=200;
		};
		memcpy(&vals[trace_count*ZDIM],trace_vals,ZDIM*sizeof(float));
		spArray.push_back(ShotPoint(trace_hdr.x_source_coordinate, trace_hdr.y_source_coordinate,trace_count));
		++trace_count;
	};

	XDIM=trace_count/YDIM;
	float ts = pow(10.0,(int)timescale);

	#ifdef DEBUG
	    fprintf(stdout,"XDIM: %i\n",XDIM);
		fprintf (stdout,"minx: %f, maxx: %f, miny: %f, maxy: %f, intervall: %f\n",minx,maxx,miny,maxy,intervall);
	    fprintf(stdout,"trace_count: %i\n",trace_count);
	    fprintf(stdout,"ts: %f\n",ts);
	#endif

	fclose(infile);
	
	// out (Mesh_Unif+Node_Data write)

	out.ndim = 3; 
	// out.dims (int []) 
	int *out_dims = (int *)out.dims.ret_array_ptr(OM_GET_ARRAY_WR);
	out_dims[0] = XDIM;
	out_dims[1] = YDIM;
	out_dims[2] = ZDIM;	
	
	
	out.nspace = 3; 
	// out.nnodes (int)
	out.nnodes = XDIM * YDIM * ZDIM;
	float *out_points = (float *)out.points.ret_array_ptr(OM_GET_ARRAY_WR);

	if ((int)normalize)
	{
		out_points[0] = out_points[1] = out_points[2] = 0;
		out_points[3] = out_points[4] = out_points[5] = 1;
	} else {
		out_points[0] = minx;
		out_points[1] = miny;
		out_points[2] = 0;
		out_points[3] = maxx;
		out_points[4] = maxy;
		out_points[5] = intervall*(ZDIM)/ts;
	};
	
	out.nnode_data = 1;	
	
	int  out_data_size;
	out.node_data[0].veclen = 1; 
	float *out_node_data = (float*)out.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_WR, DTYPE_FLOAT, &out_data_size);
	index=0;
	for (spIterator = spArray.begin(); spIterator != spArray.end(); spIterator++) {
		memcpy(&out_node_data[index*ZDIM],&vals[spIterator->Trace*ZDIM],ZDIM*sizeof(float));
		index++;
	}	

	if (out_node_data)
		ARRfree(out_node_data);
	if (out_dims)
		ARRfree(out_dims);
	if (out_points)
		ARRfree(out_points);
	
	if (vals)
	{
		free(vals);
		vals=NULL;
	}
	if (trace_vals)
	{
		free(trace_vals);
		trace_vals=NULL;
	}
	
	// return 1 for success
	return(1);
}



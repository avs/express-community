
#include "avs/err.h"
#include "avs/gd_def.h"
#include "gen.h"

#define ERROR_MSG(a) ERRerror("read_Station::read", 1, ERR_ORIG, a)

#define DEBUG
	
int
ReadMETARSCore::read(OMevent_mask event_mask, int seq_num)
{
	// filename (OMXstr read req notify)
	// StationLocation (Mesh read req notify)
	// StationID (string read req notify)

	// out (Mesh+Node_Data write)

	int  out_data_size, out_data_type;
	float *out_node_data_temperature;
	float *out_node_data_pressure;

	// out.coordinates.values (float [])
	float *out_coordinates; 
	METARS_c *MetarsObject;

	unsigned long ReturnValue;
	int Count;

	/***********************/
	/* Function's Body	  */
	/***********************/

	ERROR_MSG("In Read_METARS");

	char * lookat = (char *)filename;

	// create an METARS object
	MetarsObject = new METARS_c((char *)filename);

	if (MetarsObject == NULL)
	{
		ERROR_MSG("Error creating MetarsObject");
		return OM_STAT_FAILURE;
	}

	int NumberOfStations = StationLocation.nnodes;

	MetarsObject->SetNumberOfStations(NumberOfStations);
	out.nnodes = NumberOfStations;

	char** InternalStationID;
	InternalStationID	 = (char**)malloc(sizeof(char *) * NumberOfStations);
	if (InternalStationID == NULL)
	{
		ERROR_MSG("Error creating InternalStatID");
		delete MetarsObject;
		return OM_STAT_FAILURE;
	}

	for (Count = 0;Count < NumberOfStations;Count++)
	{
		InternalStationID[Count] = (char *)malloc(sizeof(char) * 5);
		StationID.ret_str_array_val(Count,InternalStationID[Count],5); 
#ifdef DEBUG
		fprintf(stderr,"%s\t",InternalStationID[Count]);
#endif  
	}

	out_coordinates =
		(float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
	
	
	
	out.nnode_data = 2;
	out.node_data[0].veclen = 1;
	out.node_data[0].null_flag = 1;
	out.node_data[0].null_value = 500.0;
	out.node_data[1].veclen = 1;
	out.node_data[1].null_flag = 1;
	out.node_data[1].null_value = 500.0;

	out_node_data_temperature = 
		(float *)out.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR,
										&out_data_size, &out_data_type);

	if (!out_node_data_temperature)
	{
		ERROR_MSG( "can't get out_node_data_temperature array for write" );
		delete MetarsObject;
		return OM_STAT_FAILURE;
	}

	out_node_data_pressure =
			 (float *)out.node_data[1].values.ret_array_ptr(OM_GET_ARRAY_WR,
														  &out_data_size, &out_data_type);

	if (!out_node_data_pressure)
	{
		ERROR_MSG( "can't get out_node_data_temperature array for write" );
		delete MetarsObject;
		return OM_STAT_FAILURE;
	}

	ReturnValue = MetarsObject->ReadData(InternalStationID, 
										 out_node_data_temperature,
										 out_node_data_pressure);


	for (Count = 0;Count < NumberOfStations;Count++)
	{
		free(InternalStationID[Count]);
	}
	free(InternalStationID);

	ARRfree((void *)out_coordinates);
	ARRfree((void *)out_node_data_temperature);
	ARRfree((void *)out_node_data_pressure);

	delete MetarsObject;

	// return 1 for success
	if( ReturnValue != XP_GIS_OK )
	{
		return OM_STAT_FAILURE;
	}

	return OM_STAT_SUCCESS;

}





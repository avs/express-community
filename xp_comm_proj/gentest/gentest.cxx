
#include "gen.h"

int GenerateTestData_GenerateTestDataMods_Generate_Test_Data::GenerateTestData(OMevent_mask event_mask, int seq_num)
{
  // pattern (OMXint read notify)
  // width (OMXint read notify)
  // height (OMXint read notify)
  // num_frames (OMXint read notify)
  // output (Mesh_Unif+Node_Data write)

  output.ndim = 3;

  int *output_dims = (int *)output.dims.ret_array_ptr(OM_GET_ARRAY_WR);

  output_dims[0] = width;
  output_dims[1] = height;
  output_dims[2] = num_frames;

  output.nspace = 3;
  output.npoints = 2;

  float *output_points = (float *)output.points.ret_array_ptr(OM_GET_ARRAY_WR);

  output_points[0] = 0.0;
  output_points[1] = 0.0;
  output_points[2] = 0.0;
  output_points[3] = (float)(width - 1);
  output_points[4] = (float)(height - 1);
  output_points[5] = (float)(num_frames - 1);

  output.nnode_data = 1;

  int  output_data_comp;
  int  output_data_size, output_data_type;
  for (output_data_comp = 0; output_data_comp < output.nnode_data; output_data_comp++) 
    { 
      output.node_data[output_data_comp].veclen = 1;

      char *output_node_data = (char *)output.node_data[output_data_comp].values.ret_array_ptr(
                                                OM_GET_ARRAY_WR, &output_data_size, &output_data_type);

      // Fill in output data
      //
      unsigned char *dptr = (unsigned char *)output_node_data;
      switch( (int)pattern )
        {
        case 0:
          {
            for( int k = 0; k < num_frames; k++ )
              {
                for( int j = 0; j < height; j++ )
                  {
                    for( int i = 0; i < width; i++ )
                      {
                        *dptr++ = ((k * 255) / num_frames);
                      }
                  }
              }
            break;
          }
        
        case 1:
          {
            for( int k = 0; k < num_frames; k++ )
              {
                int start_value = ((k * 255) / num_frames);
                int end_value = 255 - start_value;
                float xdelta = (float)(end_value - start_value) / (float)width;
                for( int j = 0; j < height; j++ )
                  {
                    for( int i = 0; i < width; i++ )
                      {
                        *dptr++ = (start_value + (int)((float)i * xdelta));
                      }
                  }
              }
            break;
          }
        
        default:
          printf( "GenerateTestData: Invalid test pattern %d\n", (int)pattern );
        }

      if (output_node_data)
        ARRfree((char *)output_node_data);
    }
  
  if (output_dims)
    ARRfree((char *)output_dims);
  if (output_points)
    ARRfree((char *)output_points);
  
  return(1);
}



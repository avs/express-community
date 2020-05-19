#include <avs/f_utils.h>

#include "xp_comm_proj/rd_spot/gen.hxx"
#include "xp_comm_proj/rd_spot/gsspot.hxx"

// Uncomment the following line to enable the display of debugging information
//#define DEBUG

#ifdef DEBUG
#define PRINT(A) { printf(A); }
#else
#define PRINT(A) { }
#endif

int ReadSpot_ReadSpotMods_ReadSpotHeader::update(OMevent_mask event_mask, int seq_num)
{
   // HeaderFileName (OMXstr read req notify)
   // ProcessingLevel (OMXstr write)
   // NumberOfBands (OMXint write)
   // NumberOfRows (OMXint write)
   // NumberOfColumns (OMXint write)
   // Year (OMXint write)
   // Month (OMXint write)
   // Day (OMXint write)
   // Hour (OMXint write)
   // Minute (OMXint write)
   // Second (OMXfloat write)

   XP_GIS_Spot_c SpotFile;

   char header_filename_buf[AVS_PATH_MAX];


   /***********************/
   /* Function's Body     */
   /***********************/
   PRINT("BEGIN ReadSPOTHeader\n");

   //
   //  Create the spot file object and give it the file name.
   //

   // Use AVS/Express function to map enviroment variables
   FILEmap_variables((char *)input_params.HeaderFileName, header_filename_buf);

   if (SpotFile.HeaderFileName(header_filename_buf) != XP_GIS_OK)
   {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                "Error opening spot header file.\n");

      output_params.ProcessingLevel = "unknown";
      output_params.NumberOfBands   = 0;
      output_params.NumberOfRows    = 0;
      output_params.NumberOfColumns = 0;
      output_params.Year            = 0;
      output_params.Month           = 0;
      output_params.Day             = 0;
      output_params.Hour            = 0;
      output_params.Minute          = 0;
      output_params.Second          = 0;

      return 1;
   }

   //
   //  Assign the Express values
   //

   switch (SpotFile.ProcessingLevel())
   {
     case XP_GIS_Spot_c::ONE_A:
       output_params.ProcessingLevel = "1A";
       break;
     case XP_GIS_Spot_c::ONE_B:
       output_params.ProcessingLevel = "1B";
       break;
     default:
       output_params.ProcessingLevel = "unknown";
   }

   output_params.NumberOfBands   = (int) SpotFile.NumberOfBands();
   output_params.NumberOfRows    = (int) SpotFile.NumberOfRows();
   output_params.NumberOfColumns = (int) SpotFile.NumberOfColumns();
   output_params.Year            = (int) SpotFile.Year();
   output_params.Month           = (int) SpotFile.Month();
   output_params.Day             = (int) SpotFile.Day();
   output_params.Hour            = (int) SpotFile.Hour();
   output_params.Minute          = (int) SpotFile.Minute();
   output_params.Second          = SpotFile.Second();

   PRINT("END ReadSPOTHeader\n");

   // return 1 for success
   return(1);
}

// end of file


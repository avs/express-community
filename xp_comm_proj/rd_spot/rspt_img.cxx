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

int ReadSpot_ReadSpotMods_ReadSpotImage::update(OMevent_mask event_mask, int seq_num)
{
   // HeaderFileName (OMXstr read req notify)
   // ImageFileName (OMXstr read req notify)
   // BandNumberToRead (OMXint read notify)
   // StartRow (OMXint read notify)
   // StartColumn (OMXint read notify)
   // NumberOfOutputRows(OMXint read notify)
   // NumberOfOutputColumns(OMXint read notify)
   // XScaleFactor (OMXfloat read notify)
   // YScaleFactor (OMXfloat read notify)
   // ActivateFlag (OMXint read notify)
   // Image (Field_Rect write)

   // Image.ndim (int) 
   // Image.dims (int []) 
   // Image.nspace (int) 

   // Image.npoints (int)
   // Image.points (float [])

   // Image.nnodes (int)
   // Image.nnode_data (int)

   XP_GIS_Spot_c SpotFile;
   unsigned char *ImageBuffer;
   unsigned long  NumberOfPixels;
   unsigned long  ReturnValue;
   unsigned long  LocalBandNumberToRead;
   unsigned long  LocalStartRow;
   unsigned long  LocalStartColumn;
   unsigned long  LocalNumberOfOutputRows;
   unsigned long  LocalNumberOfOutputColumns;
   unsigned long  LocalActivateFlag;
   float          LocalXScaleFactor;
   float          LocalYScaleFactor;

   char header_filename_buf[AVS_PATH_MAX];
   char image_filename_buf[AVS_PATH_MAX];


   /***********************/
   /* Function's Body     */
   /***********************/
   PRINT("BEGIN ReadSPOTImage\n");

   //
   //  Assign local copies of Express variables.
   //

   LocalBandNumberToRead = (int) input_params.BandNumberToRead;
   LocalStartRow = (int) input_params.StartRow;
   LocalStartColumn = (int) input_params.StartColumn;
   LocalNumberOfOutputRows = (int) NumberOfOutputRows;
   LocalNumberOfOutputColumns = (int) NumberOfOutputColumns;
   LocalXScaleFactor = (float) input_params.XScaleFactor;
   LocalYScaleFactor = (float) input_params.YScaleFactor;
   LocalActivateFlag = (int) input_params.ActivateFlag;

   //
   //  If the activate flag has been set, check to see if it is 0 (false)
   //  If so, simply return, because we don't want to run this yet.
   //  If the activate flag has not been set or is 1 (true), then go ahead
   //  and run this module.
   //

   if(input_params.ActivateFlag.valid_obj())
   {
      if (LocalActivateFlag == 0)
         return 1;
   }

   //
   //  Create the spot file object and give it the file names.
   //

   // Use AVS/Express function to map enviroment variables
   FILEmap_variables((char *)input_params.HeaderFileName, header_filename_buf);
   FILEmap_variables((char *)input_params.ImageFileName, image_filename_buf);

   if (SpotFile.HeaderFileName(header_filename_buf) != XP_GIS_OK)
   {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                "Error opening spot header file.\n");
      return 1;
   }
   if (SpotFile.ImageFileName(image_filename_buf) != XP_GIS_OK)
   {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                "Error opening spot image file.\n");
      return 1;
   }

   //
   //  If the output rows and/or columns have not been specified, 
   //  default to the all rows/columns.  Make sure we adjust for the
   //  scale factor!
   //

   if (!NumberOfOutputRows.valid_obj())
   {
      LocalNumberOfOutputRows = (unsigned long) (SpotFile.NumberOfRows() * 
                                    LocalYScaleFactor);
   }
   if (!NumberOfOutputColumns.valid_obj())
   {
      LocalNumberOfOutputColumns = (unsigned long) (SpotFile.NumberOfColumns() * 
                                       LocalXScaleFactor);
   }

   //
   //  Allocate the image buffer.  If the band number to read is not
   //  specified, default to all bands.
   //

   NumberOfPixels = LocalNumberOfOutputRows * LocalNumberOfOutputColumns;
   if (!input_params.BandNumberToRead.valid_obj())
   {
      NumberOfPixels *= SpotFile.NumberOfBands();
   }
    
   ImageBuffer = (unsigned char *) malloc(sizeof(unsigned char) *
                                          NumberOfPixels);
   if (ImageBuffer == NULL)
   {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                "Error allocating space for image buffer.\n");
      return 1;
   }

   //
   //  Read the image.
   //

   if (input_params.BandNumberToRead.valid_obj())
   {
      if ((LocalBandNumberToRead < 1) ||
          (LocalBandNumberToRead > SpotFile.NumberOfBands()))
      {
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "The band number to read is invalid.\n");
         free(ImageBuffer);
         return 1;
      }
      PRINT("   BEGIN ReadBand\n");
      ReturnValue = SpotFile.ReadBand(LocalBandNumberToRead,
                                      LocalStartRow,
                                      LocalStartColumn,
                                      LocalNumberOfOutputRows,
                                      LocalNumberOfOutputColumns,
                                      LocalXScaleFactor,
                                      LocalYScaleFactor,
                                      ImageBuffer);
      PRINT("   END ReadBand\n");
      if (ReturnValue != XP_GIS_OK)
      {
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error reading image.\n");
         free(ImageBuffer);
         return 1;
      }
   }
   else
   {
      PRINT("   BEGIN ReadImage\n");
      ReturnValue = SpotFile.ReadImage(LocalStartRow,
                                       LocalStartColumn,
                                       LocalNumberOfOutputRows,
                                       LocalNumberOfOutputColumns,
                                       LocalXScaleFactor,
                                       LocalYScaleFactor,
                                       ImageBuffer);
      PRINT("   END ReadImage\n");
      if (ReturnValue != XP_GIS_OK)
      {
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error reading image.\n");
         free(ImageBuffer);
         return 1;
      }
   }

   //
   //  Load the field data
   //

   int *Image_dims = (int *)Image.dims.ret_array_ptr(OM_GET_ARRAY_WR);

   Image_dims[0] = LocalNumberOfOutputColumns;     // X direction
   Image_dims[1] = LocalNumberOfOutputRows;        // Y direction

   Image.nnode_data = 1;                                   // 1 set of data
   if (input_params.BandNumberToRead.valid_obj())
   {
      Image.node_data[0].veclen = 1;                        // only 1 channel
   }
   else
   {
      Image.node_data[0].veclen = SpotFile.NumberOfBands(); // multi channel
   }
   Image.node_data[0].values.set_array(OM_TYPE_BYTE,
                                       (char *) ImageBuffer,
                                       (int) NumberOfPixels,
                                       OM_SET_ARRAY_FREE);
   if ((int)Image.node_data[0].veclen > 1)  // it is RGB data
   {
      Image.node_data[0].id = 669;
   }
   else                                    // it is grayscale data
   {
      OMset_obj_val((OMobj_id)(Image.node_data[0].id),OMnull_obj,0);
   }

   //
   //  Populate the points array
   //

   Image.npoints = 2;
   float *Image_points = (float *)Image.points.ret_array_ptr(OM_GET_ARRAY_WR);

   Image_points[0] = 0;
   Image_points[1] = 0;
   Image_points[2] = float(LocalNumberOfOutputColumns);     // X direction
   Image_points[3] = float(LocalNumberOfOutputRows);        // Y direction

   if (Image_dims)
      ARRfree((char *)Image_dims);
   if (Image_points)
      ARRfree((char *)Image_points);

   // return 1 for success
   PRINT("END ReadSPOTImage\n");

   return(1);
}

// end of file


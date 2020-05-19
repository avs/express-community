
#include <avs/f_utils.h>

#include "xp_comm_proj/rd_dted/gsdted.h"
#include "xp_comm_proj/rd_dted/gen.hxx"


int
ReadDted_ReadDtedMods_ReadDtedCore::update(OMevent_mask event_mask, int seq_num)
{
   // FileName (OMXstr read req notify)
   // StartRow (OMXint read notify)
   // StartColumn (OMXint read notify)
   // NumberOfOutputRows(OMXint read notify)
   // NumberOfOutputColumns(OMXint read notify)
   // XScaleFactor (OMXfloat read notify)
   // YScaleFactor (OMXfloat read notify)
   // ActivateFlag (OMXint read notify)
   // Data (Field_Rect write)

   // Data.ndim (int) 
   // Data.dims (int []) 
   // Data.nspace (int) 

   // Data.npoints (int)
   // Data.points (float [])

   // Data.nnodes (int)
   // Data.nnode_data (int)

   XP_GIS_DTED_c DTEDFile;
   short         *DataBuffer;
   unsigned long  NumberOfPixels;
   unsigned long  ReturnValue;
   unsigned long  LocalStartRow;
   unsigned long  LocalStartColumn;
   unsigned long  LocalNumberOfOutputRows;
   unsigned long  LocalNumberOfOutputColumns;
   unsigned long  LocalActivateFlag;
   float          LocalXScaleFactor;
   float          LocalYScaleFactor;

   char file_buf[AVS_PATH_MAX];

   /***********************/
   /* Function's Body     */
   /***********************/
#ifdef DEBUG
   ERRverror("ReadDtedCore",ERR_NO_HEADER | ERR_PRINT,"I'm in method: ReadDted_ReadDtedMods_ReadDtedCore::update\n");
#endif

   //
   //  If the activate flag has been set, check to see if it is 0 (false)
   //  If so, simply return, because we don't want to run this yet.
   //  If the activate flag has not been set or is 1 (true), then go ahead
   //  and run this module.
   //

   if(ActivateFlag.valid_obj())
   {
     if ( (int)ActivateFlag == 0) {
        return 1;
     }
   }


   printf("ReadDTED:  Entered routine\n");
   fflush(stdout);

   //
   //  Assign local copies of Express variables.
   //

   LocalStartRow = (int) StartRow;
   LocalStartColumn = (int) StartColumn;
   LocalNumberOfOutputRows = (int) NumberOfOutputRows;
   LocalNumberOfOutputColumns = (int) NumberOfOutputColumns;
   LocalXScaleFactor = (float) XScaleFactor;
   LocalYScaleFactor = (float) YScaleFactor;
   LocalActivateFlag = (int) ActivateFlag;

   printf("ReadDTED:  Finished copying to local values\n");
   fflush(stdout);


   //
   //  Create the dted object and give it the file name.
   //

   FILEmap_variables((char *)FileName, file_buf);

   if (DTEDFile.FileName(file_buf) != XP_GIS_OK)
   {
     ERRverror("ReadDtedCore",ERR_NO_HEADER | ERR_PRINT,"Error opening DTED file.\n");
     return 0;
   }

   printf("ReadDTED:  Finished creating DTED object\n");
   fflush(stdout);


   //
   //  If the start column or row have not been specified, 
   //  default to starting at origin.  Check values if start is specified.
   //
   if (!StartRow.valid_obj()) {
      LocalStartRow = 1;
   }
   else if (LocalStartRow < 1) {
      ERRverror("ReadDtedCore",ERR_NO_HEADER | ERR_PRINT, "Start Row must be at least 1.\n");
      return 0;
   }
   
   if (!StartColumn.valid_obj()) {
      LocalStartColumn = 1;
   }
   else if (LocalStartColumn < 1) {
      ERRverror("ReadDtedCore",ERR_NO_HEADER | ERR_PRINT, "Start Column must be at least 1.\n");
      return 0;
   }

   printf("ReadDTED:  Finished checking start position\n");
   fflush(stdout);
   
   
   //
   //  If the output scaling has not been specified, 
   //  default to 1:1 scaling.  Check values if scaling is specified.
   //
   if (!XScaleFactor.valid_obj()) {
      LocalXScaleFactor = 1.0;
   }
   else if (LocalXScaleFactor <= 0.0) {
      ERRverror("ReadDtedCore",ERR_NO_HEADER | ERR_PRINT, "X Scaling Factor must be greater than zero.\n");
      return 0;
   }
   
   if (!YScaleFactor.valid_obj()) {
      LocalYScaleFactor = 1.0;
   }
   else if (LocalYScaleFactor <= 0.0) {
      ERRverror("ReadDtedCore",ERR_NO_HEADER | ERR_PRINT, "Y Scaling Factor must be greater than zero.\n");
      return 0;
   }

   
   printf("ReadDTED:  Finished checking scaling factors\n");
   fflush(stdout);

   
   //
   //  If the output rows and/or columns have not been specified, 
   //  default to all rows/columns.  Make sure we adjust for the
   //  scale factor!
   //

   if (!NumberOfOutputRows.valid_obj())
   {
     LocalNumberOfOutputRows = (unsigned long)(DTEDFile.NumberOfRows() * LocalYScaleFactor);
   }
   if (!NumberOfOutputColumns.valid_obj())
   {
     LocalNumberOfOutputColumns = (unsigned long)(DTEDFile.NumberOfColumns() * LocalXScaleFactor);
   }

   printf("ReadDTED:  Finished checking output rows and columns\n");
   fflush(stdout);


   //
   // Check that requested data array can be read from input DTEDFile.
   //
   
   if ((LocalStartRow    + (LocalNumberOfOutputRows    / LocalYScaleFactor) - 1 > DTEDFile.NumberOfRows()) ||
       (LocalStartColumn + (LocalNumberOfOutputColumns / LocalXScaleFactor) - 1 > DTEDFile.NumberOfColumns()))
   {
      ERRverror("ReadDtedCore",ERR_NO_HEADER | ERR_PRINT, "Requested data block cannot be read from input data.\n\
Change Start Position, Increase Scaling or Reduce Output Array size.");
      return 0;
   }

   printf("ReadDTED:  Finished checking dimensions of requested data block\n");
   fflush(stdout);

   
   NumberOfPixels = LocalNumberOfOutputRows * LocalNumberOfOutputColumns;
    
   DataBuffer = (short *) malloc(sizeof(short) * NumberOfPixels);
   if (DataBuffer == NULL)
   {
     ERRverror("ReadDtedCore",ERR_NO_HEADER | ERR_PRINT, "Error allocating space for elevation buffer.\n");
     return 0;
   }

   printf("ReadDTED:  Finished allocating DataBuffer\n");
   fflush(stdout);
   printf("ReadDTED:     LocalStartRow = %d\n",LocalStartRow);
   fflush(stdout);
   printf("ReadDTED:     LocalStartColumn = %d\n",LocalStartColumn);
   fflush(stdout);
   printf("ReadDTED:     LocalNumberOfOutputRows = %d\n",LocalNumberOfOutputRows);
   fflush(stdout);
   printf("ReadDTED:     LocalNumberOfOutputColumns = %d\n",LocalNumberOfOutputColumns);
   fflush(stdout);
   printf("ReadDTED:     LocalXScaleFactor = %f\n",LocalXScaleFactor);
   fflush(stdout);
   printf("ReadDTED:     LocalYScaleFactor = %f\n",LocalYScaleFactor);
   fflush(stdout);

   //
   //  Read the elevation data.
   //
   ReturnValue = DTEDFile.ReadData(LocalStartRow,
                                   LocalStartColumn,
                                   LocalNumberOfOutputRows,
                                   LocalNumberOfOutputColumns,
                                   LocalXScaleFactor,
                                   LocalYScaleFactor,
                                   DataBuffer);

   if (ReturnValue != XP_GIS_OK)
   {
     ERRverror("ReadDtedCore",ERR_NO_HEADER | ERR_PRINT, "Error reading elevation data.\n");
     printf("ReadData returned code of %d\n",ReturnValue);
     fflush(stdout);
     return 0;
   }

   printf("ReadDTED:  Finished reading elevation data\n");
   fflush(stdout);


   
   //
   // Get dimension array
   // 
   int *Data_dims = (int *)Data.dims.ret_array_ptr(OM_GET_ARRAY_WR);

   //
   //  Load the field data
   //
   Data_dims[0] = LocalNumberOfOutputColumns;     // X direction
   Data_dims[1] = LocalNumberOfOutputRows;        // Y direction

   //
   //  Populate the points array
   //
   Data.npoints = 2;
   float *Data_points = (float *)Data.points.ret_array_ptr(OM_GET_ARRAY_WR);

   Data_points[0] = (float)DTEDFile.WestLongitude();
   Data_points[1] = (float)DTEDFile.SouthLatitude();
   Data_points[2] = (float)DTEDFile.EastLongitude();     // X direction
   Data_points[3] = (float)DTEDFile.NorthLatitude();     // Y direction

   printf("ReadDTED:  Finished loading point data\n");
   fflush(stdout);

   //
   // Now fill in node data
   //
   Data.nnode_data = 1;                                   // 1 set of data
   Data.node_data[0].veclen = 1;
   Data.node_data[0].values.set_array(OM_TYPE_SHORT,
                                     (char *)DataBuffer,
                                     (int)NumberOfPixels,
                                     OM_SET_ARRAY_FREE);

   OMset_obj_val( (OMobj_id)(Data.node_data[0].id), OMnull_obj, 0 );

   printf("ReadDTED:  Finished loading node data\n");
   fflush(stdout);

   if (Data_dims)
      ARRfree((char *)Data_dims);

   if (Data_points)
      ARRfree((char *)Data_points);

   printf("ReadDTED:  Leaving routine\n");
   fflush(stdout);

   // return 1 for success
   return(1);
}

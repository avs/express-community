
#include "avs/err.h"
#include "avs/gd_def.h"
#include "gen.h"

#define ERROR_MSG(a) ERRerror("GISread_gtiff::read", 1, ERR_ORIG, a)

#define INPUT_MODE_ROWCOL    0
#define INPUT_MODE_LATLON    1

int
ReadGeoTiff_ReadGeoTiffMods_readgeotiff::read(OMevent_mask event_mask, int seq_num)
{
    // Filename (OMXstr read req notify)
    // InputMode (OMXenum read req notify)
    // ChipStartRow (OMXint read req notify)
    // ChipStartColumn (OMXint read req notify)
    // ChipNumberOfRows (OMXint read req notify)
    // ChipNumberOfColumns (OMXint read req notify)

    // ChipSouthLatitude (OMXdouble read req notify)
    // ChipWestLongitude (OMXdouble read req notify)
    // ChipNorthLatitude (OMXdouble read req notify)
    // ChipEastLongitude (OMXdouble read req notify)

    // FileSouthLatitude (OMXdouble write)
    // FileWestLongitude (OMXdouble write)
    // FileNorthLatitude (OMXdouble write)
    // FileEastLongitude (OMXdouble write)

    // FileLongitudeResolution (OMXdouble write)
    // FileLatitudeResolution (OMXdouble write)

    // OutField (Mesh_Unif+Node_Data write)
    // OutField.ndim (int) 
    // OutField.dims (int []) 
    // OutField.nspace (int) 
    // OutField.npoints (int)
    // OutField.points (float [])
    // OutField.nnodes (int)
    // OutField.nnode_data (int)
    // OutField.node_data[0].veclen (int) 
    // OutField.node_data[0].values (char []) 
    int   *OutFieldDims;
    float *OutFieldPoints;
    int   ArraySize, ArrayType;
    char  *OutFieldNodeData;

    XP_GIS_GTIFF_c *GISGTiffObject = NULL;

    int ImagePixelDepth;
    unsigned long ReturnValue;

    char         *LocalFilename;

    double LocalWestLongitude;
    double LocalSouthLatitude;
    double LocalLongitudePixelSize;
    double LocalLatitudePixelSize;

    int    FileNumberOfColumns;
    int    FileNumberOfRows;

    int    LocalInputMode;

    /***********************/
    /* Function's Body     */
    /***********************/

    LocalInputMode = (int)InputMode;

    LocalFilename = (char *)Filename;

    // create a XP_GIS_GTIFF_c object
    GISGTiffObject = new XP_GIS_GTIFF_c(LocalFilename);

    if (GISGTiffObject == NULL)
    {
      ERROR_MSG("Error creating GISGTiffObject");
      return OM_STAT_FAILURE;
    }

    ReturnValue = GISGTiffObject->FileIsOpen();
    if( ReturnValue != XP_GIS_OK )
    {
      ERROR_MSG("could not open file");
      delete GISGTiffObject;
      return OM_STAT_FAILURE;
    }

    if( Filename.changed(seq_num) )
    {
      // if the filename has changed, clear all of the input fields
      // to cause the whole image to be read.
	  ChipStartRow = 0;
	  ChipStartColumn =0;
	  ChipNumberOfRows = (int)GISGTiffObject->NumberOfRows();;
	  ChipNumberOfColumns = (int)GISGTiffObject->NumberOfColumns();
    }

    FileNumberOfColumns = (int)GISGTiffObject->NumberOfColumns();
    FileNumberOfRows    = (int)GISGTiffObject->NumberOfRows();

    GISGTiffObject->SWCorner( &LocalWestLongitude,
                              &LocalSouthLatitude);

    GISGTiffObject->PixelScale( &LocalLongitudePixelSize,
                                &LocalLatitudePixelSize);

    FileWestLongitude = LocalWestLongitude;
    FileSouthLatitude = LocalSouthLatitude;
    FileEastLongitude = LocalWestLongitude +
                    LocalLongitudePixelSize * FileNumberOfColumns;
    FileNorthLatitude = LocalSouthLatitude +
                    LocalLatitudePixelSize * FileNumberOfRows;

    FileLongitudeResolution = LocalLongitudePixelSize;
    FileLatitudeResolution = LocalLatitudePixelSize;

    if( LocalInputMode == INPUT_MODE_LATLON )
    {
      //
      // If ChipSouthLatitude and ChipWestLongitude are valid,
      // use them to control the chip location.  Otherwise read
      // the whole image.
      //
  
      if( ChipSouthLatitude.valid_obj() )
      {
        ChipStartRow =
                    ((double)ChipSouthLatitude - FileSouthLatitude) /
                    ((double)FileNorthLatitude - FileSouthLatitude) *
                    (FileNumberOfRows -1);
  
      }
      else
      {
        ChipStartRow = 0;
        ChipSouthLatitude = (double)FileSouthLatitude;
      }

      if( ChipWestLongitude.valid_obj() )
      {
        ChipStartColumn =
                    ((double)ChipWestLongitude - FileWestLongitude) /
                    ((double)FileEastLongitude - FileWestLongitude) *
                    (FileNumberOfColumns -1);
      }
      else
      {
        ChipStartColumn = 0;
        ChipWestLongitude = (double)FileWestLongitude;
      }
  
      //
      // If ChipNorthLatitude and ChipEastLongitude have been
      // specified, use them to control chip size.  Otherwise
      // read all of the image that is north and east of
      // ChipSouthLatitude and ChipWestLongitude.
      //

      if( ChipNorthLatitude.valid_obj() )
      {
        ChipNumberOfRows =
                  ((double)ChipNorthLatitude - ChipSouthLatitude) /
                  LocalLatitudePixelSize;
      }
      else
      {
        // set to rest of image
        ChipNumberOfRows =
                  ((double)FileNorthLatitude - ChipSouthLatitude) /
                  LocalLatitudePixelSize;
        ChipNorthLatitude = (double)FileNorthLatitude;
      }

      if( ChipEastLongitude.valid_obj() )
      {
        ChipNumberOfColumns =
                  ((double)ChipEastLongitude - ChipWestLongitude) /
                  LocalLongitudePixelSize;
      }
      else
      {
        // set to rest of image
        ChipNumberOfColumns =
                  ((double)FileEastLongitude - ChipWestLongitude) /
                  LocalLongitudePixelSize;
        ChipEastLongitude = (double)FileEastLongitude;
      }
    }

    else  // InputMode is "Row/Column"
    {

      if( !ChipStartRow.valid_obj() )
      {
        ChipStartRow = 0;
        ChipNumberOfRows    = FileNumberOfRows;
        ChipNorthLatitude = (double)FileNorthLatitude;
      }
  
      if( !ChipStartColumn.valid_obj() )
      {
        ChipStartColumn     = 0;
        ChipNumberOfColumns = FileNumberOfColumns;
        ChipWestLongitude = (double)FileWestLongitude;
      }
  
      //
      // If user hasn't set numger of Rows/Cols (or we haven't
      // computed them above), read the rest of the image
      //
  
      if( !ChipNumberOfRows.valid_obj() )
      {
        ChipNumberOfRows    = FileNumberOfRows - ChipStartRow;
      }

      if( !ChipNumberOfColumns.valid_obj() )
      {
        ChipNumberOfColumns = FileNumberOfColumns - ChipStartColumn;
      }

      //
      // compute chip boundaries in Lat/Lon
      //
  
      if( ChipStartRow == 0 )
      {
        ChipNorthLatitude = (double)FileNorthLatitude;
      }
      else
      {
        ChipNorthLatitude = (double)FileNorthLatitude -
                   ((double)ChipStartRow * FileLatitudeResolution);
      }
  
      if( ChipStartColumn == 0 )
      {
        ChipWestLongitude = (double)FileWestLongitude;
      }
      else
      {
        ChipWestLongitude = (double)FileWestLongitude +
                   ((double)ChipStartColumn * FileLongitudeResolution);
      }
  
      if( (int)ChipNumberOfRows == FileNumberOfRows )
      {
        ChipSouthLatitude = (double)FileSouthLatitude;
      }
      else
      {
        ChipSouthLatitude = (double)ChipNorthLatitude -
                   ((double)ChipNumberOfRows * FileLatitudeResolution);
      }
   
      if( (int)ChipNumberOfColumns == FileNumberOfColumns )
      {
        ChipEastLongitude = (double)FileEastLongitude;
      }
      else
      {
        ChipEastLongitude = (double)ChipWestLongitude +
                   ((double)ChipNumberOfColumns * FileLongitudeResolution);
      }

    }

    // Check to see if the Image size the user specified is within the 
    // area of the total image.

    ReturnValue = GISGTiffObject->CheckImageSize(
                                       (unsigned long)ChipStartRow,
                                       (unsigned long)ChipStartColumn,
                                       (unsigned long)ChipNumberOfRows,
                                       (unsigned long)ChipNumberOfColumns);
    if (ReturnValue != XP_GIS_OK)
    {
      ERROR_MSG("requested chip bounds not in image");
      delete GISGTiffObject;
      return OM_STAT_FAILURE;
    }

    //
    // Set the output field's dimensions.
    //

    OutFieldDims = (int *)OutField.dims.ret_array_ptr(
                                                OM_GET_ARRAY_WR);
    if (!OutFieldDims)
    {
      ERROR_MSG( "can't get OutFieldDims array for write" );
      delete GISGTiffObject;
      return OM_STAT_FAILURE;
    }

    OutFieldDims[0] = (int)ChipNumberOfColumns;
    OutFieldDims[1] = (int)ChipNumberOfRows;

    ARRfree((char *)OutFieldDims);

    //
    // Set data characteristics of output field
    //

    OutField.nnode_data = 1;

    ImagePixelDepth = GISGTiffObject->DepthOfImage();

    if (ImagePixelDepth == 8)
    {
      OutField.node_data[0].veclen = 1;
      OMset_obj_val((OMobj_id)OutField.node_data[0].id,OMnull_obj,0);
      FLDset_node_data_type(OutField.obj_id(),0,DTYPE_BYTE);
    }

    else if (ImagePixelDepth == 16)
    {
      OutField.node_data[0].veclen = 1;
      OMset_obj_val((OMobj_id)OutField.node_data[0].id,OMnull_obj,0);
      FLDset_node_data_type(OutField.obj_id(),0,DTYPE_SHORT);

    }
    else if (ImagePixelDepth == 24) 
    {
      OutField.node_data[0].veclen = 4;
      OutField.node_data[0].id = GD_RGB_DATA_ID;
      FLDset_node_data_type(OutField.obj_id(),0,DTYPE_BYTE);
    }
    else 
    {
      ERROR_MSG( "image depth not 8,16 or 24 bits" );
      delete GISGTiffObject;
      return OM_STAT_FAILURE;
    }

    //
    // setting the node data type makes the field forget the
    // definitions of min, max, min_vec and max_vec.  They are
    // normally defined as functions on the "values" array.
    // Use FLDreset_xx functions to restore the function-based
    // definition of these field elements.
    //

    ReturnValue = FLDreset_node_minmax(OutField.obj_id(), 0 );
    if (ReturnValue != OM_STAT_SUCCESS)
    {
      ERROR_MSG("error resetting OutField node data min/max\n" );
      delete GISGTiffObject;
      return OM_STAT_FAILURE;
    }

    ReturnValue = FLDreset_node_minmax_vec( OutField.obj_id(), 0 );
    if (ReturnValue != OM_STAT_SUCCESS)
    {
      ERROR_MSG("error resetting OutField node data min/max vec\n" );
      delete GISGTiffObject;
      return OM_STAT_FAILURE;
    }

    //
    // Get the pointer to the node data and read the data.
    //

    OutFieldNodeData =
          (char *)OutField.node_data[0].values.ret_array_ptr(
                                            OM_GET_ARRAY_WR,
                                            &ArraySize, &ArrayType);
    if (!OutFieldNodeData)
    {
      ERROR_MSG( "can't get OutFieldNodeData array for write" );
      delete GISGTiffObject;
      return OM_STAT_FAILURE;
    }

    if (ImagePixelDepth == 16)
    {
      ReturnValue = GISGTiffObject->ReadData((unsigned long)ChipStartRow,
                                        (unsigned long)ChipStartColumn, 
                                        (unsigned long)ChipNumberOfRows,
                                        (unsigned long)ChipNumberOfColumns,
                                        (short *)OutFieldNodeData);
      if (ReturnValue != XP_GIS_OK)
      {
        ERROR_MSG("error reading GeoTIFF image data (short)\n" );
        ARRfree((char *)OutFieldNodeData);
        delete GISGTiffObject;
        return OM_STAT_FAILURE;
      }
    }
    else
    {
      ReturnValue = GISGTiffObject->ReadData((unsigned long)ChipStartRow,
                                  (unsigned long)ChipStartColumn, 
                                  (unsigned long)ChipNumberOfRows,
                                  (unsigned long)ChipNumberOfColumns,
                                  (unsigned char *)OutFieldNodeData);
      if (ReturnValue != XP_GIS_OK)
      {
        ERROR_MSG("error reading GeoTIFF image data (byte)\n" );
        ARRfree((char *)OutFieldNodeData);
        delete GISGTiffObject;
        return OM_STAT_FAILURE;
      }
    }

    ARRfree((char *)OutFieldNodeData);

    //
    // Set the output field's points array to the location of
    // the chip.
    //

    OutFieldPoints = (float *)OutField.points.ret_array_ptr(
                                               OM_GET_ARRAY_WR);
    if (!OutFieldPoints)
    {
      ERROR_MSG( "can't get OutFieldPoints array for write" );
      return OM_STAT_FAILURE;
    }

    OutFieldPoints[0] = FileWestLongitude +
                         ChipStartColumn * LocalLongitudePixelSize;
    OutFieldPoints[1] = FileNorthLatitude -
                         ChipStartRow * LocalLatitudePixelSize;
    OutFieldPoints[2] = OutFieldPoints[0] + ChipNumberOfColumns *
                         LocalLongitudePixelSize;
    OutFieldPoints[3] = OutFieldPoints[1] - ChipNumberOfRows *
                         LocalLatitudePixelSize;

    ARRfree((char *)OutFieldPoints);

    delete GISGTiffObject;

    return OM_STAT_SUCCESS;
}







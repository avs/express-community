
#include <stdio.h>
#include "geotiff.h"   /* public interface        */
#include "geo_tiffp.h" /* external TIFF interface */
//#include "geokeys.h"
#include "tiffio.h"
#include "tiffiop.h"
#include "xtiffiop.h"   /* public interface        */

#include "gsgtiff.h"

//
//  Constructor.
//

XP_GIS_GTIFF_c::XP_GIS_GTIFF_c(const char *LocalFileName)
{
  _Initialization(LocalFileName, GEOTIFF_READ);
}

XP_GIS_GTIFF_c::XP_GIS_GTIFF_c(const char *LocalFileName,
                               ReadWriteMode_e Mode)
{
  _Initialization(LocalFileName, Mode);
}

void
XP_GIS_GTIFF_c::_Initialization(const char *LocalFileName,
                                ReadWriteMode_e Mode)
{
    //
    //  Initialize strings to empty.
    //

    _FileName[0] = '\0';

    //
    //  Initialize file state to closed.
    //

    // Substituted zero for false here
    _FileIsOpen = XP_GIS_FALSE;

    //
    //  If the file name is not null, then use it to
    //  set the object's file name.
    //

    if (LocalFileName != NULL)
    {
      if( Mode == GEOTIFF_READ )
      {
        FileName(LocalFileName);
      }
      else
      {
        FileNameWrite(LocalFileName);
      }
    }

    _BufferAllocated = XP_GIS_FALSE;
    _InternalBufferPointerChar = NULL;
    _InternalBufferPointerShort = NULL;
    _InternalBufferPointerFloat = NULL;

}

//
//  destructor.
//

XP_GIS_GTIFF_c::~XP_GIS_GTIFF_c()
{
    //
    //  Close open file.
    //

    _CloseFile();

   // Delete allocated buffers.
   if (_BufferAllocated)
   {
      if (_InternalBufferPointerChar) 
      {
        delete _InternalBufferPointerChar;
        _InternalBufferPointerChar = NULL;
      }
      if (_InternalBufferPointerShort) 
      {
        delete _InternalBufferPointerShort;
        _InternalBufferPointerShort = NULL;
      }
      if (_InternalBufferPointerFloat) 
      {
        delete _InternalBufferPointerFloat;
        _InternalBufferPointerFloat = NULL;
      }
   }
}

//
//  Copy constructor.
//

XP_GIS_GTIFF_c::XP_GIS_GTIFF_c(const XP_GIS_GTIFF_c &object)
{
    //
    //  Initialize pointers to NULL.
    //

    _InternalBufferPointerChar = NULL;
    _InternalBufferPointerShort = NULL;
    _InternalBufferPointerFloat = NULL;

    *this = object;
}

//
//  Assignment operator.
//

XP_GIS_GTIFF_c &XP_GIS_GTIFF_c::operator=(
                      const XP_GIS_GTIFF_c &object)
{

    //
    //  To set up the object properly, we have to set the
    //  file name via the accessor.  This will set the private
    //  variables and open the files.  Since this object opens the files
    //  read-only, there should be no conflicts with open files.
    //

    _FileIsOpen = XP_GIS_FALSE;
    FileName(object._FileName);

    return *this;
}

//
//  Method to set the file name.  If there is already a 
//  file opened, this method closes the file before attempting
//  to open the newly specified file.  Returns XP_GIS_OK if successful.
//  Otherwise, returns one of the following:
//
//    XP_GIS_OPEN_ERROR;
//

unsigned long XP_GIS_GTIFF_c::FileName(const char* LocalFileName) 
{


    //
    //  If a null is passed in, then close any open files.
    //

    if (LocalFileName == NULL)
    {
      _CloseFile();
      return XP_GIS_OK;
    }

    //
    //  If there is already a file open, close it.
    //

    if (_FileName[0] != '\0')
    {
      _CloseFile();
    }

    //
    //  Copy the file name that was passed in to the private variable.
    //  If does not have a suffix of .dt1, assume it is a base name, and
    //  add the suffix.
    //

    strcpy(_FileName,LocalFileName);
    if (strcmp(&_FileName[strlen(_FileName)-4],".tif") != 0) 
    {
      strcat(_FileName,".tif");
    }

    //
    //  Now, attempt to open the requested file.  If not successful,
    //  clear the file name.  Otherwise, populate the file name and
    //  read the file header.
    //

    /* Open TIFF descriptor to read GeoTIFF tags */
    _tif = XTIFFOpen(_FileName,"r");

    if (!_tif)
    {
        _FileIsOpen = XP_GIS_FALSE;
        return XP_GIS_OPEN_ERROR;
    }
    
    /* Open GTIF Key parser; keys will be read at this time. */
    _gtif = GTIFNew(_tif);
    if (!_gtif)
    {
        _FileIsOpen = XP_GIS_FALSE;
        return XP_GIS_OPEN_ERROR;
    }
    else
    {

        int versions[3];
        geocode_t model;    /* all key-codes are of this type */
        double *data;
        double *original_data;
        int count;
        TIFFDirectory *td;


        _FileIsOpen = XP_GIS_TRUE;
        _FileMode = GEOTIFF_READ;

        /* Get the GeoTIFF directory info */
        GTIFDirectoryInfo(_gtif,versions,0);
        if (versions[MAJOR] > 1)
        {
            return XP_GIS_BAD_VERSION;
        }

        if (!GTIFKeyGet(_gtif, GTModelTypeGeoKey, &model, 0, 1))
        {
            return XP_GIS_ERROR;
        }

        td = &_tif->tif_dir; 
        _Orientation = XP_GIS_GTIFF_c::TW_ORIENTATION_TOPLEFT;
	if (TIFFFieldSet(_tif,FIELD_ORIENTATION)) 
        {
          if (td->td_orientation <= ORIENTATION_LEFTBOT)
          {
            _Orientation = (Orientation_e)(td->td_orientation - 1);
          }
	}

        // This is the information for the LAT LON points.
        // The first three data items are the point in the image that the 
        // TIE POINTs are valid for and the second three are the lat lon 
        // for that image position.

        if ((_gtif->gt_methods.get)(_tif, GTIFF_TIEPOINTS, &count, &data ))
        {
          original_data = data;
          _RasterSpacePosition[0] = *data++;
          _RasterSpacePosition[1] = *data++;
          _RasterSpacePosition[2] = *data++;
          _LonLat[0] = *data++;
          _LonLat[1] = *data++;
          _LonLat[2] = *data++;
          _GTIFFree(original_data);
        }
        if ((_gtif->gt_methods.get)(_tif, GTIFF_PIXELSCALE, &count, &data ))
        {
          original_data = data;
          _PixelScale[0] = *data++;
          _PixelScale[1] = *data++;
          _PixelScale[2] = *data++;
          _GTIFFree(original_data);
        }

        // Set Latitude and Longitude for the whole image.

        _NWLongitude = _LonLat[0]; 
        _NWLatitude = _LonLat[1]; 
        _NELongitude = _NWLongitude + (_PixelScale[0] * NumberOfColumns());
        _NELatitude = _NWLatitude;
        _SWLongitude = _NWLongitude;
        _SWLatitude = _NWLatitude - (_PixelScale[1] * NumberOfRows());
        _SELongitude = _NELongitude;
        _SELatitude = _SWLatitude;

    }

    return XP_GIS_OK;     
}

unsigned long XP_GIS_GTIFF_c::FileNameWrite(const char* LocalFileName) 
{


    //
    //  If a null is passed in, then close any open files.
    //

    if (LocalFileName == NULL)
    {
      _CloseFile();
      return XP_GIS_OK;
    }

    //
    //  If there is already a file open, close it.
    //

    if (_FileName[0] != '\0')
    {
      _CloseFile();
    }

    //
    //  Copy the file name that was passed in to the private variable.
    //  If does not have a suffix of .dt1, assume it is a base name, and
    //  add the suffix.
    //

    strcpy(_FileName,LocalFileName);
    if (strcmp(&_FileName[strlen(_FileName)-4],".tif") != 0) 
    {
      strcat(_FileName,".tif");
    }

    //
    //  Now, attempt to open the requested file.  If not successful,
    //  clear the file name.  Otherwise, populate the file name and
    //  read the file header.
    //

    /* Open TIFF descriptor to read GeoTIFF tags */
    _tif = XTIFFOpen(_FileName,"w");

    if (!_tif)
    {
        _FileIsOpen = XP_GIS_FALSE;
        return XP_GIS_OPEN_ERROR;
    }
    
    /* Open GTIF Key parser; keys will be read at this time. */
    _gtif = GTIFNew(_tif);
    if (!_gtif)
    {
        _FileIsOpen = XP_GIS_FALSE;
        return XP_GIS_OPEN_ERROR;
    }

    _FileIsOpen = XP_GIS_TRUE;
    _FileMode = GEOTIFF_WRITE;

    return XP_GIS_OK;     
}

unsigned long XP_GIS_GTIFF_c::FileIsOpen() const
{
    if( _FileIsOpen )
    {
      return XP_GIS_OK;
    }
    else
    {
      return XP_GIS_NOT_OPEN;
    }
}

void XP_GIS_GTIFF_c::_CloseFile()
{

    //
    //  Close the file.
    //

    if (_FileIsOpen == XP_GIS_TRUE)
    {

      _FileIsOpen = XP_GIS_FALSE;
      _FileName[0] = '\0';

      /* get rid of the key parser */
      GTIFFree(_gtif);

      /* close the TIFF file descriptor */
      XTIFFClose(_tif);

    }

    return;
}

//
// The South West Corner of the image returned in the doubles.
//
unsigned long XP_GIS_GTIFF_c::SWCorner(double *SWLongitude,double *SWLatitude) const
{

  if (_FileIsOpen == XP_GIS_FALSE)
  {
    return XP_GIS_ERROR;
  }

  *SWLongitude = _SWLongitude;
  *SWLatitude = _SWLatitude;

  return XP_GIS_OK;

}

unsigned long XP_GIS_GTIFF_c::NWCorner(double NWLongitude,double NWLatitude)
{

  if (_FileIsOpen == XP_GIS_FALSE)
  {
    return XP_GIS_ERROR;
  }
  if( _FileMode != GEOTIFF_WRITE )
  {
    return XP_GIS_ERROR;
  }

  _NWLongitude = NWLongitude;
  _NWLatitude = NWLatitude;

  return XP_GIS_OK;

}

//
// The Pixel scale of the image returned in the doubles.
//
unsigned long XP_GIS_GTIFF_c::PixelScale(double *PixelScaleLongitude,
                                       double *PixelScaleLatitude) const
{

  if (_FileIsOpen == XP_GIS_FALSE)
  {
    return XP_GIS_ERROR;
  }

  *PixelScaleLongitude = _PixelScale[0];
  *PixelScaleLatitude = _PixelScale[1];

  return XP_GIS_OK;

}

unsigned long XP_GIS_GTIFF_c::PixelScale(double PixelScaleLongitude,
                                         double PixelScaleLatitude)
{

  if (_FileIsOpen == XP_GIS_FALSE)
  {
    return XP_GIS_ERROR;
  }

  if( _FileMode != GEOTIFF_WRITE )
  {
    return XP_GIS_ERROR;
  }

  _PixelScale[0] = PixelScaleLongitude;
  _PixelScale[1] = PixelScaleLatitude;
  _PixelScale[2] = 0.0;

  return XP_GIS_OK;

}

//
// The number of rows are returned. The file must be open.
//
int XP_GIS_GTIFF_c::NumberOfRows() const
{

  unsigned int rows;


  if (_FileIsOpen == XP_GIS_FALSE)
  {
    return 0;
  }

  TIFFGetField(_tif,TIFFTAG_IMAGELENGTH,&rows);

  return(rows);
 
}

//
// The number of columns are returned. The file must be open.
//
int XP_GIS_GTIFF_c::NumberOfColumns() const
{

  unsigned int columns;

  if (_FileIsOpen == XP_GIS_FALSE)
  {
    return 0;
  }

  TIFFGetField(_tif,TIFFTAG_IMAGEWIDTH,&columns);

  return(columns);

}

//
// Returns the orientation of the image.
//
XP_GIS_GTIFF_c::Orientation_e XP_GIS_GTIFF_c::GetOrientation(void)
{

  return _Orientation;

}
//
// Checks the validity of the requested chip.
//
unsigned long XP_GIS_GTIFF_c::CheckImageSize(unsigned long RowStart,
                                   unsigned long ColumnStart,
                                   unsigned long RowsInBuffer,
                                   unsigned long ColumnsInBuffer) const

{


  if (_FileIsOpen == XP_GIS_FALSE)
  {
    return XP_GIS_ERROR;
  }

  if ((RowStart + RowsInBuffer > NumberOfRows()) ||
      (ColumnStart + ColumnsInBuffer > NumberOfColumns()))
  {
    printf("XP_GIS_DTED_C::ReadData(lots) -- row/column start check bad\n");
    printf("    RowStart = %d\n",RowStart);
    printf("    ColumnStart = %d\n",ColumnStart);
    printf("    RowsInBuffer = %d\n",RowsInBuffer);
    printf("    ColumnsInBuffer = %d\n",ColumnsInBuffer);
    printf("    NumberOfRows() = %d\n",NumberOfRows());
    printf("    NumberOfColumns() = %d\n",NumberOfColumns());
    fflush(stdout);
    return XP_GIS_BAD_VALUE;
  }
  else 
  {
    return XP_GIS_OK;
  }


}

//
// Returns the depth of the image.
//
int XP_GIS_GTIFF_c::DepthOfImage() const
{

  int depth;
  uint16 bitspersample;
  uint16 samplesperpixel;

  TIFFGetField(_tif,TIFFTAG_BITSPERSAMPLE,&bitspersample);
  TIFFGetField(_tif,TIFFTAG_SAMPLESPERPIXEL,&samplesperpixel);

  depth = bitspersample * samplesperpixel;

  return(depth);

}

//
// This method creates the memory for a buffer and reads in the data from the
// GeoTiff file. This is for 8 bit images.
//
//  If successfull, returns XP_GIS_OK.  
//
unsigned long XP_GIS_GTIFF_c::ReadChip(unsigned long RowStart,
				       unsigned long ColumnStart,
                                       unsigned long RowsInBuffer,
                                       unsigned long ColumnsInBuffer,
				       unsigned char **Buffer) 

{

  int imagedepth = DepthOfImage();
  int buffersize;

  if (imagedepth == 24)
  {
    buffersize = (RowsInBuffer - RowStart) * (ColumnsInBuffer - ColumnStart) * 4;    
  } 
  else
  {
    buffersize = (RowsInBuffer - RowStart) * (ColumnsInBuffer - ColumnStart);
  }
  *Buffer = new unsigned char[buffersize];
  if (!(*Buffer))
  {
    _BufferAllocated = XP_GIS_FALSE;
    return XP_GIS_ERROR;
  }

  _BufferAllocated = XP_GIS_TRUE;
  _InternalBufferPointerChar = *Buffer;

  return(ReadData(RowStart,ColumnStart,RowsInBuffer,ColumnsInBuffer,*Buffer));

}


//
// This method creates the memory for a buffer and reads in the data from the
// GeoTiff file. This is for 16 bit images.
//
//  If successfull, returns XP_GIS_OK.  
//
unsigned long XP_GIS_GTIFF_c::ReadChip(unsigned long RowStart,
				       unsigned long ColumnStart,
                                       unsigned long RowsInBuffer,
                                       unsigned long ColumnsInBuffer,
				       short **Buffer) 

{

  int buffersize = (RowsInBuffer - RowStart) * (ColumnsInBuffer - ColumnStart);
  *Buffer = new short[buffersize];
  if (!(*Buffer))
  {
    _BufferAllocated = XP_GIS_FALSE;
    return XP_GIS_ERROR;
  }

  _BufferAllocated = XP_GIS_TRUE;
  _InternalBufferPointerShort = *Buffer;

  return(ReadData(RowStart,ColumnStart,RowsInBuffer,ColumnsInBuffer,*Buffer));

}

//
// This method creates the memory for a buffer and reads in the data from the
// GeoTiff file. This is for floats
//
//  If successfull, returns XP_GIS_OK.  
//
unsigned long XP_GIS_GTIFF_c::ReadChip(unsigned long RowStart,
				       unsigned long ColumnStart,
                                       unsigned long RowsInBuffer,
                                       unsigned long ColumnsInBuffer,
				       float **Buffer) 

{

  int buffersize = (RowsInBuffer - RowStart) * (ColumnsInBuffer - ColumnStart);
  *Buffer = new float[buffersize];
  if (!(*Buffer))
  {
    _BufferAllocated = XP_GIS_FALSE;
    return XP_GIS_ERROR;
  }

  _BufferAllocated = XP_GIS_TRUE;
  _InternalBufferPointerFloat = *Buffer;

  return(ReadData(RowStart,ColumnStart,RowsInBuffer,ColumnsInBuffer,*Buffer));

}

//
//  Method to read the geotiff data.  This method reads a chip
//  of an image
//
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_ERROR
//

unsigned long XP_GIS_GTIFF_c::ReadData(unsigned long RowStart,
				       unsigned long ColumnStart,
                                       unsigned long RowsInBuffer,
                                       unsigned long ColumnsInBuffer,
				       unsigned char *Buffer)
{

    unsigned long ReturnValue;

    ReturnValue = CheckImageSize(RowStart,ColumnStart,
                                 RowsInBuffer,ColumnsInBuffer);

    if (ReturnValue == XP_GIS_BAD_VALUE)
    {
      return XP_GIS_BAD_VALUE;
    }

    int depth = DepthOfImage();
    if (depth == 32)
    {
      return XP_GIS_ERROR;
    }
    // Handle the non-tiled images
    else if (!TIFFIsTiled(_tif))
    {
      _ReadDataUnTiled(RowStart,ColumnStart,
                                    RowsInBuffer,ColumnsInBuffer,Buffer);
    }

    // Handle the tiled images

    else
    {
      return XP_GIS_ERROR;
    }

    return XP_GIS_OK;

}

//
// Checks for the type of image file and reads the chip based on
// row start and column start and number of rows and number of columns. 
//
unsigned long XP_GIS_GTIFF_c::ReadData(unsigned long RowStart,
				       unsigned long ColumnStart,
                                       unsigned long RowsInBuffer,
                                       unsigned long ColumnsInBuffer,
				       short *Buffer)
{

    unsigned long ReturnValue;

    ReturnValue = CheckImageSize(RowStart,ColumnStart,
                                 RowsInBuffer,ColumnsInBuffer);

    if (ReturnValue == XP_GIS_BAD_VALUE)
    {
      return XP_GIS_BAD_VALUE;
    }

    int depth = DepthOfImage();
    if (depth == 32)
    {
      return XP_GIS_ERROR;
    }
    // Handle the non-tiled images
    else if (!TIFFIsTiled(_tif))
    {
      _ReadDataUnTiled(RowStart,ColumnStart,
                                    RowsInBuffer,ColumnsInBuffer,Buffer);
    }

    // Handle the tiled images

    else
    {
      return XP_GIS_ERROR;
    }

    return XP_GIS_OK;

}

//
// Checks for the type of image file and reads the chip based on
// row start and column start and number of rows and number of columns. 
//
unsigned long XP_GIS_GTIFF_c::ReadData(unsigned long RowStart,
				       unsigned long ColumnStart,
                                       unsigned long RowsInBuffer,
                                       unsigned long ColumnsInBuffer,
				       float *Buffer)
{

    unsigned long ReturnValue;

    ReturnValue = CheckImageSize(RowStart,ColumnStart,
                                 RowsInBuffer,ColumnsInBuffer);

    if (ReturnValue == XP_GIS_BAD_VALUE)
    {
      return XP_GIS_BAD_VALUE;
    }

    int depth = DepthOfImage();
    if (depth != 32)
    {
      return XP_GIS_ERROR;
    }
    // Handle the non-tiled images
    else if (!TIFFIsTiled(_tif))
    {
      _ReadDataUnTiled(RowStart,ColumnStart,
                                    RowsInBuffer,ColumnsInBuffer,Buffer);
    }

    // Handle the tiled images

    else
    {
      return XP_GIS_ERROR;
    }

    return XP_GIS_OK;

}

//
//  Method to read the geotiff data that is Untiled.  This method reads the 
//  image based on the arguments with ColumnsInBuffer and RowsInBuffer the 
//  nuber of rows and columns to read. These are byte based images.
//
//  The RowStart and ColumnStart are zero based (start at 0)
//

unsigned long XP_GIS_GTIFF_c::_ReadDataUnTiled(unsigned long RowStart,
				              unsigned long ColumnStart,
					      unsigned long RowsInBuffer,
                                              unsigned long ColumnsInBuffer,
					      unsigned char *Buffer)



{

  uint32 height;
  uint16 config;
  uint16 orientation;
  tdata_t buf;
  int row;
  int col;
  int bufferrow;
  int scanlinesize;
  int index;
  unsigned char *newbuf;
  uint16 bitspersample;
  uint16 samplesperpixel;
  int width;	


  TIFFGetField(_tif,TIFFTAG_BITSPERSAMPLE,&bitspersample);
  TIFFGetField(_tif,TIFFTAG_SAMPLESPERPIXEL,&samplesperpixel);
  TIFFGetField(_tif, TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(_tif, TIFFTAG_PLANARCONFIG, &config);
  TIFFGetField(_tif, TIFFTAG_ORIENTATION, &orientation);

  // This is the size of a scan line with the number of channels figured in.
  scanlinesize = TIFFScanlineSize(_tif);
  buf = _TIFFmalloc(scanlinesize);

  // Scanline based images

  if (config == PLANARCONFIG_CONTIG) 
  {
      // For the case of 3 values per pixel (RGB) the alpha channel must be 
      // added
      if (samplesperpixel == 3) 
      {
          // An empty alpha channel must be added for express so the pixels
	  // must be put in the buffer with a 0 alpha.
          width = ColumnsInBuffer * 4;
//	  bufferrow = RowsInBuffer - 1;
	  bufferrow = 0;
          for (row = RowStart; row < (RowStart + RowsInBuffer); row++)
          {

	    // This reads one Scanline from the image.
            TIFFReadScanline(_tif, buf, row);
	    newbuf = (unsigned char *)buf;
	    index = ColumnStart * 3;

	    // For each column in the row assign to the newbuffer the values.
  	    for (col = 0;col < width;col += 4)
            {
	      Buffer[(bufferrow * width) + col] = 0;
	      Buffer[(bufferrow * width) + col + 1] = newbuf[index];
	      Buffer[(bufferrow * width) + col + 2] = newbuf[index + 1];
	      Buffer[(bufferrow * width) + col + 3] = newbuf[index + 2];
              index += 3;
	    }
//	    bufferrow--;
	    bufferrow++;
          }
      }
      else
      {
          // A single value per pixel (gray scale).
          width = ColumnsInBuffer;
//	  bufferrow = RowsInBuffer - 1;
	  bufferrow = 0;

          for (row = RowStart; row < (RowStart + RowsInBuffer); row++)
          {

            TIFFReadScanline(_tif, buf, row);
	    newbuf = (unsigned char *)buf;
	    index = ColumnStart;

  	    for (col = 0;col < width;col++)
            {
	      Buffer[(bufferrow * width) + col] = newbuf[index++];
	    }
//	    bufferrow--;
	    bufferrow++;
          }
      }

  }

  // Strip oriented image
  else if (config == PLANARCONFIG_SEPARATE) 
  {
    _TIFFfree( buf );
    return XP_GIS_ERROR;
  }

  _TIFFfree( buf );

  return XP_GIS_OK;

}



//
//  Method to read the geotiff data that is Untiled.  This method reads the 
//  image based on the arguments with ColumnsInBuffer and RowsInBuffer the 
//  nuber of rows and columns to read. These are short (16 bit) based images.
//
//  The RowStart and ColumnStart are zero based (start at 0)

unsigned long XP_GIS_GTIFF_c::_ReadDataUnTiled(unsigned long RowStart,
				              unsigned long ColumnStart,
					      unsigned long RowsInBuffer,
                                              unsigned long ColumnsInBuffer,
					      short *Buffer)



{

  uint32 height;
  uint16 config;
  uint16 orientation;
  tdata_t buf;
  int row;
  int col;
  int bufferrow;
  int scanlinesize;
  int index;
  short *newbuf;
  uint16 bitspersample;
  uint16 samplesperpixel;
  int width;	


  TIFFGetField(_tif,TIFFTAG_BITSPERSAMPLE,&bitspersample);
  TIFFGetField(_tif,TIFFTAG_SAMPLESPERPIXEL,&samplesperpixel);
  TIFFGetField(_tif, TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(_tif, TIFFTAG_PLANARCONFIG, &config);
  TIFFGetField(_tif, TIFFTAG_ORIENTATION, &orientation);

  // This is the size of a scan line with the number of channels figured in.

  scanlinesize = TIFFScanlineSize(_tif);
  buf = _TIFFmalloc(scanlinesize);

  // Scanline based images

  if (config == PLANARCONFIG_CONTIG) 
  {
      // For the case of 1 value per pixel 16 bits.
      if (bitspersample == 16) 
      {
          // A single value per pixel (gray scale).
          width = ColumnsInBuffer;
//	  bufferrow = RowsInBuffer - 1;
	  bufferrow = 0;

          for (row = RowStart; row < (RowStart + RowsInBuffer); row++)
          {

            TIFFReadScanline(_tif, buf, row);
	    newbuf = (short *)buf;
	    index = ColumnStart;

  	    for (col = 0;col < width;col++)
            {
	      Buffer[(bufferrow * width) + col] = newbuf[index++];
	    }
//	    bufferrow--;
	    bufferrow++;
          }
      }
      else
      {
        _TIFFfree( buf );
        return XP_GIS_ERROR;

      }
  }

  // Strip oriented image
  else if (config == PLANARCONFIG_SEPARATE) 
  {
   _TIFFfree( buf );
   return XP_GIS_ERROR;
  }

  _TIFFfree( buf );

  return XP_GIS_OK;

}

//
//  Method to read the geotiff data that is Untiled.  This method reads the 
//  image based on the arguments with ColumnsInBuffer and RowsInBuffer the 
//  nuber of rows and columns to read. These are float (32 bit) based images.
//
//  The RowStart and ColumnStart are zero based (start at 0)

unsigned long XP_GIS_GTIFF_c::_ReadDataUnTiled(unsigned long RowStart,
				              unsigned long ColumnStart,
					      unsigned long RowsInBuffer,
                                              unsigned long ColumnsInBuffer,
					      float *Buffer)



{

  uint32 height;
  uint16 config;
  uint16 orientation;
  tdata_t buf;
  int row;
  int col;
  int bufferrow;
  int scanlinesize;
  int index;
  float *newbuf;
  uint16 bitspersample;
  uint16 samplesperpixel;
  int width;	


  TIFFGetField(_tif,TIFFTAG_BITSPERSAMPLE,&bitspersample);
  TIFFGetField(_tif,TIFFTAG_SAMPLESPERPIXEL,&samplesperpixel);
  TIFFGetField(_tif, TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(_tif, TIFFTAG_PLANARCONFIG, &config);
  TIFFGetField(_tif, TIFFTAG_ORIENTATION, &orientation);

  // This is the size of a scan line with the number of channels figured in.

  scanlinesize = TIFFScanlineSize(_tif);
  buf = _TIFFmalloc(scanlinesize);

  // Scanline based images

  if (config == PLANARCONFIG_CONTIG) 
  {
      // For the case of 1 value per pixel 32 bits.
      if (bitspersample == 32) 
      {
          // A single value per pixel (gray scale).
          width = ColumnsInBuffer;
//	  bufferrow = RowsInBuffer - 1;
	  bufferrow = 0;

          for (row = RowStart; row < (RowStart + RowsInBuffer); row++)
          {

            TIFFReadScanline(_tif, buf, row);
	    newbuf = (float *)buf;
	    index = ColumnStart;

  	    for (col = 0;col < width;col++)
            {
	      Buffer[(bufferrow * width) + col] = newbuf[index++];
	    }
//	    bufferrow--;
	    bufferrow++;
          }
      }
      else
      {
        _TIFFfree( buf );
        return XP_GIS_ERROR;
      }
  }

  // Strip oriented image
  else if (config == PLANARCONFIG_SEPARATE) 
  {
    _TIFFfree( buf );
    return XP_GIS_ERROR;
  }

  _TIFFfree( buf );

  return XP_GIS_OK;

}

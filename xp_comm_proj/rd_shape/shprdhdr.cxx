
#include "hdr_gen.h"

#include "gsshape.h"
#include "gsesri.h"

int
ReadShape_ReadShapeMods_ReadShapeHeader::ReadShapeHeader(OMevent_mask event_mask, int seq_num)
{
   // ShapeFileName (OMXstr read req notify)
   // ShapeType (OMXint write)
   // NumberOfFeatures (OMXint write)

   XP_GIS_ESRI_Shape_c ShapeFile;         // shape file object
   unsigned long NumberOfDataRecords;     // # of data records in file

   /***********************/
   /* Function's Body     */
   /***********************/
#ifdef DEBUG
   ERRverror("",ERR_NO_HEADER | ERR_PRINT,
             "I'm in method: ReadShapeHeader::ReadShapeHeader\n");
#endif

   //
   //  Tell the shape file object what the shape file name is.
   //

   if (ShapeFile.FileName(ShapeFileName) == NULL)
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error opening shape file.\n");
     return 0;
   }


   //
   //  Print out the header to see what we've got.
   //

#ifdef DEBUG
   if (ShapeFile.ShapeHeader().PrintFileHeader(cout) != XP_GIS_OK)
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error printing shape file header\n");
     return 0;
   }
#endif


   //
   //  Validate the header to make sure we have a good shape file.
   //

   if (ShapeFile.ShapeHeader().ValidateFileHeader() != XP_GIS_OK)
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error validating shape file header\n");
     return 0;
   }

   //
   //  Get the shape type.
   //

   ShapeType = (int) ShapeFile.ShapeHeader().ShapeType();
   if ((ShapeType != (int) XP_GIS_NULL_SHAPE)       &&
       (ShapeType != (int) XP_GIS_POINT_SHAPE)      &&
       (ShapeType != (int) XP_GIS_MULTIPOINT_SHAPE) &&
       (ShapeType != (int) XP_GIS_ARC_SHAPE)        &&
       (ShapeType != (int) XP_GIS_POLYGON_SHAPE))
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "The shape type is not valid.\n");
     return 0;
   }

   //
   //  Get the number of features in the shape file.
   //

   if (ShapeFile.NumberOfDataRecords(&NumberOfDataRecords) != XP_GIS_OK)
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error getting number of data records\n");
     return 0;
   }
   NumberOfFeatures = (int) NumberOfDataRecords;

   // return 1 for success
   return(1);
}


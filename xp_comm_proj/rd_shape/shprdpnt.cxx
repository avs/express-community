#include "pnt_gen.h"

#include "gsshape.h"
#include "gsesri.h"


int
ReadShape_ReadShapeMods_ReadShapePoint::ReadShapePoint(OMevent_mask event_mask, int seq_num)
{
   // ShapeFileName (OMXstr read req notify)
   // FeatureNumber (OMXint read notify)
   // Coordinates (OMXdouble_array write)
   double *Coordinates_arr;

   // FeatureID (OMXint_array write)
   int *FeatureID_arr;

   XP_GIS_ESRI_Shape_c ShapeFile;         // shape file reader object
   unsigned long ReturnValue;             // return from read method
   unsigned long LocalNumberOfPoints;     // local version of express value
   unsigned long LocalFeatureNumber;      // local version of express value
   unsigned long LocalNumberOfFeatures;   // local version of express value
   unsigned long LocalShapeType;          // local version of express value
   unsigned long TotalNumberOfPoints;     // total # of points output
   //unsigned long TotalNumberOfPointsInFeature; // # of features in 1 feature
   unsigned long FeatureStart;            // first feature to process
   unsigned long FeatureEnd;              // last feature to process
   long          NumberOfPointsInFeature; // # of points in 1 feature
   long          RecordNumber;            // record number read
   double       *CoordinatesPtr;          // ptr to coordinate list
   int          *FeatureIDPtr;            // ptr to feature id list

   /***********************/
   /* Function's Body     */
   /***********************/
#ifdef DEBUG
   ERRverror("",ERR_NO_HEADER | ERR_PRINT,
             "I'm in method: ReadShapePoint::ReadShapePoint\n");
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

   LocalShapeType = ShapeFile.ShapeHeader().ShapeType();
   if ((LocalShapeType != XP_GIS_POINT_SHAPE) &&
       (LocalShapeType != XP_GIS_MULTIPOINT_SHAPE))
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "The shape type is not point or multipoint.\n");
     return 0;
   }
   

   //
   //  Get the number of features in the shape file.
   //

   if (ShapeFile.NumberOfDataRecords(&LocalNumberOfFeatures) != XP_GIS_OK)
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error getting number of data records\n");
     return 0;
   }

   //
   //  If the feature number was provided, we want to read only that
   //  feature.  Otherwise, we want to read all the features.  FeatureStart
   //  and FeatureEnd are loop start and end control variables that
   //  tell us which feature(s) to process.
   //

   FeatureStart = 1;
   FeatureEnd = LocalNumberOfFeatures;
   LocalFeatureNumber = (int) FeatureNumber;
   if (FeatureNumber.valid_obj())
   {

     //
     //  Check to see if the feature number requested is valid.
     //

     if ((LocalFeatureNumber <= 0) ||
         (LocalFeatureNumber > LocalNumberOfFeatures))
     {
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                 "The feature number is not valid.\n");
       return 0;
     }
     FeatureStart = (unsigned long) (LocalFeatureNumber);
     FeatureEnd = FeatureStart;

     //
     //  We are only reading one feature.  We need to figure out the
     //  number of parts and points for the requested  feature so we
     //  can allocate the correct amount of space for the arrays.
     //

     LocalNumberOfFeatures = 1;
     if (LocalShapeType == XP_GIS_POINT_SHAPE)
     {
       TotalNumberOfPoints = 1;
     }
     else if (LocalShapeType == XP_GIS_MULTIPOINT_SHAPE)
     {
       if (ShapeFile.SeekToDataRecord((long) LocalFeatureNumber) != XP_GIS_OK)
       {
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error seeking to the feature.\n");
         return 0;
       }

       if (ShapeFile.GetMultiPointShapeInfo(&RecordNumber,
                                   &NumberOfPointsInFeature) != XP_GIS_OK)
       {
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error getting information for feature.\n");
         return 0;
       }
       else if (RecordNumber != LocalFeatureNumber)
       {
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Unexpected record number found.\n");
         return 0;
       }

       TotalNumberOfPoints = NumberOfPointsInFeature;
     }
   }
   else
   {

     //
     //  We are going to read all the features in the file.  We need to
     //  figure out the total number of points for all features
     //  in the file so we can allocate the correct amount of space
     //  for the arrays.
     //

     if (ShapeFile.NumberOfPoints(&TotalNumberOfPoints) != XP_GIS_OK)
     {
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                 "Error getting number of parts in file.\n");
       return 0;
     }
   }
   LocalNumberOfPoints = TotalNumberOfPoints;

   //
   //  Allocate space for the coordinates array.
   //

   Coordinates_arr = (double *) malloc(sizeof(double) * 
                                       (LocalNumberOfPoints * 2));
   if (Coordinates_arr == NULL)
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error allocating space for coordinate array.\n");
     return 0;
   }

   //
   //  Allocate space for the feature id array.
   //

   FeatureID_arr = (int *) malloc(sizeof(int) * LocalNumberOfFeatures);
   if (FeatureID_arr == NULL)
   {
     if (Coordinates_arr   != NULL) free(Coordinates_arr);
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error allocating space for FeatureID array.\n");
     return 0;
   }

   //
   //  Seek to the start of the requested feature.
   //

   if (ShapeFile.SeekToDataRecord((long) FeatureStart) != XP_GIS_OK)
   {
     if (Coordinates_arr   != NULL) free(Coordinates_arr);
     if (FeatureID_arr     != NULL) free(FeatureID_arr);
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error seeking to the feature.\n");
     return 0;
   }

   //
   //  Assign temporary pointers to allow more convenient increment.
   //

   CoordinatesPtr = Coordinates_arr;
   FeatureIDPtr   = FeatureID_arr;

   //
   //  This module handles point feature types.  It handles both
   //  shape points and multipoints.  Switch based on the shape
   //  type.
   //

   switch (LocalShapeType)
   {
     case XP_GIS_POINT_SHAPE:        // point shape type
     {
       double XPoint;         // x point for the feature
       double YPoint;         // y point for the feature

       //
       //  Loop for each feature.
       //

       for (LocalFeatureNumber = FeatureStart; 
            LocalFeatureNumber <= FeatureEnd;
            LocalFeatureNumber++)
       {

         //
         //  Read the feature.
         //

         ReturnValue = ShapeFile.ReadPointShape(&RecordNumber,
                                                &XPoint,
                                                &YPoint);
         if (ReturnValue != XP_GIS_OK)
         {
           if (ReturnValue == XP_GIS_EOF)
           {
             ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                       "Reached unexpected end of file.\n");
             break;
           }
           else
           {
             ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                         "Error reading point feature.\n");
             continue;
           }
         }
         else if (RecordNumber != LocalFeatureNumber)
         {
           ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                     "Unexpected record number found.\n");
           break;
         }
         else
         {
           *CoordinatesPtr++ = XPoint;           // assign the X coordinate
           *CoordinatesPtr++ = YPoint;           // assign the Y coordinate
           *FeatureIDPtr++ = LocalFeatureNumber; // assign the feature #
         }
       }
       break;
     }
     case XP_GIS_MULTIPOINT_SHAPE:    // multipoint shape type
     {
       double        XMin;                    // feature X min
       double        YMin;                    // feature Y min
       double        XMax;                    // feature X max
       double        YMax;                    // feature Y max
       double       *XPoints;                 // array of feature X coords
       double       *YPoints;                 // array of feature Y coords
       unsigned long PointNumber;             // point # we are working with
       long          MaximumNumberOfPoints;   // max # of pts we can handle

       MaximumNumberOfPoints = 0;        // init to 0...will be reassigned
       XPoints               = NULL;     // init to NULL...will be allocated
       YPoints               = NULL;     // init to NULL...will be allocated

       //
       //  For each feature in the file
       //

       for (LocalFeatureNumber = FeatureStart; 
            LocalFeatureNumber < FeatureEnd; 
            LocalFeatureNumber++)
       {

         //
         //  Read the feature.
         //

         ReturnValue = ShapeFile.ReadMultiPointShape(&RecordNumber,
                                                     &XMin,
                                                     &YMin,
                                                     &XMax,
                                                     &YMax,
                                                     &NumberOfPointsInFeature,
                                                     &MaximumNumberOfPoints,
                                                     &XPoints,
                                                     &YPoints);
         if (ReturnValue != XP_GIS_OK)
         {
           if (ReturnValue == XP_GIS_EOF)
           {
             ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                       "Reached unexpected end of file.\n");
             break;
           }
           else
           {
             ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                       "Error reading multipoint feature.\n");
             continue;
           }
         }
         else if (RecordNumber != LocalFeatureNumber)
         {
           ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                     "Unexpected record number found.\n");
           break;
         }
         else
         {

           //
           //  For each point in the feature.
           //

           for (PointNumber = 0;
                PointNumber < NumberOfPointsInFeature; 
                PointNumber++)
           {
             *CoordinatesPtr++ = XPoints[PointNumber]; // assign X coord
             *CoordinatesPtr++ = YPoints[PointNumber]; // assign Y coord
             *FeatureIDPtr++ = LocalFeatureNumber;     // assign feature #
           }
         }
       }

       //
       //  Free the arrays that were allocated by the read method.
       //

       if (XPoints != NULL) free(XPoints);
       if (YPoints != NULL) free(YPoints);
     }
     default:          // Invalid shape type
     {
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                 "The shape type is not point or multipoint.\n");
       return 0;
     }
   }

   //
   //  Now, set the values that belong to express.
   //

   NumberOfPoints = (int) LocalNumberOfPoints;
   NumberOfFeatures = (int) LocalNumberOfFeatures;
   ShapeType = (int) LocalShapeType;

   Coordinates.set_array(OM_TYPE_DOUBLE,
                (char *) Coordinates_arr,
                   (int) (LocalNumberOfPoints * 2),
                         OM_SET_ARRAY_FREE);

   FeatureID.set_array(OM_TYPE_INT,
                (char *) FeatureID_arr,
                   (int) LocalNumberOfFeatures,
                         OM_SET_ARRAY_FREE);


   // return 1 for success
   return(1);
}


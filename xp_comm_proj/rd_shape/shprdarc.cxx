
#include "arc_gen.h"

#include "gsshape.h"
#include "gsesri.h"

int
ReadShape_ReadShapeMods_ReadShapeArc::ReadShapeArc(OMevent_mask event_mask, int seq_num)
{
   // ShapeFileName (OMXstr read req notify)
   // FeatureNumber (OMXint read notify)
   // Coordinates (OMXdouble_array write)
   // ConnectList (OMXint_array write)
   // FeatureID (OMXint_array write)

   XP_GIS_ESRI_Shape_c ShapeFile;         // shape file object
   //unsigned long TempUnsignedLong;      // temporary value
   unsigned long ReturnValue;             // value returned from shape obj
   unsigned long NumberOfPointsSoFar;     // # of points so far
   unsigned long LocalNumberOfPoints;
   unsigned long LocalNumberOfConnections;
   unsigned long LocalNumberOfPolylines;
   unsigned long LocalFeatureNumber;      // feature # we are working with
   unsigned long NumberOfFeatures;
   unsigned long FeatureStart;
   unsigned long FeatureEnd;
   unsigned long TotalNumberOfParts;
   unsigned long TotalNumberOfPoints;
   unsigned long NumberOfDataRecords;
   long          PolylineStart;           // index to start of polylin
   long          PolylineEnd;             // index to end of polyline
   long          RecordNumber;            // record number
   long          NumberOfPartsInFeature;  // # of polylines in this feature
   long          MaximumNumberOfParts;    // max # parts that can be stored
   long         *PartIndex;               // index to part start/end
   long          NumberOfPointsInFeature; // # of points in this feature
   long          MaximumNumberOfPoints;   // max # points that can be stored
   long          PartNumber;              // polyline (part) # 
   long          PointNumber;             // point # 
   int          *ConnectList_arr;
   int          *ConnectListPtr;          // ptr to connection list
   int          *FeatureID_arr;
   int          *FeatureIDPtr;            // ptr to feature ID list
   double       *Coordinates_arr;
   double       *CoordinatesPtr;          // ptr to coordinate list
   double        XMin;                    // feature hull xmin
   double        YMin;                    // feature hull ymin
   double        XMax;                    // feature hull xmax
   double        YMax;                    // feature hull ymax
   double       *XPoints;                 // feature X coordinates
   double       *YPoints;                 // feature Y coordinates

   /***********************/
   /* Function's Body     */
   /***********************/
#ifdef DEBUG
   ERRverror("",ERR_NO_HEADER | ERR_PRINT,
             "I'm in method: ReadShapeArc::ReadShapeArc\n");
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
   if (ShapeType != (int) XP_GIS_ARC_SHAPE)
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "The shape type is not arc.\n");
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
   NumberOfFeatures = (unsigned long) NumberOfDataRecords;

   //
   //  If the feature number was provided, we want to read only that
   //  feature.  Otherwise, we want to read all the features.  FeatureStart
   //  and FeatureEnd are loop start and end control variables that
   //  tell us which feature(s) to process.
   //

   FeatureStart = 1;
   FeatureEnd = NumberOfFeatures;
   LocalFeatureNumber = (int) FeatureNumber;
   if (FeatureNumber.valid_obj())
   {

     //
     //  Check to see if the feature number requested is valid.
     //

     if ((LocalFeatureNumber <= 0) ||
         (LocalFeatureNumber > NumberOfFeatures))
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

     if (ShapeFile.SeekToDataRecord((long) LocalFeatureNumber) != XP_GIS_OK)
     {
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                 "Error seeking to the feature.\n");
       return 0;
     }

     if (ShapeFile.GetArcShapeInfo(&RecordNumber,
                                   &NumberOfPartsInFeature,
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

     TotalNumberOfParts = NumberOfPartsInFeature;
     TotalNumberOfPoints = NumberOfPointsInFeature;
   }
   else
   {

     //
     //  We are going to read all the features in the file.  We need to
     //  figure out the total number of parts and points for all features
     //  in the file so we can allocate the correct amount of space
     //  for the arrays.
     //

     //
     //  Get the total number of parts in all features in the file.
     //
  
     if (ShapeFile.NumberOfParts(&TotalNumberOfParts) != XP_GIS_OK)
     {
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                 "Error getting number of parts in file.\n");
       return 0;
     }

     //
     //  Get the total number of points in all features in the file.
     //
  
     if (ShapeFile.NumberOfPoints(&TotalNumberOfPoints) != XP_GIS_OK)
     {
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                 "Error getting number of parts in file.\n");
       return 0;
     }
   }

   //
   //  Figure out the array sizes.
   //

   LocalNumberOfPoints      = TotalNumberOfPoints;
   LocalNumberOfConnections = TotalNumberOfParts * 2;
   LocalNumberOfPolylines   = TotalNumberOfParts;

   //
   //  Allocate the Coordinates array.
   //

   Coordinates_arr = (double *) malloc(sizeof(double) * LocalNumberOfPoints * 2);
   if (Coordinates_arr == NULL)
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error allocating Coordinates array.\n");
     return 0;
   }

   //
   //  Allocate the ConnectList array.
   //

   ConnectList_arr = (int *) malloc(sizeof(int) * LocalNumberOfConnections);
   if (ConnectList_arr == NULL)
   {
     if (Coordinates_arr != NULL) free(Coordinates_arr);
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error allocating ConnectList array.\n");
     return 0;
   }

   //
   //  Allocate the FeatureID array.
   //

   FeatureID_arr = (int *) malloc(sizeof(int) * LocalNumberOfPolylines);
   if (FeatureID_arr == NULL)
   {
     if (Coordinates_arr   != NULL) free(Coordinates_arr);
     if (ConnectList_arr   != NULL) free(ConnectList_arr);
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error allocating FeatureID array.\n");
     return 0;
   }

   //
   //  Seek to the start of the requested feature.
   //

   if (ShapeFile.SeekToDataRecord((long) FeatureStart) != XP_GIS_OK)
   {
     if (Coordinates_arr   != NULL) free(Coordinates_arr);
     if (ConnectList_arr   != NULL) free(ConnectList_arr);
     if (FeatureID_arr     != NULL) free(FeatureID_arr);
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error seeking to the feature.\n");
     return 0;
   }

   //
   //  Assign pointer values for arrays so we can conveniently increment.
   //

   CoordinatesPtr = Coordinates_arr;
   ConnectListPtr = ConnectList_arr;
   FeatureIDPtr   = FeatureID_arr;

   //
   //  For each feature in the shape file
   //

   NumberOfPointsSoFar = 0;
   MaximumNumberOfParts = 0;
   MaximumNumberOfPoints = 0;
   PartIndex = NULL;
   XPoints   = NULL;
   YPoints   = NULL;

   for (LocalFeatureNumber = FeatureStart; 
        LocalFeatureNumber <= FeatureEnd; 
        LocalFeatureNumber++)
   {

     //
     //  Read the Arc shape.
     //

     ReturnValue = ShapeFile.ReadArcShape(&RecordNumber,
                                          &XMin,
                                          &YMin,
                                          &XMax,
                                          &YMax,
                                          &NumberOfPartsInFeature,
                                          &MaximumNumberOfParts,
                                          &PartIndex,
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
                   "Error reading arc feature.\n");
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
       //  Load the connectivity information for each polyline (part) in
       //  the feature.
       //

       for (PartNumber = 0;
            PartNumber < NumberOfPartsInFeature; 
            PartNumber++)
       {
         PolylineStart = PartIndex[PartNumber];
         PolylineEnd   = (PartNumber < NumberOfPartsInFeature-1) ?
                                     PartIndex[PartNumber+1] - 1:
                                     NumberOfPointsInFeature - 1;
         *ConnectListPtr++ = NumberOfPointsSoFar + PolylineStart;
         *ConnectListPtr++ = NumberOfPointsSoFar + PolylineEnd;
         *FeatureIDPtr++ = LocalFeatureNumber;
       }

       //
       //  Load the all the points that make up the feature.
       //

       for (PointNumber = 0;
            PointNumber < NumberOfPointsInFeature; 
            PointNumber++)
       {
         *CoordinatesPtr++ = XPoints[PointNumber];
         *CoordinatesPtr++ = YPoints[PointNumber];
         NumberOfPointsSoFar++;
       }
     }
   }

   //
   //  Free the arrays we allocated in the read method.
   //

   if (XPoints   != NULL) free(XPoints);
   if (YPoints   != NULL) free(YPoints);
   if (PartIndex != NULL) free(PartIndex);

   //
   //  Now, set the values that belong to express.
   //

   NumberOfPoints = (int) LocalNumberOfPoints;
   NumberOfConnections = (int) LocalNumberOfConnections;
   NumberOfPolylines = (int) LocalNumberOfPolylines;

   Coordinates.set_array(OM_TYPE_DOUBLE,
                (char *) Coordinates_arr,
                   (int) (LocalNumberOfPoints * 2),
                         OM_SET_ARRAY_FREE);

   ConnectList.set_array(OM_TYPE_INT,
                (char *) ConnectList_arr,
                   (int) LocalNumberOfConnections,
                         OM_SET_ARRAY_FREE);

   FeatureID.set_array(OM_TYPE_INT,
                (char *) FeatureID_arr,
                   (int) LocalNumberOfPolylines,
                         OM_SET_ARRAY_FREE);

   // return 1 for success
   return(1);
}



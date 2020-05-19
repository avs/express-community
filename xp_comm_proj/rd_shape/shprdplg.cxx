
#include <stdio.h>

#include "plg_gen.h"


#include <iostream.h>
#include <string.h>


int
ReadShape_ReadShapeMods_ReadShapePolygon::ReadShapePolygon(OMevent_mask event_mask, int seq_num)
{
   // ShapeFileName (OMXstr read req notify)
   // FeatureNumber (OMXint read req notify)
   // TriangulateAllPolygons (OMXint read notify)
   // Coordinates (OMXdouble_array write)
   double *Coordinates_arr;

   // PolyNodes (OMXint_array write)
   int *PolyNodes_arr;

   // ConnectList (OMXint_array write)
   int *ConnectList_arr;

   // ShapeType (OMXint write)
   // FeatureID (OMXint_array write)
   int *FeatureID_arr;

   unsigned long ReturnValue;
   unsigned long LocalShapeType;
   unsigned long NumberOfDataRecords;
   unsigned long NumberOfFeatures;
   unsigned long LocalFeatureNumber;
   unsigned long FeatureStart;
   unsigned long FeatureEnd;
   unsigned long PointNumber;
   unsigned long PolyNodesNumber;
   unsigned long ConnectionNumber;
   unsigned long PolygonNumber;
   unsigned long TotalNumberOfParts;
   unsigned long TotalNumberOfPoints;
   unsigned long AllocatedNumberOfPoints;
   unsigned long AllocatedNumberOfConnections;
   unsigned long AllocatedNumberOfPolyNodes;
   unsigned long AllocatedNumberOfPolygons;
   unsigned long LocalNumberOfPoints;
   unsigned long LocalNumberOfConnections;
   unsigned long LocalNumberOfPolyNodes;
   unsigned long LocalNumberOfPolygons;
   long NumberOfPointsInFeature;
   unsigned long NumberOfConnectionsInFeature;
   unsigned long NumberOfPolyNodesInFeature;
   unsigned long NumberOfPolygonsInFeature;
   unsigned long NumberOfPointsSoFar;
   unsigned long NumberOfPolyNodesSoFar;
   unsigned long NumberOfConnectionsSoFar;
   unsigned long NumberOfPolygonsSoFar;
   //unsigned long PartNumber;
   //unsigned long PartStart;
   //unsigned long PartEnd;
   long RecordNumber;
   long NumberOfParts;
   long MaximumNumberOfParts;
   long MaximumNumberOfPoints;
   long *PartIndex;
   int *ConnectListPtr;
   int *PolyNodesPtr;
   int *FeatureIDPtr;
   double *CoordinatesPtr;
   double *XArray;
   double *YArray;
   double XMin;
   double YMin;
   double XMax;
   double YMax;
   XP_GIS_ESRI_Shape_c   ShapeFile;
   XP_GIS_PolygonTree_c *PolygonTree;

   /***********************/
   /* Function's Body     */
   /***********************/
#ifdef DEBUG
   ERRverror("",ERR_NO_HEADER | ERR_PRINT,
             "I'm in method: ReadShapePolygon::ReadShapePolygon\n");
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
   //  Get the number of features in the shape file.
   //

   if (ShapeFile.NumberOfDataRecords(&NumberOfDataRecords) != XP_GIS_OK)
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error getting number of data records\n");
     return 0;
   }
   NumberOfFeatures = (int) NumberOfDataRecords;

   //
   //  Get the shape type and make sure it is valid.
   //

   LocalShapeType = (int) ShapeFile.ShapeHeader().ShapeType();
   if (LocalShapeType != (int) XP_GIS_POLYGON_SHAPE)
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "The shape type is not polygon.\n");
     return 0;
   }

   //
   //  If the feature number was provided, we want to read only that
   //  feature.  Otherwise, we want to read all features.  FeatureStart
   //  and FeatureEnd are loop start and end control variables that
   //  tell us which feature(s) to process.
   //

   FeatureStart = 1;
   FeatureEnd = NumberOfFeatures;
   if (FeatureNumber.valid_obj())
   {

     //
     //  Check to see if the feature number requested is valid.
     //
  
     if (((int) FeatureNumber <= 0) || 
         ((int) FeatureNumber > NumberOfFeatures))
     {
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                 "The feature number is not valid.\n");
       return 0;
     }
     FeatureStart = (unsigned long) ((int) FeatureNumber);
     FeatureEnd = FeatureStart;

     //
     //  If we are only reading one feature, don't allocate space
     //  for the arrays.  In this case, we need to initialize the
     //  arrays to NULL so they will be reallocated to the appropriate
     //  size later.
     //

     AllocatedNumberOfPoints      = 0;
     AllocatedNumberOfPolyNodes   = 0;
     AllocatedNumberOfConnections = 0;
     AllocatedNumberOfPolygons    = 0;
     Coordinates_arr = NULL;
     PolyNodes_arr   = NULL;
     ConnectList_arr = NULL;
     FeatureID_arr   = NULL;
   }
   else
   {

     //
     //  We are going to read all the features in the file.  To avoid
     //  heap fragmentation, we want to do a pretty good job of 
     //  estimating the amount of space we need for the arrays and allocate
     //  that space up-front.  Then, in the main processing loop, we
     //  can reallocate the arrays as necessary (but hopefully, not
     //  so many times that heap fragmentation becomes a problem).
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

     //
     //  Figure out the array sizes.
     //
  
     AllocatedNumberOfPoints = TotalNumberOfPoints * 2;
     AllocatedNumberOfPolyNodes = (TotalNumberOfPoints - 2) + 
                                  ((TotalNumberOfParts - 1) * 2);
     AllocatedNumberOfConnections = AllocatedNumberOfPolyNodes * 3;
     AllocatedNumberOfPolygons = AllocatedNumberOfPolyNodes;

     //
     //  Allocate the Coordinates array.
     //

     Coordinates_arr = (double *) malloc(
                                  sizeof(double) * AllocatedNumberOfPoints);
     if (Coordinates_arr == NULL)
     {
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                 "Error allocating Coordinates array.\n");
       return 0;
     }

     //
     //  Allocate the PolyNodes array.
     //
  
     PolyNodes_arr = (int *) malloc(
                                  sizeof(int) * AllocatedNumberOfPolyNodes);
     if (PolyNodes_arr == NULL)
     {
       if (Coordinates_arr != NULL) free(Coordinates_arr);
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                 "Error allocating PolyNodes array.\n");
       return 0;
     }

     //
     //  Allocate the ConnectList array.
     //
  
     ConnectList_arr = (int *) malloc(
                                  sizeof(int) * AllocatedNumberOfConnections);
     if (ConnectList_arr == NULL)
     {
       if (Coordinates_arr != NULL) free(Coordinates_arr);
       if (PolyNodes_arr   != NULL) free(PolyNodes_arr);
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                 "Error allocating ConnectList array.\n");
       return 0;
     }

     //
     //  Allocate the FeatureID array.
     //

     FeatureID_arr = (int *) malloc(
                                  sizeof(int) * AllocatedNumberOfPolygons);
     if (FeatureID_arr == NULL)
     {
       if (Coordinates_arr   != NULL) free(Coordinates_arr);
       if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
       if (ConnectList_arr   != NULL) free(ConnectList_arr);
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                 "Error allocating FeatureID array.\n");
       return 0;
     }
   }

   //
   //  Seek to the start of the feature
   //

   if (ShapeFile.SeekToDataRecord((long) FeatureStart) != XP_GIS_OK)
   {
     if (Coordinates_arr   != NULL) free(Coordinates_arr);
     if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
     if (ConnectList_arr   != NULL) free(ConnectList_arr);
     if (FeatureID_arr     != NULL) free(FeatureID_arr);
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error seeking to the feature.\n");
     return 0;
   }

   //
   //  Initialize values
   //

   MaximumNumberOfParts = 0;
   PartIndex = NULL;
   MaximumNumberOfPoints = 0;
   XArray = NULL;
   YArray = NULL;
   LocalNumberOfPoints = 0;
   LocalNumberOfPolyNodes = 0;
   LocalNumberOfConnections = 0;
   LocalNumberOfPolygons = 0;
   NumberOfPointsSoFar = 0;
   NumberOfConnectionsSoFar = 0;
   NumberOfPolyNodesSoFar = 0;
   NumberOfPolygonsSoFar = 0;

   //
   //  Loop through all the features.
   //

   for (LocalFeatureNumber = FeatureStart;
        LocalFeatureNumber <= FeatureEnd;
        LocalFeatureNumber++)
   {

     //
     //  Read the feature
     //

     ReturnValue = ShapeFile.ReadPolygonShape(&RecordNumber,
                                              &XMin,
                                              &YMin,
                                              &XMax,
                                              &YMax,
                                              &NumberOfParts,
                                              &MaximumNumberOfParts,
                                              &PartIndex,
                                              &NumberOfPointsInFeature,
                                              &MaximumNumberOfPoints,
                                              &XArray,
                                              &YArray);
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
                   "error reading the requested feature.\n");
         continue;
       }
     }

     //
     //  Check to see if we need to triangulate this feature.  We will
     //  triangulate it if either the number of parts is greater than
     //  one (meaning we could have nested rings) or if the 
     //  TriangulateAllPolygons flag is set to 1 (true).
     //

     if ((NumberOfParts > 1) || 
         ((int)TriangulateAllPolygons == 1))
     {

       //
       //  Reallocate the Coordinates array to hold the coordinates
       //  for this feature.
       //

       NumberOfPointsSoFar = LocalNumberOfPoints;  
       LocalNumberOfPoints += NumberOfPointsInFeature;
       AllocatedNumberOfPoints = MAX(AllocatedNumberOfPoints,
                                     (LocalNumberOfPoints * 2));
       Coordinates_arr = (double *) realloc(Coordinates_arr,
                                            sizeof(double) *
                                            AllocatedNumberOfPoints);
       if (Coordinates_arr == NULL)
       {
         if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
         if (ConnectList_arr   != NULL) free(ConnectList_arr);
         if (FeatureID_arr     != NULL) free(FeatureID_arr);
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error reallocating Coordinates array.\n");
         return 0;
       }

       //
       //  Copy the X and Y arrays into the Coordinates array.
       //
  
       CoordinatesPtr = &Coordinates_arr[NumberOfPointsSoFar*2];
       for (PointNumber = 0;
            PointNumber < NumberOfPointsInFeature;
            PointNumber++)
       {
         *CoordinatesPtr++ = XArray[PointNumber];
         *CoordinatesPtr++ = YArray[PointNumber];
       }
  
       //
       //  Build the feature tree.
       //
  
       PolygonTree = new XP_GIS_PolygonTree_c(XMin,
                                              YMin,
                                              XMax,
                                              YMax,
                                              NumberOfParts,
                            (unsigned long *) PartIndex,
                                              NumberOfPointsInFeature,
                                              XArray,
                                              YArray);
       if (PolygonTree == NULL)
       {
         if (Coordinates_arr   != NULL) free(Coordinates_arr);
         if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
         if (ConnectList_arr   != NULL) free(ConnectList_arr);
         if (FeatureID_arr     != NULL) free(FeatureID_arr);
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error building ring tree from feature.\n");
         return 0;
       }

       //
       //  Get the number of connections in this feature.
       //
  
       if (PolygonTree->NumberOfConnections(&NumberOfConnectionsInFeature) != 
           XP_GIS_OK)
       {
         if (XArray            != NULL) free(XArray);
         if (YArray            != NULL) free(YArray);
         if (PartIndex         != NULL) free(PartIndex);
         if (Coordinates_arr   != NULL) free(Coordinates_arr);
         if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
         if (ConnectList_arr   != NULL) free(ConnectList_arr);
         if (FeatureID_arr     != NULL) free(FeatureID_arr);
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error getting number of connections in feature.\n");
         return 0;
       }
  
       //
       //  Reallocate the ConnectList array to hold the connections
       //  for this feature.
       //
  
       NumberOfConnectionsSoFar = LocalNumberOfConnections;  
       LocalNumberOfConnections += NumberOfConnectionsInFeature;
       AllocatedNumberOfConnections = MAX(AllocatedNumberOfConnections,
                                          LocalNumberOfConnections);
       ConnectList_arr = (int *) realloc(ConnectList_arr,
                                  sizeof(int) * AllocatedNumberOfConnections);
       if (ConnectList_arr == NULL)
       {
         if (XArray            != NULL) free(XArray);
         if (YArray            != NULL) free(YArray);
         if (PartIndex         != NULL) free(PartIndex);
         if (Coordinates_arr   != NULL) free(Coordinates_arr);
         if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
         if (FeatureID_arr     != NULL) free(FeatureID_arr);
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error reallocating ConnectList array.\n");
         return 0;
       }

       //
       //  Triangulate the polygon tree.  This returns a list of
       //  connections ordered CounterClockwise, like we need.
       //
    
       ConnectListPtr = &ConnectList_arr[NumberOfConnectionsSoFar];
       if (PolygonTree->Triangulate(&NumberOfConnectionsInFeature,
                  (unsigned long *) ConnectListPtr) != XP_GIS_OK)
       {
         if (XArray            != NULL) free(XArray);
         if (YArray            != NULL) free(YArray);
         if (PartIndex         != NULL) free(PartIndex);
         if (Coordinates_arr   != NULL) free(Coordinates_arr);
         if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
         if (ConnectList_arr   != NULL) free(ConnectList_arr);
         if (FeatureID_arr     != NULL) free(FeatureID_arr);
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error triangulating ring tree.\n");
         return 0;
       }

       //
       //  Modify the values returned in ConnectList so they reference
       //  positions in the Coordinates array, rather than in Xarray/Yarray.
       //  Note that we don't need to do this for the first feature.
       //

       if (NumberOfConnectionsSoFar > 0)
       {
         ConnectListPtr = &ConnectList_arr[NumberOfConnectionsSoFar];
         for (ConnectionNumber = 0;
              ConnectionNumber < NumberOfConnectionsInFeature;
              ConnectionNumber++)
         {
           *ConnectListPtr = *ConnectListPtr + NumberOfPointsSoFar;
           ConnectListPtr++;
         }
       }

       //
       //  Allocate the PolyNodes array to hold the # of vertices for
       //  each triangle.  Then set each element in the array to 3.
       //

       NumberOfPolyNodesInFeature = NumberOfConnectionsInFeature / 3; 
       NumberOfPolyNodesSoFar = LocalNumberOfPolyNodes;
       LocalNumberOfPolyNodes += NumberOfPolyNodesInFeature;
       AllocatedNumberOfPolyNodes = MAX(AllocatedNumberOfPolyNodes,
                                        LocalNumberOfPolyNodes);
       PolyNodes_arr = (int *) realloc(PolyNodes_arr,
                                    sizeof(int) * AllocatedNumberOfPolyNodes);
       if (PolyNodes_arr == NULL)
       {
         if (XArray            != NULL) free(XArray);
         if (YArray            != NULL) free(YArray);
         if (PartIndex         != NULL) free(PartIndex);
         if (Coordinates_arr   != NULL) free(Coordinates_arr);
         if (ConnectList_arr   != NULL) free(ConnectList_arr);
         if (FeatureID_arr     != NULL) free(FeatureID_arr);
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error allocating PolyNodes array.\n");
         return 0;
       }
  
       PolyNodesPtr = &PolyNodes_arr[NumberOfPolyNodesSoFar];
       for (PolyNodesNumber = 0;
            PolyNodesNumber < NumberOfPolyNodesInFeature;
            PolyNodesNumber++)
       {
         *PolyNodesPtr++ = 3;
       }
  
       //
       //  Reallocate the FeatureID array to hold the feature ids for
       //  each triangle in this feature.  Then populate the appropriate
       //  portion of the list with the feature id of this feature.
       //
  
       NumberOfPolygonsInFeature = NumberOfConnectionsInFeature / 3; 
       NumberOfPolygonsSoFar = LocalNumberOfPolygons;
       LocalNumberOfPolygons += NumberOfPolygonsInFeature;
       AllocatedNumberOfPolygons = MAX(AllocatedNumberOfPolygons,
                                      LocalNumberOfPolygons);
       FeatureID_arr = (int *) realloc(FeatureID_arr,
                                    sizeof(int) * AllocatedNumberOfPolygons);
       if (FeatureID_arr == NULL)
       {
         if (XArray            != NULL) free(XArray);
         if (YArray            != NULL) free(YArray);
         if (PartIndex         != NULL) free(PartIndex);
         if (Coordinates_arr   != NULL) free(Coordinates_arr);
         if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
         if (ConnectList_arr   != NULL) free(ConnectList_arr);
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error reallocating FeatureID array.\n");
         return 0;
       }

       FeatureIDPtr = &FeatureID_arr[NumberOfPolygonsSoFar];
       for (PolygonNumber = 0;
            PolygonNumber < NumberOfPolygonsInFeature;
            PolygonNumber++)
       {
         *FeatureIDPtr++ = LocalFeatureNumber;
       }

       //
       //  Delete the polygon tree for this feature.
       //

       delete PolygonTree;
     }
     else              // handle single-part polygons...don't triangulate
     {

       //
       //  If the polygon is closed, open it.
       //

       if ((XArray[0] == XArray[NumberOfPointsInFeature-1]) &&
           (YArray[0] == YArray[NumberOfPointsInFeature-1]))
       {
         NumberOfPointsInFeature--;
       }

       //
       //  Reallocate the Coordinates array to hold the coordinates
       //  for this feature.
       //

       NumberOfPointsSoFar = LocalNumberOfPoints;  
       LocalNumberOfPoints += NumberOfPointsInFeature;
       AllocatedNumberOfPoints = MAX(AllocatedNumberOfPoints,
                                     (LocalNumberOfPoints * 2));
       Coordinates_arr = (double *) realloc(Coordinates_arr,
                                            sizeof(double) *
                                            AllocatedNumberOfPoints);
       if (Coordinates_arr == NULL)
       {
         if (XArray            != NULL) free(XArray);
         if (YArray            != NULL) free(YArray);
         if (PartIndex         != NULL) free(PartIndex);
         if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
         if (ConnectList_arr   != NULL) free(ConnectList_arr);
         if (FeatureID_arr     != NULL) free(FeatureID_arr);
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error reallocating Coordinates array.\n");
         return 0;
       }

       //
       //  Copy the X and Y arrays into the Coordinates array.
       //

       CoordinatesPtr = &Coordinates_arr[NumberOfPointsSoFar*2];
       for (PointNumber = 0;
            PointNumber < NumberOfPointsInFeature;
            PointNumber++)
       {
         *CoordinatesPtr++ = XArray[PointNumber];
         *CoordinatesPtr++ = YArray[PointNumber];
       }
  
       //
       //  Reallocate the ConnectList array to hold the connections
       //  for this feature.
       //

       NumberOfConnectionsInFeature = NumberOfPointsInFeature;
       NumberOfConnectionsSoFar = LocalNumberOfConnections;  
       LocalNumberOfConnections += NumberOfConnectionsInFeature;
       AllocatedNumberOfConnections = MAX(AllocatedNumberOfConnections,
                                          LocalNumberOfConnections);
       ConnectList_arr = (int *) realloc(ConnectList_arr,
                                  sizeof(int) * AllocatedNumberOfConnections);
       if (ConnectList_arr == NULL)
       {
         if (XArray            != NULL) free(XArray);
         if (YArray            != NULL) free(YArray);
         if (PartIndex         != NULL) free(PartIndex);
         if (Coordinates_arr   != NULL) free(Coordinates_arr);
         if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
         if (FeatureID_arr     != NULL) free(FeatureID_arr);
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error reallocating ConnectList array.\n");
         return 0;
       }

       //
       //  Populate the ConnectList array.  The polygons are clockwise,
       //  but we need them to be counter clockwise.  So reverse their
       //  order as we populate the ConnectList array.
       //

       ConnectListPtr = &ConnectList_arr[NumberOfConnectionsSoFar];
       for (ConnectionNumber = 0;
            ConnectionNumber < NumberOfConnectionsInFeature;
            ConnectionNumber++)
       {
         *ConnectListPtr++ = NumberOfPointsSoFar + 
                             (NumberOfConnectionsInFeature - 1) -
                             ConnectionNumber;
       }

       //
       //  Reallocate the PolyNodes array to hold the # of vertices for
       //  each polygon.  Then set each element in the array to the
       //  number of connections in the polygon.
       //

       NumberOfPolyNodesInFeature  = 1;
       NumberOfPolyNodesSoFar = LocalNumberOfPolyNodes;
       LocalNumberOfPolyNodes += NumberOfPolyNodesInFeature;
       AllocatedNumberOfPolyNodes = MAX(AllocatedNumberOfPolyNodes,
                                        LocalNumberOfPolyNodes);
       PolyNodes_arr = (int *) realloc(PolyNodes_arr,
                                    sizeof(int) * AllocatedNumberOfPolyNodes);
       if (PolyNodes_arr == NULL)
       {
         if (XArray            != NULL) free(XArray);
         if (YArray            != NULL) free(YArray);
         if (PartIndex         != NULL) free(PartIndex);
         if (Coordinates_arr   != NULL) free(Coordinates_arr);
         if (ConnectList_arr   != NULL) free(ConnectList_arr);
         if (FeatureID_arr     != NULL) free(FeatureID_arr);
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error allocating PolyNodes array.\n");
         return 0;
       }
    
       PolyNodes_arr[NumberOfPolyNodesSoFar] = NumberOfPointsInFeature;
  
       //
       //  Reallocate the FeatureID array to hold the feature ids for
       //  each triangle in this feature.  Then populate the appropriate
       //  portion of the list with the feature id of this feature.
       //

       NumberOfPolygonsInFeature = 1;
       NumberOfPolygonsSoFar = LocalNumberOfPolygons;
       LocalNumberOfPolygons += NumberOfPolygonsInFeature;
       AllocatedNumberOfPolygons = MAX(AllocatedNumberOfPolygons,
                                        LocalNumberOfPolygons);

       FeatureID_arr = (int *) realloc(FeatureID_arr,
                                    sizeof(int) * AllocatedNumberOfPolygons);
       if (FeatureID_arr == NULL)
       {
         if (XArray            != NULL) free(XArray);
         if (YArray            != NULL) free(YArray);
         if (PartIndex         != NULL) free(PartIndex);
         if (Coordinates_arr   != NULL) free(Coordinates_arr);
         if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
         if (ConnectList_arr   != NULL) free(ConnectList_arr);
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                   "Error reallocating FeatureID array.\n");
         return 0;
       }

       FeatureID_arr[NumberOfPolygonsSoFar] = LocalFeatureNumber;
     }
   }

   //
   //  Free the working arrays allocated in the loop.
   //

   if (XArray    != NULL) free(XArray);
   if (YArray    != NULL) free(YArray);
   if (PartIndex != NULL) free(PartIndex);

   //
   //  Now, reallocate the arrays to just the size needed.
   //

   Coordinates_arr = (double *) realloc(Coordinates_arr,
                                   sizeof(double) * (LocalNumberOfPoints * 2));
   if (Coordinates_arr == NULL)
   {
     if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
     if (ConnectList_arr   != NULL) free(ConnectList_arr);
     if (FeatureID_arr     != NULL) free(FeatureID_arr);
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error reallocating Coordinates array.\n");
     return 0;
   }

   PolyNodes_arr = (int *) realloc(PolyNodes_arr,
                                     sizeof(int) * LocalNumberOfPolyNodes);
   if (PolyNodes_arr == NULL)
   {
     if (Coordinates_arr   != NULL) free(Coordinates_arr);
     if (ConnectList_arr   != NULL) free(ConnectList_arr);
     if (FeatureID_arr     != NULL) free(FeatureID_arr);
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error reallocating PolyNodes array.\n");
     return 0;
   }

   ConnectList_arr = (int *) realloc(ConnectList_arr,
                                     sizeof(int) * LocalNumberOfConnections);
   if (ConnectList_arr == NULL)
   {
     if (Coordinates_arr   != NULL) free(Coordinates_arr);
     if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
     if (FeatureID_arr     != NULL) free(FeatureID_arr);
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error reallocating ConnectList array.\n");
     return 0;
   }

   FeatureID_arr = (int *) realloc(FeatureID_arr,
                                     sizeof(int) * LocalNumberOfPolygons);
   if (FeatureID_arr == NULL)
   {
     if (Coordinates_arr   != NULL) free(Coordinates_arr);
     if (PolyNodes_arr     != NULL) free(PolyNodes_arr);
     if (ConnectList_arr   != NULL) free(ConnectList_arr);
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error reallocating FeatureID array.\n");
     return 0;
   }


   //
   //  Now, set the values that belong to express.
   //

   NumberOfPoints = (int) LocalNumberOfPoints;
   NumberOfPolyNodes = (int) LocalNumberOfPolyNodes;
   NumberOfConnections = (int) LocalNumberOfConnections;
   NumberOfPolygons = (int) LocalNumberOfPolygons;
   ShapeType = (int) LocalShapeType;
   

   Coordinates.set_array(OM_TYPE_DOUBLE,
                  (char*)Coordinates_arr,
                   (int) (LocalNumberOfPoints * 2),
                         OM_SET_ARRAY_FREE);

   PolyNodes.set_array(OM_TYPE_INT,
                (char*)PolyNodes_arr,
                 (int) LocalNumberOfPolyNodes,
                       OM_SET_ARRAY_FREE);

   ConnectList.set_array(OM_TYPE_INT,
                  (char*)ConnectList_arr,
                   (int) LocalNumberOfConnections,
                         OM_SET_ARRAY_FREE);

   FeatureID.set_array(OM_TYPE_INT,
                (char*)FeatureID_arr,
                 (int) LocalNumberOfPolygons,
                       OM_SET_ARRAY_FREE);

/*
   int k1;
   printf("-----------------------------------\n");
   printf("There are %d Points\n",LocalNumberOfPoints);
   printf("There are %d PolyNodes\n",LocalNumberOfPolyNodes);
   printf("There are %d Connections\n",LocalNumberOfConnections);
   printf("There are %d Polygons\n",LocalNumberOfPolygons);
   printf("Performing normal return.\n");

   printf("  There are %d coordinate pairs.\n",LocalNumberOfPoints);
   for (k1 = 0; k1 < LocalNumberOfPoints * 2; k1+=2)
   {
     printf("  X, Y[%d] = %f %f\n",
            k1/2,
            Coordinates_arr[k1],
            Coordinates_arr[k1+1]);
   }

   printf("  There are %d polynodes.\n",LocalNumberOfPolyNodes);
   for (k1 = 0; k1 < LocalNumberOfPolyNodes; k1++)
   {
     printf("  PolyNodes[%d] = %d\n",k1,PolyNodes_arr[k1]);
   }

   printf("  There are %d connections.\n",LocalNumberOfConnections);
   for (k1 = 0; k1 < LocalNumberOfConnections; k1++)
   {
     printf("  ConnectList[%d] = %d\n",k1,ConnectList_arr[k1]);
   }

   printf("  There are %d triangles.\n",LocalNumberOfPolygons);
   for (k1 = 0; k1 < LocalNumberOfPolygons; k1++)
   {
     printf("  FeatureID[%d] = %d\n",k1,FeatureID_arr[k1]);
   }
   fflush(stdout);
*/
  

   return 1;
}




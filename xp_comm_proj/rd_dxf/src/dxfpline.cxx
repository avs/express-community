/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

Implementation of the DXFPolyline and DXFVertex classes, which are used to
represent DXF polyline objects.

******************************************************************************
*****************************************************************************/
#include "dxfpline.hxx"
#include "dxfprim.hxx"
#include "blistcpp.hxx"
#include "blistcpp.cxx"   //## AWD: Added to resolve templates problem
#include "dxfdefine.hxx"
#include <math.h>

/*********************  Macros and Manifest Constants  **********************/

#define DXF_POLYLINE_CLOSED_M            0x0001   // "flags" masks
#define DXF_POLYLINE_CURVE_VERTS_ADDED   0x0002
#define DXF_POLYLINE_SPLINE_VERTS_ADDED  0x0004
#define DXF_POLYLINE_3D_POLYLINE         0x0008
#define DXF_POLYLINE_3D_POLYGON_MESH     0x0010
#define DXF_POLYLINE_CLOSED_N            0x0020
#define DXF_POLYLINE_POLYFACE_MESH       0x0040
#define DXF_POLYLINE_LINE_PATTERN        0x0080


/************************  Public Global Variables  *************************/

/* <none> */


/************************  Private Type Definitions  ************************/

/* <none> */


/*******************  Private (Static) Global Variables  ********************/

/* <none> */



/*****************************************************************************
*********--------   Implementations of "DXFVertex" Methods   --------*********
*****************************************************************************/


/*****************************************************************************
Method:   DXFVertex::Make()
Purpose:  This is the static "virtual constructor" for the DXFVertex class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a vertex,
	  then an instance of DXFVertex is created, and its attributes
	  are read from the DXF file (via the "group" object).
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
		  firstLayer   start of the linked list fo LAYER information. This 
		  will be used to assign color if appropriate
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
Returns:  A pointer to a DXFVertex object if the current object was
          recognized as a DXF vertex, or
	  NULL if it wasn't.
*****************************************************************************/
DXFVertex *DXFVertex::Make(DXFGroup &group, CLIST <DXFLayer>  *firstLayer)
{
   DXFVertex *instance = 0;

   if (group.DataCmp("VERTEX"))
   {
      instance = new DXFVertex(group, firstLayer);

      if (instance)
      {
	 if (!instance->IsValid())
	 {
	    delete instance;
	    instance = 0;
	 }
      }
   }

   return instance;
}


/*****************************************************************************
Method:   DXFVertex::DXFVertex()
Purpose:  A Constructor for the DXFVertex class.  Reads the coordinates
          and any other attributes from the DXF file.
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
		  firstLayer   start of the linked list fo LAYER information. This 
		  will be used to assign color if appropriate
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
*****************************************************************************/
DXFVertex::DXFVertex(DXFGroup &group, CLIST <DXFLayer> *firstLayer) :
                     point(), faceVertexIndex(0), next(0)
{
   isValid = 1;
   startingWidth = 0.0;
   endingWidth = 0.0;
   bulge = 0.0;
   curveFitTangentAngle = 0.0;
   flags = 0;
   hasData = 0;

   isValid = 1;

   while (group.Read() && isValid)
   {
      if (group.Code() == 0)   // stop at next entity definition
	 break;

      switch (group.Code())
      {
	case 10:   // x coordinate
	 point.SetX(group.DataAsFloat());
	 break;

	case 20:   // y coordinate
	 point.SetY(group.DataAsFloat());
	 break;

	case 30:   // z coordinate
	 point.SetZ(group.DataAsFloat());
	 break;

	case 40:   // optional starting width
	 startingWidth = group.DataAsDouble();
	 break;

	case 41:   // optional ending width
	 endingWidth = group.DataAsDouble();
	 break;

	case 42:   // optional bulge
	 bulge = group.DataAsDouble();
	 break;

	case 50:   // optional curve fit tangent direction
	 curveFitTangentAngle = group.DataAsDouble();
	 break;

	case 70:   // optional vertex flags
	 flags = group.DataAsUnsigned();
	 break;

	case 71:   // optional vertex face indexes
	case 72:
	case 73:
	case 74:
	 if (!faceVertexIndex)
	 {
	    faceVertexIndex = new int[4];

	    if (faceVertexIndex)
	       for (int i = 0; i < 4; i++)
		  faceVertexIndex[i] = 0;
	    else
	       isValid = 0;
	 }

	 if (isValid)
	    faceVertexIndex[group.Code() - 71] = group.DataAsInt();
	 break;
    case 39:   //something we presume is height data
		hasData = 1;
		data = group.DataAsDouble();
		break;
	default:   // ignore (and read past) all other group codes
        //ReadCommon(group, firstLayer);
		break;
      }
   }
}


/*****************************************************************************
Method:   DXFVertex::DXFVertex()
Purpose:  Copy constructor for the DXFVertex class.
Params:   v   A reference to the DXFVertex object to copy.
State:    ---
*****************************************************************************/
DXFVertex::DXFVertex(const DXFVertex &v) : point(v.point), faceVertexIndex(0)
{
   isValid = 1;
   startingWidth = v.startingWidth;
   endingWidth = v.endingWidth;
   bulge = v.bulge;
   curveFitTangentAngle = v.curveFitTangentAngle;
   flags = v.flags;
   hasData = v.hasData;
   if(hasData)
	   data = v.data;
   if (v.faceVertexIndex)
   {
      faceVertexIndex = new int[4];

      if (faceVertexIndex)
      {
	 for (int i = 0; i < 4; i++)
	    faceVertexIndex[i] = v.faceVertexIndex[i];
      }
      else
	 isValid = 0;
   }
}




void DXFVertex::Dump()
{
   float x, y, z;

   point.Get(&x, &y, &z);

//   printf("      Vertex\n");
//   printf("         Point = (%f, %f, %f)\n", x, y, z);
//   printf("         startingWidth = %f\n", startingWidth);
//   printf("         endingWidth = %f\n", endingWidth);
//   printf("         bulge = %f\n", bulge);
//   printf("         curveFitTangentAngle = %f\n", curveFitTangentAngle);
//   printf("         flags = %04X\n", flags);

//   if (faceVertexIndex)
//      printf("         faceVertexIndexes = { %d, %d, %d, %d }\n",
//	     faceVertexIndex[0], faceVertexIndex[1],
//	     faceVertexIndex[2], faceVertexIndex[3]);
}



/*****************************************************************************
********--------   Implementations of "DXFPolyline" Methods   --------********
*****************************************************************************/




/*****************************************************************************
Method:   DXFPolyline::Make()
Purpose:  This is the static "virtual constructor" for the DXFPolyline class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a polyline,
	  then an instance of DXFPolyline is created, and its attributes
	  are read from the DXF file (via the "group" object).
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
		  firstLayer   start of the linked list fo LAYER information. This 
		  will be used to assign color if appropriate
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
Returns:  A pointer to a DXFPolyline object (cast to DXFPrim *) if the
          current object was recognized as a DXF polyline, or
	  NULL if it wasn't.
*****************************************************************************/
DXFPrim *DXFPolyline::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("POLYLINE"))
      instance = new DXFPolyline(group, firstLayer);
   return instance;
}


/*****************************************************************************
Method:   DXFPolyline::DXFPolyline()
Purpose:  A Constructor for the DXFPolyline class.  Reads the flags and all
          other data, including all the vertices, from the DXF file.
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
		  firstLayer   start of the linked list fo LAYER information. This 
		  will be used to assign color if appropriate
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
*****************************************************************************/
DXFPolyline::DXFPolyline(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   flags = 0;
   elevation = 0.0;
   mVertices = 0;
   nVertices = 0;
   vertexCount = 0;
   mSmooth = 0;
   nSmooth = 0;
   curves = 0;
   verticesHead = 0;
   verticesTail = 0;
   hasData = 0;
   isValid = 0;
   extrusion.SetX(0);
   extrusion.SetY(0);
   extrusion.SetZ(1);
  

   
   
   printf("DXFPolyline::DXFPolyline colour0: %i\n",color);
   
   while (group.Read())
   {
      if (group.Code() == 0)   // stop at next entity definition
	 break;

      switch (group.Code())
      {
	case 10:   // x coordinate (always 0; skip)
	case 20:   // y coordinate (always 0; skip)
	case 66:   // "Vertices follow" flag-- always 1; skip
	 break;

	case 30:   // z coordinate (elevation)
	 elevation = group.DataAsDouble();
	 break;

	case 70:   // Polyline flags
	 flags = group.DataAsUnsigned();
	 break;

	case 71:   // Polygon mesh M vertex count
	 mVertices = group.DataAsInt();
	 break;

	case 72:   // Polygon mesh N vertex count
	 nVertices = group.DataAsInt();
	 break;

	case 73:   // Smooth surface M density
	 mSmooth = group.DataAsInt();
	 break;

	case 74:   // Smooth surface N density
	 nSmooth = group.DataAsInt();
	 break;

	case 75:   // curves and smooth surface type
	 curves = group.DataAsInt();
	 break;


        case 39:   //something we presume is height data
	 hasData = 1;
	 data = group.DataAsDouble();
	 break;

        case 210:
	 extrusion.SetX(group.DataAsFloat());
	 break;

        case 220:
	 extrusion.SetY(group.DataAsFloat());
	 break;

	case 230:
	 extrusion.SetZ(group.DataAsFloat());
	 break;


    default:
	 DXFPrim::ReadCommon(group, firstLayer);  // see if the base class wants it
	 break;
      }
   }

   // At this point all data associated with the polyline object itself
   // has been read (or EOF encountered).  Now we need to read all the
   // vertices that follow this object in the DXF file.
   //
   do
      verticesHead = verticesTail = DXFVertex::Make(group, firstLayer);
   while (verticesHead &&  (verticesHead->flags & 0x0010));    //overread frame control point

   if (verticesHead)
   {

      DXFVertex *vertex1=(DXFVertex *)malloc(sizeof(DXFVertex));
      *vertex1=*verticesTail;
      vertex1->next=NULL;
      DXFVertex *vertex = DXFVertex::Make(group, firstLayer);
      printf("DXFPolyline %f %f %f\n",vertex1->point.GetX(),vertex1->point.GetY(),vertex1->point.GetZ());
      //vertex1=*vertex;

      vertexCount = 1;

      while (vertex)
      {
         if(!(vertex->flags & 0x0010))		//overread spline vertex
	 {
	   verticesTail->next = vertex;
	   verticesTail = vertex;

	   ++vertexCount;
         }

	 vertex = DXFVertex::Make(group, firstLayer);

      }
      if(flags & DXF_POLYLINE_CLOSED_M)
      {
          //vertex=&vertex1;
	  printf("DXFPolyline %f %f %f\n",vertex1->point.GetX(),vertex1->point.GetY(),vertex1->point.GetZ());
	  verticesTail->next = vertex1;
	  verticesTail = vertex1;

	  ++vertexCount;
      }

   }

   // At this point, we have encountered an entity definition in the
   // DXF file that is not a vertex.  Unless it is a SEQEND entity, there
   // is something wrong with the file.
   //
   if (group.DataCmp("SEQEND"))
   {
      // Now read to the end of this SEQEND object, to leave the file
      // pointer in the proper place.
      //
      while (group.Read())
	    if (group.Code() == 0)
	      break;

      // Now that we have all the vertices associated with this polyline,
      // we need to establish the connectivity.
      //
      if (DefineCellSets(group.ReverseNormals()))
	     isValid = 1;
	  if(hasData)
		  SetCellData(data);
   }
   else
      errorDesc = "Missing SEQEND after polyline vertex list";
      
  // if(GetNumPoints()<2)isValid = 0;
} 


/*****************************************************************************
Method:   DXFPolyline::~DXFPolyline()
Purpose:  Destructor for the DXFPolyline class.  Destroys the vertex and
          cell set lists.
Params:   ---
State:    ---
*****************************************************************************/
DXFPolyline::~DXFPolyline()
{
   DXFVertex *vert = verticesHead;
   DXFVertex *doomedVert;

   printf("destructor DXFPolyline\n");

   while (vert)
   {
      doomedVert = vert;

      vert = vert->next;
      delete doomedVert;
   }

   printf("destructor DXFPolyline end\n");
}


/*****************************************************************************
Method:   DXFPolyline::DefineCellSets()
Purpose:  Determines the connectivity of this polyline and creates whatever
          cell sets are needed to represent that connectivity.
Params:   ---
State:    The cell sets are added to the linked list of DXFCellSet objects
          defined in the DXFPrim base class.
Returns:  True   if the cell sets were successfully added,
          False  if they weren't.
*****************************************************************************/
char DXFPolyline::DefineCellSets(char reverseNormals)
{
   char ok = 0;
   if (vertexCount<2) return 0;
   if (flags & DXF_POLYLINE_3D_POLYGON_MESH)
      ok = DefineMeshCellSet(reverseNormals);
   else if (flags & DXF_POLYLINE_POLYFACE_MESH)
      ok = DefinePolyfaceMeshCellSet(reverseNormals);
   else   // A simple polyline or 3D polyline
      ok = DefinePolylineCellSet(reverseNormals);

   return ok;
}


/*****************************************************************************
Method:   DXFPolyline::DefineMeshCellSet()
Purpose:  Defines a cell set for this polyline assuming it is a 3D mesh.
          Quad cells are used.
Params:   ---
State:    This function should be used if the DXF_POLYLINE_3D_POLYGON_MESH
          bit is set in the flags word.
Returns:  True   if the cell set was successfully added,
          False  if it wasn't.
*****************************************************************************/
char DXFPolyline::DefineMeshCellSet(char reverseNormals)
{
   char ok = 0;
   if (vertexCount<2) return 0;
   if (flags & DXF_POLYLINE_SPLINE_VERTS_ADDED)
   {
     mVertices= mSmooth;
     nVertices= nSmooth;
   }

   int m = (flags & DXF_POLYLINE_CLOSED_M) ? mVertices : mVertices - 1;
   int n = (flags & DXF_POLYLINE_CLOSED_N) ? nVertices : nVertices - 1;
   


   // Apparently, the vertices of meshes in the DXF file should be
   // traversed in a clockwise direction to get the surface normals
   // pointing in the right direction.
   //
   if (AddCellSet(4, m*n, DXF_CELL_FACE))
   {
      int r, c;
      int r1, c1;

      for (r = 0; r < m; r++)
      {
	 r1 = (r + 1) % mVertices;

	 for (c = 0; c < n; c++)
	 {
	    c1 = (c + 1) % nVertices;

	    if (reverseNormals)
	    {
	       cellSetsTail->AddVertexIndex((r*nVertices) + c);
	       cellSetsTail->AddVertexIndex((r1*nVertices) + c);
	       cellSetsTail->AddVertexIndex((r1*nVertices) + c1);
	       cellSetsTail->AddVertexIndex((r*nVertices) + c1);
	    }
	    else
	    {
	       cellSetsTail->AddVertexIndex((r*nVertices) + c);
	       cellSetsTail->AddVertexIndex((r*nVertices) + c1);
	       cellSetsTail->AddVertexIndex((r1*nVertices) + c1);
	       cellSetsTail->AddVertexIndex((r1*nVertices) + c);
	    }
	 }
      }

      ok = 1;
   }

   return ok;
}


/*****************************************************************************
Method:   DXFPolyline::DefinePolyfaceMeshCellSet()
Purpose:  Defines a cell set for this polyline assuming it is a polyface
          mesh.  Quad cells are used.
Params:   ---
State:    This function should be used if the DXF_POLYLINE_POLYFACE_MESH
          bit is set in the flags word.
Returns:  True   if the cell set was successfully added,
          False  if it wasn't.
*****************************************************************************/
char DXFPolyline::DefinePolyfaceMeshCellSet(char reverseNormals)
{
   char ok = 0;
   int m = (flags & DXF_POLYLINE_CLOSED_M) ? mVertices : mVertices - 1;
   int n = (flags & DXF_POLYLINE_CLOSED_N) ? nVertices : nVertices - 1;
   if (vertexCount<2) return 0;
   // Apparently, the vertices of meshes in the DXF file should be
   // traversed in a clockwise direction to get the surface normals
   // pointing in the right direction.
   //
   if (AddCellSet(4, m*n, DXF_CELL_FACE))
   {
      int r, c;
      int r1, c1;

      for (r = 0; r < m; r++)
      {
	 r1 = (r + 1) % mVertices;

	 for (c = 0; c < n; c++)
	 {
	    c1 = (c + 1) % nVertices;

	    if (reverseNormals)
	    {
	       cellSetsTail->AddVertexIndex((r*nVertices) + c);
	       cellSetsTail->AddVertexIndex((r1*nVertices) + c);
	       cellSetsTail->AddVertexIndex((r1*nVertices) + c1);
	       cellSetsTail->AddVertexIndex((r*nVertices) + c1);
	    }
	    else
	    {
	       cellSetsTail->AddVertexIndex((r*nVertices) + c);
	       cellSetsTail->AddVertexIndex((r*nVertices) + c1);
	       cellSetsTail->AddVertexIndex((r1*nVertices) + c1);
	       cellSetsTail->AddVertexIndex((r1*nVertices) + c);
	    }
	 }
      }

      ok = 1;
   }

   return ok;
}


/*****************************************************************************
Method:   DXFPolyline::DefinePolylineCellSet()
Purpose:  Defines a cell set for this polyline assuming it is a simple
          polyline or 3D polyline.  Line cells are used.
Params:   ---
State:    This function should be used if neither of the flag bits
          DXF_POLYLINE_3D_POLYGON_MESH or DXF_POLYLINE_3D_POLYGON_MESH
	  are set in the flags word.
Returns:  True   if the cell set was successfully added,
          False  if it wasn't.
*****************************************************************************/
char DXFPolyline::DefinePolylineCellSet(char reverseNormals)
{
   char ok = 0;
   int m = vertexCount;
   int q;
   if(vertexCount<2) return 0;
   if (flags & DXF_POLYLINE_CLOSED_M)   // if a closed polygon
   {
      if (AddCellSet(m, 1, DXF_CELL_CHOOSE, DXF_CLOSED_POLYGON))
      {
	 printf("DefinePolylineCellSet DXF_CLOSED_POLYGON\n");
	 if (reverseNormals)
	    for (q = m - 1; q >= 0; q--)
	       cellSetsTail->AddVertexIndex(q);
	 else
	    for (q = 0; q < m; q++)
	       cellSetsTail->AddVertexIndex(q);

	 ok = 1;
      }
   }
   else   // it's a non-closed polyline
   {
      printf("DefinePolylineCellSet non-closed polyline\n");
      if (AddCellSet(2, --m, DXF_CELL_LINE))
      {
	 for (q = 0; q < m; q++)
	 {
	    cellSetsTail->AddVertexIndex(q);
	    cellSetsTail->AddVertexIndex((q + 1) % vertexCount);
	 }

	 ok = 1;
      }
   }

   return ok;
}


/*****************************************************************************
Method:   DXFPolyline::GetPoints()
Purpose:  Loads the points (vertices) associated with this polyline into
          the specified array.
Params:   array   An array of doubles, which we assume is big enough to hold
                  all the points.
		  Note that this is a one-dimensional array.  The x, y, and
		  z values are stored sequentially, so that the coordinates
		  associated with a point whose index is I would be at
		  [3*I], [3*I + 1], and [3*I + 2].
	  offset  An optional offset into the array at which to start
                  placing the coordinates.
State:    ---
Returns:  The number of *NUMBERS* (i.e. 3 times the number of vertices)
          placed in the array PLUS the starting offset, if any.
*****************************************************************************/
int DXFPolyline::GetPoints(float *array, int offset)
{
   int i = offset;
   DXFVertex *vert = verticesHead;
   DXFPoint3D Ax,Ay;

   float c=(extrusion.GetX()<0) ? (extrusion.GetX()*(-1)) : extrusion.GetX();
   float d=(extrusion.GetY()<0) ? (extrusion.GetY()*(-1)) : extrusion.GetY();
   float e=1.0/64.0;
   if((c<e) && (d<e))
	{
	//printf("DXFPolyline::GetPoints smal x and smal y values\n");
	Ax.SetX(extrusion.GetZ());
	Ax.SetY(0);
	Ax.SetZ(-1*extrusion.GetX());
	}
   else
	{
	//printf("DXFPolyline::GetPoints biger values\n");
	Ax.SetX(-1*extrusion.GetY());
	Ax.SetY(extrusion.GetX());
	Ax.SetZ(0);
	}

   float k=sqrt(1/(Ax.GetX()*Ax.GetX()+Ax.GetY()*Ax.GetY()+Ax.GetZ()*Ax.GetZ()));
   Ax.SetX(Ax.GetX()*k);
   Ax.SetY(Ax.GetY()*k);
   Ax.SetZ(Ax.GetZ()*k);


   Ay.SetX(extrusion.GetY()*Ax.GetZ()-extrusion.GetZ()*Ax.GetY());
   Ay.SetY(extrusion.GetZ()*Ax.GetX()-extrusion.GetX()*Ax.GetZ());
   Ay.SetZ(extrusion.GetX()*Ax.GetY()-extrusion.GetY()*Ax.GetX());
   //printf("DXFPolyline::GetPoints Az: %f %f %f Ax: %f %f %f Ay: %f %f %f \n",extrusion.GetX(),extrusion.GetY(),extrusion.GetZ(),
	//Ax.GetX(),Ax.GetY(),Ax.GetZ(),Ay.GetX(),Ay.GetY(),Ay.GetZ());

   k=sqrt(1/(Ay.GetX()*Ay.GetX()+Ay.GetY()*Ay.GetY()+Ay.GetZ()*Ay.GetZ()));
   Ay.SetX(Ay.GetX()*k);
   Ay.SetY(Ay.GetY()*k);
   Ay.SetZ(Ay.GetZ()*k);
  

   while (vert)
   {
      array[i++] = vert->point.GetX()*Ax.GetX()+vert->point.GetY()*Ay.GetX()+vert->point.GetZ()*extrusion.GetX();
      array[i++] = vert->point.GetX()*Ax.GetY()+vert->point.GetY()*Ay.GetY()+vert->point.GetZ()*extrusion.GetY();
      array[i++] = vert->point.GetX()*Ax.GetZ()+vert->point.GetY()*Ay.GetZ()+vert->point.GetZ()*extrusion.GetZ();
      //printf("DXFPolyline::GetPoints %f %f %f\n",array[i-3], array[i-2], array[i-1]);
      vert = vert->next;
   }
Dump();
   return i;
}


void DXFPolyline::SetCellData(double data)
{
     cellSetsTail->setCellData(cellSetsTail->GetNumVertices(), data);
	 return;
};

void DXFPolyline::Dump()
{
//   printf("   Polyline\n");
//   printf("      Flags = %04X\n", flags);
//   printf("      Elevation = %f\n", elevation);
//   printf("      mVertices = %d\n", mVertices);
//   printf("      nVertices = %d\n", nVertices);
//   printf("      mSmooth = %d\n", mSmooth);
//   printf("      nSmooth = %d\n", nSmooth);
//   printf("      curves = %d\n", curves);

   DXFVertex *vert = 0;   // verticesHead;

   while (vert)
   {
      vert->Dump();
      vert = vert->next;
   }
}

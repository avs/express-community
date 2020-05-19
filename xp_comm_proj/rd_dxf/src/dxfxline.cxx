/* This is the first attempt to produce something similar to the AVS 
   DFX reader programs to extend the entities read. 
   Version 1.0 started 28th July 1997 by Stephen Marsland


******************************************************************************

Implementation of the DXFXLine class, which represents DXF ray objects.

******************************************************************************
*****************************************************************************/

#include "dxfxline.hxx"
#include "dxfprim.hxx"
#include "blistcpp.hxx"
#include "blistcpp.cxx"   //## AWD: Added to resolve templates problem
#include "dxfdefine.hxx"
#include "dxfheader.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Global Variables  *************************/

extern DXFHeader *globalVar;


/************************  Private Type Definitions  ************************/

/* <none> */


/*******************  Private (Static) Global Variables  ********************/

/* <none> */



/*****************************************************************************
**********--------   Implementations of "DXFLine" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   DXFXLine::Make()
Purpose:  This is the static "virtual constructor" for the DXFXLine class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a ray, then
	  an instance of DXFXLine is created, and its attributes are read
	  from the DXF file (via the "group" object).
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
		  firstLayer   start of the linked list fo LAYER information. This 
		  will be used to assign color if appropriate
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
Returns:  A pointer to a DXFXLine object (cast to DXFPrim *) if the current
          object was recognized as a DXF ray, or
	  NULL if it wasn't.
*****************************************************************************/
DXFPrim *DXFXLine::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("XLINE"))
      instance = new DXFXLine(group, firstLayer);

   return instance;
}


/*****************************************************************************
Method:   DXFXLine::DXFXLine()
Purpose:  A Constructor for the DXFXLine class.  Reads the start point
          and direction from the DXF file.
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
DXFXLine::DXFXLine(DXFGroup &group, CLIST <DXFLayer> *firstLayer) : start(), dir()
{
   isValid = 0;
   k1=0.0;
   k2=0.0;
   int c=0;

   while (group.Read())
   {
      if (group.Code() == 0)   // stop at next entity definition
	 break;

      switch (group.Code())
      {
	case 10:   // XLine start point x coordinate
	 start.SetX(group.DataAsFloat());
	 break;

	case 20:   // XLine start point y coordinate
	 start.SetY(group.DataAsFloat());
	 break;

	case 30:   // XLine start point z coordinate
	 start.SetZ(group.DataAsFloat());
	 break;

	case 11:   // x part of unit direction vector
	 dir.SetX(group.DataAsFloat());
	 break;

	case 21:   // y part of unit direction vector
	 dir.SetY(group.DataAsFloat());
	 break;

	case 31:   // z part of unit direction vector
	 dir.SetZ(group.DataAsFloat());
	 break;

	default:                      // we don't care about this group code
	 DXFPrim::ReadCommon(group, firstLayer);  // see if the base class wants it
	 break;
      }
   }

   // Now that we have the Ray data (its start point and direction), we need to
   // generate the connectivity information.
   //  **This is the dubious bit. I'm assuming that a ray starts at a 
   // well-defined point and continues indefinately.

  // if(start.GetX()<globalVar->GetExtMin().GetX() || start.GetX()>globalVar->GetExtMax().GetX() ||
  //    start.GetY()<globalVar->GetExtMin().GetY() || start.GetY()>globalVar->GetExtMax().GetY())
  //	isValid = 0;

  // test if the xline is in our "visiable window" given by $EXTMAX and $EXTMIN

    if(start.GetX()<globalVar->GetExtMin().GetX() || start.GetX()>globalVar->GetExtMax().GetX() ||
    start.GetY()<globalVar->GetExtMin().GetY() || start.GetY()>globalVar->GetExtMax().GetY())
    {
   	if(dir.GetX()!=0)
	{
	   float n=(globalVar->GetExtMax().GetX()-start.GetX())/dir.GetX();
	   if(start.GetY()+n*dir.GetY() <= globalVar->GetExtMax().GetY() &&
	      start.GetY()+n*dir.GetY() >= globalVar->GetExtMin().GetY()) k1=n;
           printf("k1 %f k2 %f\n",k1,k2);
           n=(globalVar->GetExtMin().GetX()-start.GetX())/dir.GetX();
	   if(start.GetY()+n*dir.GetY() <= globalVar->GetExtMax().GetY() &&
	      start.GetY()+n*dir.GetY() >= globalVar->GetExtMin().GetY()) 
           {
                if(k1)k2=n;
                else k1=n;
	   }
           printf("k1 %f k2 %f\n",k1,k2);
        }
        if(dir.GetY()!=0)
	{
	   float n=(globalVar->GetExtMax().GetY()-start.GetY())/dir.GetY();
	   if(start.GetX()+n*dir.GetX() <= globalVar->GetExtMax().GetX() &&
	      start.GetX()+n*dir.GetX() >= globalVar->GetExtMin().GetX())
	   {
                if(k1)k2=n;
                else k1=n;
	   }
           printf("k1 %f k2 %f\n",k1,k2);
           n=(globalVar->GetExtMin().GetY()-start.GetY())/dir.GetY();
	   if(start.GetX()+n*dir.GetX() <= globalVar->GetExtMax().GetX() &&
	      start.GetX()+n*dir.GetX() >= globalVar->GetExtMin().GetX()) 
           {
                if(k1)k2=n;
                else k1=n;
	   }
           printf("k1 %f k2 %f\n",k1,k2);
        }
        if(k1 && k2) c=1;
     }
     else
     {
        float a=(dir.GetX()<0) ? (globalVar->GetExtMin().GetX()-start.GetX())/dir.GetX() : (globalVar->GetExtMax().GetX()-start.GetX())/dir.GetX();
        float b=(dir.GetY()<0) ? (globalVar->GetExtMin().GetY()-start.GetY())/dir.GetY() : (globalVar->GetExtMax().GetY()-start.GetY())/dir.GetY();
        k1=(a*a<b*b) ? a : b;
        if(dir.GetX()==0.0) k1=b;
        if(dir.GetY()==0.0) k1=a;
        a=(dir.GetX()<0) ? (globalVar->GetExtMax().GetX()-start.GetX())/dir.GetX() : (globalVar->GetExtMin().GetX()-start.GetX())/dir.GetX();
        b=(dir.GetY()<0) ? (globalVar->GetExtMax().GetY()-start.GetY())/dir.GetY() : (globalVar->GetExtMin().GetY()-start.GetY())/dir.GetY();
        k2=(a*a<b*b) ? a : b;
        if(dir.GetX()==0.0) k2=b;
        if(dir.GetY()==0.0) k2=a;
	c=1;
     }

  
   if (c && AddCellSet(2, 1, DXF_CELL_LINE))
   {
      cellSetsTail->AddVertexIndex(0);
      // *DON'T NEED THIS??? 
      cellSetsTail->AddVertexIndex(1);

      isValid = 1;
   }
}


/*****************************************************************************
Method:   DXFXLine::GetPoints()
Purpose:  This function generates the vertex associated with this Ray
          (i.e. its start point).
Params:   array   A float array in which to place the coordinates.  It is
                  assumed that this array is at least < GetNumPoints() >
		  floats in length.
	  offset  An optional offset into the array, at which to start
                  placing coordinates.  Thus, if this parameter is supplied
		  the array length must be < GetNumPoints() + offset >
		  floats long.
State:    ---
Returns:  The number of *NUMBERS* (i.e. 3 times the number of vertices)
          placed in the array PLUS the starting offset, if any.
*****************************************************************************/
int DXFXLine::GetPoints(float *array, int offset)
{
   //printf("DXFXLine::GetPoints %f k1 %f k2 %f\n",globalVar->GetExtMax().GetX(),k1,k2);


   array[offset++] = k1*dir.GetX()+start.GetX();
   array[offset++] = k1*dir.GetY()+start.GetY();
   array[offset++] = k1*dir.GetZ()+start.GetZ();

   array[offset++] = k2*dir.GetX()+start.GetX();
   array[offset++] = k2*dir.GetY()+start.GetY();
   array[offset++] = k2*dir.GetZ()+start.GetZ();
 
Dump();   
return offset;
}




void DXFXLine::Dump()
{
//   printf("   Ray\n");
//   printf("      Start = (%f, %f, %f)\n",
//	  start.GetX(), start.GetY(), start.GetZ());
//   printf("      Direction = (%f, %f, %f)/n", 
//	  dir.GetX(), dir.GetY(), dir.GetZ());
   //   printf("        End = (%f, %f, %f)\n", end.GetX(), end.GetY(), end.GetZ());
}





/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

Implementation of the DXFLine class, which represents DXF line objects.

******************************************************************************
*****************************************************************************/
#include "blistcpp.hxx"
#include "blistcpp.cxx"   //## AWD: Added to resolve templates problem
#include "dxflayer.hxx"
#include "dxfprim.hxx"
#include <string.h>

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Global Variables  *************************/

/* <none> */


/************************  Private Type Definitions  ************************/

/* <none> */


/*******************  Private (Static) Global Variables  ********************/

/* <none> */



/*****************************************************************************
**********--------   Implementations of "DXFLine" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   DXFLine::Make()
Purpose:  This is the static "virtual constructor" for the DXFLine class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a line, then
	  an instance of DXFLine is created, and its attributes are read
	  from the DXF file (via the "group" object).
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
Returns:  A pointer to a DXFLine object (cast to DXFPrim *) if the current
          object was recognized as a DXF line, or
	  NULL if it wasn't.
*****************************************************************************/
DXFPrim *DXFLayer::Make(DXFGroup &group)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("LAYER"))
      instance = new DXFLayer(group);

   return instance;
}


/*****************************************************************************
Method:   DXFLayer::DXFLayer()
Purpose:  A Constructor for the DXFLayer class.  Reads the color
          and flags from the DXF file.
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file is in
          the TABLES section.
*****************************************************************************/
DXFLayer::DXFLayer(DXFGroup &group) : flags()
{
   isValid = 0;

   while (group.Read())
   {
      if (group.Code() == 0)   // stop at next entity definition
	 break;

      switch (group.Code())
      {
	case 62:   // color number
	  color = group.DataAsInt();
	  break;

	case 70:   // flags - these are not transferred yet
	 flags = group.DataAsUnsigned();
	 break;

	case 2:   // name which defines the layer
	 strcpy(name, group.Data());
	 break;

	default:                      // we don't care about this group code
	 DXFPrim::ReadCommon(group);  // see if the base class wants it
	 break;
      }
   }

}

/*****************************************************************************
Method:   DXFLayer::NameCmp()
Purpose:  Performs a case-insensitive string compare to determine if the
          contents of the "data" attribute matches the specified string.
Params:   s   The string to compare with the contents of "data".
State:    This function assumes that the character encoding is ASCII;
          specifically that [A-Z] and [a-z] are separate contiguous ranges,
	  and that the characters are ordered low-to-high as indicated.
Return:   True   if the contents of the "data" attribute matches the
                 specified string,
          False  if the strings don't match
*****************************************************************************/
char DXFLayer::NameCmp(const char *s) const
{
   char rCode = 0;

   const char *x = name;
   char a, b;
   char conv = 'a' - 'A';

   rCode = 1;   // assume strings are equal until we find a mismatch

   do
     {
	 a = *x++;
	 b = *s++;

	 if ((a >= 'a') && (a <= 'z'))   // convert to upper case
	    a -= conv;

	 if ((b >= 'a') && (b <= 'z'))   // convert to upper case
	    b -= conv;

	 if (a != b)
	    {
	    rCode = 0;
	    break;
	    }
     } while (a && b);
   
   return rCode;
}










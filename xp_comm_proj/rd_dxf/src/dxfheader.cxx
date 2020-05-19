/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************/

#include "dxfheader.hxx"
#include "dxfprim.hxx"
#include "dxfdefine.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Global Variables  *************************/

/* <none> */


/************************  Private Type Definitions  ************************/

/* <none> */


/*******************  Private (Static) Global Variables  ********************/

/* <none> */



/*****************************************************************************
*********--------   Implementations of "DXFHeader" Methods   --------*********
*****************************************************************************
Implementation of the DXFHeader class, which provides global variables
*****************************************************************************
Method:   DXFHeader::Make()
Purpose:  This is the static "virtual constructor" for the DXFHeader class.
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into HEADER section.
Returns:  A pointer to a DXFHEADER object 
*****************************************************************************/
DXFHeader *DXFHeader::Make(DXFGroup &group)
{ 
DXFHeader *instance = 0;
instance=new DXFHeader(group);
return instance;
}

/*****************************************************************************
Method:   DXFHeader::DXFHeader()
Purpose:  A Constructor for the DXFHeader class.  Reads the global variable 
          from the DXF file.
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the HEADER section.
*****************************************************************************/
DXFHeader::DXFHeader(DXFGroup &group)
{
int i=2;
int a=1;
const char * temp;
   while (a)
   {
      if(i>1) a=group.Read();
      i++;
      if (group.Code() == 0)   // stop at next definition
	 break;

      if (group.Code() == 9)   // new Variable
      {
          if(group.DataCmp("$EXTMAX"))
	  {
             i=1;
             while (a=group.Read())
             {
                if(group.Code()==9)
                {
                   i=0;
                   break;
	        }
      		switch (group.Code())
      		{
		   case 10:  
	 	    rextmax.SetX(group.DataAsFloat());
                    temp=group.Data();
		    if(strncmp(temp,"-1.000000E+20",12)==0) extmax.SetX(rextmax.GetX()*(-1));
		    if(extmax.GetX()<0)extmax.SetX(rextmax.GetX()*1.2);
		    else extmax.SetX(rextmax.GetX()+rextmax.GetX()*0.2);
	 	    break;

		   case 20:  
	 	    rextmax.SetY(group.DataAsFloat());
                    temp=group.Data();
		    if(strncmp(temp,"-1.000000E+20",12)==0) extmax.SetY(rextmax.GetY()*(-1));
		    if(extmax.GetY()<0)extmax.SetY(rextmax.GetY()*1.2);
		    else extmax.SetY(rextmax.GetY()+rextmax.GetY()*0.2);
	 	    break;

		   case 30:   
	 	    rextmax.SetZ(group.DataAsFloat());
                    temp=group.Data();
		    if(strncmp(temp,"-1.000000E+20",12)==0) extmax.SetZ(rextmax.GetZ()*(-1));
		    if(extmax.GetZ()<0)extmax.SetZ(rextmax.GetZ()*1.2);
		    else extmax.SetZ(rextmax.GetZ()+rextmax.GetZ()*0.2);
	 	    break;
		}
             }
	  }
          if(group.DataCmp("$EXTMIN"))
	  {
	     i=1;
             while (a=group.Read())
	     {
                if(group.Code()==9)
                {
                   i=0;
                   break;
	        }
      		switch (group.Code())
      		{
		   case 10:   
	 	    rextmin.SetX(group.DataAsFloat());
                    temp=group.Data();
		    if(strcmp(temp,"1.000000E+20")==0) extmin.SetX(rextmin.GetX()*(-1));
		    if(extmin.GetX()<0) extmin.SetX(rextmin.GetX()*1.2);
		    else extmin.SetX(rextmin.GetX()-rextmin.GetX()*0.2);
	 	    break;

		   case 20:   
	 	    rextmin.SetY(group.DataAsFloat());
                    temp=group.Data();
		    if(strcmp(temp,"1.000000E+20")==0) extmin.SetY(rextmin.GetY()*(-1));
		    if(extmin.GetY()<0) extmin.SetY(rextmin.GetY()*1.2);
		    else extmin.SetY(rextmin.GetY()-rextmin.GetY()*0.2);
	 	    break;

		   case 30:   
	 	    rextmin.SetZ(group.DataAsFloat());
                    temp=group.Data();
		    if(strcmp(temp,"1.000000E+20")==0) extmin.SetZ(rextmin.GetZ()*(-1));
		    if(extmin.GetZ()<0) extmin.SetZ(rextmin.GetZ()*1.2);
		    else extmin.SetZ(rextmin.GetZ()-rextmin.GetZ()*0.2);
	 	    break;
		}
	     }
	  }
      }
   }
}

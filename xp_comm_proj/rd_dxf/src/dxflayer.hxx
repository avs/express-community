/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

This file contains the DXFLayer class definition, which is the subclass of
DXFPrim that represents the DXF layer primitive.

******************************************************************************
*****************************************************************************/
#ifndef _DXF_LAYER_H
#define _DXF_LAYER_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"


/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFLayer
Purpose:  This class is used to represent DXF layer objects.
Publics:  
          
	  
*****************************************************************************/
class DXFLayer : public DXFPrim
{
  public:
   DXFLayer(DXFGroup &group);
   DXFLayer(const DXFLayer &l) :  flags(l.flags)  {}
   virtual ~DXFLayer() {prinf("destructor layer\n");}
   unsigned GetFlags()  { return flags; }
   char NameCmp(const char *s) const;
   static DXFPrim *Make(DXFGroup &group);
   char *GetName() {return name;}


  private:
    unsigned flags;
	char name[DXF_MAX_LINE_LEN + 1];
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif













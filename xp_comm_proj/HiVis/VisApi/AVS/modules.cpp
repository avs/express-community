// Technische Universität München - Peter.Breitling@in.tum.de
// Please read the file LICENSE for conditions associated with this file.

#include "../VisApi.h"

int HiVis_HiVisMods_HiVis_Grid::upd(OMevent_mask, int)
{
  // hasChanged (OMXint read req notify)
  // type (OMXint write)
  // attributeCount (OMXint write)
  // cells (OMXint write)
  // nodes (OMXint write)
  
  /***********************/
  /* Function's Body     */
  /***********************/
  
  visMsgCall << "HiVis_HiVisMods_HiVis_Grid::"
	     << "upd: called."
	     << visMsgEnd;

   // Get access to visAdaptiveGrid
   
   visGridLink *gridLink = 
     (visGridLink *) ret_class_ptr("visGridLink");
   
   if (!gridLink) {

     
     visMsgError << "HiVis_HiVisMods_HiVis_Grid::"
		 << "upd: gridLink is uninitialized" 
		 << visMsgErrorEnd;
     return 0;
   }
   
   visGrid *fieldAccess = gridLink->myFieldAccess;
   
   if (!fieldAccess || !fieldAccess->isAllocated()) {
     visMsgError << "HiVis_HiVisMods_HiVis_Grid::"
		 << "upd: fieldAccess is uninitialized."
		 << visMsgErrorEnd;
     return 0;
   }
   
   // Update fieldaccess info

   int i;

   dim = (int) fieldAccess->getDim();
   //type = fieldAccess->getFieldAccessType();

   int *sizeArray = (int *) size.ret_array_ptr(OM_GET_ARRAY_WR);
   if (sizeArray) {
     for (i = 0; i < fieldAccess->getDim(); i++) {
       sizeArray[i] = (int) fieldAccess->getSize(i);
     }
     ARRfree((void *) sizeArray);
   }

   attributeCount = fieldAccess->getAttributes();
   cells = fieldAccess->getCells();
   nodes = fieldAccess->getNodes();
   
   double *minArray =  (double *) minScalarValue.ret_array_ptr(OM_GET_ARRAY_WR);
   double *maxArray =  (double *) maxScalarValue.ret_array_ptr(OM_GET_ARRAY_WR);
   int *attrDim = (int *) attributeDim.ret_array_ptr(OM_GET_ARRAY_WR);
   for (i = 0; i < fieldAccess->getAttributes(); i++) {
     visAttributeFactory *factory = fieldAccess->getAttrFactory(i);
     attrDim[i] = factory->getDim();
     attributeName.
       set_str_array_val(i, (char *)fieldAccess->getAttrName(i));
     visAttribute *min = (visAttribute *)fieldAccess->getMinAttribute(i);
     visAttribute *max = (visAttribute *)fieldAccess->getMaxAttribute(i);
     if (min) {
       minArray[i] = min->getScalarValue(factory->getDim()); 
       min->decRef(); 
     }
     if (max) {
       maxArray[i] = max->getScalarValue(factory->getDim());
       max->decRef(); 
     }
     factory->decRef();
   }
   ARRfree((void *) attrDim);
   ARRfree((void *) minArray);
   ARRfree((void *) maxArray);

   // return 1 for success

   return(1);
}


int HiVis_HiVisMods_AG_Cell2Node::upd(OMevent_mask, int)
{
  // gridInterp (HiVis_Grid_Interp read notify)
  // out (HiVis_Grid write)
  
  /***********************/
  /* Function's Body     */
  /***********************/
  visMsgCall << "HiVis_HiVisMods_AG_Cell2Node::"
	     << "upd: called."
	     << visMsgEnd;
  
  // Get pointer to in.fieldAccess
  
  visGridLink *inFieldAccessLink = 
    (visGridLink *) in.ret_class_ptr("visGridLink");
  
  if (!inFieldAccessLink) {
    visMsgError << "HiVis_HiVisMods_AG_Cell2Node::"
		<< "upd: in.gridLink is uninitialized"
		<< visMsgEnd;
    return 0;
  }
  
  visAdaptiveGrid *inFieldAccess = inFieldAccessLink->castToAdaptiveGrid();
  
  if (!inFieldAccess) {
    visMsgError << "HiVis_HiVisMods_AG_Cell2Node::"
		<< "upd: in.fieldAccess is uninitialized"
		<< visMsgEnd;
    return 0;
  }
  
  
  // allocate requested default cell to node proxy class
  
  visCell2Node *nodeGrid = NULL;
  if (!gridInterp.valid_obj()) {
    visMsgError << "HiVis_HiVisMods_AG_Cell2Node: grid interpolation undefined."
		<< visMsgErrorEnd;
    return 0;
  }

  int i;
  char *id = (char*) gridInterp.id;
  if (id == NULL) {
    nodeGrid = new visCell2NodeBlock();
  } else {
    for (i = 0; i < visCell2NodeTypes; i++) {
      if (!strcmp(id, visCell2NodeStrings[i])) {
	switch (i) {
	case visCell2NodeTypeBlock:
	  nodeGrid = new visCell2NodeBlock();
	  break;
	case visCell2NodeTypeOLinear:
	  nodeGrid = new visCell2NodeOLinear();
	  break;
	case visCell2NodeTypeMLinear:
	  nodeGrid = new visCell2NodeMLinear();
	  break;
	}
	break;
      }
    }
    if (nodeGrid == NULL) {
      visMsgError << "HiVis_HiVisMods_AG_Cell2Node::"
		  << "upd: unsupported cell2node interpolation type"      
		  << visMsgEnd;
      return 0;
    }
  }
  
  nodeGrid->allocate(inFieldAccess);
  
  // Set pointer to nodeGrid in out.fieldAccess
   
  visGridLink *outFieldAccessLink = 
    (visGridLink *) out.ret_class_ptr("visGridLink");
  
  if (!outFieldAccessLink) {
    visMsgError << "HiVis_HiVisMods_AG_Cell2Node::"
		<< "upd: out.gridLink is uninitialized"
		<< visMsgEnd;
    return 0;
  }
  
  outFieldAccessLink->myFieldAccess = nodeGrid;
  
  
  // Sign change
  
  if (!out.hasChanged.valid_obj()) {
    out.hasChanged = 0;
  }

  int hasChanged = (int) out.hasChanged; out.hasChanged = ++hasChanged;

  // return 1 for success

  return(1);
}

int HiVis_HiVisMods_Field_To_HiVis::upd(OMevent_mask, int)
{
  // in (Grid_Struct+Node_Data read req notify)
  
  // in.nnodes (int)
  // in.nnode_data (int)
  
  /*
    int  in_data_comp;
    int  in_data_size, in_data_type;
    for (in_data_comp = 0; in_data_comp < in.nnode_data; in_data_comp++) { 
    // in.node_data[in_data_comp].veclen (int) 
    // in.node_data[in_data_comp].values (char [])
    char *in_node_data = (char *)in.node_data[in_data_comp].values.ret_array_ptr(OM_GET_ARRAY_RD, &in_data_size, &in_data_type);
    if (in_node_data)
    ARRfree((char *)in_node_data);
    }
  */
  
  // out (HiVis_Grid write)
  
  /***********************/
  /* Function's Body     */
  /***********************/
  visMsgCall << "HiVis_HiVisMods_Field_To_HiVis::"
	     << "upd: called."
	     << visMsgEnd;
  
  // Set pointer to this in out.fieldAccess
   
  visGridLink *gridLink = 
    (visGridLink *) out.ret_class_ptr("visGridLink");
   
  if (!gridLink) {
    visMsgError << "HiVis_HiVisMods_Field_To_HiVis::"
		<< "upd: gridLink is uninitialized"
		<< visMsgErrorEnd;
    return 0;
  }

  // allocate requested default cell to node proxy class


  if (0) {
    int refs;
    OMobj_id sourceId;
    OMXgroup *source;
    sourceId = OMfind_subobj(this->obj_id(), OMstr_to_name("in"), 
			     OM_SUB_TEMPL | OM_OBJ_RD);
    source = (OMXgroup*) OMret_omx_ptr(sourceId, 0);
    
    if (!source) {
      visMsgError << "HiVis_HiVisMods_Field_To_HiVis::"
		  << "upd: couldn't cast to source object"
		  << visMsgErrorEnd;
      return 0;
    }
  }

  FLD_Grid_Struct *grid_struct = 
    (FLD_Grid_Struct*)in.ret_omx_ptr("FLD_Grid_Struct");
  if (!grid_struct) visMsgDbg << "no grid struct" << visMsgEnd;

  FLD_Node_Data *node_data = 
    (FLD_Node_Data*)in.ret_omx_ptr("FLD_Node_Data");
  if (!node_data) visMsgDbg << "no node data" << visMsgEnd;

  FLD_Cell_Data *cell_data = 
    (FLD_Cell_Data*)in.ret_omx_ptr("FLD_Cell_Data");
  if (!cell_data) visMsgDbg << "no cell data" << visMsgEnd;


  // Check if source field is initialized
  
  {
    int *p = (int *) in.dims.ret_array_ptr(OM_GET_ARRAY_RD);
    if (!p) {
      visMsgError << "HiVis_HiVisMods_Field_To_HiVis::"
		  << "upd: field is uninitialized"
		  << visMsgErrorEnd;
      return 0;
    }
    ARRfree(p);
  }

  // Get grid type and allocate appropriate VisApi grid if supported

  visGrid *grid;
  if (grid_struct) {
    grid = new visUniformGrid();
    if (!((visUniformGrid*)grid)->allocate(grid_struct, node_data, cell_data,
					   (int) assoc)) {
      return 0;
    }
  } else {
    visMsgError << "HiVis_HiVisMods_Field_To_HiVis::"
		<< "upd: grid type not supported"
		<< visMsgErrorEnd;
    return 0;
  }

  gridLink->myFieldAccess = grid;

  // Copy attribute type id

  if (node_data) {
    out.attributeCount = grid->getAttributes();
    int *dataID = (int *) out.attributeDataID.ret_array_ptr(OM_GET_ARRAY_WR);
    int i;
    for (i = 0; i < grid->getAttributes(); i++) {
      if (node_data->node_data[i].id.valid_obj()) {
	dataID[i] = node_data->node_data[i].id;
      } else {
	dataID[i] = 0;
      }
    }
    ARRfree((void *) dataID);
  }

  // Sign change

  if (!out.hasChanged.valid_obj()) {
    out.hasChanged = 0;
  }

  int hasChanged = (int) out.hasChanged; out.hasChanged = ++hasChanged;

  // return 1 for success

  return(1);
}

int HiVis_HiVisMods_HiVis_Read_Field::upd(OMevent_mask, int)
{
   // in (HiVis_Grid read req notify)
   // out (HiVis_Grid write)

   /***********************/
   /* Function's Body     */
   /***********************/

   visMsgCall << "HiVis_HiVisMods_HiVis_Read_Field::"
	      << "upd: called."
	      << visMsgEnd;

   // Set pointer to this in out.fieldAccess
   
   visGridLink *outFieldAccessLink = 
     (visGridLink *) out.ret_class_ptr("visGridLink");
   
   if (!outFieldAccessLink) {
     visMsgError << "HiVis_HiVisMods_HiVis_Read_Field::"
		 << "upd: out.gridLink is uninitialized"
		 << visMsgEnd;
     return 0;
   }

   ifstream in(fileName);

   visGrid *grid = visGrid::readField(in);
   
   if (!grid || !grid->isAllocated()) {
     visMsgDbg << "read field failed!" << visMsgEnd;
     return(0);
   }
   
   // Update out parameters

   outFieldAccessLink->myFieldAccess = grid;

   // Sign change

   if (!out.hasChanged.valid_obj()) {
     out.hasChanged = 0;
   }

   int hasChanged = (int) out.hasChanged; out.hasChanged = ++hasChanged;

   // return 1 for success

   return 1;
}



int HiVis_HiVisMods_HiVis_Read_Octree::upd(OMevent_mask, int)
{
   // in (HiVis_Grid read req notify)
   // out (HiVis_Grid write)

   /***********************/
   /* Function's Body     */
   /***********************/

   visMsgCall << "HiVis_HiVisMods_HiVis_Read_Octree::"
	      << "upd: called."
	      << visMsgEnd;


   // Get OctreeAccess
   
   visOctree* fieldAccess = (visOctree*) this->ret_class_ptr("visOctree");
   
   if (!fieldAccess) {
     visMsgError << "HiVis_HiVisMods_HiVis_Read_Octree::"
		 << "upd: fieldAccess is uninitialized"
		 << visMsgEnd;
     return 0;
   }
   
   // Set pointer to this in out.fieldAccess
   
   visGridLink *outFieldAccessLink = 
     (visGridLink *) out.ret_class_ptr("visGridLink");
   
   if (!outFieldAccessLink) {
     visMsgError << "HiVis_HiVisMods_HiVis_Read_Octree::"
		 << "upd: out.gridLink is uninitialized"
		 << visMsgEnd;
     return 0;
   }

   outFieldAccessLink->myFieldAccess = fieldAccess;

   // Perform the conversion
   
   ifstream in(fileName);

   if (!fieldAccess->allocate(in)) {
     visMsgDbg << "allocate failed!" << visMsgEnd;
     return(0);
   }
   
   // Update out parameters

   maxLevel = fieldAccess->myMaxLevel;

   // Sign change

   if (!out.hasChanged.valid_obj()) {
     out.hasChanged = 0;
   }

   int hasChanged = (int) out.hasChanged; out.hasChanged = ++hasChanged;

   // return 1 for success

   return 1;
}

int HiVis_HiVisMods_AG_Octree::upd(OMevent_mask, int)
{
   // in (HiVis_Grid read req notify)
   // out (HiVis_Grid write)

   /***********************/
   /* Function's Body     */
   /***********************/

   visMsgCall << "HiVis_HiVisMods_AG_Octree::"
	      << "upd: called."
	      << visMsgEnd;


   // Get OctreeAccess
   
   visOctree* fieldAccess = (visOctree*)
     this->ret_class_ptr("visOctree");
   
   if (!fieldAccess) {
     visMsgError << "HiVis_HiVisMods_AG_Octree::"
		 << "upd: fieldAccess is uninitialized"
		 << visMsgEnd;
     return 0;
   }
   
   // Set pointer to this in out.fieldAccess
   
   visGridLink *outFieldAccessLink = 
     (visGridLink *) out.ret_class_ptr("visGridLink");
   
   if (!outFieldAccessLink) {
     visMsgError << "HiVis_HiVisMods_AG_Octree::"
		 << "upd: out.gridLink is uninitialized"
		 << visMsgEnd;
     return 0;
   }

   outFieldAccessLink->myFieldAccess = fieldAccess;

   // Get pointer to in.fieldAccess

   visGridLink *inFieldAccessLink = 
     (visGridLink *) in.ret_class_ptr("visGridLink");
   
   if (!inFieldAccessLink) {
     visMsgError << "HiVis_HiVisMods_AG_Octree::"
		 << "upd: in.gridLink is uninitialized"
		 << visMsgEnd;
     return 0;
   }

   visAdaptiveGrid *inFieldAccess = inFieldAccessLink->castToAdaptiveGrid();

   if (!inFieldAccess) {
     visMsgError << "HiVis_HiVisMods_AG_Octree::"
		 << "upd: in.fieldAccess is uninitialized"
		 << visMsgEnd;
     return 0;
   }

   // Set level to max if not defined

   if (!level.valid_obj()) {
     int maxSizeComp = 0;
     int i;
     for (i = 0; i < inFieldAccess->getDim(); i++) {
       if (maxSizeComp < inFieldAccess->getSize(i)) {
	 maxSizeComp = inFieldAccess->getSize(i);
       }
     }
     
     // Use this to calculate virtual size and max cursor level
     
     level = 0;
     int s = maxSizeComp * 2 - 1;
     while ((s /= 2) > 0) { 
       level = level + 1;
     }
   }

   // Allocate attribute combiner

   visAttrComb *attrComb;
   if (attr_Comb.valid_obj()) {
     if (((int) attr_Comb.id) == visAttrCombTypeDefault) {
       visMsgDbg << "allocating comb default" << visMsgEnd;
       attrComb = new visAttrCombDefault();
     } else if (((int) attr_Comb.id) == visAttrCombTypeEpsilon) {
       visMsgDbg << "allocating comb epsilon" << visMsgEnd;
       attrComb = new visAttrCombEpsilon((int) attr_Comb.attribute,
					 (double) attr_Comb.epsilon,
					 (int) attr_Comb.epsCalculateBy,
					 (int) attr_Comb.epsApplyOn);
     }
   } else {
     visMsgDbg << "allocating none" << visMsgEnd;
     attrComb = NULL;
   }
   
   // Perform the conversion
   
   fieldAccess->allocate(inFieldAccess, 
			 attrComb,
			 (int) level);
   
   // Update out parameters

   maxLevel = fieldAccess->myMaxLevel;

   // Sign change

   if (!out.hasChanged.valid_obj()) {
     out.hasChanged = 0;
   }

   int hasChanged = (int) out.hasChanged; out.hasChanged = ++hasChanged;

   // return 1 for success

   return 1;
}

int HiVis_HiVisMods_HiVis_Read_SparseGrid::upd(OMevent_mask, int)
{
   // in (HiVis_Grid read req notify)
   // out (HiVis_Grid write)

   /***********************/
   /* Function's Body     */
   /***********************/

   visMsgCall << "HiVis_HiVisMods_HiVis_Read_SparseGrid::"
	      << "upd: called."
	      << visMsgEnd;


   // Get SparseGridAccess
   
   visSparseGrid* fieldAccess = (visSparseGrid*)
     this->ret_class_ptr("visSparseGrid");
   
   if (!fieldAccess) {
     visMsgError << "HiVis_HiVisMods_HiVis_Read_SparseGrid::"
		 << "upd: fieldAccess is uninitialized"
		 << visMsgEnd;
     return 0;
   }
   
   // Set pointer to this in out.fieldAccess
   
   visGridLink *outFieldAccessLink = 
     (visGridLink *) out.ret_class_ptr("visGridLink");
   
   if (!outFieldAccessLink) {
     visMsgError << "HiVis_HiVisMods_HiVis_Read_SparseGrid::"
		 << "upd: out.gridLink is uninitialized"
		 << visMsgEnd;
     return 0;
   }

   outFieldAccessLink->myFieldAccess = fieldAccess;

   // Perform the conversion
   
   ifstream in(fileName);

   if (in.fail() || in.bad() || in.eof()) {
     visMsgError << "Couldn't open file " 
		 << (const char*) fileName << visMsgErrorEnd;
     return(0);
   }
   
   // Set level to max if not defined

   if (!level.valid_obj()) {
     level = 0x7fffffff;
   }

   // Allocate attribute combiner

   visAttrComb *attrComb;
   if (attr_Comb.valid_obj()) {
     if (((int) attr_Comb.id) == visAttrCombTypeDefault) {
       visMsgDbg << "allocating comb default" << visMsgEnd;
       attrComb = new visAttrCombDefault();
     } else if (((int) attr_Comb.id) == visAttrCombTypeEpsilon) {
       visMsgDbg << "allocating comb epsilon" << visMsgEnd;
       attrComb = new visAttrCombEpsilon((int) attr_Comb.attribute,
					 (double) attr_Comb.epsilon,
					 (int) attr_Comb.epsCalculateBy,
					 (int) attr_Comb.epsApplyOn);
     }
   } else {
     visMsgDbg << "allocating none" << visMsgEnd;
     attrComb = NULL;
   }
   
   if (!fieldAccess->allocate(in, attrComb, (int) level, 
			      (int) maxLdScopeDiff)) {
     visMsgDbg << "allocate failed!" << visMsgEnd;
     return(0);
   }
   
   // Update out parameters

   maxLevel = fieldAccess->myMaxLevel;

   // Sign change

   if (!out.hasChanged.valid_obj()) {
     out.hasChanged = 0;
   }

   int hasChanged = (int) out.hasChanged; out.hasChanged = ++hasChanged;

   // return 1 for success

   return 1;
}

int HiVis_HiVisMods_AG_SparseGrid::upd(OMevent_mask, int)
{
   // level (OMXint read notify)
   // attrComb (HiVis_Attr_Comb read req notify)
   // out (HiVis_Grid write)
  
   /***********************/
   /* Function's Body     */
   /***********************/
  
  visMsgCall << "HiVis_HiVisMods_AG_SparseGrid::"
	     << "upd: called."
	     << visMsgEnd;
  
  
  // Get SparseGridAccess
  
  visSparseGrid* fieldAccess = (visSparseGrid*)
     this->ret_class_ptr("visSparseGrid");
   
   if (!fieldAccess) {
     visMsgError << "HiVis_HiVisMods_AG_SparseGrid::"
		 << "upd: fieldAccess is uninitialized"
		 << visMsgEnd;
     return 0;
   }
   
   // Set pointer to this in out.fieldAccess
   
   visGridLink *outFieldAccessLink = 
     (visGridLink *) out.ret_class_ptr("visGridLink");
   
   if (!outFieldAccessLink) {
     visMsgError << "HiVis_HiVisMods_Field_Generate_SparseGrid::"
		 << "upd: out.gridLink is uninitialized"
		 << visMsgEnd;
     return 0;
   }

   outFieldAccessLink->myFieldAccess = fieldAccess;

   // Get pointer to in.fieldAccess

   visGridLink *inFieldAccessLink = 
     (visGridLink *) in.ret_class_ptr("visGridLink");
   
   if (!inFieldAccessLink) {
     visMsgError << "HiVis_HiVisMods_Generate_SparseGrid::"
		 << "upd: in.gridLink is uninitialized"
		 << visMsgEnd;
     return 0;
   }

   visAdaptiveGrid *inFieldAccess = inFieldAccessLink->castToAdaptiveGrid();

   if (!inFieldAccess) {
     visMsgError << "HiVis_HiVisMods_Generate_SparseGrid::"
		 << "upd: in.fieldAccess is uninitialized"
		 << visMsgEnd;
     return 0;
   }

   // Set level to max if not defined

   if (!level.valid_obj()) {
     level = 0x7fffffff;
   }

   // Allocate attribute combiner

   visAttrComb *attrComb;
   if (attr_Comb.valid_obj()) {
     if (((int) attr_Comb.id) == visAttrCombTypeDefault) {
       visMsgDbg << "allocating comb default" << visMsgEnd;
       attrComb = new visAttrCombDefault();
     } else if (((int) attr_Comb.id) == visAttrCombTypeEpsilon) {
       visMsgDbg << "allocating comb epsilon" << visMsgEnd;
       attrComb = new visAttrCombEpsilon((int) attr_Comb.attribute,
					 (double) attr_Comb.epsilon,
					 (int) attr_Comb.epsCalculateBy,
					 (int) attr_Comb.epsApplyOn);
     }
   } else {
     visMsgDbg << "allocating none" << visMsgEnd;
     attrComb = NULL;
   }
   
   // Perform the conversion
   
   if (!fieldAccess->allocate(inFieldAccess, 
			      attrComb,
			      (int) level,
			      (int) maxLdScopeDiff)) {
     return 0;
   }
   
   // Update out parameters

   maxLevel = fieldAccess->myMaxLevel;

   // Sign change

   if (!out.hasChanged.valid_obj()) {
     out.hasChanged = 0;
   }

   int hasChanged = (int) out.hasChanged; out.hasChanged = ++hasChanged;

   // return 1 for success

   return 1;
}

int HiVis_HiVisMods_HiVis_To_Field::upd(OMevent_mask, int)
{
  // in (HiVis_Grid read req notify)
  // gridInterp (HiVis_Grid_Interp read notify)
  // out (Grid_Struct+Node_Data write)
  
  // out.nnodes (int)
  // out.nnode_data (int)
  
  /*   int  out_data_comp;
       int  out_data_size, out_data_type;
       for (out_data_comp = 0; out_data_comp < out.nnode_data; out_data_comp++) { 
       // out.node_data[out_data_comp].veclen (int) 
       // out.node_data[out_data_comp].values (char [])
       char *out_node_data = (char *)out.node_data[out_data_comp].values.ret_array_ptr(OM_GET_ARRAY_WR, &out_data_size, &out_data_type);
       if (out_node_data)
       ARRfree((char *)out_node_data);
       }
  */
  
  
  /***********************/
  /* Function's Body     */
  /***********************/

  visMsgCall << "HiVis_HiVisMods_HiVis_To_Field::"
	     << "upd: called."
	     << visMsgEnd;

    // Get access to visAdaptiveGrid

  visGridLink *gridLink = 
     (visGridLink *) in.ret_class_ptr("visGridLink");

  if (!gridLink) {
    visMsgError << "HiVis_HiVisMods_HiVis_To_Field::"
		<< "upd: in.gridLink is uninitialized" 
		<< visMsgErrorEnd;
    return 0;
  }

  visAdaptiveGrid *fieldAccess = gridLink->castToAdaptiveGrid();

  if (!fieldAccess || !fieldAccess->isAllocated()) {
    visMsgError << "HiVis_HiVisMods_HiVis_To_Field::"
		<< "upd: in.fieldAccess is uninitialized."
		<< visMsgErrorEnd;
    return 0;
  }

  visAdaptiveGrid *nodeGrid = fieldAccess;
  if (!(nodeGrid->getAttrAssoc() & visHasNodeData)) {
    visMsgError << "HiVis_HiVisMods_HiVis_To_Field::"
		<< "upd: only node associated fields supported"
		<< visMsgErrorEnd;
    return 0;
  }
  
  int i, j;

  // Init grid type and 

  out.grid_type = visAVS_Grid_Unif;
  out.ndim = nodeGrid->getDim();
  out.nspace = nodeGrid->getDim();

  // Init field dimension

  int *dims = (int *) out.dims.ret_array_ptr(OM_GET_ARRAY_WR);
  for (i = 0; i < nodeGrid->getDim(); i++) {
    dims[i] = nodeGrid->getSize(i) + 1;
  }
  if (dims) ARRfree((void *) dims);

  // Init field extends

  out.npoints = 2;
  float *points = (float *) out.points.ret_array_ptr(OM_GET_ARRAY_WR);
  for (i = 0; i < nodeGrid->getDim(); i++) {  
    points[i] = 0;
    points[i + nodeGrid->getDim()] = nodeGrid->getSize(i) + 1;
  }
  if (points) ARRfree((void *) points);

  // init attribute information

  out.nnode_data = nodeGrid->getAttributes();
  visAttribute *a;
  for (i = 0; i < nodeGrid->getAttributes(); i++) {
    out.node_data[i].labels = nodeGrid->getAttrName(i);
    a = (visAttribute *) nodeGrid->getMinAttribute(i);
    out.node_data[i].min = a->getScalarValue(nodeGrid->getDim());
    a->decRef();
    a = (visAttribute *) nodeGrid->getMaxAttribute(i);
    out.node_data[i].max = a->getScalarValue(nodeGrid->getDim());
    a->decRef();
  }

  // init value buffer (for more efficient access)
  
  int *inc = new int[nodeGrid->getAttributes()];
  //double** val = new double*[nodeGrid->getAttributes()];
  int *valDim = new int[nodeGrid->getAttributes()];
  visAttribute **attr = new visAttribute*[nodeGrid->getAttributes()];
  for (i = 0; i < nodeGrid->getAttributes(); i++) {
    visAttributeFactory* factory = nodeGrid->getAttrFactory(i);
    valDim[i] = factory->getDim();
    //val[i] = new double[valDim[i]];
    attr[i] = factory->getNewAttribute();
    factory->decRef();
  }

  // Init size (nnodes) and increment table

  int size = 1;
  for (i = 0; i < nodeGrid->getDim(); i++) {
    inc[i] = size;
    size *= nodeGrid->getSize(i) + 1;
  }
  out.nnodes = size;
  
  // Get access to node data

  double **data = new double*[nodeGrid->getAttributes()];
  int comp;
  for (comp = 0; comp < nodeGrid->getAttributes(); comp++) {
    out.node_data[comp].veclen = valDim[comp];
    data[comp] = (double *) out.node_data[comp].values.
      ret_typed_array_ptr(OM_GET_ARRAY_WR, OM_TYPE_DOUBLE);
  }

  float* pos = new float[nodeGrid->getDim()];
  for (i = 0; i < nodeGrid->getDim(); i++) {
    pos[i] = 0.0f;
  }

  int attributeCount = nodeGrid->getAttributes();
  for (i = 0; i < size; i++) {
    visSYSTEM.setStatus("HiVis To Field", (float) i / size);
    nodeGrid->setCursorF(pos);
    for (comp = 0; comp < nodeGrid->getAttributes(); comp++) {
      nodeGrid->getCellAttributeF(comp, attr[comp], pos);
      for (j = 0; j < valDim[comp]; j++) {
	data[comp][i * valDim[comp] + j] = attr[comp]->getValue(j);
      }
    }
    for (j = 0; j < nodeGrid->getDim(); j++) {
      if (++pos[j] < nodeGrid->getSize(j) + 1) {
	break;
      }
      pos[j] = 0;
    }
  }

  delete[] pos;
  delete[] valDim;
  for (i = 0; i < nodeGrid->getAttributes(); i++) {
    attr[i]->decRef();
  }
  delete[] attr;
  for (comp = 0; comp < nodeGrid->getAttributes(); comp++) {
    ARRfree((char *)data[comp]);
  }
  delete[] data;
  delete[] inc;
  
  // Copy attribute type id
  
  int *dataID = (int *) in.attributeDataID.ret_array_ptr(OM_GET_ARRAY_RD);
  for (i = 0; i < fieldAccess->getAttributes(); i++) {
    if (dataID && dataID[i] != 0) {
      out.node_data[i].id = dataID[i];
    } else {
      out.node_data[i].id.set_obj_val(OMnull_obj);
    }
  }
  if (dataID) ARRfree((void *) dataID);

  visSETSTATUSIDLE
  
  // return 1 for success

  return 1;
}

#define visRenderCellsEdges      0
#define visRenderCellsFaces      1

int HiVis_HiVisMods_HiVis_Render_Cells::upd(OMevent_mask, int)
{
   // in (HiVis_Grid read req notify)
   // out (Field write)

  visMsgCall << "HiVis_HiVisMods_HiVis_Render_Cells::"
	     << "upd: called." << visMsgEnd;
  
  // Get access to visAdaptiveGrid

  visGridLink *gridLink = 
     (visGridLink *) in.ret_class_ptr("visGridLink");

  if (!gridLink) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Cells::"
		<< "upd: in.gridLink is uninitialized" 
		<< visMsgErrorEnd;
    return 0;
  }

  visAdaptiveGrid *fieldAccess = gridLink->castToAdaptiveGrid();

  if (!fieldAccess) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Cells::"
		<< "upd: in.fieldAccess is uninitialized."
		<< visMsgErrorEnd;
    return 0;
   }

  if (!fieldAccess->isAllocated()) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Cells::"
		<< "upd: in.fieldAccess is unallocated."
		<< visMsgErrorEnd;
    return 0;
  }
  
  // handle node association

  int nodeAssoc = fieldAccess->getAttrAssoc();

  // Get field dimension

  int dim = fieldAccess->getDim();

  out.nspace = dim;

  // Sanity checks

  if (dim < 1 || dim > 3) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Cells::"
		  << "upd: Only one, two or three dimensional fields "
		  << "supported."
		  << visMsgErrorEnd;
     return 0;
  }

  // Init AVS/Express field structure
  
  int i;

  // Map mix and max arrays

  double *minRange = NULL;
  double *maxRange = NULL;
  int theRanges = ranges;
  if (theRanges > 0) {
    minRange = (double *) rangeMin.ret_array_ptr(OM_GET_ARRAY_RD);
    maxRange = (double *) rangeMax.ret_array_ptr(OM_GET_ARRAY_RD);
    if (!minRange || !maxRange) {
      visMsgError << "Couldn't map ranges array - ignoring ranges!" <<
	visMsgErrorEnd;
      theRanges = 0;
    } else {
      for (i = 0; i < theRanges; i++) {
	visMsgDbg << "min: " << minRange[i] << " max: " << maxRange[i] 
		  << visMsgEnd;
      }
    }
  }

  unsigned int colorAttribute = (int) attribute;
  if (colorAttribute >= fieldAccess->getAttributes()) {
    colorAttribute = 0;
  }
  unsigned int rangeAttrN = (int) rangeAttribute;
  if (rangeAttrN >= fieldAccess->getAttributes()) {
    rangeAttrN = 0;
  }

  // Allocate color Attribute and get dimension

  visAttribute* attr;
  int attrDim;
  {
    visAttributeFactory *factory = 
      fieldAccess->getAttrFactory(colorAttribute);
    attr = factory->getNewAttribute();
    attrDim = factory->getDim();
    factory->decRef();
  }

  // Allocate range Attribute and get dimension

  visAttribute* rangeAttr;
  int rangeAttrDim;
  {
    visAttributeFactory *factory =
      fieldAccess->getAttrFactory(rangeAttrN);
    rangeAttr = factory->getNewAttribute();
    rangeAttrDim = factory->getDim();
    factory->decRef();
  }

  // Get number of cells that are in range
  
  visAttributeEnumeration *e = new visAdaptiveGridCellEnumeration(fieldAccess);
  int numberOfCells;
  if (theRanges > 0) {
    e->setCursorFirstElement();
    numberOfCells = 0;
    while (e->isValidElement()) {
      fieldAccess->getCellAttribute(rangeAttrN, rangeAttr);
      double x = rangeAttr->getScalarValue(rangeAttrDim);
      int doDrawCell;
      if (theRanges > 0) {
	doDrawCell = 0;
	for (i = 0; i < theRanges; i++) {
	  if (minRange[i] <= x && x <= maxRange[i]) {
	    doDrawCell = 1;
	    break;
	  }
	}
      } else {
	doDrawCell = 1;
      }
      if (doDrawCell) numberOfCells++;
      e->setCursorNextElement();
    }
  } else {
    numberOfCells = fieldAccess->getCells();
  }

  visMsgDbg << "numberOfCells = " 
	    << numberOfCells
	    << visMsgEnd;

  out.nnodes = (1 << dim) * numberOfCells;

  int theRenderType = this->renderType;
  if (theRenderType == visRenderCellsFaces && dim == 1) {
    visMsgDbg
      << "Switching back from faces to vertices in one dimensional field" 
      << visMsgEnd;
    theRenderType = visRenderCellsEdges;
  }
  switch (theRenderType) {
  case visRenderCellsEdges:
    out.ncell_sets = 1;
    out.cell_set[0].cell_nnodes = 2;
    out.cell_set[0].cell_ndim = 1;
    out.cell_set[0].cell_order = 1;
    switch (dim) {
    case 1:
      out.cell_set[0].ncells = 1 * numberOfCells;
      break;
    case 2:
      out.cell_set[0].ncells = 4 * numberOfCells;;
      break;
    case 3:
      out.cell_set[0].ncells = 12 * numberOfCells;;
      break;
    }
    break;
  case visRenderCellsFaces:
    out.ncell_sets = 1;
    out.cell_set[0].cell_nnodes = 4;
    out.cell_set[0].cell_ndim = 2;
    out.cell_set[0].cell_order = 1;
    switch (dim) {
    case 2:
      out.cell_set[0].ncells = 1 * numberOfCells;;
      break;
    case 3:
      out.cell_set[0].ncells = 6 * numberOfCells;
      break;
    }
    break;
  default:
     visMsgError << "HiVis_HiVisMods_HiVis_Render_Cells::"
		 << "unknown cell render type."
		 << visMsgErrorEnd;
     return 0;
  }    

  out.cell_set[0].poly_flag = 0;
  if (numberOfCells > 0) { 

  double minVal = 0, maxVal = 0;

  int *conOut = (int *)
    out.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
  
  // out.cell_set[out_set].ncells (int)
  // out.cell_set[out_set]. cell_nnodes (int)
  // out.cell_set[out_set]. cell_ndim (int)
  // out.cell_set[out_set]. cell_order (int)
  // out.cell_set[out_set]. cell_name (char *)
  // out.cell_set[out_set].node_connect_list (int *)
  

  // out.nspace (int) 
  // out.nnodes (int)
  // out.coordinates.values (float [])

  // out.nnode_data (int)
  // out.node_data[out_data_comp].veclen (int) 
  // out.node_data[out_data_comp].values (char [])

  float *cOut = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);

  // Check if data is scalar or of other type directly supported by the renderer

  bool doConvert;
  {
    out.nnode_data = 1;
    int *dataID = (int *) in.attributeDataID.ret_array_ptr(OM_GET_ARRAY_RD);
    
    if (dataID && dataID[colorAttribute] != 0) {
      out.node_data[0].id = dataID[colorAttribute];
      out.node_data[0].veclen = attrDim;
      doConvert = false;
    } else {
      out.node_data[0].id.set_obj_val(OMnull_obj);
      out.node_data[0].veclen = 1;
      doConvert = true;
    }
    if (dataID) ARRfree((void *) dataID);
  }

  // Define node_data
  
  float *nOut = (float*) out.node_data[0].values.
    ret_typed_array_ptr(OM_GET_ARRAY_WR, OM_TYPE_FLOAT);

  float pd = ipad;
  if (pd < 0) { pd = 0; } else if (pd > 1) { pd = 1; }
  pd /= 2;
  float PD[3], nPD[3];

  // Initialize min and max value
  
  visAttribute *minAttr = (visAttribute *) 
    fieldAccess->getMinAttribute(colorAttribute);
  visAttribute *maxAttr = (visAttribute *) 
    fieldAccess->getMaxAttribute(colorAttribute);
  minVal = minAttr->getScalarValue(attrDim);
  maxVal = maxAttr->getScalarValue(attrDim);

  visMsgDbg << "min attribute = " << minVal << ", max attribute = " << maxVal 
	    << visMsgEnd;
  minAttr->decRef();
  maxAttr->decRef();

  int cellCount = 0;

  int cIndex = 0;
  int conIndex = 0;
  
  int rIPos[3], rISize[3];
  float rPos[3], rSize[3];
  
  // iterate through values
  
  e->setCursorFirstElement();
  while (e->isValidElement()) {
    
    // count cells
    
    cellCount++;
    
    // update percentage status
    
    visSYSTEM.setStatus("HiVis Render Cells",
			(float) cellCount / numberOfCells);
    
    fieldAccess->getCellPos(rIPos);
    fieldAccess->getCellSize(rISize);

    // crop outer half cell boundary

    for (i = 0; i < dim; i++) {
      rSize[i] = rISize[i];
      if (cropBorder && rIPos[i] == 0) {
	rPos[i] = 0;
	rSize[i] -= 0.5;
	PD[i] = 0;
      } else {
	//rPos[i] =  - 0.5 + rIPos[i];
	rPos[i] =  rIPos[i];
	PD[i] = pd;
      }
      nPD[i] = pd;
      if (cropBorder && rIPos[i] + rISize[i] >= fieldAccess->getSize(i)) {
	rSize[i] -= 0.5;
	nPD[i] = 0;
      }
    }

    // Get range color attribute and check if it is in a valid range

    fieldAccess->getCellAttribute(rangeAttrN, rangeAttr);
    double x = rangeAttr->getScalarValue(rangeAttrDim);
    int doDrawCell;
    if (theRanges > 0) {
      doDrawCell = 0;
      for (i = 0; i < theRanges; i++) {
	if (minRange[i] <= x && x <= maxRange[i]) {
	  doDrawCell = 1;
	  break;
	}
      }
    } else {
      doDrawCell = 1;
    }

    // Get color attribute

    if (doDrawCell) switch (dim) {
      
    case 1:
      cOut[(cIndex+0) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+1) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      
      if (nodeAssoc == visHasNodeData) {
	if (doConvert) {
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 0);
	  nOut[(cIndex+0)] = attr->getScalarValue(attrDim);
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 1);
	  nOut[(cIndex+1)] = attr->getScalarValue(attrDim);
	} else {
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 0);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+0) * attrDim + i] = attr->getValue(i);
	  }
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 1);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+1) * attrDim + i] = attr->getValue(i);
	  }
	}
      } else if (nodeAssoc == visHasCellData) {
	fieldAccess->getCellAttribute(colorAttribute, attr);
	if (doConvert) {
	  nOut[(cIndex+0)] = attr->getScalarValue(attrDim);
	  nOut[(cIndex+1)] = attr->getScalarValue(attrDim);
	} else {
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+0) * attrDim + i] = attr->getValue(i);
	    nOut[(cIndex+1) * attrDim + i] = attr->getValue(i);
	  }
	}
      }
      
      conOut[(conIndex + 0 ) * 2 + 0] = cIndex + 0;
      conOut[(conIndex + 0 ) * 2 + 1] = cIndex + 1;
      
      cIndex += 2; conIndex += 1;
      
      break;
      
    case 2:
      cOut[(cIndex+0) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+0) * dim + visY] = rPos[visY] +  PD[visY];
      
      cOut[(cIndex+1) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      cOut[(cIndex+1) * dim + visY] = rPos[visY] +  PD[visY];
      
      cOut[(cIndex+2) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+2) * dim + visY] = rPos[visY] - nPD[visY] + rSize[visY];
      
      cOut[(cIndex+3) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      cOut[(cIndex+3) * dim + visY] = rPos[visY] - nPD[visY] + rSize[visY];
      
      if (nodeAssoc == visHasNodeData) {
	if (doConvert) {
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 0);
	  nOut[(cIndex+0)] = attr->getScalarValue(attrDim);
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 1);
	  nOut[(cIndex+1)] = attr->getScalarValue(attrDim);
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 2);
	  nOut[(cIndex+2)] = attr->getScalarValue(attrDim);
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 3);
	  nOut[(cIndex+3)] = attr->getScalarValue(attrDim);
	} else {
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 0);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+0) * attrDim + i] = attr->getValue(i);
	  }
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 1);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+1) * attrDim + i] = attr->getValue(i);
	  }
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 2);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+2) * attrDim + i] = attr->getValue(i);
	  }
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 3);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+3) * attrDim + i] = attr->getValue(i);
	  }
	}
      } else if (nodeAssoc == visHasCellData) {
	fieldAccess->getCellAttribute(colorAttribute, attr);
	if (doConvert) {
	  nOut[(cIndex+0)] = attr->getScalarValue(attrDim);
	  nOut[(cIndex+1)] = attr->getScalarValue(attrDim);
	  nOut[(cIndex+2)] = attr->getScalarValue(attrDim);
	  nOut[(cIndex+3)] = attr->getScalarValue(attrDim);
	} else {
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+0) * attrDim + i] = attr->getValue(i);
	    nOut[(cIndex+1) * attrDim + i] = attr->getValue(i);
	  nOut[(cIndex+2) * attrDim + i] = attr->getValue(i);
	  nOut[(cIndex+3) * attrDim + i] = attr->getValue(i);
	  }
	}
      }

      switch (theRenderType) {
      case visRenderCellsEdges:

	conOut[(conIndex + 0 ) * 2 + 0] = cIndex+ 0;
	conOut[(conIndex + 0 ) * 2 + 1] = cIndex+ 1;
	conOut[(conIndex + 1 ) * 2 + 0] = cIndex+ 1;
	conOut[(conIndex + 1 ) * 2 + 1] = cIndex+ 3;
	conOut[(conIndex + 2 ) * 2 + 0] = cIndex+ 3;
	conOut[(conIndex + 2 ) * 2 + 1] = cIndex+ 2;
	conOut[(conIndex + 3 ) * 2 + 0] = cIndex+ 2;
	conOut[(conIndex + 3 ) * 2 + 1] = cIndex+ 0;
	cIndex += 4; conIndex += 4;
	break;
      case visRenderCellsFaces:
	conOut[(conIndex + 0 ) * 4 + 0] = cIndex+ 0;
	conOut[(conIndex + 0 ) * 4 + 1] = cIndex+ 1;
	conOut[(conIndex + 0 ) * 4 + 2] = cIndex+ 3;
	conOut[(conIndex + 0 ) * 4 + 3] = cIndex+ 2;
	cIndex += 4; conIndex += 1;
	break;
      }
      break;

    case 3:
      cOut[(cIndex+0) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+0) * dim + visY] = rPos[visY] +  PD[visY];
      cOut[(cIndex+0) * dim + visZ] = rPos[visZ] +  PD[visZ];
      
      cOut[(cIndex+1) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      cOut[(cIndex+1) * dim + visY] = rPos[visY] +  PD[visY];
      cOut[(cIndex+1) * dim + visZ] = rPos[visZ] +  PD[visZ];
      
      cOut[(cIndex+2) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+2) * dim + visY] = rPos[visY] - nPD[visY] + rSize[visY];
      cOut[(cIndex+2) * dim + visZ] = rPos[visZ] +  PD[visZ];
      
      cOut[(cIndex+3) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      cOut[(cIndex+3) * dim + visY] = rPos[visY] - nPD[visY] + rSize[visY];
      cOut[(cIndex+3) * dim + visZ] = rPos[visZ] +  PD[visZ];
      
      cOut[(cIndex+4) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+4) * dim + visY] = rPos[visY] +  PD[visY];
      cOut[(cIndex+4) * dim + visZ] = rPos[visZ] - nPD[visZ] + rSize[visZ];
      
      cOut[(cIndex+5) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      cOut[(cIndex+5) * dim + visY] = rPos[visY] +  PD[visY];
      cOut[(cIndex+5) * dim + visZ] = rPos[visZ] - nPD[visZ] + rSize[visZ];
      
      cOut[(cIndex+6) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+6) * dim + visY] = rPos[visY] - nPD[visY] + rSize[visY];
      cOut[(cIndex+6) * dim + visZ] = rPos[visZ] - nPD[visZ] + rSize[visZ];
      
      cOut[(cIndex+7) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      cOut[(cIndex+7) * dim + visY] = rPos[visY] - nPD[visY] + rSize[visY];
      cOut[(cIndex+7) * dim + visZ] = rPos[visZ] - nPD[visZ] + rSize[visZ];
      
      if (nodeAssoc == visHasNodeData) {
	if (doConvert) {
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 0);
	  nOut[(cIndex+0)] = attr->getScalarValue(attrDim);
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 1);
	  nOut[(cIndex+1)] = attr->getScalarValue(attrDim);
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 2);
	  nOut[(cIndex+2)] = attr->getScalarValue(attrDim);
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 3);
	  nOut[(cIndex+3)] = attr->getScalarValue(attrDim);
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 4);
	  nOut[(cIndex+4)] = attr->getScalarValue(attrDim);
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 5);
	  nOut[(cIndex+5)] = attr->getScalarValue(attrDim);
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 6);
	  nOut[(cIndex+6)] = attr->getScalarValue(attrDim);
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 7);
	  nOut[(cIndex+7)] = attr->getScalarValue(attrDim);
	} else {
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 0);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+0) * attrDim + i] = attr->getValue(i);
	  }
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 1);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+1) * attrDim + i] = attr->getValue(i);
	  }
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 2);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+2) * attrDim + i] = attr->getValue(i);
	  }
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 3);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+3) * attrDim + i] = attr->getValue(i);
	  }
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 4);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+4) * attrDim + i] = attr->getValue(i);
	  }
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 5);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+5) * attrDim + i] = attr->getValue(i);
	  }
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 6);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+6) * attrDim + i] = attr->getValue(i);
	  }
	  fieldAccess->getNodeAttribute(colorAttribute, attr, 7);
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+7) * attrDim + i] = attr->getValue(i);
	  }
	}
      } else if (nodeAssoc == visHasCellData) {
	fieldAccess->getCellAttribute(colorAttribute, attr);
	if (doConvert) {
	  nOut[(cIndex+0)] = attr->getScalarValue(attrDim);
	  nOut[(cIndex+1)] = attr->getScalarValue(attrDim);
	  nOut[(cIndex+2)] = attr->getScalarValue(attrDim);
	  nOut[(cIndex+3)] = attr->getScalarValue(attrDim);
	  nOut[(cIndex+4)] = attr->getScalarValue(attrDim);
	  nOut[(cIndex+5)] = attr->getScalarValue(attrDim);
	  nOut[(cIndex+6)] = attr->getScalarValue(attrDim);
	  nOut[(cIndex+7)] = attr->getScalarValue(attrDim);
	} else {
	  for (i = 0; i < attrDim; i++) {
	    nOut[(cIndex+0) * attrDim + i] = attr->getValue(i);
	    nOut[(cIndex+1) * attrDim + i] = attr->getValue(i);
	    nOut[(cIndex+2) * attrDim + i] = attr->getValue(i);
	    nOut[(cIndex+3) * attrDim + i] = attr->getValue(i);
	    nOut[(cIndex+4) * attrDim + i] = attr->getValue(i);
	    nOut[(cIndex+5) * attrDim + i] = attr->getValue(i);
	    nOut[(cIndex+6) * attrDim + i] = attr->getValue(i);
	    nOut[(cIndex+7) * attrDim + i] = attr->getValue(i);
	  }
	}
      }

      switch (theRenderType) {
      case visRenderCellsEdges:

	conOut[(conIndex + 0 ) * 2 + 0] = cIndex+ 0;
	conOut[(conIndex + 0 ) * 2 + 1] = cIndex+ 1;
	conOut[(conIndex + 1 ) * 2 + 0] = cIndex+ 1;
	conOut[(conIndex + 1 ) * 2 + 1] = cIndex+ 3;
	conOut[(conIndex + 2 ) * 2 + 0] = cIndex+ 3;
	conOut[(conIndex + 2 ) * 2 + 1] = cIndex+ 2;
	conOut[(conIndex + 3 ) * 2 + 0] = cIndex+ 2;
	conOut[(conIndex + 3 ) * 2 + 1] = cIndex+ 0;
	  
	conOut[(conIndex + 4 ) * 2 + 0] = cIndex+ 4;
	conOut[(conIndex + 4 ) * 2 + 1] = cIndex+ 5;
	conOut[(conIndex + 5 ) * 2 + 0] = cIndex+ 5;
	conOut[(conIndex + 5 ) * 2 + 1] = cIndex+ 7;
	conOut[(conIndex + 6 ) * 2 + 0] = cIndex+ 7;
	conOut[(conIndex + 6 ) * 2 + 1] = cIndex+ 6;
	conOut[(conIndex + 7 ) * 2 + 0] = cIndex+ 6;
	conOut[(conIndex + 7 ) * 2 + 1] = cIndex+ 4;
      
	conOut[(conIndex + 8 ) * 2 + 0] = cIndex+ 0;
	conOut[(conIndex + 8 ) * 2 + 1] = cIndex+ 4;
	conOut[(conIndex + 9 ) * 2 + 0] = cIndex+ 1;
	conOut[(conIndex + 9 ) * 2 + 1] = cIndex+ 5;
	conOut[(conIndex + 10) * 2 + 0] = cIndex+ 2;
	conOut[(conIndex + 10) * 2 + 1] = cIndex+ 6;
	conOut[(conIndex + 11) * 2 + 0] = cIndex+ 3;
	conOut[(conIndex + 11) * 2 + 1] = cIndex+ 7;
	cIndex += 8; conIndex += 12;
	
	break;
      case visRenderCellsFaces:
	conOut[(conIndex + 0 ) * 4 + 0] = cIndex+ 2;
	conOut[(conIndex + 0 ) * 4 + 1] = cIndex+ 3;
	conOut[(conIndex + 0 ) * 4 + 2] = cIndex+ 1;
	conOut[(conIndex + 0 ) * 4 + 3] = cIndex+ 0;
	
	conOut[(conIndex + 1 ) * 4 + 0] = cIndex+ 1;
	conOut[(conIndex + 1 ) * 4 + 1] = cIndex+ 5;
	conOut[(conIndex + 1 ) * 4 + 2] = cIndex+ 4;
	conOut[(conIndex + 1 ) * 4 + 3] = cIndex+ 0;
	
	conOut[(conIndex + 2 ) * 4 + 0] = cIndex+ 4;
	conOut[(conIndex + 2 ) * 4 + 1] = cIndex+ 6;
	conOut[(conIndex + 2 ) * 4 + 2] = cIndex+ 2;
	conOut[(conIndex + 2 ) * 4 + 3] = cIndex+ 0;
	
	conOut[(conIndex + 3 ) * 4 + 0] = cIndex+ 7;
	conOut[(conIndex + 3 ) * 4 + 1] = cIndex+ 5;
	conOut[(conIndex + 3 ) * 4 + 2] = cIndex+ 1;
	conOut[(conIndex + 3 ) * 4 + 3] = cIndex+ 3;
	
	conOut[(conIndex + 4 ) * 4 + 0] = cIndex+ 6;
	conOut[(conIndex + 4 ) * 4 + 1] = cIndex+ 4;
	conOut[(conIndex + 4 ) * 4 + 2] = cIndex+ 5;
	conOut[(conIndex + 4 ) * 4 + 3] = cIndex+ 7;
	
	conOut[(conIndex + 5 ) * 4 + 0] = cIndex+ 6;
	conOut[(conIndex + 5 ) * 4 + 1] = cIndex+ 7;
	conOut[(conIndex + 5 ) * 4 + 2] = cIndex+ 3;
	conOut[(conIndex + 5 ) * 4 + 3] = cIndex+ 2;
	
	cIndex += 8; conIndex += 6;
	
	break;
      }
      break;
    }
    e->setCursorNextElement();
  }
  if (conOut)
    ARRfree((char *)conOut);

  if (nOut)
    ARRfree((char *)nOut);
  
  if (cOut)
    ARRfree((char *)cOut);
  
  // Set min and max value;
  
  OMset_data_type((OMobj_id)out.node_data[0].min,OM_TYPE_FLOAT);
  OMset_data_type((OMobj_id)out.node_data[0].max,OM_TYPE_FLOAT);
  out.node_data[0].min = minVal;
  out.node_data[0].max = maxVal;

  // Copy attribute type id
  
  int *dataID = (int *) in.attributeDataID.ret_array_ptr(OM_GET_ARRAY_RD);
  if (dataID && dataID[colorAttribute] != 0) {
    out.node_data[0].id = dataID[colorAttribute];
  } else {
    out.node_data[0].id.set_obj_val(OMnull_obj);
  }
  if (dataID) ARRfree((void *) dataID);

  }
  visMsgDbg << "e-1 " << fieldAccess->getNumberOfRefs() << visMsgEnd;
  
  //delete e;
  e->decRef();

  visMsgDbg << "e-2 " << fieldAccess->getNumberOfRefs() << visMsgEnd;
  // Unmap mix and max arrays

  if (minRange) ARRfree((char *)minRange);
  if (maxRange) ARRfree((char *)maxRange);

  visSETSTATUSIDLE;

  // return 1 for success
  visMsgDbg << "e-3 " << fieldAccess->getNumberOfRefs() << visMsgEnd;

  return 1;
}


int HiVis_HiVisMods_AG_OrthoSlice::upd(OMevent_mask, int)
{
  // in (HiVis_Grid read req notify)
  // axis (OMXint read req notify)
  // pos (OMXint read req notify)
  // out (HiVis_Grid write)

   /***********************/
   /* Function's Body     */
   /***********************/
  visMsgCall << "HiVis_HiVisMods_AG_OrthoSlice::"
	     << "upd: called." << visMsgEnd;
  
  
  // Get access to visAdaptiveGrid

  visGridLink *gridLink = 
     (visGridLink *) in.ret_class_ptr("visGridLink");

  if (!gridLink) {
    visMsgError << "HiVis_HiVisMods_AG_OrthoSlice::"
		<< "upd: in.gridLink is uninitialized" 
		<< visMsgErrorEnd;
    return 0;
  }

  visAdaptiveGrid *fieldAccess = gridLink->castToAdaptiveGrid();

  if (!fieldAccess) {
    visMsgError << "HiVis_HiVisMods_AG_OrthoSlice::"
		<< "upd: in.fieldAccess is uninitialized."
		<< visMsgErrorEnd;
    return 0;
   }

  // Get visAdaptiveGridSlice
  
  visAdaptiveGridSlice* fieldSliceAccess = (visAdaptiveGridSlice*)
    this->ret_class_ptr("visAdaptiveGridSlice");
  
  if (!fieldSliceAccess) {
    visMsgError << "HiVis_HiVisMods_AG_OrthoSlice::"
		<< "upd: fieldSliceAccess is uninitialized"
		<< visMsgErrorEnd;
    return 0;
  }
   
  // Set pointer to visAdaptiveGridSlice in out.fieldAccess
   
  visGridLink *outFieldAccessLink = 
    (visGridLink *) out.ret_class_ptr("visGridLink");
   
  if (!outFieldAccessLink) {
    visMsgError << "HiVis_HiVisMods_AG_OrthoSlice::"
		<< "upd: gridLink is uninitialized"
		<< visMsgErrorEnd;
    return 0;
  }

  outFieldAccessLink->myFieldAccess = fieldSliceAccess;

  // Sanity checks

  if (fieldAccess->getDim() <= 1) {
    visMsgError << "HiVis_HiVisMods_AG_OrthoSlice::"
		<< "upd: in.fieldAccess dimension must be greater than one."
		<< visMsgErrorEnd;
     return 0;
  }

  if (int(axis) < 0 || int(axis) >= fieldAccess->getDim()) {
    visMsgError << "HiVis_HiVisMods_AG_OrthoSlice::"
		<< "upd: axis out of bounds."
		<< visMsgErrorEnd;
     return 0;
  }

  if (int(pos) < 0 || int(pos) >= fieldAccess->getSize(axis)) {
    visMsgError << "HiVis_HiVisMods_AG_OrthoSlice::"
		<< "upd: pos out of bounds."
		<< visMsgErrorEnd;
     return 0;
  }

  fieldSliceAccess->allocate(fieldAccess, (int) axis, (int) pos, 
			     (int) adjust_Min_Max);

  // Transform slice if dimension = 3

#define matSize (4*4)
#define PI (3.1415927)


  if (fieldAccess->getDim() == 3) {
    float rotMatrix[3*16] = {
      // Rotate around X by 90 degrees and Z by 90 degrees
      0, 1, 0, 0,  0, 0, 1, 0,  1, 0, 0, 0,  0, 0, 0, 1,
      // Rotate around X by 90 degrees
      1, 0, 0, 0,  0, 0, 1, 0,  0, 1, 0, 0,  0, 0, 0, 1,
      // Identity
      1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1,
    };

    // Translate
    rotMatrix[axis * 16 + 4 * 3 + axis] = pos;
    float *inMat = (float *) in.xform.mat.
      ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_FLOAT);
    float *outMat = (float *) out.xform.mat.
      ret_typed_array_ptr(OM_GET_ARRAY_WR, OM_TYPE_FLOAT);
    MATmat4_multiply((Matr4 *)inMat, (Matr4 *)&rotMatrix[16 * axis],
		     (Matr4 *)outMat);
    ARRfree((char *)inMat);
    ARRfree((char *)outMat);
  } else {

    // Otherwise just copy

    float *inMat = (float *) in.xform.mat.
      ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_FLOAT);
    float *outMat = (float *) out.xform.mat.
      ret_typed_array_ptr(OM_GET_ARRAY_WR, OM_TYPE_FLOAT);
    int i;
    for (i = 0; i < 16; i++) {
      outMat[i] = inMat[i];
    }
    ARRfree((char *)inMat);
    ARRfree((char *)outMat);
  }

  // Sign change

  if (!out.hasChanged.valid_obj()) {
    out.hasChanged = 0;
  }
  
  int hasChanged = (int) out.hasChanged; out.hasChanged = ++hasChanged;

  // return 1 for success

  return 1;
}


#include "../IsoFaces.cpp"

int HiVis_HiVisMods_HiVis_Iso::upd(OMevent_mask, int)
{
   // in (HiVis_Grid read req notify)
   // isoval (OMXfloat read req notify)
   // out (Mesh+Node_Data write)

   // out.nspace (int) 
   // out.nnodes (int)
   // out.coordinates.values (float [])
  /*
   float *out_coordinates = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
   // out.ncell_sets (int)
   int out_set;
   int out_ncell_sets = out.ncell_sets;
   for (out_set=0; out_set < out.ncell_sets; out_set++) {
   	// out.cell_set[out_set].ncells (int)
   	// out.cell_set[out_set]. cell_nnodes (int)
   	// out.cell_set[out_set]. cell_ndim (int)
   	// out.cell_set[out_set]. cell_order (int)
   	// out.cell_set[out_set]. cell_name (char *)
   	// out.cell_set[out_set].node_connect_list (int *)

   	int *out_node_connect_list = (int *)out.cell_set[out_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);

	if (out_node_connect_list)
           ARRfree((char *)out_node_connect_list);
   }


   // out.nnodes (int)
   // out.nnode_data (int)
   int  out_data_comp;
   int  out_data_size, out_data_type;
   for (out_data_comp = 0; out_data_comp < out.nnode_data; out_data_comp++) { 
	// out.node_data[out_data_comp].veclen (int) 
   	// out.node_data[out_data_comp].values (char [])
   	char *out_node_data = (char *)out.node_data[out_data_comp].values.ret_array_ptr(OM_GET_ARRAY_WR, &out_data_size, &out_data_type);
        if (out_node_data)
           ARRfree((char *)out_node_data);
   }

   */
   /***********************/
   /* Function's Body     */
   /***********************/
   // in (HiVis_Grid read req notify)
   // out (Field write)
    visMsgDbg << "b-5" << visMsgEnd;

  visMsgCall << "HiVis_HiVisMods_HiVis_Iso::"
	     << "upd: called." << visMsgEnd;

  /*
    go through all cells
    add faces to faces stack
    allocate express unstructured field with #faces
    copy faces stack to unstructured field
  */
  
  // Get access to visAdaptiveGrid
    visMsgDbg << "b-4" << visMsgEnd;

  visGridLink *gridLink = 
    (visGridLink *) in.ret_class_ptr("visGridLink");
  visMsgDbg << "b-3" << visMsgEnd;

  if (!gridLink) {
    visMsgError << "HiVis_HiVisMods_HiVis_Iso::"
		<< "upd: in.gridLink is uninitialized" 
		<< visMsgErrorEnd;
    return 0;
  }

  visMsgDbg << "b-2.2" << visMsgEnd;
  visAdaptiveGrid *fieldAccess = gridLink->castToAdaptiveGrid();
  visMsgDbg << "b-2" << visMsgEnd;

  if (!fieldAccess || !fieldAccess->isAllocated()) {
    visMsgError << "HiVis_HiVisMods_HiVis_Iso::"
		<< "upd: in.fieldAccess is uninitialized."
		<< visMsgErrorEnd;
    return 0;
  }

  visAdaptiveGrid *nodeGrid = fieldAccess;
  
  // Check if field is cell associated

  if (!(nodeGrid->getAttrAssoc() & visHasNodeData)) {
    visMsgError << "HiVis_HiVisMods_HiVis_Iso::upd"
		<< ": field must contain be node associated data."
		<< visMsgErrorEnd;
    return false;
  }
  
  // get color attribute

  unsigned int colorAttribute = (int) colorattribute;
  if (colorAttribute >= nodeGrid->getAttributes()) {
    colorAttribute = 0;
  }
  visMsgDbg << "b6" << visMsgEnd;

  // get iso attribute

  unsigned int isoAttribute = (int) attribute;
  if (isoAttribute >= nodeGrid->getAttributes()) {
    isoAttribute = 0;
  }
  visMsgDbg << "b7" << visMsgEnd;

  // get iso value

  double isoVal = (double) isoval;

  visAttributeFactory *factory = 
    nodeGrid->getAttrFactory(isoAttribute);
  visAttribute* attr = factory->getNewAttribute();
  int attrDim = factory->getDim();
  factory->decRef();
  
  visAttributeFactory *cFactory = 
    nodeGrid->getAttrFactory(colorAttribute);
  int cAttrDim = cFactory->getDim();
  
  // Get field dimension

  int dim = nodeGrid->getDim();

  out.nspace = dim;

  // Sanity checks

  if (out.nspace < 2 || out.nspace > 3) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Field::"
		<< "upd: Only two or three dimensional fields "
		<< "supported."
		<< visMsgErrorEnd;

    nodeGrid->decRef();
    return 0;
  }

  // Build faces and coordinates stack

  visStack<int> conStack;
  visStack<float> cStack;
  visStack<visAttribute*> nStack;
  int cIndex = 0;
  int conIndex = 0;
  
  visAttributeEnumeration *e = new visAdaptiveGridCellEnumeration(nodeGrid);
  int numberOfCells = nodeGrid->getCells();
  e->setCursorFirstElement();
  int i, j, k;
  double val[8]; // common: 1 << dim, but dim <= 3 !
  float c[3], ct[3]; // common: dim, but dim <= 3 !
  int vertex;
  int *v2Edge, *v2Axis, *pPrim;
  float *v2Offset;

  typedef visKAVLTree<visSGPos, int> visSGPos2Index;
  visSGPos2Index *pos2Index;

  switch (dim) {
  case 2:
    v2Edge = myIsoPrim2DVertex2Edge;
    v2Axis = myIsoPrim2DVertex2Axis;
    v2Offset = myIsoPrim2DVertex2Offset;
    pPrim = myIsoPrim2D;
    pos2Index = new visSGPos2Index[4];
    break;
  case 3:
    v2Edge = myIsoPrim3DVertex2Edge;
    v2Axis = myIsoPrim3DVertex2Axis;
    v2Offset = myIsoPrim3DVertex2Offset;
    pPrim = myIsoPrim3D;
    pos2Index = new visSGPos2Index[12];
    break;
  }
  int swapFaces = (int) invertfaces;

  visSGPos key(dim);

  float cellCount = 0.0f;
  visAttribute *cAttr;
  while (e->isValidElement()) {
    visSYSTEM.setStatus("HiVis Render Iso", cellCount++ / numberOfCells);
    
    for (i = 0; i < dim; i++) {
      c[i] = nodeGrid->getCellPos(i);
    }
    int set = 0;
    for (i = 0; i < (1 << dim); i++) {
      nodeGrid->getNodeAttribute(isoAttribute, attr, i);
      val[i] = attr->getScalarValue(attrDim);
      /*visMsgDbg << "val = " << val[i] << visMsgEnd;
	visMsgDbg << "isoVal = " << isoVal << visMsgEnd;    */
      if (swapFaces) {
	if (isoVal <= val[i]) {
	  set |= 1 << i;
	}
      } else {
	if (isoVal >= val[i]) {
	  set |= 1 << i;
	}
      }
    }
    //set = ((1 << dim) - 1) ^ set;
    set <<= dim + 1;

    /*visMsgDbg << "set = " << set << visMsgEnd;
    visMsgDbg << "Number of primitives = " << pPrim[set] << visMsgEnd;*/
    for (k = set + 1; k < set + 1 + pPrim[set]*dim; k += dim) {
      for (j = 0; j < dim; j++) {
	vertex = pPrim[k + j];
	for (i = 0; i < dim; i++) {
	  ct[i] = c[i] + v2Offset[dim*vertex+i] * nodeGrid->getCellSize(i);
	}
	for (i = 0; i < dim; i++) {
	  key.setValue(i, (int) ct[i]);
	}
	int index;
	if (pos2Index[v2Axis[vertex]].get(key, index)) {
	  // push existing index
	  conStack.push(index);
	} else {
	  ct[v2Axis[vertex]] += (isoVal - val[v2Edge[2*vertex+0]])
	    * nodeGrid->getCellSize(v2Axis[vertex]) / 
	    (val[v2Edge[2*vertex+1]] - val[v2Edge[2*vertex+0]]);
	  for (i = 0; i < dim; i++) {
	    cStack.push(ct[i]);
	  }
	  // push interpolated color attribute
	  cAttr = cFactory->getNewAttribute();
	  nodeGrid->getCellAttributeF(colorAttribute, cAttr, ct);
	  nStack.push(cAttr);
	  // insert new key
	  pos2Index[v2Axis[vertex]].insert(key, cIndex);
	  // push new index
	  conStack.push(cIndex++);	
	}
      }
    }
    e->setCursorNextElement();
  }

  delete[] pos2Index;

  //delete e;
  e->decRef();

  visMsgDbg << "number of primitives = " << conStack.getSize() / dim
	    << visMsgEnd;

  out.nnodes = nStack.getSize();
  out.ncell_sets = 1;
  out.cell_set[0].cell_nnodes = dim;
  out.cell_set[0].cell_ndim = dim - 1;
  out.cell_set[0].cell_order = 1;
  out.cell_set[0].ncells = conStack.getSize() / dim;
  out.cell_set[0].poly_flag = 0;
  
  // Check if data is scalar or of other type directly supported by the renderer

  bool doConvert;
  {
    out.nnode_data = 1;
    int *dataID = (int *) in.attributeDataID.ret_array_ptr(OM_GET_ARRAY_RD);
    
    if (dataID && dataID[colorAttribute] != 0) {
      out.node_data[0].id = dataID[colorAttribute];
      out.node_data[0].veclen = attrDim;
      doConvert = false;
    } else {
      out.node_data[0].id.set_obj_val(OMnull_obj);
      out.node_data[0].veclen = 1;
      doConvert = true;
    }
    if (dataID) ARRfree((void *) dataID);
  }

  // Copy node_data
  
  {
    float *nOut = (float*) out.node_data[0].values.
      ret_typed_array_ptr(OM_GET_ARRAY_WR, OM_TYPE_FLOAT);
    for (i = 0; i < nStack.getSize(); i++) {
      nOut[i] = nStack[i]->getScalarValue(cAttrDim);
      nStack[i]->decRef();
    }
    if (nOut) ARRfree((char *)nOut);
  }    

  // Copy connections
  
  {
    int *conOut = (int *)
      out.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
    for (i = 0; i < conStack.getSize(); i++) {
      conOut[i] = conStack[i];
    }
    if (conOut) ARRfree((char *)conOut);
  }

  // Copy coordinates

  {
    float *cOut = (float *)
      out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
    for (i = 0; i < cStack.getSize(); i++) {
      cOut[i] = cStack[i];
    }
    if (cOut) ARRfree((char *)cOut);
  }
  
  // Initialize min and max value
  
  visAttribute *minAttr = (visAttribute *) 
    nodeGrid->getMinAttribute(colorAttribute);
  visAttribute *maxAttr = (visAttribute *) 
    nodeGrid->getMaxAttribute(colorAttribute);
  double minVal = minAttr->getScalarValue(cAttrDim);
  double maxVal = maxAttr->getScalarValue(cAttrDim);

  visMsgDbg << "min attribute = " << minVal << ", max attribute = " << maxVal 
	    << visMsgEnd;
  minAttr->decRef();
  maxAttr->decRef();

  /*   if (out_coordinates)
      ARRfree((char *)out_coordinates);*/

  cFactory->decRef();

  // Set min and max value;
  
  OMset_data_type((OMobj_id)out.node_data[0].min,OM_TYPE_FLOAT);
  OMset_data_type((OMobj_id)out.node_data[0].max,OM_TYPE_FLOAT);
  out.node_data[0].min = minVal;
  out.node_data[0].max = maxVal;

  // Copy attribute type id
  
  int *dataID = (int *) in.attributeDataID.ret_array_ptr(OM_GET_ARRAY_RD);
  if (dataID && dataID[colorAttribute] != 0) {
    out.node_data[0].id = dataID[colorAttribute];
  } else {
    out.node_data[0].id.set_obj_val(OMnull_obj);
  }
  if (dataID) ARRfree((void *) dataID);
  
  visSETSTATUSIDLE;

  // return 1 for success

  return 1;
}

int HiVis_HiVisMods_HiVis_Render_Field::upd(OMevent_mask, int)
{

   /***********************/
   /* Function's Body     */
   /***********************/

  visMsgCall << "HiVis_HiVisMods_HiVis_Render_Field::"
	     << "upd: called." << visMsgEnd;
  
  // Get access to visAdaptiveGrid

  visGridLink *gridLink = 
     (visGridLink *) in.ret_class_ptr("visGridLink");

  if (!gridLink) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Field::"
		<< "upd: in.gridLink is uninitialized" 
		<< visMsgErrorEnd;
    return 0;
  }

  visAdaptiveGrid *fieldAccess = gridLink->castToAdaptiveGrid();

  if (!fieldAccess || !fieldAccess->isAllocated()) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Field::"
		<< "upd: in.fieldAccess is uninitialized."
		<< visMsgErrorEnd;
    return 0;
  }

  visAdaptiveGrid *nodeGrid = fieldAccess;
  if (!(nodeGrid->getAttrAssoc() & visHasNodeData)) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Field::"
		<< "upd: only node associated fields supported"
		<< visMsgErrorEnd;
    return 0;
  }
  
  int colorAttribute = (int) attribute;
  if (colorAttribute >= fieldAccess->getAttributes()) {
    colorAttribute = 0;
  }
  visAttributeFactory *factory = 
    nodeGrid->getAttrFactory(colorAttribute);
  visAttribute* attr = factory->getNewAttribute();
  int attrDim = factory->getDim();
  factory->decRef();
  
  // Get field dimension

  int dim = nodeGrid->getDim();

  out.nspace = dim;

  // Sanity checks

  if (out.nspace < 2 || out.nspace > 3) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Field::"
		  << "upd: Only two or three dimensional fields "
		  << "supported."
		  << visMsgErrorEnd;

    nodeGrid->decRef();
    return 0;
  }

  // Init AVS/Express field structure
  
  int numberOfCells = nodeGrid->getCells();

  visMsgDbg << "nodeGrid->getCells() = " 
	    << numberOfCells
	    << visMsgEnd;

  out.nnodes = (1 << dim) * numberOfCells;

  int theRenderType = this->renderType;
  if (theRenderType == visRenderCellsFaces && dim == 1) {
    visMsgDbg
      << "Switching back from faces to vertices in one dimensional field" 
      << visMsgEnd;
    theRenderType = visRenderCellsEdges;
  }
  switch (theRenderType) {
  case visRenderCellsEdges:
    out.ncell_sets = 1;
    out.cell_set[0].cell_nnodes = 2;
    out.cell_set[0].cell_ndim = 1;
    out.cell_set[0].cell_order = 1;
    switch (dim) {
    case 1:
      out.cell_set[0].ncells = 1 * numberOfCells;
      break;
    case 2:
      out.cell_set[0].ncells = 4 * numberOfCells;;
      break;
    case 3:
      out.cell_set[0].ncells = 12 * numberOfCells;;
      break;
    }
    break;
  case visRenderCellsFaces:
    out.ncell_sets = 1;
    out.cell_set[0].cell_nnodes = 4;
    out.cell_set[0].cell_ndim = 2;
    out.cell_set[0].cell_order = 1;
    switch (dim) {
    case 2:
      out.cell_set[0].ncells = 1 * numberOfCells;;
      break;
    case 3:
      out.cell_set[0].ncells = 6 * numberOfCells;
      break;
    }
    break;
  default:
     visMsgError << "HiVis_HiVisMods_HiVis_Render_Field::"
		 << "unknown cell render type."
		 << visMsgErrorEnd;
     return 0;
  }    
  out.cell_set[0].poly_flag = 0;
  int *conOut = (int *)
    out.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
  
  // out.cell_set[out_set].ncells (int)
  // out.cell_set[out_set]. cell_nnodes (int)
  // out.cell_set[out_set]. cell_ndim (int)
  // out.cell_set[out_set]. cell_order (int)
  // out.cell_set[out_set]. cell_name (char *)
  // out.cell_set[out_set].node_connect_list (int *)
  

  // out.nspace (int) 
  // out.nnodes (int)
  // out.coordinates.values (float [])

  // out.nnode_data (int)
  // out.node_data[out_data_comp].veclen (int) 
  // out.node_data[out_data_comp].values (char [])

  float *cOut = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);

  // Check if data is scalar or of other type directly supported by the renderer

  bool doConvert;
  {
    out.nnode_data = 1;
    int *dataID = (int *) in.attributeDataID.ret_array_ptr(OM_GET_ARRAY_RD);
    
    if (dataID && dataID[colorAttribute] != 0) {
      out.node_data[0].id = dataID[colorAttribute];
      out.node_data[0].veclen = attrDim;
      doConvert = false;
    } else {
      out.node_data[0].id.set_obj_val(OMnull_obj);
      out.node_data[0].veclen = 1;
      doConvert = true;
    }
    if (dataID) ARRfree((void *) dataID);
  }

  // Define node_data
  
  float *nOut = (float*) out.node_data[0].values.
    ret_typed_array_ptr(OM_GET_ARRAY_WR, OM_TYPE_FLOAT);

  float pd = ipad;
  if (pd < 0) { pd = 0; } else if (pd > 1) { pd = 1; }
  pd /= 2;
  float PD[3], nPD[3];

  // Initialize min and max value
  
  visAttribute *minAttr = (visAttribute *) 
    nodeGrid->getMinAttribute(colorAttribute);
  visAttribute *maxAttr = (visAttribute *) 
    nodeGrid->getMaxAttribute(colorAttribute);
  double minVal = minAttr->getScalarValue(nodeGrid->getDim());
  double maxVal = maxAttr->getScalarValue(nodeGrid->getDim());

  visMsgDbg << "min attribute = " << minVal << ", max attribute = " << maxVal 
	    << visMsgEnd;
  minAttr->decRef();
  maxAttr->decRef();

  visAttributeEnumeration *e = new visAdaptiveGridCellEnumeration(nodeGrid);

  int cellCount = 0;
  int cIndex = 0;
  int conIndex = 0;
  
  int rIPos[3], rISize[3];
  float rPos[3], rSize[3];
  
  int i;

  // iterate through values
  
  e->setCursorFirstElement();
  while (e->isValidElement()) {
    
    // count cells
    
    cellCount++;
    
    visSYSTEM.setStatus("HiVis Render Field", 
			(float) cellCount / numberOfCells);
    
    nodeGrid->getCellPos(rIPos);
    nodeGrid->getCellSize(rISize);

    // crop outer half cell boundary

    for (i = 0; i < dim; i++) {
      rSize[i] = rISize[i];
      rPos[i] = rIPos[i];
      PD[i] = pd;
      nPD[i] = pd;
    }

    // Get cell color attribute

    nodeGrid->getCellAttribute(colorAttribute, attr);

    switch (dim) {
      
    case 1:
      cOut[(cIndex+0) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+1) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      
      if (doConvert) {
        nodeGrid->getNodeAttribute(colorAttribute, attr, 0);
        nOut[(cIndex+0)] = attr->getScalarValue(attrDim);
        nodeGrid->getNodeAttribute(colorAttribute, attr, 1);
        nOut[(cIndex+1)] = attr->getScalarValue(attrDim);
      } else {
	nodeGrid->getNodeAttribute(colorAttribute, attr, 0);
	for (i = 0; i < attrDim; i++) {
	  nOut[(cIndex+0) * attrDim + i] = attr->getValue(i);
	}
	nodeGrid->getNodeAttribute(colorAttribute, attr, 1);
	for (i = 0; i < attrDim; i++) {
	  nOut[(cIndex+1) * attrDim + i] = attr->getValue(i);
	}
      }
      
      conOut[(conIndex + 0 ) * 2 + 0] = cIndex + 0;
      conOut[(conIndex + 0 ) * 2 + 1] = cIndex + 1;
      
      cIndex += 2; conIndex += 1;
      
      break;
      
    case 2:
      cOut[(cIndex+0) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+0) * dim + visY] = rPos[visY] +  PD[visY];
      
      cOut[(cIndex+1) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      cOut[(cIndex+1) * dim + visY] = rPos[visY] +  PD[visY];
      
      cOut[(cIndex+2) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+2) * dim + visY] = rPos[visY] - nPD[visY] + rSize[visY];
      
      cOut[(cIndex+3) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      cOut[(cIndex+3) * dim + visY] = rPos[visY] - nPD[visY] + rSize[visY];
      
      if (doConvert) {
        nodeGrid->getNodeAttribute(colorAttribute, attr, 0);
        nOut[(cIndex+0)] = attr->getScalarValue(attrDim);
        nodeGrid->getNodeAttribute(colorAttribute, attr, 1);
        nOut[(cIndex+1)] = attr->getScalarValue(attrDim);
        nodeGrid->getNodeAttribute(colorAttribute, attr, 2);
        nOut[(cIndex+2)] = attr->getScalarValue(attrDim);
        nodeGrid->getNodeAttribute(colorAttribute, attr, 3);
        nOut[(cIndex+3)] = attr->getScalarValue(attrDim);
      } else {
	nodeGrid->getNodeAttribute(colorAttribute, attr, 0);
	for (i = 0; i < attrDim; i++) {
	  nOut[(cIndex+0) * attrDim + i] = attr->getValue(i);
	}
	nodeGrid->getNodeAttribute(colorAttribute, attr, 1);
	for (i = 0; i < attrDim; i++) {
	  nOut[(cIndex+1) * attrDim + i] = attr->getValue(i);
	}
	nodeGrid->getNodeAttribute(colorAttribute, attr, 2);
	for (i = 0; i < attrDim; i++) {
	  nOut[(cIndex+2) * attrDim + i] = attr->getValue(i);
	}
        nodeGrid->getNodeAttribute(colorAttribute, attr, 3);
        for (i = 0; i < attrDim; i++) {
          nOut[(cIndex+3) * attrDim + i] = attr->getValue(i);
        }
      }

      switch (theRenderType) {
      case visRenderCellsEdges:

	conOut[(conIndex + 0 ) * 2 + 0] = cIndex+ 0;
	conOut[(conIndex + 0 ) * 2 + 1] = cIndex+ 1;
	conOut[(conIndex + 1 ) * 2 + 0] = cIndex+ 1;
	conOut[(conIndex + 1 ) * 2 + 1] = cIndex+ 3;
	conOut[(conIndex + 2 ) * 2 + 0] = cIndex+ 3;
	conOut[(conIndex + 2 ) * 2 + 1] = cIndex+ 2;
	conOut[(conIndex + 3 ) * 2 + 0] = cIndex+ 2;
	conOut[(conIndex + 3 ) * 2 + 1] = cIndex+ 0;
	cIndex += 4; conIndex += 4;
	break;
      case visRenderCellsFaces:
	conOut[(conIndex + 0 ) * 4 + 0] = cIndex+ 0;
	conOut[(conIndex + 0 ) * 4 + 1] = cIndex+ 1;
	conOut[(conIndex + 0 ) * 4 + 2] = cIndex+ 3;
	conOut[(conIndex + 0 ) * 4 + 3] = cIndex+ 2;
	cIndex += 4; conIndex += 1;
	break;
      }
      break;

    case 3:
      cOut[(cIndex+0) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+0) * dim + visY] = rPos[visY] +  PD[visY];
      cOut[(cIndex+0) * dim + visZ] = rPos[visZ] +  PD[visZ];
      
      cOut[(cIndex+1) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      cOut[(cIndex+1) * dim + visY] = rPos[visY] +  PD[visY];
      cOut[(cIndex+1) * dim + visZ] = rPos[visZ] +  PD[visZ];
      
      cOut[(cIndex+2) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+2) * dim + visY] = rPos[visY] - nPD[visY] + rSize[visY];
      cOut[(cIndex+2) * dim + visZ] = rPos[visZ] +  PD[visZ];
      
      cOut[(cIndex+3) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      cOut[(cIndex+3) * dim + visY] = rPos[visY] - nPD[visY] + rSize[visY];
      cOut[(cIndex+3) * dim + visZ] = rPos[visZ] +  PD[visZ];
      
      cOut[(cIndex+4) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+4) * dim + visY] = rPos[visY] +  PD[visY];
      cOut[(cIndex+4) * dim + visZ] = rPos[visZ] - nPD[visZ] + rSize[visZ];
      
      cOut[(cIndex+5) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      cOut[(cIndex+5) * dim + visY] = rPos[visY] +  PD[visY];
      cOut[(cIndex+5) * dim + visZ] = rPos[visZ] - nPD[visZ] + rSize[visZ];
      
      cOut[(cIndex+6) * dim + visX] = rPos[visX] +  PD[visX];
      cOut[(cIndex+6) * dim + visY] = rPos[visY] - nPD[visY] + rSize[visY];
      cOut[(cIndex+6) * dim + visZ] = rPos[visZ] - nPD[visZ] + rSize[visZ];
      
      cOut[(cIndex+7) * dim + visX] = rPos[visX] - nPD[visX] + rSize[visX];
      cOut[(cIndex+7) * dim + visY] = rPos[visY] - nPD[visY] + rSize[visY];
      cOut[(cIndex+7) * dim + visZ] = rPos[visZ] - nPD[visZ] + rSize[visZ];
            
      if (doConvert) {
        nodeGrid->getNodeAttribute(colorAttribute, attr, 0);
        nOut[(cIndex+0)] = attr->getScalarValue(attrDim);
        nodeGrid->getNodeAttribute(colorAttribute, attr, 1);
        nOut[(cIndex+1)] = attr->getScalarValue(attrDim);
        nodeGrid->getNodeAttribute(colorAttribute, attr, 2);
        nOut[(cIndex+2)] = attr->getScalarValue(attrDim);
        nodeGrid->getNodeAttribute(colorAttribute, attr, 3);
        nOut[(cIndex+3)] = attr->getScalarValue(attrDim);
        nodeGrid->getNodeAttribute(colorAttribute, attr, 4);
        nOut[(cIndex+4)] = attr->getScalarValue(attrDim);
        nodeGrid->getNodeAttribute(colorAttribute, attr, 5);
        nOut[(cIndex+5)] = attr->getScalarValue(attrDim);
        nodeGrid->getNodeAttribute(colorAttribute, attr, 6);
        nOut[(cIndex+6)] = attr->getScalarValue(attrDim);
        nodeGrid->getNodeAttribute(colorAttribute, attr, 7);
        nOut[(cIndex+7)] = attr->getScalarValue(attrDim);
      } else {
        nodeGrid->getNodeAttribute(colorAttribute, attr, 0);
        for (i = 0; i < attrDim; i++) {
          nOut[(cIndex+0) * attrDim + i] = attr->getValue(i);
        }
        nodeGrid->getNodeAttribute(colorAttribute, attr, 1);
        for (i = 0; i < attrDim; i++) {
          nOut[(cIndex+1) * attrDim + i] = attr->getValue(i);
        }
        nodeGrid->getNodeAttribute(colorAttribute, attr, 2);
        for (i = 0; i < attrDim; i++) {
          nOut[(cIndex+2) * attrDim + i] = attr->getValue(i);
        }
        nodeGrid->getNodeAttribute(colorAttribute, attr, 3);
        for (i = 0; i < attrDim; i++) {
          nOut[(cIndex+3) * attrDim + i] = attr->getValue(i);
        }
        nodeGrid->getNodeAttribute(colorAttribute, attr, 4);
        for (i = 0; i < attrDim; i++) {
          nOut[(cIndex+4) * attrDim + i] = attr->getValue(i);
        }
        nodeGrid->getNodeAttribute(colorAttribute, attr, 5);
        for (i = 0; i < attrDim; i++) {
          nOut[(cIndex+5) * attrDim + i] = attr->getValue(i);
        }
        nodeGrid->getNodeAttribute(colorAttribute, attr, 6);
        for (i = 0; i < attrDim; i++) {
          nOut[(cIndex+6) * attrDim + i] = attr->getValue(i);
        }
        nodeGrid->getNodeAttribute(colorAttribute, attr, 7);
        for (i = 0; i < attrDim; i++) {
          nOut[(cIndex+7) * attrDim + i] = attr->getValue(i);
        }
      }

      switch (theRenderType) {
      case visRenderCellsEdges:

	conOut[(conIndex + 0 ) * 2 + 0] = cIndex+ 0;
	conOut[(conIndex + 0 ) * 2 + 1] = cIndex+ 1;
	conOut[(conIndex + 1 ) * 2 + 0] = cIndex+ 1;
	conOut[(conIndex + 1 ) * 2 + 1] = cIndex+ 3;
	conOut[(conIndex + 2 ) * 2 + 0] = cIndex+ 3;
	conOut[(conIndex + 2 ) * 2 + 1] = cIndex+ 2;
	conOut[(conIndex + 3 ) * 2 + 0] = cIndex+ 2;
	conOut[(conIndex + 3 ) * 2 + 1] = cIndex+ 0;
	  
	conOut[(conIndex + 4 ) * 2 + 0] = cIndex+ 4;
	conOut[(conIndex + 4 ) * 2 + 1] = cIndex+ 5;
	conOut[(conIndex + 5 ) * 2 + 0] = cIndex+ 5;
	conOut[(conIndex + 5 ) * 2 + 1] = cIndex+ 7;
	conOut[(conIndex + 6 ) * 2 + 0] = cIndex+ 7;
	conOut[(conIndex + 6 ) * 2 + 1] = cIndex+ 6;
	conOut[(conIndex + 7 ) * 2 + 0] = cIndex+ 6;
	conOut[(conIndex + 7 ) * 2 + 1] = cIndex+ 4;
      
	conOut[(conIndex + 8 ) * 2 + 0] = cIndex+ 0;
	conOut[(conIndex + 8 ) * 2 + 1] = cIndex+ 4;
	conOut[(conIndex + 9 ) * 2 + 0] = cIndex+ 1;
	conOut[(conIndex + 9 ) * 2 + 1] = cIndex+ 5;
	conOut[(conIndex + 10) * 2 + 0] = cIndex+ 2;
	conOut[(conIndex + 10) * 2 + 1] = cIndex+ 6;
	conOut[(conIndex + 11) * 2 + 0] = cIndex+ 3;
	conOut[(conIndex + 11) * 2 + 1] = cIndex+ 7;
	cIndex += 8; conIndex += 12;
	
	break;
      case visRenderCellsFaces:
	conOut[(conIndex + 0 ) * 4 + 0] = cIndex+ 2;
	conOut[(conIndex + 0 ) * 4 + 1] = cIndex+ 3;
	conOut[(conIndex + 0 ) * 4 + 2] = cIndex+ 1;
	conOut[(conIndex + 0 ) * 4 + 3] = cIndex+ 0;
	
	conOut[(conIndex + 1 ) * 4 + 0] = cIndex+ 1;
	conOut[(conIndex + 1 ) * 4 + 1] = cIndex+ 5;
	conOut[(conIndex + 1 ) * 4 + 2] = cIndex+ 4;
	conOut[(conIndex + 1 ) * 4 + 3] = cIndex+ 0;
	
	conOut[(conIndex + 2 ) * 4 + 0] = cIndex+ 4;
	conOut[(conIndex + 2 ) * 4 + 1] = cIndex+ 6;
	conOut[(conIndex + 2 ) * 4 + 2] = cIndex+ 2;
	conOut[(conIndex + 2 ) * 4 + 3] = cIndex+ 0;
	
	conOut[(conIndex + 3 ) * 4 + 0] = cIndex+ 7;
	conOut[(conIndex + 3 ) * 4 + 1] = cIndex+ 5;
	conOut[(conIndex + 3 ) * 4 + 2] = cIndex+ 1;
	conOut[(conIndex + 3 ) * 4 + 3] = cIndex+ 3;
	
	conOut[(conIndex + 4 ) * 4 + 0] = cIndex+ 6;
	conOut[(conIndex + 4 ) * 4 + 1] = cIndex+ 4;
	conOut[(conIndex + 4 ) * 4 + 2] = cIndex+ 5;
	conOut[(conIndex + 4 ) * 4 + 3] = cIndex+ 7;
	
	conOut[(conIndex + 5 ) * 4 + 0] = cIndex+ 6;
	conOut[(conIndex + 5 ) * 4 + 1] = cIndex+ 7;
	conOut[(conIndex + 5 ) * 4 + 2] = cIndex+ 3;
	conOut[(conIndex + 5 ) * 4 + 3] = cIndex+ 2;
	
	cIndex += 8; conIndex += 6;
	
	break;
      }
      break;
    }
    e->setCursorNextElement();
  }

  //delete e;
  e->decRef();
  

  // Set min and max value;

  OMset_data_type((OMobj_id)out.node_data[0].min,OM_TYPE_FLOAT);
  OMset_data_type((OMobj_id)out.node_data[0].max,OM_TYPE_FLOAT);
  out.node_data[0].min = minVal;
  out.node_data[0].max = maxVal;

  if (conOut)
    ARRfree((char *)conOut);

  if (nOut)
    ARRfree((char *)nOut);
  
  if (cOut)
    ARRfree((char *)cOut);
  
  // Copy attribute type id
  
  int *dataID = (int *) in.attributeDataID.ret_array_ptr(OM_GET_ARRAY_RD);
  if (dataID && dataID[colorAttribute] != 0) {
    out.node_data[0].id = dataID[colorAttribute];
  } else {
    out.node_data[0].id.set_obj_val(OMnull_obj);
  }
  if (dataID) ARRfree((void *) dataID);

  nodeGrid->decRef();

  visSETSTATUSIDLE;

  // return 1 for success

  return 1;
}

static struct Render_SparseGrid_Data {
  float *nOut;
  float *cOut;
  int *conOut;
  int *cursorBuf;
  int *size;
  int conIndex;
  int cIndex;
  int dim;
  int maxLevel;
  int *pos;
  int *ldScope;
  float scale;
  visSparseGrid* fieldAccess;
  int colorAttribute;
  visAttribute *attr;
  int attrDim;
  int doConvert;
  float min, max;
  int numberOfCells, nodeIndex;
} Render_SparseGrid_AddNode_Data;

static void HiVis_Render_SparseGrid_AddNode(visSparseGridNode *node,
					    int index, int dim)
{
  int i;
  Render_SparseGrid_Data &my = Render_SparseGrid_AddNode_Data;

  if (!node) return;

  if (dim + 1 == my.dim) {
    
    visSYSTEM.setStatus("SparseGrid Render Nodes", 
			(float) (my.nodeIndex++) / my.numberOfCells);

    /*int node = 0;
    for (i = 0; i < my.dim; i++) {
      my.cursorBuf[i] = my.pos[index * my.dim + i];
      if (my.cursorBuf[i] >= my.size[i]) {
	my.cursorBuf[i]--;
	node |= 1 << i;
      }
      }*/
    bool doEvalAttribute = true;
    for (i = 0; i < my.dim; i++) {
      if (&my.pos[index * my.dim + i] < 0 || 
	  my.pos[index * my.dim + i] > my.fieldAccess->getSize(i)) {
	doEvalAttribute = false;
	break;
      }
    }
    float x;
    if (doEvalAttribute) {
      my.fieldAccess->setCursorN(&my.pos[index * my.dim]);
      my.fieldAccess->getAttributeN(my.colorAttribute, my.attr);
      x = my.attr->getScalarValue(my.attrDim);
      if (x > my.max) {
	my.max = x;
      }
      if (x < my.min) {
	my.min = x;
      }
    } else {
      x = my.min;
    }

    switch (my.dim) {
      
    case 2:
      my.cOut[(my.cIndex+0) * my.dim + visX] = 
	(float) my.pos[index * my.dim + visX] - 0.5 * my.scale;
      my.cOut[(my.cIndex+0) * my.dim + visY] = 
	(float) my.pos[index * my.dim + visY] -	0.5 * my.scale;
    
      my.cOut[(my.cIndex+1) * my.dim + visX] = 
	(float) my.pos[index * my.dim + visX] + 0.5 * my.scale;
      my.cOut[(my.cIndex+1) * my.dim + visY] =
	(float) my.pos[index * my.dim + visY] - 0.5 * my.scale;
      
      my.cOut[(my.cIndex+2) * my.dim + visX] =
	(float) my.pos[index * my.dim + visX] -	0.5 * my.scale;
      my.cOut[(my.cIndex+2) * my.dim + visY] =
	(float) my.pos[index * my.dim + visY] +	0.5 * my.scale;
      
      my.cOut[(my.cIndex+3) * my.dim + visX] = 
	(float) my.pos[index * my.dim + visX] + 0.5 * my.scale;
      my.cOut[(my.cIndex+3) * my.dim + visY] =
	(float) my.pos[index * my.dim + visY] +	0.5 * my.scale;
      
      if (my.doConvert) {
	my.nOut[(my.cIndex+0)] = x;
	my.nOut[(my.cIndex+1)] = x;
	my.nOut[(my.cIndex+2)] = x;
	my.nOut[(my.cIndex+3)] = x;
      } else {
	for (i = 0; i < my.attrDim; i++) {
	  my.nOut[(my.cIndex+0) * my.attrDim + i] = x;
	}
	for (i = 0; i < my.attrDim; i++) {
	  my.nOut[(my.cIndex+1) * my.attrDim + i] = x;
	}
	for (i = 0; i < my.attrDim; i++) {
	  my.nOut[(my.cIndex+2) * my.attrDim + i] = x;
	}
	for (i = 0; i < my.attrDim; i++) {
	  my.nOut[(my.cIndex+3) * my.attrDim + i] = x;
	}
      }
    
      my.conOut[(my.conIndex + 0 ) * 4 + 0] = my.cIndex+ 0;
      my.conOut[(my.conIndex + 0 ) * 4 + 1] = my.cIndex+ 1;
      my.conOut[(my.conIndex + 0 ) * 4 + 2] = my.cIndex+ 3;
      my.conOut[(my.conIndex + 0 ) * 4 + 3] = my.cIndex+ 2;
      my.cIndex += 4; my.conIndex += 1;
      break;
    
    case 3:
      my.cOut[(my.cIndex+0) * my.dim + visX] = 
	(float) my.pos[index * my.dim + visX] -	0.5 * my.scale;
      my.cOut[(my.cIndex+0) * my.dim + visY] =
	(float) my.pos[index * my.dim + visY] -	0.5 * my.scale;
      my.cOut[(my.cIndex+0) * my.dim + visZ] = 
	(float) my.pos[index * my.dim + visZ] -	0.5 * my.scale;
    
      my.cOut[(my.cIndex+1) * my.dim + visX] =
	(float) my.pos[index * my.dim + visX] +	0.5 * my.scale;
      my.cOut[(my.cIndex+1) * my.dim + visY] =
	(float) my.pos[index * my.dim + visY] -	0.5 * my.scale;
      my.cOut[(my.cIndex+1) * my.dim + visZ] =
	(float) my.pos[index * my.dim + visZ] -	0.5 * my.scale;
    
      my.cOut[(my.cIndex+2) * my.dim + visX] =
	(float) my.pos[index * my.dim + visX] -	0.5 * my.scale;
      my.cOut[(my.cIndex+2) * my.dim + visY] =
	(float) my.pos[index * my.dim + visY] +	0.5 * my.scale;
      my.cOut[(my.cIndex+2) * my.dim + visZ] =
	(float) my.pos[index * my.dim + visZ] -	0.5 * my.scale;
    
      my.cOut[(my.cIndex+3) * my.dim + visX] = 
	(float) my.pos[index * my.dim + visX] +	0.5 * my.scale;
      my.cOut[(my.cIndex+3) * my.dim + visY] = 
	(float) my.pos[index * my.dim + visY] + 0.5 * my.scale;
      my.cOut[(my.cIndex+3) * my.dim + visZ] = 
	(float) my.pos[index * my.dim + visZ] -	0.5 * my.scale;
    
      my.cOut[(my.cIndex+4) * my.dim + visX] = 
	(float) my.pos[index * my.dim + visX] - 0.5 * my.scale;
      my.cOut[(my.cIndex+4) * my.dim + visY] = 
	(float) my.pos[index * my.dim + visY] -	0.5 * my.scale;
      my.cOut[(my.cIndex+4) * my.dim + visZ] =
	(float) my.pos[index * my.dim + visZ] +	0.5 * my.scale;
    
      my.cOut[(my.cIndex+5) * my.dim + visX] = 
	(float) my.pos[index * my.dim + visX] +	0.5 * my.scale;
      my.cOut[(my.cIndex+5) * my.dim + visY] =
	(float) my.pos[index * my.dim + visY] -	0.5 * my.scale;
      my.cOut[(my.cIndex+5) * my.dim + visZ] =
	(float) my.pos[index * my.dim + visZ] +	0.5 * my.scale;
    
      my.cOut[(my.cIndex+6) * my.dim + visX] = 
	(float) my.pos[index * my.dim + visX] -	0.5 * my.scale;
      my.cOut[(my.cIndex+6) * my.dim + visY] =
	(float) my.pos[index * my.dim + visY] +	0.5 * my.scale;
      my.cOut[(my.cIndex+6) * my.dim + visZ] =
	(float) my.pos[index * my.dim + visZ] +	0.5 * my.scale;
    
      my.cOut[(my.cIndex+7) * my.dim + visX] = 
	(float) my.pos[index * my.dim + visX] +	0.5 * my.scale;
      my.cOut[(my.cIndex+7) * my.dim + visY] =
	(float) my.pos[index * my.dim + visY] +	0.5 * my.scale;
      my.cOut[(my.cIndex+7) * my.dim + visZ] = 
	(float) my.pos[index * my.dim + visZ] +	0.5 * my.scale;
    
      if (my.doConvert) {
	my.nOut[(my.cIndex+0)] = x;
	my.nOut[(my.cIndex+1)] = x;
	my.nOut[(my.cIndex+2)] = x;
	my.nOut[(my.cIndex+3)] = x;
	my.nOut[(my.cIndex+4)] = x;
	my.nOut[(my.cIndex+5)] = x;
	my.nOut[(my.cIndex+6)] = x;
	my.nOut[(my.cIndex+7)] = x;
      } else {
	for (i = 0; i < my.attrDim; i++) {
	  my.nOut[(my.cIndex+0) * my.attrDim + i] = x;
	}
	for (i = 0; i < my.attrDim; i++) {
	  my.nOut[(my.cIndex+1) * my.attrDim + i] = x;
	}
	for (i = 0; i < my.attrDim; i++) {
	  my.nOut[(my.cIndex+2) * my.attrDim + i] = x;
	}
	for (i = 0; i < my.attrDim; i++) {
	  my.nOut[(my.cIndex+3) * my.attrDim + i] = x;
	}
	for (i = 0; i < my.attrDim; i++) {
	  my.nOut[(my.cIndex+4) * my.attrDim + i] = x;
	}
	for (i = 0; i < my.attrDim; i++) {
	  my.nOut[(my.cIndex+5) * my.attrDim + i] = x;
	}
	for (i = 0; i < my.attrDim; i++) {
	  my.nOut[(my.cIndex+6) * my.attrDim + i] = x;
	}
	for (i = 0; i < my.attrDim; i++) {
	  my.nOut[(my.cIndex+7) * my.attrDim + i] = x;
	}
      }
    
      my.conOut[(my.conIndex + 0 ) * 4 + 0] = my.cIndex+ 2;
      my.conOut[(my.conIndex + 0 ) * 4 + 1] = my.cIndex+ 3;
      my.conOut[(my.conIndex + 0 ) * 4 + 2] = my.cIndex+ 1;
      my.conOut[(my.conIndex + 0 ) * 4 + 3] = my.cIndex+ 0;
    
      my.conOut[(my.conIndex + 1 ) * 4 + 0] = my.cIndex+ 1;
      my.conOut[(my.conIndex + 1 ) * 4 + 1] = my.cIndex+ 5;
      my.conOut[(my.conIndex + 1 ) * 4 + 2] = my.cIndex+ 4;
      my.conOut[(my.conIndex + 1 ) * 4 + 3] = my.cIndex+ 0;
    
      my.conOut[(my.conIndex + 2 ) * 4 + 0] = my.cIndex+ 4;
      my.conOut[(my.conIndex + 2 ) * 4 + 1] = my.cIndex+ 6;
      my.conOut[(my.conIndex + 2 ) * 4 + 2] = my.cIndex+ 2;
      my.conOut[(my.conIndex + 2 ) * 4 + 3] = my.cIndex+ 0;
    
      my.conOut[(my.conIndex + 3 ) * 4 + 0] = my.cIndex+ 7;
      my.conOut[(my.conIndex + 3 ) * 4 + 1] = my.cIndex+ 5;
      my.conOut[(my.conIndex + 3 ) * 4 + 2] = my.cIndex+ 1;
      my.conOut[(my.conIndex + 3 ) * 4 + 3] = my.cIndex+ 3;
    
      my.conOut[(my.conIndex + 4 ) * 4 + 0] = my.cIndex+ 6;
      my.conOut[(my.conIndex + 4 ) * 4 + 1] = my.cIndex+ 4;
      my.conOut[(my.conIndex + 4 ) * 4 + 2] = my.cIndex+ 5;
      my.conOut[(my.conIndex + 4 ) * 4 + 3] = my.cIndex+ 7;
    
      my.conOut[(my.conIndex + 5 ) * 4 + 0] = my.cIndex+ 6;
      my.conOut[(my.conIndex + 5 ) * 4 + 1] = my.cIndex+ 7;
      my.conOut[(my.conIndex + 5 ) * 4 + 2] = my.cIndex+ 3;
      my.conOut[(my.conIndex + 5 ) * 4 + 3] = my.cIndex+ 2;
    
      my.cIndex += 8; my.conIndex += 6;
      break;
    }
  } else {
    my.pos[(index) * my.dim + dim + 1] = 0;
    HiVis_Render_SparseGrid_AddNode(((visSparseGridStructure*)node)->next,
				    index, dim + 1);
  }

  for (i = 0; i < my.dim; i++) {
    if (i == dim) {
      my.pos[(index + 1) * my.dim + i] = my.pos[index * my.dim + i] - 
	(1 << (my.ldScope[index * my.dim + i] - 1));
      my.ldScope[(index + 1) * my.dim + i] = my.ldScope[index * my.dim + i] - 1;
    } else {
      my.pos[(index + 1) * my.dim + i] = my.pos[index * my.dim + i];
      my.ldScope[(index + 1) * my.dim + i] = my.ldScope[index * my.dim + i];
    }
  }
  HiVis_Render_SparseGrid_AddNode(node->left, index + 1, dim);
  for (i = 0; i < my.dim; i++) {
    if (i == dim) {
      my.pos[(index + 1) * my.dim + i] = my.pos[index * my.dim + i] + 
	(1 << (my.ldScope[index * my.dim + i] - 1));
      my.ldScope[(index + 1) * my.dim + i] = my.ldScope[index * my.dim + i] - 1;
    } else {
      my.pos[(index + 1) * my.dim + i] = my.pos[index * my.dim + i];
      my.ldScope[(index + 1) * my.dim + i] = my.ldScope[index * my.dim + i];
    }
  }
  HiVis_Render_SparseGrid_AddNode(node->right, index + 1, dim);
};

int HiVis_HiVisMods_HiVis_Render_SparseGrid::upd(OMevent_mask, int)
{

   /***********************/
   /* Function's Body     */
   /***********************/

  visMsgCall << "HiVis_HiVisMods_HiVis_Render_SparseGrid::"
	     << "upd: called." << visMsgEnd;
  
  // Get access to visAdaptiveGrid

  visGridLink *gridLink = 
     (visGridLink *) in.ret_class_ptr("visGridLink");

  if (!gridLink) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_SparseGrid::"
		<< "upd: in.gridLink is uninitialized" 
		<< visMsgErrorEnd;
    return 0;
  }

  visAdaptiveGrid *fieldAccess = gridLink->castToAdaptiveGrid();

  if (!fieldAccess) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_SparseGrid::"
		<< "upd: in.fieldAccess is uninitialized."
		<< visMsgErrorEnd;
    return 0;
   }

  if (!fieldAccess->isAllocated()) {
    visMsgError << "HiVis_HiVisMods_Render_SparseGrid::"
		<< "upd: in.fieldAccess is unallocated."
		<< visMsgErrorEnd;
    return 0;
  }
  
  if (!fieldAccess->instanceOf(visSparseGridId)) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_SparseGrid::"
		<< "upd: Field is not of type SparseGrid!"
		<< visMsgErrorEnd;
    return 0;
  }

  // Get field dimension

  int dim = fieldAccess->getDim();

  out.nspace = dim;

  // Sanity checks

  if (out.nspace < 2 || out.nspace > 3) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_SparseGrid::"
		  << "upd: Only two or three dimensional fields "
		  << "supported."
		  << visMsgErrorEnd;

    return 0;
  }

  // Init AVS/Express field structure
  
  int numberOfCells = ((visSparseGrid*)fieldAccess)->myNodes;

  visMsgDbg << "fieldAccess->myNodes = " 
	    << numberOfCells
	    << visMsgEnd;

  out.nnodes = (1 << dim) * numberOfCells;

  out.ncell_sets = 1;
  out.cell_set[0].cell_nnodes = 4;
  out.cell_set[0].cell_ndim = 2;
  out.cell_set[0].cell_order = 1;
  switch (dim) {
  case 2:
    out.cell_set[0].ncells = 1 * numberOfCells;;
    break;
  case 3:
    out.cell_set[0].ncells = 6 * numberOfCells;
    break;
  }
  out.cell_set[0].poly_flag = 0;
  int *conOut = (int *)
    out.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
  
  float *cOut = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);

  // Check if data is scalar or of other type directly supported by the renderer

  int colorAttribute = (int) attribute;
  if (colorAttribute >= fieldAccess->getAttributes()) {
    colorAttribute = 0;
  }
  visAttributeFactory *factory = fieldAccess->getAttrFactory(colorAttribute);
  Render_SparseGrid_AddNode_Data.attr = factory->getNewAttribute();
  Render_SparseGrid_AddNode_Data.attrDim = factory->getDim();
  Render_SparseGrid_AddNode_Data.numberOfCells = numberOfCells;
  Render_SparseGrid_AddNode_Data.nodeIndex = 0;
  factory->decRef();

  bool doConvert;
  {
    out.nnode_data = 1;
    int *dataID = (int *) in.attributeDataID.ret_array_ptr(OM_GET_ARRAY_RD);
    
    if (dataID && dataID[colorAttribute] != 0) {
      out.node_data[0].id = dataID[colorAttribute];
      out.node_data[0].veclen = Render_SparseGrid_AddNode_Data.attrDim;
      doConvert = false;
    } else {
      out.node_data[0].id.set_obj_val(OMnull_obj);
      out.node_data[0].veclen = 1;
      doConvert = true;
    }
    if (dataID) ARRfree((void *) dataID);
  }

  // Define node_data
  
  float *nOut = (float*) out.node_data[0].values.
    ret_typed_array_ptr(OM_GET_ARRAY_WR, OM_TYPE_FLOAT);

  int theLdScale = ldScale;

  // Initialize min and max value
  
  visAttribute *minAttr = (visAttribute *) 
    fieldAccess->getMinAttribute(colorAttribute);
  visAttribute *maxAttr = (visAttribute *) 
    fieldAccess->getMaxAttribute(colorAttribute);
  double minVal = minAttr->getScalarValue(fieldAccess->getDim());
  double maxVal = maxAttr->getScalarValue(fieldAccess->getDim());
  Render_SparseGrid_AddNode_Data.min = minVal;
  Render_SparseGrid_AddNode_Data.max = maxVal;

  visMsgDbg << "min attribute = " << minVal << ", max attribute = " << maxVal 
	    << visMsgEnd;
  visMsgDbg << "dim = " << dim << ", max level = " 
	    << ((visSparseGrid*)fieldAccess)->myMaxLevel
	    << visMsgEnd;
  minAttr->decRef();
  maxAttr->decRef();

  Render_SparseGrid_AddNode_Data.nOut = nOut;
  Render_SparseGrid_AddNode_Data.cOut = cOut;
  Render_SparseGrid_AddNode_Data.conOut = conOut;
  Render_SparseGrid_AddNode_Data.dim = dim;
  Render_SparseGrid_AddNode_Data.maxLevel = 
    ((visSparseGrid*)fieldAccess)->myMaxLevel;
  Render_SparseGrid_AddNode_Data.pos = 
    new int[dim * (((visSparseGrid*)fieldAccess)->myMaxLevel + 2)];
  Render_SparseGrid_AddNode_Data.ldScope = 
    new int[dim * (((visSparseGrid*)fieldAccess)->myMaxLevel + 2)];

  Render_SparseGrid_AddNode_Data.scale = 0.5f * (1 << theLdScale);
  
  Render_SparseGrid_AddNode_Data.colorAttribute = colorAttribute;
  Render_SparseGrid_AddNode_Data.fieldAccess = (visSparseGrid*)fieldAccess;
  Render_SparseGrid_AddNode_Data.conIndex = 0;
  Render_SparseGrid_AddNode_Data.cIndex = 0;
  Render_SparseGrid_AddNode_Data.doConvert = doConvert;
  Render_SparseGrid_Data &my = Render_SparseGrid_AddNode_Data;
  my.cursorBuf = new int[dim];
  my.size = new int[dim];
  int i;
  for (i = 0; i < dim; i++) {
    my.size[i] = fieldAccess->getSize(i);
    Render_SparseGrid_AddNode_Data.pos[i] = 0;
    Render_SparseGrid_AddNode_Data.ldScope[i] = 
      ((visSparseGrid*)fieldAccess)->myLdScope[i];
  }
  HiVis_Render_SparseGrid_AddNode
    (((visSparseGrid*)fieldAccess)->myRootNode, 0, 0);

  delete[] my.cursorBuf;
  delete[] my.size;
  delete[] Render_SparseGrid_AddNode_Data.pos;
  delete[] Render_SparseGrid_AddNode_Data.ldScope;
  Render_SparseGrid_AddNode_Data.attr->decRef();
  minVal = Render_SparseGrid_AddNode_Data.min;
  maxVal = Render_SparseGrid_AddNode_Data.max;


  /*  {
  int i;

  }

  */
  // Set min and max value;

  OMset_data_type((OMobj_id)out.node_data[0].min,OM_TYPE_FLOAT);
  OMset_data_type((OMobj_id)out.node_data[0].max,OM_TYPE_FLOAT);
  out.node_data[0].min = minVal;
  out.node_data[0].max = maxVal;

  if (conOut)
    ARRfree((char *)conOut);

  if (nOut)
    ARRfree((char *)nOut);
  
  if (cOut)
    ARRfree((char *)cOut);
  
  // Copy attribute type id
  
  int *dataID = (int *) in.attributeDataID.ret_array_ptr(OM_GET_ARRAY_RD);
  if (dataID && dataID[colorAttribute] != 0) {
    out.node_data[0].id = dataID[colorAttribute];
  } else {
    out.node_data[0].id.set_obj_val(OMnull_obj);
  }
  if (dataID) ARRfree((void *) dataID);

  visSETSTATUSIDLE;

  // return 1 for success

  return 1;
}

int HiVis_HiVisMods_HiVis_Bounds::upd(OMevent_mask, int)
{

   /***********************/
   /* Function's Body     */
   /***********************/

  visMsgCall << "HiVis_HiVisMods_HiVis_Bounds::"
	     << "upd: called." << visMsgEnd;
  
  // Get access to visAdaptiveGrid

  visGridLink *gridLink = 
     (visGridLink *) in.ret_class_ptr("visGridLink");

  if (!gridLink) {
    visMsgError << "HiVis_HiVisMods_HiVis_Bounds::"
		<< "upd: in.gridLink is uninitialized" 
		<< visMsgErrorEnd;
    return 0;
  }

  visAdaptiveGrid *fieldAccess = gridLink->castToAdaptiveGrid();

  if (!fieldAccess) {
    visMsgError << "HiVis_HiVisMods_HiVis_Bounds::"
		<< "upd: in.fieldAccess is uninitialized."
		<< visMsgErrorEnd;
    return 0;
   }

  if (!fieldAccess->isAllocated()) {
    visMsgError << "HiVis_HiVisMods_Bounds::"
		<< "upd: in.fieldAccess is unallocated."
		<< visMsgErrorEnd;
    return 0;
  }
  
  int colorAttribute = (int) attribute;
  if (colorAttribute >= fieldAccess->getAttributes()) {
    colorAttribute = 0;
  }
  visAttributeFactory *factory = 
    fieldAccess->getAttrFactory(colorAttribute);
  visAttribute* attr = factory->getNewAttribute();
  int attrDim = factory->getDim();
  factory->decRef();
  
  // Get field dimension

  int dim = fieldAccess->getDim();

  out.nspace = dim;

  // Sanity checks

  if (out.nspace < 2 || out.nspace > 3) {
    visMsgError << "HiVis_HiVisMods_HiVis_Bounds::"
		  << "upd: Only two or three dimensional fields "
		  << "supported."
		  << visMsgErrorEnd;

    return 0;
  }

  out.nnodes = (1 << dim) * 1;

  int theRenderType = this->renderType;
  if (theRenderType == visRenderCellsFaces && dim == 1) {
    visMsgDbg
      << "Switching back from faces to vertices in one dimensional field" 
      << visMsgEnd;
    theRenderType = visRenderCellsEdges;
  }
  out.ncell_sets = 1;
  out.cell_set[0].cell_nnodes = 2;
  out.cell_set[0].cell_ndim = 1;
  out.cell_set[0].cell_order = 1;
  switch (dim) {
  case 2:
    out.cell_set[0].ncells = 4;
    break;
  case 3:
    out.cell_set[0].ncells = 12;
    break;
  }
  out.cell_set[0].poly_flag = 0;
  int *conOut = (int *)
    out.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
  
  float *cOut = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);

  bool doConvert = true;
  out.nnode_data = 1;
  out.node_data[0].veclen = attrDim;

  // Copy attribute type id
  
  int *dataID = (int *) in.attributeDataID.ret_array_ptr(OM_GET_ARRAY_RD);
  if (dataID && dataID[colorAttribute] != 0) {
    out.node_data[0].id = dataID[colorAttribute];
  } else {
    out.node_data[0].id.set_obj_val(OMnull_obj);
  }
  if (dataID) ARRfree((void *) dataID);

  // Define node_data
  
  float *nOut = (float*) out.node_data[0].values.
    ret_typed_array_ptr(OM_GET_ARRAY_WR, OM_TYPE_FLOAT);

  // Initialize min and max value
  
  visAttribute *minAttr = (visAttribute *) 
    fieldAccess->getMinAttribute(colorAttribute);
  visAttribute *maxAttr = (visAttribute *) 
    fieldAccess->getMaxAttribute(colorAttribute);
  double minVal = minAttr->getScalarValue(fieldAccess->getDim());
  double maxVal = maxAttr->getScalarValue(fieldAccess->getDim());

  visMsgDbg << "min attribute = " << minVal << ", max attribute = " << maxVal 
	    << visMsgEnd;
  minAttr->decRef();
  maxAttr->decRef();

  int i;

  switch (dim) {
  case 2:
    if (doConvert) {
      nOut[0] = 0;
      nOut[1] = 0;
      nOut[2] = 0;
      nOut[3] = 0;
    } else {
      for (i = 0; i < attrDim; i++) {
	nOut[0 * attrDim + i] = 0;
	nOut[1 * attrDim + i] = 0;
	nOut[2 * attrDim + i] = 0;
	nOut[3 * attrDim + i] = 0;
      }
    }

    cOut[0 * dim + visX] = 0;
    cOut[0 * dim + visY] = 0;
    
    cOut[1 * dim + visX] = fieldAccess->getSize(visX);
    cOut[1 * dim + visY] = 0;
    
    cOut[2 * dim + visX] = 0;
    cOut[2 * dim + visY] = fieldAccess->getSize(visY);
    
    cOut[3 * dim + visX] = fieldAccess->getSize(visX);
    cOut[3 * dim + visY] = fieldAccess->getSize(visY);
    
    conOut[0 * 2 + 0] = 0;
    conOut[0 * 2 + 1] = 1;
    conOut[1 * 2 + 0] = 1;
    conOut[1 * 2 + 1] = 3;
    conOut[2 * 2 + 0] = 3;
    conOut[2 * 2 + 1] = 2;
    conOut[3 * 2 + 0] = 2;
    conOut[3 * 2 + 1] = 0;
    break;
    
  case 3:
    
    if (doConvert) {
      nOut[0] = 0;
      nOut[1] = 0;
      nOut[2] = 0;
      nOut[3] = 0;
      nOut[4] = 0;
      nOut[5] = 0;
      nOut[6] = 0;
      nOut[7] = 0;
    } else {
      for (i = 0; i < attrDim; i++) {
	nOut[0 * attrDim + i] = 0;
      }
      for (i = 0; i < attrDim; i++) {
	nOut[1 * attrDim + i] = 0;
      }
      for (i = 0; i < attrDim; i++) {
	nOut[2 * attrDim + i] = 0;
      }
      for (i = 0; i < attrDim; i++) {
	nOut[3 * attrDim + i] = 0;
      }
      for (i = 0; i < attrDim; i++) {
	nOut[4 * attrDim + i] = 0;
      }
      for (i = 0; i < attrDim; i++) {
	nOut[5 * attrDim + i] = 0;
      }
      for (i = 0; i < attrDim; i++) {
	nOut[6 * attrDim + i] = 0;
      }
      for (i = 0; i < attrDim; i++) {
	nOut[7 * attrDim + i] = 0;
      }
    }
    
    cOut[0 * dim + visX] = 0;
    cOut[0 * dim + visY] = 0;
    cOut[0 * dim + visZ] = 0;
    
    cOut[1 * dim + visX] = fieldAccess->getSize(visX);
    cOut[1 * dim + visY] = 0;
    cOut[1 * dim + visZ] = 0;
    
    cOut[2 * dim + visX] = 0;
    cOut[2 * dim + visY] = fieldAccess->getSize(visY);
    cOut[2 * dim + visZ] = 0;
    
    cOut[3 * dim + visX] = fieldAccess->getSize(visX);
    cOut[3 * dim + visY] = fieldAccess->getSize(visY);
    cOut[3 * dim + visZ] = 0;
    
    cOut[4 * dim + visX] = 0;
    cOut[4 * dim + visY] = 0;
    cOut[4 * dim + visZ] = fieldAccess->getSize(visZ);
    
    cOut[5 * dim + visX] = fieldAccess->getSize(visX);
    cOut[5 * dim + visY] = 0;
    cOut[5 * dim + visZ] = fieldAccess->getSize(visZ);
    
    cOut[6 * dim + visX] = 0;
    cOut[6 * dim + visY] = fieldAccess->getSize(visY);
    cOut[6 * dim + visZ] = fieldAccess->getSize(visZ);
    
    cOut[7 * dim + visX] = fieldAccess->getSize(visX);
    cOut[7 * dim + visY] = fieldAccess->getSize(visY);
    cOut[7 * dim + visZ] = fieldAccess->getSize(visZ);
    
    conOut[0  * 2 + 0] = 0;
    conOut[0  * 2 + 1] = 1;
    conOut[1  * 2 + 0] = 1;
    conOut[1  * 2 + 1] = 3;
    conOut[2  * 2 + 0] = 3;
    conOut[2  * 2 + 1] = 2;
    conOut[3  * 2 + 0] = 2;
    conOut[3  * 2 + 1] = 0;
    
    conOut[4  * 2 + 0] = 4;
    conOut[4  * 2 + 1] = 5;
    conOut[5  * 2 + 0] = 5;
    conOut[5  * 2 + 1] = 7;
    conOut[6  * 2 + 0] = 7;
    conOut[6  * 2 + 1] = 6;
    conOut[7  * 2 + 0] = 6;
    conOut[7  * 2 + 1] = 4;
    
    conOut[8  * 2 + 0] = 0;
    conOut[8  * 2 + 1] = 4;
    conOut[9  * 2 + 0] = 1;
    conOut[9  * 2 + 1] = 5;
    conOut[10 * 2 + 0] = 2;
    conOut[10 * 2 + 1] = 6;
    conOut[11 * 2 + 0] = 3;
    conOut[11 * 2 + 1] = 7;
    break;
  }

  // Set min and max value;

  OMset_data_type((OMobj_id)out.node_data[0].min,OM_TYPE_FLOAT);
  OMset_data_type((OMobj_id)out.node_data[0].max,OM_TYPE_FLOAT);
  out.node_data[0].min = 0;
  out.node_data[0].max = 0;

  if (conOut)
    ARRfree((char *)conOut);

  if (nOut)
    ARRfree((char *)nOut);
  
  if (cOut)
    ARRfree((char *)cOut);
  
  visSETSTATUSIDLE;

  // return 1 for success

  return 1;
}

int HiVis_HiVisMods_HiVis_Render_Streamline::upd(OMevent_mask, int seq_num)
{
   // max_Steps (OMXint read req notify)
   // out (Mesh+Node_Data write)

   /***********************/
   /* Function's Body     */
   /***********************/

  // if manual update is enabled and no change is signed then return

  if (manual_Update.changed(seq_num) ||
      (manual_Update && !do_Update.changed(seq_num))) {
    return 1;
  }

  visMsgCall << "HiVis_HiVisMods_HiVis_Render_Streamline::"
	     << "upd: called." << visMsgEnd;
  
  // Get access to visAdaptiveGrid

  visGridLink *gridLink = 
     (visGridLink *) in.ret_class_ptr("visGridLink");

  if (!gridLink) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Streamline::"
		<< "upd: in.gridLink is uninitialized" 
		<< visMsgErrorEnd;
    return 0;
  }

  visAdaptiveGrid *fieldAccess = gridLink->castToAdaptiveGrid();

  if (!fieldAccess || !fieldAccess->isAllocated()) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Streamline::"
		<< "upd: in.fieldAccess is uninitialized."
		<< visMsgErrorEnd;
    return 0;
  }

  visAdaptiveGrid *nodeGrid = fieldAccess;
  
  // Check if field is cell associated

  if (!(nodeGrid->getAttrAssoc() & visHasNodeData)) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Streamline::upd"
		<< ": field must contain node associated data."
		<< visMsgErrorEnd;
    return false;
  }
  
  // Profiling for Linear Interpolation

  if (nodeGrid->instanceOf(visCell2NodeMLinearId)) {
    ((visCell2NodeMLinear*)nodeGrid)->myGetLinearInterpValueCalls
      = 0;
    ((visCell2NodeMLinear*)nodeGrid)->myGetLinearInterpValueCachedCalls
      = 0;
    ((visCell2NodeMLinear*)nodeGrid)->myGetEdgeAttributeCalls
      = 0;
    ((visCell2NodeMLinear*)nodeGrid)->myGetEdgeAttributeCachedCalls
      = 0;
    ((visCell2NodeMLinear*)nodeGrid)->mySetCursorCalls
      = 0;
    ((visCell2NodeMLinear*)nodeGrid)->mySetCursorCachedCalls
      = 0;
  }

  // Now as this class is allocated, also allocate nodeGrid class

  int dim = fieldAccess->getDim();

  if ((int) probe.nspace != dim) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Streamline::"
		<< "upd: probe must have same dimension as field."
		<< visMsgErrorEnd;
    return 0;
  }

  if (dim < 2 || dim > 3) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Streamline::"
		  << "upd: Only two or three dimensional fields "
		  << "supported."
		  << visMsgErrorEnd;
     return 0;
  }

  int theOrder = (int) order;
  if (theOrder != 1 && theOrder != 4) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Streamline::"
		<< "upd: Yet order can only be equal to 1 (Euler-Cauchy-) "
		<< "or 4 (Runge-Kutta-Interpolation)"
		<< visMsgErrorEnd;
    return 0;
  }
  
  int i;
  int vectorAttribute = (int) comp_Vector;

  if (vectorAttribute < 0 || 
      vectorAttribute >= nodeGrid->getAttributes()) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Streamline::"
		<< "upd: Vector component out of range."
		<< visMsgErrorEnd;
    return 0;
  }

  visAttributeFactory** attrFactory =
    new visAttributeFactory*[nodeGrid->getAttributes()];
  for (i = 0; i < nodeGrid->getAttributes(); i++) {
    attrFactory[i] = nodeGrid->getAttrFactory(i);
  }
  
  if (attrFactory[vectorAttribute]->getDim() != dim) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Streamline::"
		<< "upd: Vector dimension must be equal to field dimension."
		<< visMsgErrorEnd;
    return 0;
  }
  
  // Get field dimension

  out.nspace = dim;

  // Sanity checks

  int maxSteps = (int) max_Steps;
  int remainingSteps = maxSteps;

  // Define node_data
  
  out.nnode_data = 1;
  out.node_data[0].veclen = 1; 

  // Initialize min and max value
  

  visAttribute *minAttr = (visAttribute *) 
    nodeGrid->getMinAttribute(vectorAttribute);
  visAttribute *maxAttr = (visAttribute *) 
    nodeGrid->getMaxAttribute(vectorAttribute);
  double minVal = minAttr->getScalarValue(nodeGrid->getDim());
  double maxVal = maxAttr->getScalarValue(nodeGrid->getDim());

  minAttr->decRef();
  maxAttr->decRef();

  float aPos[3];
  float bPos[3];

  float *probePos = (float*)
    probe.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
  if (!probePos) {
    visMsgError << "HiVis_HiVisMods_HiVis_Render_Streamline::upd: "
		<< "couldn't get probe coordinates." << visMsgErrorEnd;
    return 0;
  }
  float *mat = (float *) probe.xform.xlate.
    ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_FLOAT);
  for (i = 0; i < dim; i++) {
    aPos[i] = probePos[i] + mat[i];
  }
  ARRfree((char *)mat);
  ARRfree((void*) probePos);
  
  /*  pos[visX] = probe.coordinates.values;
  pos[visY] = nodeGrid->getSize(visX) / 2;
  pos[visZ] = nodeGrid->getSize(visX) / 2;*/
  //  int iPos[3];

  int stepCount = (int) steps_Per_Cell;
  
  visStack<float> streamCoord;
  visStack<float> streamVector;
  visStack<float> streamDir;
  visStack<visAttribute*> streamAttribute;

  visAttribute *attr = attrFactory[vectorAttribute]->getNewAttribute();
  double aVector[3], bVector[3], cVector[3];

  if (nodeGrid->setCursorF(aPos)) {
    nodeGrid->getCellAttributeF(vectorAttribute, attr, aPos);
    bVector[0] = - (double) attr->getValue(1);
    bVector[1] = (double) attr->getValue(0);
    bVector[2] = (double) attr->getValue(2);

    double l, m;
    
    while (--remainingSteps >= 0) {

      visSYSTEM.setStatus("HiVis Streamline", 
			  (float) (maxSteps - remainingSteps) / maxSteps);

      // Check if aPos in field
      
      if (!nodeGrid->setCursorF(aPos)) {
	visMsgDbg << "aPos out of field" << visMsgEnd;
	break;
      }

      // Push attributes on stack
      
      for (i = 0; i < nodeGrid->getAttributes(); i++) {
	visAttribute *a = attrFactory[i]->getNewAttribute();
	nodeGrid->getCellAttributeF(i, a, aPos);
	streamAttribute.push(a);
      }
      
      attr->setValue(dim, streamAttribute[streamAttribute.getSize() - 
					 nodeGrid->getAttributes() +
					 vectorAttribute]);
      
      // get vector a

      for (i = 0; i < dim; i++) {
	aVector[i] = attr->getValue(i);
      }

      /*      visMsgDbg << "raw: aPos=("
		<< aPos[0] << "," << aPos[1] << "," << aPos[2] << ") "
		<< "bVector=("
		<< bVector[0] << "," << bVector[1] << "," << bVector[2] << ") "
		<< "aVector=("
		<< aVector[0] << "," << aVector[1] << "," << aVector[2] << ") "
		<< visMsgEnd;*/
      // normalize a vector

      l = 0;
      for (i = 0; i < dim; i++) {
	double x = aVector[i];
	l += x * x;
      }
      if (l == 0) {
	visMsgDbg << "Length is zero (A)." << visMsgEnd;

	for (i = 0; i < dim; i++) {
	  visMsgDbg << "Pos[" << i << "] = " << aPos[i] << visMsgEnd;
	}
	nodeGrid->setCursorF(aPos);
	nodeGrid->getCellAttributeF(vectorAttribute, attr, aPos);
	for (i = 0; i < dim; i++) {
	  visMsgDbg << "Val[" << i << "] = " << attr->getValue(i) << visMsgEnd;
	}
	break;
      }

      m = (double) 1 / sqrt(l);
      for (i = 0; i < dim; i++) {
	aVector[i] *= m;
      }
      
      // align b vector to be orthogonal to stream
      
      m = 0;
      for (i = 0; i < dim; i++) {
	m += aVector[i] * bVector[i];
      }
      for (i = 0; i < dim; i++) {
	bVector[i] -= m * aVector[i];
      }
      
      // normalize b vector
      
      l = 0;
      for (i = 0; i < dim; i++) {
	l += bVector[i] * bVector[i];
      }
      if (l == 0) {
	visMsgDbg << "bVector length is zero." << visMsgEnd;
	break;
      }
      m = (double) 1 / sqrt(l);
      for (i = 0; i < dim; i++) {
	bVector[i] *= m;
      }

      /*      visMsgDbg << "proc: aPos=("
		<< aPos[0] << "," << aPos[1] << "," << aPos[2] << ") "
		<< "bVector=("
		<< bVector[0] << "," << bVector[1] << "," << bVector[2] << ") "
		<< "aVector=("
		<< aVector[0] << "," << aVector[1] << "," << aVector[2] << ") "
		<< visMsgEnd;*/
      // push a vector on stack
      
      for (i = 0; i < dim; i++) {
	streamDir.push(aVector[i]);
      }
      
      // Push pos a on stack
      
      for (i = 0; i < dim; i++) {
	streamCoord.push(aPos[i]);
      }

      // push b vector on stack
      
      for (i = 0; i < dim; i++) {
	streamVector.push(bVector[i]);
      }
      
      // calculate pos b
      
      for (i = 0; i < dim; i++) {
	bPos[i] = aPos[i] + bVector[i] / stepCount;
      }
      if (!nodeGrid->setCursorF(bPos)) {
	visMsgDbg << "bPos out of field." << visMsgEnd;
	break;
      }
      nodeGrid->getCellAttributeF(vectorAttribute, attr, bPos);
      
      // calculate new pos b
      
      for (i = 0; i < dim; i++) {
	cVector[i] = attr->getValue(i);
      }
      l = 0;
      for (i = 0; i < dim; i++) {
	l += cVector[i] * cVector[i];
      }
      if (l == 0) {
	break;
      }
      l = sqrt(l);
      m = (double) 1 / (l * stepCount);
      if (stream_Go_Backward) {
	m = -m;
      }
      for (i = 0; i < dim; i++) {
	bPos[i] += cVector[i] * m;
      }

      // calculate new pos a
      
      m = (double) 1 / stepCount;
      if (stream_Go_Backward) {
	m = -m;
      }
      
      for (i = 0; i < dim; i++) {
	aPos[i] += aVector[i] * m;
      }

      // calculate new vector b

      for (i = 0; i < dim; i++) {
	bVector[i] = bPos[i] - aPos[i];
      }
    }
  }
  attr->decRef();

  
#define visStreamVisLine   1
#define visStreamVisRibbon 2
#define visStreamVisObject 3
#define visStreamVisTube   4

  float ribbonScale = (float) scale_Ribbon;
  float objectScale = (float) scale_Object;
  float stripeScale = (float) scale_Stripe;
  bool ribbonInverseFaces = (bool) ribbon_Inverse_Faces;
  int stripeAttrComp = (int) comp_Stripe;
  int colorAttrComp = (int) comp_Color;
  int scaleAttrComp = (int) comp_Scale;
  int streamVisType = (int) vis_Type;
  int scaleXAttrComp = (int) comp_ScaleX;
  
  visMsgDbg << "Stream built" << visMsgEnd;

  // Profiling for Linear Interpolation

  if (nodeGrid->instanceOf(visCell2NodeMLinearId)) {
    visMsgDbg << "getLinearInterpValue() calls = " << 
      ((visCell2NodeMLinear*)nodeGrid)->myGetLinearInterpValueCalls
	      << ", cached = " << 
      ((visCell2NodeMLinear*)nodeGrid)->myGetLinearInterpValueCachedCalls
	      << ", ratio = " << 
      (float)
      ((visCell2NodeMLinear*)nodeGrid)->myGetLinearInterpValueCachedCalls
      /
      ((visCell2NodeMLinear*)nodeGrid)->myGetLinearInterpValueCalls
	      << visMsgEnd;

    visMsgDbg << "getNodeAttribute() calls = " << 
      ((visCell2NodeMLinear*)nodeGrid)->myGetEdgeAttributeCalls
	      << ", cached = " << 
      ((visCell2NodeMLinear*)nodeGrid)->myGetEdgeAttributeCachedCalls
	      << ", ratio = " << 
      (float)
      ((visCell2NodeMLinear*)nodeGrid)->myGetEdgeAttributeCachedCalls
      /
      ((visCell2NodeMLinear*)nodeGrid)->myGetEdgeAttributeCalls
	      << visMsgEnd;

    visMsgDbg << "setCursor() calls = " << 
      ((visCell2NodeMLinear*)nodeGrid)->mySetCursorCalls
	      << ", cached = " << 
      ((visCell2NodeMLinear*)nodeGrid)->mySetCursorCachedCalls
	      << ", ratio = " << 
      (float)
      ((visCell2NodeMLinear*)nodeGrid)->mySetCursorCachedCalls
      /
      ((visCell2NodeMLinear*)nodeGrid)->mySetCursorCalls
	      << visMsgEnd;
  }

  if (streamVisType == visStreamVisLine) {
    
    // Init AVS/Express field structure
    
    out.ncell_sets = 1;
    out.cell_set[0].cell_nnodes = 2;
    out.cell_set[0].cell_ndim = 1;
    out.cell_set[0].cell_order = 1;
    
    out.cell_set[0].ncells = streamCoord.getSize() / dim - 1; // number of lines
    out.nnodes = streamCoord.getSize() * 2 / dim; // number of points
    
    out.cell_set[0].poly_flag = 0;
    int *conOut = (int *)
      out.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
    float *cOut = (float *)
      out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
    float *nOut = (float*)
      out.node_data[0].values.
      ret_typed_array_ptr(OM_GET_ARRAY_WR, OM_TYPE_FLOAT);
    //    e->setCursorFirstElement();
    //    while (e->isValidElement()) {
    //      numberOfCells--;
    
    int index;
    visMsgDbg << "size = " << streamAttribute.getSize() << visMsgEnd;

    // variables for percentage status
    
    int percent = 0;

    for (index = 0; index < streamCoord.getSize() / dim; index++) {

      visSYSTEM.setStatus("HiVis Streamline", 
			  (float) index * dim / streamCoord.getSize());
      
      for (i = 0; i < dim; i++) {
	cOut[index * dim + i] = streamCoord[index * dim + i];
      }
      if (colorAttrComp >= 0) {
	nOut[index] = streamAttribute[index * nodeGrid->getAttributes()
				     + colorAttrComp]->
	  getScalarValue(attrFactory[colorAttrComp]->getDim());
      } else {
	nOut[index] = 1;
      }
      if (index < streamCoord.getSize() / dim - 1) {
	conOut[2 * index] = index;
	conOut[2 * index + 1] = index + 1;
      }
    }
    // Set min and max value;
    
    OMset_data_type((OMobj_id)out.node_data[0].min,OM_TYPE_FLOAT);
    OMset_data_type((OMobj_id)out.node_data[0].max,OM_TYPE_FLOAT);
    if (colorAttrComp >= 0) {
      out.node_data[0].min = minVal;
      out.node_data[0].max = maxVal;
    } else {
      out.node_data[0].min = 1;
      out.node_data[0].max = 1;
    }
    
    ARRfree((char *)conOut);
    ARRfree((char *)nOut);
    ARRfree((char *)cOut);
    
  }
  else if (streamVisType == visStreamVisRibbon) {
    // Init AVS/Express field structure
    
    out.ncell_sets = 1;
    out.cell_set[0].cell_nnodes = 4;
    out.cell_set[0].cell_ndim = 2;
    out.cell_set[0].cell_order = 1;
    
    // number of faces
    out.cell_set[0].ncells = streamCoord.getSize() / dim - 1; 
    out.nnodes = streamCoord.getSize() * 2 / dim; // number of points
    
    out.cell_set[0].poly_flag = 0;
    int *conOut = (int *)
      out.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
    float *cOut = (float *)
      out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);

    
    float *nOut = (float*)
      out.node_data[0].values.
      ret_typed_array_ptr(OM_GET_ARRAY_WR, OM_TYPE_FLOAT);
    
    int index;
    //visMsgDbg << "size = " << streamAttribute.getSize() << visMsgEnd;

    float stripeX, stripeXa, stripeXi, stripeVal[2] = { 0, 1 };
    int stripeValIndex;
    if (stripeAttrComp >= 0) {
      stripeXa = (float) stripeScale / streamAttribute[stripeAttrComp]->
	getScalarValue(attrFactory[stripeAttrComp]->getDim());
      stripeX = stripeXa;
      stripeValIndex = 0;
    }

    for (index = 0; index < streamCoord.getSize() / dim; index++) {

      visSYSTEM.setStatus("HiVis Streamline",
			  (float) index * dim / streamCoord.getSize());

      for (i = 0; i < dim; i++) {
	aPos[i] = streamCoord[index * dim + i];
	bPos[i] = aPos[i] + streamVector[index * dim + i] * ribbonScale;
      }
      
      for (i = 0; i < dim; i++) {
	cOut[(2 * index + 0) * dim + i] = aPos[i];
	cOut[(2 * index + 1) * dim + i] = bPos[i];
      }

      if (stripeAttrComp < 0) {
	if (colorAttrComp >= 0) {
	nOut[index * 2 + 0] = nOut[index * 2 + 1] =
	  streamAttribute[index * nodeGrid->getAttributes()
			 + colorAttrComp]->
	  getScalarValue(attrFactory[colorAttrComp]->getDim());
	} else {
	  nOut[index * 2 + 0] = nOut[index * 2 + 1] = 1;
	}
      } else {
	nOut[index * 2 + 0] = nOut[index * 2 + 1] = stripeVal[stripeValIndex];
	stripeXi = 
	  (float) 1 / (streamAttribute[index * nodeGrid->getAttributes()
				      + stripeAttrComp]->
		       getScalarValue(attrFactory[stripeAttrComp]->getDim()) *
		       stepCount);
	stripeX -= stripeXi;
	if (stripeX < 0) {
	  stripeX += stripeXa;
	  stripeValIndex = (stripeValIndex == 1 ? 0 : 1);
	}
      }
      if (index < streamCoord.getSize() / dim - 1) {
	if (!ribbonInverseFaces) {
	  conOut[4 * index + 0] = 2 * index + 0;
	  conOut[4 * index + 1] = 2 * index + 1;
	  conOut[4 * index + 2] = 2 * index + 3;
	  conOut[4 * index + 3] = 2 * index + 2;
	} else {
	  conOut[4 * index + 0] = 2 * index + 1;
	  conOut[4 * index + 1] = 2 * index + 0;
	  conOut[4 * index + 2] = 2 * index + 2;
	  conOut[4 * index + 3] = 2 * index + 3;
	}
      }
    }    // Set min and max value;
    
    OMset_data_type((OMobj_id)out.node_data[0].min,OM_TYPE_FLOAT);
    OMset_data_type((OMobj_id)out.node_data[0].max,OM_TYPE_FLOAT);
    if (stripeAttrComp < 0) {
      if (colorAttrComp >= 0) {
	out.node_data[0].min = minVal;
	out.node_data[0].max = maxVal;
      } else {
	out.node_data[0].min = 1;
	out.node_data[0].max = 1;
      }
    } else {
      out.node_data[0].min = stripeVal[0];
      out.node_data[0].max = stripeVal[1];
    }
    
    if (conOut)
      ARRfree((char *)conOut);
    
    if (nOut)
      ARRfree((char *)nOut);
    
    if (cOut)
      ARRfree((char *)cOut);
    
  }
  else if (streamVisType == visStreamVisObject) {

    bool valid = true;
    if (valid && !particle_Shape.valid_obj()) {
      visMsgError << "HiVis_Render_Streamline: particle_Shape undefined."
		  << visMsgErrorEnd;
      valid = false;
    }
      
    if (valid && particle_Shape.nspace != dim) {
      visMsgError << "HiVis_Render_Streamline: particle_Shape must have"
		  << " same dimension as field."
		  << visMsgErrorEnd;
      valid = false;
    }
    
    if (valid) {

      // get stream indices of locations of the object

      float stripeX, stripeXa, stripeXi;
      visStack<int> streamIndices;
      if (stripeAttrComp < 0) {
	stripeAttrComp = vectorAttribute;
      }
      stripeXa = (float) stripeScale / streamAttribute[stripeAttrComp]->
	getScalarValue(attrFactory[stripeAttrComp]->getDim());
      stripeX = stripeXa;
      int index;
      for (index = 0; index < streamCoord.getSize() / dim; index++) {
	stripeXi = 
	  (float) 1 / (streamAttribute[index * nodeGrid->getAttributes()
			      + stripeAttrComp]->
	       getScalarValue(attrFactory[stripeAttrComp]->getDim()) *
	       stepCount);
	stripeX -= stripeXi;
	if (stripeX < 0) {
	  stripeX += stripeXa;
	  streamIndices.push(index);
	}
      }

      int streamIndicesSize = streamIndices.getSize();

      int nCellSets = (int) particle_Shape.ncell_sets;
      out.ncell_sets = nCellSets;
      int nNodes = (int) particle_Shape.coordinates.nvals;
      out.nnodes = streamIndicesSize * nNodes;
      for (i = 0; i < nCellSets; i++) {
	out.cell_set[i].cell_nnodes = 
	  particle_Shape.cell_set[i].cell_nnodes;
	out.cell_set[i].ncells = 
	  streamIndicesSize * particle_Shape.cell_set[i].ncells;
	out.cell_set[i].cell_ndim = 
	  particle_Shape.cell_set[i].cell_ndim;
	out.cell_set[i].cell_order = 
	  particle_Shape.cell_set[i].cell_order;
	out.cell_set[i].poly_flag = 
	  particle_Shape.cell_set[i].poly_flag;
      }

      float *cIn = (float *) 
	particle_Shape.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
      float *cInT = new float[nNodes * dim];
      for (i = 0; i < nNodes * dim; i++) {
	cInT[i] = cIn[i];
      }
      ARRfree((char *)cIn);

      float *nOut = (float*)
	out.node_data[0].values.
	ret_typed_array_ptr(OM_GET_ARRAY_WR, OM_TYPE_FLOAT);

      float *cOut = (float *) 
	out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);

      int *consIn = new int[nCellSets];
      int **conIn = new int*[nCellSets];
      int **conOut = new int*[nCellSets];
      for (i = 0; i < nCellSets; i++) {
	conOut[i] = (int *)
	  out.cell_set[i].node_connect_list.
	  ret_array_ptr(OM_GET_ARRAY_WR);
	consIn[i] = (int) particle_Shape.cell_set[i].ncells *
	  (int) particle_Shape.cell_set[i].cell_nnodes;
	conIn[i] = (int *)
	  particle_Shape.cell_set[i].node_connect_list.
	  ret_array_ptr(OM_GET_ARRAY_RD);
      }

      float *tMat = new float[16];

      // transform object coordinates with object xform

      float *partMat = (float *) particle_Shape.xform.mat.
	ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_FLOAT);
      for (i = 0; i < 16; i++) {
	tMat[i] = partMat[i];
      }
      ARRfree((char*) partMat);
      if (objectScale >= 0) {
	if (objectScale == 0) {
	  tMat[15] *= (float) visMAXFLOAT;
	} else {
	  tMat[15] /= objectScale;
	}
      }
      for (i = 0; i < nNodes; i++) {
	MATvec3_mat4_multiply(&cInT[i * dim], (Matr4 *) tMat);
      }

      // initialize last row

      for (i = 0; i < dim; i++) {
	tMat[4 * i + 3] = 0;
      }
      tMat[15] = 1;

      // fill object scale factor

      float scale;
      if (scaleAttrComp < 0) {
	scale = 1;
      } else {
	if (streamAttribute[scaleAttrComp]->
	    getScalarValue(attrFactory[scaleAttrComp]->getDim()) == 0) {
	  scale = visMAXFLOAT;
	} else {
	  scale = (float) 1 /
	    streamAttribute[scaleAttrComp]->
	    getScalarValue(attrFactory[scaleAttrComp]->getDim());
	}
      }

      // fill object scaleX factor

      float scaleX;
      if (scaleXAttrComp < 0) {
	scaleX = 1;
      } else {
	if (streamAttribute[scaleXAttrComp]->
	    getScalarValue(attrFactory[scaleXAttrComp]->getDim()) == 0) {
	  scaleX = visMAXFLOAT;
	} else {
	  scaleX = (float) 1 /
	    streamAttribute[scaleXAttrComp]->
	    getScalarValue(attrFactory[scaleXAttrComp]->getDim());
	}
      }

      for (index = 0; index < streamIndicesSize; index++) {
	
	visSYSTEM.setStatus("HiVis Streamline",
			    (float) index / streamIndicesSize);
	int set, j;
	for (set = 0; set < nCellSets; set++) {
	  for (i = 0; i < consIn[set]; i++) {
	    conOut[set][index * consIn[set] + i] =
	      index * nNodes + conIn[set][i];
	  }
	}

	for (j = 0; j < dim; j++) {
	  // fill translation part
	  tMat[4 * 3 + j] = streamCoord[streamIndices[index] * dim + j];
	  // fill rotation part
	  tMat[4 * 0 + j] = streamDir[streamIndices[index] * dim + j];
	  tMat[4 * 1 + j] = streamVector[streamIndices[index] * dim + j];
	}

	// vector multiply last component
	tMat[4 * 2 + 0] = 
	  streamDir[streamIndices[index] * dim + 1] *
	  streamVector[streamIndices[index] * dim + 2] -
	  streamDir[streamIndices[index] * dim + 2] *
	  streamVector[streamIndices[index] * dim + 1];
	tMat[4 * 2 + 1] = 
	  streamDir[streamIndices[index] * dim + 2] *
	  streamVector[streamIndices[index] * dim + 0] -
	  streamDir[streamIndices[index] * dim + 0] *
	  streamVector[streamIndices[index] * dim + 2];
	tMat[4 * 2 + 2] = 
	  streamDir[streamIndices[index] * dim + 0] *
	  streamVector[streamIndices[index] * dim + 1] -
	  streamDir[streamIndices[index] * dim + 1] *
	  streamVector[streamIndices[index] * dim + 0];

	if (scaleAttrComp >= 0) {

	  float m = scale * 
	    streamAttribute[streamIndices[index]
			   * nodeGrid->getAttributes()
			   + scaleAttrComp]->
	    getScalarValue(attrFactory[scaleAttrComp]->getDim());
	  
	  for (j = 0; j < dim; j++) {
	    tMat[4 * j + 0] *= m;
	    tMat[4 * j + 1] *= m;
	    tMat[4 * j + 2] *= m;
	  }
	}
	
	if (scaleXAttrComp >= 0) {
	  float m = scaleX * 
	    streamAttribute[streamIndices[index]
			   * nodeGrid->getAttributes()
			   + scaleXAttrComp]->
	    getScalarValue(attrFactory[scaleXAttrComp]->getDim());
	  
	  for (j = 0; j < dim; j++) {
	    tMat[4 * j + visX] *= m;
	  }
	}

	float val;
	if (colorAttrComp >= 0) {
	  val = streamAttribute[streamIndices[index]
			       * nodeGrid->getAttributes()
			       + colorAttrComp]->
	    getScalarValue(attrFactory[colorAttrComp]->getDim());
	} else {
	  val = 1;
	}

	float X[3];
	for (i = 0; i < nNodes; i++) {
	  for (j = 0; j < dim; j++) {
	    X[j] = cInT[i * dim + j];
	  }
	  MATvec3_mat4_multiply(X, (Matr4 *) tMat);
	  for (j = 0; j < dim; j++) {
	    cOut[(index * nNodes + i) * dim + j] = X[j];
	  }
	  nOut[index * nNodes + i] = val;
	}
      }

      delete[] tMat;

      OMset_data_type((OMobj_id)out.node_data[0].min,OM_TYPE_FLOAT);
      OMset_data_type((OMobj_id)out.node_data[0].max,OM_TYPE_FLOAT);
      if (colorAttrComp >= 0) {
	out.node_data[0].min = minVal;
	out.node_data[0].max = maxVal;
      } else {
	out.node_data[0].min = 1;
	out.node_data[0].max = 1;
      }

      for (i = 0; i < nCellSets; i++) {
	ARRfree((char *)conOut[i]);
	ARRfree((char *)conIn[i]);
      }
      delete[] conOut;
      delete[] conIn;
      delete[] consIn;
      ARRfree((char *)cOut);
      ARRfree((char *)nOut);
      delete[]cInT;
    }
  }

  for (i = 0; i < streamAttribute.getSize(); i++) {
    streamAttribute[i]->decRef();
  }
  for(i = 0; i < nodeGrid->getAttributes(); i++) {
    attrFactory[i]->decRef();
  }
  delete[] attrFactory;

  // correct scale factor

#ifdef visScaleCorrect
  /*  if (nodeGrid->getDim() == 3) {
    float scaleMatrix[16] = {
      float(nodeGrid->getSize(visX) + 1) / nodeGrid->getSize(visX), 0, 0, 0,
      0, float(nodeGrid->getSize(visY) + 1) / nodeGrid->getSize(visY), 0, 0,
      0, 0, float(nodeGrid->getSize(visZ) + 1) / nodeGrid->getSize(visZ), 0,
      0, 0, 0, 1,
    };

    // Scale

    float *inMat = (float *) in.xform.mat.
      ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_FLOAT);
    float *outMat = (float *) out.xform.mat.
      ret_typed_array_ptr(OM_GET_ARRAY_WR, OM_TYPE_FLOAT);
    MATmat4_multiply((Matr4 *)inMat, (Matr4 *)scaleMatrix, (Matr4 *)outMat);
    ARRfree((char *)inMat);
    ARRfree((char *)outMat);
    }*/
#endif

  visSETSTATUSIDLE;

  // return 1 for success

  return 1;
}

int HiVis_HiVisMods_HiVis_Debug::upd(OMevent_mask, int)
{
  // probe_Attribute (OMXint read req notify)
  // probe_Pos (OMXint read req notify)
  // pos (OMXint read req notify)
  // cell_No_Overlap (OMXint write)
  // cell_Complete (OMXint write)
  // good_NumberOfCells (OMXint write)
  
  /***********************/
  /* Function's Body     */
  /***********************/
  visMsgCall << "HiVis_HiVisMods_HiVis_Debug::"
	     << "upd: called." << visMsgEnd;
  
  // Get access to visAdaptiveGrid

  visGridLink *gridLink = 
    (visGridLink *) in.ret_class_ptr("visGridLink");

  if (!gridLink) {
    visMsgError << "HiVis_HiVisMods_HiVis_Debug::"
		<< "upd: in.gridLink is uninitialized" 
		<< visMsgErrorEnd;
    return 0;
  }

  visAdaptiveGrid *fieldAccess = gridLink->castToAdaptiveGrid();

  if (!fieldAccess) {
    visMsgError << "HiVis_HiVisMods_HiVis_Debug::"
		<< "upd: in.fieldAccess is uninitialized."
		<< visMsgErrorEnd;
    return 0;
   }

  visAdaptiveGrid *nodeGrid = fieldAccess;
  
  // Now as this class is allocated, also allocate nodeGrid class

  int dim = fieldAccess->getDim();

  if (!nodeGrid->isAllocated()) {
    visMsgError << "HiVis_HiVisMods_HiVis_Debug::"
		<< "upd: in.fieldAccess->getNodeGrid() is unallocated."
		<< visMsgErrorEnd;
    return 0;
  }

  int *pos = (int*) probe_Pos.ret_array_ptr(OM_GET_ARRAY_RD);
  int attribute = (int) probe_Attribute;

  visAttributeFactory *factory = fieldAccess->getAttrFactory(attribute);
  if (!pos) {
    visMsgDbg << "HiVis_HiVisMods_HiVis_Debug::"
	      << "upd: couldn't get probe_Pos array."
	      << visMsgEnd;
    nodeGrid->decRef();
    return 0;
  }

  visMsgError << "HiVis_Debug:"	<< visMsgErrorEnd;
  visMsgError << "  Vector dim = " << factory->getDim() << visMsgErrorEnd;

  fieldAccess->setCursor(pos);
  visAttribute *attr = factory->getNewAttribute();
  fieldAccess->getCellAttribute(attribute, attr);
  int i;
  for (i = 0; i < factory->getDim(); i++) {
    visMsgError << "  Cell Vector[" << i << "] = " << attr->getValue(i) 
		<< visMsgErrorEnd;
  }

  nodeGrid->setCursor(pos);
  int j;
  for (j = 0; j < (1 << dim); j++) {
    nodeGrid->getNodeAttribute(attribute, attr, j);
    for (i = 0; i < factory->getDim(); i++) {
      visMsgError << "  Edge << " << j << " Vector[" << i << "] = "
		  << attr->getValue(i) << visMsgErrorEnd;
    }
  }

  attr->decRef();

  ARRfree((char *) pos);
  factory->decRef();
  nodeGrid->decRef();

  // return 1 for success

  return 1;
}

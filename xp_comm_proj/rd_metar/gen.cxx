/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */

#include "xp_comm_proj/rd_metar/gen.h"
#ifdef __cplusplus


//
// Program generated src definition for class: ReadMETARSCore_out
//
void *
ReadMETARSCore_out::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ReadMETARSCore_out"))
      return(this);
   void *ptr;
   if ((ptr = FLD_Mesh::ret_omx_ptr(name)) != NULL)
      return(ptr);
   if ((ptr = FLD_Node_Data::ret_omx_ptr(name)) != NULL)
      return(ptr);
   return(NULL);
}

ReadMETARSCore_out::ReadMETARSCore_out(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr,
const char *class_name) :
	OMXgroup(id, name, mode, parent_ptr, class_name),
	FLD_Mesh(id, name, mode, parent_ptr),
	FLD_Node_Data(id, name, mode, parent_ptr),
	nnodes(obj_id(OM_OBJ_RD),"nnodes",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{}


//
// Program generated src definition for class: ReadMETARSCore
//
void
ReadMETARSCore__free(void *obj)
{
   ReadMETARSCore* obj_ptr = (ReadMETARSCore* )
             ((OMXgroup *)obj)->ret_omx_ptr("ReadMETARSCore");
   ((OMXgroup *) obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *
ReadMETARSCore__inst(OMobj_id obj_id)
{
   ReadMETARSCore *obj_ptr = new ReadMETARSCore(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *
ReadMETARSCore::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ReadMETARSCore"))
      return(this);
   return(NULL);
}

ReadMETARSCore::ReadMETARSCore(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr,
const char *class_name) :
	OMXgroup(id, name, mode, parent_ptr, class_name),
	filename(obj_id(OM_OBJ_RD),"filename",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	StationID(obj_id(OM_OBJ_RD),"StationID",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	StationLocation(obj_id(OM_OBJ_RD),"StationLocation",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	out(obj_id(OM_OBJ_RD),"out",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{}
int ReadMETARSCore__read__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num)
{
   return(((ReadMETARSCore *) obj->ret_omx_ptr("ReadMETARSCore"))->read(event_mask, seq_num));
}

#endif /* end c++ section */

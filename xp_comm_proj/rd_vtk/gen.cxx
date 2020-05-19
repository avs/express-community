/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */

#include "xp_comm_proj\rd_vtk\gen.h"
#ifdef __cplusplus

//
// Program generated src definition for class: ReadVtk_ReadVtkMods_ReadVtkCore
//
void ReadVtk_ReadVtkMods_ReadVtkCore__free(void *obj)
{
   ((OMXgroup *)obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *ReadVtk_ReadVtkMods_ReadVtkCore__inst(OMobj_id obj_id)
{
   ReadVtk_ReadVtkMods_ReadVtkCore *obj_ptr = new ReadVtk_ReadVtkMods_ReadVtkCore(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *ReadVtk_ReadVtkMods_ReadVtkCore::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ReadVtk_ReadVtkMods_ReadVtkCore"))
      return(this);
   return(NULL);
}
ReadVtk_ReadVtkMods_ReadVtkCore::ReadVtk_ReadVtkMods_ReadVtkCore(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr, const char *class_name) :
   OMXgroup(id, name, mode, parent_ptr, class_name),
   filename(obj_id(OM_OBJ_RD),"filename",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   num_coords(obj_id(OM_OBJ_RD),"num_coords",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   num_connect(obj_id(OM_OBJ_RD),"num_connect",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   coords(obj_id(OM_OBJ_RD),"coords",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   connect(obj_id(OM_OBJ_RD),"connect",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{
}
int ReadVtk_ReadVtkMods_ReadVtkCore__Read__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num)
{
   return(((ReadVtk_ReadVtkMods_ReadVtkCore *)obj->ret_omx_ptr("ReadVtk_ReadVtkMods_ReadVtkCore"))->Read(event_mask, seq_num));
}
#endif /* end c++ section */

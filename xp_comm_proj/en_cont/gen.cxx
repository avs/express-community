/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */

#include "xp_comm_proj\en_cont\gen.hxx"
#ifdef __cplusplus

//
// Program generated src definition for class: EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore
//
void EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore__free(void *obj)
{
   ((OMXgroup *)obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore__inst(OMobj_id obj_id)
{
   EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore *obj_ptr = new EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore"))
      return(this);
   return(NULL);
}
EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore::EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr, const char *class_name) :
   OMXgroup(id, name, mode, parent_ptr, class_name),
   level(obj_id(OM_OBJ_RD),"level",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   in_dims(obj_id(OM_OBJ_RD),"in_dims",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   img(obj_id(OM_OBJ_RD),"img",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   out(obj_id(OM_OBJ_RD),"out",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{
}
int EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore__Enhance__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num)
{
   return(((EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore *)obj->ret_omx_ptr("EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore"))->Enhance(event_mask, seq_num));
}
#endif /* end c++ section */

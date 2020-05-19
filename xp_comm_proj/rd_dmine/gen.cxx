/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */

#include "xp_comm_proj\rd_dmine\gen.hxx"
#ifdef __cplusplus

//
// Program generated src definition for class: ReadDatamine_ReadDatamineMods_ReadDatamineParams
//
void ReadDatamine_ReadDatamineMods_ReadDatamineParams__free(void *obj)
{
   ((OMXgroup *)obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *ReadDatamine_ReadDatamineMods_ReadDatamineParams__inst(OMobj_id obj_id)
{
   ReadDatamine_ReadDatamineMods_ReadDatamineParams *obj_ptr = new ReadDatamine_ReadDatamineMods_ReadDatamineParams(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *ReadDatamine_ReadDatamineMods_ReadDatamineParams::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ReadDatamine_ReadDatamineMods_ReadDatamineParams"))
      return(this);
   return(NULL);
}
ReadDatamine_ReadDatamineMods_ReadDatamineParams::ReadDatamine_ReadDatamineMods_ReadDatamineParams(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr, const char *class_name) :
   OMXgroup(id, name, mode, parent_ptr, class_name),
   dir(obj_id(OM_OBJ_RD),"dir",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   filenames(obj_id(OM_OBJ_RD),"filenames",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   model_as_point(obj_id(OM_OBJ_RD),"model_as_point",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   model_scale(obj_id(OM_OBJ_RD),"model_scale",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   err(obj_id(OM_OBJ_RD),"err",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   err_str(obj_id(OM_OBJ_RD),"err_str",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{
}
ReadDatamine_ReadDatamineMods_ReadDatamineCore_fld_array::ReadDatamine_ReadDatamineMods_ReadDatamineCore_fld_array(OMobj_id obj_id, const char *name, int mode, OMXobj *parent_ptr) :
   OMXgroup_array(obj_id, name, mode, parent_ptr)
{
}
OMXgroup *ReadDatamine_ReadDatamineMods_ReadDatamineCore_fld_array::ret_array_member(OMobj_id val_id)
{
   return((OMXgroup *)new FLD_Field(val_id, NULL,OMX_TRANSIENT_OBJ,NULL));
}
FLD_Field &ReadDatamine_ReadDatamineMods_ReadDatamineCore_fld_array::operator[](const xp_long index)
{
   return(*(FLD_Field *)(OMXgroup_array::operator[](index)).ret_omx_ptr("FLD_Field"));
}

//
// Program generated src definition for class: ReadDatamine_ReadDatamineMods_ReadDatamineCore
//
void ReadDatamine_ReadDatamineMods_ReadDatamineCore__free(void *obj)
{
   ((OMXgroup *)obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *ReadDatamine_ReadDatamineMods_ReadDatamineCore__inst(OMobj_id obj_id)
{
   ReadDatamine_ReadDatamineMods_ReadDatamineCore *obj_ptr = new ReadDatamine_ReadDatamineMods_ReadDatamineCore(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *ReadDatamine_ReadDatamineMods_ReadDatamineCore::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ReadDatamine_ReadDatamineMods_ReadDatamineCore"))
      return(this);
   return(NULL);
}
ReadDatamine_ReadDatamineMods_ReadDatamineCore::ReadDatamine_ReadDatamineMods_ReadDatamineCore(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr, const char *class_name) :
   OMXgroup(id, name, mode, parent_ptr, class_name),
   params(obj_id(OM_OBJ_RD),"params",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   dir(obj_id(OM_OBJ_RD),"dir",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   filenames(obj_id(OM_OBJ_RD),"filenames",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   model_as_point(obj_id(OM_OBJ_RD),"model_as_point",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   model_scale(obj_id(OM_OBJ_RD),"model_scale",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   err(obj_id(OM_OBJ_RD),"err",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   err_str(obj_id(OM_OBJ_RD),"err_str",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   nFlds(obj_id(OM_OBJ_RD),"nFlds",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   fld(obj_id(OM_OBJ_RD),"fld",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{
}
int ReadDatamine_ReadDatamineMods_ReadDatamineCore__update__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num)
{
   return(((ReadDatamine_ReadDatamineMods_ReadDatamineCore *)obj->ret_omx_ptr("ReadDatamine_ReadDatamineMods_ReadDatamineCore"))->update(event_mask, seq_num));
}
#endif /* end c++ section */

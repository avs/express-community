/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */

#include "xp_comm_proj\i_filter\gen.hxx"
#ifdef __cplusplus

//
// Program generated src definition for class: ImageFilter_ImageFilterMods_ImageFilterParams
//
void ImageFilter_ImageFilterMods_ImageFilterParams__free(void *obj)
{
   ((OMXgroup *)obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *ImageFilter_ImageFilterMods_ImageFilterParams__inst(OMobj_id obj_id)
{
   ImageFilter_ImageFilterMods_ImageFilterParams *obj_ptr = new ImageFilter_ImageFilterMods_ImageFilterParams(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *ImageFilter_ImageFilterMods_ImageFilterParams::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ImageFilter_ImageFilterMods_ImageFilterParams"))
      return(this);
   return(NULL);
}
ImageFilter_ImageFilterMods_ImageFilterParams::ImageFilter_ImageFilterMods_ImageFilterParams(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr, const char *class_name) :
   OMXgroup(id, name, mode, parent_ptr, class_name),
   filterOp(obj_id(OM_OBJ_RD),"filterOp",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   alphaConst(obj_id(OM_OBJ_RD),"alphaConst",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   redConst(obj_id(OM_OBJ_RD),"redConst",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   greenConst(obj_id(OM_OBJ_RD),"greenConst",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   blueConst(obj_id(OM_OBJ_RD),"blueConst",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{
}

//
// Program generated src definition for class: ImageFilter_ImageFilterMods_ImageFilterCore
//
void ImageFilter_ImageFilterMods_ImageFilterCore__free(void *obj)
{
   ((OMXgroup *)obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *ImageFilter_ImageFilterMods_ImageFilterCore__inst(OMobj_id obj_id)
{
   ImageFilter_ImageFilterMods_ImageFilterCore *obj_ptr = new ImageFilter_ImageFilterMods_ImageFilterCore(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *ImageFilter_ImageFilterMods_ImageFilterCore::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ImageFilter_ImageFilterMods_ImageFilterCore"))
      return(this);
   return(NULL);
}
ImageFilter_ImageFilterMods_ImageFilterCore::ImageFilter_ImageFilterMods_ImageFilterCore(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr, const char *class_name) :
   OMXgroup(id, name, mode, parent_ptr, class_name),
   input(obj_id(OM_OBJ_RD),"input",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   params(obj_id(OM_OBJ_RD),"params",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   out_size(obj_id(OM_OBJ_RD),"out_size",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   output(obj_id(OM_OBJ_RD),"output",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{
}
int ImageFilter_ImageFilterMods_ImageFilterCore__update__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num)
{
   return(((ImageFilter_ImageFilterMods_ImageFilterCore *)obj->ret_omx_ptr("ImageFilter_ImageFilterMods_ImageFilterCore"))->update(event_mask, seq_num));
}
#endif /* end c++ section */

/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */

#include "xp_comm_proj\chromad\gen.hxx"
#ifdef __cplusplus

//
// Program generated src definition for class: ChromaDepth_ChromaDepthMods_ChromaDepthCore
//
void ChromaDepth_ChromaDepthMods_ChromaDepthCore__free(void *obj)
{
   ((OMXgroup *)obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *ChromaDepth_ChromaDepthMods_ChromaDepthCore__inst(OMobj_id obj_id)
{
   ChromaDepth_ChromaDepthMods_ChromaDepthCore *obj_ptr = new ChromaDepth_ChromaDepthMods_ChromaDepthCore(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *ChromaDepth_ChromaDepthMods_ChromaDepthCore::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ChromaDepth_ChromaDepthMods_ChromaDepthCore"))
      return(this);
   return(NULL);
}
ChromaDepth_ChromaDepthMods_ChromaDepthCore::ChromaDepth_ChromaDepthMods_ChromaDepthCore(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr, const char *class_name) :
   OMXgroup(id, name, mode, parent_ptr, class_name),
   framebuffer(obj_id(OM_OBJ_RD),"framebuffer",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   zbuffer(obj_id(OM_OBJ_RD),"zbuffer",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   mix(obj_id(OM_OBJ_RD),"mix",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   out_size(obj_id(OM_OBJ_RD),"out_size",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   output(obj_id(OM_OBJ_RD),"output",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{
}
int ChromaDepth_ChromaDepthMods_ChromaDepthCore__update__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num)
{
   return(((ChromaDepth_ChromaDepthMods_ChromaDepthCore *)obj->ret_omx_ptr("ChromaDepth_ChromaDepthMods_ChromaDepthCore"))->update(event_mask, seq_num));
}
#endif /* end c++ section */

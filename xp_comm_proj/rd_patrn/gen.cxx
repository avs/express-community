/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */

#include "xp_comm_proj\rd_patrn\gen.h"
#ifdef __cplusplus

//
// Program generated src definition for class: ReadPatran_ReadPatranMods_ReadPatranParams
//
void ReadPatran_ReadPatranMods_ReadPatranParams__free(void *obj)
{
   ((OMXgroup *)obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *ReadPatran_ReadPatranMods_ReadPatranParams__inst(OMobj_id obj_id)
{
   ReadPatran_ReadPatranMods_ReadPatranParams *obj_ptr = new ReadPatran_ReadPatranMods_ReadPatranParams(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *ReadPatran_ReadPatranMods_ReadPatranParams::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ReadPatran_ReadPatranMods_ReadPatranParams"))
      return(this);
   return(NULL);
}
ReadPatran_ReadPatranMods_ReadPatranParams::ReadPatran_ReadPatranMods_ReadPatranParams(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr, const char *class_name) :
   OMXgroup(id, name, mode, parent_ptr, class_name),
   filename(obj_id(OM_OBJ_RD),"filename",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   points_visible(obj_id(OM_OBJ_RD),"points_visible",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   line_visible(obj_id(OM_OBJ_RD),"line_visible",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   tri_visible(obj_id(OM_OBJ_RD),"tri_visible",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   quad_visible(obj_id(OM_OBJ_RD),"quad_visible",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   tet_visible(obj_id(OM_OBJ_RD),"tet_visible",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   patch_visible(obj_id(OM_OBJ_RD),"patch_visible",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{
}

//
// Program generated src definition for class: ReadPatran_ReadPatranMods_neutral_output
//
void ReadPatran_ReadPatranMods_neutral_output__free(void *obj)
{
   ((OMXgroup *)obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *ReadPatran_ReadPatranMods_neutral_output__inst(OMobj_id obj_id)
{
   ReadPatran_ReadPatranMods_neutral_output *obj_ptr = new ReadPatran_ReadPatranMods_neutral_output(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *ReadPatran_ReadPatranMods_neutral_output::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ReadPatran_ReadPatranMods_neutral_output"))
      return(this);
   return(NULL);
}
ReadPatran_ReadPatranMods_neutral_output::ReadPatran_ReadPatranMods_neutral_output(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr, const char *class_name) :
   OMXgroup(id, name, mode, parent_ptr, class_name),
   title1(obj_id(OM_OBJ_RD),"title1",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   title2(obj_id(OM_OBJ_RD),"title2",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   n_nodes(obj_id(OM_OBJ_RD),"n_nodes",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   xyz(obj_id(OM_OBJ_RD),"xyz",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   n_bar(obj_id(OM_OBJ_RD),"n_bar",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   bar_conn(obj_id(OM_OBJ_RD),"bar_conn",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   n_tri(obj_id(OM_OBJ_RD),"n_tri",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   tri_conn(obj_id(OM_OBJ_RD),"tri_conn",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   n_quad(obj_id(OM_OBJ_RD),"n_quad",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   quad_conn(obj_id(OM_OBJ_RD),"quad_conn",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   n_tet(obj_id(OM_OBJ_RD),"n_tet",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   tet_conn(obj_id(OM_OBJ_RD),"tet_conn",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   n_wedge(obj_id(OM_OBJ_RD),"n_wedge",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   wedge_conn(obj_id(OM_OBJ_RD),"wedge_conn",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   n_hex(obj_id(OM_OBJ_RD),"n_hex",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   hex_conn(obj_id(OM_OBJ_RD),"hex_conn",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   n_line(obj_id(OM_OBJ_RD),"n_line",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   line(obj_id(OM_OBJ_RD),"line",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   n_patch(obj_id(OM_OBJ_RD),"n_patch",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   patch(obj_id(OM_OBJ_RD),"patch",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{
}

//
// Program generated src definition for class: ReadPatran_ReadPatranMods_ReadPatranCore
//
void ReadPatran_ReadPatranMods_ReadPatranCore__free(void *obj)
{
   ((OMXgroup *)obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *ReadPatran_ReadPatranMods_ReadPatranCore__inst(OMobj_id obj_id)
{
   ReadPatran_ReadPatranMods_ReadPatranCore *obj_ptr = new ReadPatran_ReadPatranMods_ReadPatranCore(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *ReadPatran_ReadPatranMods_ReadPatranCore::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ReadPatran_ReadPatranMods_ReadPatranCore"))
      return(this);
   return(NULL);
}
ReadPatran_ReadPatranMods_ReadPatranCore::ReadPatran_ReadPatranMods_ReadPatranCore(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr, const char *class_name) :
   OMXgroup(id, name, mode, parent_ptr, class_name),
   ReadPatranParams(obj_id(OM_OBJ_RD),"ReadPatranParams",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   filename(obj_id(OM_OBJ_RD),"filename",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
   neutral_output(obj_id(OM_OBJ_RD),"neutral_output",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{
}
int ReadPatran_ReadPatranMods_ReadPatranCore__update__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num)
{
   return(((ReadPatran_ReadPatranMods_ReadPatranCore *)obj->ret_omx_ptr("ReadPatran_ReadPatranMods_ReadPatranCore"))->update(event_mask, seq_num));
}
#endif /* end c++ section */

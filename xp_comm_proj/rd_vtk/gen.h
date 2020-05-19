/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */
#ifndef OM_NO_DEF_XP_COMM_PROJ_ReadVtk_ReadVtkMods
#define OM_NO_DEF_XP_COMM_PROJ_ReadVtk_ReadVtkMods

#include "avs/om.h"

#ifdef __cplusplus
#include "avs/omx.hxx"
#endif
#ifdef __cplusplus

//
// Program generated hdr definition for class: ReadVtk_ReadVtkMods_ReadVtkCore
//
void ReadVtk_ReadVtkMods_ReadVtkCore__free(void *obj);
OMXgroup *ReadVtk_ReadVtkMods_ReadVtkCore__inst(OMobj_id obj_id);
class ReadVtk_ReadVtkMods_ReadVtkCore : public virtual OMXgroup {
   public:
      ReadVtk_ReadVtkMods_ReadVtkCore(OMobj_id id = OMinst_obj,
         const char *name = NULL,
         int mode = OMX_CREATE_NEW_OBJ,
         OMXobj *parent_ptr = NULL,
         const char *class_name = "XP_COMM_PROJ.ReadVtk.ReadVtkMods.ReadVtkCore");
      virtual void *ret_omx_ptr(const char *name);
      OMXstr filename;
      virtual int Read(OMevent_mask event_mask=0, int seq_num=0);
      OMXint num_coords;
      OMXint num_connect;
      OMXfloat_array coords;
      OMXlong_array connect;
};
int ReadVtk_ReadVtkMods_ReadVtkCore__Read__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num);
#endif /* end c++ section */
#endif /* OM_NO_DEF_XP_COMM_PROJ.ReadVtk.ReadVtkMods */

/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */
#ifndef OM_NO_DEF_XP_COMM_PROJ_EnhanceContrast_EnhanceContrastMods
#define OM_NO_DEF_XP_COMM_PROJ_EnhanceContrast_EnhanceContrastMods

#include "avs/om.h"

#ifdef __cplusplus
#include "avs/omx.hxx"
#endif
#ifdef __cplusplus

//
// Program generated hdr definition for class: EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore
//
void EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore__free(void *obj);
OMXgroup *EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore__inst(OMobj_id obj_id);
class EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore : public virtual OMXgroup {
   public:
      EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore(OMobj_id id = OMinst_obj,
         const char *name = NULL,
         int mode = OMX_CREATE_NEW_OBJ,
         OMXobj *parent_ptr = NULL,
         const char *class_name = "XP_COMM_PROJ.EnhanceContrast.EnhanceContrastMods.EnhanceContrastCore");
      virtual void *ret_omx_ptr(const char *name);
      OMXfloat level;
      OMXint_array in_dims;
      OMXbyte_array img;
      virtual int Enhance(OMevent_mask event_mask=0, int seq_num=0);
      OMXbyte_array out;
};
int EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore__Enhance__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num);
#endif /* end c++ section */
#endif /* OM_NO_DEF_XP_COMM_PROJ.EnhanceContrast.EnhanceContrastMods */

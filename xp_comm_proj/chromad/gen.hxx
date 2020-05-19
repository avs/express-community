/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */
#ifndef OM_NO_DEF_XP_COMM_PROJ_ChromaDepth_ChromaDepthMods
#define OM_NO_DEF_XP_COMM_PROJ_ChromaDepth_ChromaDepthMods

#include "avs/om.h"

#ifdef __cplusplus
#include "avs/omx.hxx"
#endif
#ifdef __cplusplus

//
// Program generated hdr definition for class: ChromaDepth_ChromaDepthMods_ChromaDepthCore
//
void ChromaDepth_ChromaDepthMods_ChromaDepthCore__free(void *obj);
OMXgroup *ChromaDepth_ChromaDepthMods_ChromaDepthCore__inst(OMobj_id obj_id);
class ChromaDepth_ChromaDepthMods_ChromaDepthCore : public virtual OMXgroup {
   public:
      ChromaDepth_ChromaDepthMods_ChromaDepthCore(OMobj_id id = OMinst_obj,
         const char *name = NULL,
         int mode = OMX_CREATE_NEW_OBJ,
         OMXobj *parent_ptr = NULL,
         const char *class_name = "XP_COMM_PROJ.ChromaDepth.ChromaDepthMods.ChromaDepthCore");
      virtual void *ret_omx_ptr(const char *name);
      OMXbyte_array framebuffer;
      OMXfloat_array zbuffer;
      OMXfloat mix;
      virtual int update(OMevent_mask event_mask=0, int seq_num=0);
      OMXint out_size;
      OMXbyte_array output;
};
int ChromaDepth_ChromaDepthMods_ChromaDepthCore__update__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num);
#endif /* end c++ section */
#endif /* OM_NO_DEF_XP_COMM_PROJ.ChromaDepth.ChromaDepthMods */

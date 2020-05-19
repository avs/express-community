/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */
#ifndef OM_NO_DEF_XP_COMM_PROJ_ImageFilter_ImageFilterMods
#define OM_NO_DEF_XP_COMM_PROJ_ImageFilter_ImageFilterMods

#include "avs/om.h"

#ifdef __cplusplus
#include "avs/omx.hxx"
#endif
#ifdef __cplusplus

//
// Program generated hdr definition for class: ImageFilter_ImageFilterMods_ImageFilterParams
//
void ImageFilter_ImageFilterMods_ImageFilterParams__free(void *obj);
OMXgroup *ImageFilter_ImageFilterMods_ImageFilterParams__inst(OMobj_id obj_id);
class ImageFilter_ImageFilterMods_ImageFilterParams : public virtual OMXgroup {
   public:
      ImageFilter_ImageFilterMods_ImageFilterParams(OMobj_id id = OMinst_obj,
         const char *name = NULL,
         int mode = OMX_CREATE_NEW_OBJ,
         OMXobj *parent_ptr = NULL,
         const char *class_name = "XP_COMM_PROJ.ImageFilter.ImageFilterMods.ImageFilterParams");
      virtual void *ret_omx_ptr(const char *name);
      OMXint filterOp;
      OMXfloat alphaConst;
      OMXfloat redConst;
      OMXfloat greenConst;
      OMXfloat blueConst;
};

//
// Program generated hdr definition for class: ImageFilter_ImageFilterMods_ImageFilterCore
//
void ImageFilter_ImageFilterMods_ImageFilterCore__free(void *obj);
OMXgroup *ImageFilter_ImageFilterMods_ImageFilterCore__inst(OMobj_id obj_id);
class ImageFilter_ImageFilterMods_ImageFilterCore : public virtual OMXgroup {
   public:
      ImageFilter_ImageFilterMods_ImageFilterCore(OMobj_id id = OMinst_obj,
         const char *name = NULL,
         int mode = OMX_CREATE_NEW_OBJ,
         OMXobj *parent_ptr = NULL,
         const char *class_name = "XP_COMM_PROJ.ImageFilter.ImageFilterMods.ImageFilterCore");
      virtual void *ret_omx_ptr(const char *name);
      OMXbyte_array input;
      class ImageFilter_ImageFilterMods_ImageFilterParams params;
      virtual int update(OMevent_mask event_mask=0, int seq_num=0);
      OMXint out_size;
      OMXbyte_array output;
};
int ImageFilter_ImageFilterMods_ImageFilterCore__update__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num);
#endif /* end c++ section */
#endif /* OM_NO_DEF_XP_COMM_PROJ.ImageFilter.ImageFilterMods */

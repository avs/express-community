/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */
#ifndef OM_NO_DEF_XP_COMM_PROJ_ReadDatamine_ReadDatamineMods
#define OM_NO_DEF_XP_COMM_PROJ_ReadDatamine_ReadDatamineMods

#include "avs/om.h"

#ifdef __cplusplus
#include "avs/omx.hxx"
#endif
#ifdef __cplusplus

#ifdef __cplusplus
#include "fld/Xfld.h"
#endif

//
// Program generated hdr definition for class: ReadDatamine_ReadDatamineMods_ReadDatamineParams
//
void ReadDatamine_ReadDatamineMods_ReadDatamineParams__free(void *obj);
OMXgroup *ReadDatamine_ReadDatamineMods_ReadDatamineParams__inst(OMobj_id obj_id);
class ReadDatamine_ReadDatamineMods_ReadDatamineParams : public virtual OMXgroup {
   public:
      ReadDatamine_ReadDatamineMods_ReadDatamineParams(OMobj_id id = OMinst_obj,
         const char *name = NULL,
         int mode = OMX_CREATE_NEW_OBJ,
         OMXobj *parent_ptr = NULL,
         const char *class_name = "XP_COMM_PROJ.ReadDatamine.ReadDatamineMods.ReadDatamineParams");
      virtual void *ret_omx_ptr(const char *name);
      OMXstr dir;
      OMXstr_array filenames;
      OMXint model_as_point;
      OMXfloat model_scale;
      OMXint err;
      OMXstr err_str;
};
class ReadDatamine_ReadDatamineMods_ReadDatamineCore_fld_array : public OMXgroup_array {
   public:
      ReadDatamine_ReadDatamineMods_ReadDatamineCore_fld_array(OMobj_id obj_id,
         const char *name = NULL,
         int mode = OMX_CREATE_NEW_OBJ,
         OMXobj *parent_ptr = NULL);
      virtual OMXgroup *ret_array_member(OMobj_id val_id);
      FLD_Field &operator[](const xp_long index);
};

//
// Program generated hdr definition for class: ReadDatamine_ReadDatamineMods_ReadDatamineCore
//
void ReadDatamine_ReadDatamineMods_ReadDatamineCore__free(void *obj);
OMXgroup *ReadDatamine_ReadDatamineMods_ReadDatamineCore__inst(OMobj_id obj_id);
class ReadDatamine_ReadDatamineMods_ReadDatamineCore : public virtual OMXgroup {
   public:
      ReadDatamine_ReadDatamineMods_ReadDatamineCore(OMobj_id id = OMinst_obj,
         const char *name = NULL,
         int mode = OMX_CREATE_NEW_OBJ,
         OMXobj *parent_ptr = NULL,
         const char *class_name = "XP_COMM_PROJ.ReadDatamine.ReadDatamineMods.ReadDatamineCore");
      virtual void *ret_omx_ptr(const char *name);
      class ReadDatamine_ReadDatamineMods_ReadDatamineParams params;
      OMXstr dir;
      OMXstr_array filenames;
      OMXint model_as_point;
      OMXfloat model_scale;
      virtual int update(OMevent_mask event_mask=0, int seq_num=0);
      OMXint err;
      OMXstr err_str;
      OMXint nFlds;
      ReadDatamine_ReadDatamineMods_ReadDatamineCore_fld_array fld;
};
int ReadDatamine_ReadDatamineMods_ReadDatamineCore__update__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num);
#endif /* end c++ section */
#endif /* OM_NO_DEF_XP_COMM_PROJ.ReadDatamine.ReadDatamineMods */

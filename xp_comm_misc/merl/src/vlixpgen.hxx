/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */
#ifndef OM_NO_DEF_MERL
#define OM_NO_DEF_MERL

#include "avs/om.h"

#ifdef __cplusplus
#include "avs/omx.hxx"
#endif
#ifdef __cplusplus

#ifdef __cplusplus
#include "c:\progra~1\VolumePro\inc\vli.h"
#endif


//
// Program generated hdr definition for class: VLICropTerm
//
void VLICropTerm__free(void *obj);
OMXgroup *VLICropTerm__inst(OMobj_id obj_id);
class VLICropTerm : public virtual OMXgroup {
   public:
	VLICropTerm(OMobj_id id = OMinst_obj , char *name = NULL,
	         int mode = OMX_CREATE_NEW_OBJ,
	         OMXobj *parent_ptr = NULL,
	         char *class_name = "VLICropTerm");
	virtual void *ret_omx_ptr(char *);
      OMXshort en_x;
      OMXshort en_y;
      OMXshort en_z;
      OMXshort inv;
};


//
// Program generated hdr definition for class: VLICropBounds
//
void VLICropBounds__free(void *obj);
OMXgroup *VLICropBounds__inst(OMobj_id obj_id);
class VLICropBounds : public virtual OMXgroup {
   public:
	VLICropBounds(OMobj_id id = OMinst_obj , char *name = NULL,
	         int mode = OMX_CREATE_NEW_OBJ,
	         OMXobj *parent_ptr = NULL,
	         char *class_name = "VLICropBounds");
	virtual void *ret_omx_ptr(char *);
      OMXfloat min_x;
      OMXfloat max_x;
      OMXfloat min_y;
      OMXfloat max_y;
      OMXfloat min_z;
      OMXfloat max_z;
      class VLICropTerm term0;
      class VLICropTerm term1;
      class VLICropTerm term2;
      OMXenum combine;
      OMXint invert_result;
      OMXenum predefined;
      OMXint crop_enabled;
};


//
// Program generated hdr definition for class: VLICursorAxis
//
void VLICursorAxis__free(void *obj);
OMXgroup *VLICursorAxis__inst(OMobj_id obj_id);
class VLICursorAxis : public virtual OMXgroup {
   public:
	VLICursorAxis(OMobj_id id = OMinst_obj , char *name = NULL,
	         int mode = OMX_CREATE_NEW_OBJ,
	         OMXobj *parent_ptr = NULL,
	         char *class_name = "VLICursorAxis");
	virtual void *ret_omx_ptr(char *);
      OMXint enabled;
      OMXdouble red;
      OMXdouble green;
      OMXdouble blue;
      OMXdouble alpha;
};


//
// Program generated hdr definition for class: VLICursorInfo
//
void VLICursorInfo__free(void *obj);
OMXgroup *VLICursorInfo__inst(OMobj_id obj_id);
class VLICursorInfo : public virtual OMXgroup {
   public:
	VLICursorInfo(OMobj_id id = OMinst_obj , char *name = NULL,
	         int mode = OMX_CREATE_NEW_OBJ,
	         OMXobj *parent_ptr = NULL,
	         char *class_name = "VLICursorInfo");
	virtual void *ret_omx_ptr(char *);
      OMXdouble_array position;
      OMXint enabled;
      OMXint disable_crop;
      OMXenum type;
      OMXdouble width;
      OMXdouble length;
      class VLICursorAxis x_axis;
      class VLICursorAxis y_axis;
      class VLICursorAxis z_axis;
};

#ifdef __cplusplus
#include "D:\MERL\.\src\vliren.hxx"
#endif
#endif /* end c++ section */
#ifdef __cplusplus
extern "C" {
#endif
int VLIVolumeDestroy PTARGS((OMobj_id, OMevent_mask, int));
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include "src\hist.hxx"
#endif
#ifdef __cplusplus


//
// Program generated hdr definition for class: Utility_macros_MakeDefaultLUT
//
void Utility_macros_MakeDefaultLUT__free(void *obj);
OMXgroup *Utility_macros_MakeDefaultLUT__inst(OMobj_id obj_id);
class Utility_macros_MakeDefaultLUT : public virtual OMXgroup {
   public:
	Utility_macros_MakeDefaultLUT(OMobj_id id = OMinst_obj , char *name = NULL,
	         int mode = OMX_CREATE_NEW_OBJ,
	         OMXobj *parent_ptr = NULL,
	         char *class_name = "Utility_macros.MakeDefaultLUT");
	virtual void *ret_omx_ptr(char *);
      OMXint LUTsize;
      OMXint_array alpha;
      OMXbyte_array red;
      OMXbyte_array green;
      OMXbyte_array blue;
      virtual int update(OMevent_mask event_mask=0, int seq_num=0);
};
int
Utility_macros_MakeDefaultLUT__update__stub(OMXgroup *obj,OMevent_mask event_mask, int seq_num);
#endif /* end c++ section */

#include "src\MakeLUT_gen.h"
#endif /* OM_NO_DEF_MERL */

/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */
#ifndef OM_NO_DEF_ISWrapperMod
#define OM_NO_DEF_ISWrapperMod

#include "avs/om.h"

#ifdef __cplusplus
#include "avs/omx.hxx"
#endif
#ifdef __cplusplus


//
// Program generated hdr definition for class: ISWrapperMod
//
void ISWrapperMod__free(void *obj);
OMXgroup *ISWrapperMod__inst(OMobj_id obj_id);
class ISWrapperMod : public virtual OMXgroup {
   public:
	ISWrapperMod(OMobj_id id = OMinst_obj , char *name = NULL,
	         int mode = OMX_CREATE_NEW_OBJ,
	         OMXobj *parent_ptr = NULL,
	         char *class_name = "ISWrapperMod");
	virtual void *ret_omx_ptr(char *);
      OMXint OpenDeviceTrigger;
      OMXint CloseDeviceTrigger;
      OMXint ShowStatsTrigger;
      OMXint UpdateXformTrigger;
      OMXint DeviceState;
      OMXfloat Xrot;
      OMXfloat Yrot;
      OMXfloat Zrot;
      OMXfloat Xtrans;
      OMXfloat Ytrans;
      OMXfloat Ztrans;
      virtual int OpenDevice(OMevent_mask event_mask=0, int seq_num=0);
      virtual int CloseDevice(OMevent_mask event_mask=0, int seq_num=0);
      virtual int ShowStats(OMevent_mask event_mask=0, int seq_num=0);
      virtual int UpdateXform(OMevent_mask event_mask=0, int seq_num=0);
};
int
ISWrapperMod__OpenDevice__stub(OMXgroup *obj,OMevent_mask event_mask, int seq_num);
int
ISWrapperMod__CloseDevice__stub(OMXgroup *obj,OMevent_mask event_mask, int seq_num);
int
ISWrapperMod__ShowStats__stub(OMXgroup *obj,OMevent_mask event_mask, int seq_num);
int
ISWrapperMod__UpdateXform__stub(OMXgroup *obj,OMevent_mask event_mask, int seq_num);
#endif /* end c++ section */
#endif /* OM_NO_DEF_ISWrapperMod */

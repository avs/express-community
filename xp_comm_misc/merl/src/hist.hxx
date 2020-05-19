/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */
#ifndef OM_NO_DEF_Volume_Histogram
#define OM_NO_DEF_Volume_Histogram

#include "avs/om.h"

#ifdef __cplusplus
#include "avs/omx.hxx"
#endif
#ifdef __cplusplus


//
// Program generated hdr definition for class: Volume_Histogram_volume
//
class Volume_Histogram_volume : public virtual OMXgroup, public FLD_Mesh_Unif, public FLD_Node_Data {
   public:
	Volume_Histogram_volume(OMobj_id id = OMinst_obj , char *name = NULL,
	         int mode = OMX_CREATE_NEW_OBJ,
	         OMXobj *parent_ptr = NULL,
	         char *class_name = "Volume_Histogram.volume");
	virtual void *ret_omx_ptr(char *);
      OMXint nnodes;
};


//
// Program generated hdr definition for class: Volume_Histogram
//
void Volume_Histogram__free(void *obj);
OMXgroup *Volume_Histogram__inst(OMobj_id obj_id);
class Volume_Histogram : public virtual OMXgroup {
   public:
	Volume_Histogram(OMobj_id id = OMinst_obj , char *name = NULL,
	         int mode = OMX_CREATE_NEW_OBJ,
	         OMXobj *parent_ptr = NULL,
	         char *class_name = "Volume_Histogram");
	virtual void *ret_omx_ptr(char *);
      class Volume_Histogram_volume volume;
      OMXint data_component;
      OMXint threshold;
      OMXint_array histogram;
      virtual int ComputeHistogram(OMevent_mask event_mask=0, int seq_num=0);
};
int
Volume_Histogram__ComputeHistogram__stub(OMXgroup *obj,OMevent_mask event_mask, int seq_num);
#endif /* end c++ section */
#endif /* OM_NO_DEF_Volume_Histogram */

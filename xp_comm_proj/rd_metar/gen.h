/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */
#ifndef OM_NO_DEF_XP_COMM_PROJ_ReadMETARS_ReadMETARSMods_ReadMETARSCore
#define OM_NO_DEF_XP_COMM_PROJ_ReadMETARS_ReadMETARSMods_ReadMETARSCore

#include "avs/om.h"

#ifdef __cplusplus
#include "avs/omx.hxx"
#endif
#ifdef __cplusplus

#ifdef __cplusplus
#include "METARS.h"
#endif

#ifdef __cplusplus
#include "fld/Xfld.h"
#endif


//
// Program generated hdr definition for class: ReadMETARSCore_out
//
class ReadMETARSCore_out : public virtual OMXgroup, public FLD_Mesh, public FLD_Node_Data {
   public:
	ReadMETARSCore_out(OMobj_id id = OMinst_obj,
	         const char *name = NULL,
	         int mode = OMX_CREATE_NEW_OBJ,
	         OMXobj *parent_ptr = NULL,
	         const char *class_name = "XP_COMM_PROJ.ReadMETARS.ReadMETARSMods.ReadMETARSCore.out");
	virtual void *ret_omx_ptr(const char *);
      OMXint nnodes;
};


//
// Program generated hdr definition for class: ReadMETARSCore
//
void ReadMETARSCore__free(void *obj);
OMXgroup *ReadMETARSCore__inst(OMobj_id obj_id);
class ReadMETARSCore : public virtual OMXgroup {
   public:
	ReadMETARSCore(OMobj_id id = OMinst_obj,
	         const char *name = NULL,
	         int mode = OMX_CREATE_NEW_OBJ,
	         OMXobj *parent_ptr = NULL,
	         const char *class_name = "XP_COMM_PROJ.ReadMETARS.ReadMETARSMods.ReadMETARSCore");
	virtual void *ret_omx_ptr(const char *);
      OMXstr filename;
      OMXstr_array StationID;
      class FLD_Mesh StationLocation;
      virtual int read(OMevent_mask event_mask=0, int seq_num=0);
      class ReadMETARSCore_out out;
};
int
ReadMETARSCore__read__stub(OMXgroup *obj,OMevent_mask event_mask, int seq_num);
#endif /* end c++ section */
#endif /* OM_NO_DEF_XP_COMM_PROJ.ReadMETARS.ReadMETARSMods.ReadMETARSCore */

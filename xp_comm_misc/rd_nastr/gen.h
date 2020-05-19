/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */
#ifndef OM_NO_DEF_XP_COMM_PROJ_ReadNastran_ReadNastranMods
#define OM_NO_DEF_XP_COMM_PROJ_ReadNastran_ReadNastranMods

#include "avs/om.h"

#ifdef __cplusplus
#include "avs/omx.hxx"
#endif
#ifdef __cplusplus


//
// Program generated hdr definition for class: ReadNastran_ReadNastranMods_ReadNastranParams
//
void ReadNastran_ReadNastranMods_ReadNastranParams__free(void *obj);
OMXgroup *ReadNastran_ReadNastranMods_ReadNastranParams__inst(OMobj_id obj_id);
class ReadNastran_ReadNastranMods_ReadNastranParams : public virtual OMXgroup {
   public:
	ReadNastran_ReadNastranMods_ReadNastranParams(OMobj_id id = OMinst_obj,
	         const char *name = NULL,
	         int mode = OMX_CREATE_NEW_OBJ,
	         OMXobj *parent_ptr = NULL,
	         const char *class_name = "XP_COMM_PROJ.ReadNastran.ReadNastranMods.ReadNastranParams");
	virtual void *ret_omx_ptr(const char *);
      OMXstr filename;
      OMXenum WireframeMode;
      OMXenum SurfaceMode;
      OMXenum ColorMode;
      OMXint NormalGenerationMode;
      OMXint NodeDataOn;
      OMXint AlternateOn;
      OMXint CacheSize;
};


//
// Program generated hdr definition for class: ReadNastran_ReadNastranMods_nastran_output
//
void ReadNastran_ReadNastranMods_nastran_output__free(void *obj);
OMXgroup *ReadNastran_ReadNastranMods_nastran_output__inst(OMobj_id obj_id);
class ReadNastran_ReadNastranMods_nastran_output : public virtual OMXgroup {
   public:
	ReadNastran_ReadNastranMods_nastran_output(OMobj_id id = OMinst_obj,
	         const char *name = NULL,
	         int mode = OMX_CREATE_NEW_OBJ,
	         OMXobj *parent_ptr = NULL,
	         const char *class_name = "XP_COMM_PROJ.ReadNastran.ReadNastranMods.nastran_output");
	virtual void *ret_omx_ptr(const char *);
      OMXint Size;
      OMXint_array IDS;
      OMXfloat_array Coords;
      OMXint CTRIAX6_Size;
      OMXint_array CTRIAX6;
      OMXint_array CTRIAX6IDS;
      OMXint_array CTRIAX6MATERIALS;
      OMXint CHEXA8_Size;
      OMXint_array CHEXA8;
      OMXint_array CHEXA8IDS;
      OMXint_array CHEXA8MATERIALS;
      OMXint CHEXA20_Size;
      OMXint_array CHEXA20;
      OMXint_array CHEXA20IDS;
      OMXint_array CHEXA20MATERIALS;
      OMXint CTETRA4_Size;
      OMXint_array CTETRA4;
      OMXint_array CTETRA4IDS;
      OMXint_array CTETRA4MATERIALS;
      OMXint CTETRA10_Size;
      OMXint_array CTETRA10;
      OMXint_array CTETRA10IDS;
      OMXint_array CTETRA10MATERIALS;
      OMXint CPENTA6_Size;
      OMXint_array CPENTA6;
      OMXint_array CPENTA6IDS;
      OMXint_array CPENTA6MATERIALS;
      OMXint CPENTA15_Size;
      OMXint_array CPENTA15;
      OMXint_array CPENTA15IDS;
      OMXint_array CPENTA15MATERIALS;
      OMXint CurrentNode;
      OMXint CurrentCellSet;
      OMXint CurrentElement;
};


//
// Program generated hdr definition for class: ReadNastran_ReadNastranMods_ReadNastranCore
//
void ReadNastran_ReadNastranMods_ReadNastranCore__free(void *obj);
OMXgroup *ReadNastran_ReadNastranMods_ReadNastranCore__inst(OMobj_id obj_id);
class ReadNastran_ReadNastranMods_ReadNastranCore : public virtual OMXgroup {
   public:
	ReadNastran_ReadNastranMods_ReadNastranCore(OMobj_id id = OMinst_obj,
	         const char *name = NULL,
	         int mode = OMX_CREATE_NEW_OBJ,
	         OMXobj *parent_ptr = NULL,
	         const char *class_name = "XP_COMM_PROJ.ReadNastran.ReadNastranMods.ReadNastranCore");
	virtual void *ret_omx_ptr(const char *);
      class ReadNastran_ReadNastranMods_ReadNastranParams ReadNastranParams;
      class ReadNastran_ReadNastranMods_nastran_output nastran_output;
      virtual int Import(OMevent_mask event_mask=0, int seq_num=0);
};
int
ReadNastran_ReadNastranMods_ReadNastranCore__Import__stub(OMXgroup *obj,OMevent_mask event_mask, int seq_num);
#endif /* end c++ section */
#endif /* OM_NO_DEF_XP_COMM_PROJ.ReadNastran.ReadNastranMods */

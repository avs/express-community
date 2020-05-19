/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */
#ifndef OM_NO_DEF_XP_COMM_PROJ_ReadPatran_ReadPatranMods
#define OM_NO_DEF_XP_COMM_PROJ_ReadPatran_ReadPatranMods

#include "avs/om.h"

#ifdef __cplusplus
#include "avs/omx.hxx"
#endif
#ifdef __cplusplus
extern "C" {
#endif

#include "xp_comm_proj\rd_patrn\pneutral.h"

#include "xp_comm_proj\rd_patrn\rnum.h"
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus

#ifdef __cplusplus
#include "xp_comm_proj\rd_patrn\patran.hxx"
#endif

//
// Program generated hdr definition for class: ReadPatran_ReadPatranMods_ReadPatranParams
//
void ReadPatran_ReadPatranMods_ReadPatranParams__free(void *obj);
OMXgroup *ReadPatran_ReadPatranMods_ReadPatranParams__inst(OMobj_id obj_id);
class ReadPatran_ReadPatranMods_ReadPatranParams : public virtual OMXgroup {
   public:
      ReadPatran_ReadPatranMods_ReadPatranParams(OMobj_id id = OMinst_obj,
         const char *name = NULL,
         int mode = OMX_CREATE_NEW_OBJ,
         OMXobj *parent_ptr = NULL,
         const char *class_name = "XP_COMM_PROJ.ReadPatran.ReadPatranMods.ReadPatranParams");
      virtual void *ret_omx_ptr(const char *name);
      OMXstr filename;
      OMXint points_visible;
      OMXint line_visible;
      OMXint tri_visible;
      OMXint quad_visible;
      OMXint tet_visible;
      OMXint patch_visible;
};

//
// Program generated hdr definition for class: ReadPatran_ReadPatranMods_neutral_output
//
void ReadPatran_ReadPatranMods_neutral_output__free(void *obj);
OMXgroup *ReadPatran_ReadPatranMods_neutral_output__inst(OMobj_id obj_id);
class ReadPatran_ReadPatranMods_neutral_output : public virtual OMXgroup {
   public:
      ReadPatran_ReadPatranMods_neutral_output(OMobj_id id = OMinst_obj,
         const char *name = NULL,
         int mode = OMX_CREATE_NEW_OBJ,
         OMXobj *parent_ptr = NULL,
         const char *class_name = "XP_COMM_PROJ.ReadPatran.ReadPatranMods.neutral_output");
      virtual void *ret_omx_ptr(const char *name);
      OMXstr title1;
      OMXstr title2;
      OMXint n_nodes;
      OMXdouble_array xyz;
      OMXint n_bar;
      OMXint_array bar_conn;
      OMXint n_tri;
      OMXint_array tri_conn;
      OMXint n_quad;
      OMXint_array quad_conn;
      OMXint n_tet;
      OMXint_array tet_conn;
      OMXint n_wedge;
      OMXint_array wedge_conn;
      OMXint n_hex;
      OMXint_array hex_conn;
      OMXint n_line;
      OMXint_array line;
      OMXint n_patch;
      OMXint_array patch;
};

//
// Program generated hdr definition for class: ReadPatran_ReadPatranMods_ReadPatranCore
//
void ReadPatran_ReadPatranMods_ReadPatranCore__free(void *obj);
OMXgroup *ReadPatran_ReadPatranMods_ReadPatranCore__inst(OMobj_id obj_id);
class ReadPatran_ReadPatranMods_ReadPatranCore : public virtual OMXgroup {
   public:
      ReadPatran_ReadPatranMods_ReadPatranCore(OMobj_id id = OMinst_obj,
         const char *name = NULL,
         int mode = OMX_CREATE_NEW_OBJ,
         OMXobj *parent_ptr = NULL,
         const char *class_name = "XP_COMM_PROJ.ReadPatran.ReadPatranMods.ReadPatranCore");
      virtual void *ret_omx_ptr(const char *name);
      class ReadPatran_ReadPatranMods_ReadPatranParams ReadPatranParams;
      OMXstr filename;
      class ReadPatran_ReadPatranMods_neutral_output neutral_output;
      virtual int update(OMevent_mask event_mask=0, int seq_num=0);
load_neutral loadN;};
int ReadPatran_ReadPatranMods_ReadPatranCore__update__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num);
#endif /* end c++ section */
#endif /* OM_NO_DEF_XP_COMM_PROJ.ReadPatran.ReadPatranMods */

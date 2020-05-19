/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */

#include "xp_comm_proj\rd_nastr\gen.h"
#ifdef __cplusplus


//
// Program generated src definition for class: ReadNastran_ReadNastranMods_ReadNastranParams
//
void
ReadNastran_ReadNastranMods_ReadNastranParams__free(void *obj)
{
   ReadNastran_ReadNastranMods_ReadNastranParams* obj_ptr = (ReadNastran_ReadNastranMods_ReadNastranParams* )
             ((OMXgroup *)obj)->ret_omx_ptr("ReadNastran_ReadNastranMods_ReadNastranParams");
   ((OMXgroup *) obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *
ReadNastran_ReadNastranMods_ReadNastranParams__inst(OMobj_id obj_id)
{
   ReadNastran_ReadNastranMods_ReadNastranParams *obj_ptr = new ReadNastran_ReadNastranMods_ReadNastranParams(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *
ReadNastran_ReadNastranMods_ReadNastranParams::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ReadNastran_ReadNastranMods_ReadNastranParams"))
      return(this);
   return(NULL);
}

ReadNastran_ReadNastranMods_ReadNastranParams::ReadNastran_ReadNastranMods_ReadNastranParams(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr,
const char *class_name) :
	OMXgroup(id, name, mode, parent_ptr, class_name),
	filename(obj_id(OM_OBJ_RD),"filename",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	WireframeMode(obj_id(OM_OBJ_RD),"WireframeMode",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	SurfaceMode(obj_id(OM_OBJ_RD),"SurfaceMode",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	ColorMode(obj_id(OM_OBJ_RD),"ColorMode",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	NormalGenerationMode(obj_id(OM_OBJ_RD),"NormalGenerationMode",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	NodeDataOn(obj_id(OM_OBJ_RD),"NodeDataOn",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	AlternateOn(obj_id(OM_OBJ_RD),"AlternateOn",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CacheSize(obj_id(OM_OBJ_RD),"CacheSize",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{}


//
// Program generated src definition for class: ReadNastran_ReadNastranMods_nastran_output
//
void
ReadNastran_ReadNastranMods_nastran_output__free(void *obj)
{
   ReadNastran_ReadNastranMods_nastran_output* obj_ptr = (ReadNastran_ReadNastranMods_nastran_output* )
             ((OMXgroup *)obj)->ret_omx_ptr("ReadNastran_ReadNastranMods_nastran_output");
   ((OMXgroup *) obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *
ReadNastran_ReadNastranMods_nastran_output__inst(OMobj_id obj_id)
{
   ReadNastran_ReadNastranMods_nastran_output *obj_ptr = new ReadNastran_ReadNastranMods_nastran_output(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *
ReadNastran_ReadNastranMods_nastran_output::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ReadNastran_ReadNastranMods_nastran_output"))
      return(this);
   return(NULL);
}

ReadNastran_ReadNastranMods_nastran_output::ReadNastran_ReadNastranMods_nastran_output(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr,
const char *class_name) :
	OMXgroup(id, name, mode, parent_ptr, class_name),
	Size(obj_id(OM_OBJ_RD),"Size",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	IDS(obj_id(OM_OBJ_RD),"IDS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	Coords(obj_id(OM_OBJ_RD),"Coords",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CTRIAX6_Size(obj_id(OM_OBJ_RD),"CTRIAX6_Size",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CTRIAX6(obj_id(OM_OBJ_RD),"CTRIAX6",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CTRIAX6IDS(obj_id(OM_OBJ_RD),"CTRIAX6IDS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CTRIAX6MATERIALS(obj_id(OM_OBJ_RD),"CTRIAX6MATERIALS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CHEXA8_Size(obj_id(OM_OBJ_RD),"CHEXA8_Size",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CHEXA8(obj_id(OM_OBJ_RD),"CHEXA8",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CHEXA8IDS(obj_id(OM_OBJ_RD),"CHEXA8IDS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CHEXA8MATERIALS(obj_id(OM_OBJ_RD),"CHEXA8MATERIALS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CHEXA20_Size(obj_id(OM_OBJ_RD),"CHEXA20_Size",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CHEXA20(obj_id(OM_OBJ_RD),"CHEXA20",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CHEXA20IDS(obj_id(OM_OBJ_RD),"CHEXA20IDS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CHEXA20MATERIALS(obj_id(OM_OBJ_RD),"CHEXA20MATERIALS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CTETRA4_Size(obj_id(OM_OBJ_RD),"CTETRA4_Size",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CTETRA4(obj_id(OM_OBJ_RD),"CTETRA4",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CTETRA4IDS(obj_id(OM_OBJ_RD),"CTETRA4IDS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CTETRA4MATERIALS(obj_id(OM_OBJ_RD),"CTETRA4MATERIALS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CTETRA10_Size(obj_id(OM_OBJ_RD),"CTETRA10_Size",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CTETRA10(obj_id(OM_OBJ_RD),"CTETRA10",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CTETRA10IDS(obj_id(OM_OBJ_RD),"CTETRA10IDS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CTETRA10MATERIALS(obj_id(OM_OBJ_RD),"CTETRA10MATERIALS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CPENTA6_Size(obj_id(OM_OBJ_RD),"CPENTA6_Size",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CPENTA6(obj_id(OM_OBJ_RD),"CPENTA6",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CPENTA6IDS(obj_id(OM_OBJ_RD),"CPENTA6IDS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CPENTA6MATERIALS(obj_id(OM_OBJ_RD),"CPENTA6MATERIALS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CPENTA15_Size(obj_id(OM_OBJ_RD),"CPENTA15_Size",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CPENTA15(obj_id(OM_OBJ_RD),"CPENTA15",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CPENTA15IDS(obj_id(OM_OBJ_RD),"CPENTA15IDS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CPENTA15MATERIALS(obj_id(OM_OBJ_RD),"CPENTA15MATERIALS",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CurrentNode(obj_id(OM_OBJ_RD),"CurrentNode",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CurrentCellSet(obj_id(OM_OBJ_RD),"CurrentCellSet",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CurrentElement(obj_id(OM_OBJ_RD),"CurrentElement",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{}


//
// Program generated src definition for class: ReadNastran_ReadNastranMods_ReadNastranCore
//
void
ReadNastran_ReadNastranMods_ReadNastranCore__free(void *obj)
{
   ReadNastran_ReadNastranMods_ReadNastranCore* obj_ptr = (ReadNastran_ReadNastranMods_ReadNastranCore* )
             ((OMXgroup *)obj)->ret_omx_ptr("ReadNastran_ReadNastranMods_ReadNastranCore");
   ((OMXgroup *) obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *
ReadNastran_ReadNastranMods_ReadNastranCore__inst(OMobj_id obj_id)
{
   ReadNastran_ReadNastranMods_ReadNastranCore *obj_ptr = new ReadNastran_ReadNastranMods_ReadNastranCore(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *
ReadNastran_ReadNastranMods_ReadNastranCore::ret_omx_ptr(const char *name)
{
   if (!strcmp(name,"ReadNastran_ReadNastranMods_ReadNastranCore"))
      return(this);
   return(NULL);
}

ReadNastran_ReadNastranMods_ReadNastranCore::ReadNastran_ReadNastranMods_ReadNastranCore(OMobj_id id, const char *name, int mode, OMXobj *parent_ptr,
const char *class_name) :
	OMXgroup(id, name, mode, parent_ptr, class_name),
	ReadNastranParams(obj_id(OM_OBJ_RD),"ReadNastranParams",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	nastran_output(obj_id(OM_OBJ_RD),"nastran_output",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{}
int ReadNastran_ReadNastranMods_ReadNastranCore__Import__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num)
{
   return(((ReadNastran_ReadNastranMods_ReadNastranCore *) obj->ret_omx_ptr("ReadNastran_ReadNastranMods_ReadNastranCore"))->Import(event_mask, seq_num));
}

#endif /* end c++ section */

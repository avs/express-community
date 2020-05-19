/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */

#include "src\ISWrapperMod_gen.hxx"
#ifdef __cplusplus


//
// Program generated src definition for class: ISWrapperMod
//
void
ISWrapperMod__free(void *obj)
{
   ISWrapperMod* obj_ptr = (ISWrapperMod* )
             ((OMXgroup *)obj)->ret_omx_ptr("ISWrapperMod");
   ((OMXgroup *) obj)->free_class_ptr();
   delete (OMXobj *)obj;
}
OMXgroup *
ISWrapperMod__inst(OMobj_id obj_id)
{
   ISWrapperMod *obj_ptr = new ISWrapperMod(obj_id,NULL,OMX_EXISTING_OBJ);
   obj_ptr->init_class_ptr();
   return((OMXgroup *)obj_ptr);
}
void *
ISWrapperMod::ret_omx_ptr(char *name)
{
   if (!strcmp(name,"ISWrapperMod"))
      return(this);
   return(NULL);
}

ISWrapperMod::ISWrapperMod(OMobj_id id, char *name, int mode, OMXobj *parent_ptr,
char *class_name) :
	OMXgroup(id, name, mode, parent_ptr, class_name),
	OpenDeviceTrigger(obj_id(OM_OBJ_RD),"OpenDeviceTrigger",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	CloseDeviceTrigger(obj_id(OM_OBJ_RD),"CloseDeviceTrigger",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	ShowStatsTrigger(obj_id(OM_OBJ_RD),"ShowStatsTrigger",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	UpdateXformTrigger(obj_id(OM_OBJ_RD),"UpdateXformTrigger",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	DeviceState(obj_id(OM_OBJ_RD),"DeviceState",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	Xrot(obj_id(OM_OBJ_RD),"Xrot",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	Yrot(obj_id(OM_OBJ_RD),"Yrot",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	Zrot(obj_id(OM_OBJ_RD),"Zrot",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	Xtrans(obj_id(OM_OBJ_RD),"Xtrans",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	Ytrans(obj_id(OM_OBJ_RD),"Ytrans",OMX_CREATE_SUB_OBJ,ret_OMXobj_this()),
	Ztrans(obj_id(OM_OBJ_RD),"Ztrans",OMX_CREATE_SUB_OBJ,ret_OMXobj_this())
{}
int ISWrapperMod__OpenDevice__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num)
{
   return(((ISWrapperMod *) obj->ret_omx_ptr("ISWrapperMod"))->OpenDevice(event_mask, seq_num));
}

int ISWrapperMod__CloseDevice__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num)
{
   return(((ISWrapperMod *) obj->ret_omx_ptr("ISWrapperMod"))->CloseDevice(event_mask, seq_num));
}

int ISWrapperMod__ShowStats__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num)
{
   return(((ISWrapperMod *) obj->ret_omx_ptr("ISWrapperMod"))->ShowStats(event_mask, seq_num));
}

int ISWrapperMod__UpdateXform__stub(OMXgroup *obj, OMevent_mask event_mask, int seq_num)
{
   return(((ISWrapperMod *) obj->ret_omx_ptr("ISWrapperMod"))->UpdateXform(event_mask, seq_num));
}

#endif /* end c++ section */

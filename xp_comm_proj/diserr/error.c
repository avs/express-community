/*
 * error.c
 * When called, this routine pops up the
 * error dialog panel
 */
#include <avs/om.h>

#define ERROR_OBJ "Applications.DisplayErrorDialogEg.Error"


void DisplayErrorDialog(char *title, char *msg)
{
	OMobj_id error_id, vis_id, msg_id, tit_id;

	printf("%s: %s\n", title, msg);

	/* get the id of the error object */
	error_id = OMfind_str_subobj(OMroot_obj, ERROR_OBJ, OM_OBJ_RW);

	/* check it exists */
	if (OMis_null_obj(error_id)) {
		printf("error: unable to display error dialog.\n");
		return;
	}

	OMpush_ctx(error_id, 0, 0, 0);

	/* get object id's */
	vis_id = OMfind_str_subobj(error_id, "visible", OM_OBJ_RW);
	tit_id = OMfind_str_subobj(error_id, "title", OM_OBJ_RW);
	msg_id = OMfind_str_subobj(error_id, "message", OM_OBJ_RW);

	/* set object values */
	OMset_int_val(vis_id, 1);
	OMset_str_val(tit_id, title);
	OMset_str_val(msg_id, msg);

	OMpop_ctx(error_id);
}


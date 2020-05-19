/*
 * cursor.c
 * When called, this routine sets the cursor.
 */
#include <avs/om.h>


#define CURSOR_OBJ "Applications.SetCursorEg.Cursor"
 
 
/* if enable=1, the 'working' cursor is attached,
 * if enable=0. the default cursor is attached
 */
void SetApplicationCursor(int enable)
{
	OMobj_id cursor_id, attach_id;

	/* get the id of the cursor object */
	cursor_id = OMfind_str_subobj(OMroot_obj, CURSOR_OBJ, OM_OBJ_RW);

	/* check it exists */
	if (OMis_null_obj(cursor_id)) {
		printf("error: unable to set cursor.\n");
		return;
	}

	OMpush_ctx(cursor_id, 0, 0, 0);

	/* get object id's */
	attach_id = OMfind_str_subobj(cursor_id, "attach_cursor", OM_OBJ_RW);

	/* set object values */
	OMset_int_val(attach_id, enable);
	OMpop_ctx(cursor_id);
}


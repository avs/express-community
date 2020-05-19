#ifndef _FIELD_UTIL_H_
#define _FIELD_UTIL_H_

#include <avs/om.h>

void FUset_null_mesh(OMobj_id elem_id);
int  FUcopy_field(OMobj_id inMesh_id, OMobj_id outMesh_id);
int  FUmerge_isolines(OMobj_id in, OMobj_id out, OMobj_id points);

#endif /* _FIELD_UTIL_H_ */

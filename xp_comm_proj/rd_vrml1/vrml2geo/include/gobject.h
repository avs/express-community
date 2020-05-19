#ifndef _CLASS_GEOM_OBJECTS_H_
#define _CLASS_GEOM_OBJECTS_H_

#include <avs/geom.h>

class geom_objects {

public:
	geom_objects();
	~geom_objects();
	int add_obj (GEOMobj* new_obj);
    GEOMobj* get_obj (int num);
    GEOMobj** get_all_objs ();
    int get_nobjs ();

private:
	GEOMobj	**obj_list;
	int num_obj;
	int max_obj;
	
};

#endif

#include "gobject.h"
#include <avs/geom.h>

// #define DEBUG

void * GobjRealloc(void* ptr, size_t new_size, size_t old_size)
{
	void * tmp;
	#ifdef MSDOS
		tmp = malloc(new_size);
		if (!tmp) return NULL;
		memcpy(tmp,ptr,old_size);
		free (ptr);
	#else
		tmp = realloc (ptr,new_size);
	#endif
	return tmp;
}

geom_objects::geom_objects()
{
	max_obj=200;
	num_obj=-1;
	obj_list = (GEOMobj**) malloc(max_obj*sizeof(GEOMobj*));
#ifdef DEBUG
	fprintf(stderr,"geom_objects constructor\n");fflush(stderr);
#endif
}

geom_objects::~geom_objects()
{
	for (int i=0; i < num_obj; i++) {
			GEOMdestroy_obj(obj_list[i]);
			obj_list[i] = NULL;
	}	 
	free(obj_list);
	obj_list=NULL;
	num_obj=-1;
}


int geom_objects::add_obj(GEOMobj* new_obj)
{
	GEOMobj	**tmp_obj;
	int oldsize;
		
#ifdef DEBUG
	fprintf(stderr,"in geom_objects::add_obj numobj=%i\n",num_obj); fflush(stderr);
#endif
	num_obj++;
	if (num_obj>=max_obj) {
		oldsize = max_obj;
		max_obj+=200;	
		tmp_obj = (GEOMobj**) GobjRealloc(obj_list,max_obj*sizeof(GEOMobj*),oldsize*sizeof(GEOMobj*));
		if (!tmp_obj) {
			fprintf(stderr,"Can't allocate memory for new objects.");fflush(stderr);
			return -1;
		}
		obj_list=tmp_obj;
	}
#ifdef DEBUG
	fprintf(stderr,"add obj_num: %d\n",num_obj); fflush(stderr);
#endif
	obj_list[num_obj]=new_obj;
#ifdef DEBUG
	fprintf(stderr,"Object added obj_num: %d \t %lp\n",num_obj,new_obj); fflush(stderr);
#endif
	return num_obj;
}

GEOMobj* geom_objects::get_obj (int num)
{
	if (num>=num_obj) 
		return NULL;
	return obj_list[num];
}

GEOMobj** geom_objects::get_all_objs ()
{
	return obj_list;
}

int geom_objects::get_nobjs ()
{
	return (num_obj+1);
}

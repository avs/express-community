#ifndef _GROW_REGION_H_
#define _GROW_REGION_H_

#include "stdlib.h"
#include "stdio.h"

class grow_region {
	public :
		grow_region (int veclen, int dims[3], double* fld_data);
		~grow_region();
		void grow (int x, int y, int z); 
		void set_out_value(int out_val, int null_val); 
		void set_delta(double * vec);
		void set_increment(double * vec);
		void set_min_max(int x, int y, int z); 
		void set_diagonals(int diagonals); 
		void set_and_precedence(int and_prec);
		void get_out_data (int ** out);
		void set_and_or (int * vec);
		
	private :
		double * in_data;
		int dims[3];
		int use_diagonals;
		int out_value;
		int null_value;
		int veclen;
		int array_size;
		double * min_vec;
		double * max_vec;
		double * delta;
		double * increment;
		int * out_data;
		int * and_or;
		int and_precedence;
		int vals_set;
		void* grow_realloc (void* ptr, size_t size, size_t oldsize);
		int add_point(int x, int y, int z, int index_old);
		int use_point();
		int point_check(int x, int y, int z);
		int value_check(int index, int index_old);
		int and_check(int index, int index_old, int result);
		int or_check(int index, int index_old);
		int _3D;
		
		int grow_x;
		int grow_y;
		int grow_z;
		int grow_data;
		int *grow_points;
		int max_growpoints;
		int growptr;
		int addptr;
		int numvals;
};

#endif

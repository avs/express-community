/*
			Copyright (c) 2004 by
			Advanced Visual Systems Inc.
			All Rights Reserved

	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

	This file is under Perforce control
	$Id: //depot/express/trunk/segm_edtr/seggrow.h#1 $
*/

#ifndef _SEGEDIT_GROW_REGION_H_
#define _SEGEDIT_GROW_REGION_H_

class segment_grow_region {
	public :
		segment_grow_region(int veclen, int dims[3], double* fld_data);
		segment_grow_region (int dims[3], double* fld_data, unsigned char ** out, int veclen=1);
		~segment_grow_region();
		void grow (int x, int y, int z);
		void set_out_value(unsigned char out_val, unsigned char null_val=0, int init_out=0);
		void set_delta(double * vec);
		void set_params(int x, int y, int z, double delta, int incremental=0, int and_or=1, int diagonals=0, int and_prec=1);
		void set_increment(double * vec);
		void set_min_max(int x, int y, int z);
		void set_diagonals(int diagonals);
		void set_and_precedence(int and_prec);
		void get_out_data (unsigned char ** out);
		void set_and_or (int * vec);

	private :
		double * in_data;
		int dims[3];
		int use_diagonals;
		unsigned char out_value;
		unsigned char null_value;
		int veclen;
		int array_size;
		double * min_vec;
		double * max_vec;
		double * delta;
		double * increment;
		unsigned char * out_data;
		unsigned char * out;
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

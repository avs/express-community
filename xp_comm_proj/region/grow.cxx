#include "grow.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"


void* grow_region::grow_realloc (void* ptr, size_t size, size_t oldsize)
{
#ifdef MS_DOS
	/* fix for missing realloc in MS VC6.0 */
	void *new;
	if (new=(void*)malloc(size))
	{
		memcpy(new,ptr,oldsize);
		free(ptr);
		ptr=new;
	}
	return new;
#else
	return realloc(ptr,size);
#endif
}


int grow_region::add_point(int x, int y, int z, int index_old)
{
	int * tmp;
	int index;

	if (!point_check(x,y,z)) return 2;	

	index = (z * dims[1] + y) * dims[0] + x;

	if (out_data[index] == out_value) return 2;
	if (!value_check(index,index_old)) return 2;	

	out_data[index]=out_value;
	
	if (addptr==growptr)
	{
		addptr=max_growpoints;
		max_growpoints += 500;
		tmp = (int*)grow_realloc(grow_points,4*max_growpoints*sizeof(int),4*addptr*sizeof(int));
		if (!tmp) { 
			max_growpoints = addptr;
			fprintf(stderr,"region_incomplete\n");		
			return 0;
		}
		grow_points = tmp;
	}	
	grow_points[4*addptr] = x;
	grow_points[4*addptr+1] = y;
	grow_points[4*addptr+2] = z;
	grow_points[4*addptr+3] = index;

	if (++addptr == max_growpoints) addptr = 0;
	
	numvals++;
	return 1;
}


int grow_region::use_point()
{
	if (!numvals) return 0;

	if (++growptr == max_growpoints) 
		growptr = 0;
	numvals--;
	
	grow_x = grow_points[4*growptr];		// x_coordinate
	grow_y = grow_points[4*growptr+1];		// y_coordinate
	grow_z = grow_points[4*growptr+2];		// z_coordinate
	grow_data = grow_points[4*growptr+3];	// index of datavalue to compare
	return 1;
}


grow_region::grow_region(int veclen, int dims[3], double* fld_data)
{
	int i;
	this->veclen = veclen;
	memcpy(this->dims,dims,3*sizeof(int));
	delta = new double [veclen];
	increment = new double [veclen];
	min_vec = new double [veclen];
	max_vec = new double [veclen];
	and_or	= new int [veclen];
	_3D = (dims[2])?1:0;
	array_size = dims[1]*dims[0];
	array_size *= (dims[2])?dims[2]:1;
	in_data = fld_data;
	vals_set = 0; 
	out_data = new int [array_size];
	max_growpoints = 500;
	grow_points = (int*)malloc(4*max_growpoints*sizeof(int));
	for (i=0;i<4*max_growpoints;i++) grow_points[i]=0;
	addptr=0;
	growptr=0;
	numvals=0;
}


grow_region::~grow_region()
{
	delete[] out_data;
	delete[] max_vec;
	delete[] min_vec;
	delete[] delta;
	delete[] increment;
	delete[] and_or;
	free (grow_points);
}


void grow_region::get_out_data(int** out)
{
	memcpy(*out,out_data,array_size*sizeof(int));
}


void grow_region::set_min_max(int x, int y, int z)
{
	int index;
	
	index = (z * dims[1] + y) * dims[0] + x;

   	for (int i=0; i<veclen; i++){
		if (delta[i]) {
			min_vec[i] = in_data[index*veclen+i] - delta[i];
			max_vec[i] = in_data[index*veclen+i] + delta[i];
		} else {
			min_vec[i] = max_vec[i] = 0;
		}
	}

}


void grow_region::grow(int x,int y,int z)
{
	int index;
	

	if (!point_check(x,y,z)) return;	

	index = (z * dims[1] + y) * dims[0] + x;
	
	addptr=1;
	add_point (x,y,z,index);
	growptr=0;
	
	out_data[index]=out_value;			

	while(numvals)
	{		
		use_point();
		
		add_point (grow_x+1, grow_y  , grow_z, grow_data);
		add_point (grow_x  , grow_y+1, grow_z, grow_data);
		add_point (grow_x-1, grow_y  , grow_z, grow_data);
		add_point (grow_x  , grow_y-1, grow_z, grow_data);

		if (_3D) {
			add_point (grow_x, grow_y, grow_z+1,grow_data);
			add_point (grow_x, grow_y, grow_z-1,grow_data);
		}

		if (use_diagonals) {

			add_point (grow_x+1, grow_y+1, grow_z, grow_data);
			add_point (grow_x+1, grow_y-1, grow_z, grow_data);
			add_point (grow_x-1, grow_y+1, grow_z, grow_data);
			add_point (grow_x-1, grow_y-1, grow_z, grow_data);

			if (_3D) {
				add_point (grow_x+1, grow_y+1, grow_z+1, grow_data);
				add_point (grow_x+1, grow_y+1, grow_z-1, grow_data);
				add_point (grow_x+1, grow_y-1, grow_z+1, grow_data);
				add_point (grow_x+1, grow_y-1, grow_z-1, grow_data);		
				add_point (grow_x-1, grow_y+1, grow_z+1, grow_data);
				add_point (grow_x-1, grow_y+1, grow_z-1, grow_data);
				add_point (grow_x-1, grow_y-1, grow_z+1, grow_data);
				add_point (grow_x-1, grow_y-1, grow_z-1, grow_data);
				add_point (grow_x+1, grow_y  , grow_z+1, grow_data);
				add_point (grow_x+1, grow_y  , grow_z-1, grow_data);
				add_point (grow_x  , grow_y+1, grow_z+1, grow_data);
				add_point (grow_x  , grow_y+1, grow_z-1, grow_data);
				add_point (grow_x  , grow_y-1, grow_z+1, grow_data);
				add_point (grow_x  , grow_y-1, grow_z-1, grow_data);
				add_point (grow_x-1, grow_y  , grow_z+1, grow_data);
				add_point (grow_x-1, grow_y  , grow_z-1, grow_data);
			}
		}
	}
}


void grow_region::set_diagonals(int diagonals)
{
	use_diagonals = diagonals;
}


void grow_region::set_and_precedence(int and_prec)
{
	and_precedence = and_prec;
}


void grow_region::set_out_value (int out_val, int null_val)
{
	int i;
	out_value = out_val;
	null_value = null_val;
	for (i=0;i<array_size;i++) out_data[i]=null_value;
}


void grow_region::set_delta(double* vec)
{
	memcpy(delta,vec,veclen*sizeof(double));
}


void grow_region::set_and_or(int* vec)
{
	memcpy(and_or,vec,veclen*sizeof(int));
}


void grow_region::set_increment(double* vec)
{
	memcpy(increment,vec,veclen*sizeof(double));
}


int grow_region::point_check(int x,int y,int z)
{
	if (x<0) return 0;
	if (y<0) return 0;

	if (x>=dims[0]) return 0;
	if (y>=dims[1]) return 0;

	if (_3D)
	{
		if (z<0) return 0;
		if (z>=dims[2]) return 0;
	}
	return 1;
};


int grow_region::and_check(int index, int index_old, int result)
{
	int i;
	// check all components with and
	for (i=0; i<veclen; i++){
		if (!and_or[i]) {
			result=1;
			if (increment[i]) { 
				// do incremental region grow
	  			if (fabs(in_data[index*veclen+i]-in_data[index_old*veclen+i]) > increment[i]) 
					return 0;
			} else {
				// check data_value
				if (in_data[index*veclen+i] > max_vec[i]) return 0;
				if (in_data[index*veclen+i] < min_vec[i]) return 0;
			}
		}
	}
	return result;
}


int grow_region::or_check(int index, int index_old)
{
	int i;
	for (i=0; i<veclen; i++){
		if (and_or[i]) {
			if (increment[i]) { 
				// do incremental region grow
	  			if (fabs(in_data[index*veclen+i]-in_data[index_old*veclen+i]) <= increment[i]) 
					return 1;
			} else {
				// check data_value
				if ((in_data[index*veclen+i] <= max_vec[i]) && (in_data[index*veclen+i] >= min_vec[i])) 
					return 1;
			}
		}
	}	
	return 0 ;
};


int grow_region::value_check(int index, int index_old)
{
	int result;	

	if (and_precedence) 
	{
		result = and_check(index, index_old, 0);
		if (result) return result;
		result = or_check(index, index_old);
	} else {
		result = or_check(index, index_old);
		if (!result) return result;
		result = and_check(index, index_old, 1);
	}
	return result;
}


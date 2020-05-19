#include "cropbox.h"
#include <memory.h>

extern "C" int compare_ints(const void *pa, const void *pb, void *param)
{
	if(*(int *)pa < *(int *)pb)			return -1;
	else if(*(int *)pa > *(int *)pb)	return +1;
	else								return  0;
}

void destroy(void *avl_item, void *avl_param)
{
	delete [] avl_item;
}

CropBox::CropBox()
{
	nspace = 0;
	nincluded = 0;

	tree = avl_create(compare_ints, NULL, NULL);
}

CropBox::~CropBox()
{
	avl_destroy(tree, destroy);
	cell_area.clear();
	valid_cell_list.clear();
}

//
// Prepare list of valid points (1D)
//
int CropBox::CheckCoords1D(float *in_coordinates, int nnodes,
						   float xmin, float xmax)
{
	nincluded = 0;
	nspace = 1;

	for(int i=0; i < nnodes; i++)
	{
		if(in_coordinates[i] < xmin) continue;
		if(in_coordinates[i] > xmax) continue;

		++nincluded;

		int *x = new int[2];
		x[0] = i;
		avl_insert(tree, x);
	}

	return nincluded;
}

//
// Prepare list of valid points (2D)
//
int CropBox::CheckCoords2D(float *in_coordinates, int nnodes,
						   float xmin, float xmax, float ymin, float ymax)
{
	int i, j;
	nincluded = 0;
	nspace = 2;

	for(i=j=0; i < nnodes; i++,j+=2)
	{
		if(in_coordinates[j+0] < xmin) continue;
		if(in_coordinates[j+0] > xmax) continue;
		if(in_coordinates[j+1] < ymin) continue;
		if(in_coordinates[j+1] > ymax) continue;

		++nincluded;

		int *x = new int[2];
		x[0] = i;
		avl_insert(tree, x);
	}

	return nincluded;
}

//
// Prepare list of valid points (3D)
//
int CropBox::CheckCoords3D(float *in_coordinates, int nnodes,
						   float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
	int i, j;
	nincluded = 0;
	nspace = 3;

	for(i=j=0; i < nnodes; i++,j+=3)
	{
		if(in_coordinates[j+0] < xmin) continue;
		if(in_coordinates[j+0] > xmax) continue;
		if(in_coordinates[j+1] < ymin) continue;
		if(in_coordinates[j+1] > ymax) continue;
		if(in_coordinates[j+2] < zmin) continue;
		if(in_coordinates[j+2] > zmax) continue;

		++nincluded;

		int *x = new int[2];
		x[0] = i;
		avl_insert(tree, x);
	}

	return nincluded;
}

//
//	Copy coordinates of valid points and update the tree with the index of the node in the
//	output field
//
void CropBox::ValidPoints(float *in_coordinates, float *out_coordinates)
{
	int *item;
	int i, j;
	struct avl_traverser trav;

	switch(nspace)
	{
	case 1:
		for(item = (int *)avl_t_first(&trav, tree),i=0; item != NULL; item = (int *)avl_t_next(&trav),i++)
		{
			out_coordinates[i] = in_coordinates[item[0]];
			item[1] = i;
		}
		break;

	case 2:
		for(item = (int *)avl_t_first(&trav, tree),i=j=0; item != NULL; item = (int *)avl_t_next(&trav),i+=2,j++)
		{
			out_coordinates[i+0] = in_coordinates[item[0]*2+0];
			out_coordinates[i+1] = in_coordinates[item[0]*2+1];
			item[1] = j;
		}
		break;

	case 3:
		for(item = (int *)avl_t_first(&trav, tree),i=j=0; item != NULL; item = (int *)avl_t_next(&trav),i+=3,j++)
		{
			out_coordinates[i+0] = in_coordinates[item[0]*3+0];
			out_coordinates[i+1] = in_coordinates[item[0]*3+1];
			out_coordinates[i+2] = in_coordinates[item[0]*3+2];
			item[1] = j;
		}
		break;
	};
}

// Table of data type size from DTYPE_* constants
static int size_table[] = {
	1, 1, sizeof(short), sizeof(int), sizeof(float), sizeof(double), 0, 0, 0, 0, 0
};

void CropBox::CopyNodeData(char *in_node_data, char *out_node_data, int veclen, int datatype)
{
	int *item;
	int i;
	struct avl_traverser trav;
	int elem_size = veclen*size_table[datatype];

	for(item = (int *)avl_t_first(&trav, tree),i=0; item != NULL; item = (int *)avl_t_next(&trav),i++)
	{
		memcpy(out_node_data+i*elem_size, in_node_data+item[0]*elem_size, elem_size);
	}
}

int CropBox::PrepareCells(int *connect_list, int ncells, int cell_nnodes)
{
	int i, j;
	int vertex[20]; // Max number of vertex of a cell
	int *p;
	int valid_cells = 0;

	// Empty temporary lists
	cell_area.clear();
	valid_cell_list.clear();

	for(i=0; i < ncells; i++)
	{
		// Find if all the cell vertex are inside the crop volume
		int this_cell = 0;
		for(j=0; j < cell_nnodes; j++)
		{
			if((p = (int *)avl_find(tree, &connect_list[i*cell_nnodes+j])) != NULL)
			{
				this_cell++;
				vertex[j] = p[1];
			}
		}

		// If yes, save the cell connect list
		if(this_cell == cell_nnodes)
		{
			valid_cells++;

			// Save cell nodes
			for(j=0; j < cell_nnodes; j++)
			{
				cell_area.push_back(vertex[j]);
			}

			// Save the cell number
			valid_cell_list.push_back(i);
		}
	}

	return valid_cells;
}

void CropBox::CopyCells(int *connect_list, int ncells, int cell_nnodes)
{
	for(int i=0; i < ncells*cell_nnodes; i++)
	{
		connect_list[i] = cell_area[i];
	}
}

//
// Copy the cell data of the cell set just Prepared()
//
void CropBox::CopyCellData(char *in_values, char *out_values, int veclen, int data_type)
{
	int elem_size = veclen*size_table[data_type];

	for(int i=0; i < valid_cell_list.size(); i++)
	{
		memcpy(out_values+elem_size*i, in_values+elem_size*valid_cell_list[i], elem_size);
	}
}

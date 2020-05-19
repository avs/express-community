extern "C"
{
#include "avl.h"
}
#pragma warning(disable:4786) 
#include <vector>
using namespace std;

class CropBox
{
public:
	// Constructor/destructor
	CropBox();
	~CropBox();

	// Prepare list of valid points
	int CheckCoords1D(float *in_coordinates, int nnodes,
				float xmin, float xmax);
	int CheckCoords2D(float *in_coordinates, int nnodes,
				float xmin, float xmax, float ymin, float ymax);
	int CheckCoords3D(float *in_coordinates, int nnodes,
				float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);

	// Copy point coordinates of valid points
	void ValidPoints(float *in_coordinates, float *out_coordinates);

	// Copy valid points node data
	void CopyNodeData(char *in_node_data, char *out_node_data, int veclen, int datatype);

	// Prepare a list of cells contained inside the crop box
	int PrepareCells(int *connect_list, int ncells, int cell_nnodes);

	// Output the connection list of valid cells
	void CopyCells(int *connect_list, int ncells, int cell_nnodes);

	// Copy the cell data of the cell set just Prepared()
	void CopyCellData(char *in_values, char *out_values, int veclen, int data_type);

private:
	int nspace;					// of the input field
	int nincluded;				// number of points included
	struct avl_table *tree;		// contains a list of valid points (old-index, nex-index)
	vector<int> cell_area;		// temporary connectivity list
	vector<int> valid_cell_list;// list of cells included

};
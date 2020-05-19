#include <stdio.h>
#include "Utils.h"
#include "GridClasses.h"
#include "ConnectivityClasses.h"
#include "MaterialsClasses.h"

//database dei vertici del modello
class GridDB: public IndexableArray
{
	public:
	GridDB(int a): IndexableArray(a){};
	~GridDB();
	void ReturnXPcoords(float* ret_array);

};
//database della connettivita' del modello  
class NastranCellDB: public IndexableArray
{
	public:
	int Cardinality;
	NastranCellDB(int a,int cardin);
	~NastranCellDB();
	void ReturnXPconnectivity(int* ret_array,GridDB *theGridDB);
	void ReturnMaterialInfo(int* ret_array);
};

//insieme dei PSolid 3D
class PSolidDB: public IndexableArray
{
	public:
	PSolidDB(int a): IndexableArray(a){};
	~PSolidDB();
	void SubstituteMaterial(NastranCellDB* aCellDB);
};

//database dei coordsys del modello
class CoordSysDB: public IndexableArray
{
	public:
	CoordSysDB(int a): IndexableArray(a){};
	~CoordSysDB();
	void ApplyGridTransformation(GridDB* aGridDB);
};


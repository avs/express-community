#include "WP1_MeshClasses.h"
#include "DBFparserClasses.h"
#include <math.h>

//dato un array della giusta dimensione, lo riempie con le coordinate dei nodi
void GridDB::ReturnXPcoords(float* ret_array){ 
	for (int i=0;i< Size ;i++ )
	{
		ret_array[i*3]=((GridPoint*)TheArray[i])->V1;
		ret_array[i*3+1]=((GridPoint*)TheArray[i])->V2;
		ret_array[i*3+2]=((GridPoint*)TheArray[i])->V3;
	}
};

GridDB::~GridDB(){
	for (register i=0;i<Size ;i++ )
	{
		delete TheArray[i];
	}
};

#include <avs/math.h>

void CSApplyGridTrans(GridPoint* aGP, CoordSys* CSys) {

	if (aGP && CSys) {
		int CSysType=-1;
	float Rho;
	float Theta;
	float Zeta;
	float Deg2Rad = 2.0*acos(-1.)/360.0;

		CSysType = CSys->SType;

		if (CSysType==DCORD2C) {
				//convert Cylindrical Coords into Cartesian Coords
				Rho = aGP->V1;
				Theta = (aGP->V2)*Deg2Rad;
				Zeta = aGP->V3;
				aGP->V1 = Rho * cos(Theta);
				aGP->V2 = Rho * sin(Theta);
				aGP->V3 = Zeta;
		}
		if (CSysType==DCORD2R) {
				
		}

		float P[3];
		//then rotate according to B,C
		P[0] = aGP->V1;
		P[1] = aGP->V2;
		P[2] = aGP->V3;

		MATvec3_mat4_multiply(P, (Matr4 *)(CSys->Mat));


		//translate according to A
		aGP->V1 = P[0] + CSys->A[0];
		aGP->V2 = P[1] + CSys->A[1];
		aGP->V3 = P[2] + CSys->A[2];

	}
}

void CoordSysDB::ApplyGridTransformation(GridDB* aGridDB) {
	//not yet fully implemented

	int NNodes = aGridDB->Size;
	int II=0;
	GridPoint* aGP;
	int CSysID=-1;
	CoordSys* CSys=NULL;
	for (II=0;II<NNodes;II++) {
		aGP = (GridPoint*)(aGridDB->TheArray[II]);
		CSysID = aGP->ID2; //get the CoordSys ID
		if (CSysID>0) { //if not defined means basic system, leave the node as it is
			CSys = (CoordSys*) operator[](CSysID);
			if (CSys) {
				CSApplyGridTrans(aGP, CSys);
			} else {
				printf("CoordSysDB::ApplyGridTransformation Warning: cannot find CoordSys ID: %d\n",CSysID);
			}				
		}
	}
}

CoordSysDB::~CoordSysDB(){
	for (register i=0;i<Size ;i++ )
	{
		delete TheArray[i];
	}
};


NastranCellDB::NastranCellDB(int a,int cardin): IndexableArray(a){
	Cardinality=cardin;
};

NastranCellDB::~NastranCellDB(){
	for (register i=0;i<Size ;i++ )
	{
		delete TheArray[i];
	}
};

//costruisce l'array di connettivita' per XP
void NastranCellDB::ReturnXPconnectivity(int* ret_array,GridDB *theGridDB){
	int idbuf;
	int ndxbuf;
	printf("NastranCellDB::ReturnXPconnectivity (%d)\n",Size);	
	for (int j=0;j<Size ;j++ )
	{
		for (int i=0;i<Cardinality ;i++ )
		{
				idbuf=((NastranCell*)TheArray[j])->Gi[i];
				ndxbuf = theGridDB->Index_Of(idbuf);
				ret_array[j*Cardinality+i]= ndxbuf;
				if (ndxbuf<0) {
					printf("NastranCellDB::ReturnXPconnectivity Cannot map [%d] ID: %d\n",i,idbuf);
				}
		}
	}
};

void NastranCellDB::ReturnMaterialInfo(int* ret_array){
	int idbuf;
	for (int j=0;j<Size ;j++ )
	{
				ret_array[j]=((NastranCell*)TheArray[j])->MID;
	}
};


PSolidDB::~PSolidDB() {
	for (register i=0;i<Size ;i++ )
	{
		delete TheArray[i];
	}
}
void PSolidDB::SubstituteMaterial(NastranCellDB* aCellDB) {
	int NCells = aCellDB->Size;
	int II=0;
	NastranCell* aNC;
	int PSolidID=-1;
	PSolid* PS=NULL;

	for (II=0;II<NCells;II++) {
		aNC = (NastranCell*)(aCellDB->TheArray[II]);
		PSolidID = aNC->MID; //get the PSolid ID (stored in the MID)
		if (PSolidID!=-1) { //if not undefined
			PS = (PSolid*) operator[](PSolidID);
			if (PS) {
				aNC->MID = PS->MID;
			} else {
					printf("PSolidDB::SubstituteMaterial Warning: cannot find PSolid ID: %d\n",PSolidID);
			}				
		}
	}

}

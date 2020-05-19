#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utils.h"

#ifndef GRIDPOINT_PARAPARA
#define GRIDPOINT_PARAPARA

//classe per lettura vertici
// usata nel WP2 per leggere un nodo
// usata nel WP1 per costruire il DB dei nodi con le vite
class GridPoint: public Indexable
{
	public:
		//int ID;//should be NODE ID
		int ID2;//ID of the CoordSys
		int ID3;//maybe NODE ID again, maybe ZONE, maybe COORDSYS
		float V1;//vertexes
		float V2;
		float V3;

		int Life;
		float TimeMin;
		float TimeMax;
		float TempMin;
		float TempMax;
		float StressMin;
		float StressMax;

		GridPoint():Indexable(-1){
			ID2=ID3=-1;
			V1=V2=V3=-1.0;
			Life=-1;
			TimeMin=TimeMax=TempMin=TempMax=StressMin=StressMax=-1.0;
		};
		GridPoint(int id,int id2, int id3,float v1,float v2,float v3,
						int life,float time_min, float time_max, 
						float temp_min, float temp_max,
						float stress_min, float stress_max );
		operator!=(const GridPoint& rv) const{
			return(ID!=rv.ID);
		}
};

GridPoint * ParseGridCard(char* line,FILE* fp);

//classe per lettura sistemi di riferimento
class CoordSys: public Indexable
{
	public:
		//int ID;//should be NODE ID
		int ID2;//ID of the independed CoordSys
		int SType;
		float A[3];
		float B[3];
		float C[3];

		float BA[3];
		float CA[3];
		float Xnorm[3];
		float Ynorm[3];
		float Znorm[3];
		float Mat[4][4];

		CoordSys():Indexable(-1){
			ID2=-1;
			SType=-1;
			A[0]=A[1]=A[2]=B[0]=B[1]=B[2]=C[0]=C[1]=C[2]=0.0;
		};
		CoordSys(int a, int b,int st,float a1,float a2,float a3,float b1,float b2,float b3,float c1,float c2,float c3);
		operator!=(const CoordSys& rv) const{
			return(ID!=rv.ID);
		}
		void Serialize(ofstream &fp);
};

CoordSys * ParseCoordSysCard(char* line,FILE* fp);

#endif

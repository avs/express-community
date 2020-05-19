
#include "express.h"
#include "DBFparserClasses.h"
#include "WP1_MeshClasses.h"
#include <math.h>
#include <fstream.h>
#include <limits.h>
#include "Utils.h"


int
ReadNastran_ReadNastranMods_ReadNastranCore::Import(OMevent_mask event_mask, int seq_num)
{

   	char * FileName_app=NULL;
	FileName_app = (char*)ReadNastranParams.filename;
	FILE* fp;

	fp=fopen(FileName_app,"r");
	if (fp==NULL) {
		printf("can't find %s",(char*)FileName_app);
		return 0;
	}
	char line[1024];
	char LastComment[1024];
	char line2[1024];
	char* tmp;

	GFstrset(line,' ',1024);;
	tmp=fgets(line,1024,fp);
//sizes of all parsable cards
	int cardtype;
	int ngrid=0;
	int ntria=0;
	int ntetra4 =0;
	int ntetra10=0;
	int nhexa8  =0;
	int nhexa20 =0;
	int npenta6 =0; 
	int npenta15=0; 
	int ncoordsys=0;
	int npsolid =0;
	int IsHexa8=1;
	int IsPenta6=1;
	int IsTetra4=1;
	int nTotalCards=0;
	int nCards=0;
//(1) faccio un primo scan per determinare le varie sizes
	int i=0;
	cardtype=DetectCardType(line); 
	while ((cardtype!=DENDDATA))//esimate all sizes
	{ 
		switch (cardtype)
		{
		case DGRID: nCards++;ngrid++;GFstrset(line,' ',1024);;tmp=fgets(line,1024,fp);break;
		case DCTRIAX6:nCards++;ntria++;GFstrset(line,' ',1024);;tmp=fgets(line,1024,fp);break;
		case DCTETRA  :nCards++;
			IsTetra4 = ParseCtetraCard_Is4(line,fp);
			if (IsTetra4) {
				ntetra4++;
			} else {
				ntetra10++;
			}
			GFstrset(line,' ',1024);tmp=fgets(line,1024,fp); 
			break;
		case DCHEXA   :nCards++;
			IsHexa8 = ParseChexaCard_Is8(line,fp);
			if (IsHexa8) {
				nhexa8++;
			} else {
				nhexa20++;
			}
			GFstrset(line,' ',1024);tmp=fgets(line,1024,fp); 
			break;
		case DCPENTA  :nCards++;
			IsPenta6 = ParseCpentaCard_Is6(line,fp); //okkio this gets extra line already
			if (IsPenta6) {
				npenta6++;
			} else {
				npenta15++;
			}
			break;
		case DCORD2R :nCards++;ncoordsys++;GFstrset(line,' ',1024);tmp=fgets(line,1024,fp);break;
		case DCORD2C :nCards++;ncoordsys++;GFstrset(line,' ',1024);tmp=fgets(line,1024,fp);break;

		case DPSOLID  :nCards++;npsolid++;GFstrset(line,' ',1024);;tmp=fgets(line,1024,fp); break;

		case DOTHER:nCards++;GFstrset(line,' ',1024);;tmp=fgets(line,1024,fp);break;
		case DCOMMENT:nCards++;memcpy(LastComment,line,1024); GFstrset(line,' ',1024);;tmp=fgets(line,1024,fp); break	;

		case DENDDATA:nCards++;break	;		 	
		default:nCards++;GFstrset(line,' ',1024);;tmp=fgets(line,1024,fp);break	; 
		} 
	cardtype=DetectCardType(line);  
	}
	fclose(fp);

	nTotalCards = nCards;
	printf("TotalCard:%d\n",nTotalCards);
	int is2d_flag=(ntria)? 1 : 0; //flag per stabilire se e' un modello 2D 

	printf("(2)alloco tutti i DB delle varie schede\n");

	//(2)alloco tutti i DB delle varie schede
	GridDB* VertexDB=new GridDB(ngrid);
	CoordSysDB* theCoordSysDB=new CoordSysDB(ncoordsys);
	NastranCellDB* Triax6DB=new NastranCellDB(ntria,6);
	NastranCellDB* Hexa8DB=new NastranCellDB(nhexa8,8);
	NastranCellDB* Hexa20DB=new NastranCellDB(nhexa20,20);
	NastranCellDB* Penta6DB=new NastranCellDB(npenta6,6);
	NastranCellDB* Penta15DB=new NastranCellDB(npenta15,15);
	NastranCellDB* Tetra4DB=new NastranCellDB(ntetra4,4);
	NastranCellDB* Tetra10DB=new NastranCellDB(ntetra10,10);

	PSolidDB* thePSolidDB=new PSolidDB(npsolid);


	//dichiara gli elementi buffer
	GridPoint *TheGridPoint;
	int currGridPoint=0;
	CoordSys *theCoordSys;
	int currCoordSys=0;
	Ctriax6 *TheCtriax6;
	int currCtriax6=0;
	Chexa *TheChexa;
	int currChexa8=0;
	int currChexa20=0;
	Cpenta *TheCpenta;
	int currCpenta6=0;
	int currCpenta15=0;
	Ctetra *TheCtetra;
	int currCtetra4=0;
	int currCtetra10=0;

	PSolid* ThePSolid;
	int currPSolid=0;
	nCards = 0;

	printf("reopen DB & parsing Mesh File\n");

//(3) riapro e popolo il database
	fp=fopen(FileName_app,"r");
	if (fp==NULL) {
		printf("can't find %s",(char*)FileName_app);
		return 0;
	}
	GFstrset(line,' ',1024);;
	tmp=fgets(line,1024,fp);
	cardtype=DetectCardType(line); 

	int UserStop=0;
	float Percentage=0.;

	OMstatus_check(0,"Start",&UserStop);



	char AppDbg[10];

	while ((cardtype!=DENDDATA))
	{
		//printf("CardType: %d\n",cardtype);
		switch (cardtype)
		{
		case DGRID: nCards++;
					TheGridPoint = ParseGridCard(line,fp);
					VertexDB->TheArray[currGridPoint] = TheGridPoint;
					currGridPoint++;
					GFstrset(line,' ',1024);;
					tmp=fgets(line,1024,fp);break;
		case DCTRIAX6:
					nCards++;
					TheCtriax6 = ParseCtriax6Card(line);
					Triax6DB->TheArray[currCtriax6] = TheCtriax6;
					currCtriax6++;
					GFstrset(line,' ',1024);;
					tmp=fgets(line,1024,fp);break; 
		case DCTETRA   :
					nCards++;
					TheCtetra = ParseCtetraCard(line,fp);
					if (TheCtetra->Cardinality==4) {
						Tetra4DB->TheArray[currCtetra4] = TheCtetra;
						currCtetra4++;
					} else {
						Tetra10DB->TheArray[currCtetra10] = TheCtetra;
						currCtetra10++;
					}
					GFstrset(line,' ',1024);
					tmp=fgets(line,1024,fp);
					break; 
		case DCHEXA   :
					nCards++;
					TheChexa = ParseChexaCard(line,fp);
					if (TheChexa->Cardinality==8) {
						Hexa8DB->TheArray[currChexa8] = TheChexa;
						currChexa8++;
					} else {
						Hexa20DB->TheArray[currChexa20] = TheChexa;
						currChexa20++;
					}
					GFstrset(line,' ',1024);
					tmp=fgets(line,1024,fp);
					break; 
		case DCPENTA   :
					nCards++;
					TheCpenta = ParseCpentaCard(line,fp);
					if (TheCpenta->Cardinality==6) {
						Penta6DB->TheArray[currCpenta6] = TheCpenta;
						currCpenta6++;
					} else {
						Penta15DB->TheArray[currCpenta15] = TheCpenta;
						currCpenta15++;
					}
					//the Parses of the card gets already an extra line
					break; 
		case DCORD2R :
					nCards++;
					theCoordSys = ParseCoordSysCard(line,fp);
					theCoordSysDB->TheArray[currCoordSys] = theCoordSys;
					currCoordSys++;
				break;
		case DCORD2C :
					nCards++;
					theCoordSys = ParseCoordSysCard(line,fp);
					theCoordSysDB->TheArray[currCoordSys] = theCoordSys;
					currCoordSys++;				
				break;
		case DPSOLID  :	
					nCards++;
						ThePSolid = ParsePSOLIDCard(line);
						thePSolidDB->TheArray[currPSolid] = ThePSolid;
						currPSolid++;
						GFstrset(line,' ',1024);;
						tmp=fgets(line,1024,fp);
						break; 
		case DOTHER:
					nCards++;
						GFstrset(line,' ',1024);;
						tmp=fgets(line,1024,fp);
						break;
		case DCOMMENT:
					nCards++;
						memcpy(LastComment,line,1024);
						GFstrset(line,' ',1024);
						tmp=fgets(line,1024,fp);
						break;

		case DENDDATA:
					nCards++;
					break	;		 	
		default:		
					nCards++;
						GFstrset(line,' ',1024);;
						tmp=fgets(line,1024,fp);
						break; 
		} 
		cardtype=DetectCardType(line);  
		if ((nCards%100)==0) {
			Percentage = 100.0 * (float)nCards/(float)nTotalCards;
			OMstatus_check((int)Percentage,"Reading...",&UserStop);
		}
	}
	fclose(fp);


	printf("Mesh File Parsed\n");

	printf("	Sort all DB\n");

	//una volta fatti, ordinali
	int aSize=0;

	aSize = VertexDB->Size;
	printf("	VertexDB(%d)\n",aSize);
	VertexDB->Sort(0,aSize-1);

	aSize = theCoordSysDB->Size;
	printf("	CoordSysDB(%d)\n",aSize);
	theCoordSysDB->Sort(0,aSize-1);

	aSize = Triax6DB->Size;
	printf("	Triax6DB(%d)\n",aSize);
	Triax6DB->Sort(0,aSize-1);

	aSize = Hexa8DB->Size;
	printf("	Hexa8DB(%d)\n",aSize);
	Hexa8DB->Sort(0,aSize-1);

	aSize = Hexa20DB->Size;
	printf("	Hexa20DB(%d)\n",aSize);
	Hexa20DB->Sort(0,aSize-1);

	aSize = Penta6DB->Size;
	printf("	Penta6DB(%d)\n",aSize);
	Penta6DB->Sort(0,aSize-1);

	aSize = Penta15DB->Size;
	printf("	Penta15DB(%d)\n",aSize);
	Penta15DB->Sort(0,aSize-1);
	
	aSize = Tetra4DB->Size;
	printf("	Tetra4DB(%d)\n",aSize);
	Tetra4DB->Sort(0,aSize-1);

	aSize = Tetra10DB->Size;
	printf("	Tetra10DB(%d)\n",aSize);
	Tetra10DB->Sort(0,aSize-1);

	aSize = thePSolidDB->Size;
	printf("	PSolidDB(%d)\n",aSize);
	thePSolidDB->Sort(0,aSize-1);
	
	printf(" All DB Sorted\n");


	// aggiorna gli indici di materiale nel 3D (invece dei PSolid)
	if (thePSolidDB->Size>0) {
			printf("Updating PSolid Indexes to Materials\n");
			thePSolidDB->SubstituteMaterial(Hexa8DB);
			thePSolidDB->SubstituteMaterial(Hexa20DB);
			thePSolidDB->SubstituteMaterial(Penta6DB);
			thePSolidDB->SubstituteMaterial(Penta15DB);
			thePSolidDB->SubstituteMaterial(Tetra4DB);
			thePSolidDB->SubstituteMaterial(Tetra10DB);
	}
	//(2) applica le trasformaz. di coordinate

	printf("Apply Grid Transformation to VertexDB\n");
	theCoordSysDB->ApplyGridTransformation(VertexDB);


	nastran_output.Size = VertexDB->Size;
	nastran_output.CTRIAX6_Size = Triax6DB->Size;
	nastran_output.CTETRA4_Size = Tetra4DB->Size;
	nastran_output.CTETRA10_Size = Tetra10DB->Size;
	nastran_output.CHEXA8_Size = Hexa8DB->Size;
	nastran_output.CHEXA20_Size = Hexa20DB->Size;
	nastran_output.CPENTA6_Size = Penta6DB->Size;
	nastran_output.CPENTA15_Size = Penta15DB->Size;


	int* IDS_arr=NULL;
	int* MAT_arr=NULL;
	float* Coords_arr=NULL;
	int* Connect_arr=NULL;

	//Nodes infos
	IDS_arr=(int*)nastran_output.IDS.ret_array_ptr(OM_GET_ARRAY_RW);
	VertexDB->ReturnID_Array(IDS_arr);
	if (IDS_arr) ARRfree(IDS_arr);
	IDS_arr=NULL;

	Coords_arr=(float*)nastran_output.Coords.ret_array_ptr(OM_GET_ARRAY_RW);
	VertexDB->ReturnXPcoords(Coords_arr);
	if (Coords_arr) ARRfree(Coords_arr);

	//Cells infos

	// TRIAX6
	Connect_arr=NULL;
	IDS_arr=NULL;
	MAT_arr=NULL;
	Connect_arr=(int*)nastran_output.CTRIAX6.ret_array_ptr(OM_GET_ARRAY_RW);
	IDS_arr=(int*)nastran_output.CTRIAX6IDS.ret_array_ptr(OM_GET_ARRAY_RW);
	MAT_arr=(int*)nastran_output.CTRIAX6MATERIALS.ret_array_ptr(OM_GET_ARRAY_RW);

	Triax6DB->ReturnXPconnectivity(Connect_arr,VertexDB);
	Triax6DB->ReturnID_Array(IDS_arr);
	Triax6DB->ReturnMaterialInfo(MAT_arr);

	if (Connect_arr) ARRfree(Connect_arr);
	if (IDS_arr) ARRfree(IDS_arr);
	if (MAT_arr) ARRfree(MAT_arr);
	
	// TETRA4
	Connect_arr=NULL;
	IDS_arr=NULL;
	MAT_arr=NULL;
	Connect_arr=(int*)nastran_output.CTETRA4.ret_array_ptr(OM_GET_ARRAY_RW);
	IDS_arr=(int*)nastran_output.CTETRA4IDS.ret_array_ptr(OM_GET_ARRAY_RW);
	MAT_arr=(int*)nastran_output.CTETRA4MATERIALS.ret_array_ptr(OM_GET_ARRAY_RW);

	Tetra4DB->ReturnXPconnectivity(Connect_arr,VertexDB);
	Tetra4DB->ReturnID_Array(IDS_arr);
	Tetra4DB->ReturnMaterialInfo(MAT_arr);

	if (Connect_arr) ARRfree(Connect_arr);
	if (IDS_arr) ARRfree(IDS_arr);
	if (MAT_arr) ARRfree(MAT_arr);
	
	// TETRA10
	Connect_arr=NULL;
	IDS_arr=NULL;
	MAT_arr=NULL;
	Connect_arr=(int*)nastran_output.CTETRA10.ret_array_ptr(OM_GET_ARRAY_RW);
	IDS_arr=(int*)nastran_output.CTETRA10IDS.ret_array_ptr(OM_GET_ARRAY_RW);
	MAT_arr=(int*)nastran_output.CTETRA10MATERIALS.ret_array_ptr(OM_GET_ARRAY_RW);

	Tetra10DB->ReturnXPconnectivity(Connect_arr,VertexDB);
	Tetra10DB->ReturnID_Array(IDS_arr);
	Tetra10DB->ReturnMaterialInfo(MAT_arr);

	if (Connect_arr) ARRfree(Connect_arr);
	if (IDS_arr) ARRfree(IDS_arr);
	if (MAT_arr) ARRfree(MAT_arr);
	
	// HEXA8
	Connect_arr=NULL;
	IDS_arr=NULL;
	MAT_arr=NULL;
	Connect_arr=(int*)nastran_output.CHEXA8.ret_array_ptr(OM_GET_ARRAY_RW);
	IDS_arr=(int*)nastran_output.CHEXA8IDS.ret_array_ptr(OM_GET_ARRAY_RW);
	MAT_arr=(int*)nastran_output.CHEXA8MATERIALS.ret_array_ptr(OM_GET_ARRAY_RW);

	Hexa8DB->ReturnXPconnectivity(Connect_arr,VertexDB);
	Hexa8DB->ReturnID_Array(IDS_arr);
	Hexa8DB->ReturnMaterialInfo(MAT_arr);

	if (Connect_arr) ARRfree(Connect_arr);
	if (IDS_arr) ARRfree(IDS_arr);
	if (MAT_arr) ARRfree(MAT_arr);
	
	// HEXA20
	Connect_arr=NULL;
	IDS_arr=NULL;
	MAT_arr=NULL;
	Connect_arr=(int*)nastran_output.CHEXA20.ret_array_ptr(OM_GET_ARRAY_RW);
	IDS_arr=(int*)nastran_output.CHEXA20IDS.ret_array_ptr(OM_GET_ARRAY_RW);
	MAT_arr=(int*)nastran_output.CHEXA20MATERIALS.ret_array_ptr(OM_GET_ARRAY_RW);

	Hexa20DB->ReturnXPconnectivity(Connect_arr,VertexDB);
	Hexa20DB->ReturnID_Array(IDS_arr);
	Hexa20DB->ReturnMaterialInfo(MAT_arr);

	if (Connect_arr) ARRfree(Connect_arr);
	if (IDS_arr) ARRfree(IDS_arr);
	if (MAT_arr) ARRfree(MAT_arr);
	
	// PENTA6
	Connect_arr=NULL;
	IDS_arr=NULL;
	MAT_arr=NULL;
	Connect_arr=(int*)nastran_output.CPENTA6.ret_array_ptr(OM_GET_ARRAY_RW);
	IDS_arr=(int*)nastran_output.CPENTA6IDS.ret_array_ptr(OM_GET_ARRAY_RW);
	MAT_arr=(int*)nastran_output.CPENTA6MATERIALS.ret_array_ptr(OM_GET_ARRAY_RW);

	Penta6DB->ReturnXPconnectivity(Connect_arr,VertexDB);
	Penta6DB->ReturnID_Array(IDS_arr);
	Penta6DB->ReturnMaterialInfo(MAT_arr);

	if (Connect_arr) ARRfree(Connect_arr);
	if (IDS_arr) ARRfree(IDS_arr);
	if (MAT_arr) ARRfree(MAT_arr);
	
	// PENTA15
	Connect_arr=NULL;
	IDS_arr=NULL;
	MAT_arr=NULL;
	Connect_arr=(int*)nastran_output.CPENTA15.ret_array_ptr(OM_GET_ARRAY_RW);
	IDS_arr=(int*)nastran_output.CPENTA15IDS.ret_array_ptr(OM_GET_ARRAY_RW);
	MAT_arr=(int*)nastran_output.CPENTA15MATERIALS.ret_array_ptr(OM_GET_ARRAY_RW);

	Penta15DB->ReturnXPconnectivity(Connect_arr,VertexDB);
	Penta15DB->ReturnID_Array(IDS_arr);
	Penta15DB->ReturnMaterialInfo(MAT_arr);

	if (Connect_arr) ARRfree(Connect_arr);
	if (IDS_arr) ARRfree(IDS_arr);
	if (MAT_arr) ARRfree(MAT_arr);
	

	printf(" Delete All DB\n");
	//ritorna

	printf("       VertexDB\n");
	delete VertexDB;
	printf("       Triax6DB\n");
	delete Triax6DB;
	printf("       Hexa8DB\n");
	delete Hexa8DB;
	printf("       Hexa20DB\n");
	delete Hexa20DB;
	printf("       Penta6DB\n");
	delete Penta6DB;
	printf("       Penta15DB\n");
	delete Penta15DB;
	printf("       Tetra4DB\n");
	delete Tetra4DB;
	printf("       Tetra10DB\n");
	delete Tetra10DB;

	printf("       PSolidDB\n");
	delete thePSolidDB;

	printf(" Done Delete All DB\n");
			
	OMstatus_check((int)100,"Done",&UserStop);
	OMstatus_check((int)100,"<idle>",&UserStop);

   // return 1 for success
   return(1);
}


/*

int
modules_PickInfo_Mgr::Update(OMevent_mask event_mask, int seq_num)
{

//check if MasterMesh has data inside

	int NNodes = (int)MasterMesh.TheMesh.Size;
	if (NNodes<=0) return(1);

	//nodes infos
	int verti = (int)PickInfo.verti;
	if ((verti<0) || (verti>=NNodes)) return(1);
	int* IDS_arr=(int*)MasterMesh.TheMesh.IDS.ret_array_ptr(OM_GET_ARRAY_RD);
		PickInfo.NodeID = IDS_arr[verti];
	if (IDS_arr) ARRfree(IDS_arr);

	int* Lifes_arr=(int*)MasterMesh.TheMesh.Lifes.ret_array_ptr(OM_GET_ARRAY_RD);
		PickInfo.NodeLife = Lifes_arr[verti];
	if (Lifes_arr) ARRfree(Lifes_arr);

	int res = QueryBinaryMesh_LifeInfos(PickInfo,MESHheader, MeasurementUnitsLocal,  MeasurementUnitsGlobal);

	float* Coords_arr=(float*)MasterMesh.TheMesh.Coords.ret_array_ptr(OM_GET_ARRAY_RD);
	float* XYZ_arr=(float*)PickInfo.XYZ.ret_array_ptr(OM_GET_ARRAY_WR);
		memcpy(XYZ_arr,Coords_arr+(verti*3),sizeof(float)*3);
	if (Coords_arr) ARRfree(Coords_arr);
	if (XYZ_arr) ARRfree(XYZ_arr);

	//cells infos
	int primi = (int)PickInfo.primi;
	if (primi<0) return(1);
	int CellMaterialID = -1;

	int cell_nnodes = (int)PickInfo.cell_nnodes;
	int cell_ndim = (int)PickInfo.cell_ndim;
	PickInfo.CellType = "Unknown";

	int HConnect[100];
	int HSize=0;
	int* Connect_arr=NULL;

	if ((cell_nnodes==6) && (cell_ndim==2))	{ //CTRIAX6

		PickInfo.CellType = "CTRIAX6";
		int NCTRIAX6 = (int)MasterMesh.TheMesh.CTRIAX6_Size;
		if (primi>=NCTRIAX6) return(1);
		int* CTRIAX6IDS_arr=(int*)MasterMesh.TheMesh.CTRIAX6IDS.ret_array_ptr(OM_GET_ARRAY_RD);
			PickInfo.CellID = CTRIAX6IDS_arr[primi];
		if (CTRIAX6IDS_arr) ARRfree(CTRIAX6IDS_arr);
		int* CTRIAX6MATERIALS_arr=(int*)MasterMesh.TheMesh.CTRIAX6MATERIALS.ret_array_ptr(OM_GET_ARRAY_RD);
			CellMaterialID = CTRIAX6MATERIALS_arr[primi];
			PickInfo.CellMaterialID = CellMaterialID;
		if (CTRIAX6MATERIALS_arr) ARRfree(CTRIAX6MATERIALS_arr);
		
		HSize = 6;
		Connect_arr=(int*)MasterMesh.TheMesh.CTRIAX6.ret_array_ptr(OM_GET_ARRAY_RD);
		HConnect[0]=HConnect[5]=0;
		HConnect[1]=HConnect[2]=1;
		HConnect[3]=HConnect[4]=2;

	}

	if ((cell_nnodes==8) && (cell_ndim==3))	{ //CHEXA8

		PickInfo.CellType = "CHEXA8";
		int NCHEXA8 = (int)MasterMesh.TheMesh.CHEXA8_Size;
		if (primi>=NCHEXA8) return(1);
		int* CHEXA8IDS_arr=(int*)MasterMesh.TheMesh.CHEXA8IDS.ret_array_ptr(OM_GET_ARRAY_RD);
			PickInfo.CellID = CHEXA8IDS_arr[primi];
		if (CHEXA8IDS_arr) ARRfree(CHEXA8IDS_arr);
		int* CHEXA8MATERIALS_arr=(int*)MasterMesh.TheMesh.CHEXA8MATERIALS.ret_array_ptr(OM_GET_ARRAY_RD);
			CellMaterialID = CHEXA8MATERIALS_arr[primi];
			PickInfo.CellMaterialID = CellMaterialID;
		if (CHEXA8MATERIALS_arr) ARRfree(CHEXA8MATERIALS_arr);

		HSize = 24;
		Connect_arr=(int*)MasterMesh.TheMesh.CHEXA8.ret_array_ptr(OM_GET_ARRAY_RD);
		//superior face
		HConnect[0]=HConnect[7]=0;
		HConnect[1]=HConnect[2]=1;
		HConnect[3]=HConnect[4]=2;
		HConnect[5]=HConnect[6]=3;
		//lower face
		HConnect[ 8]=HConnect[15]=4;
		HConnect[ 9]=HConnect[10]=5;
		HConnect[11]=HConnect[12]=6;
		HConnect[13]=HConnect[14]=7;

		HConnect[16]=0; HConnect[17]=4;
		HConnect[18]=1; HConnect[19]=5;
		HConnect[20]=2; HConnect[21]=6;
		HConnect[22]=3; HConnect[23]=7;

		
	}

	if ((cell_nnodes==20) && (cell_ndim==3))	{ //CHEXA20

		PickInfo.CellType = "CHEXA20";
		int NCHEXA20 = (int)MasterMesh.TheMesh.CHEXA20_Size;
		if (primi>=NCHEXA20) return(1);
		int* CHEXA20IDS_arr=(int*)MasterMesh.TheMesh.CHEXA20IDS.ret_array_ptr(OM_GET_ARRAY_RD);
			PickInfo.CellID = CHEXA20IDS_arr[primi];
		if (CHEXA20IDS_arr) ARRfree(CHEXA20IDS_arr);
		int* CHEXA20MATERIALS_arr=(int*)MasterMesh.TheMesh.CHEXA20MATERIALS.ret_array_ptr(OM_GET_ARRAY_RD);
			CellMaterialID = CHEXA20MATERIALS_arr[primi];
			PickInfo.CellMaterialID = CellMaterialID;
		if (CHEXA20MATERIALS_arr) ARRfree(CHEXA20MATERIALS_arr);
		
		HSize = 24;
		Connect_arr=(int*)MasterMesh.TheMesh.CHEXA20.ret_array_ptr(OM_GET_ARRAY_RD);
		//superior face
		HConnect[0]=HConnect[7]=0;
		HConnect[1]=HConnect[2]=1;
		HConnect[3]=HConnect[4]=2;
		HConnect[5]=HConnect[6]=3;
		//lower face
		HConnect[ 8]=HConnect[15]=4;
		HConnect[ 9]=HConnect[10]=5;
		HConnect[11]=HConnect[12]=6;
		HConnect[13]=HConnect[14]=7;

		HConnect[16]=0; HConnect[17]=4;
		HConnect[18]=1; HConnect[19]=5;
		HConnect[20]=2; HConnect[21]=6;
		HConnect[22]=3; HConnect[23]=7;

	}
	
	if ((cell_nnodes==6) && (cell_ndim==3))	{ //CPENTA6

		PickInfo.CellType = "CPENTA6";
		int NCPENTA6 = (int)MasterMesh.TheMesh.CPENTA6_Size;
		if (primi>=NCPENTA6) return(1);
		int* CPENTA6IDS_arr=(int*)MasterMesh.TheMesh.CPENTA6IDS.ret_array_ptr(OM_GET_ARRAY_RD);
			PickInfo.CellID = CPENTA6IDS_arr[primi];
		if (CPENTA6IDS_arr) ARRfree(CPENTA6IDS_arr);
		int* CPENTA6MATERIALS_arr=(int*)MasterMesh.TheMesh.CPENTA6MATERIALS.ret_array_ptr(OM_GET_ARRAY_RD);
			CellMaterialID = CPENTA6MATERIALS_arr[primi];
			PickInfo.CellMaterialID = CellMaterialID;
		if (CPENTA6MATERIALS_arr) ARRfree(CPENTA6MATERIALS_arr);

		HSize = 18;
		Connect_arr=(int*)MasterMesh.TheMesh.CPENTA6.ret_array_ptr(OM_GET_ARRAY_RD);

		//upper face
		HConnect[0]=HConnect[5]=0;
		HConnect[1]=HConnect[2]=1;
		HConnect[3]=HConnect[4]=2;

		//lower face
		HConnect[ 6]=HConnect[11]=3;
		HConnect[ 7]=HConnect[ 8]=4;
		HConnect[ 9]=HConnect[10]=5;

		HConnect[12]=0; HConnect[13]=3;
		HConnect[14]=1; HConnect[15]=4;
		HConnect[16]=2; HConnect[17]=5;
		
	}
	
	if ((cell_nnodes==15) && (cell_ndim==3))	{ //CPENTA15

		PickInfo.CellType = "CPENTA15";
		int NCPENTA15 = (int)MasterMesh.TheMesh.CPENTA15_Size;
		if (primi>=NCPENTA15) return(1);
		int* CPENTA15IDS_arr=(int*)MasterMesh.TheMesh.CPENTA15IDS.ret_array_ptr(OM_GET_ARRAY_RD);
			PickInfo.CellID = CPENTA15IDS_arr[primi];
		if (CPENTA15IDS_arr) ARRfree(CPENTA15IDS_arr);
		int* CPENTA15MATERIALS_arr=(int*)MasterMesh.TheMesh.CPENTA15MATERIALS.ret_array_ptr(OM_GET_ARRAY_RD);
			CellMaterialID = CPENTA15MATERIALS_arr[primi];
			PickInfo.CellMaterialID = CellMaterialID;
		if (CPENTA15MATERIALS_arr) ARRfree(CPENTA15MATERIALS_arr);
		
		HSize = 18;
		Connect_arr=(int*)MasterMesh.TheMesh.CPENTA6.ret_array_ptr(OM_GET_ARRAY_RD);

		//upper face
		HConnect[0]=HConnect[5]=0;
		HConnect[1]=HConnect[2]=1;
		HConnect[3]=HConnect[4]=2;

		//lower face
		HConnect[ 6]=HConnect[11]=3;
		HConnect[ 7]=HConnect[ 8]=4;
		HConnect[ 9]=HConnect[10]=5;

		HConnect[12]=0; HConnect[13]=3;
		HConnect[14]=1; HConnect[15]=4;
		HConnect[16]=2; HConnect[17]=5;
		
	}
	
	if ((cell_nnodes==10) && (cell_ndim==3))	{ //CTETRA10

		PickInfo.CellType = "CTETRA10";
		int NCTETRA10 = (int)MasterMesh.TheMesh.CTETRA10_Size;
		if (primi>=NCTETRA10) return(1);
		int* CTETRA10IDS_arr=(int*)MasterMesh.TheMesh.CTETRA10IDS.ret_array_ptr(OM_GET_ARRAY_RD);
			PickInfo.CellID = CTETRA10IDS_arr[primi];
		if (CTETRA10IDS_arr) ARRfree(CTETRA10IDS_arr);
		int* CTETRA10MATERIALS_arr=(int*)MasterMesh.TheMesh.CTETRA10MATERIALS.ret_array_ptr(OM_GET_ARRAY_RD);
			CellMaterialID = CTETRA10MATERIALS_arr[primi];
			PickInfo.CellMaterialID = CellMaterialID;
		if (CTETRA10MATERIALS_arr) ARRfree(CTETRA10MATERIALS_arr);

		HSize = 12;
		Connect_arr=(int*)MasterMesh.TheMesh.CTETRA10.ret_array_ptr(OM_GET_ARRAY_RD);
		//lower face
		HConnect[0]=HConnect[5]=1;
		HConnect[1]=HConnect[2]=2;
		HConnect[3]=HConnect[4]=3;

		HConnect[ 6]=0; HConnect[ 7]=1;
		HConnect[ 8]=0; HConnect[ 9]=2;
		HConnect[10]=0; HConnect[11]=3;
		
	}
	
	if ((cell_nnodes==4) && (cell_ndim==3))	{ //CTETRA4

		PickInfo.CellType = "CTETRA4";
		int NCTETRA4 = (int)MasterMesh.TheMesh.CTETRA4_Size;
		if (primi>=NCTETRA4) return(1);
		int* CTETRA4IDS_arr=(int*)MasterMesh.TheMesh.CTETRA4IDS.ret_array_ptr(OM_GET_ARRAY_RD);
			PickInfo.CellID = CTETRA4IDS_arr[primi];
		if (CTETRA4IDS_arr) ARRfree(CTETRA4IDS_arr);
		int* CTETRA4MATERIALS_arr=(int*)MasterMesh.TheMesh.CTETRA4MATERIALS.ret_array_ptr(OM_GET_ARRAY_RD);
			CellMaterialID = CTETRA4MATERIALS_arr[primi];
			PickInfo.CellMaterialID = CellMaterialID;
		if (CTETRA4MATERIALS_arr) ARRfree(CTETRA4MATERIALS_arr);

		HSize = 12;
		Connect_arr=(int*)MasterMesh.TheMesh.CTETRA10.ret_array_ptr(OM_GET_ARRAY_RD);
		//lower face
		HConnect[0]=HConnect[5]=1;
		HConnect[1]=HConnect[2]=2;
		HConnect[3]=HConnect[4]=3;

		HConnect[ 6]=0; HConnect[ 7]=1;
		HConnect[ 8]=0; HConnect[ 9]=2;
		HConnect[10]=0; HConnect[11]=3;
		
	}
	
	PickInfo.HSize = HSize;
	if (HSize>0) {
		int II=0;
		int* HConnect_arr=(int*)PickInfo.HConnect.ret_array_ptr(OM_GET_ARRAY_WR);
		int CellsOffset = primi*cell_nnodes;
		for (II=0;II<HSize;II++) {
			HConnect_arr[II]=Connect_arr[CellsOffset+HConnect[II]];
		}

		float P0[3];
		float P1[3];
		float DeltaP[3];
		float GlyphSize=1.;
		int P0_offset = HConnect_arr[0]*3;
		int P1_offset = HConnect_arr[1]*3;
		P0[0]=P0[1]=P0[2]=P1[0]=P1[1]=P1[2]=0.;

		Coords_arr = (float*)MasterMesh.TheMesh.Coords.ret_array_ptr(OM_GET_ARRAY_RD);
		memcpy(P0,Coords_arr+P0_offset,sizeof(float)*3);
		memcpy(P1,Coords_arr+P1_offset,sizeof(float)*3);
		
		for (II=0;II<3;II++) {
			DeltaP[II] = P1[II]-P0[II];
		}
		GlyphSize=0.;
		for (II=0;II<3;II++) {
			GlyphSize += (DeltaP[II]*DeltaP[II]);
		}

		GlyphSize = sqrt(GlyphSize) / 8.;
		PickInfo.GlyphSize = GlyphSize;

		if (Coords_arr) ARRfree(Coords_arr);
		if (HConnect_arr) ARRfree(HConnect_arr);
	}
	if (Connect_arr) ARRfree(Connect_arr);
	//should search CellMaterialID to retrieve the Material Name in the MasterMesh.MAT1_arr & MasterMesh.MAT3_arr

   // return 1 for success
   return(1);
}


int
modules_PickInfo_Mgr::QueryByNodeID(OMevent_mask event_mask, int seq_num)
{

	int NNodes = (int)MasterMesh.TheMesh.Size;
	if (NNodes<=0) return(1);

	char msg[1024];

	int keep_verti =(int)PickInfo.verti;

	int NodeID = (int)PickInfo.NodeID;
	int* IDS_arr=(int*)MasterMesh.TheMesh.IDS.ret_array_ptr(OM_GET_ARRAY_RD);
		int NodeNdx = SimpleBinSearch(IDS_arr,0,NNodes-1,NodeID);
	if (IDS_arr) ARRfree(IDS_arr);
	if (NodeNdx<0) {
			sprintf(msg,"Cannot find NodeID [%d]",NodeID);
			printf("%s\n",msg);
			PickInfo.Error=1;
			PickInfo.ErrorStr=msg;
			PickInfo.verti = keep_verti;
	} else {
		 PickInfo.verti = NodeNdx;
	}
	Update(event_mask,seq_num);

   // return 1 for success
   return(1);
}

*/
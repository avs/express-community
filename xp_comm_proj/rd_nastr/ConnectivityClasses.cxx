#include "ConnectivityClasses.h"


NastranCell::NastranCell(int bb,int a,int b, int *d):Indexable(a){
	MID=b;
	Cardinality=bb;//default
//	Gi=NULL;
//	Gi=new int[Cardinality];
//	Gi=(int*)malloc(Cardinality*sizeof(int));
	memcpy(Gi,d,Cardinality*sizeof(int));
}
NastranCell::~NastranCell(){
//			delete [] Gi;
//	if (Gi) free(Gi);
//	Cardinality=0;
}	

int GetIntID (char* line,int offset) {
	char Buf[10];
	int aID;
	int res;

	if (line[0]=='$') 
		return -2;

	GFstrset(Buf,' ',10);
	memcpy(Buf,line+offset,sizeof(char)*8);
	Buf[8]='\0';
	if (DetectBlank(Buf))
	{
		aID=-1;
	}else {
		res = sscanf(Buf,"%d",&aID);
		if (res!=1) aID=-2;
	}
	return aID;
}

//----------------- TRIAX6 ------------------

Ctriax6 * ParseCtriax6Card(char* line){//multiline card
	
	int aID;
	int aMID;//material id
	int aGi[6];//vertexes ids
	char Buf[10];
	int Gbuf;

	aID = GetIntID(line,8);
	aMID = GetIntID(line,16);

// 2       4         1       3      0      5 
	aGi[2] = GetIntID(line,24);
	aGi[4] = GetIntID(line,32);
	aGi[1] = GetIntID(line,40);
	aGi[3] = GetIntID(line,48);
	aGi[0] = GetIntID(line,56);
	aGi[5] = GetIntID(line,64);

	Ctriax6 *PBufff=new Ctriax6( 6,aID,aMID,aGi);
	return (PBufff);
}

//----------------- HEXA ------------------

int ParseChexaCard_Is8(char* line, FILE* fp){//determine if it's a 8/20 hexa card
	int Gbuf;
	GFstrset(line,' ',1024);
	fgets(line,1024,fp);	//get second line 

	// try to determine if it's a HEXA8 or HEXA20 GetIntID
	Gbuf = GetIntID(line,64);
	if (Gbuf==-1) {
		return 1;
	} else {
		return 0;
	}
	return 1;
}


Chexa * ParseChexaCard(char* line, FILE* fp){//multiline card
	
	int aID;
	int aMID;//material id
	int Is8 = 1;
	int aGiSize=8;
	int aGi8[8];//vertexes ids
	int aGi20[20];//vertexes ids
	int* aGi=aGi8;
	char Buf[10];
	int Gbuf;
	char line1[1024];
	GFstrset(line1,' ',1024);

	aID = GetIntID(line,8);
	aMID = GetIntID(line,16);

	//backup the first line
	strcpy(line1,line);
	GFstrset(line,' ',1024);
	fgets(line,1024,fp);	//get second line 

	// try to determine if it's a HEXA8 or HEXA20
	Gbuf = GetIntID(line,64);
	if (Gbuf==-1) {
		Is8 = 1;
		aGiSize = 8;
		aGi = aGi8;
	} else {
		Is8 = 0;
		aGiSize = 20;
		aGi = aGi20;
	}

// 6 7 4 5 2 3 0 1 ==== 14 15 12 13 18 19 16 17 10 11 8 9  
	//now parse line1	

	aGi[6] = GetIntID(line1,24);
	aGi[7] = GetIntID(line1,32);
	aGi[4] = GetIntID(line1,40);
	aGi[5] = GetIntID(line1,48);
	aGi[2] = GetIntID(line1,56);
	aGi[3] = GetIntID(line1,64);

//now parse second line (that's already in line)
	aGi[0] = GetIntID(line,8);
	aGi[1] = GetIntID(line,16);

	//goes on only if is a 20 nodes CHEXA
	if (!Is8) {
		aGi[14] = GetIntID(line,24);
		aGi[15] = GetIntID(line,32);
		aGi[12] = GetIntID(line,40);
		aGi[13] = GetIntID(line,48);
		aGi[18] = GetIntID(line,56);
		aGi[19] = GetIntID(line,64);

		GFstrset(line,' ',1024);
		fgets(line,1024,fp);	//get third line 
		aGi[16] = GetIntID(line,8);
		aGi[17] = GetIntID(line,16);
		aGi[10] = GetIntID(line,24);
		aGi[11] = GetIntID(line,32);
		aGi[ 8] = GetIntID(line,40);
		aGi[ 9] = GetIntID(line,48);

	}

	Chexa *PBufff=new Chexa( aGiSize,aID,aMID,aGi);
	return (PBufff);
}


//----------------- PENTA ------------------


int ParseCpentaCard_Is6(char* line, FILE* fp){//determine if it's a 6/15 penta card
	int Gbuf;
	GFstrset(line,' ',1024);
	fgets(line,1024,fp);	//get second line 

	// try to determine if it's a PENTA8 or PENTA20 GetIntID
	Gbuf = GetIntID(line,0);
	if (Gbuf==-1) {	//blank means second good line
		GFstrset(line,' ',1024);
		fgets(line,1024,fp);	//get third extra line (both cases get extra line)
		return 0;
	} else {
		return 1;
	}
	return 1;
}


Cpenta * ParseCpentaCard(char* line, FILE* fp){//multiline card
	
	int aID;
	int aMID;//material id
	int Is6 = 1;
	int aGiSize=6;
	int aGi6[6];//vertexes ids
	int aGi15[15];//vertexes ids
	int* aGi=aGi6;
	char Buf[10];
	int Gbuf;
	char line1[1024];
	GFstrset(line1,' ',1024);

	aID = GetIntID(line,8);
	aMID = GetIntID(line,16);

	//backup the first line
	strcpy(line1,line);
	GFstrset(line,' ',1024);
	fgets(line,1024,fp);	//get second line 

	// try to determine if it's a PENTA6 or PENTA15
	Gbuf = GetIntID(line,8);
	if (Gbuf==-1) {	//blank means second good line
		Is6 = 0;
		aGiSize = 15;
		aGi = aGi15;
	} else {
		Is6 = 1;
		aGiSize = 6;
		aGi = aGi6;
	}
// 3 4 5 0 1 2  ==== 9 10 11 12 13 14 6 7 8 
	//now parse line1	

	aGi[3] = GetIntID(line1,24);
	aGi[4] = GetIntID(line1,32);
	aGi[5] = GetIntID(line1,40);
	aGi[0] = GetIntID(line1,48);
	aGi[1] = GetIntID(line1,56);
	aGi[2] = GetIntID(line1,64);
// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14
// 3, 4, 5, 0, 1, 2,12,13,14, 6, 7, 8, 9,10,11

	//goes on only if is a 15 nodes CPENTA
	if (!Is6) {
		aGi[ 9] = GetIntID(line, 8);
		aGi[10] = GetIntID(line,16);
		aGi[11] = GetIntID(line,24);
		aGi[12] = GetIntID(line,32);
		aGi[13] = GetIntID(line,40);
		aGi[14] = GetIntID(line,48);
		aGi[ 6] = GetIntID(line,56);
		aGi[ 7] = GetIntID(line,64);
		GFstrset(line,' ',1024);
		fgets(line,1024,fp);	//get third line 
		aGi[ 8] = GetIntID(line,8);

		GFstrset(line,' ',1024);
		fgets(line,1024,fp);	//get extra line (both 6 and 15 case get an extra line)
	}

	Cpenta *PBufff=new Cpenta( aGiSize,aID,aMID,aGi);
	return (PBufff);
}



//----------------- TETRA ------------------


int ParseCtetraCard_Is4(char* line, FILE* fp){//determine if it's a 4/10 tetra card
	int Gbuf;
	// try to determine if it's a TETRA4 or TETRA10 GetIntID
	Gbuf = GetIntID(line,64);
	if (Gbuf==-1) {	//blank means 4 nodes
		return 1;
	} else {
		GFstrset(line,' ',1024);
		fgets(line,1024,fp);	//get second line
		return 0;
	}
	return 1;
}


Ctetra * ParseCtetraCard(char* line, FILE* fp){//multiline card
	
	int aID;
	int aMID;//material id
	int Is4 = 1;
	int aGiSize=4;
	int aGi4[4];//vertexes ids
	int aGi10[10];//vertexes ids
	int* aGi=aGi4;
	char Buf[10];
	int Gbuf;
	char line1[1024];
	GFstrset(line1,' ',1024);

	aID = GetIntID(line,8);
	aMID = GetIntID(line,16);

	// try to determine if it's a TETRA4 or TETRA10
	Gbuf = GetIntID(line,56);
	if (Gbuf==-1) {	//blank means 4 nodes
		Is4 = 1;
		aGiSize = 4;
		aGi = aGi4;
	} else {
		Is4 = 0;
		aGiSize = 10;
		aGi = aGi10;
	}

// 2 3 1 0  ==== 8 9 7 5 6 4 
	//now parse first line	

	aGi[2] = GetIntID(line,24);
	aGi[3] = GetIntID(line,32);
	aGi[1] = GetIntID(line,40);
	aGi[0] = GetIntID(line,48);



	//goes on only if is a 10 nodes CTETRA
	if (!Is4) {
		aGi[8] = GetIntID(line,56);
		aGi[9] = GetIntID(line,64);

		GFstrset(line,' ',1024);
		fgets(line,1024,fp);	//get second line 

		aGi[7] = GetIntID(line, 8);
		aGi[5] = GetIntID(line,16);
		aGi[6] = GetIntID(line,24);
		aGi[4] = GetIntID(line,32);
	}
	

	Ctetra *PBufff=new Ctetra( aGiSize,aID,aMID,aGi);
	return (PBufff);
}

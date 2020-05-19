#include "GridClasses.h"
#include "DBFparserClasses.h"


GridPoint::GridPoint(int id,int id2, int id3,float v1,float v2,float v3,
						int life,float time_min, float time_max, 
						float temp_min, float temp_max,
						float stress_min, float stress_max	): Indexable(id){
		//ID=id;
		ID2=id2;
		ID3=id3;
		V1=v1;
		V2=v2;
		V3=v3;

		Life = life;
		TimeMin = time_min;
		TimeMax = time_max;
		TempMin = temp_min;
		TempMax = temp_max;
		StressMin = stress_min;
		StressMax = stress_max;
	};



GridPoint * ParseGridCard(char* line, FILE* fp){
	int aID; 
	int aID2;
	int aID3;
	float aV1;
	float aV2;
	float aV3;
	char Buf[20];
	char lineapp[1024];
	char* line2;

	//check GRID*
	int IsGridAsterix = (line[4]=='*');
	int FloatSize = (IsGridAsterix?16:8);

	GFstrset(Buf,' ',20);
	memcpy(Buf,line+8,sizeof(char)*8);//via il nome della scheda
	if (DetectBlank(Buf))
	{
		aID=-1;
	}else {
		sscanf(Buf,"%d",&aID);
	}
	GFstrset(Buf,' ',20);
	memcpy(Buf,line+8+FloatSize,sizeof(char)*FloatSize);
	Buf[FloatSize]='\0';
	if (DetectBlankN(Buf,FloatSize))
	{
		aID2=-1;
	}else {
		sscanf(Buf,"%d",&aID2);
	}
	GFstrset(Buf,' ',20);
	memcpy(Buf,line+8+(2*FloatSize),sizeof(char)*FloatSize);	
	Buf[FloatSize]='\0';
	if (DetectBlankN(Buf,FloatSize))
	{
		aV1=-1;
	}else {
		GFcheckE(Buf);
		sscanf(Buf,"%f",&aV1);
	}
	GFstrset(Buf,' ',20);
	memcpy(Buf,line+8+(3*FloatSize),sizeof(char)*FloatSize);	
	Buf[FloatSize]='\0';
	if (DetectBlankN(Buf,FloatSize))
	{
		aV2=-1;
	}else {
		GFcheckE(Buf);
		sscanf(Buf,"%f",&aV2);
	}

	if (IsGridAsterix) {
		//read lineapp and set-up line2 in order to 
		GFstrset(lineapp,' ',1024);;
		fgets(lineapp,1024,fp);
		line2 = lineapp+8;
	} else {
		line2 = line+8+(4*FloatSize);
	}

	GFstrset(Buf,' ',20);
	memcpy(Buf,line2,sizeof(char)*FloatSize);	
	Buf[FloatSize]='\0';
	if (DetectBlankN(Buf,FloatSize))
	{
		aV3=-1;
	}else {
		GFcheckE(Buf);
		sscanf(Buf,"%f",&aV3);
	}
	GFstrset(Buf,' ',20);
	memcpy(Buf,line2+FloatSize,sizeof(char)*FloatSize);
	Buf[FloatSize]='\0';
	if (DetectBlankN(Buf,FloatSize))
	{
		aID3=-1;
	}else {
		sscanf(Buf,"%d",&aID3);
	}

	int aLife=-1;
	float aTimeMin=-1.0;
	float aTimeMax=-1.0;
	float aTempMin=-1.0;
	float aTempMax=-1.0;
	float aStressMin=-1.0;
	float aStressMax=-1.0;
	GridPoint *PBufff=new GridPoint( aID,aID2,aID3,aV1,aV2,aV3,aLife,aTimeMin,aTimeMax,aTempMin,aTempMax,aStressMin,aStressMax);
	return (PBufff);
};



#include <avs/math.h>

static float IXFORM[16] = {1.0, 0.0, 0.0, 0.0,
			   0.0, 1.0, 0.0, 0.0,
			   0.0, 0.0, 1.0, 0.0,
			   0.0, 0.0, 0.0, 1.0};

CoordSys::CoordSys(int a,int b,int st,float a1,float a2,float a3,float b1,float b2,float b3,float c1,float c2,float c3): Indexable(a){
		//ID=a;
		ID2=b;
		SType=st;
		A[0]=a1;
		A[1]=a2;
		A[2]=a3;
		B[0]=b1;
		B[1]=b2;
		B[2]=b3;
		C[0]=c1;
		C[1]=c2;
		C[2]=c3;

		int II=0;
		for (II=0;II<3;II++) {
			BA[II] = B[II]-A[II];
			CA[II] = C[II]-A[II];

			Znorm[II]=BA[II];
			Xnorm[II]=CA[II];
			Ynorm[II]=0.;
		}
		VEC_UNITIZE(Znorm, 1.0e-32);
		VEC_UNITIZE(Xnorm, 1.0e-32);


		VEC_CROSS(Ynorm, Znorm, Xnorm);
		VEC_UNITIZE(Ynorm, 1.0e-32);

		VEC_CROSS(Xnorm, Ynorm, Znorm); //to be safe recompute Xnorm
		VEC_UNITIZE(Xnorm, 1.0e-32);

		//now we have X,Y,Z normalized

		printf("CoordSys: [%d]\n",ID);
		printf("   X: [%.3f,%.3f,%.3f]\n",Xnorm[0],Xnorm[1],Xnorm[2]);
		printf("   Y: [%.3f,%.3f,%.3f]\n",Ynorm[0],Ynorm[1],Ynorm[2]);
		printf("   Z: [%.3f,%.3f,%.3f]\n",Znorm[0],Znorm[1],Znorm[2]);
		
		memcpy(Mat, IXFORM, 16*sizeof(float));

		memcpy(Mat[0], Xnorm, 3*sizeof(float));
		memcpy(Mat[1], Ynorm, 3*sizeof(float));
		memcpy(Mat[2], Znorm, 3*sizeof(float));

	};

void CoordSys::Serialize(ofstream &fp){
	fp << "<CoordSys>\n";
		fp << "<ID>\n"    << ID << "\n</ID>\n"   ; 
		fp << "<SType>\n"    << SType << "\n</SType>\n"   ; 
		fp << "<A>\n" << A[0] << " " << A[1] << " " << A[2] << "\n</A>\n"   ; 
		fp << "<B>\n" << B[0] << " " << B[1] << " " << B[2] << "\n</B>\n"   ; 
		fp << "<C>\n" << C[0] << " " << C[1] << " " << C[2] << "\n</C>\n"   ; 
		fp << "<Mat>\n" << Mat[0][0] << " "  << Mat[0][1] << " " << Mat[0][2] << " " << Mat[0][3] << " "
						<< Mat[1][0] << " "  << Mat[1][1] << " " << Mat[1][2] << " " << Mat[1][3] << " "
						<< Mat[2][0] << " "  << Mat[2][1] << " " << Mat[2][2] << " " << Mat[2][3] << " "
						<< Mat[3][0] << " "  << Mat[3][1] << " " << Mat[3][2] << " " << Mat[3][3] << "\n</Mat>\n"   ; 
	fp << "</CoordSys>\n";
};
float GetFloatCoord (char* line,int offset) {
	char Buf[20];
	float aX;
	int res;

	GFstrset(Buf,' ',20);
	memcpy(Buf,line+offset,sizeof(char)*8);
	Buf[8]='\0';
	if (DetectBlank(Buf))
	{
		aX=-1.;
	}else {
		GFcheckE(Buf);
		res = sscanf(Buf,"%f",&aX);
		if (res!=1) aX=-2.;
	}
	return aX;
}


CoordSys * ParseCoordSysCard(char* line, FILE* fp){
	int aID; 
	int aID2;
	int aSType;
	float aA1;
	float aA2;
	float aA3;
	float aB1;
	float aB2;
	float aB3;
	float aC1;
	float aC2;
	float aC3;
	char Buf[20];
	int cardtype;

	cardtype=DetectCardType(line);

	switch (cardtype) 	{
		case DCORD2R: 
			aSType = DCORD2R;
			break;
		case DCORD2C:
			aSType = DCORD2C;
			break;
		default:
			aSType = DOTHER;
			break; 
	}

	GFstrset(Buf,' ',20);
	memcpy(Buf,line+8,sizeof(char)*8);
	if (DetectBlank(Buf))
	{
		aID=-1;
	}else {
		sscanf(Buf,"%d",&aID);
	}
	GFstrset(Buf,' ',20);
	memcpy(Buf,line+16,sizeof(char)*8);
	Buf[8]='\0';
	if (DetectBlank(Buf))
	{
		aID2=-1;
	}else {
		sscanf(Buf,"%d",&aID2);
	}

	aA1 = GetFloatCoord (line,24);
	aA2 = GetFloatCoord (line,32);
	aA3 = GetFloatCoord (line,40);
	aB1 = GetFloatCoord (line,48);
	aB2 = GetFloatCoord (line,56);
	aB3 = GetFloatCoord (line,64);

	//read second line
	GFstrset(line,' ',1024);
	fgets(line,1024,fp);

	aC1 = GetFloatCoord (line,8);
	aC2 = GetFloatCoord (line,16);
	aC3 = GetFloatCoord (line,24);


	CoordSys *PBufff=new CoordSys( aID,aID2,aSType,aA1,aA2,aA3,aB1,aB2,aB3,aC1,aC2,aC3);
	return (PBufff);
};

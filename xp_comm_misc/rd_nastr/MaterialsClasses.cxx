
#include "DBFparserClasses.h"
#include "MaterialsClasses.h"

PSolid *ParsePSOLIDCard(char* line) {
	int aID=-1;
	int aMID=-1;

	char Buf[10];

	GFstrset(Buf,' ',10);
	memcpy(Buf,line+8,sizeof(char)*8);//via il nome della scheda
	Buf[8]='\0';
	if (!(DetectBlank(Buf))) sscanf(Buf,"%d",&aID);

	GFstrset(Buf,' ',10);
	memcpy(Buf,line+16,sizeof(char)*8);
	Buf[8]='\0';
	if (!(DetectBlank(Buf))) sscanf(Buf,"%d",&aMID);

	PSolid *PBufff=new PSolid(aID,aMID);
	return (PBufff);
};

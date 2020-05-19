#include "Utils.h"
#include "memory.h"
#define PI 3.1415926535
#include "math.h"
#include "string.h"

//i primi N caratteri sono blank / tab ?
int DetectBlankN(char* line,int Size){
	int Cond=1;
	int II;
	for (II=0;II<Size;II++) {
			Cond = Cond && ((line[II]==32) || (line[II]==10) || (line[II]==13) || (line[II]==0));
	}
	return (Cond);
}

//i primi 8 caratteri sono blank?
int DetectBlank(char* line){
	return DetectBlankN(line,8);
	/*
	if ((line[0]==' ')&&(line[1]==' ')&&(line[2]==' ')&&(line[3]==' ')
		&&(line[4]==' ')&&(line[5]==' ')&&(line[6]==' ')&&(line[7]==' '))
	return (1);
	else
	return (0); */
}

//siamo in fondo?
int DetectEndt(char* line){
	if ((line[0]==' ')&&(line[1]=='E')&&(line[2]=='N')&&(line[3]=='D')
		&&(line[4]=='T'))
	return (1);
	else
	return (0);
}

//la linea comincia con una cifra?
int DetectDigit(char* line){
	if ((line[0]=='0')||(line[0]=='1')||(line[0]=='2')||(line[0]=='3')
		||(line[0]=='4')||(line[0]=='5')||(line[0]=='6')||(line[0]=='7')||(line[0]=='8')||(line[0]=='9'))
	return (1);
	else
	return (0);
}

//inizializza una stringa
int GFstrset(char* line,char ch, int size) {
	memset(line,ch,size);
	line[size-1]=0;
	return 1;
};

float VectorModule(float F,float X,float Y,float Z) {
	return (F* sqrt(X*X+Y*Y+Z*Z));
};


void GFcheckEch(char* buf,char ch) {
	//cerco +/-
	//se il char a sx e' un blank cerco ancora
				//se trovo
	//altrimenti sostituisco il +/- con E+/-
	char* curr = NULL;
	char ccc=' ';
	char app[1024];
	curr = strchr(buf,ch);
	while (curr) {
		if (curr==buf) {
			ccc=' ';
		} else {
			ccc = (curr-1)[0];
		}
		if ((ccc!=' ')&&(ccc!='E')&&(ccc!='e')  ) {
			memcpy(app,curr,3); //copy 3 chars, the sign + 2 digits
			memcpy(curr+1,app,3);
			curr[0]='E';
			curr++;		
		}
		curr++;
		curr = strchr(curr,ch);
	}		
}

void GFcheckE(char* buf) {
	GFcheckEch(buf,'+');
	GFcheckEch(buf,'-');
}



int SimpleBinSearch(int* theArr,int lo,int hi,int &searchID){
	if (theArr==NULL) return(-1);
	if ((hi-lo)==1)
	{
			if (searchID==theArr[lo])return (lo);
			if (searchID==theArr[hi])return (hi);
			return (-1);
	} else 
		if (lo!=hi)
		{
			int mid=lo+(hi-lo)/2;
			if (searchID==theArr[mid])return (mid);
			if (searchID>theArr[mid]) return (SimpleBinSearch(theArr,mid,hi,searchID));
				else return (SimpleBinSearch(theArr,lo,mid,searchID));
		} else {
			if (searchID==theArr[lo]) return (lo);
				else return (-1);
		}
}; 

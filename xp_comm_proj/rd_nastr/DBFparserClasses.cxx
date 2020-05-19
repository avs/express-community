#include "DBFparserClasses.h"
 
int DetectCardType(char* line){//to be extended to encompass EVERY managed card
	if (line[0]=='$') return (DCOMMENT);
	if ((line[0]=='G')&&(line[1]=='R')&&(line[2]=='I')&&(line[3]=='D')) return (DGRID);
	if ((line[0]=='C')&&(line[1]=='T')&&(line[2]=='R')&&(line[3]=='I')
		&&(line[4]=='A')&&(line[5]=='X')&&(line[6]=='6')) return (DCTRIAX6);
	if ((line[0]=='C')&&(line[1]=='T')&&(line[2]=='E')&&(line[3]=='T')
		&&(line[4]=='R')&&(line[5]=='A')) return (DCTETRA);
	if ((line[0]=='C')&&(line[1]=='H')&&(line[2]=='E')&&(line[3]=='X')
		&&(line[4]=='A')) return (DCHEXA);
	if ((line[0]=='C')&&(line[1]=='P')&&(line[2]=='E')&&(line[3]=='N')
		&&(line[4]=='T')&&(line[5]=='A')) return (DCPENTA);
	if ((line[0]=='C')&&(line[1]=='O')&&(line[2]=='R')
		&&(line[3]=='D')&&(line[4]=='2')&&(line[5]=='R')) return (DCORD2R);
	if ((line[0]=='C')&&(line[1]=='O')&&(line[2]=='R')
		&&(line[3]=='D')&&(line[4]=='2')&&(line[5]=='C')) return (DCORD2C);
	if ((line[0]=='P')&&(line[1]=='S')&&(line[2]=='O')&&(line[3]=='L')
		&&(line[4]=='I')&&(line[5]=='D')) return (DPSOLID);

	if ((line[0]=='E')&&(line[1]=='N')&&(line[2]=='D')&&(line[3]=='D')
		&&(line[4]=='A')&&(line[5]=='T')&&(line[6]=='A')) return (DENDDATA);
	return (DOTHER);
}




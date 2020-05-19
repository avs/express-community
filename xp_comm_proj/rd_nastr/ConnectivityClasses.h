#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utils.h"

#ifndef NASTRANCELL_PARAPARA
#define NASTRANCELL_PARAPARA

//classe base per connettivita' in stile nastran
class NastranCell: public Indexable
{
	public:
		//int EID;//element id (gia' presente in Indexable)
		int MID;//material id
		int Cardinality;
		int Gi[100];//vertexes ids
		NastranCell():Indexable(-1){
			MID=-1;
		};
		NastranCell(int bb,int a,int b, int *d);
		~NastranCell();	
/*		operator!=(const NastranCell& rv) const{
			return(ID!=rv.ID);
		}		
*/
};


//istanza di NastranCell, specializzata per le celle di tipo CTRIAX6
class Ctriax6: public NastranCell
{
	public:
		//int ID;//should be NODE ID
		Ctriax6():NastranCell(){
		};
		Ctriax6(int bb,int a,int b, int *d): NastranCell(bb,a,b,d){
		};
};

//parser di schede CTRIAX6
Ctriax6 * ParseCtriax6Card(char* line);//multiline card, but we ignore the 2nd one


//istanza di NastranCell, specializzata per le celle di tipo CHEXA (8/20)
class Chexa: public NastranCell
{
	public:
		//int ID;//should be NODE ID
		Chexa():NastranCell(){
		};
		Chexa(int bb,int a,int b, int *d): NastranCell(bb,a,b,d){
		};
};

//parser di schede CHEXA
Chexa * ParseChexaCard(char* line,FILE* fp);//multiline card
int ParseChexaCard_Is8(char* line,FILE* fp);//determine if it's a 8/20 hexa card


//istanza di NastranCell, specializzata per le celle di tipo CPENTA (6/15)
class Cpenta: public NastranCell
{
	public:
		//int ID;//should be NODE ID
		Cpenta():NastranCell(){
		};
		Cpenta(int bb,int a,int b, int *d): NastranCell(bb,a,b,d){
		};
};

//parser di schede CPENTA
Cpenta * ParseCpentaCard(char* line,FILE* fp);//multiline card
int ParseCpentaCard_Is6(char* line,FILE* fp);//determine if it's a 6/15 penta card

//istanza di NastranCell, specializzata per le celle di tipo CTETRA (4/10)
class Ctetra: public NastranCell
{
	public:
		//int ID;//should be NODE ID
		Ctetra():NastranCell(){
		};
		Ctetra(int bb,int a,int b, int *d): NastranCell(bb,a,b,d){
		};
};

//parser di schede CTETRA
Ctetra * ParseCtetraCard(char* line,FILE* fp);//multiline card
int ParseCtetraCard_Is4(char* line,FILE* fp);//determine if it's a 4/10 tetra card

#endif

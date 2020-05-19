#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utils.h"

#include <string.h>

//classi per i materiali 2D

class PSolid: public Indexable
{
	public:
		//int ID;//should be NODE ID
		int MID;
		PSolid():Indexable(-1){
		};
		PSolid(int a,int aMID):Indexable(a) {
			MID=aMID;
		};
		operator!=(const PSolid& rv) const{
			return(ID!=rv.ID);
		}
};
PSolid *ParsePSOLIDCard(char* line);


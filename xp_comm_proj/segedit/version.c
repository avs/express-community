#include "avs/version.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE* outfile;
	outfile = fopen("xpversion.v","wb");
	if (!outfile) return 1;
	printf ("version: %i\n",XP_LIB_MAJOR_VERSION);
	if (XP_LIB_MAJOR_VERSION<63)
		fprintf (outfile,"#define XP_VERSION_PRE_63\n");
	fprintf (outfile,"#define XP_VERSION_%i\n",XP_LIB_MAJOR_VERSION);
	fclose(outfile);
	return 0;
}

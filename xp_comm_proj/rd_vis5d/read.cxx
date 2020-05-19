
#include <stdio.h>
#include <string.h>
#include "v5d.h"

void main(int ac, char **av)
{
	char line[256];

	if(ac > 1)
	{
		strcpy(line, av[1]);
	}
	else
	{
		printf("filename> "); fflush(stdout);
		gets(line);
	}
	v5dstruct *v5d = v5dOpenFile(line, NULL);

	if(v5d == NULL) 
	{
		printf("cannot open %s\n", line);
		return;
	}

	v5dPrintStruct(v5d);

	v5dFreeStruct(v5d);
}

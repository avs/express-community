
#include "stm3.h"
#include <ctype.h>

int atom_to_z(char *name, int forced_to)
{
	int Z;
	char n[2];
	char *p;
	
	// convert the protein data CA CB etc. to C. Same for similar names
	if(name[0] == 'H' && (name[1] == 'E' || name[1] == 'G'  || name[1] == 'O' || name[1] == 'F')) {n[0] = 'H'; n[1] = ' ';}
	else if(name[0] == 'C' && (name[1] == 'A' || name[1] == 'B' || name[1] == 'D' || name[1] == 'E' || name[1] == 'F' || name[1] == 'M')) {n[0] = 'C'; n[1] = ' ';}
	else if(name[0] == 'N' && (name[1] == 'A' || name[1] == 'B' || name[1] == 'E' || name[1] == 'D')) {n[0] = 'N'; n[1] = ' ';}
	else
	{
		n[0] = toupper(name[0]);
		if(isdigit(name[1])) n[1] = ' ';
		else                 n[1] = tolower(name[1]);
	}
	
#if 0
	// lookup the atom number
	for(Z=1; p=atom_properties[Z].atom_name; ++Z) if(n[0] == p[0] && n[1] == p[1]) break;
	
	// if not found try to check only the first letter
	if(!atom_properties[Z].atom_name)
	{
		for(Z=1; p=atom_properties[Z].atom_name; ++Z) if(n[0] == p[0] && p[1] == ' ') break;
		if(!atom_properties[Z].atom_name)
		{
			Z = forced_to; // atom name not found. Forced to H
		}
	}
#else
	int Zblank = 0;
	for(Z=1; (p=atom_properties[Z].atom_name); ++Z)
	{
		if(n[0] == p[0])
		{
			if(n[1] == p[1]) break;
			if(p[1] == ' ' && !Zblank) Zblank = Z;
		}
	}

	// if not found try to check only the first letter
	// else atom name not found so forced to H
	if(!p)
	{
		Z = (Zblank) ? Zblank : forced_to;
	}
#endif

	return Z;
}


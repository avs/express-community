
#include <string.h>

static char *aminnames[] = { "THR", "CYS", "PRO", "SER",
                             "ILE", "VAL", "ALA", "ARG",
                             "ASN", "PHE", "LEU", "GLY",
                             "TYR", "ASP", "MET", "GLN",
                             "GLU", "HOH", "TRP", "LYS",
                             "HIS", "SO4", "BEN", "AGLN",
                             "BGLN","AGLU","BGLU","AVAL",
                             "BVAL","AARG","BARG","ALYS",
                             "BLYS", "ZN1","ZN2", "MPD",
                             0};

static float amincolors[][3] = {{1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,0.0,1.0}, {1.0,0.0,0.0},
								{1.0,1.0,0.5}, {1.0,1.0,0.5}, {1.0,1.0,0.5}, {0.0,1.0,1.0},
								{1.0,0.5,.75}, {0.5,1.0,0.5}, {1.0,1.0,0.5}, {0.0,0.0,1.0},
								{0.5,1.0,0.5}, {1.0,0.5,.75}, {1.0,1.0,0.0}, {1.0,0.5,.75},
								{1.0,0.5,.75}, {1.0,1.0,1.0}, {0.5,1.0,0.5}, {0.0,1.0,1.0},
								{0.0,1.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,1.0},
								{1.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,1.0},
								{1.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,1.0},
								{1.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,1.0},
                                {1.0,1.0,1.0}};

void res_to_color(char *residue, float *colors)
{
	if(residue[0] == '\0')
	{
		colors[0] = 0.7F;
		colors[1] = 0.8F;
		colors[2] = 0.7F;
		return;
	}
	
	char *p;
	int i;
	for(i=0; (p=aminnames[i]); ++i)
	{
		int len = strlen(p);
		if(!strncmp(p, residue, len)) break;
	}
	colors[0] = amincolors[i][0];
	colors[1] = amincolors[i][1];
	colors[2] = amincolors[i][2];
}

int res_number(char *residue)
{
	if(residue[0] == '\0') return -1;
	char *p;
	int i;
	for(i=0; (p=aminnames[i]); ++i)
	{
		int len = strlen(p);
		if(!strncmp(p, residue, len)) return i;
	}
	return -1;
}

char *res_name(int res_idx)
{
	if(res_idx < 0 || res_idx > sizeof(aminnames)/sizeof(char *)-2) return "UNK";
	return aminnames[res_idx];
}

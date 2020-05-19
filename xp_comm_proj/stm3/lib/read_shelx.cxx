/*
TITL *-Na0.98K0.02NbO3-[P2MM]-Athee M, Hewat A W   34 (1978) P. 309-317        
CELL 1.54180 7.9004 7.8362 7.9376 90.0 90.0 90.0
SYMM P 2 M M (0) - Replace with explicit SYMMetry cards.
UNIT      8   60   24    4
SFAC Na K Na K Na K Na K Na K Na K Na K Na K Nb O
Na1  1   0.99010 0.00000 0.00000 10.2450
K1   2   0.99010 0.00000 0.00000 10.0050
Na2  3   0.00990 0.50000 0.50000 10.2450
K2   4   0.00990 0.50000 0.50000 10.0050
Na3  5   0.49010 0.00000 0.00000 10.2450
K3   6   0.49010 0.00000 0.00000 10.0050
Na4  7   0.50990 0.50000 0.50000 10.2450
K4   8   0.50990 0.50000 0.50000 10.0050
Na5  9   0.49010 0.00000 0.50000 10.2450
K5   10   0.49010 0.00000 0.50000 10.0050
Na6  11   0.50990 0.50000 0.00000 10.2450
K6   12   0.50990 0.50000 0.00000 10.0050
Na7  13   0.99010 0.00000 0.50000 10.2450
K7   14   0.99010 0.00000 0.50000 10.0050
Na8  15   0.00990 0.50000 0.00000 10.2450
K8   16   0.00990 0.50000 0.00000 10.0050
Nb1  17   0.26590 0.24660 0.25290 11.0000
Nb2  17   0.76590 0.24660 0.75290 11.0000
O1   18   0.21850 0.00000 0.23070 10.5000
O2   18   0.71850 0.00000 0.73070 10.5000
O3   18   0.22550 0.23070 0.50000 10.5000
O4   18   0.72550 0.23070 0.00000 10.5000
O5   18   0.50000 0.21850 0.27450 11.0000
O6   18   0.00000 0.18500 0.77450 11.0000
END

*/

#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include "stm3.h"
#include "../base/mol_type.h"
#include <math.h>
#include <string.h>
#ifndef M_PI
#define M_PI 3.141592653589793238462643383279F
#endif

int read_shelx(FILE *fp, int read_hydrogens, vector<Atom> &atoms, vector<Bond> &bonds, UnitCell &unit_cell, char *space_group)
{
	char linebuf[82];
	int k; /* counter of atoms */
	float dx;
	float dy;
	float dz;
	char dum1[8];
	int seq;
	
	k=0;
	while(fgets(linebuf, sizeof linebuf, fp) != NULL && strncmp("END", linebuf, 3))
	{
		// ignore keywords
		if(!strncmp("TITL", linebuf, 4) ||
		   !strncmp("UNIT", linebuf, 4) ||
		   !strncmp("SFAC", linebuf, 4)) continue;

		// symmetry
		if(!strncmp("SYMM", linebuf, 4))
		{
			if(!space_group) continue;
			char *p = strchr(linebuf+4, '(');
			if(p) *p = '\0';
			for(p=linebuf+4; *p && *p == ' '; ++p);
			strcpy(space_group, p);
			continue;
		}

		// unit cell
		if(!strncmp("CELL", linebuf, 4))
		{
			float a, b, c, lambda;
			float alpha, beta, gamma;
			
			sscanf(&linebuf[4], "%f %f %f %f %f %f %f",
								&lambda,
								&a, &b, &c,
								&alpha, &beta, &gamma);
								
			unit_cell.angles[0] = alpha;
			unit_cell.angles[1] = beta;
			unit_cell.angles[2] = gamma;
			
			alpha *= M_PI/180.0F;
			beta  *= M_PI/180.0F;
			gamma *= M_PI/180.0F;
			
			unit_cell.side_vectors[0][0] = a;
			unit_cell.side_vectors[0][1] = 0.0F;
			unit_cell.side_vectors[0][2] = 0.0F;
			
			unit_cell.side_vectors[1][0] = b * cos(gamma);
			unit_cell.side_vectors[1][1] = b * sin(gamma);
			unit_cell.side_vectors[1][2] = 0.0F;
			
			double V = a*b*c*sqrt(1. - cos(alpha)*cos(alpha) - cos(beta)*cos(beta) - cos(gamma)*cos(gamma) + 2.*(cos(alpha)*cos(beta)*cos(gamma)));
			unit_cell.side_vectors[2][0] = c * cos(beta);
			unit_cell.side_vectors[2][1] = c * (cos(alpha) - cos(beta)*cos(gamma)) / sin(gamma);
			unit_cell.side_vectors[2][2] = V/(a*b*sin(gamma));
			
			unit_cell.origin[0] = 0.0f;
			unit_cell.origin[1] = 0.0f;
			unit_cell.origin[2] = 0.0f;
		}		
		else
		{
			//atomname sfac x y z sof [11] U [0.05] or U11 U22 U33 U23 U13 U12
			if(sscanf(linebuf,"%4s %d %f %f %f", dum1, &seq, &dx, &dy, &dz) < 5) continue;

			if(read_hydrogens == 1 || !(dum1[0]=='H' || dum1[0]=='h'))
			{
				Atom a;

				//for(j=0, i=strspn(dum1, " 0123456789"); i < 5; i++) a.name[j++] = dum1[i];
				strcpy(a.name, dum1);
				if((a.atom_z = atom_to_z(a.name, 0)) == 0) continue;
				
				a.x = dx*unit_cell.side_vectors[0][0]+dy*unit_cell.side_vectors[1][0]+dz*unit_cell.side_vectors[2][0];
				a.y = dx*unit_cell.side_vectors[0][1]+dy*unit_cell.side_vectors[1][1]+dz*unit_cell.side_vectors[2][1];
				a.z = dx*unit_cell.side_vectors[0][2]+dy*unit_cell.side_vectors[1][2]+dz*unit_cell.side_vectors[2][2];

				// no residue name provided
				a.res_id = 0;
				a.res_name[0] = '\0';
				
 				a.c = 0.0;

				atoms.push_back(a);
				
        		k++;
 			}
 		}
	}

	return k;
}

//
//        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
//        Taken from coord_to_geom.c to create a reusable library.
//        The original code was written by Lee Bartolotti and Ken Flurchick.
//
//	Redone by Mario Valle - Swiss Center for Scientific Computing on April 3rd, 2003
//

/*
  Option: pdb
        Read a Brookhaven file putting the coords into apos and the char
        string names into atype. Return number of atoms.  M Pique
        Stop reading on End-of-file, or 'END' line, or line with just a ".".
        Here is some sample input:

ATOM     19  HN  THR     2      15.386  10.901   4.600
ATOM     20  HOG THR     2      14.161   9.481   4.420
ATOM     21  N   CYS     3      13.507  11.238   8.398
ATOM     22  CA  CYS     3      13.659  10.715   9.763
ATOM     23  C   CYS     3      12.265  10.443  10.307
HETATM   99  FE  XXX     4      33.265  10.443  10.307
ATOM         OT  OME    52     -16.009  30.871 -13.037                -0.543
HETATM       CU+2CU    152     -20.577   2.601 -14.587                 2.000
HETATM       ZN+2ZN    152     -17.911   1.372 -19.974                 2.000
END

These charge values
are non-standard
and are not genuine
PDB. They are optional.
*/

#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include "stm3.h"
#include "../base/mol_type.h"
#include <map>
#include <math.h>
#include <ctype.h>
#ifndef M_PI
#define M_PI 3.141592653589793238462643383279F
#endif

/*++*******************************************************
 *  Inverse 3x3 matrix, returns determinant of input matrix
 *  or hard 0.0  if singular.
 ******************************************************--*/
#ifndef SMALL
#define SMALL (1e-30)
#endif
#ifndef  _ABS
#define _ABS(A) ((A)>0?(A):-(A))
#endif
static float MATmat3_inverse(
    float res[3][3], 
    float src[3][3])
{
	float det;
	float a00, a01, a02, a10, a11, a12, a20, a21, a22;

	a00 = src[0][0]; a01 = src[0][1]; a02 = src[0][2];
	a10 = src[1][0]; a11 = src[1][1]; a12 = src[1][2];
	a20 = src[2][0]; a21 = src[2][1]; a22 = src[2][2];

	det =  -a02 * a11 * a20 + a01 * a12 * a20 + a02 * a10 * a21 - a00 * a12 * a21 -
		a01 * a10 * a22 + a00 * a11 * a22;
	if (_ABS(det) < SMALL)
		return(0.0);
	res[0][0] =  (a11 * a22 - a12 * a21)/det;
	res[0][1] =  (a02 * a21 - a01 * a22)/det;
	res[0][2] =  (a01 * a12 - a02 * a11)/det;

	res[1][0] =  (a12 * a20 - a10 * a22)/det;
	res[1][1] =  (a00 * a22 - a02 * a20)/det;
	res[1][2] =  (a02 * a10 - a00 * a12)/det;

	res[2][0] =  (a10 * a21 - a11 * a20)/det;
	res[2][1] =  (a01 * a20 - a00 * a21)/det;
	res[2][2] =  (a00 * a11 - a01 * a10)/det;
	return(det);
}

// check if the given bond is valid and is not repeated
static void check_and_add_bond(int from, int to, map<int, int> &sn_map, vector<Bond> &bonds, int type)
{
	if(sn_map.find(to) != sn_map.end())
	{
		to = sn_map[to];

		Bond b;

		if(from > to)
		{
			b.from = to;
			b.to   = from;
		}
		else
		{
			b.from = from;
			b.to   = to;
		}
		b.type = type;

		vector<Bond>::iterator ib;
		for(ib = bonds.begin(); ib != bonds.end(); ib++)
		{
			if(ib->from == b.from && ib->to == b.to) return;
		}
		bonds.push_back(b);
	}
}

int read_pdb(FILE *fp, int read_hydrogens, vector<Atom> &atoms, vector<Bond> &bonds, UnitCell &unit_cell, char *space_group)
{
	char linebuf[82];
	register int k, i, j; /* counter of atoms */
	float tcharge;
	float dx;
	float dy;
	float dz;
	char dum1[8], dum2[8];
	int seq, sn;
	map<int, int> sn_map;
	float mat[3][3];
	
	k=0;
	while(fgets(linebuf, sizeof linebuf, fp) != NULL && strncmp("END", linebuf, 3))
	{
		if(!strncmp("ATOM", linebuf, 4) || !strncmp("HETATM", linebuf, 6))
		{
			sscanf(&linebuf[6], "%5d", &sn);
			sscanf(&linebuf[12],"%4s", dum1);
			sscanf(&linebuf[17],"%3s", dum2);
			sscanf(&linebuf[22],"%4d", &seq);
			sscanf(&linebuf[30],"%8f%8f%8f", &dx, &dy, &dz);

			if(read_hydrogens == 1 || !(dum1[0]=='H' || dum1[0]=='h'))
			{
				Atom a;

				a.x = dx;
				a.y = dy;
				a.z = dz;

				for(j=0, i=strspn(dum1, " 0123456789"); i < 5; i++)
				{
					a.name[j++] = dum1[i];
				}

 				sprintf(a.res_name, "%s %d", dum2, seq);
				a.res_id = seq;
				
 				a.c = (1 == sscanf(&linebuf[60], "%f", &tcharge)) ? tcharge : 0.0;

				a.atom_z = atom_to_z(a.name);
				
				atoms.push_back(a);
				
				// create mapping from sn to position in list (k)
				sn_map.insert(map<int, int>::value_type(sn, k));
				
        		k++;
 			}
 		}
		else if(!strncmp("CONECT", linebuf, 6))
		{
			int from, to1, to2, to3, to4, hb1, hb2;
			int ne0, ne1, ne2, ne3, ne4, ne5, ne6;
			char tmp[6];
			tmp[5] = '\0';
			
			int len = strlen(linebuf);
			
			if(len > 11)
			{
				strncpy(tmp, &linebuf[6], 5);
				ne0 = sscanf(tmp,  "%d", &from);
			}
			else ne0 = 0;
			if(len > 16)
			{
				strncpy(tmp, &linebuf[11], 5);
				ne1 = sscanf(tmp,  "%d", &to1);
			}
			else ne1 = 0;
			if(len > 21)
			{
				strncpy(tmp, &linebuf[16], 5);
				ne2 = sscanf(tmp,  "%d", &to2);
			}
			else ne2 = 0;
			if(len > 26)
			{
				strncpy(tmp, &linebuf[21], 5);
				ne3 = sscanf(tmp,  "%d", &to3);
			}
			else ne3 = 0;
			if(len > 31)
			{
				strncpy(tmp, &linebuf[26], 5);
				ne4 = sscanf(tmp,  "%d", &to4);
			}
			else ne4 = 0;
			if(len > 36)
			{
				strncpy(tmp, &linebuf[31], 5);
				ne5 = sscanf(tmp,  "%d", &hb1);
			}
			else ne5 = 0;
			if(len > 41)
			{
				strncpy(tmp, &linebuf[36], 5);
				ne6 = sscanf(tmp,  "%d", &hb2);
			}
			else ne6 = 0;
		
			if(ne0 > 0)
			{
				// normalize the from value
   				if(sn_map.find(from) != sn_map.end()) from = sn_map[from];
				else ne1 = ne2 = ne3 = ne4 = ne5 = ne6 = 0;
			
				// add the bonds
				if(ne1 > 0)
				{
					check_and_add_bond(from, to1, sn_map, bonds, SINGLE_BOND);
				}
				if(ne2 > 0)
				{
					check_and_add_bond(from, to2, sn_map, bonds, SINGLE_BOND);
				}
				if(ne3 > 0)
				{
					check_and_add_bond(from, to3, sn_map, bonds, SINGLE_BOND);
				}
				if(ne4 > 0)
				{
					check_and_add_bond(from, to4, sn_map, bonds, SINGLE_BOND);
				}
				if(ne5 > 0)
				{
					check_and_add_bond(from, hb1, sn_map, bonds, H_BOND);
				}
				if(ne6 > 0)
				{
					check_and_add_bond(from, hb2, sn_map, bonds, H_BOND);
				}
			}
		}
		else if(!strncmp("CRYST1", linebuf, 6))
		{
			float a, b, c;
			float alpha, beta, gamma;
			
			sscanf(&linebuf[6], "%9f%9f%9f%9f%9f%9f",
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
			
			if(!space_group) continue;
			strncpy(space_group, linebuf+55, 11);
			space_group[11] = '\0';
			
			// Trim trailing spaces
			int len = strlen(space_group);
			for(i=len-1; i >= 0; --i) if(!isspace(space_group[i])) break;
			space_group[i+1] = '\0';
		}
		else if(!strncmp("SCALE", linebuf, 5))
		{
			float a, b, c, d;
			
			sscanf(&linebuf[6], "%10f%10f%10f%10f", &a, &b, &c, &d);
			switch(linebuf[5])
			{
			case '1':
				unit_cell.origin[0] = d;
				mat[0][0] = a;
				mat[0][1] = b;
				mat[0][2] = c;
				break;
			case '2':
				unit_cell.origin[1] = d;
				mat[1][0] = a;
				mat[1][1] = b;
				mat[1][2] = c;
				break;
			case '3':
				unit_cell.origin[2] = d;
				mat[2][0] = a;
				mat[2][1] = b;
				mat[2][2] = c;
				break;
			};
		}
	}

	if(unit_cell.origin[0] != 0.0F || unit_cell.origin[1] != 0.0F || unit_cell.origin[2] != 0.0F)
	{
		float res[3][3], origin[3];
		MATmat3_inverse(res, mat);
		origin[0] = unit_cell.origin[0];		
		origin[1] = unit_cell.origin[1];		
		origin[2] = unit_cell.origin[2];		
		unit_cell.origin[0] = -(res[0][0]*origin[0]+res[0][1]*origin[1]+res[0][2]*origin[2]);
		unit_cell.origin[1] = -(res[1][0]*origin[0]+res[1][1]*origin[1]+res[1][2]*origin[2]);
		unit_cell.origin[2] = -(res[2][0]*origin[0]+res[2][1]*origin[1]+res[2][2]*origin[2]);
	}
	
	return k;
}

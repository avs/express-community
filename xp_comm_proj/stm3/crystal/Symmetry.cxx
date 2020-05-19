
#include "Symmetry_gen.h"
#include "../utils/copy_mol.h"
#include <stdio.h>
#include <string.h>
#include <avs/mat.h>

extern "C" {
#include "../extern/sginfo/sginfo.h"
}

// uncomment to display info on the generated symmetry matrices
//#define DEBUG

static void apply_symmetries(STM3_BASE_MolecularDataType& in, STM3_BASE_MolecularDataType& out, T_SgInfo& SgInfo, float R[3][3], float Rinv[3][3], float T[3]);
static void fract_conversion(float *basis, float *origin, float R[3][3], float Rinv[3][3], float T[3]);

int
STM3_CRYSTAL_Symmetry::Symmetry(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// out (MolecularDataType write)

	// if no unit cell or no space group symbol or space group symbol P1, simply copy the molecule
	if((char *)in.symmetry.space_group == NULL || strlen((char *)in.symmetry.space_group) < 2 ||
	   !strcmp((char *)in.symmetry.space_group, "P 1") || !strcmp((char *)in.symmetry.space_group, "P1"))
	{
		copy_full_molecule(in, out, true);
	   	return 1;
	}

	// get the unit cell basis vectors
	float *basis = (float *)in.unit_cell.side_vectors.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!basis)
	{
		copy_full_molecule(in, out, true);
	   	return 1;
	}

	// if no unit cell data available, stop here
	if(basis[0] == 0.0F && basis[1] == 0.0F && basis[2] == 0.0F)
	{
		copy_full_molecule(in, out, true);
		ARRfree(basis);
		return 1;
	}

	// access the SG library
	const T_TabSgName *tsgn;
    tsgn = FindTabSgNameEntry((char *)in.symmetry.space_group, 'A');
    if(tsgn == NULL)
	{
		// no matching table entry
		printf("\nNo table entry for space group <%s>\n", (char *)in.symmetry.space_group);
		copy_full_molecule(in, out, true);
		
		return 1;
	}

	T_SgInfo  SgInfo;
	
	// Allocate memory for the list of Seitz matrices and
	// a supporting list which holds the characteristics of
	// the rotation parts of the Seitz matrices
	SgInfo.MaxList = 192; // absolute maximum number of symops
	SgInfo.ListSeitzMx = (T_RTMx *)malloc(SgInfo.MaxList * sizeof(*SgInfo.ListSeitzMx));
	if(SgInfo.ListSeitzMx == NULL)
	{
		printf("\nNot enough core for ListSeitzMx\n");
		copy_full_molecule(in, out, true);
		return 1;
	}

	SgInfo.ListRotMxInfo = (T_RotMxInfo *)malloc(SgInfo.MaxList * sizeof(*SgInfo.ListRotMxInfo));
	if(SgInfo.ListRotMxInfo == NULL)
	{
		printf("\nNot enough core for ListRotMxInfo\n");
		copy_full_molecule(in, out, true);
		return 1;
	}

	// Initialize the SgInfo structure
	InitSgInfo(&SgInfo);
	SgInfo.TabSgName = tsgn;

	// Translate the Hall symbol and generate the whole group
	ParseHallSymbol(tsgn->HallSymbol, &SgInfo);
	if(SgError != NULL)
	{
		printf("\n%s\n", SgError);
		copy_full_molecule(in, out, true);
		return 1;
	}

	// Do some book-keeping and derive crystal system, point group,
	// and - if not already set - find the entry in the internal
	// table of space group symbols
	if(CompleteSgInfo(&SgInfo) != 0)
	{
		printf("\n%s\n", SgError);
		copy_full_molecule(in, out, true);
		return 1;
	}
	
#ifdef DEBUG
	// For debugging print out the entire structure
	ListSgInfo(&SgInfo, 1, 0, stdout);
	if(SgError) printf("\n%s - continuing\n", SgError);
	fflush(stdout);
#endif

	// no symmetries
	if(SgInfo.nList <= 1)
	{
		copy_full_molecule(in, out, true);
		free(SgInfo.ListSeitzMx);
		free(SgInfo.ListRotMxInfo);
		ARRfree(basis);
		return 1;
	}

	// Prepare conversion to fractional crystallographyc coordinates
	float R[3][3], Rinv[3][3], T[3];
	float *origin = (float *)in.unit_cell.origin.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!origin)
	{
		copy_full_molecule(in, out, true);
		free(SgInfo.ListSeitzMx);
		free(SgInfo.ListRotMxInfo);
		ARRfree(basis);
		return 1;
	}
	fract_conversion(basis, origin, R, Rinv, T);
	ARRfree(origin);
	ARRfree(basis);
	
	// At last replicate the atoms using symmetries found
	apply_symmetries(in, out, SgInfo, R, Rinv, T);

	// Release everything
	free(SgInfo.ListSeitzMx);
	free(SgInfo.ListRotMxInfo);
	return 1;
}

static void fract_conversion(float *basis, float *origin, float R[3][3], float Rinv[3][3], float T[3])
{
	Rinv[0][0] = basis[0]; Rinv[1][0] = basis[1]; Rinv[2][0] = basis[2];
	Rinv[0][1] = basis[3]; Rinv[1][1] = basis[4]; Rinv[2][1] = basis[5];
	Rinv[0][2] = basis[6]; Rinv[1][2] = basis[7]; Rinv[2][2] = basis[8];

	MATmat3_inverse(R, Rinv);					

	T[0] = -(R[0][0]*origin[0]+R[0][1]*origin[1]+R[0][2]*origin[2]);
	T[1] = -(R[1][0]*origin[0]+R[1][1]*origin[1]+R[1][2]*origin[2]);
	T[2] = -(R[2][0]*origin[0]+R[2][1]*origin[1]+R[2][2]*origin[2]);
}

//
//	A: base atom position
//	R, T: converts from absolute position to fractional crystallographyc coordinates
//	sR, sT: Seitz symmetry matrices
//	Rinv: inverse of R
//	A': new atom position
//
//	Afr = R*A+T
//	Bfr = sR*Afr+sT/STBF
//	A'=Rinv*(Bfr-T)
//
//	rotation = Rinv*sR*R
//	translation = Rinv*((sR-I)*T+sT/STBF)
//
static void make_rotation(float rotation[3][3], float R[3][3], float Rinv[3][3], int sR[9])
{
	int r, c;
	for(r = 0; r < 3; r++)
	{
		for(c = 0; c < 3; c++)
		{
			rotation[r][c] = sR[r*3+0] * R[0][c] + sR[r*3+1] * R[1][c] + sR[r*3+2] * R[2][c];
		}
	}
#if 0
	float res[3][3];
	int r, c;
	for(r = 0; r < 3; r++)
	{
		for(c = 0; c < 3; c++)
		{
			res[r][c] = sR[r*3+0] * R[0][c] + sR[r*3+1] * R[1][c] + sR[r*3+2] * R[2][c];
		}
	}
	for(r = 0; r < 3; r++)
	{
		for(c = 0; c < 3; c++)
		{
			rotation[r][c] = Rinv[r][0] * res[0][c] + Rinv[r][1] * res[1][c] + Rinv[r][2] * res[2][c];
		}
	}
#endif
}

static void make_translation(float translation[3], float Rinv[3][3], int sR[9], float T[3], int sT[3], int stbf)
{
	translation[0] = sR[0] * T[0] + sR[1] * T[1] + sR[2] * T[2] + sT[0]/(float)stbf;
	translation[1] = sR[3] * T[0] + sR[4] * T[1] + sR[5] * T[2] + sT[1]/(float)stbf;
	translation[2] = sR[6] * T[0] + sR[7] * T[1] + sR[8] * T[2] + sT[2]/(float)stbf;
#if 0
	float tmp[3];
	tmp[0] = (sR[0]-1) * T[0] + sR[1] * T[1] + sR[2] * T[2] + sT[0]/(float)stbf;
	tmp[1] = sR[3] * T[0] + (sR[4]-1) * T[1] + sR[5] * T[2] + sT[1]/(float)stbf;
	tmp[2] = sR[6] * T[0] + sR[7] * T[1] + (sR[8]-1) * T[2] + sT[2]/(float)stbf;
	
	translation[0] = Rinv[0][0]*tmp[0]+Rinv[0][1]*tmp[1]+Rinv[0][2]*tmp[2];
	translation[1] = Rinv[1][0]*tmp[0]+Rinv[1][1]*tmp[1]+Rinv[1][2]*tmp[2];
	translation[2] = Rinv[2][0]*tmp[0]+Rinv[2][1]*tmp[1]+Rinv[2][2]*tmp[2];
#endif
}

static void apply_symmetries(STM3_BASE_MolecularDataType& in, STM3_BASE_MolecularDataType& out, T_SgInfo& SgInfo, float R[3][3], float Rinv[3][3], float T[3])
{
	float rotation[3][3], translation[3];	
	int   i;

	int nLoopInv   = Sg_nLoopInv(&SgInfo);
	int nTrV       = SgInfo.LatticeInfo->nTrVector;
	const int *TrV = SgInfo.LatticeInfo->TrVector;

	// Compute the resulting number of atoms and bonds
	int nrepl = nTrV * nLoopInv * SgInfo.nList;
	out.num_atoms = in.num_atoms * nrepl;
	out.num_bonds = in.num_bonds * nrepl;

	// Compute all symmetries (see http://www.kristall.ethz.ch/LFK/software/sginfo/sginfo_loop_symops.html)
	int atoms_start_idx = 0;
	int bonds_start_idx = 0;
	for(int iTrV = 0; iTrV < nTrV; iTrV++, TrV += 3)
	{
		for(int iLoopInv = 0; iLoopInv < nLoopInv; iLoopInv++)
		{
			int f = (iLoopInv == 0) ? 1 : -1;

			const T_RTMx  *lsmx = SgInfo.ListSeitzMx;

			for(int iList = 0; iList < SgInfo.nList; iList++, lsmx++)
			{
				T_RTMx SMx;
				
				for(i = 0; i < 9; i++) SMx.s.R[i] = f * lsmx->s.R[i];
				for(i = 0; i < 3; i++) SMx.s.T[i] = iModPositive(f * lsmx->s.T[i] + TrV[i], STBF);

				// use SMx at this point
#ifdef DEBUG
    			printf("Applying matrix [%2d %2d %2d; %2d %2d %2d; %2d %2d %2d] and translation (%d,%d,%d)\n",
					   SMx.s.R[0],SMx.s.R[1],SMx.s.R[2],
					   SMx.s.R[3],SMx.s.R[4],SMx.s.R[5],
					   SMx.s.R[6],SMx.s.R[7],SMx.s.R[8],
					   SMx.s.T[0],SMx.s.T[1],SMx.s.T[2]);
				fflush(stdout);
#endif
				make_rotation(rotation, R, Rinv, SMx.s.R);
				make_translation(translation, Rinv, SMx.s.R, T, SMx.s.T, STBF);

				//replicate_symmetry(in, out, atoms_start_idx, bonds_start_idx, rotation, translation);
				replicate_symmetry(in, out, atoms_start_idx, bonds_start_idx, rotation, translation, Rinv, T);

				atoms_start_idx += in.num_atoms;
				bonds_start_idx += in.num_bonds;
			}
		}
	}
#ifdef DEBUG
    printf("*** End\n");
	fflush(stdout);
#endif
}



#include "ComputeBonds_gen.h"
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "../lib/stm3.h"
#include "mol_type.h"
//#include "ann/include/ANN/ANN.h"
#include "../extern/ann/include/ANN/ANN.h"
#include <vector>

// max bonds per atom tested
#define MAX_BONDS	10

// uncomment if you want a log of the module operation. The logfile is ./zz_compute_bonds.log
//#define WRITE_LOG

// b is the vertex and the other two define the angle to be measured
static double valence_angle(float *a, float *b, float *c)
{
	double v0, v1, v2;
	double w0, w1, w2;
	double dot_product, lv2, lw2;

	v0 = a[0] - b[0];
	w0 = c[0] - b[0];
	v1 = a[1] - b[1];
	w1 = c[1] - b[1];
	v2 = a[2] - b[2];
	w2 = c[2] - b[2];

	dot_product = v0*w0 + v1*w1 + v2*w2;
	lv2 = v0*v0 + v1*v1 + v2*v2;
	lw2 = w0*w0 + w1*w1 + w2*w2;

	return acos(dot_product/sqrt(lv2*lw2))*180./M_PI;
}

int
STM3_BASE_ComputeBonds::ComputeBonds(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// num_bonds (OMXint write)
	// connect_lst (OMXint_array write)
	// bond_type_lst (OMXint_array write)
	int i, j;
	
	// special case: if someone already computed the bonds in the reader module, simply copy them to output
	if((int)in.num_bonds > 0 && (int)force_recomputation == 0)
	{
		bool fallback = false;
		
		num_bonds = in.num_bonds;
		int *in_connect_lst = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(in_connect_lst)
		{
			int *out_connect_lst = (int *)connect_lst.ret_array_ptr(OM_GET_ARRAY_WR);
			if(out_connect_lst)
			{
				memcpy(out_connect_lst, in_connect_lst, 2*(int)in.num_bonds*sizeof(int));
				ARRfree(out_connect_lst);
			}
			else fallback = true;
			
			ARRfree(in_connect_lst);
		}
		else fallback = true;
		
		if(!fallback)
		{
			int *in_bond_type = (int *)in.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_RD);
			if(in_bond_type)
			{
				int *out_bond_type = (int *)bond_type_lst.ret_array_ptr(OM_GET_ARRAY_WR);
				if(out_bond_type)
				{
					memcpy(out_bond_type, in_bond_type, (int)in.num_bonds*sizeof(int));
					ARRfree(out_bond_type);
				}
				else fallback = true;
				
				ARRfree(in_bond_type);
			}
			else fallback = true;
		}
		
		if(!fallback) return 1;
	}
	
	// get the atom positions
	float *xyz_arr = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!xyz_arr) return 0;

	// initialize the output arrays size
	num_bonds = 0;
	
#ifdef WRITE_LOG
	FILE *fp = fopen("zz_compute_bonds.log", "w");
#endif

	// get the atom identities
	int *atom_z_arr = (int *)in.atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!atom_z_arr)
	{
		ARRfree(xyz_arr);
		return 0;
	}
		
#ifdef WRITE_LOG
	for(i=0; i < in.num_atoms; ++i)
	{
		char tmp[128];
		char *n_orig = in.atom_name.ret_str_array_val(i, tmp);
		fprintf(fp, "n: %4d R: %.3f Z: %2d %s\n", i+1, atom_properties[atom_z_arr[i]].coval, atom_z_arr[i], n_orig);
	}
#endif

	// honor the compute_h_bonds request only if there are H atoms
	bool local_compute_h_bonds = false;
	if(compute_h_bonds != 0)
	{
		for(i=0; i < in.num_atoms; ++i)
		{
			if(atom_z_arr[i] == 1)
			{
				local_compute_h_bonds = true;
				break;
			}
		}
	}
	
	// init the array of bonds for each atom and some values that don't depend on loop indices
	int *atom_nb = new int[in.num_atoms];
	memset(atom_nb, 0, in.num_atoms * sizeof(int));
	float max_dist_squared = max_h_bond_dist*max_h_bond_dist;
	float scX = (1.0F + adj_bonding_len/100.0F)*(1.0F + adj_bonding_len/100.0F);
	float scH = (1.0F + adj_H_bonding_len/100.0F)*(1.0F + adj_H_bonding_len/100.0F);
	
	// restrict the search distance. 4.06 is the maximum covalent radius for two K atoms
	float search_radius = (local_compute_h_bonds) ? max_h_bond_dist : 4.06F;
	if(search_radius < 4.06F) search_radius = 4.06F;
	float search_radius_squared = search_radius*search_radius;

	// build the ANN tree
	ANNpointArray data_pts = new ANNpoint[in.num_atoms];
	for(i=0; i < in.num_atoms; ++i) data_pts[i] = &xyz_arr[3*i];
	ANNkd_tree *the_tree = new ANNkd_tree(data_pts,				// the data points
		    							  (int)in.num_atoms,	// number of points
		    							  3);					// dimension of space
										  
	// build an array to guard against symmetrical linking
	bool *already_visited = new bool[in.num_atoms];
	for(i=0; i < in.num_atoms; ++i) already_visited[i] = false;
	
	// set the number of neighbors to be checked
	int max_nn_to_check = (in.num_atoms < MAX_BONDS) ? (int)in.num_atoms : MAX_BONDS;

	// initialize the bond arrays
	vector<int> connect_lst_vect;
	vector<int> bond_type_lst_vect;
	
	// check every pair of atoms for bonds
    //
    // The outer loop index 'i' is AFTER the inner loop 'j': 'i'
    // leads 'j' in the list: since hydrogens traditionally follow
    // the heavy atom they're bonded to, this makes it easy to quit
    // bonding to hydrogens after one bond is made by breaking out of
    // the 'j' loop when 'i' is a hydrogen and we make a bond to it.
    // Working backwards like this makes it easy to find the heavy
    // atom that came 'just before' the Hydrogen. mp
    // Base distance criteria on vdw...lb
    //
	for(i=in.num_atoms-1; i >= 0; --i)
	{
		// take out of the inner loop some memory access
		int atom_Z_i = atom_z_arr[i];
		
		// mark this atom as visited: here we find all its neighbors, so no more bonds will be added later
		already_visited[i] = true;
		
		// never test further if this atom has already filled all its bonds
		if(!local_compute_h_bonds || (atom_Z_i != 1 && atom_Z_i != 7 && atom_Z_i != 8 && atom_Z_i != 9))
		{
			if(atom_nb[i] >= atom_properties[atom_Z_i].nbonds) continue;
		}
		
		// take out of the inner loop some memory access
		float coval_i = atom_properties[atom_Z_i].coval;
		
		// ANN search
		// First load the point to be queried
		ANNpoint query_pt = &xyz_arr[3*i];
		ANNidx	nn_idx[MAX_BONDS];
		ANNdist dists[MAX_BONDS];
		the_tree->annkSearch(query_pt,			// query point
							 max_nn_to_check,	// number of nearest neighbors
							 nn_idx,			// nearest neighbors (returned)
     						 dists,				// distance (returned)
							 0.1);				// error bound (must be >= 0)

		for(int kk=0; kk < max_nn_to_check; ++kk)
		{
			// visit all valid neighbors
			j = nn_idx[kk];
			if(already_visited[j]) continue;
			if(dists[kk] > search_radius_squared) break; // the distance are in ascending ordered
			
			// take out some memory access
			int atom_Z_j = atom_z_arr[j];
			
			// never bond hydrogens to each other...
			if((atom_Z_i == 1)  && (atom_Z_j == 1))  continue;  /* H - H   */
#if 0
			// those texts are from STM makebonds() routine: what does they mean?
			if((atom_Z_i == 22) && (atom_Z_j == 12)) continue;  /* Ti - Mg */
			if((atom_Z_i == 17) && (atom_Z_j == 17)) continue;  /* Cl - Cl */
			if((atom_Z_i == 1)  && (atom_Z_j == 22)) continue;  /* H - Ti  */
			if((atom_Z_i == 6)  && (atom_Z_j == 17)) continue;  /* C - Cl  */
			if((atom_Z_i == 1)  && (atom_Z_j == 17)) continue;  /* H - Cl  */
			if((atom_Z_i == 12) && (atom_Z_j == 12)) continue;  /* Mg - Mg */
#endif			
			float dist_squared = dists[kk];
			float sum_coval = -1.0F;
			float sum_coval_squared = 0.0F;

			// check for H-bond
			// the H bonds forms when X___H...Y and X, Y are N, O, F
			if(local_compute_h_bonds &&
			   ((atom_Z_i == 1 && (atom_Z_j == 7 || atom_Z_j == 8 || atom_Z_j == 9)) ||
			    (atom_Z_j == 1 && (atom_Z_i == 7 || atom_Z_i == 8 || atom_Z_i == 9))))
			{ 
				sum_coval = coval_i + atom_properties[atom_Z_j].coval;
				sum_coval_squared = sum_coval*sum_coval;
				
				if((dist_squared <= max_dist_squared) && (dist_squared > sum_coval_squared))
				{
					connect_lst_vect.push_back(i);
					connect_lst_vect.push_back(j);
					int bt = H_BOND;
					bond_type_lst_vect.push_back(bt);				

#ifdef WRITE_LOG
					fprintf(fp, "%4d %4d %10f %10f %10f (possible h bond)\n", i+1, j+1, sqrt(dist_squared), sqrt(sum_coval_squared), sqrt(max_dist_squared));			
#endif			
					continue;
				}
			}

			// check for ordinary bond
			if(atom_nb[i] >= atom_properties[atom_Z_i].nbonds) continue;
			if(atom_nb[j] >= atom_properties[atom_Z_j].nbonds) continue;

			// if not already computed for H bonds, compute them now
			if(sum_coval < 0.0F)
			{
				sum_coval = coval_i + atom_properties[atom_Z_j].coval;
				sum_coval_squared = sum_coval*sum_coval;
			}

			float scale = ((atom_Z_i == 1) || (atom_Z_j == 1)) ? scH : scX;
			
#ifdef WRITE_LOG
			fprintf(fp, "%4d %4d %10f %10f\n", i+1, j+1, sqrt(dist_squared), sqrt(sum_coval_squared*scale));			
#endif			
			if(dist_squared <= sum_coval_squared*scale)
			{
				connect_lst_vect.push_back(i);
				connect_lst_vect.push_back(j);
				int bt = (atom_Z_i == 1 || atom_Z_j == 1) ? H_NORMAL_BOND : SINGLE_BOND;
				bond_type_lst_vect.push_back(bt);				
			
				++atom_nb[i];
				++atom_nb[j];
			}
		}
	}

	// OK now copy the bonds data to the outside arrays
	num_bonds = bond_type_lst_vect.size();

	int *connect_lst_arr = (int *)connect_lst.ret_array_ptr(OM_GET_ARRAY_RW);
	if(!connect_lst_arr)
	{
		ARRfree(xyz_arr);
		return 0;
	}
	int *bond_type_lst_arr = (int *)bond_type_lst.ret_array_ptr(OM_GET_ARRAY_RW);
	if(!bond_type_lst_arr)
	{
		ARRfree(connect_lst_arr);
		ARRfree(xyz_arr);
		return 0;
	}
	for(i=0; i < num_bonds; ++i)
	{
		// OK now copy the bonds data to the outside arrays
		connect_lst_arr[2*i+0] = connect_lst_vect[2*i+0];
		connect_lst_arr[2*i+1] = connect_lst_vect[2*i+1];
		bond_type_lst_arr[i]   = bond_type_lst_vect[i];
	}
	
	// One H bond forms when X___H...Y where X, Y are N, O or F. Here we check the angle HXY. It should be less than 30 deg.
	for(i=0; i < num_bonds; ++i)
	{
		// recheck H-bonds for min angle constrain
		if(bond_type_lst_arr[i] != H_BOND) continue;

		int idx_X, idx_H, idx_Y;
		int idx1 = connect_lst_arr[2*i+0];
		int idx2 = connect_lst_arr[2*i+1];

		if(atom_z_arr[idx1] == 1)
		{
			idx_H = idx1;
			idx_Y = idx2;
		}
		else
		{
			idx_H = idx2;
			idx_Y = idx1;
		}

#ifdef WRITE_LOG
		fprintf(fp, "idx_H = %2d idx_Y = %2d atom_z_arr[idx1] = %2d atom_z_arr[idx2] = %2d\n", idx_H+1, idx_Y+1, atom_z_arr[idx1], atom_z_arr[idx2]);
#endif
		// find the other atom connected to H
		idx_X = -1;
		for(j=0; j < num_bonds; ++j)
		{
			if(bond_type_lst_arr[j] != H_BOND && bond_type_lst_arr[j] != NO_BOND)
			{
#ifdef WRITE_LOG
				fprintf(fp, "[+0] = %2d [+1] = %2d\n", connect_lst_arr[2*j+0]+1, connect_lst_arr[2*j+1]+1);
#endif
				if(connect_lst_arr[2*j+0] == idx_H) {idx_X = connect_lst_arr[2*j+1]; break;}
				if(connect_lst_arr[2*j+1] == idx_H) {idx_X = connect_lst_arr[2*j+0]; break;}
			}
		}
#ifdef WRITE_LOG
		fprintf(fp, "idx_X = %2d (j=%d/%d)\n", idx_X, j, (int)num_bonds);
#endif
		if(idx_X < 0) 
		{
#ifdef WRITE_LOG
			fprintf(fp, "Incoherent bonds found for H atom n. %d", idx_H+1);
#endif
			//ERRverror("ComputeBonds", ERR_ERROR, "Incoherent bonds found for H atom n. %d", idx_H+1);
			bond_type_lst_arr[i] = NO_BOND;
			continue;
		}

#ifdef WRITE_LOG
		fprintf(fp, "atom_z_arr[idx_X] = %2d\n", atom_z_arr[idx_X]);
#endif
		// check if the Y atom is a valid one
		if(atom_z_arr[idx_X] != 7 && atom_z_arr[idx_X] != 8 && atom_z_arr[idx_X] != 9) {bond_type_lst_arr[i] = NO_BOND; continue;}
		
		// check angle
		if(valence_angle(&xyz_arr[3*idx_H], &xyz_arr[3*idx_X], &xyz_arr[3*idx_Y]) > max_h_bond_angle) {bond_type_lst_arr[i] = NO_BOND; continue;}
	}

	// clean up bond list removing invalid H-bonds
	for(i=j=0; i < num_bonds; ++i)
	{
		if(bond_type_lst_arr[i] != NO_BOND)
		{
			if(i != j)
			{
				// copy j <- i
				bond_type_lst_arr[j]   = bond_type_lst_arr[i];
				connect_lst_arr[2*j+0] = connect_lst_arr[2*i+0];
				connect_lst_arr[2*j+1] = connect_lst_arr[2*i+1];
			}
			++j;
		}
	}	
	ARRfree(connect_lst_arr);
	ARRfree(bond_type_lst_arr);
	
	// adjust the total number of bonds
	if(i != j)
	{
		num_bonds = j;
		int *connect_lst_arr = (int *)connect_lst.ret_array_ptr(OM_GET_ARRAY_RW);
		if(connect_lst_arr) ARRfree(connect_lst_arr);
		int *bond_type_lst_arr = (int *)bond_type_lst.ret_array_ptr(OM_GET_ARRAY_RW);
		if(bond_type_lst_arr) ARRfree(bond_type_lst_arr);
	}
	
	// release resources
	ARRfree(xyz_arr);
	ARRfree(atom_z_arr);
	delete [] atom_nb;
#ifdef WRITE_LOG
	fclose(fp);
#endif
	delete the_tree;
	delete [] data_pts;
	delete [] already_visited;
	return 1;
}

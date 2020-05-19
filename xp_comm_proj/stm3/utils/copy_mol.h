#include "../base/ReadMol_gen.h"

extern void copy_full_molecule(STM3_BASE_MolecularDataType& in, STM3_BASE_MolecularDataType& out, bool set_sizes_also = true);
extern void empty_molecule(STM3_BASE_MolecularDataType& out);
extern void copy_partial_molecule(STM3_BASE_MolecularDataType& in, STM3_BASE_MolecularDataType& out, bool *included);

// support for lattice replication
extern void replicate_full_molecule(STM3_BASE_MolecularDataType& in, STM3_BASE_MolecularDataType& out,
									int atoms_start_idx, int bonds_start_idx, float position_offset[3]);
									
// support for crystal symmetries
#if 0
// old version
extern void replicate_symmetry(STM3_BASE_MolecularDataType& in, STM3_BASE_MolecularDataType& out,
							   int atoms_start_idx, int bonds_start_idx, float rotation[3][3], float translation[3]);
#endif
extern void replicate_symmetry(STM3_BASE_MolecularDataType& in, STM3_BASE_MolecularDataType& out,
							   int atoms_start_idx, int bonds_start_idx,
							   float R1[3][3], float T1[3], float R2[3][3], float T2[3]);

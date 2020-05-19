group MolecularDataType<NEportLevels={0,1},
                        NEnumColors=4,
                        NEcolor0=0xff0000,NEcolor1=0x000000,
                        NEcolor2=0x00ffff,NEcolor3=0x000000> {
						
	int      num_atoms<export=2>;							// total number of atoms
	int      atom_z<export=2>[.num_atoms];				// atomic numbers list
	string   atom_name<export=2>[.num_atoms];			// atom names as read from the file
	string   residue_name<export=2>[.num_atoms];		// residue names
	int      residue_id<export=2>[.num_atoms];		// residue number
	float    xyz_lst<export=2>[.num_atoms][3];		// atom coordinates
	float    charge_lst<export=2>[.num_atoms];		// atom charge (or any other scalar parameter associated to the atom)

	int      num_bonds<export=2>;							// numeber of bonds
	int      connect_lst<export=2>[2*.num_bonds];	// connection list
	int      bond_type_lst<export=2>[.num_bonds];	// type of bond; the values are defined in mol_type.h

	float    offset_lst<export=2>[.num_atoms][3];	// atom offsets provided by some formats

	group unit_cell<export=2> {							// info on unit cell placement (some info are redundant, but keep them for convenience)
		float    side_vectors<export=2>[3][3]; 		// vectors defining the three unit cell sides (first vector (0, 0, 0) means no unit cell available)
		float 	angles<export=2>[3]; 					// angles between unit cell sides (as defined for PDB format)
		float 	origin<export=2>[3]; 					// the origin of the unit cell
	};
	
	group symmetry<export=2> {								// info on atom symmetries
		string	space_group<export=2>;					// the space group symmetry group label (eg. P 21 21 21)
	};

};

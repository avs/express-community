group DisplayParams<NEportLevels={0,1},
                    NEnumColors=4,
                    NEcolor0=0x00ffff,NEcolor1=0x000000,
                    NEcolor2=0x000000,NEcolor3=0x000000> {
						
	int      num_atoms<export=2>;							// total number of atoms
	int      atom_z<export=2>[.num_atoms];				// atomic numbers list
	string   residue_name<export=2>[.num_atoms];		// residue names
	float    charge_min<export=2>;						// min atom charge (or any other scalar parameter associated to the atom)
	float    charge_max<export=2>;						// max atom charge (or any other scalar parameter associated to the atom)
	int		atoms_representation<export=2>;			// representation for atoms
	int		bonds_representation<export=2>;			// representation for bonds
	int		bonds_split_color<export=2>;				// use split coloring for bonds
   float 	normal_scale<export=2>;						// scale for atoms
   float 	cpk_scale<export=2>;							// scale for atoms in CPK representation
   float 	licorice_scale<export=2>;					// scale for atoms in "licorice" representation
   float 	default_colors<export=2>[3];				// default color
};


module ComputeSES<src_file="ComputeSES.cxx",
                out_hdr_file="ComputeSES_gen.h",
                out_src_file="ComputeSES_gen.cxx",
                cxx_hdr_files="fld/Xfld.h",
                build_dir="xp_comm_proj/stm3/msms"> {

   cxxmethod+req ComputeSES(
        .xyz_lst+read+notify+req,
        .atom_z+read+notify+req,
        .charge+read+notify,
        .probe_radius+read+notify+req,
        .density+read+notify+req,
        .color+read+notify,
		  .status+write,
		  .last_atoms_idx+read+write,
        .fld+write);

   float xyz_lst<NEportLevels={2,0}>[];
   int   atom_z<NEportLevels={2,0}>[];
   float charge<NEportLevels={2,0}>[];
   float probe_radius<NEportLevels={2,0}> = 1.4;
   float density<NEportLevels={2,0}> = 1.0;
   enum color<NEportLevels={2,0}> {
      choices => { "no_color", "atom type", "atom charge", "atom z"};
   } = "atom type";
   string status<NEportLevels={0,2}> = "";
   Field+Float fld<NEportLevels={0,2}> {
      nspace = 3;
      ncell_sets = 1;
      cell_set+Tri = {
      {
         name=>"SES"
      }};
      nnode_data = 2;
   };
   int   last_atoms_idx<NEportLevels={0,0},NEvisible=0>[0];
   float last_normals<NEportLevels={0,0},NEvisible=0>[0];
};


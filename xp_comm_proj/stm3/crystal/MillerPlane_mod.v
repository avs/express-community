module MillerPlane<src_file="MillerPlane.cxx",
                   out_hdr_file="MillerPlane_gen.h",
                   out_src_file="MillerPlane_gen.cxx",
                   cxx_hdr_files="../base/ReadMol_gen.h",
                   build_dir="xp_comm_proj/stm3/crystal"> {

   cxxmethod+req ComputePlane(
      .in+read+notify+req,
      .n1+read+notify,
      .n2+read+notify,
      .n3+read+notify,
      .side+read+notify,
      .offset+read+notify,
      .scale_u+read+notify,
      .scale_v+read+notify,
      .vertices+write,
      .out+write);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
	
   int   n1<NEportLevels={2,0}> = 1;	// Miller plane indices
   int   n2<NEportLevels={2,0}> = 0;
   int   n3<NEportLevels={2,0}> = 0;

   enum  side<NEportLevels={2,0}> {choices => {"below", "above"};} = "below"; 	// plane side where to leave atoms
   float offset<NEportLevels={2,0}> = 0.;  // offset of the plane respect to the unit cell origin or the normal position of the plane
   float scale_u<NEportLevels={2,0}> = 1.; // scale for one plane dimension
   float scale_v<NEportLevels={2,0}> = 1.; // scale for one plane dimension
	
   float vertices<NEportLevels={0,2}>[4][3]; // the output quad vertices
   XP_COMM_PROJ.STM3.BASE.MolecularDataType out<NEportLevels={0,2}> {
		&unit_cell => <-.in.unit_cell;
		&symmetry => <-.in.symmetry;
	};	
};


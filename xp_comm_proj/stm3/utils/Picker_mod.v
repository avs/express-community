
//
//	Measures the distance between two picked points
//
module Picker<src_file="Picker.cxx",
				  out_src_file="Picker_gen.cxx",
				  out_hdr_file="Picker_gen.h",
              cxx_hdr_files="fld/Xfld.h ../base/ReadMol_gen.h",
              build_dir="xp_comm_proj/stm3/utils"> {

   cxxmethod+req DoPicking(
			.in+read,
			.mode+read+notify+req,
			.pick_info+read+notify+req,
			.label_pt+write,
			.label+write,
			.extended_label+write,
			.line_size+write,
			.line_pt+write,
			.line_conn+write,
			.nprev_atoms+read+write,
			.prev_atoms+read+write,
			.nlast_atoms+read+write
   );

   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;

   enum mode<NEportLevels={2,0}>{
      choices => {"disabled", "point", "distance", "angle", "torsion"};
   } = "disabled";

   //
   //	To be connected to .picked_obj
   // 
   GDM.GDobject_templ &picked_obj<NEportLevels={2,0}>;
   GDpick_info+nres &pick_info<NEportLevels={0,0}> => .picked_obj.pick_info;

   //
   //	Current picked point coords, previous coords and distance between them
   //
   float  label_pt<NEportLevels={0,2}>[1][3];
   string label<NEportLevels={0,2}> = "";
   string extended_label<NEportLevels={0,2}> = "";
   int    line_size<NEportLevels={0,2}> = 0;
   float  line_pt<NEportLevels={0,2}>[line_size][3];
   int    line_conn<NEportLevels={0,2}>[];

   int   nprev_atoms<NEportLevels=0,NEvisible=0> = 0;
   int   nlast_atoms<NEportLevels=0,NEvisible=0> = 0;
   int   prev_atoms<NEportLevels=0,NEvisible=0>[4];
};

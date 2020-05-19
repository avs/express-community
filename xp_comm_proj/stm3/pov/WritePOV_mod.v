module WritePovCore<src_file="WritePOV.cxx",
   					  out_src_file="WritePOV_gen.cxx",
   					  out_hdr_file="WritePOV_gen.h",
						  cxx_hdr_files="../utils/Picker_gen.h fld/Xfld.h ../base/ColorLegendCore_gen.h",
						  cxx_members="   private:\n      void output_fields(FILE *fp, float radius);\n",
						  build_dir="xp_comm_proj/stm3/pov"> {

		cxxmethod+req WritePov(
				.in+read+notify,
				.filename+read+notify+req,
				.fld+read+notify,
				.enable_animation+read,
				.step+read,
				.display_params+read,
				.transparency+read,
				.background_type+read,
				.finish+read,
				.err+write,
				.err_str+write
		);

		XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
		Mesh+Node_Data &fld<NEportLevels={2,0}>[];
		string filename<NEportLevels={2,0}>;

		// for animation support
		enum        enable_animation<NEportLevels={2,0}> {
   		  choices => {"no", "yes"};
		} = "no";
		int			step<NEportLevels={2,0}> = 0;
				
		// rendering parameters
   	XP_COMM_PROJ.STM3.BASE.DisplayParams &display_params<NEportLevels={2,0}>;
		float       transparency<NEportLevels={2,0}> = 0.0;
		enum        background_type<NEportLevels={2,0}> {
   		  choices => {"solid blue", "sunny sky"};
		} = "solid blue";

		// surface rendering type
		enum        finish<NEportLevels={2,0}> {
   		  choices => {"normal", "phong", "PovChem"};
		} = "PovChem";

		// status output
		int			err<NEportLevels={0,2}> = 0;
		string		err_str<NEportLevels={0,2}> = "";
};


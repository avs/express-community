module ReadGaussianCube<src_file="ReadGaussianCube.cxx",
                        out_hdr_file="ReadGaussianCube_gen.h",
                        out_src_file="ReadGaussianCube_gen.cxx",
                        cxx_hdr_files="fld/Xfld.h",
                        build_dir="xp_comm_proj/stm3/base"> {

   cxxmethod+req+notify_inst ReadGaussianCube(
		.filename+read+notify+req,
		.units+read+notify+req,
		.out+write);
		
   string filename<NEportLevels={2,0}>;
   int units<NEportLevels={2,0}> = 1;
   Field_Unif+Float out<NEportLevels={0,2}>;
};

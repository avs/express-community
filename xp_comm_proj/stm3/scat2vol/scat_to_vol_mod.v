module scat_to_vol<src_file="scat_to_vol.cxx",
                   out_hdr_file="scat_to_vol_gen.h",
                   out_src_file="scat_to_vol_gen.cxx",
			          cxx_hdr_files="fld/Xfld.h",
                   build_dir="xp_comm_proj/stm3/scat2vol"> {

   cxxmethod+req upd(
      .in+read+notify+req,
	   .radius+read+notify+req,
      .resolution+read+notify+req,
      .use_manual_limits+read,
      .manual_limits+read,
	   .out+write
   );

   Mesh+Node_Data+Float &in<NEportLevels={2,0}>;

   float radius<NEportLevels={2,0}> = 0.3;
   float resolution<NEportLevels={2,0}> = 0.05;
   enum  use_manual_limits<NEportLevels={2,0}> {
		choices => {"no", "yes"};
   } = "no";
   float manual_limits<NEportLevels={2,0}>[2][3];
   
   Field_Unif+Byte out<NEportLevels={0,2}>;
};

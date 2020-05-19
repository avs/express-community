library StrArrSubMods<build_dir="xp_comm_proj/strarsub",out_src_file="sas_gen.cxx",out_hdr_file="sas_gen.h"> {
      module string_array_subset<src_file="strarsub.cxx"> {
         string in<NEportLevels={2,0}>[];
         int start<NEportLevels={2,0}>;
         int end<NEportLevels={2,0}>;
         string out<NEportLevels={0,2}>[end-start+1];
         cxxmethod+req+notify_inst copy (
            in+read+req+notify,
            start+read+req+notify,
            end+read+req+notify,
            out+write
         );
      };

};

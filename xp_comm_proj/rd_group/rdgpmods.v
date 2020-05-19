
flibrary ReadGroupMods <build_dir="xp_comm_proj/rd_group",
                        cxx_name="",
 		        out_hdr_file="gen.h",
		        out_src_file="gen.cxx"> {

  module ReadGroup <src_file = "rd_group.cxx",
                    cxx_members = <"
      protected:
        char*
	ReadString (FILE *fp);

        int
        DeserializeGroup (OMobj_id groupId,
                          FILE *fp);
        int
        Deserialize (OMobj_id groupId,
                     FILE *fp);
     "> > {

    group &group <NEportLevels = {2,0}>;
    string filename <NEportLevels = {2,0}>;

    prim trigger <NEportLevels = {2,0}>;

    cxxmethod+req Read ( .trigger+req+notify,
		         .filename+req+read,
		         .group+req+write );
  };

};


flibrary WriteGroupMods <build_dir="xp_comm_proj/wr_group",
                         cxx_name="",
 		         out_hdr_file="gen.h",
		         out_src_file="gen.cxx"> {

  module WriteGroup <src_file = "wr_group.cxx",
                     cxx_members = <"
      protected:
        int
	WriteString (const char *string,
                     FILE *fp);

        int
        SerializeGroup (OMobj_id groupId,
                        OMobj_id templateId,
                        FILE *fp);
        int
        Serialize (OMobj_id objId,
                   OMobj_id templateId,
                   FILE *fp);
    "> > {

    group &group <NEportLevels = {2,0}>;
    string filename <NEportLevels = {2,0}>;

    prim trigger <NEportLevels = {2,0}>;

    cxxmethod+req Write( .trigger+req+notify,
		         .filename+req+read,
		         .group+req+read );
  };
};

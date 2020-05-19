
flibrary CatCSMods <build_dir="xp_comm_proj/cat_cs",
                      cxx_hdr_files="fld/Xfld.h",
                      out_src_file="ccs_gen.cxx",
                      out_hdr_file="ccs_gen.h">
{
  module CatCellSets<src_file="cat_cs.cxx">
  {
     cxxmethod+notify_inst+req update;
     Mesh+read+notify+req &mesh_in<NEportLevels={2,0}>;
     Mesh+write mesh_out<NEportLevels={0,2}>
     {
        nspace => <-.mesh_in.nspace;
        nnodes => <-.mesh_in.nnodes;
        coordinates
        {
           values => <-.<-.mesh_in.coordinates.values;
        };
     };
  };

  group Add_Data_to_Mesh
  {
     Mesh &mesh_in<NEportLevels={2,0}>;
     Node_Data &data_in<NEportLevels={2,0}>;
     Mesh+Node_Data &out<NEportLevels={0,2}> => merge(mesh_in,data_in);
  };

};

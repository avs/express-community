flibrary Convert2ByteMods <build_dir="xp_comm_proj/con2byt",
		       out_hdr_file="gen.h",
		       out_src_file="gen.cxx"> {



   module ConvertToByteNodeData <src_file="con2byt.c">{
      Node_Data &in <NEportLevels={2,0}> ;
      group out <NEportLevels={0,2}> {
            int nnodes => <-.in.nnodes;
            int nnode_data = 1;
            Data_Array node_data[.nnode_data] {
               byte values[.nvals][.veclen];
               byte+opt null_value;
               byte min;
               byte max;
               byte min_vec[.veclen];
               byte max_vec[.veclen];
            };
          };
      omethod+notify_inst+req update(
            in+read+notify+req,
            out+write )

            = "ConvertToByte";
  };
};



flibrary ReadBRPicMods <build_dir="xp_comm_proj/rd_brpic",
                        process="express",
                        out_hdr_file="gen.h",
                        out_src_file="gen.cxx"> {
 
  // define read_pic

    module ReadPicDims<src_file="rd_brpic.c"> {
         string filename<NEportLevels={2,0}>;
         int dims<NEportLevels={0,2}>[3];

         omethod+notify_inst+req read_pic_update (
            filename+read+notify+req,
            dims+write
	      ) = "read_pic_update";
    };


  // ReadPicData    reads image data using dimensions calculated by ReadPicDims

  group ReadPicData {
    string filename<NEportLevels={2,0}>;

    file FILE {
      name => <-.filename;
    };

    ReadPicDims ReadPicDims {
      filename=> <-.filename;
    };

    int dims<NEportLevels={0,2}>[3] => ReadPicDims.dims;
    byte data<NEportLevels={0,2}>[prod(dims)] => cache(file_obj(.FILE,76,1,,1));
  };

};


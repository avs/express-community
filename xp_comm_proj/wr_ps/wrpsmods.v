
flibrary WritePsMods {

   module WritePsCore<  build_dir="xp_comm_proj/wr_ps",
                        src_file="wr_ps.c",
                        out_src_file="gen.cxx",
                        out_hdr_file="gen.h" >
   {

      Image_ARGB+IPort2 &image_in;
      string+IPort2 file_name;

      int+OPort2 done;

      omethod+req FLDtoPS(image_in+read+req,
                          file_name+read+notify+req,
                          done+write+nonotify) = "FLDtoPS";
   };

};


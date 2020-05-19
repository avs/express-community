

flibrary ReadVtkMods <build_dir="xp_comm_proj/rd_vtk",
                      out_src_file="gen.cxx",
                      out_hdr_file="gen.h">
{

   module ReadVtkCore <src_file="rd_vtk.cxx"> {
      string+IPort2 filename;

      cxxmethod+notify_inst+req Read<status=1> (
         .filename+read+notify+req,
         .coords+write,
         .connect+write,
         .num_coords+write,
         .num_connect+write
      );

      int num_coords;
      int num_connect;
      float+OPort2 coords[num_coords][3];
      long+OPort2 connect[num_connect];
   };

};


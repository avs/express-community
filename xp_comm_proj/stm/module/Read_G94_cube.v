      module Read_G94_cube<NEx=231.,NEy=55.,build_dir="xp_comm_proj/stm/src",src_file="Read_G94_cube.c",process="express"> {
         omethod+notify_inst+req Update(
            File_Name+read+notify+req,total_data+write,total_coords+write,
            Orbital_Data+write,geometry+write,dims+write
         ) = "Read_G94_cube";
         string File_Name<NEportLevels={2,0}>;
         int total_data<export=2>;
         int total_coords<export=2>;
         float Orbital_Data<export=2,NEportLevels={0,2}>[total_data];
         float geometry<export=2,NEportLevels={0,2}>[total_data][3];
         int dims<export=2,NEportLevels={0,2}>[3];
      };

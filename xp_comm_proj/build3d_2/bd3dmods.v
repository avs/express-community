
flibrary Build3D_2_Mods <build_dir="xp_comm_proj/build3d_2",
                         out_hdr_file="gen.h">
//                         out_src_file="gen.cxx">
{
   group+OPort Build3D_2_Params {
      string+Port2 filename_head;
      string+Port2 filename_tail;
      int+Port2    min_length;
      int+Port2    format;
      int+Port2    start;
      int+Port2    end;
      int+Port2    out_format;
   };


   module Build3D_2_Core <src_file="build3d_2.c"> {
      string+IPort2  filename;
      int+IPort2     format;
      int+IPort2     start;
      int+IPort2     end;
      int+IPort2     out_format;

      omethod+notify_inst+req update<status=1> (
         .filename+read+notify+req,
         .format+read+notify+opt,
         .start+read+notify+req,
         .end+read+notify+req,
         .out_format+read+notify+req,
         .info+write,
         .out+write
      ) = "Build3D_2_Core_update";

      string+OPort2 info;
      Mesh_Unif+Dim3+Space3+Node_Data+OPort2 out;
   };



   module AdjustSliceSpacingCore <src_file="adjust.c"> {
      Mesh_Unif+IPort2 &in;
      int+IPort2       spacing_axis;
      float+IPort2     &slice_position[];

      omethod+notify_inst+req update<status=1> (
         .in+read+notify+req,
         .spacing_axis+read+notify+req,
         .slice_position+read+notify+req,
         .out+write
      ) = "AdjustSliceSpacingCore_update";

      Mesh_Rect+OPort2 out;
   };

};


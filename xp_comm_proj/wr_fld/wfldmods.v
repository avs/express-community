// 
// file: wfldmods.v
// author: Dave Goughnour, AVS
// purpose: Modules to generate/manipulate AVS fields


flibrary WriteFldMods <build_dir="xp_comm_proj/wr_fld",
                       out_src_file="gen.c",
                       out_hdr_file="gen.h">
{

   // module: Write_Field
   // purpose: Write a field to a AVS .fld file
   //
   module WriteFldCore <src_file="wr_fld.c"> {
      string+read+nonotify+req+IPort2    filename;
      Mesh_Struct+Node_Data+read+nonotify+req+IPort2 &input;
      int+read+notify+IPort2             trigger;

      omethod+notify_inst+req update = "WriteField_update";
   };

};


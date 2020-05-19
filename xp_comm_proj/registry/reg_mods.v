

flibrary RegistryMods <build_dir="xp_comm_proj/registry",
//                     process="user",
                       out_hdr_file="gen.h",
                       out_src_file="gen.cxx"> {


   module RegistryRead <src_file="regread.cxx"> {
      string+IPort2 HKey;
      string+IPort2 KeyName;
      string+IPort2 ValueName;
      int+IPort2    DoIt;

      cxxmethod+notify_inst update (
         HKey+read+req,
         KeyName+read+notify,
         ValueName+read+notify,
         DoIt+read+notify+req,
         Value+write,
         ValueF+write,
         ErrorCode+write
      );

      string+OPort2 Value;
      float+OPort2  ValueF;
      int+OPort2    ErrorCode;
   };


   module RegistryWrite<src_file="regwrite.cxx"> {
      string+IPort2 HKey;
      string+IPort2 KeyName;
      string+IPort2 ValueName;
      string+IPort2 Value;
      int+IPort2    DoIt;

      cxxmethod+notify_inst update (
         HKey+read+req,
         KeyName+read,
         ValueName+read,
         Value+read+notify+req,
         DoIt+read+notify+req,
         ErrorCode+write
      );

      int+OPort2    ErrorCode;
   };

};


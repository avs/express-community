// 
// file: rdflmods.v
// authors: Dr. Jean M. Favre, CSCS, Swiss Center for Scientific Computing
//   		Tobias Schiebeck, IAC
//
// purpose: Modules to read Fluent Universal File format

flibrary ReadFluentMods<build_dir="xp_comm_proj/rdfluent",
						out_hdr_file="gen.h"> 
{
  group ReadFluentParams {
  		string	filename;
		int 	trigger;
  };
  module read_fluent_core<src_file="rdfluent.c"> {
    string+req+read+nonotify+IPort2		filename;
    Mesh_Struct+Node_Data+write+OPort2	out;
    int+read+notify+IPort2			trigger;

    omethod+notify_inst+req read = "read_fluent";
  };
};

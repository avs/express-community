flibrary Slice_to_Volume_Mods<build_dir="xp_comm_proj/slc2vol",
                              out_hdr_file="gen.h"> 
{

	#ifndef long;
	int long;
	#endif 
	
	group+OPort Slice_to_Volume_Params
	{
		int+Port2 axis;
		long+Port2 Slice;
		long+Port2 dims[];
		int+Port2 use_value;
		int+Port2 value;
	};
	
	module Slice_to_Volume_Core<src_file="slc2vol.c"> 
	{
		omethod+notify_inst+req update( .Slice+read+notify+req,
										.axis+read+notify+req,
										.value+read+notify,
										.use_value+read+notify,
										.In_Slice+read+notify+req,
										.Ref_Volume+read+notify+req,
										.Out_Volume+read+notify+write
										) = "Slice_to_Volume";
		long+IPort2 Slice;
		int+IPort2 axis;
		int+IPort2 value;
		int+IPort2 use_value;
		Mesh_Unif+IPort2 &Ref_Volume;
		Mesh_Unif+Node_Data+IPort2 &In_Slice;
		Mesh_Unif+Node_Data+Port2 &Out_Volume{
			dims => Ref_Volume.dims;
			ndim => Ref_Volume.ndim;
			nspace => Ref_Volume.nspace;
			points => Ref_Volume.points;
			nnode_data = 1;
			node_data = {
				{
					veclen=1,,,,,,,
				}
			};
		};
	};
};

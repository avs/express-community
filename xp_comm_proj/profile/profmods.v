flibrary ProfileMods <build_dir="xp_comm_proj/profile", 
				   out_hdr_file="gen.h",
				   out_src_file="gen.cxx">
{

	group+OPort ProfileParams 
	{
    	int+Port2 nx;
    	int+Port2 ny;
    	int+Port2 x;
    	int+Port2 y;
    	int+Port2 state;
    	int+Port2 mode;
	};	

	module profile<src_file="profile.cxx"> 
	{
		cxxmethod+notify_inst+req update(
				.nx+read+notify+req,
				.ny+read+notify+req,
				.x+read+notify+req,
				.y+read+notify+req,
				.state+read+notify+req,
				.mode+read+notify+req,
				.px+read+write,
				.py+read+write,
				.out_nx+write,
				.out_ny+write,
				.map+read+write
				.line+read+write);
		ProfileParams+IPort2 &params;
		int nx => params.nx;
		int ny => params.ny;
		int x => params.x;
		int y => params.y;
		int state => params.state;
		int mode => params.mode;
		int px = 1;
		int py = 1;
		int out_nx;
		int out_ny;
		byte+OPort2 map[out_ny][out_nx];
		int+OPort2 line[.ny];
	};
};

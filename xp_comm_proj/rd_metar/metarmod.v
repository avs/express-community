flibrary ReadMETARSMods <cxx_name="", build_dir="xp_comm_proj/rd_metar"> 
{
	group+OPort ReadMETARSParams {
		string+Port2 filename;
	};
	
	module ReadMETARSCore <src_file="rd_metar.cxx",
		cxx_hdr_files="METARS.h fld/Xfld.h",
		hdr_dirs="xp_comm_proj/rd_metar/libMETARS",
		out_hdr_file="gen.h",
		out_src_file="gen.cxx">
	{
		string+IPort2 filename;
		string+IPort2 StationID[];
		Mesh+Space3+IPort2 &StationLocation;
		cxxmethod+req+notify_inst read(filename+notify+read+req,
			StationLocation+notify+read+req,
			StationID+notify+read+req,
			out+write);
		Mesh+Space3+Node_Data+Float+OPort2 out 
		{
			nnodes => <-.StationLocation.nnodes;
			nspace => <-.StationLocation.nspace;
			&coordinates => <-.StationLocation.coordinates;
		};
	};
};

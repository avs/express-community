// 
// file: rdtpmods.v
// author: I.Hallstein, DLR, WT-DV
//
// modifications: Tobias Schiebeck, IAC
//
// purpose: Modules to read Tecplot-Data-Files

flibrary ReadTecplotMods<build_dir="xp_comm_proj/rd_tecpl",out_hdr_file="gen.h">
{
	group ReadTecplotParams {
		string	filename;
		int 	trigger;
	};

	// Read Tecplot Modul, Version 1.0
	module ReadTecplotCore <c_src_files="tecplot.c rdtecpl.c"> 
	{
		string filename<NEportLevels={2,0}>;
		omethod+req+notify_inst update(filename+read+notify+req) = "ReadTecplotData";
		Multi_Block+read+write mb {
			fields<NEportLevels={0,1}>;
		};
	};
};

// Modules-Library of ReadVRML-project

flibrary ReadVRMLMods <	out_hdr_file="gen.h", out_src_file="gen.cxx">
{
	// Parameter Block - ReadVRMLParams

	group+Port ReadVRMLParams {
		string+Port2 filename;
		int+Port2 Subdivision;
	};	

	// Module - ReadVRMLCore

	module ReadVRMLCore <src_file="rd_vrml.cpp",
				cxx_hdr_files="fld/Xfld.h",libdeps="FLD",
		hdr_dirs="vrml2geo/include vrml2geo/qv/include">
	{
		string+IPort2 Filename;
		int+IPort2 Subdivision;
		cxxmethod+notify_inst+req readVRML(
			.Filename+req+read+notify,
			.Subdivision+read+notify,
			.out+write
		);
		Field+OPort2 out;
	};	
}; 

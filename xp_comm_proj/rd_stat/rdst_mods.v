
flibrary ReadStatMods <build_dir="xp_comm_proj/rd_stat",
		       out_hdr_file="gen.h",
		       out_src_file="gen.cxx"> {

   // define the parameter block group
   group ReadStatParams <NEportLevels={0,1}> {
     string  filename;
     double  StartLatitude = 35.0;
     double  StartLongitude = -70.0;
     double  EndLatitude = 36.0;
     double  EndLongitude = 80.0;
    };

	module read_Stations 
		   <
			src_file="read_Stations.cxx",
			cxx_src_files="METARSStation.cxx",
			cxx_hdr_files="METARSStation.h fld/Xfld.h"
		   >
	{
	   ReadStatParams &Params <NEportLevels={2,0}>;

	   cxxmethod+req+notify_inst read<status=1>
										  (filename+notify+read+req,
										   StartLatitude+notify+read+req,
										   StartLongitude+notify+read+req,
										   EndLatitude+notify+read+req,
										   EndLongitude+notify+read+req,
										   StationID+write,
										   StationLocation+write
										   );

			string filename      => Params.filename;
			float StartLatitude  => Params.StartLatitude;
			float StartLongitude => Params.StartLongitude;
			float EndLatitude    => Params.EndLatitude;
			float EndLongitude   => Params.EndLongitude;
			int+OPort2 NumberOfStations;
			Mesh+Space3+OPort2 StationLocation {
			   ncell_sets = 1;
			   Point cell_set[ncell_sets];
			   !cell_set[0] {
				  ncells => nnodes;
				  node_connect_list => init_array(ncells, 0, ncells-1);
			   };
			};
			string+OPort2 StationID[NumberOfStations];

	};
};

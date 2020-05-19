
// This library contains the V definitions of the cgns modules

flibrary ReadCgnsMods <build_dir="xp_comm_proj/rd_cgns",
                       out_hdr_file="gen.h"> {

   //define the parameter block group
   group+OPort CgnsParams {
      string+Port2 filename;
      boolean+Port2 load;
      string+Port2 status = "";
   }; 

   group+OPort CgnsOutputParams {
      int+Port2 currentBase;
      int+Port2 currentActiveZones[];
      int+Port2 currentSolutions[];
   };


	group BaseInfo {
		int nBase = 0;
		group base_info[0] {
			string base_name;
			int cell_dim;
			int phys_dim;
		};
	};


	group ZoneInfo {
		int nZone = 0;
		group zone_info[0] {
			string zone_name;
			int zone_type;
			string zone_label;
			int index_dim;
			boolean cartesian;
			int VertexSize[index_dim];
			int CellSize[index_dim];
			int VertexSizeBoundary[index_dim];
			int Rind[2*index_dim];
			int XYZ[3];
			string zone_info;
			int nSolution = 0;
			group sol_info[0] {
				string sol_name;
				int Rind[2*<-.index_dim];
				int GridLocation;
			};
			int nSection = 0;
			group sect_info[0] {
				string sect_name;
				int ElementType;
				int ElementRangeStart;
				int ElementRangeEnd;
				int ElementSizeBoundary;
				int ParentFlag;
			};
		};
	};



   module DVread_cgns <c_src_files="scan_cgns.c load_cgns.c structured.c unstructured.c",
#ifdef MSDOS
                       link_files="libcgns.lib"
#else
                       link_files="-Lxp_comm_proj/rd_cgns -lcgns"
#endif
                       >
   {
      CgnsParams+IPort2 &CgnsParams;
		CgnsOutputParams+IPort2 &CgnsOutputParams;

		string file_name => CgnsParams.filename;
		string status => CgnsParams.status;
		boolean load => CgnsParams.load;

    	int CurrentBase => CgnsOutputParams.currentBase;
		int CurrentActiveZones[] => CgnsOutputParams.currentActiveZones;
		int CurrentSolutions[] => CgnsOutputParams.currentSolutions;

		omethod+notify_val+notify_inst+req scan_cgns<weight=0> (
			file_name+read+req
		) = "scan_cgns";

		omethod+notify+req base_change_cgns<weight=0> (
			CurrentBase+read+req
		) = "base_change_cgns";

		omethod+notify+req load_cgns<weight=0,status=1> (
			load+read+req
		) = "load_cgns";

		BaseInfo+OPort2 BaseInfo;
		ZoneInfo+OPort2 ZoneInfo;

		Multi_Block+OPort2 Multi_Block {
         nblocks = 0;
         fields+Cell_Data;
      };

		group+OPort2 Multi_Block_Unstr {
			int nblocks = 0;
			Mesh+Node_Data+Cell_Data+OPort2 fields[nblocks] {
				ncell_sets = 0;
			};
		};
	};

};


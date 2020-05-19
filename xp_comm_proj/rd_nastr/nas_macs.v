
flibrary ReadNastranMacs <compile_subs=0>  {


   macro ReadNastranUI {

      XP_COMM_PROJ.ReadNastran.ReadNastranMods.ReadNastranParams &ReadNastranParams<NEportLevels={2,1}>;
      XP_COMM_PROJ.ReadNastran.ReadNastranMods.nastran_output &nastran_output<NEportLevels={2,1}>;

      UImod_panel UImod_panel {
         width = 500;
         title => "Read Nastran Neutral File";
         clientWidth = 500;
      };


      UItext FileName {
	  x = 5;
	  y = 5;
         parent => UImod_panel;
         text =>  ReadNastranParams.filename;
         width = 200;
      };

      UIbutton BrowseButton {
	  x = 205;
	  y = 5;
         parent => UImod_panel;
         label => "Browse...";
         width = 80;
      };
      UIfileDialog NastranFileDialog {
         parent => UImod_panel;
         title = "Read Nastran Filename";
         visible => BrowseButton.do;
         searchPattern = "*.bdf";
         filename => ReadNastranParams.filename;
      };


	VUIOptionMenuLabel WireframeMode {
	   parent => UImod_panel;
	   x = 5;
	   y => FileName.y + FileName.height + 5; 
	   options =>  ReadNastranParams.WireframeMode.choices;
	   selectedItem => ReadNastranParams.WireframeMode;
	   width => 200;
	   label = "Wireframe";
	   OPmenu {
	      x => <-.x + OPlabel.width + 5;
	      width => <-.width - OPlabel.width - 5;
	   };
	   OPlabel.width => 60;
	};
	WireframeMode SurfaceMode {
	   label = "Surface";
	   y => WireframeMode.y + WireframeMode.height + 5; 
	   options =>  ReadNastranParams.SurfaceMode.choices;
	   selectedItem => ReadNastranParams.SurfaceMode;
	};
	WireframeMode ColorMode {
	   label = "Color";
	   y => SurfaceMode.y + SurfaceMode.height + 5; 
	   options =>  ReadNastranParams.ColorMode.choices;
	   selectedItem => ReadNastranParams.ColorMode;
	};

   };


   macro ReadNastranFunc {
      XP_COMM_PROJ.ReadNastran.ReadNastranMods.ReadNastranParams& ReadNastranParams<NEportLevels={1,2}>;
      XP_COMM_PROJ.ReadNastran.ReadNastranMods.nastran_output nastran_output<NEportLevels={1,2}>;

      XP_COMM_PROJ.ReadNastran.ReadNastranMods.ReadNastranCore ReadNastranCore {
         ReadNastranParams => <-.ReadNastranParams;
         nastran_output => <-.nastran_output;
      };


	macro BuildMeshField {

		 Tri2 Triax6Cells<NEx=22.,NEy=33.> {
		    ncells => nastran_output.CTRIAX6_Size;
		    node_connect_list => nastran_output.CTRIAX6;
		    name => "Triax6";
		    int ncell_data=>!ReadNastranParams.NodeDataOn;
		    Data_Array cell_data[ncell_data] { 
			nvals => ncells;
			veclen=1;
			values => nastran_output.CTRIAX6MATERIALS;
			null_flag=0;
		    };
		 };
		 Hex Hexa8Cells<NEx=22.,NEy=66.> {
		    ncells => nastran_output.CHEXA8_Size;
		    node_connect_list => nastran_output.CHEXA8;
		    name => "Hexa8";
		    int ncell_data=>!ReadNastranParams.NodeDataOn;
		    Data_Array cell_data[ncell_data] { 
			nvals => ncells;
			veclen=1;
			values => nastran_output.CHEXA8MATERIALS;
			null_flag=0;
		    };
		 };
		 Hex2 Hexa20Cells<NEx=22.,NEy=99.> {
		    ncells => nastran_output.CHEXA20_Size;
		    node_connect_list => nastran_output.CHEXA20;
		    name => "Hexa20";
		    int ncell_data=>!ReadNastranParams.NodeDataOn;
		    Data_Array cell_data[ncell_data] { 
			nvals => ncells;
			veclen=1;
			values => nastran_output.CHEXA20MATERIALS;
			null_flag=0;
		    };
		 };

		 Prism Penta6Cells<NEx=22.,NEy=130.> {
		    ncells => nastran_output.CPENTA6_Size;
		    node_connect_list => nastran_output.CPENTA6;
		    name => "Penta6";
		    int ncell_data=>!ReadNastranParams.NodeDataOn;
		    Data_Array cell_data[ncell_data] { 
			nvals => ncells;
			veclen=1;
			values => nastran_output.CPENTA6MATERIALS;
			null_flag=0;
		    };
		 };

		 Prism2 Penta15Cells<NEx=22.,NEy=160.> {
		    ncells => nastran_output.CPENTA15_Size;
		    node_connect_list => nastran_output.CPENTA15;
		    name => "Penta15";
		    int ncell_data=>!ReadNastranParams.NodeDataOn;
		    Data_Array cell_data[ncell_data] { 
			nvals => ncells;
			veclen=1;
			values => nastran_output.CPENTA15MATERIALS;
			null_flag=0;
		    };
		 };

		 Tet Tetra4Cells<NEx=22.,NEy=190.> {
		    ncells => nastran_output.CTETRA4_Size;
		    node_connect_list => nastran_output.CTETRA4;
		    name => "Tetra4";
		    int ncell_data=>!ReadNastranParams.NodeDataOn;
		    Data_Array cell_data[ncell_data] { 
			nvals => ncells;
			veclen=1;
			values => nastran_output.CTETRA4MATERIALS;
			null_flag=0;
		    };
		 };

		 Tet2 Tetra10Cells<NEx=22.,NEy=220.> {
		    ncells => nastran_output.CTETRA10_Size;
		    node_connect_list => nastran_output.CTETRA10;
		    name => "Tetra10";
		    int ncell_data=>!ReadNastranParams.NodeDataOn;
		    Data_Array cell_data[ncell_data] { 
			nvals => ncells;
			veclen=1;
			values => nastran_output.CTETRA10MATERIALS;
			null_flag=0;
		    };
		 };

		 GMOD.select_array select_cells<NEx=242.,NEy=143.> {
			 array_input => {	Triax6Cells,
						Hexa8Cells,
						Hexa20Cells,
						Penta6Cells,
						Penta15Cells,
						Tetra4Cells,
						Tetra10Cells
					};
			 array_output<NEportLevels={0,2}>;
			 select => {	nastran_output.CTRIAX6_Size>0,
					nastran_output.CHEXA8_Size>0,
					nastran_output.CHEXA20_Size>0,
					nastran_output.CPENTA6_Size>0,
					nastran_output.CPENTA15_Size>0,
					nastran_output.CTETRA4_Size>0,
					nastran_output.CTETRA10_Size>0
					};
		 };

		 Field out_field<NEportLevels={0,2},NEx=363.,NEy=286.> {
			 nnodes => nastran_output.Size;
			 nspace => 3;
			 coordinates.values => nastran_output.Coords;
			 ncell_sets => array_size(cell_set);
			 group& cell_set<NEportLevels={2,0}>[] => select_cells.array_output;
		 };

		   macro ext_edges_unstruct<NEx=561.,NEy=231.> {
		      ilink in_field_unstruct<NEportLevels={2,1}> => <-.out_field;
		      DVext_edge DVext_edge {
			 in =>  <-.in_field_unstruct;
			 angle = 1.3;
		      };
		      olink out_fld<NEportLevels={1,2}> => .DVext_edge.out;
		   };

		 DataObject out_obj<NEx=572.,NEy=352.> {
			in => out_field;
			Modes {
				mode => {0,ReadNastranParams.WireframeMode+1,ReadNastranParams.SurfaceMode+1,0,0};
				normals => ReadNastranParams.NormalGenerationMode+1; 
				//colors = 0;
				//outline = 1;
			};
			Obj {
				//dmap => switch(ReadNastranParams.NodeDataOn+1,Datamap,NodeDatamaps.NodeDataMap);
				surf_chunk = 100000;
				line_chunk = 100000;
				cache_size => ReadNastranParams.CacheSize;
				use_altobj => ReadNastranParams.AlternateOn;
			};
			AltObject {
				AltModes.mode => {1,2,1,1,1}; //line rendering
				Obj.cache_size => ReadNastranParams.CacheSize;
				alt_in<NEportLevels={3,1}> => <-.<-.ext_edges_unstruct.out_fld;
			};
			obj<NEportLevels={1,3}>;
		 };
	};


      olink out_mesh => BuildMeshField.out_field;
      olink out_obj => BuildMeshField.out_obj.obj;
   };




   macro ReadNastran {

      XP_COMM_PROJ.ReadNastran.ReadNastranMods.ReadNastranParams ReadNastranParams<NEportLevels={1,2}>;

      ReadNastranFunc ReadNastranFunc {
         ReadNastranParams => <-.ReadNastranParams;
      };

      ReadNastranUI ReadNastranUI {
         ReadNastranParams => <-.ReadNastranParams;
         nastran_output => <-.ReadNastranFunc.nastran_output;
      };


      olink out_mesh => ReadNastranFunc.out_mesh;
      olink out_obj => ReadNastranFunc.out_obj;
   };



   APPS.SingleWindowApp ReadNastranEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {ReadNastran.out_obj};
            };
         };
      };

      ReadNastran ReadNastran;

   };


}; // end of library


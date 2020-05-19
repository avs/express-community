
flibrary ReadVis5DApps<compile_subs=0>
{
	APPS.SingleWindowApp ReadVis5DEg<NEdisplayMode="maximized"> {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.ReadVis5D.title,<-.<-.<-.isosurface.out_obj,
						<-.<-.<-.bounds.out_obj};
					Xform {
						ocenter = {26.5,21.5,24.5};
						dcenter = {3.91867,3.25895,3.59831};
						mat = {
							-0.00986319,0.0405551,-0.0949449,0.,
							 0.0992831,-0.0224384,-0.0198983,0.,
							-0.0283219,-0.0927812,-0.0366886,0.,
							0.,0.,0.,1.
						};
						xlate = {-3.91867,-3.25895,-3.59831};
					};
				};
				Lights {
					Lights = {
						{
							type="BiDirectional"
						},,,};
				};
			};
		};
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMacs.ReadVis5D ReadVis5D {
			params {
				filename = "$XP_PATH<1>/xp_comm_proj/rd_vis5d/data/99041600d1f24.v5d";
			};
			ReadVis5DUI {
				UIfileDialog {
					ok = 1;
					dirMaskCache = "$XP_PATH<1>/xp_comm_proj/rd_vis5d/data/*.v5d";
				};
			};
		};
		MODS.isosurface isosurface {
			in_field => <-.ReadVis5D.vars3d_out;
			IsoParam {
				iso_component = 4;
				iso_level => 3e-06;
				map_component = {1};
			};
		};
		MODS.bounds bounds {
			in_field => <-.ReadVis5D.vars3d_out;
			BoundsParam {
				edges = 1;
				imin = 1;
				imax = 1;
				jmin = 1;
				jmax = 1;
				kmin = 1;
				kmax = 1;
				data = 1;
			};
		};
	};

	APPS.SingleWindowApp ReadVis5DTopographyEg<NEdisplayMode="maximized"> {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.ReadVis5DTopography.obj,<-.<-.<-.ReadVis5D.title,
						<-.<-.<-.bounds.out_obj,<-.<-.<-.isosurface_A.out_obj,
						<-.<-.<-.isosurface_B.out_obj,<-.<-.<-.orthoslice.out_obj
					};
					Xform {
						ocenter = {20.,17.,18.9953};
						dcenter = {4.,3.4,3.79905};
						mat = { 
							0.2,0.,0.,0.,
							0.,0.2,0.,0.,
							0.,0.,0.2,0.,
							0.,0.,0.,1.
						};
						xlate = {-4.,-3.4,-3.79905};
						center = {20.,17.,18.9953};
					};
				};
				Lights {
					Lights = {
						{
							type="BiDirectional"
						},,,};
				};
				Camera {
					Camera {
						perspec = 1;
						front = 1.;
					};
				};
			};
		};
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMacs.ReadVis5D ReadVis5D {
			params {
				filename = "$XP_PATH<1>/xp_comm_proj/rd_vis5d/data/example.v5d";
				load_all_steps = 1;
				time_step = 0;
				show_title = 1;
			};
			ReadVis5DUI {
				UIfileDialog {
					ok = 1;
					dirMaskCache = "$XP_PATH<1>/xp_comm_proj/rd_vis5d/data/*.v5d";
				};
				WindSelectUI {
					list_x {
						selectedText = "U";
						selectedItem => 2;
					};
					list_y {
						selectedText = "V";
						selectedItem => 3;
					};
					list_z {
						selectedText = "W";
						selectedItem => 4;
					};
				};
			};
		};
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMacs.ReadVis5DTopography ReadVis5DTopography {
			Vis5Dfile => <-.ReadVis5D.filename;
			Vis5Dxform => <-.ReadVis5D.vars3d_out;
			params {
				topofile = "$XP_PATH<1>/xp_comm_proj/rd_vis5d/data/EARTH.TOPO";
			};
			ReadVis5DTopographyUI {
				UIfileDialog {
					ok = 1;
					dirMaskCache = "$XP_PATH<1>/xp_comm_proj/rd_vis5d/data/*.TOPO";
				};
			};
		};
		MODS.bounds bounds {
			in_field => <-.ReadVis5D.vars3d_out;
			BoundsParam {
				edges = 1;
				imin = 1;
				data = 1;
			};
		};
		MODS.isosurface isosurface_A {
			in_field => <-.ReadVis5D.vars3d_out;
			IsoParam {
				iso_level => 5e-05;
			};
			IsoUI {
				UIiso_level {
					min => ;
					max => ;
					decimalPoints = 4;
				};
			};
			obj {
				Props {
					trans = 0.4;
					inherit = 0;
				};
			};
		};
		MODS.isosurface isosurface_B {
			in_field => <-.ReadVis5D.vars3d_out;
			IsoParam {
				iso_component = 3;
				iso_level => 0.004;
				map_component = {0,2};
			};
			IsoUI {
				UIiso_level {
					min => ;
					max => ;
					decimalPoints = 4;
				};
			};
			obj {
				Props {
					trans = 0.4;
					inherit = 0;
				};
			};
		};
		MODS.orthoslice orthoslice {
			in_field => <-.ReadVis5D.vars3d_out;
			OrthoSliceParam {
				axis = 1;
				plane => 0;
			};
		};
	};
	
	ReadVis5DTopographyEg ReadVis5DStreamlinesEg {
		Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.ReadVis5DTopography.obj,<-.<-.<-.ReadVis5D.title,
						<-.<-.<-.bounds.out_obj,<-.<-.<-.isosurface_A.out_obj,
						<-.<-.<-.isosurface_B.out_obj,<-.<-.<-.orthoslice.out_obj,
						<-.<-.<-.streamlines.out_obj
					};
					Xform {
						ocenter = {20.,17.,18.9953};
						center = {20.,17.,18.9953};
					};
				};
			};
		};
		MODS.streamlines streamlines {
			in_field => <-.ReadVis5D.vars3d_out;
			in_probe => <-.isosurface_B.out_fld;
			StreamParam {
				component = 4;
				order = 1;
				nseg = 9;
				max_seg = 48;
			};
		};
	};
};

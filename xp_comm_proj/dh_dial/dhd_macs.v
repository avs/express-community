
flibrary DashDialMacs <compile_subs=0> {

	//
	// macro defining gd objects to represent
	// the dashboard face
	//
	// value input ranges from -2 to +2
	//


	//
	// define a very simple camera system to
	// display the dashboard widget, no picking, 2D only
	//
	macro dash_camera {

		float vertical_offset = -1.0;

		GDM.GroupObject Top2D  {
			child_objs <NEportLevels={3,0}>;
			Xform {
				xlate => { 0., <-.<-.vertical_offset, 0. };
			};
		};

		GDM.Mcamera Mcamera {
			Camera<NEx=572.,NEy=176.> {
				objects => {<-.<-.Top2D.obj};
			};
		};

		GDM.Mview Mview {
			VirtPal {
				vclass=1;
			};
			View {
				cameras => {
					<-.<-.Mcamera.Camera};
				handle<NEportLevels={3,0}> => <-.<-.UIrenderView.handle;
				trigger = 2;
				back_col = {0.5,0.5,0.5};
			};
		};
		BestVirtPal BestVirtPal;
		UIrenderView UIrenderView {
			x = 0;
			y = 0;
			parent => <-.UIframe;
			width => <-.UIframe.clientWidth - 8;
			height => <-.UIframe.clientHeight - 40;
		};
		UIframe UIframe {
			parent<NEportLevels={3,0}>;
			width = 200;
			height = 200;
			shadowType = 3;
			shadowThickness = 8;
			x <NEportLevels={3,0}> = 0;
			y <NEportLevels={3,0}> = 0;
		};
		UIlabel UIlabel  {
			parent => <-.UIframe;
			width => <-.UIframe.clientWidth;
			height = 40;
			alignment = 1;
			color.backgroundColor = "#C0C0C0";
			label<NEportLevels={3,0}>;
			x = 0;
			y => <-.UIrenderView.height;
		};
	};

	macro DashDial {
		int visible<NEportLevels={2,1}> = 1;
		ilink parent;
		float step_value <NEportLevels={2,1}> = 0.0;
		string title => name_of(<-,1);
		UIpanel UIpanel {
			visible => <-.visible;
			parent => <-.parent;
		};

		GEOMS.Arrow2 Arrow2 {
			obj {
				Obj {
					xform<NEportLevels={2,4}>;
				};
				Props {
					col = {1,1,1};
				};
				Modes {
					mode = {0,2,2,0,0};
				};
			};
		};

		group+OPort zrot_xform {
			DefaultXform xform {
				float+IPort3 angle = 0.;
				float tsin => sin(angle);
				float tcos => cos(angle);
				mat => {
					tcos,tsin,0.,0.,
					-tsin,tcos,0.,0.,
					0.,0.,1.0,0.,
					0.,0.,0.,1.
				};
			};
		};		

		zrot_xform zrot_xform {
			xform {
				angle => ((2.0-(<-.<-.step_value )) * 45.*3.1415926/180.0);
			};
		};
		MODS.set_xform set_xform {
			in_field1 => <-.Arrow2.out_fld;
			in_field2 => <-.zrot_xform;
		};
		MODS.scale scale {
			in_field => <-.set_xform.out_fld;
			ScaleParam {
				scale_x = 3.;
				scale_y = 3.;
			};
			ScaleUI {
				UIpanel UIpanel;
			};
		};

		//////// Added by Ma YingLiang, 21st Feb. 2002//////
		// Dash Tic Blocks, RGB node data
		macro mk_dashtic {
			Quad	set1 {
				ncells = 5;
				node_connect_list = {
					0,1,2,3,
					4,5,6,7,
					8,9,10,11,
					12,13,14,15,
					16,17,18,19
				};
			};
			Mesh+Node_Data+Vector+Float  glyph2 {
				int nnodes = 20;
				int nspace = 3;
				coordinates {

					float sc = 0.35; // glyph scale factor

					float values[nvals][veclen] = {
						{sc * 6,sc * -1,0},	// 1
						{sc * 6,sc * 1,0},
						{sc * 12,sc * 1,0},
						{sc * 12,sc * -1,0},

						{sc * 5.2,sc * 3.8,0},	// 2
						{sc * 3.8,sc * 5.2,0},
						{sc * 7.5,sc * 8.8,0},
						{sc * 8.8,sc * 7.5,0},

						{sc * -1,sc * 6,0},	// 3
						{sc * -1,sc * 12,0},
						{sc * 1,sc * 12,0},
						{sc * 1,sc * 6,0},

						{sc * -5.2,sc * 3.8,0},	 // 4
						{sc * -3.8,sc * 5.2,0},
						{sc * -7.5,sc * 8.8,0},
						{sc * -8.8,sc * 7.5,0},

						{sc * -6,sc * -1,0},	 // 5
						{sc * -6,sc * 1,0},
						{sc * -12,sc * 1,0},
						{sc * -12,sc * -1,0}
					};
				};
				int ncell_sets = 1;
				cell_set[ncell_sets] => {set1};

				#define DASHTIC_RED    {1,0,0}
				#define DASHTIC_PINK   {0.7,0.4,0.4}
				#define DASHTIC_WHITE  {1,1,1}
				#define DASHTIC_LTGRN  {0.4,0.7,0.4}
				#define DASHTIC_GREEN  {0,1,0}
				#define DASHTIC_BLACK  {0.1,0.1,0.1}

				nnode_data = 1;
				node_data[nnode_data];

				!node_data[0] {
					values[nvals][veclen] = {
						DASHTIC_RED,
						DASHTIC_RED,
						DASHTIC_RED,
						DASHTIC_RED,
						DASHTIC_PINK,
						DASHTIC_PINK,
						DASHTIC_PINK,
						DASHTIC_PINK,
						DASHTIC_WHITE,
						DASHTIC_WHITE,
						DASHTIC_WHITE,
						DASHTIC_WHITE,
						DASHTIC_LTGRN,
						DASHTIC_LTGRN,
						DASHTIC_LTGRN,
						DASHTIC_LTGRN,
						DASHTIC_GREEN,
						DASHTIC_GREEN,
						DASHTIC_GREEN,
						DASHTIC_GREEN
					};
					nvals => <-.nnodes;
					veclen = 3;
					id = 667;
				};
			};
			// std object output configuration
			GDM.DataObject obj {
				in => glyph2;
			};
			olink out_fld => glyph2;
			olink out_obj => obj.obj;
		};


		//
		// labels on dashboard object
		//
		macro mk_dashtext {

			Point	 set1 
			{
				ncells = 5;
				node_connect_list = {0,1,2,3,4};
			};

			Mesh mesh {
				int nnodes = 5;
				int nspace = 2;
				coordinates {
					float values[nvals][veclen] = {
						{4.5,-0.2},
						{3,3.2},
						{-0.3,4.5},
						{-3.6,3.2},
						{-5.5,-0.2}
					};
				};
				int ncell_sets = 1;
				cell_set[ncell_sets] => {set1};
			};
			string in_text[] = {"2","1","0","-1","-2"};

			TextValues TextValues {
				text_values => <-.in_text;
				align_horiz<NEportLevels={2,0}> = 0;
				align_vert<NEportLevels={2,0}> = 0;
				drop_shadow<NEportLevels={2,0}> = 0;
				bounds<NEportLevels={2,0}> = 0;
				underline<NEportLevels={2,0}> =0;
				background<NEportLevels={2,0}> =0;
				lead_line<NEportLevels={2,0}> =0;
				radial<NEportLevels={2,0}> =0;
				do_offset<NEportLevels={2,0}> =0;
				offset = { 0.05, 0.05, 0.0 };
			};

			TextField &TextField => merge(TextValues, mesh);

			GDM.DataObject obj {
				in => TextField;
				Props.col = DASHTIC_WHITE;
			};

			olink out_obj => obj.obj;

		};

		//////// Added by Ma YingLiang, 21st Feb. 2002//////

		dash_camera dash_camera {
			Top2D {
				child_objs => {
					<-.<-.mk_dashtic.out_obj,
					<-.<-.mk_dashtext.out_obj,
					<-.<-.scale.out_obj
				};
				Modes {
					mode = {0,0,2,0,0};
				};
			};
			UIframe {
				parent => <-.<-.UIpanel;
			};
			UIlabel {
				label => <-.<-.title;
			};
		};
	
	};

};

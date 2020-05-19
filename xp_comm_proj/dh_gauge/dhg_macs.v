
flibrary DashGaugeMacs <compile_subs=0> {

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

	//
	// labels on dashboard object
	//
	macro mk_dashtext {

		Point  set1 {
			ncells = 5;
			node_connect_list = {0,1,2,3,4};
		};

		Mesh mesh {
			int nnodes = 5;
			int nspace = 2;
			coordinates {
				float values[nvals][veclen] = {
					{4.2,0},
					{3.2,3.2},
					{0,4.3},
					{-3.2,3.2},
					{-4.2,0}
				};
			};
			int ncell_sets = 1;
			cell_set[ncell_sets] => {set1};
			int nnodes = 5;
			int nspace = 2;
			coordinates {
				float values[nvals][veclen] = {
					{4.2,0},
					{3.2,3.2},
					{0,4.3},
					{-3.2,3.2},
					{-4.2,0}
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

		#define DASHTIC_BLACK  {0.1,0.1,0.1}

		GDM.DataObject obj {
			in => TextField;
			Props.col = DASHTIC_BLACK;
		};

		olink out_obj => obj.obj;

	};

	////////////////////////////////
	// Needle and center dot with colored verts
	macro mk_needle {
		Tri  set1 {
			ncells = 16;
			node_connect_list = {
				2,0,1,
				3,6,2,
				4,7,3,
				5,8,4,

				9,10,11,
				9,11,12,
				9,12,13,
				9,13,14,
				9,14,15,
				9,15,16,
				9,16,17,
				9,17,18,
				9,18,19,
				9,19,20,
				9,20,21,
				9,21,10
			};
		};
		Mesh+Node_Data+Vector+Float+Space3+OPort tri_rgb {

			nnodes = 22;

			coordinates {
				float sc = 0.22;
				values[nvals][veclen] = {

				// needle
					{0,6,1},
					{-1,0,1},
					{-0.5,0.7,1},
					{0,0.9,1},
					{0.5,0.7,1},
					{1,0,1},
					{0,6.02,1},
					{0,6.03,1},
					{0,6.04,1},
				// circle
					{0,0,1},
					{sc*4,sc*0,1},
					{sc*3.4,sc*2,1},
					{sc*2,sc*3.4,1},
					{sc*0,sc*4,1},
					{sc*(-2),sc*3.4,1},
					{sc*(-3.4),sc*2,1},
					{sc*(-4),sc*0,1},
					{sc*(-3.4),sc*(-2),1},
					{sc*(-2),sc*(-3.4),1},
					{sc*0,sc*(-4),1},
					{sc*2,sc*(-3.4),1},
					{sc*3.4,sc*(-2),1}

				};
			};

			ncell_sets = 1;
			cell_set[ncell_sets] => {set1};

			nnode_data = 1;
			node_data[nnode_data];

			!node_data[0] {
				float lr = 0.99;
				float lg = 0.40;
				float lb = 0.88;

				values[nvals][veclen] = {
				// needle
					{ 0.25 * lr, 0.25 * lg, 0.25 * lb },
					{ 0.25 * lr, 0.25 * lg, 0.25 * lb },
					{ 0.25 * lr, 0.25 * lg, 0.25 * lb },
					{ 1.00 * lr, 1.00 * lg, 1.00 * lb },
					{ 0.50 * lr, 0.50 * lg, 0.50 * lb },
					{ 0.25 * lr, 0.25 * lg, 0.25 * lb },
					{ 0.25 * lr, 0.25 * lg, 0.25 * lb },
					{ 1.00 * lr, 1.00 * lg, 1.00 * lb },
					{ 0.75 * lr, 0.75 * lg, 0.75 * lb },
				// center circle
					{ 0.9, 0.9, 0.99 },
					{ 0.2, 0.2, 0.99 },
					{ 0.2, 0.2, 0.99 },
					{ 0.2, 0.2, 0.99 },
					{ 0.2, 0.2, 0.99 },
					{ 0.2, 0.2, 0.99 },
					{ 0.2, 0.2, 0.99 },
					{ 0.2, 0.2, 0.99 },
					{ 0.2, 0.2, 0.99 },
					{ 0.2, 0.2, 0.99 },
					{ 0.2, 0.2, 0.99 },
					{ 0.2, 0.2, 0.99 },
					{ 0.2, 0.2, 0.99 }
				};
				nvals => <-.nnodes;
				veclen = 3;
				id = 667;
			};
		};

		// std object output configuration

		VIEW.DataObject DataObject<NEx=418,NEy=209> {
			in => <-.tri_rgb;
		};

		olink out_fld => tri_rgb;
		olink out_obj<NEx=462,NEy=330> => .DataObject.obj;
	};


	macro transform_needle {
		float scale_factor;
		ilink in_fld;
		ilink step_value;
		group zrot_xform<NEportLevels={0,1}> {
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
				angle => (((50.- (<-.<-.step_value))/25) * 45.*3.1415926/180.0);
			};
		};
		MODS.set_xform set_xform {
			in_field1 => <-.in_fld;
			in_field2 => <-.zrot_xform;
		};
		MODS.scale scale {
			in_field => <-.set_xform.out_fld;
			ScaleParam {
				scale_x => <-.<-.scale_factor;
				scale_y => <-.<-.scale_factor;
			};
			ScaleUI {
				UIpanel UIpanel;
			};
		};
		olink out_obj => scale.out_obj;
 	};

	macro arc_grid {
		float+IPort2 dims[2];
		float+IPort2 p1[2];
		float+IPort2 p2[2];

		Mesh_Cyl_Unif CylMesh<NEportLevels={0,1}> {
			nspace = 2;
			ndim = 2;
			dims => <-.dims;
			points => {<-.p1,<-.p2};
		};
		GDM.DataObject DataObject {
			in => <-.CylMesh;
			Modes {
				mode = {0,2,1,0,0};
				normals = "None";
			};
		};
		olink out_obj => DataObject.obj;
	};

	// dual color dial face, with top end in red - danger zone
	macro arc_solid_2b {
		Mesh_Cyl_Unif CylMesh_lo<NEportLevels={0,1}> {
			nspace = 2;
			ndim = 2;
			float a1<NEportLevels={2,0}> = 1.5;
			float a2<NEportLevels={2,0}> = 3.1416;
			dims = {2,48};
			points => {
				{1.,1.},
				{.a1,.a2}
			};
		};
		GDM.DataObject obj_lo {
			in => <-.CylMesh_lo;
			Modes {
				mode = {0,2,2,0,0};
				normals = "None";
			};
			Props {
				col = {0.4233,0.537176,0.83};
				inherit = 0;
			};
		};
		Mesh_Cyl_Unif CylMesh_hi<NEportLevels={0,1}> {
			nspace = 2;
			ndim = 2;
			float a1<NEportLevels={2,0}> = 1.5;
			float a2<NEportLevels={2,0}> = 1.;
			dims = {2,22};
			points => {
				{1.,0.},
				{.a1,.a2}
			};
		};
		GDM.DataObject obj_hi {
			in => <-.CylMesh_hi;
			Modes {
				mode = {0,2,2,0,0};
				normals = "None";
			};
			Props {
				col = {0.97,0.,0.};
				inherit = 0;
			};
		};
		GDM.GroupObject GroupObject {
			child_objs => {
				<-.obj_lo.obj,
				<-.obj_hi.obj
			};
			obj<NEportLevels={1,3}>;
		};
	};


	//////// Added by Ma YingLiang, 21st Feb. 2002//////


	macro DashGaugeA {
		int visible<NEportLevels={2,1}> = 1;
		ilink parent;
		float step_value<NEportLevels={2,1}> = 0.04;
		string title => name_of(<-,1);
		UIpanel UIpanel {
			visible => <-.visible;
			parent => <-.parent;
		};
		mk_needle mk_needle;
		transform_needle transform_needle {
			scale_factor =0.8;
			in_fld => <-.mk_needle.out_fld;
			step_value => <-.step_value;
		};
		dash_camera dash_camera {
			Top2D {
				child_objs => {
					<-.<-.arc_solid_2b.GroupObject.obj,
					<-.<-.mk_dashtext.out_obj,
					<-.<-.transform_needle.out_obj
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
		mk_dashtext mk_dashtext {
			mesh {
				coordinates<NEdisplayMode="open"> {
					values = {
						4.2,0.,
						4.,3.2,
						0.,5.,
						-4.8,3.2,
						-5.7,0.
					};
				};
			};
			in_text = {
				"100","75","50","25","0"
			};
			obj{
				Props{
					col = {0.0,1.0,0.0};
				};
			};
		};
		arc_solid_2b arc_solid_2b {
			CylMesh_lo {
				dims = {2,24};
				points => {
					{5.,1.},
					{.a1,.a2}
				};
				a1 = 2.5;
			};
			CylMesh_hi {
				dims = {2,12};
				points => {
					{5,0.},
					{.a1,.a2}
				};
				a1 = 2.5;
			};
		};
	};

	macro DashGaugeB {
		int visible<NEportLevels={2,1}> = 1;
		ilink parent;
		float step_value<NEportLevels={2,1}> = 0.06;
		string title => name_of(<-,1);

		UIpanel UIpanel {
			visible => <-.visible;
			parent => <-.parent;
		};
		mk_needle mk_needle;
		transform_needle transform_needle {
			scale_factor = 0.7;
			in_fld => <-.mk_needle.out_fld;
			step_value => <-.step_value;
		};
		dash_camera dash_camera {
			Top2D {
				child_objs => {
					<-.<-.arc_grid.out_obj,
					<-.<-.mk_dashtext.out_obj,
					<-.<-.transform_needle.out_obj
				};
			};
			DefaultView {
				back_col = {0.8,0.8,0.8};
			};
			UIframe {
				parent => <-.<-.UIpanel;
			};
			UIlabel {
				label => <-.<-.title;
			};
		};

		mk_dashtext mk_dashtext {
			mesh<NEwidth=495.,NEheight=242.> {
				nnodes = 7;
				coordinates<NEdisplayMode="open"> {
					values = {
						4.2,-3.,
						4.2,0.,
						4.,3.2,
						0.,5.,
						-4.8,3.2,
						-5.7,0., 
						-5.7,-3.
					};
				};
			};
			in_text = {
				"125","100","75","50","25","0","-25"
			};
			obj{
				Props {
					col = {0.2,0.1,0.8};
				};
			};
		};

		//  wire frame grid
		arc_grid arc_grid {
			dims = {2,24};
			p1 = {4.1,-1.0};
			p2 = {1.3,4.1416};
			DataObject {
				Props {
					col = {0.,0.,0.};
				};
			};
		};

	};


	macro DashGaugeC {
		int visible<NEportLevels={2,1}> = 1;
		ilink parent;
		float step_value<NEportLevels={2,1}> = 0.04;
		string title => name_of(<-,1);
		UIpanel UIpanel {
			visible => <-.visible;
			parent => <-.parent;
		};
		mk_needle mk_needle;
		transform_needle transform_needle {
			scale_factor = 0.8;
			in_fld => <-.mk_needle.out_fld;
			step_value => <-.step_value;
		};
		mk_dashtext dashtext {
			mesh<NEx=154.,NEy=55.> {
				coordinates<NEdisplayMode="open"> {
					values = {
						4.2,0.,
						4.,3.2,
						0.,5.,
						-4.8,3.2,
						-5.7,0.
					};
				};
			};
			in_text = {
				"100","75","50","25","0"
			};
			obj {
				Props.col => { 0.9, 0.1, 0.1 };
			};
		};

		//  wire frame grid
		arc_grid arc_grid {
			dims = {2,24};
			p1 = {5.0,0.0};
			p2 = {1.5,3.1416};
		};
		dash_camera dash_camera {
			Top2D {
				child_objs => {
					<-.<-.arc_grid.out_obj,
					<-.<-.dashtext.out_obj,
					<-.<-.transform_needle.out_obj
				};
			};
			DefaultView {
				back_col = {0.2,0.2,0.6};
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


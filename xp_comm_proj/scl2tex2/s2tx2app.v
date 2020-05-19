
flibrary Scalar2Texture2Apps
{

	APPS.SingleWindowApp Scalar2Texture2Eg {

		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.Scalar2Texture2.out_obj,
						<-.<-.<-.isoline.out_obj
					};
				};
			};
		};

		MODS.Read_UCD Read_UCD {
			read_ucd_ui {
				filename = "$XP_PATH<0>/data/ucd/yoke.inp";
			};
		};

		MODS.Read_Image Read_Image {
			read_image_ui {
				file_browser {
					filename = "$XP_PATH<1>/xp_comm_proj/scl2tex2/rainbow.jpg";
				};
			};
		};

		MODS.extract_scalar extract_scalar {
			in_field => <-.Read_UCD.field;
			ExtractScalarParam {
				vector = 2;
				component = 0;
			};
		};

		XP_COMM_PROJ.Scalar2Texture2.Scalar2Texture2Macs.Scalar2Texture2 Scalar2Texture2 {
			in_fld => <-.extract_scalar.out_fld;
			in_image => <-.Read_Image.field;
		};

		MODS.external_faces external_faces {
			in_field => <-.extract_scalar.out_fld;
		};

		MODS.isoline isoline {
			in_field => <-.external_faces.out_fld;
			IsoParam {
				level_min => -100;
				level_max => 100;
				color = 0;
			};
		};

	};



	Scalar2Texture2Eg s2tComparison2Eg {

		Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.BreakableLink1.out,
						<-.<-.<-.BreakableLink2.out,
						<-.<-.<-.isoline.out_obj
					};
				};
			};
		};

		XP_COMM_PROJ.Common.UIWidgets.IAC_StandardUI Scalar2Texture2EgUI {
			panel {
				parent+IPort3;
				message = "Select ComparisonEg control panel.";
			};
			mainTitleLabel {
				label = "Texture/Colormap Toggle";
			};
			UIbutton Toggle_Coloring {
				parent => <-.panel;
				do+OPort3;
			};
		};

		XP_COMM_PROJ.Common.LogicWidgets.Toggle Toggle {
			trigger => <-.Scalar2Texture2EgUI.Toggle_Coloring.do;
		};

		XP_COMM_PROJ.Common.NetworkWidgets.BreakableLink BreakableLink1 {
			in => <-.Scalar2Texture2.out_obj;
			connect => <-.Toggle.value_true;
			disconnect => <-.Toggle.value_false;
			BreakLinkCore {
				out => in;
			};
		};
		XP_COMM_PROJ.Common.NetworkWidgets.BreakableLink BreakableLink2 {
			in => <-.extract_scalar.out_obj;
			connect => <-.Toggle.value_false;
			disconnect => <-.Toggle.value_true;
			BreakLinkCore {
				out => in;
			};
		};

	};

	APPS.SingleWindowApp Datamap2TextureEg {
		float+OPort points [10][3] = {
			0.051103,-0.0073453,-0.15,
			0.05,0.,-0.15,
			0.05,0.,-0.14167,
			0.052359,-0.0018363,-0.14792,
			0.050552,-0.0036726,-0.15,
			0.05,0.,-0.14583,
			0.050552,-0.0036726,-0.14583,
			0.05118,-0.0009182,-0.14896,
			0.05118,-0.0009182,-0.14479,
			0.051731,-0.0045908,-0.14896
		};

		int+OPort tet_connect [4] = {0,1,2,3};
		FLD_MAP.tet_mesh tet_mesh {
			coord => <-.points;
			connect => <-.tet_connect;
			out {
				xform {
					xlate = {0.00243589,-0.000509381,0.0168606};
				};
			};
		};

		int+OPort tet2_connect [10] = {0,1,2,3,4,6,9,5,8,7};
		FLD_MAP.tet2_mesh tet2_mesh {
			coord => <-.points;
			connect => <-.tet2_connect;
			out {
				xform {
					xlate = {0.00548719,-0.00981199,0.00397544};
				};
			};
		};

		FLD_MAP.node_scalar node_scalar {
			in_data = {0,0,2,2,0,1,1,1,2,1};
		};

		FLD_MAP.combine_mesh_data tet_node_data {
			in_mesh => <-.tet_mesh.out;
			in_nd => <-.node_scalar.out;
		};

		FLD_MAP.combine_mesh_data tet2_node_data {
			in_mesh => <-.tet2_mesh.out;
			in_nd => <-.node_scalar.out;
		};

		XP_COMM_PROJ.Dmap2Image.Dmap2ImageMacs.Dmap2ImageSquare Dmap2ImageSquare {
			dmap_in => <-.Datamap;
		};
		XP_COMM_PROJ.Scalar2Texture2.Scalar2Texture2Macs.Scalar2Texture2 Scalar2Texture2_tet {
			in_fld => <-.tet_node_data.out;
			in_image => <-.Dmap2ImageSquare.out_fld;
		};
		MODS.reset_xform reset_xform_tet {
			in_field => <-.tet_node_data.out;
			xfm {
				xform {
					xlate = {-0.000170365,0.008758,0.00979046};
				};
			};
		};

		XP_COMM_PROJ.Scalar2Texture2.Scalar2Texture2Macs.Scalar2Texture2 Scalar2Texture2_tet2 {
			in_fld => <-.tet2_node_data.out;
			in_image => <-.Dmap2ImageSquare.out_fld;
		};
		MODS.reset_xform reset_xform_tet2 {
			in_field => <-.tet2_node_data.out;
		};

		FLD_MAP.point_mesh point_mesh {
			coord => <-.points;
			DataObject {
				Props {
					col = {1.,0.,0.};
					inherit = 0;
					line_width = 12;
					subdiv = 20;
					dir_pt_size = 2.;
				};
			};
		};

		GDM.DataObject.Datamap Datamap{
			dataMin=>0;
			dataMax=>1;
		};
		
		string+OPort string [10] = {"0","1","2","3","4","5","6","7","8","9"};
		MODS.text_glyph text_glyph {
			in_mesh => <-.point_mesh.out;
			in_text => <-.string;
			TextUI {
				Offset {
					set = 1;
				};
				Leadline {
					set = 1;
				};
			};
		};

		int+OPort line_connect [12] = {0,1,1,2,2,3,3,0,1,3,0,2};
		FLD_MAP.line_mesh line_mesh {
			coord => <-.points;
			connect => <-.line_connect;
		};

		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.Scalar2Texture2_tet.out_obj,
						<-.<-.<-.reset_xform_tet.out_obj,
						<-.<-.<-.Scalar2Texture2_tet2.out_obj,
						<-.<-.<-.reset_xform_tet2.out_obj,
						<-.<-.<-.line_mesh.obj,
						<-.<-.<-.point_mesh.obj,
						<-.<-.<-.text_glyph.out_obj
					};
					Xform {
						ocenter = {0.0538379,-0.00419965,-0.137405};
						dcenter = {18.2659,-1.99109,-49.0859};
						mat = {
								  -64.6573,-70.0893,283.769,0.,
								  -81.1923,283.497,51.5216,0.,
								  -280.794,-65.8352,-80.2401,0.,
								  0.,0.,0.,1.
						};
						xlate = {-18.0832,1.9566,49.4646};
						center = {0.0538379,-0.00419965,-0.137405};
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
	};

};


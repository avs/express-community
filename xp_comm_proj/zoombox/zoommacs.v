flibrary ZoomBoxMacs<compile_subs=0>
{
	macro ZoomBoxFunc {
		link camera => view.picked_camera;
		ilink obj;
		link rv => view.render_view;
		UItwoPoint be {
			view => <-.rv;
			startEvent = "Shift<Btn1Down>";
			runEvent = "Shift<Btn1Motion>";
			stopEvent = "Shift<Btn1Up>";
		};
		GDmap2d_ss screen_center_coords {
			in_buf => <-.screen_center;
			view_in => <-.view;
			cam_in => <-.camera;
			//obj_in => <-.obj;
		};
		GDmap2d_ss box_coords_world {
			in_buf => <-.box.out_buf;
			view_in => <-.view;
			cam_in => <-.camera;
			//			obj_in => <-.obj;
		};
		GDmap2d_ss box_coords_obj {
			in_buf => <-.box.out_buf;
			view_in => <-.view;
			cam_in => <-.camera;
			obj_in => <-.obj;
			upd_func<weight=2>; //Need this to run after box_coords_world				
		};
		GDroi2d_cont box {
			state => <-.be.state;
			view_in => <-.view;
			x => <-.be.x;
			y => <-.be.y;
			mode = "Clear";
			option = "Box";
			immed = "On Button Up";
			draw_mode = "XOR";
		};
		GDroi2d_buffer screen_center<NEportLevels={0,1}> {
			pointbuf => {{rv.width / 2, rv.height / 2}};
			npoints = 1;
		};

		float box_xmin_world => box_coords_world.out_mesh.coordinates.values[0][0];
		float box_xmax_world => box_coords_world.out_mesh.coordinates.values[1][0];
		float box_ymin_world => box_coords_world.out_mesh.coordinates.values[0][1];
		float box_ymax_world => box_coords_world.out_mesh.coordinates.values[2][1];
		float box_center_x_world => (.box_xmin_world + box_xmax_world) / 2.;
		float box_center_y_world => (.box_ymin_world + .box_ymax_world)/ 2.;

		float box_xmin_obj => box_coords_obj.out_mesh.coordinates.values[0][0];
		float box_xmax_obj => box_coords_obj.out_mesh.coordinates.values[1][0];
		float box_ymin_obj => box_coords_obj.out_mesh.coordinates.values[0][1];
		float box_ymax_obj => box_coords_obj.out_mesh.coordinates.values[2][1];
		float box_center_x_obj => (.box_xmin_obj + .box_xmax_obj) / 2.;
		float box_center_y_obj => (.box_ymin_obj + .box_ymax_obj) / 2;

		float screen_center_x => 
			screen_center_coords.out_mesh.coordinates.values[0][0];
		float screen_center_y =>
			screen_center_coords.out_mesh.coordinates.values[0][1];

		float dx => (.screen_center_x - .box_center_x_world);
		float dy => (.screen_center_y - .box_center_y_world);

		float sdx => .dx * (xform.mat[0][0] + xform.mat[0][1] + xform.mat[0][2]);
		float sdy => .dy * (xform.mat[1][0] + xform.mat[1][1] + xform.mat[1][2]);

		DefaultXform+nres &xform<NEportLevels={1,0}> => obj.xform;
		GMOD.parse_v do_xform {
			v_commands = "screen_center.npoints = 1; do_xlate.trigger = 1;";
			trigger => <-.box_coords_obj.out_mesh.nnodes;
			active => <-.box_coords_obj.out_mesh.nnodes;
			on_inst = 0;
			relative => ".";
		};
		GMOD.parse_v do_xlate {
			v_commands = "f0 = xform.xlate[0] + dx; 
										f1 => xform.xlate[0]; 
										f1 = f0;
										f0 = xform.xlate[1] + dy;
										f1 => xform.xlate[1];
										f1 = f0;
										do_center.trigger = 1;
										";
			on_inst = 0;
			relative => ".";
		};
		GMOD.parse_v do_center {
			v_commands = "f1 => xform.center[0]; 
										f1 = box_center_x_obj;
										f1 => xform.center[1];
										f1 = box_center_y_obj;
										do_scale.trigger = 1;";
			on_inst = 0;
			relative => ".";
			};
		GMOD.parse_v do_scale {
			v_commands = "f0 = xform.mat[0][0]/scale;
										f1 => xform.mat[0][0];
										f1 = f0;
										f0 = xform.mat[0][1]/scale;
										f1 => xform.mat[0][1];
										f1 = f0;
										f0 = xform.mat[0][2]/scale;
										f1 => xform.mat[0][2];
										f1 = f0;
										f0 = xform.mat[1][0]/scale;
										f1 => xform.mat[1][0];
										f1 = f0;
										f0 = xform.mat[1][1]/scale;
										f1 => xform.mat[1][1];
										f1 = f0;
										f0 = xform.mat[1][2]/scale;
										f1 => xform.mat[1][2];
										f1 = f0;
										f0 = xform.mat[2][0]/scale;
										f1 => xform.mat[2][0];
										f1 = f0;
										f0 = xform.mat[2][1]/scale;
										f1 => xform.mat[2][1];
										f1 = f0;
										f0 = xform.mat[2][2]/scale;
										f1 => xform.mat[2][2]; 
										f1 = f0;";
			on_inst = 0;
			relative => ".";
		};
		float f0;
		float f1;
		short box_pix_xmin => box.out_buf.boxbuf[0];
		short box_pix_xmax => box.out_buf.boxbuf[2];
		short box_pix_ymin => box.out_buf.boxbuf[1];
		short box_pix_ymax => box.out_buf.boxbuf[3];
		short dbox_xpix => abs((.box_pix_xmax - .box_pix_xmin));
		short dbox_ypix => abs((.box_pix_ymax - .box_pix_ymin));
		short rv_width => rv.width;
		short rv_height => rv.height;
		float scalex => ((1. * .dbox_xpix) / (1. * .rv_width));
		float scaley => ((1. * .dbox_ypix) / (1. * .rv_height));
		float scale => max_array({0.01,.scalex, .scaley});
		DefaultView &view<NEportLevels={2,1}> {
			UIrenderView render_view {
				visible => ;
				height => ;
				width => ;
				x => ;
			};
		};
	};
	
	macro ZoomBox
	{
		ilink+nres obj => <-.Uviewer3D.Scene.Top.obj;
		ilink+nres view => <-.Uviewer3D.Scene_Selector.curr_view;
		XP_COMM_PROJ.ZoomBox.ZoomBoxMacs.ZoomBoxFunc ZoomBoxFunc
		{
			view => <-.view;
			obj => <-.obj;
		};
	};
	
};

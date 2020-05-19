
flibrary ConfocalMacs <compile_subs=0> {
	macro Axis {
	   macro X_axis {
		  DVaxis DVaxis {
			 in+IPort3 => <-.<-.Data;
			 axis_name = "X Axis";
			 limits => {
				in.coordinates.min_vec[0],in.coordinates.max_vec[0]};
			 reference => <-.axis_param.x_ref;
			 tick_step => ((limits[1] - limits[0]) / 4);
			 tick_y_below = 10.;
			 tick_y_above = 0.;
			 tick_z_below = 10.;
			 tick_z_above = 0.;
			 minor_ticks = 1;
			 minor_scale = 1.;
			 ndig = 1;
			 y_anno = -50.;
			 z_anno = 0.;
			 ntick = 6;
		  };
		  DefaultObject+OPort2 X_axis {
			 input => <-.DVaxis.out;
			 objects => {<-.DefaultObject#1,
				<-.DefaultObject#2,<-.DefaultObject#3};
			 xform => <-.DVconcat_xform.xform_out;
		  };
		  DefaultObject+OPort DefaultObject#1 {
			 input => <-.DVaxis.out_minor;
		  };
		  DefaultObject+OPort DefaultObject#2 {
			 input => <-.DVaxis.out_anno;
		  };
		  DefaultObject+OPort DefaultObject#3 {
			 input => <-.DVaxis.out_name;
		  };
		  group+IPort2 &axis_param {
			 float+Port x_ref;
			 float+Port y_ref;
			 float+Port z_ref;
		  } => <-.axis_param;
		  DVconcat_xform DVconcat_xform {
			 xform_in1 => DVaxis.in.xform;
			 xform_in2 => <-.y_xform;
		  };
		  DefaultXform+nonotify y_xform {
			 mat => {{1,0,0,0},
				{0,1,0,0},
				{0,0,1,0},
				{0,0,0,1}};
			 xlate => {0.,
				<-.axis_param.y_ref,<-.axis_param.z_ref};
		  };
	   };
	   DataObject DataObject {
		  child_objs => {<-.X_axis.X_axis,
			 <-.Y_axis.X_axis,<-.Z_axis.X_axis};
		  obj<NEportLevels={1,3}>;
	   };
	   macro Y_axis {
		  DVaxis DVaxis {
			 in+IPort3 => <-.<-.Data;
			 axis_name = "Y Axis";
			 limits => {
				in.coordinates.min_vec[1],in.coordinates.max_vec[1]};
			 reference => <-.axis_param.y_ref;
			 tick_step => ((limits[1] - limits[0]) / 4);
			 tick_y_below = 0.;
			 tick_y_above = 10.;
			 tick_z_below = 10.;
			 tick_z_above = 0.;
			 minor_ticks = 1;
			 minor_scale = 1.;
			 ndig = 1;
			 y_anno = 50.;
			 z_anno = 0.;
			 ntick = 7;
		  };
		  DefaultObject+OPort2 X_axis {
			 input => <-.DVaxis.out;
			 objects => {<-.DefaultObject#1,
				<-.DefaultObject#2,<-.DefaultObject#3};
			 xform => <-.DVconcat_xform.xform_out;
		  };
		  DefaultObject+OPort DefaultObject#1 {
			 input => <-.DVaxis.out_minor;
		  };
		  DefaultObject+OPort DefaultObject#2 {
			 input => <-.DVaxis.out_anno;
		  };
		  DefaultObject+OPort DefaultObject#3 {
			 input => <-.DVaxis.out_name;
		  };
		  DVconcat_xform DVconcat_xform {
			 xform_in1 => DVaxis.in.xform;
			 xform_in2 => <-.y_xform;
		  };
		  DefaultXform+nonotify y_xform {
			 mat => {{0,1,0,0},
				{-1,0,0,0},
				{0,0,1,0},
				{0,0,0,1}};
			 xlate => {
				<-.axis_param.x_ref,0.,<-.axis_param.z_ref};
		  };
		  group+IPort2 &axis_param {
			 float+Port x_ref;
			 float+Port y_ref;
			 float+Port z_ref;
		  } => <-.axis_param;
	   };
	   macro Z_axis {
		  DVaxis DVaxis {
			 in+IPort3 => <-.<-.Data;
			 axis_name = "Z Axis";
			 limits => {
				in.coordinates.min_vec[2],in.coordinates.max_vec[2]};
			 reference => <-.axis_param.z_ref;
			 tick_step => (limits[1] - limits[0]);
			 tick_y_below = 10.;
			 tick_y_above = 0.;
			 tick_z_below = 0.;
			 tick_z_above = 10.;
			 minor_ticks = 1;
			 minor_scale = 1.;
			 ndig = 1;
			 y_anno = -50.;
			 z_anno = 50.;
			 ntick = 2;
		  };
		  DefaultObject+IPort2 X_axis {
			 input => <-.DVaxis.out;
			 objects => {<-.DefaultObject#1,
				<-.DefaultObject#2,<-.DefaultObject#3};
			 xform => <-.DVconcat_xform.xform_out;
		  };
		  DefaultObject+OPort DefaultObject#1 {
			 input => <-.DVaxis.out_minor;
		  };
		  DefaultObject+OPort DefaultObject#2 {
			 input => <-.DVaxis.out_anno;
		  };
		  DefaultObject+OPort DefaultObject#3 {
			 input => <-.DVaxis.out_name;
		  };
		  DVconcat_xform DVconcat_xform {
			 xform_in1 => DVaxis.in.xform;
			 xform_in2 => <-.z_xform;
		  };
		  DefaultXform+nonotify z_xform {
			 xlate => {
				<-.axis_param.x_ref,<-.axis_param.y_ref,0.};
			 mat => {{0,0,1,0},
				{0,1,0,0},
				{-1,0,0,0},
				{0,0,0,1}};
		  };
		  group+IPort2 &axis_param {
			 float+Port x_ref;
			 float+Port y_ref;
			 float+Port z_ref;
		  } => <-.axis_param;
	   };
	   group+OPort axis_param {
		  float+Port x_ref => Data.coordinates.min_vec[0];
		  float+Port y_ref => Data.coordinates.min_vec[1];
		  float+Port z_ref => Data.coordinates.min_vec[2];
	   };
	   Mesh+IPort2 &Data;
	};


	macro my_crop {
	   DVcrop DVcrop {
		  in => <-.in_field;
		  min => param.min;
		  max => param.max;
		  DV_Param_crop+IPort2 &param => <-.CropParam;
	   };
	   olink out_fld => .DVcrop.out;
	   DV_Param_crop+OPort2 CropParam {
		  min+nres+Port2 => init_array(in_field.ndim,0,0);
		  max+nres+Port2 => (in_field.dims - 1);
	   };
	   Mesh_Struct+Node_Data+read+req+IPort2 &in_field {
		  nnodes+req;
	   };
};
};

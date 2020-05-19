
flibrary BoundsLiteMacs {

	group BoundsLiteParams
	{
	   int+Port2 hull = 1;
	};

	macro BoundsLiteUI{
		XP_COMM_PROJ.BoundsLite.BoundsLiteMacs.BoundsLiteParams+IPort2 &params;
      	UImod_panel UImod_panel{
      		title => name_of(<-.<-.<-,1);
      	};
      	UItoggle hull_toggle{
      		parent => <-.UImod_panel;
         		label => "Hull";
         		set => <-.params.hull;
         		y = 0;
      	};
	};

	macro BoundsLiteFunc {
		XP_COMM_PROJ.BoundsLite.BoundsLiteMacs.BoundsLiteParams+IPort2 &params;
		Mesh+IPort2 &Mesh;
		double+notify_inst min[3] => Mesh.coordinates.min_vec;
		double+notify_inst max[3] => Mesh.coordinates.max_vec;
		FLD_MAP.line_mesh line_mesh {
			coord[8][3] => {
				min[0],min[1],min[2],
				min[0],max[1],min[2],
				min[0],max[1],max[2],
				min[0],min[1],max[2],

				max[0],min[1],min[2],
				max[0],max[1],min[2],
				max[0],max[1],max[2],
				max[0],min[1],max[2]
			};
			connect = {
				0,1,1,2,2,3,3,0,0,4,1,5,2,6,3,7,4,5,5,6,6,7,7,4
			};
		};
		MODS.set_xform set_xform {
			in_field1 => <-.line_mesh.out;
			in_field2 => <-.Mesh;
		};
		
		olink out => set_xform.out_fld;
		olink obj => switch(params.hull,set_xform.out_obj);
	};
	
	macro BoundsLite {
      	XP_COMM_PROJ.BoundsLite.BoundsLiteMacs.BoundsLiteUI BoundsLiteUI{
         		params => <-.params;
      	};
      	XP_COMM_PROJ.BoundsLite.BoundsLiteMacs.BoundsLiteFunc BoundsLiteFunc{
			params => <-.params;
         	Mesh => <-.in_field;
      	};
      	XP_COMM_PROJ.BoundsLite.BoundsLiteMacs.BoundsLiteParams+OPort params;
      	ilink in_field;
      	olink out_field => .BoundsLiteFunc.out;
      	olink out_obj => .BoundsLiteFunc.obj;
	};
};

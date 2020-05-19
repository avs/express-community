
flibrary ProfileMacs 
{
	// ProfileFunc
	// Functional Macro

macro ProfileFunc {
   XP_COMM_PROJ.Profile.ProfileMods.ProfileParams &params<NEportLevels={2,1}>;

   XP_COMM_PROJ.Profile.ProfileMods.profile profile {
      params => <-.params;
   };
   olink out;

   int+Port y_arr[]=>init_array(params.ny,params.ny-1,0);

   FLD_MAP.interleave_2_arrays interleave_2_arrays {
      in1 => <-.profile.line;
      in2 => <-.y_arr;
   };

   olink line => interleave_2_arrays.out;
   olink map => profile.map;

};

	// ProfileUI
	// User Interface Macro

	macro ProfileUI
	{
		XP_COMM_PROJ.Profile.ProfileMods.ProfileParams &params<NEportLevels={2,1}>;

		UImod_panel UImod_panel
		{
			title => name_of(<-.<-.<-,1);
      	};
   		UIslider Mode {
      		parent => <-.UImod_panel;
      		value => <-.params.mode;
      		x = 5;
       		width => (parent.clientWidth - 10);
      		min = 0.;
      		max = 2.;
      		mode = "integer";
   		};
   		UIframe UIframe
		{
			parent => <-.UImod_panel;
      		width => (parent.clientWidth - 10);
      		height => (parent.clientWidth - 10);
      		x = 5;
      		y => <-.Mode.y + <-.Mode.height + 10;
   		};
		UIpanel UIpanel {
			parent => <-.UIframe;
			width => parent.clientWidth;
			height => parent.clientHeight;
		};
		UIlabel UIlabel {
      		parent => <-.UIpanel;
      		label => "Draw here!";
      		x => ((<-.UIpanel.width / 2) - (width / 2));
      		y => ((<-.UIpanel.height / 2) - (height /2));
   		};
		UItwoPoint UItwoPoint
		{
			view => <-.UIpanel;
			x => <-.params.x;
			y => <-.params.y;
			state => <-.params.state;
		};
   		
	};


	// Profile
	// User macro

	macro Profile {
		XP_COMM_PROJ.Profile.ProfileMods.ProfileParams params{
			x = 1;
    		y = 1;
    		state = 1;
    		mode = 0;
			nx+nres => <-.ProfileUI.UIframe.width;
			ny => nx;

		};


		XP_COMM_PROJ.Profile.ProfileMacs.ProfileUI ProfileUI {
			params => <-.params;
		};

		XP_COMM_PROJ.Profile.ProfileMacs.ProfileFunc ProfileFunc {
			params => <-.params;
		};
		FLD_MAP.point_mesh point_mesh {
			coord => <-.ProfileFunc.line;
		};
		FLD_MAP.uniform_scalar_field uniform_scalar_field {
			mesh {
				in_dims => array_dims(<-.<-.ProfileFunc.map);
			};
			data {
				in_data => <-.<-.ProfileFunc.map;
			};
		};
		olink line_out => point_mesh.out;
		olink line_obj => point_mesh.obj;

		olink img_out => uniform_scalar_field.out;
		olink img_obj => uniform_scalar_field.obj;
	};

};

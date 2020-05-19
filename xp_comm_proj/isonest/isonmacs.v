
flibrary IsonestMacs <compile_subs=0> {

    macro fld2dmap{
#ifdef VIZ_EXPRESS
        ilink in;
        GDM.DataObject DataObject{
            in => <-.in;
        };
        olink Datamap => DataObject.Datamap;
#else
        group &in<NEportLevels={2,1}> {
            GDxform_templ &xform;
            method render;
        };
        DefaultMinMax MinMax{
            input => <-.in;
        };
        DefaultLinear Datamap<NEportLevels={0,2}> {
            dataMin => <-.MinMax.min_value;
            dataMax => <-.MinMax.max_value;
        };
#endif
             
    };

	// User Interface
	macro IsonestUI{
		ilink in_fld;
		XP_COMM_PROJ.Isonest.IsonestMods.IsonestParams+IPort2 &params;
#ifdef VIZ_EXPRESS
        macro DVnode_data_labels {
            Field &in => <-.in_fld;
            string labels[in.nnode_data] => in.node_data.labels;
        };
#else
		DVnode_data_labels DVnode_data_labels {
			in => <-.in_fld;
			int+nres  ncomp => in.nnode_data;
		};
#endif
       
		UI.UImod_panel UIpanel{
			title => name_of(<-.<-,1);
		};
		UIradioBoxLabel   UIradioBoxLabel{
			parent => <-.UIpanel;
			labels+IPort2 => DVnode_data_labels.labels;
			&selectedItem+IPort2 => params.num_component;
			visible+nres => <-.UIpanel.visible;
			title = "isosurface_nesting data component";
			y		=  0;
			width	=> <-.UIpanel.clientWidth;
		};
		UIslider nleveles {
			parent => <-.UIpanel;
			title = "number of isosurface shells";
			min = 1;
			max = 8;
			mode = 1;
			value+IPort2 => params.num_levels;
			y => <-.UIradioBoxLabel.y + <-.UIradioBoxLabel.height + 4;
			width	=> <-.UIpanel.clientWidth * 11 / 12;
		};
		VUIslider_typein nleveles_typein {
			slider	=> nleveles;
		};
		UIslider iso_level_min {
			parent => <-.UIpanel;
			title = "min level";
			min+nres =>  cache(in_fld.node_data[params.num_component].min);
			max+nres =>  cache(in_fld.node_data[params.num_component].max);
			value+IPort2 => params.min;
			y		=> <-.nleveles.y + <-.nleveles.height + 4;
			width	=> <-.UIpanel.clientWidth * 11 / 12;
		};
		VUIslider_typein iso_level_min_typein {
			slider	=> iso_level_min;
		};
		UIslider iso_level_max {
			parent => <-.UIpanel;
			title = "max level";
			min+nres =>  cache(in_fld.node_data[params.num_component].min);
			max+nres =>  cache(in_fld.node_data[params.num_component].max);
			value+IPort2 => params.max;
			y		=> <-.iso_level_min.y + <-.iso_level_min.height + 4;
			width	=> <-.UIpanel.clientWidth * 11 / 12;
		};
		VUIslider_typein iso_level_max_typein {
			slider	=> iso_level_max;
		};
		UIslider trans_level {
			parent => <-.UIpanel;
			title = "transparency level";
			min = 0.0;
			max = 1.0;
			value+IPort2 => params.transparency;
			y		=> <-.iso_level_max.y + <-.iso_level_max.height + 4;
			width	=> <-.UIpanel.clientWidth * 11 / 12;
		};
		VUIslider_typein trans_level_typein {
			slider	=> trans_level;
		};
		UItoggle trans_ramp_toggle {
			parent => <-.UIpanel;
			label = "Transparency Progressive Ramp";
			&set+IPort2 => params.trans_ramp;
			y		=> <-.trans_level.y + <-.trans_level.height + 4;
			width => <-.UIpanel.clientWidth;
		};
		UIslider trans_range {
			parent => <-.UIpanel;
			title = "transparency range delta";
			min = 0.0;
			max = 1.0;
			value+IPort2 => params.ramp_delta;
			y		=> <-.trans_ramp_toggle.y + <-.trans_ramp_toggle.height + 4;
			width	=> <-.UIpanel.clientWidth * 11 / 12;
			active => <-.trans_ramp_toggle.set;
		};
		VUIslider_typein trans_range_typein {
			slider	=> trans_range;
		};
	}; // end of user interface


	 // Functional macro 
	 macro IsonestFunc {
			  ilink fld_in;
			  XP_COMM_PROJ.Isonest.IsonestMods.IsonestParams+IPort2 &params;
			  float values<NEportLevels={1,1}>[params.num_levels] =>
				  init_array ( params.num_levels, 
							   params.min,
							   params.max );
			  float trans_vals<NEportLevels={1,1}>[params.num_levels] =>
				  init_array ( params.num_levels, 
							   (params.transparency -
								 ( params.ramp_delta*0.5 ) * params.trans_ramp),
							   (params.transparency +
								 ( params.ramp_delta*0.5 ) * params.trans_ramp) );
			  fld2dmap fld2dmap {
					  in => <-.fld_in;
			  };
			  XP_COMM_PROJ.Isonest.IsonestMods.Dmap2Color Dmap2Color {
					  values => <-.values;
					  dmap => <-.fld2dmap.Datamap;
					  X_Resolution => <-.params.num_levels;
			  };
			  float color_values<NEportLevels={1,1}>[params.num_levels*3]=>Dmap2Color.output;
#ifdef VIZ_EXPRESS
              MODS.isosurface isosurface[params.num_levels]{
			      int index => index_of(isosurface);
                  UIpanel UIpanel;
                  Iso {
                  	  DViso {
				          int index => <-.<-.index;
                          in => <-.<-.<-.fld_in;
				          level => <-.<-.<-.values[index];
				          component => <-.<-.<-.params.num_component;
				          GDM.DataObjectLite dataObj {
					          in => <-.out;
					          Props.trans => <-.<-.<-.<-.<-.trans_vals[<-.<-.index];
							  Props.inherit=>0;
					          obj<NEportLevels={0,3}>;
					          Obj.xform_mode = "Parent";
					          Obj.name => "shell "+ <-.<-.index;
					          Props.col => { <-.<-.<-.<-.<-.color_values[<-.<-.index*3],
									         <-.<-.<-.<-.<-.color_values[<-.<-.index*3+1],
									         <-.<-.<-.<-.<-.color_values[<-.<-.index*3+2] };
                          };
				      };
    			  };
              };
              mlink isoObj => isosurface.Iso.DViso.dataObj.obj; 
#else
			  DViso DViso[params.num_levels] {
				  int index => index_of(DViso);
				  in => <-.fld_in;
				  level => <-.values[index];
				  component => <-.params.num_component;
				  GDM.DataObjectLite dataObj {
					  in => <-.out;
					  Props.trans => <-.<-.<-.trans_vals[<-.<-.index];
					  Props.inherit=>0;
					  obj<NEportLevels={0,3}>;
					  Obj.xform_mode = "Parent";
					  Obj.name => "shell "+ <-.<-.index;
					  Props.col => { <-.<-.<-.color_values[<-.<-.index*3],
									 <-.<-.<-.color_values[<-.<-.index*3+1],
									 <-.<-.<-.color_values[<-.<-.index*3+2] };
				  };
			  };
              mlink isoObj => DViso.dataObj.obj; 
#endif
			  XP_COMM_PROJ.Isonest.IsonestMods.group_swap_order gd_list_swap_order{
				  in_groups => <-.isoObj;
			  };
			  GDM.GroupObject GroupObjectA {
				   child_objs+nres => <-.isoObj;
				   Props.cull = "Front";
				   Top.xform_mode = "Parent";
			  };
			  GDM.GroupObject GroupObjectB {
				   child_objs+nres => <-.gd_list_swap_order.out_groups;
				   Props.cull = "Back";
				   Top.xform_mode = "Parent";
			  };
			  GDM.GroupObject GroupObjectOut {
				   child_objs+nres => { GroupObjectA.obj, GroupObjectB.obj };
				   Top.xform_mode = "Parent";
				   Top.dmap<NEportLevels={3,0}> => <-.<-.fld2dmap.Datamap;
			  };
			  olink obj => GroupObjectOut.obj;
		};

	// isosurface_nesting
	// User Macro 

	macro isosurface_nesting {
		Mesh+Node_Data &in_field<NEportLevels={2,1}>; 
		XP_COMM_PROJ.Isonest.IsonestMods.IsonestParams isonest_params {
			min+nres =>  cache(0.7*<-.fld_in.node_data[num_component].min+
								   0.3*<-.fld_in.node_data[num_component].max);
			max+nres =>  cache(0.1*<-.fld_in.node_data[num_component].min+
								   0.9*<-.fld_in.node_data[num_component].max);
			int num_levels = 3;       
			int num_component = 0;
			float transparency = 0.5; 
			int   trans_ramp = 1;
			float ramp_delta = 0.3;
		};
		IsonestFunc IsonestFunc {
			fld_in => in_field;
			params => <-.isonest_params;
		};
		olink obj => IsonestFunc.obj; 
		IsonestUI IsonestUI {
			in_fld => <-.in_field;
			params => <-.isonest_params;
			UImod_panel {
				parent<NEportLevels={4,0}>;
				title => name_of(<-.<-.<-);
				message = "Select isosurface_nesting control panel.";
		  	};
		};
	}; 

}; 


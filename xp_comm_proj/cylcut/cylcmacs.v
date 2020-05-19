flibrary CylinderCutMacs <compile_subs=0> {

  macro CylinderCut {
	ilink in_fld<NEy=77,NEx=44>;
	ilink in_pln<NEy=44,NEx=110>;
	DV_Param_cut+IPort2 &param<NEy=22,NEx=407>;

	int+nres has_node_data => (is_valid(in_fld.nnode_data) && in_fld.nnode_data > 0);

	XP_COMM_PROJ.CylinderCut.CylinderCutMods.CylinderCutDist CylinderCutDist<NEy=154,NEx=99> {
		in => in_fld;
		plane => in_pln;
	};		
	DVcomb_comp DVcomb_comp<NEy=275,NEx=176> {
		comp1+nres=> switch(<-.has_node_data,init_array(in_fld.nnode_data, 0, in_fld.nnode_data-1));
		comp2={0};
		nd1 => switch(<-.has_node_data, in_fld);
		nd2 => switch(<-.has_node_data, CylinderCutDist.out);
	};
	DVcomb_mesh_and_data DVcomb_mesh_and_data {
		in_mesh => in_fld;
		in_nd => switch(<-.has_node_data+1, <-.CylinderCutDist.out, <-.DVcomb_comp.out);
	};
	DVcut DVcut<NEy=341,NEx=297> {
		in => DVcomb_mesh_and_data.out;
		cut_comp+nres => switch(<-.has_node_data+1, 0, in_fld.nnode_data);
		map_comp+nres => switch(<-.has_node_data, param.component);
		level => CylinderCutDist.dist+param.dist;
		&above => param.above;
		cell_data => param.cell_data;
	};

	olink out_fld<NEy=396,NEx=396> => DVcut.out;
  };


  macro CylinderCutUI<instanced=0> {
	ilink in_fld;
	DV_Param_cut+IPort2 &param;

	UImod_panel+IPort2 &CylinderUIpanel;
	DVnode_data_labels+IPort2 &DVnode_data_labels;
	DVcell_data_labels+IPort2 &DVcell_data_labels;

	UItoggle above_toggle {
	    parent => <-.CylinderUIpanel;
	    label = "Above";
	    &set+IPort2 => param.above;
	    y		= 0;
	    width	=> <-.CylinderUIpanel.width;
	};

	UIslider plane_dist {
	    parent => <-.CylinderUIpanel;
	    title = "cylinder radius";
	    min+nres+IPort2 => cache(-0.5*(magnitude(in_fld.coordinates.max_vec-
					      in_fld.coordinates.min_vec)));
	    max+nres+IPort2 => cache( 0.5*(magnitude(in_fld.coordinates.max_vec-
					      in_fld.coordinates.min_vec)));
	    value+IPort2 => param.dist;
	    y		=> <-.above_toggle.y + <-.above_toggle.height + 4;
	    width	=> <-.CylinderUIpanel.width;
	};
	UItoggle xform_toggle {
	    parent => <-.CylinderUIpanel;
	    label = "Cylinder Transform Editor";
	    y		=> <-.plane_dist.y + <-.plane_dist.height + 4;
	    width	=> <-.CylinderUIpanel.width;
	};
	UIoptionBoxLabel  UIoptionBoxLabel{
	    parent => <-.CylinderUIpanel;
	    labels+IPort2 => DVnode_data_labels.labels;
	    max => DVnode_data_labels.ncomp;
	    &selectedItems+IPort2 => param.component;
	    title = "map components";
	    visible => <-.CylinderUIpanel.visible;
	    y		=> <-.xform_toggle.y + <-.xform_toggle.height + 4;
	    width	=> <-.CylinderUIpanel.width;
	};
	UIoptionBoxLabel  UIoptionBoxLabel_cell{
	    parent => <-.CylinderUIpanel;
	    labels+IPort2 => DVcell_data_labels.labels;
	    max => DVcell_data_labels.ncomp;
	    &selectedItems+IPort2 => param.cell_data;
	    title = "map cell components";
	    visible => <-.CylinderUIpanel.visible & (DVcell_data_labels.ncomp > 0);
	    y		=> <-.UIoptionBoxLabel.y + <-.UIoptionBoxLabel.height + 4;
	    width	=> <-.CylinderUIpanel.width;
	};
	XformEditor plane_xform {
	    vis => xform_toggle.set;
	};
  };


  macro cylinder_cut {
    ilink in_field<export_all=1>;

    DV_Param_cut+Port CutParam<export_all=2> {
    	component = {0};
	    dist = 0.0;
    	above = 0;
	    cell_data = {0};
    };

    int has_cell_data => (DVcell_data_labels.ncomp > 0);
 
    float+nres xlate[3]=>{0.0, 0.0,
		    (in_field.coordinates.min_vec[2]+in_field.coordinates.max_vec[2])/2};


    Plane_Mesh plane {
    	xform {
	        xlate+nres=>cache(<-.<-.xlate);
    	};
	    dims => {2, 2};
    	points+nres => {in_field.coordinates.min_vec[0], in_field.coordinates.min_vec[1], 
	    	  in_field.coordinates.max_vec[0], in_field.coordinates.max_vec[1]};
    };

    CylinderCut CylinderCut {
    	param => CutParam;
	    in_fld => in_field;
    	in_pln => <-.plane;
	    DVcut {
	        cell_data => switch(has_cell_data, param.cell_data);
    	};
    };


    DVnode_data_labels DVnode_data_labels {
	    in => in_field;
	    int+nres  ncomp => in.nnode_data;
    };
    DVcell_data_labels DVcell_data_labels {
	    in => in_field;
    };	


    GMOD.instancer instancer {
    	Value => CylindercutUIpanel.visible;
	    active = 2; // don't de-instance when visible = 0
    	Group => <-.CylinderCutUI;
    };

    UImod_panel CylindercutUIpanel {
    	parent<NEportLevels={3,0}>;
	    title => name_of(<-.<-);
    	message = "Select Cut control panel.";
    };

    CylinderCutUI CylinderCutUI<instanced=0> {
    	in_fld => in_field;
	    &param => CutParam;
    	CylinderUIpanel  => <-.CylindercutUIpanel;
	    DVnode_data_labels => <-.DVnode_data_labels;
    	DVcell_data_labels => <-.DVcell_data_labels;
	    plane_xform {
	        obj_in => <-.<-.plane;
        };
    };


    DataObject cut_obj {
    	in => CylinderCut.out_fld;
	    Obj {
	        name => name_of(<-.<-.<-);
    	    Modes.normals = 1;
	    };
    };

    DataObject plane_obj {
    	in => <-.plane;
	    Obj {
	        xform_mode = GD_XFORM_MODE_ALT;
    	    alt_xform+nres => <-.<-.plane.xform;
	    };
    };

    olink out_fld<export_all=2> => CylinderCut.out_fld;
    olink out_obj => cut_obj.obj;

  }; // end of cylinder_cut




  APPS.MultiWindowApp CylinderCutEg {
    MODS.Read_UCD Read_UCD {
       read_ucd_ui {
          filename = "$XP_PATH<0>/data/ucd/bluntfin.inp";
       };
    };

    MODS.external_edges external_edges {
       in_field => <-.Read_UCD.field;
    };
    GEOMS.Axis3D Axis3D {
       in_field => <-.Read_UCD.field;
    };

    cylinder_cut cylinder_cut {
       in_field => <-.Read_UCD.field;
       CutParam {
          dist = 1.07;
       };
    };

    MODS.external_faces external_faces {
       in_field => <-.cylinder_cut.out_fld;
    };

    GDM.Uviewer3D Uviewer3D {
       Scene {
          Top {
             child_objs => {
                <-.<-.<-.Axis3D.out_obj,<-.<-.<-.external_faces.out_obj,
                <-.<-.<-.external_edges.out_obj};
          };
       };
    };

  };


}; // end of library


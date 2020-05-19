flibrary ConeCropMacs <compile_subs=0> {
  // Crop Cone Macros
  group ConeParams<NEportLevels={0,1}> {
     int+nres revol_subs<NEportLevels=1>;
     float+nres side_angle<NEportLevels=1>;
     int+nres plane_dim1<NEportLevels=1>;
     int+nres plane_dim2<NEportLevels=1>;
	 int above<NEportLevels=1>;
     group ConeGroupXform<NEportLevels={2,1}> {
        DefaultXform xform;
     };
  };

  macro ConeFPlane {
     group &in<NEportLevels={2,0}>;
     int dim1<NEportLevels={2,0}>;
     int dim2<NEportLevels={2,0}>;
     float+nres xlate[3] => {0.,0., ((in.coordinates.min_vec[2] + in.coordinates.max_vec[2]) / 2)};
     Mesh_Unif plane {
        nspace = 2;
        ndim = 2;
        dims => {<-.dim1,<-.dim2};
        points+nres => {
           ((in.coordinates.min_vec[0] + in.coordinates.max_vec[0]) / 2),
           ((in.coordinates.min_vec[1] + in.coordinates.max_vec[1]) / 2),
           in.coordinates.max_vec[0],in.coordinates.max_vec[1]};
        xform {
           xlate+nres => cache(<-.<-.xlate);
        };
     };
     olink out_fld => .plane;
  };


  macro ConeRotateMesh {
     ilink in_fld;
     DVrotate_mesh DVrotate_mesh1 {
        in => <-.ConeFPlane.out_fld;
        center => {
           in.coordinates.min_vec[0],in.coordinates.min_vec[1],in.xform.xlate[2]};
        axis = 0;
        angle => <-.ConeParams.side_angle;
     };
     DVrotate_mesh DVrotate_mesh2 {
        in => <-.DVrotate_mesh1.out;
        center => {
           in.coordinates.min_vec[0],in.coordinates.min_vec[1],in.xform.xlate[2]};
        axis = 2;
        angle => (((2 * 3.141592653) / <-.ConeParams.revol_subs) * (index_of(<-.<-.ConeRotateMesh) + 1));
     };
     DataObjectLite DataObjectLite {
        in => <-.DVconcat_xform_field.out;
        Obj {
           name => ("plane" + index_of(<-.<-.<-.ConeRotateMes));
        };
     };
     olink obj => .DataObjectLite.obj;
     olink fld => .DVconcat_xform_field.out;
     XP_COMM_PROJ.ConeCrop.ConeCropMacs.ConeParams &ConeParams<NEportLevels={2,1}>;
     XP_COMM_PROJ.ConeCrop.ConeCropMacs.ConeFPlane ConeFPlane {
        in => <-.in_fld;
        dim1 => <-.ConeParams.plane_dim1;
        dim2 => <-.ConeParams.plane_dim2;
     };
     DVconcat_xform_field DVconcat_xform_field {
        in => <-.DVrotate_mesh2.out;
        in_xform => <-.ConeParams.ConeGroupXform;
     };
  };


  macro ConeCropUI {
     XP_COMM_PROJ.ConeCrop.ConeCropMacs.ConeParams &ConeParams<NEportLevels={2,1}>;
     UImod_panel panel {
        parent<NEportLevels={4,0}>;
        message = "Select plane control panel.";
        title => name_of(<-.<-.<-);
     };
     UItoggle above {
        parent => <-.panel;
        label => "Crop outside cone";
        set => <-.ConeParams.above;
        y = 5;
        width => <-.panel.width;
     };
	 
     UIslider dim1_slider {
        y => ((<-.above.y + <-.above.height) + 4);
        width => <-.panel.width;
        parent => <-.panel;
        min = 2.;
        max = 100.;
        value => <-.ConeParams.plane_dim1;
        mode = "integer";
        title => "plane x-dimension";
     };
     UIslider dim2_slider {
        y => ((<-.dim1_slider.y + <-.dim1_slider.height) + 4);
        width => <-.panel.width;
        parent => <-.panel;
        min = 2.;
        max = 100.;
        value => <-.ConeParams.plane_dim2;
        mode = "integer";
        title => "plane y-dimension";
     };
     UIslider revol_subs_slider {
        parent => <-.panel;
        width => <-.panel.width;
        value => <-.ConeParams.revol_subs;
        title => "revolution subdivisions";
        y => ((<-.dim2_slider.y + <-.dim2_slider.height) + 12);
        min = 5.;
        max = 50.;
        increment = 1.;
        mode = "integer";
     };
     GMOD.copy_on_change copy_on_change {
        trigger = 60;
        input => <-.side_angle_value;
        output => <-.ConeParams.side_angle;
     };
     UIslider cone_side_angle_slider {
        parent => <-.panel;
        width => <-.panel.width;
        value => <-.copy_on_change.trigger;
        title => "side angle in";
        y => ((<-.revol_subs_slider.y + <-.revol_subs_slider.height) + 4);
        min = 0.;
        max = 80.;
        increment = 1.;
        mode = "integer";
     };
     float side_angle_value => ((.cone_side_angle_slider.value * 3.141592653) / 180.);
     XformEditor probe_edit {
        obj_in => <-.ConeParams.ConeGroupXform;
        vis => <-.xform_toggle.set;
     };
     UItoggle xform_toggle {
        parent => <-.panel;
        label => "Transformation Editor";
        y => ((<-.cone_side_angle_slider.y + <-.cone_side_angle_slider.height) + 8);
        width => <-.panel.width;
     };
  };

  macro ConePlanesUI {
     XP_COMM_PROJ.ConeCrop.ConeCropMacs.ConeParams &ConeParams<NEportLevels={2,1}>;
     UImod_panel panel {
        parent<NEportLevels={4,0}>;
        message = "Select plane control panel.";
        title => name_of(<-.<-.<-);
     };
     UIslider dim1_slider {
        y = 0;
        width => <-.panel.width;
        parent => <-.panel;
        min = 2.;
        max = 100.;
        value => <-.ConeParams.plane_dim1;
        mode = "integer";
        title => "plane x-dimension";
     };
     UIslider dim2_slider {
        y => ((<-.dim1_slider.y + <-.dim1_slider.height) + 4);
        width => <-.panel.width;
        parent => <-.panel;
        min = 2.;
        max = 100.;
        value => <-.ConeParams.plane_dim2;
        mode = "integer";
        title => "plane y-dimension";
     };
     UIslider revol_subs_slider {
        parent => <-.panel;
        width => <-.panel.width;
        value => <-.ConeParams.revol_subs;
        title => "revolution subdivisions";
        y => ((<-.dim2_slider.y + <-.dim2_slider.height) + 12);
        min = 5.;
        max = 50.;
        increment = 1.;
        mode = "integer";
     };
     GMOD.copy_on_change copy_on_change {
        trigger = 60;
        input => <-.side_angle_value;
        output => <-.ConeParams.side_angle;
     };
      UIslider cone_side_angle_slider {
        parent => <-.panel;
        width => <-.panel.width;
        value => <-.copy_on_change.trigger;
        title => "side angle in (°)";
        y => ((<-.revol_subs_slider.y + <-.revol_subs_slider.height) + 4);
        min = 0.;
        max = 80.;
        increment = 1.;
        mode = "integer";
     };
     float side_angle_value => ((.cone_side_angle_slider.value * 3.141592653) / 180.);
     XformEditor probe_edit {
        obj_in => <-.ConeParams.ConeGroupXform;
        vis => <-.xform_toggle.set;
     };
     UItoggle xform_toggle {
        parent => <-.panel;
        label => "Transformation Editor";
        y => ((<-.cone_side_angle_slider.y + <-.cone_side_angle_slider.height) + 8);
        width => <-.panel.width;
     };
  };

  macro ConeCropFunc {
     ilink in_fld;
	 
	 XP_COMM_PROJ.ConeCrop.ConeCropMacs.ConeParams& ConeParams;
     XP_COMM_PROJ.ConeCrop.ConeCropMacs.ConeRotateMesh ConeRotateMesh[.ConeParams.revol_subs] {
        in_fld => <-.in_fld;
        ConeParams => <-.ConeParams;
     };
     GDM.GroupObject plane_objects {
        child_objs => <-.ConeRotateMesh.obj;
     };
     DVplane_crop DVplane_crop {
        in => <-.in_fld;
        in_out => <-.ConeParams.above;
		and_or => <-.ConeParams.above;
        plane => <-.ConeRotateMesh.fld;
     };
     GDM.DataObject DataObject {
        in => <-.DVplane_crop.out;
     };
     mlink out_plane_flds<NEportLevels={1,2}> => .ConeRotateMesh.fld;
     olink plane_objs => .plane_objects.obj;
     olink out_fld => .DVplane_crop.out;
     olink out_obj => .DataObject.obj;
  };
  macro ConeCrop {
     ilink in_fld;
     XP_COMM_PROJ.ConeCrop.ConeCropMacs.ConeParams ConeParams {
        revol_subs = 5;
        plane_dim1 = 8;
        plane_dim2 = 8;
		above = 1;
     };
     XP_COMM_PROJ.ConeCrop.ConeCropMacs.ConeCropUI ConeCropUI {
        ConeParams => <-.ConeParams;
     };
     XP_COMM_PROJ.ConeCrop.ConeCropMacs.ConeCropFunc ConeCropFunc {
	    in_fld => <-.in_fld; 
        ConeParams => <-.ConeParams;
     };
     olink out_fld => .ConeCropFunc.out_fld;
     olink out_obj => .ConeCropFunc.out_obj;
	 olink out_plane_flds => .ConeCropFunc.out_plane_flds;
	 olink out_plane_obj => .ConeCropFunc.plane_objs;
  };	

  macro ConePlanes {
     XP_COMM_PROJ.ConeCrop.ConeCropMacs.ConeParams ConeParams {
        revol_subs = 5;
        plane_dim1 = 8;
        plane_dim2 = 8;
     };
     XP_COMM_PROJ.ConeCrop.ConeCropMacs.ConeRotateMesh ConeRotateMesh[.ConeParams.revol_subs] {
        in_fld => <-.in_fld;
        ConeParams => <-.ConeParams;   
     };
     GDM.GroupObject plane_objects {
        child_objs => <-.ConeRotateMesh.obj;
     };
     link in_fld<NEportLevels={2,1}>;
     mlink out_fld<NEportLevels={1,2}> => .ConeRotateMesh.fld;
     link out_obj<NEportLevels={1,2}> => .plane_objects.obj;
     XP_COMM_PROJ.ConeCrop.ConeCropMacs.ConePlanesUI ConePlanesUI {
        ConeParams => <-.ConeParams;
     };
  };

};

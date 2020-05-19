
flibrary TextureVPlaneMacs {

  // Functional Macro

  macro TextureVPlane {

    link in_texture <NEportLevels={2,1},NEx=55.,NEy=55.>;

    XP_COMM_PROJ.TextureVPlane.TextureVPlaneMods.TextureVPlaneParams_ref
        &params <NEportLevels={2,1}>;

    float coords <NEportLevels=1,NEx=55.,NEy=99.>[4][3] =>
        { params.start_x,params.start_y,params.start_z,
          params.start_x,params.start_y,params.end_z,
          params.end_x,params.end_y,params.start_z,
          params.end_x,params.end_y,params.end_z
        };

    float uv_data <NEx=143.,NEy=187.>[4][2] =>
        { 0,0,params.flip,1-params.flip,1-params.flip,params.flip,1,1 };

    FLD_MAP.struct_mesh struct_mesh <NEx=55.,NEy=143.> {
      in_dims = {2,2};
      coord => <-.coords;
    };

    FLD_MAP.node_uv node_uv <NEx=143.,NEy=231.> {
      in_data => <-.uv_data;
    };

    FLD_MAP.combine_mesh_data combine_mesh_data <NEx=55.,NEy=275.> {
      in_mesh => <-.struct_mesh.out;
      in_nd => <-.node_uv.out;
      DataObject {
        Texture {
          data => <-.<-.<-.in_texture;
        };
      };
      out<NEportLevels={0,3}>;
    };

    link out_fld <NEportLevels={1,2},NEx=10.,NEy=349.>
        => .combine_mesh_data.out;

    link out_obj <NEportLevels={1,2},NEx=55.,NEy=319.>
        => .combine_mesh_data.obj;
  };

  // UI Macro

  macro TextureVPlaneUI <NEx=495.,NEy=55.> {

    XP_COMM_PROJ.TextureVPlane.TextureVPlaneMods.TextureVPlaneParams_ref
        &params <NEportLevels={2,1}>;

    UImod_panel panel <NEx=33.,NEy=33.> {
      parent <NEportLevels={3,0}>;
      title => "TextureVPlane";
    };

    UIlabel title <NEx=66.,NEy=66.> {
      parent => <-.panel;
      width => parent.clientWidth;
      label => "TextureVPlane Controls";
      alignment = "center";
      color {
	backgroundColor = "blue";
	foregroundColor = "white";
      };
    };
 
    UIlabel lbl_llimage <NEx=33.,NEy=99.> {
      parent => <-.panel;
      width => parent.clientWidth;
      label => "Lower Left corner of image";
    };
 
    UIfield fld_llx <NEx=231.,NEy=143.> {
      parent => <-.panel;
      value => <-.params.start_x;
      x = 0;
    };

    UIlabel lbl_llx <NEx=33.,NEy=143.> {
      parent => <-.panel;
      label => "X";
      x = 120;
      y => fld_llx.y;
    };

    UIfield fld_lly <NEx=231.,NEy=187.> {
      parent => <-.panel;
      value => <-.params.start_y;
      x = 0;
    };

    UIlabel lbl_lly <NEx=33.,NEy=187.> {
      parent => <-.panel;
      label => "Y";
      x = 120;
      y => fld_lly.y;
    };

    UIfield fld_llz <NEx=231.,NEy=231.> {
      parent => <-.panel;
      value => <-.params.start_z;
      x = 0;
    };

    UIlabel lbl_llz <NEx=33.,NEy=231.> {
      parent => <-.panel;
      label => "Z";
      x = 120;
      y => fld_llz.y;
    };

    UIlabel lbl_urimage <NEx=33.,NEy=275.> {
      parent => <-.panel;
      width => parent.clientWidth;
      label => "Upper Right corner of image";
    };

    UIfield fld_urx <NEx=231.,NEy=319.> {
      parent => <-.panel;
      value => <-.params.end_x;
      x = 0;
    };

    UIlabel lbl_urx <NEx=33.,NEy=319.> {
      parent => <-.panel;
      label => "X";
      x = 120;
      y => fld_urx.y;
    };

    UIfield fld_ury <NEx=231.,NEy=363.> {
      parent => <-.panel;
      value => <-.params.end_y;
      x = 0;
    };

    UIlabel lbl_ury <NEx=33.,NEy=363.> {
      parent => <-.panel;
      label => "Y";
      x = 120;
      y => fld_ury.y;
    };

    UIfield fld_urz <NEx=231.,NEy=407.> {
      parent => <-.panel;
      value => <-.params.end_z;
      x = 0;
    };

    UIlabel lbl_urz <NEx=33.,NEy=407.> {
      parent => <-.panel;
      label => "Z";
      x = 120;
      y => fld_urz.y;
    };

    UItoggle UItoggle <NEx=429.,NEy=143.> {
      parent => <-.panel;
      label => "Flip Upper Left and Lower right";
      set => <-.params.flip;
      width => parent.clientWidth;
    };
  };

  // User Macro

  macro texture_vplane {

    link in_texture <NEportLevels={2,1},NEx=55.,NEy=55.>;

    XP_COMM_PROJ.TextureVPlane.TextureVPlaneMods.TextureVPlaneParams
        params <NEportLevels={2,1}> {
      start_x = -1.;
      start_y = 0.;
      start_z = 0.;
      end_x = 1.;
      end_y = 0.;
      end_z = 2.;
      flip = 1;
    };

    TextureVPlane TextureVPlane <NEx=100.,NEy=100.> {
      in_texture => <-.in_texture;
      params => <-.params;
      combine_mesh_data.DataObject.Obj {
	name => name_of(<-.<-.<-.<-.<-);
      };
    };

    TextureVPlaneUI TextureVPlaneUI <NEx=33.,NEy=33.> {
      params => <-.params;
      panel {
	parent <NEportLevels={4,0}>;
        title => name_of(<-.<-.<-);
      };
    };

    link out_fld <NEportLevels={1,2},NEx=55.,NEy=369.>
        => TextureVPlane.out_fld;

    link out_obj <NEportLevels={1,2},NEx=55.,NEy=319.>
        => TextureVPlane.out_obj;
  };

};
	
	

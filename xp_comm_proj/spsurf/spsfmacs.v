
// replace SphereSurface with general name
// replace SphereSurfaceGen with low-level module name
// replace sphere_surface with user macro name

flibrary SphereSurfaceMacs {

  // SphereSurfaceUI
  //   UI Macro

  macro SphereSurfaceUI {
    XP_COMM_PROJ.SphereSurface.SphereSurfaceMods.SphereSurfaceParams &SphereSurfaceParams<NEportLevels={2,1},NEx=187.,NEy=120.>;

    UImod_panel panel<NEx=11.,NEy=55.> {
      title => "SphereSurface";
    };

    UIlabel label<NEx=132.,NEy=99.> {
      x = 0;
      y = 0;
      parent => <-.panel;
      label => "Sphere Surface Parameters";
      width => parent.width;
      color {
         backgroundColor = "blue";
         foregroundColor = "white";
      };
    };

    // left frame - longitude

    UIframe longitude<NEx=297.,NEy=99.> {
      parent => <-.panel;
      x => label.x;
      y => <-.label.y + <-.label.height + 10;
      width => (parent.width / 2);
      height => <-.longclosed.y + <-.longclosed.height + 10;
    };

    UIlabel labelA<NEx=418.,NEy=143.> {
      x = 0;
      y = 0;
      width => parent.width;
      parent => <-.longitude;
      label => "Longitude";
      color {
         backgroundColor = "blue";
         foregroundColor = "white";
      };
    };

    UIlabel labelA1<NEx=418.,NEy=187.> {
      x => <-.labelA.x;
      y => <-.labelA.y + <-.labelA.height + 5;
      parent => <-.longitude;
      label => "Start";
      alignment = "left";
    };
   
    UIfield longstart<NEx=418.,NEy=231.> {
      x => <-.labelA1.x;
      y => <-.labelA1.y + <-.labelA1.height + 2;
      width => parent.clientWidth - 2;
      parent => <-.longitude;
      value => <-.SphereSurfaceParams.start_long;
    };

    UIlabel labelA2<NEx=418.,NEy=275.> {
      x => <-.longstart.x;
      y => <-.longstart.y + <-.longstart.height + 5;
      parent => <-.longitude;
      label => "End";
      alignment = "left";
    };

    UIfield longend<NEx=418.,NEy=319.> {
      x => <-.labelA2.x;
      y => <-.labelA2.y + <-.labelA2.height + 2;
      width => parent.clientWidth - 2;
      parent => <-.longitude;
      value => <-.SphereSurfaceParams.end_long;
    };
  
    UIlabel labelA3<NEx=418.,NEy=363.> {
      x => <-.longend.x;
      y => <-.longend.y + <-.longend.height + 5;
      parent => <-.longitude;
      label => "Subdivisions";
      width => parent.width;
      alignment = "left";
    };

    UIfield longdim<NEx=418.,NEy=407.> {
      x => <-.labelA3.x;
      y => <-.labelA3.y + <-.labelA3.height + 2;
      width => parent.clientWidth - 2;
      parent => <-.longitude;
      value => <-.SphereSurfaceParams.longdim;
      min = 3.;
      decimalPoints = 0;
    };
  
    UItoggle longclosed<NEx=418.,NEy=451.> {
      x => <-.longdim.x;
      y => <-.longdim.y + <-.longdim.height + 5;
      parent => <-.longitude;
      label => "Closed";
      set+Port2 => <-.SphereSurfaceParams.closedlong;
    };

    // right frame - latitude

    UIframe latitude<NEx=473.,NEy=99.> {
      parent => <-.panel;
      x => <-.longitude.width;
      y => <-.longitude.y;
      width => (parent.width / 2);
      height => ((<-.latclosed.y + <-.latclosed.height) + 10);
    };

    UIlabel labelB<NEx=594.,NEy=143.> {
      x = 0;
      y = 0;
      width => parent.width;
      parent => <-.latitude;
      label => "Latitude";
      color {
         backgroundColor = "blue";
         foregroundColor = "white";
      };
    };

    UIlabel labelB1<NEx=594.,NEy=187.> {
      x => <-.labelB.x;
      y => <-.labelB.y + <-.labelB.height + 5;
      parent => <-.latitude;
      label => "Start";
      alignment = "left";
    };

    UIfield latstart<NEx=594.,NEy=231.> {
      x => <-.labelB1.x;
      y => <-.labelB1.y + <-.labelB1.height + 2;
      parent => <-.latitude;
      width => (parent.clientWidth - 2);
      value<NEportLevels={2,2}> => <-.SphereSurfaceParams.start_lat;
    };

    UIlabel labelB2<NEx=594.,NEy=275.> {
      x => <-.latstart.x;
      y => <-.latstart.y + <-.latstart.height + 5;
      parent => <-.latitude;
      label => "End";
      alignment = "left";
    };
  
    UIfield latend<NEx=594.,NEy=319.> {
      x => <-.labelB2.x;
      y => <-.labelB2.y + <-.labelB2.height + 2;
      parent => <-.latitude;
      width => (parent.clientWidth - 4);
      value<NEportLevels={2,2}> => <-.SphereSurfaceParams.end_lat;
    };

    UIlabel labelB3<NEx=594.,NEy=363.> {
      x => <-.latend.x;
      y => <-.latend.y + <-.latend.height + 5;
      parent => <-.latitude;
      label => "Subdivisions";
      alignment = "left";
    };

    UIfield latdim<NEx=594.,NEy=407.> {
      x => <-.labelB3.x;
      y => <-.labelB3.y + <-.labelB3.height + 2;
      parent => <-.latitude;
      value => <-.SphereSurfaceParams.latdim;
      width => (parent.clientWidth - 4);
      min = 3.;
      decimalPoints = 0;
    };

    UItoggle latclosed<NEx=594.,NEy=451.> {
      x => <-.latdim.x;
      y => <-.latdim.y + <-.latdim.height + 5;
      parent => <-.latitude;
      label => "Closed";
      set+Port2 => <-.SphereSurfaceParams.closedlat;
    };

    // main frame

    UIlabel labelC<NEx=187.,NEy=143.> {
      x => <-.longitude.x;
      y => <-.longitude.y + <-.longitude.height + 10;
      parent => <-.panel;
      label => "Radius";
      width => parent.width;
      color {
         backgroundColor = "blue";
         foregroundColor = "white";
      };
      alignment = "center";
    };

    UIfield radius<NEx=187.,NEy=187.> {
      x => <-.labelC.x;
      y => <-.labelC.y + <-.labelC.height + 2;
      parent => <-.panel;
      value => <-.SphereSurfaceParams.radius;
      width => parent.width;
      decimalPoints = 4;
    };
  };




  // SphereSurface
  //   Functional macro (Mesh output/Data Object)

  macro SphereSurface {
    XP_COMM_PROJ.SphereSurface.SphereSurfaceMods.SphereSurfaceParams &SphereSurfaceParams<NEportLevels={2,1}>;

    XP_COMM_PROJ.SphereSurface.SphereSurfaceMods.SphereSurfaceGen SphereSurfaceGen {
      SphereSurfaceParams => <-.SphereSurfaceParams;
    };

    GDM.DataObject DataObject {
      in => <-.SphereSurfaceGen.mesh;
      Obj {
         name => name_of(<-.<-.<-);
      };
    };

    olink out_mesh => SphereSurfaceGen.mesh;
    olink out_obj  => DataObject.obj;
  };


  // SphereSurfaceData
  //   Functional macro (Mesh+Node_Data output/Data Object)

  macro SphereSurfaceData {
    ilink in;
    XP_COMM_PROJ.SphereSurface.SphereSurfaceMods.SphereSurfaceParams &SphereSurfaceParams<NEportLevels={2,1}>;

    XP_COMM_PROJ.SphereSurface.SphereSurfaceMods.SphereSurfaceGen SphereSurfaceGen {
      SphereSurfaceParams => <-.SphereSurfaceParams;
    };

    XP_COMM_PROJ.SphereSurface.SphereSurfaceMods.extract_node_data extract_node_data {
      in => <-.in;
    };

#ifdef VIZ_EXPRESS
    module DVcomb_mesh_and_data {
      Mesh+IPort2 &in_mesh => <-.SphereSurfaceGen.mesh;
      Node_Data+nres+IPort2 &in_nd => <-.extract_node_data.out;
      Mesh+Node_Data+OPort2 &out => merge(in_nd, in_mesh);
    };
#else
    DVcomb_mesh_and_data DVcomb_mesh_and_data {
      in_mesh => <-.SphereSurfaceGen.mesh;
      in_nd => <-.extract_node_data.out;
    };
#endif

    GDM.DataObject DataObject {
      in => <-.DVcomb_mesh_and_data.out;
      Obj {
         name => name_of(<-.<-.<-);
      };
    };

    olink out_field => .DVcomb_mesh_and_data.out;
    olink out_obj    => .DataObject.obj;
  };



  // sphere_surface
  //   User Macro (SphereSurface Functional Macro and UI)

  macro sphere_surface {

    XP_COMM_PROJ.SphereSurface.SphereSurfaceMods.SphereSurfaceParams  SphereSurfaceParams {
      longdim = 10;
      latdim = 10;
      radius = 1.;
      start_long = -180.;
      end_long = 180.;
      start_lat = 90.;
      end_lat = -90.;
      closedlong = 1;
      closedlat = 0;
    };

    XP_COMM_PROJ.SphereSurface.SphereSurfaceMacs.SphereSurfaceUI SphereSurfaceUI {
      SphereSurfaceParams => <-.SphereSurfaceParams;
      panel {
        title => name_of(<-.<-.<-);
      };
    };

    XP_COMM_PROJ.SphereSurface.SphereSurfaceMacs.SphereSurface SphereSurface {
      SphereSurfaceParams => <-.SphereSurfaceParams;
      DataObject {
         Obj {
            name => name_of(<-.<-.<-.<-);
         };
      };
    };

    olink out_mesh => .SphereSurface.out_mesh;
    olink out_obj  => .SphereSurface.out_obj;
  };



  // sphere_surface_data
  //   User Macro (SphereSurfaceData Functional Macro and UI)

  macro sphere_surface_data {

    Grid_Struct+Node_Data+Dim2 &in <NEportLevels={2,1}>;

    XP_COMM_PROJ.SphereSurface.SphereSurfaceMods.SphereSurfaceParams SphereSurfaceParams {
      longdim => <-.in.dims[0];
      latdim => <-.in.dims[1];
      radius = 1.;
      start_long = -180.;
      end_long = 180.;
      start_lat = 90.;
      end_lat = -90.;
      closedlong = 1;
      closedlat = 0;
    };

    XP_COMM_PROJ.SphereSurface.SphereSurfaceMacs.SphereSurfaceUI SphereSurfaceUI {
      SphereSurfaceParams => <-.SphereSurfaceParams;
      panel {
        title => name_of(<-.<-.<-);
      };
    };

    XP_COMM_PROJ.SphereSurface.SphereSurfaceMacs.SphereSurfaceData SphereSurfaceData {
      in => <-.in;
      SphereSurfaceParams => <-.SphereSurfaceParams;
      DataObject {
         Obj {
            name => name_of(<-.<-.<-.<-);
         };
      };
    };

    olink out_field => SphereSurfaceData.out_field;
    olink out_obj   => SphereSurfaceData.out_obj;
  };


  // sphere_surface_lite
  //   User Macro (low number of objects for use as glyph)

  macro sphere_surface_lite {

    int+Port subdiv = 6;

    XP_COMM_PROJ.SphereSurface.SphereSurfaceMods.SphereSurfaceGen SphereSurfaceGen {
      ^SphereSurfaceParams {
        end_lat = -90.;
        start_lat = 90.;
        end_long = 180.;
        start_long = -180.;
        radius = 1.;
        closedlong = 1;
        closedlat = 0;
      };

      longdim+IPort2 => <-.subdiv;
      latdim+IPort2  => <-.subdiv;
    };
   
    olink out_mesh => .SphereSurfaceGen.mesh;
  };

};


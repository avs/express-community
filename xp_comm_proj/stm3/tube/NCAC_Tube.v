macro NCAC_Tube<NEhelpContextID=1,NEhelpFile="STM3/NCAC_Tube.html"> {
   link molecule<NEportLevels={2,1},NEx=253.,NEy=66.>;
   XP_COMM_PROJ.STM3.TUBES.NCACTube NCACTube<NEx=440.,NEy=132.> {
      molecule => <-.molecule;
      tube_coordinates[][];
   };
   FLD_MAP.polyline_mesh polyline_mesh<NEx=451.,NEy=341.> {
      coord => <-.CatmullSpline_2Core.out;
      connect => {0, (<-.CatmullSpline_2Core.out_size - 1)};
   };
   XP_COMM_PROJ.STM3.SUPPORT.tubes tubes {
      in => <-.polyline_mesh.out;
      bondradius => <-.scale.value;
      facets => <-.facets.value;
   };
   GDM.DataObject DataObject<NEx=539.,NEy=539.> {
      in => <-.tubes.out;
      Obj {
         name => "Tube";
      };
   };
   link fld<NEportLevels={1,2},NEx=319.,NEy=594.> => .tubes.out;
   link obj<NEportLevels={1,2},NEx=693.,NEy=594.> => .DataObject.obj;
   UImod_panel UImod_panel<NEx=704.,NEy=55.> {
      title => name_of(<-.<-,1);
   };
   UIlabel title<NEx=770.,NEy=121.> {
      parent => <-.UImod_panel;
      label => "Interpolation parameters";
      y = 0;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UIslider pt_added<NEx=781.,NEy=209.> {
      parent => <-.UImod_panel;
      value = 11.;
      title => "Points added";
      y = 35;
      max = 50.;
      min = 1.;
      width => parent.clientWidth;
      mode = "integer";
      decimalPoints = 0;
   };
   XP_COMM_PROJ.Catmull_2.Catmull_2Mods.Catmull_2Params Catmull_2Params<NEx=583.,NEy=198.> {
      line_type = "open";
      spline_type = "CatmullRom";
      smoothness => <-.smooth.value;
      points_added => <-.pt_added.value;
      DistanceEps = 0.01;
   };
   XP_COMM_PROJ.Catmull_2.Catmull_2Mods.CatmullSpline_2Core CatmullSpline_2Core<NEx=451.,NEy=264.> {
      points => <-.NCACTube.tube_coordinates;
      Catmull_2Params => <-.Catmull_2Params;
   };
   UIslider smooth<NEx=792.,NEy=286.> {
      parent => <-.UImod_panel;
      value = 2.;
      title => "Smoothness";
      y = 100;
      max = 2.;
      min = 0.;
      width => parent.clientWidth;
   };
   UIlabel title#1<NEx=781.,NEy=374.> {
      parent => <-.UImod_panel;
      label => "Tube parameters";
      y = 180;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UIslider scale<NEx=792.,NEy=451.> {
      parent => <-.UImod_panel;
      value = 0.3;
      title => "Tube scale";
      y = 220;
      max = 3.;
      min = 0.01;
      width => parent.clientWidth;
   };
   UIslider facets<NEx=792.,NEy=506.> {
      parent => <-.UImod_panel;
      value = 16.;
      title => "Tube facets";
      y = 280;
      max = 25.;
      min = 3.;
      width => parent.clientWidth;
      mode = "integer";
      decimalPoints = 0;
   };
};

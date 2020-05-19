macro Miller_Plane<NEhelpContextID=1,NEhelpFile="STM3/Miller_Plane.html"> {
   link molecule_in<NEportLevels={2,1},NEx=66.,NEy=33.>;
   XP_COMM_PROJ.STM3.CRYSTAL.MillerPlane MillerPlane<NEx=275.,NEy=418.> {
      in => <-.molecule_in;
   };
   FLD_MAP.quad_mesh quad_mesh<NEx=385.,NEy=528.> {
      coord => <-.MillerPlane.vertices;
      connect = {0,1,2,3};
      DataObject {
         Obj {
            name => "Miller plane";
         };
         Props {
            trans = 0.5;
            inherit = 0;
         };
         Modes {
            mode = {0,2,0,0,0};
         };
      };
   };
   link plane_fld<NEportLevels={1,2},NEx=517.,NEy=627.> => .quad_mesh.out;
   link plane_obj<NEportLevels={1,2},NEx=550.,NEy=583.> => .quad_mesh.obj;
   link out<NEportLevels={1,2},NEx=594.,NEy=462.> => .MillerPlane.out;
   UImod_panel UImod_panel<NEx=385.,NEy=44.> {
      title => name_of(<-.<-,1);
   };
   UIlabel title<NEx=660.,NEy=88.> {
      parent => <-.UImod_panel;
      label => "Miller plane indices";
      y = 0;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UIfield n1<NEx=517.,NEy=154.> {
      parent => <-.UImod_panel;
      value => <-.MillerPlane.n1;
      y = 30;
      width = 80;
      mode = "integer";
      decimalPoints = 0;
      updateMode = 7;
   };
   UIfield n2<NEx=660.,NEy=154.> {
      parent => <-.UImod_panel;
      value => <-.MillerPlane.n2;
      y = 30;
      width = 80;
      mode = "integer";
      decimalPoints = 0;
      updateMode = 7;
      x = 82;
   };
   UIfield n3<NEx=803.,NEy=154.> {
      parent => <-.UImod_panel;
      value => <-.MillerPlane.n3;
      y = 30;
      width = 80;
      mode = "integer";
      decimalPoints = 0;
      updateMode = 7;
      x = 164;
   };
   UIlabel title2<NEx=660.,NEy=209.> {
      parent => <-.UImod_panel;
      label => "Plane parameters";
      y = 70;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UIslider offset<NEx=660.,NEy=253.> {
      parent => <-.UImod_panel;
      value => <-.MillerPlane.offset;
      title => "offset";
      y = 100;
      max = 1.;
      min = -1.;
      width = 220;
      decimalPoints = 4;
   };
   VUIslider_typein offset_typein<NEx=814.,NEy=253.> {
      slider => offset;
      width => 22;
   };
   UIslider scale_u<NEx=660.,NEy=297.> {
      parent => <-.UImod_panel;
      value => <-.MillerPlane.scale_u;
      y = 160;
      max = 10.;
      min = 0.;
      width = 220;
      decimalPoints = 4;
   };
   VUIslider_typein scale_u_typein<NEx=814.,NEy=297.> {
      slider => scale_u;
      width => 22;
   };
   UIslider scale_v<NEx=660.,NEy=341.> {
      parent => <-.UImod_panel;
      value => <-.MillerPlane.scale_v;
      y = 220;
      max = 10.;
      min = 0.;
      width = 220;
      decimalPoints = 4;
   };
   VUIslider_typein scale_v_typein<NEx=814.,NEy=341.> {
      slider => scale_v;
      width => 22;
   };
   UItoggle above<NEx=814.,NEy=385.> {
      parent => <-.UImod_panel;
      set => <-.MillerPlane.side;
      y = 280;
		width = 250;
		label = "remove atoms above plane";
   };
	UIslider opacity<NEx=814.,NEy=440.> {
   	parent => <-.UImod_panel;
   	value => <-.quad_mesh.DataObject.Props.trans;
   	y = 304;
   	max = 1.;
   	min = 0.;
   	width = 220;
	};
};

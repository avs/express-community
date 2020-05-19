macro Make_tubes {
   link in<NEportLevels={2,1},NEx=88.,NEy=44.>;
   XP_COMM_PROJ.STM3.SUPPORT.tubes tubes<NEx=561.,NEy=385.> {
      in => <-.in;
      facets => <-.UIslider.value;
      bondradius<NEportLevels={2,1}> => <-.UIfield.value;
   };
   UImod_panel UImod_panel<NEx=44.,NEy=143.> {
      title => name_of(<-.<-,1);
   };
   UIlabel UIlabel<NEx=176.,NEy=209.> {
      parent => <-.UImod_panel;
      label => "Tube radius:";
      y = 13;
      x = 5;
      alignment = "left";
   };
   UIfield UIfield<NEx=319.,NEy=209.> {
      parent => <-.UImod_panel;
      value<NEportLevels={0,2}> = 0.1;
      y = 10;
      x = 106;
      min = 0.;
      updateMode = 7;
   };
   UIslider UIslider<NEx=176.,NEy=253.> {
      parent => <-.UImod_panel;
      value<NEportLevels={2,0}> = 16.;
      title => "Facets";
      y = 54;
      max = 30.;
      min = 3.;
      width => parent.clientWidth;
      mode = "integer";
      decimalPoints = 0;
   };
   GDM.DataObject DataObject<NEx=297.,NEy=517.> {
      in => <-.tubes.out;
      Obj {
         name => name_of(<-.<-.<-,1);
      };
   };
   link fld<NEportLevels={1,2},NEx=737.,NEy=451.> => .tubes.out;
   link obj<NEportLevels={1,2},NEx=462.,NEy=561.> => .DataObject.obj;
};

macro ColorLegend<NEhelpContextID=1,NEhelpFile="STM3/ColorLegend.html"> {
   XP_COMM_PROJ.STM3.BASE.DisplayParams &display_params<NEx=66.,NEy=33.,NEportLevels={2,1}>;
   int dims<NEportLevels={0,1},NEx=308.,NEy=143.>[] => {1,2};
   float values<NEportLevels={0,1},NEx=308.,NEy=110.>[] => {
      display_params.charge_min,display_params.charge_max};
   FLD_MAP.uniform_scalar_field uniform_scalar_field<NEx=451.,NEy=198.> {
      mesh {
         in_dims => <-.<-.dims;
      };
      data {
         in_data => <-.<-.values;
      };
   };
   GEOMS.LegendVert LegendVert<NEx=473.,NEy=297.> {
      obj_in => <-.uniform_scalar_field.obj;
      GroupObject {
         Top {
            visible => (is_valid(display_params.num_atoms) && (display_params.num_atoms > 0));
         };
      };
      UIpanel UImod_panel {
         y = 30;
         parent<NEportLevels={3,0}> => <-.<-.UImod_panel;
         height = 950;
         width => parent.clientWidth;
			visible => <-.<-.display_params.atoms_representation >= 7;
      };
   };
   XP_COMM_PROJ.STM3.BASE.ColorLegendCore ColorLegendCore<NEx=253.,NEy=385.> {
      in => <-.display_params;
      y = {-0.8,-0.4};
   };
   MODS.text_glyph text_glyph<NEx=330.,NEy=473.> {
      in_mesh => <-.ColorLegendCore.labels_pt;
      in_text => <-.ColorLegendCore.labels;
      obj {
         Obj {
            xform_mode = "Locked";
            name => "atom name label";
         };
      };
      UIpanel UImod_panel {
         y = 220;
         parent<NEportLevels={3,0}> => <-.<-.panel_atoms;
         height = 950;
         width => parent.clientWidth;
      };
      TextUI {
         AlignHorizMenu {
            x = 0;
         };
         AlignVertMenu {
            selectedItem = 2;
            x = 0;
         };
         UIoption#3 {
            set = 0;
         };
         UIoption#5 {
            set = 1;
         };
      };
   };
   GDM.GroupObject GroupObject<NEx=473.,NEy=539.> {
      child_objs => {<-.text_glyph.out_obj,<-.DataObject.obj};
      Top {
         visible => (is_valid(display_params.num_atoms) && (display_params.num_atoms > 0));
      };
   };
   GDM.DataObjectNoTexture DataObject<NEx=506.,NEy=473.> {
      in => <-.ColorLegendCore.fld;
      Props {
         subdiv = 16;
         inherit = 0;
         cull = "Back";
      };
      Obj {
         name => "atom example";
         xform_mode = "Locked";
      };
   };
   link legend_obj<NEportLevels={1,2},NEx=693.,NEy=583.,NEnumColors=1,NEcolor0=0xff0000> => switch(((display_params.atoms_representation > 6) + 1),.GroupObject.obj,.LegendVert.GroupObject.obj);
   UImod_panel UImod_panel<NEx=561.,NEy=44.> {
      title => name_of(<-.<-,1);
   };
   UIpanel panel_atoms<NEx=682.,NEy=110.> {
      parent => <-.UImod_panel;
      y = 30;
      width => parent.clientWidth;
      height = 1200;
		visible => <-.display_params.atoms_representation < 7;
   };
   UIlabel title<NEx=792.,NEy=176.> {
      parent => <-.UImod_panel;
      y = 0;
      width => parent.clientWidth;
      color<NEdisplayMode="open"> {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
		label = "Legend parameters";
   };
   UIlabel UIlabel1<NEx=682.,NEy=242.> {
      parent => <-.panel_atoms;
      label => "X min:";
      y = 0;
      x = 5;
      alignment = "left";
   };
   UIfield xmin<NEx=836.,NEy=242.> {
      parent => <-.panel_atoms;
      y = 0;
      x = 110;
      min = -1.;
      max = 1.;
      value => <-.ColorLegendCore.x[0];
      updateMode = 7;
   };
   UIlabel UIlabel2<NEx=682.,NEy=242.> {
      parent => <-.panel_atoms;
      label => "X max:";
      y = 30;
      x = 5;
      alignment = "left";
   };
   UIfield xmax<NEx=836.,NEy=242.> {
      parent => <-.panel_atoms;
      y = 30;
      x = 110;
      min = -1.;
      max = 1.;
      value => <-.ColorLegendCore.x[1];
      updateMode = 7;
   };
   UIlabel UIlabel3<NEx=682.,NEy=242.> {
      parent => <-.panel_atoms;
      label => "Y min:";
      y = 60;
      x = 5;
      alignment = "left";
   };
   UIfield ymin<NEx=836.,NEy=242.> {
      parent => <-.panel_atoms;
      y = 60;
      x = 110;
      min = -1.;
      max = 1.;
      value => <-.ColorLegendCore.y[0];
      updateMode = 7;
   };
   UIlabel UIlabel4<NEx=682.,NEy=242.> {
      parent => <-.panel_atoms;
      label => "Y max:";
      y = 90;
      x = 5;
      alignment = "left";
   };
   UIfield ymax<NEx=836.,NEy=242.> {
      parent => <-.panel_atoms;
      y = 90;
      x = 110;
      min = -1.;
      max = 1.;
      value => <-.ColorLegendCore.y[1];
      updateMode = 7;
   };
   UIlabel UIlabel5<NEx=682.,NEy=242.> {
      parent => <-.panel_atoms;
      label => "Z value:";
      y = 120;
      x = 5;
      alignment = "left";
   };
   UIfield zval<NEx=836.,NEy=242.> {
      parent => <-.panel_atoms;
      y = 120;
      x = 110;
      min = -1.;
      max = 1.;
      value => <-.ColorLegendCore.z;
      updateMode = 7;
   };
   UIlabel UIlabel6<NEx=682.,NEy=242.> {
      parent => <-.panel_atoms;
      label => "Scale:";
      y = 150;
      x = 5;
      alignment = "left";
   };
   UIfield scale<NEx=836.,NEy=242.> {
      parent => <-.panel_atoms;
      y = 150;
      x = 110;
      min = 0.;
      value => <-.ColorLegendCore.scale;
      updateMode = 7;
   };
   UIlabel UIlabel7<NEx=682.,NEy=242.> {
      parent => <-.panel_atoms;
      label => "Label offset:";
      y = 180;
      x = 5;
      alignment = "left";
   };
   UIfield offset<NEx=836.,NEy=242.> {
      parent => <-.panel_atoms;
      y = 180;
      x = 110;
      value => <-.ColorLegendCore.label_offset;
      updateMode = 7;
   };
};

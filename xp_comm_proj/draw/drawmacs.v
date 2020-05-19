
flibrary DrawMacs <compile_subs=0> {

   macro SuperDrawUI {
      XP_COMM_PROJ.Draw.DrawMods.DrawLineParams &DrawLineParams<NEportLevels={2,1}>;

      UImod_panel panel {
         parent<NEportLevels={4,0}>;
         title ="SuperDraw";
      };

      UIframe UIframe {
         x = 0;
         y = 0;
         width => parent.clientWidth;
         height => <-.closed.y + <-.closed.height + 20;
         parent=> <-.panel;
      };

      UIlabel title {
         parent => <-.UIframe;
         label => "Super Draw Line";
         x = 0;
         y = 0;
         width => parent.clientWidth;
         color {
            backgroundColor = "Gray";
         };
         fontAttributes {
            weight = "bold";
         };
      };

      UIlabel usage {
         parent => <-.UIframe;
         x = 5;
         y => <-.title.y + <-.title.height + 10;
         width => parent.clientWidth - 10;
         height => 2 * UIdata.UIfonts[0].lineHeight;
         label = "Use Control + right mouse button \nto draw points";
         alignment = "left";
      };

      UIradioBoxLabel drawingCommandRadioBox {
         parent => <-.UIframe;
         labels = {
            "Inactive", "Append point", "Add point between",
            "Move point", "Delete point", "Delete all points", "Translate line"
         };
         &selectedItem+IPort2 => <-.DrawLineParams.command;
         title = "Select Drawing Option";
         x => <-.usage.x;
         y => <-.usage.y + <-.usage.height + 20;
         width => parent.clientWidth;
      };

      UItoggle closed {
         x => <-.drawingCommandRadioBox.x + 5;
         y => <-.drawingCommandRadioBox.y + <-.drawingCommandRadioBox.height + 15;
         parent => <-.UIframe;
         set =><-.DrawLineParams.type;
         width => parent.clientWidth - 20;
         label => "Closed Curve";
      };
   };


   macro SuperDrawFunc {
      XP_COMM_PROJ.Draw.DrawMods.DrawLineParams &DrawLineParams<NEportLevels={2,1}>;

      UItwoPoint UItwoPoint {
         view+nres => <-.DrawLineParams.view.render_view;
         startEvent = "Control<BtnRDown>";
         runEvent = "Control<BtnRMotion>";
         stopEvent = "Control<BtnRUp>";
         state+OPort2 => <-.DrawLineParams.state;
         x+OPort2     => <-.DrawLineParams.x;
         y+OPort2     => <-.DrawLineParams.y;
      };

      XP_COMM_PROJ.Draw.DrawMods.SuperDrawCore SuperDrawCore {
         command => <-.DrawLineParams.command;
         line_type => <-.DrawLineParams.type;
         width = 2;
         x => <-.DrawLineParams.x;
         y => <-.DrawLineParams.y;
         state => <-.DrawLineParams.state;
         obj => <-.DrawLineParams.obj;
         view => <-.DrawLineParams.view;
      };

      FLD_MAP.polyline_mesh polyline_mesh {
         coord => <-.SuperDrawCore.points;
         connect => {0, (<-.SuperDrawCore.npoints - 1)};
         DataObject.Obj {
            name => name_of(<-.<-.<-.<-);
            xform_mode = "Parent";
         };
      };

      olink out_points => .SuperDrawCore.points;
      olink out_mesh   => .polyline_mesh.out;
      olink out_obj    => .polyline_mesh.obj;
   };


   macro SuperDraw {
      ilink obj_in;
      ilink view_in;

      XP_COMM_PROJ.Draw.DrawMods.DrawLineParams DrawLineParams {
         obj  => <-.obj_in;
         view => <-.view_in;
      };

      SuperDrawUI SuperDrawUI {
         DrawLineParams=><-.DrawLineParams;
         panel {
            title => name_of(<-.<-.<-,1);
         };
      };
      SuperDrawFunc SuperDrawFunc {
         DrawLineParams=><-.DrawLineParams;
         polyline_mesh.DataObject.Obj.name => name_of(<-.<-.<-.<-.<-);
      };

      olink out_points  => .SuperDrawFunc.out_points;
      olink out_mesh    => .SuperDrawFunc.out_mesh;
      olink out_obj     => .SuperDrawFunc.out_obj;
   };



   macro LineResamplerUI {
      XP_COMM_PROJ.Draw.DrawMods.LineResamplerParams &LineResamplerParams<NEportLevels={2,1}>;

      UImod_panel panel {
         parent<NEportLevels={4,0}>;
         title ="LineResampler";
      };
      UIframe UIframe {
         y = 0;
         width => parent.clientWidth;
         height = 110;
         parent =><-.panel;
      };

      UIlabel title {
         x = 0;
         y = 0;
         width => parent.clientWidth;
         color {
            backgroundColor = "Gray";
         };
         fontAttributes {
            weight = "bold";
         };
         parent => <-.UIframe;
         label => "Resample Input Line";
      };

      UIlabel NumPointsLabel {
         x = 0;
         y => <-.title.y + <-.title.height + 10;
         width => parent.clientWidth * (1./3.) - 5;
         parent => <-.UIframe;
         label => "Num. Points:";
         alignment = "left";
      };


      UIfield NumPointsField {
         x => <-.NumPointsLabel.x + <-.NumPointsLabel.width + 5;
         y => <-.NumPointsLabel.y;
         width => parent.clientWidth * (2./3.) - 10;
         parent => <-.UIframe;
         min = 0.;
         value+Port2 =><-.LineResamplerParams.npts;
         mode = "integer";
      };

      UItoggle TypeToggle {
         x = 5;
         y => <-.NumPointsField.y + <-.NumPointsField.height + 10;
         width => parent.clientWidth - 10;
         parent => <-.UIframe;
         label => "Clip points";
         set => <-.LineResamplerParams.type;
      };
   };


   macro LineResamplerFunc {
      ilink in_points;
      ilink in_dims;
      
      XP_COMM_PROJ.Draw.DrawMods.LineResamplerParams &LineResamplerParams<NEportLevels={2,1}>;

      XP_COMM_PROJ.Draw.DrawMods.LineResamplerCore LineResamplerCore {
         npts => <-.LineResamplerParams.npts;
         type => <-.LineResamplerParams.type;
         in => <-.in_points;
         dims => <-.in_dims;
      };

      FLD_MAP.point_mesh point_mesh {
         coord => <-.LineResamplerCore.points;
         DataObject.Obj.name => name_of(<-.<-.<-.<-);
      };

      olink out_mesh => .point_mesh.out;
      olink out_obj  => .point_mesh.obj;
   };


   macro LineResampler {
      ilink in_points;
      ilink in_dims;

      XP_COMM_PROJ.Draw.DrawMods.LineResamplerParams LineResamplerParams;

      LineResamplerUI LineResamplerUI{
         LineResamplerParams=><-.LineResamplerParams;
         panel {
            title => name_of(<-.<-.<-,1);
         };
      };
      LineResamplerFunc LineResamplerFunc {
         in_points => <-.in_points;
         in_dims => <-.in_dims;
         LineResamplerParams => <-.LineResamplerParams;
         point_mesh.DataObject.Obj.name => name_of(<-.<-.<-.<-.<-);
      };

      olink out_mesh => .LineResamplerFunc.out_mesh;
      olink out_obj  => .LineResamplerFunc.out_obj;
   };



   macro RegionResampler {
      ilink in_points;
      ilink in_dims;


      XP_COMM_PROJ.Draw.DrawMods.RegionResamplerCore RegionResamplerCore {
         in => <-.in_points;
         dims => <-.in_dims;
      };

      FLD_MAP.uniform_scalar_field uniform_scalar_field {
         mesh {
            in_dims => <-.<-.RegionResamplerCore.dims;
         };
         data {
            in_data => <-.<-.RegionResamplerCore.points;
         };
         DataObject.Obj.name => name_of(<-.<-.<-.<-);
      };

      olink out_fld => .uniform_scalar_field.out;
      olink out_obj => .uniform_scalar_field.obj;
   };

};


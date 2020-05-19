
flibrary CollapseMacs {

   macro CollapseUI {
      ilink in;
      XP_COMM_PROJ.Collapse.CollapseMods.CollapseParams &CollapseParams<NEportLevels={2,1}>;

      UImod_panel panel {
         parent<NEportLevels={4,0}>;
         title => "Collapse Field";
         message = "Select Collapse Field control panel.";
      };

      UIlabel CollapseLabel {
         parent => <-.panel;
         x = 0;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Collapse Field";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

      // Collapse Operation Radiobox
      UIradioBoxLabel CollapseOpRadioBox {
         parent => <-.panel;
         x => <-.CollapseLabel.x;
         y => <-.CollapseLabel.y + <-.CollapseLabel.height + 10;
         labels => {"Collapse All", "Slice", "Collapse Except"};
         selectedItem => <-.CollapseParams.operation;
         title => "Collapse Operation...";
      };

      UIslider AxisSlider {
         parent => <-.panel;
         x => <-.CollapseLabel.x;
         y => <-.CollapseOpRadioBox.y + <-.CollapseOpRadioBox.height + 15;
         title => "Collapse along Axis...";
         min = 0.;
         max = 2.;
         value => <-.CollapseParams.axis;
         mode = "integer";
         decimalPoints = 0;
      };

      UIslider PlaneSlider {
         parent => <-.panel;
         x => <-.CollapseLabel.x;
         y => <-.AxisSlider.y + <-.AxisSlider.height + 5;
         title => "Selected Plane...";
         min = 0.;
         max+nres => <-.in.points[1][<-.CollapseParams.axis];
         value => <-.CollapseParams.planes[0];
         mode = "integer";
         decimalPoints = 0;
      };
   };


   CollapseUI CollapsePlanesUI {
      CollapseOpRadioBox {
         labels => {"Collapse All", "Slice", "Collapse Except", "Collapse Only"};
      };

      -in;
      -PlaneSlider;
   };


   macro CollapseFieldFunc {
      ilink in;

      XP_COMM_PROJ.Collapse.CollapseMods.CollapseParams &CollapseParams<NEportLevels={2,1}>;

      XP_COMM_PROJ.Collapse.CollapseMods.CollapseFieldCore CollapseFieldCore {
         values+nres => <-.in.node_data[0].values;
         dims+nres   => <-.in.dims;
         CollapseParams => <-.CollapseParams;
      };

      FLD_MAP.uniform_vector_field uniform_vector_field {
         mesh {
            in_dims => <-.<-.CollapseFieldCore.out_dims;
         };
         data {
            in_data => <-.<-.CollapseFieldCore.out_values;
         };
         obj {
            name => name_of(<-.<-.<-.<-);
         };
      };

      olink out_field => .uniform_vector_field.out;
      olink out_obj   => .uniform_vector_field.obj;
   };



   macro collapse_field {
      ilink in;

      XP_COMM_PROJ.Collapse.CollapseMods.CollapseParams CollapseParams {
         planes = {0};
      };

      CollapseUI CollapseUI {
         in => <-.in;
         CollapseParams => <-.CollapseParams;
      };

      CollapseFieldFunc CollapseFieldFunc {
         in => <-.in;
         CollapseParams => <-.CollapseParams;
         uniform_vector_field {
            obj {
               name => name_of(<-.<-.<-.<-.<-);
            };
         };
      };

      olink out_field => .CollapseFieldFunc.out_field;
      olink out_obj   => .CollapseFieldFunc.out_obj;
   };



   collapse_field collapse_field_planes {
      imlink planes;

      CollapseParams {
         planes => <-.planes;
      };

      -CollapseUI;

      CollapsePlanesUI CollapsePlanesUI {
         CollapseParams => <-.CollapseParams;
      };
   };



   APPS.SingleWindowApp CollapseFieldEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.collapse_field.out_obj
               };
            };
            View {
               View {
                  renderer = "OpenGL";
               };
            };
         };
      };

      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/water.fld";
         };
      };

      XP_COMM_PROJ.Collapse.CollapseMacs.collapse_field collapse_field {
         in => <-.Read_Field.field;
      };

   };



   APPS.SingleWindowApp CollapseFieldPlanesEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.collapse_field_planes.out_obj
               };
            };
            View {
               View {
                  renderer = "OpenGL";
               };
            };
         };
      };

      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/water.fld";
         };
      };

      int+OPort planes[] = {10,11,12,13,14,15,16,17,18,19};

      XP_COMM_PROJ.Collapse.CollapseMacs.collapse_field_planes collapse_field_planes {
         in => <-.Read_Field.field;
         planes => <-.planes;
         CollapseParams {
            operation = "collapse_except";
         };
      };

   };


};


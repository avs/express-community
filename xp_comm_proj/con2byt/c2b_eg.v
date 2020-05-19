APPS.MultiWindowApp MultiWindowApp<NEdisplayMode="maximized"> {
   UI {
      shell {
         x = 0;
         y = 0;
      };
      Modules {
         IUI {
            optionList {
               selectedItem = 1;
            };
            mod_panel {
               x = 0;
               y = 0;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D<NEx=363.,NEy=418.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.volume_render.DataObject.obj};
            Xform {
               ocenter = {3.27323,4.16378,
2.86213};
               dcenter = {1.18071,1.50195,
1.03242};
               mat = {
                  -0.294013,0.044999,0.204085,0.,-0.0076254,0.349714,-0.0880944,
0.,-0.208848,-0.0761173,-0.284091,0.,0.,0.,0.,1.
               };
               xlate = {-1.18071,-1.50195,
-1.03242};
            };
         };
         View {
            View {
               renderer => "Software";
            };
            ViewUI {
               ViewPanel {
                  UI {
                     panel {
                        defaultX = 551;
                        defaultY = 247;
                     };
                  };
               };
            };
         };
      };
   };
   MODS.volume_render volume_render<NEx=275.,NEy=341.> {
      in_mesh => <-.extract_component.out_fld;
      inherit {
         set = 1;
      };
      surf_rb {
         x = 0;
      };
      ray_tracer {
         set = 1;
      };
      vol_rb {
         x = 0;
      };
      interp_tri {
         set = 1;
      };
      interp_rb {
         x = 0;
      };
      ray_direct {
         set = 1;
      };
      ray_rb {
         x = 0;
      };
      dist_glob {
         set = 1;
      };
      dist_rb {
         x = 0;
      };
      alpha_const {
         set = 1;
      };
      alpha_rb {
         x = 0;
      };
   };
   MODS.extract_component extract_component<NEx=198.,NEy=264.> {
      in_field => <-.Convert2Byte.out;
      ExtrCompUI {
         panel {
            option {
               set = 1;
            };
         };
         DVnode_data_labels {
            labels[];
         };
         UIradioBoxLabel {
            label_cmd {
               cmd[];
            };
         };
      };
   };
   IAC.FILTERS.Convert2Byte Convert2Byte<NEx=154.,NEy=198.> {
      in => <-.Read_Field.field;
   };
   MODS.Read_Field Read_Field<NEx=99.,NEy=88.> {
      read_field_ui {
         file_browser {
            x = 490;
            y = 267;
            width = 300;
            height = 390;
            ok = 1;
            dirMaskCache = "/usr/express/data/field/*.fld";
         };
         filename = "/usr/express/data/field/bluntfin.fld";
      };
      DVread_field {
         Mesh_Struct+Node_Data Output_Field;
      };
   };
};

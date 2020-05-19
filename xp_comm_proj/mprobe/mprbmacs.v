
flibrary MultiProbeMacs {

  // Functional Macro

  // UI Macro

  // User Macro

  macro multi_probe {
    mlink in_grids <NEportLevels={2,1},NEx=88.,NEy=66.>;

    XP_COMM_PROJ.MultiProbe.MultiProbeMods.MultiProbeMerge
        MultiProbeMerge <NEx=253.,NEy=165.> {
      in => <-.in_grids;
    };

    DataObject DataObject <NEx=220.,NEy=231.> {
      in => <-.MultiProbeMerge.out;
      Obj.name => name_of(<-.<-.<-);
      Obj.xform_mode = "Parent";
    };

    link out_fld <NEportLevels={1,2},NEx=341.,NEy=341.>
        => .MultiProbeMerge.out;

    link out_obj <NEportLevels={1,2},NEx=374.,NEy=286.>
        => .DataObject.obj;
  };

  // Example Application (SingleWindowApp)

  APPS.MultiWindowApp MultiProbeEg {
    UI {
      shell {
         x = 0;
         y = 0;
      };
      Modules {
         IUI {
            optionList {
               selectedItem = 3;
               cmdList => {
                  <-.<-.<-.<-.Box.box_ui.panel.option,
                  <-.<-.<-.<-.Sphere.sphere_ui.panel.option,
                  <-.<-.<-.<-.Read_Field.read_field_ui.panel.option,
                  <-.<-.<-.<-.combine_vect.CombVectUI.UIpanel.option,
                  <-.<-.<-.<-.bounds.UIpanel.option,
                  <-.<-.<-.<-.streamlines.UIpanel.option
               };
            };
            mod_panel {
               x = 0;
               y = 0;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D<NEx=603,NEy=453> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.multi_probe.out_obj,<-.<-.<-.bounds.out_obj,
               <-.<-.<-.streamlines.out_obj,<-.<-.<-.Box.out_obj,
               <-.<-.<-.Sphere.out_obj
            };
            Xform {
               ocenter = {3.27323,3.71608,
1.84716};
               dcenter = {1.18071,1.34046,
0.666305};
               mat = {
                  0.30887,0.0554002,-0.177895,0.,0.0157943,0.335379,0.131868,
0.,0.18565,-0.120702,0.284747,0.,0.,0.,0.,1.
               };
               xlate = {-1.10213,-1.43869,
-0.666305};
               center = {3.27323,3.71608,
1.84716};
            };
         };
         View {
            View {
               trigger = 2;
            };
            ViewUI {
               ViewPanel {
                  UI {
                     panel {
                        defaultX = 450;
                        defaultY = 448;
                     };
                  };
               };
            };
         };
         Camera {
            Camera {
               perspec = 1;
               front = 1.;
               auto_norm = "None";
            };
         };
      };
      Scene_Editor {
         View_Editor {
            GDobj_sel {
               curobj_name = "Top";
            };
            InfoPanel {
               curobj_sel {
                  selectedItem = 4;
                  x = 490;
                  y = 298;
                  width = 300;
                  height = 310;
                  ok = 1;
                  cancel = 1;
               };
            };
         };
         Camera_Editor {
            GDcamera_edit {
               auto_norm = "None";
               front = 1.;
            };
            IUI {
               optionList {
                  selectedItem = 0;
               };
               General {
                  IUI {
                     panel {
                        visible = 1;
                     };
                     CameraType {
                        OPcmdList = {
                           {
                              set=1,,,
                           },
                           };
                     };
                     Extents {
                        OPcmdList = {
                           {
                              set=1,,,
                           },
                           };
                     };
                     Mapping {
                        OPcmdList = {
                           {
                              set=1,,,
                           },
                           };
                     };
                     Normalize {
                        OPcmdList = {
                           {
                              set=1,,,
                           },
                           ,,};
                     };
                  };
               };
            };
         };
         Object_Editor {
            GDprops_edit {
               inherit = 0;
               line_width = 0;
            };
            IUI {
               optionList {
                  selectedItem = 3;
               };
               General {
                  IUI {
                     AltSpace {
                        OPcmdList = {
                           {
                              set=1,,,
                           },
                           };
                     };
                     TransformMode {
                        OPcmdList = {
                           {
                              set=1,,,
                           },
                           ,,};
                     };
                  };
               };
               Properties {
                  IUI {
                     panel {
                        visible = 1;
                     };
                     ObjectOptions = {
                        {
                           set=1,,,
                        },
                        };
                     EditAltProps {
                        y = 0;
                        x = 0;
                     };
                     Type {
                        IUI {
                           optionList {
                              selectedItem = 1;
                           };
                           General {
                              IUI {
                                 ColorOptions = {
                                    {
                                       set=1,,,
                                    },
                                    ,};
                                 ColorEditor {
                                    ColorEcho {
                                       ColorView {
                                          trigger = 1;
                                       };
                                    };
                                    rgb_or_hsv {
                                       s = 0.;
                                    };
                                 };
                              };
                           };
                           PointLine {
                              IUI {
                                 panel {
                                    visible = 1;
                                 };
                                 LineStyle {
                                    OPcmdList = {
                                       {
                                          set=1,,,
                                       },
                                       ,,};
                                 };
                                 DrawingMode {
                                    OPcmdList = {
                                       {
                                          set=1,,,
                                       },
                                       };
                                 };
                              };
                           };
                        };
                     };
                  };
               };
            };
         };
      };
   };
   XP_COMM_PROJ.MultiProbe.MultiProbeMacs.multi_probe multi_probe<NEx=473.,NEy=154.> {
      in_grids => {<-.Box.out_fld,
         <-.Sphere.bounds.out};
      DataObject {
         Props {
            col = {1.,0.,0.};
            inherit = 0;
            line_width = 6;
         };
      };
   };
   GEOMS.Box Box<NEx=352.,NEy=33.> {
      box {
         xform {
            mat = {
               2.80934,0.,0.,0.,0.,2.80934,0.,0.,0.,0.,2.80934,0.,0.,0.,
0.,1.
            };
            xlate = {5.28829,0.707233,
0.576004};
         };
      };
      dim1 = 3;
      dim2 = 3;
      dim3 = 3;
   };
   GEOMS.Sphere Sphere<NEx=572.,NEy=33.> {
      sphere {
         xform {
            mat = {
               0.287605,3.32413,1.6754,0.,1.6249,1.39974,-3.05613,0.,-3.3491,
0.964574,-1.33888,0.,0.,0.,0.,1.
            };
            xlate = {-2.36793,5.26114,
1.71992};
         };
      };
   };
   MODS.Read_Field Read_Field<NEx=99.,NEy=33.> {
      read_field_ui {
         file_browser {
            x = 490;
            y = 267;
            width = 300;
            height = 390;
            ok = 1;
            dirMaskCache = "$XP_ROOT/data/field/*.fld";
         };
         filename = "$XP_ROOT/data/field/bluntfin.fld";
      };
      DVread_field {
         Mesh_Struct+Node_Data Output_Field;
      };
   };
   MODS.combine_vect combine_vect<NEx=99.,NEy=121.> {
      in_field => <-.Read_Field.field;
      CombineVectParam {
         components = {1,2,3};
      };
      CombVectUI {
         DVnode_data_labels {
            labels[];
         };
         UIpanel {
            option {
               set = 1;
            };
         };
         UIoptionBoxLabel {
            label_cmd {
               cmd[] = {
                  ,,,
                  {
                     set=1,,,
                  },
               };
            };
         };
      };
   };
   MODS.bounds bounds<NEx=198.,NEy=198.> {
      in_field => <-.combine_vect.out_fld;
   };
   MODS.streamlines streamlines<NEx=77.,NEy=330.> {
      in_field => <-.combine_vect.out_fld;
      in_probe => <-.multi_probe.out_fld;
   };
 };
};




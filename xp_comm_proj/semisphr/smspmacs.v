
flibrary SemiSphereMacs {

  // define the example application

  APPS.MultiWindowApp SemiSphereEg {
   UI {
      shell {
         x = 0;
         y = 0;
      };
      Modules {
         IUI {
            panel {
               visible = 0;
            };
            mod_panel {
               x = 0;
               y = 0;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.DataObject.obj};
            Modes {
               mode = {0,1,0,0,0};
            };
            Xform {
               ocenter = {4.16152,2.21423,50.5172};
               dcenter = {0.224514,0.119458,2.72541};
               mat = {
                  0.0318937,0.0120302,-0.0418175,0.,0.0180831,-0.0508229,-0.000829294,0.,-0.0395783,-0.0135261,-0.0340772,0.,0.,0.,0.,1.
               };
               xlate = {-0.224514,-0.119458,-2.72541};
               center = {4.16152,2.21423,50.5172};
            };
         };
         Camera {
            Camera {
               auto_norm = "None";
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
                        defaultX = 63;
                        defaultY = 395;
                     };
                  };
               };
            };
         };
      };
      Scene_Editor {
         Object_Editor {
            GDmodes_edit {
               lines = "None";
            };
            IUI {
               panel {
                  visible = 1;
               };
               optionList {
                  selectedItem = 2;
               };
               General {
                  IUI {
                     Pickable {
                        OPcmdList = {
                           ,
                           {
                              set=1,,,
                           },
                           ,,
                        };
                     };
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
               Modes {
                  IUI {
                     panel {
                        visible = 1;
                     };
                     EditAltModes {
                        OPcmdList = {
                           {
                              set=1,,,
                           },
                           };
                     };
                     Point {
                        OPcmdList = {
                           
                           {
                              set=1,,,
                           },
                           ,,,,,
                        };
                     };
                     Line {
                        OPcmdList = {
                           ,
                           {
                              set=1,,
                           },
                           ,,,
                        };
                     };
                     Surface {
                        OPcmdList = {
                           
                           {
                              set=1,,,
                           },
                           ,,,,
                        };
                     };
                     Volume {
                        OPcmdList = {
                           {
                              set=1,,,
                           },
                           ,,};
                     };
                     Bounds {
                        OPcmdList = {
                           {
                              set=1,,,
                           },
                           ,};
                     };
                     Normals {
                        OPcmdList = {
                           {
                              set=1,,,
                           },
                           ,};
                     };
                  };
               };
            };
         };
      };
   };

   XP_COMM_PROJ.SemiSphere.SemiSphereMods.modSemiSphere modSemiSphere {
      num_thetas = 20;
      r_outside = 100.;
      r_inside = 60.;
      z_offset = 0.;
      half_angle = 90.;
      out {
         xform {
            ocenter = {0.,0.,49.99};
            dcenter = {0.,0.,37.0639};
            mat = {
               0.741426,0.,0.,0.,0.,0.741426,0.,0.,0.,0.,0.741426,0.,0.,0.,0.,1.
            };
            xlate = {4.16152,2.21422,13.4533};
            center = {0.,0.,49.99};
         };
      };
   };
   GDM.DataObject DataObject {
      in => <-.modSemiSphere.out;
      Modes {
         mode = {0,2,0,0,0};
      };
      Obj {
         active = 0;
      };
   };
  };


};

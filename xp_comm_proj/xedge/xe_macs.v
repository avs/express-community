
flibrary XedgeMacs <compile_subs=0> {

   macro  XedgeUI {

      XP_COMM_PROJ.Xedge.XedgeMods.XedgeParams &params<NEportLevels={2,1}>;

      UImod_panel panel {
         title = "eXternal Edges with Silhouette";
      };

      UIlabel ExtEdgeTitle {
         parent => <-.panel;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "eXternal Edges w/ Silhouette Control";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

 
      UIslider UIslider {
         parent => <-.panel;
         min = 0;
         max = 181.;
         value => <-.params.angle;
         x = 0;
         y => <-.ExtEdgeTitle.y+<-.ExtEdgeTitle.height+20;
         title = "Angle Threshold (degrees)";
      };
   
      UItoggle UItoggle_x {
         parent => <-.panel;
         label = "X-axis Silhouette";
         x = 0;
         y => <-.UIslider.y + UIslider.height + 20;
         width = 200;
         set => <-.params.xtest;
      };
      UItoggle UItoggle_y {
         parent => <-.panel;
         label = "Y-axis Silhouette";
         x = 0;
         y => <-.UItoggle_x.y + UItoggle_x.height + 20;
         width = 200;
         set => <-.params.ytest;
      };
      UItoggle UItoggle_z {
         parent => <-.panel;
         label = "Z-axis Silhouette";
         x = 0;
         y => <-.UItoggle_y.y + UItoggle_y.height + 20;
         width = 200;
         set => <-.params.ztest;
      };

   };



   macro XedgeFunc {
      ilink in_field;

      XP_COMM_PROJ.Xedge.XedgeMods.XedgeParams &params<NEportLevels={2,1}>;

      XP_COMM_PROJ.Xedge.XedgeMods.XedgeCore XedgeCore {
      	in => <-.in_field;
         params => <-.params;
      };

      GDM.DataObject obj {
         in => <-.XedgeCore.out;
         Obj {
            name => name_of(<-.<-.<-);
         };
      };

      olink out_fld => XedgeCore.out;
      olink out_obj => obj.obj;
   };




   macro xedge {
      ilink in_field;

      XP_COMM_PROJ.Xedge.XedgeMods.XedgeParams &params {
         angle = 91.0;
         xtest = 0;
         ytest = 0;
         ztest = 0;
      };

      XedgeUI XedgeUI {
         params => <-.params;
      };

      XedgeFunc XedgeFunc {
         in_field => <-.in_field;
         params => <-.params;
         obj {
            Obj {
               name => name_of(<-.<-.<-.<-);
            };
         };
      };

      olink out_fld => .XedgeFunc.out_fld;
      olink out_obj => .XedgeFunc.out_obj;
   };



   APPS.MultiWindowApp XedgeEg<NEdisplayMode="maximized"> {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.xedge.out_obj};
            };
         };
         Scene_Selector {
            input_views => {Scene.View.View, <-.OriginalScene.View.View};
         };
         GDM.Uscene3D OriginalScene {
            Top {
               child_objs<NEportLevels={4,1}> => {<-.<-.<-.Read_UCD.out_obj};
            };
         };
      };


      MODS.Read_UCD Read_UCD {
         read_ucd_ui {
            filename = "$XP_PATH<0>/data/ucd/yoke.inp";
         };
      };

      XP_COMM_PROJ.Xedge.XedgeMacs.xedge xedge {
         in_field => <-.Read_UCD.field;
         params {
            angle = 181.0;
            xtest = 1;
         };
      };
   };


};


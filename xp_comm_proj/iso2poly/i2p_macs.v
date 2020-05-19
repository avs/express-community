
flibrary Isoline2PolylineMacs <compile_subs=0> {
 
   macro Isoline2PolylineUI {
      XP_COMM_PROJ.Isoline2Polyline.Isoline2PolylineMods.Isoline2PolylineParams &params<NEportLevels={2,1}>;

      UImod_panel panel {
         title => "Isoline 2 Polyline";
         parent<NEportLevels={4,0}>;
      };

      UIslider Downsize {
         parent => <-.panel;
         value => params.Downsize;
         y = 0;
         mode = "integer";
         min = 1;
         max = 10;
      };
      UItoggle Automatic_Close {
         parent => <-.panel;
         set => params.AutoClose;
         y => Downsize.y + Downsize.height + 10;
      };
      UItoggle Use_Node_Data {
         parent => <-.panel;
         set => params.UseNodeData;
         y => Automatic_Close.y + Automatic_Close.height + 5;
      };

   };


   macro Isoline2PolylineFunc {
      ilink in_fld;
      XP_COMM_PROJ.Isoline2Polyline.Isoline2PolylineMods.Isoline2PolylineParams &params<NEportLevels={2,1}>;

      XP_COMM_PROJ.Isoline2Polyline.Isoline2PolylineMods.Isoline2PolylineCore Isoline2PolylineCore {
         in_fld => <-.in_fld;
         params => <-.params;
      };
      GDM.DataObject DataObject {
         in => Isoline2PolylineCore.out;
         obj {
            name => name_of(<-.<-.<-);
         };
      };

      olink out_fld => Isoline2PolylineCore.out;
      olink out_obj => DataObject.obj;
   };


   macro isoline2polyline {
      ilink in_fld;

      XP_COMM_PROJ.Isoline2Polyline.Isoline2PolylineMods.Isoline2PolylineParams params {
         Active = 1;
         Downsize = 1;
         UseNodeData = 1;
         AutoClose = 1;
      };

      Isoline2PolylineUI Isoline2PolylineUI {
         params => <-.params;
      };

      Isoline2PolylineFunc Isoline2PolylineFunc {
         in_fld => <-.in_fld;
         params => <-.params;
         DataObject {
            obj {
               name => name_of(<-.<-.<-.<-);
            };
         };
      };

      olink out_fld => Isoline2PolylineFunc.out_fld;
      olink out_obj => Isoline2PolylineFunc.out_obj;
   };

   macro expand_polyline_coords {
      Mesh+Cells &in_fld<NEportLevels={2,1}>;

      FLD_MAP.extract_coordinate_array extract_coordinate_array {
         -UIpanel;
         -UIoptionBoxLabel;

         in => <-.in_fld;
         comp = {0, 1, 2};
      };

      FLD_MAP.polyline_mesh polyline_mesh {
         coord => <-.extract_coordinate_array.coord;
         connect => <-.in_fld.cell_set[0].poly_connect_list;
      };

      olink out_fld => polyline_mesh.out;
      olink out_obj => polyline_mesh.obj;
   };

};


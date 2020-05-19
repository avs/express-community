
flibrary Sct2UnifMacs <compile_subs=0>
{


// Sct2UnifEg1:
// ------------
// Demonstrates the use of the sct2unif module with 3D data.
// Also demonstrates rounding to the nearest uniform grid node
// and correct copying of multiple sets of node data.

APPS.MultiWindowApp Sct2UnifEg1 {

   GDM.Uviewer Uviewer {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Bubbleviz.out_obj,<-.<-.<-.Axis3D.out_obj
            };
         };
         Top2D {
            child_objs => {
               <-.<-.<-.TextString.DefaultObject
            };
         };
         Camera {
            Camera {
               perspec = 1;
               front = 1.;
            };
         };
      };
      Scene_Selector {
         input_views => {<-.Scene.View.View, <-.Scene#1.View.View};
      };
      GDM.Uscene Scene#1 {
         Top {
            child_objs<NEportLevels={4,1}> => {
               <-.<-.<-.Bubbleviz#1.out_obj,<-.<-.<-.Axis3D#1.out_obj
            };
         };
         Top2D {
            child_objs<NEportLevels={4,1}> => {
               <-.<-.<-.TextString#1.DefaultObject
            };
         };
         Camera {
            Camera {
               perspec = 1;
               front = 1.;
            };
         };
      };
   };


   Field_Struct+OPort Simple3DField {
      coordinates {
         values => {
            -0.1,0.1,0.2,1.1,1.2,1.9
         };
      };
      nspace = 3;
      dims => {2};
      ndim = 1;
      node_data = {
         {
            labels="component0",,veclen=1,values=>{1,2},,,,,,,
         },
         {
            labels="component1",,veclen=1,values=>{3,4},,,,,,,
         }
      };
      nnode_data = 2;
   };

//---------------------------------------------------------------------------//

   HLM.Bubbleviz Bubbleviz {
      glyph {
         in_field => <-.<-.Simple3DField;
         GlyphParam {
            scale = 0.16;
            map_comp = 1;
         };
      };
      Sphere {
         subdiv = 5;
      };
   };

   GEOMS.Axis3D Axis3D {
      in_field => <-.Simple3DField;
      x_axis_param {
         start => -0.5;
         end => 1.5;
         ref => -0.5;
         step => 0.5;
      };
      y_axis_param {
         start => 0;
         end => 1.5;
         ref => 0;
         step => 0.5;
      };
      z_axis_param {
         start => 0;
         end => 2;
         ref => 0;
         step => 0.5;
      };
   };

   GEOMS.TextString TextString {
      text {
         str => "Before sct2unif";
         position = {-0.5,4.};
      };
   };

//---------------------------------------------------------------------------//

   double+Port fill_value = 0.;

   XP_COMM_PROJ.Sct2Unif.Sct2UnifMods.sct2unif sct2unif {
      in_fld => <-.Simple3DField;
      fill_value => <-.fill_value;
   };

   HLM.Bubbleviz Bubbleviz#1 {
      glyph {
         in_field => <-.<-.sct2unif.out_fld;
         GlyphParam {
            scale = 0.16;
            map_comp = 1;
         };
      };
      Sphere {
         subdiv = 5;
      };
   };

   GEOMS.Axis3D Axis3D#1 {
      in_field => <-.sct2unif.out_fld;
      x_axis_param {
         step => 1.;
      };
      y_axis_param {
         step => 1.;
      };
      z_axis_param {
         step => 1.;
      };
   };

   GEOMS.TextString TextString#1 {
      text {
         str => "After sct2unif";
         position = {-0.5,4.};
      };
   };

};



// Sct2UnifEg2:
// ------------
// Demonstrates the use of the sct2unif module with 2D data.
// This example particularly demonstrates how nodes are mapped
// to the nearest uniform grid node.

APPS.MultiWindowApp Sct2UnifEg2 {

   GDM.Uviewer2D Uviewer2D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Bubbleviz.out_obj,<-.<-.<-.Axis2D.out_obj
            };
         };
         GDM.GroupObject Top#1<export_subs=1,export=1> {
            child_objs<NEportLevels={4,1},NEcolor0=16711680> => {
               <-.<-.<-.TextString.DefaultObject
            };
            obj<export=0>;
         };
         Camera {
            Camera {
               objects => {<-.<-.Top.obj,<-.<-.Top#1.obj};
            };
         };
      };
      Scene_Selector {
         input_views => {<-.Scene.View.View,<-.Scene#1.View.View};
      };
      GDM.Uscene2D Scene#1 {
         Top {
            child_objs<NEportLevels={4,1}> => {
               <-.<-.<-.Bubbleviz#1.out_obj,<-.<-.<-.Axis2D#1.out_obj
            };
         };
         GDM.GroupObject Top#1<export_subs=1,export=1> {
            child_objs<NEportLevels={4,1},NEcolor0=16711680> => {
               <-.<-.<-.TextString#1.DefaultObject
            };
            obj<export=0>;
         };
         Camera {
            Camera {
               objects => {<-.<-.Top.obj,<-.<-.Top#1.obj};
            };
         };
      };
   };


   Field_Struct+OPort Simple2DField {
      coordinates {
         values<NEdisplayMode="open"> => {0.,1.4,1.6,1.};
      };
      nspace = 2;
      dims => {2};
      ndim = 1;
      node_data = {
         {
            labels="component0",,veclen=1,values=>{1,2},,,,,,,
         }
      };
      nnode_data = 1;
   };

//---------------------------------------------------------------------------//

   HLM.Bubbleviz Bubbleviz {
      glyph {
         in_field => <-.<-.Simple2DField;
         GlyphParam {
            scale = 0.1;
         };
      };
      Sphere {
         subdiv = 5;
      };
   };
   GEOMS.Axis2D Axis2D {
      in_field => <-.Simple2DField;
      x_axis_param {
         step => 0.5;
         minor_ticks = 0;
      };
      y_axis_param {
         ref => 0.;
         start => 0.;
         step => 0.5;
         minor_ticks = 0;
      };
   };
   GEOMS.TextString TextString {
      text {
         str => "Before sct2unif";
         position = {-0.5,4.};
      };
   };

//---------------------------------------------------------------------------//

   double+Port fill_value = 0.;

   XP_COMM_PROJ.Sct2Unif.Sct2UnifMods.sct2unif sct2unif {
      in_fld => <-.Simple2DField;
      fill_value => <-.fill_value;
   };


   HLM.Bubbleviz Bubbleviz#1 {
      glyph {
         in_field => <-.<-.sct2unif.out_fld;
         GlyphParam {
            scale = 0.2;
         };
      };
   };
   GEOMS.Axis2D Axis2D#1 {
      in_field => <-.sct2unif.out_fld;
      x_axis_param {
         ref => -1.;
         step => 1.;
         minor_ticks = 0;
      };
      y_axis_param {
         ref => 0.;
         step => 1.;
         minor_ticks = 0;
      };
   };
   GEOMS.TextString TextString#1 {
      text {
         str => "After sct2unif";
         position = {-0.5,4.};
      };
   };

};



// Sct2UnifEg3:
// ------------
// Demonstrates the use of the sct2unif module with 3D UCD data.
// This example demonstrates an instance when the interpolating
// behaviour of the standard AVS scat_to_unif macro can obscure
// the data being visualised.

APPS.MultiWindowApp Sct2UnifEg3 {

   GDM.Uviewer3D Uviewer3D {
      Scene {
         Top {
            child_objs => {
               external_edges.out_obj,<-.<-.<-.orthoslice.out_obj,<-.<-.<-.orthoslice#1.out_obj
            };
         };
         Camera {
            Camera {
               perspec = 1;
               front = 1.;
            };
         };
      };
   };

   MODS.Read_UCD Read_UCD {
      read_ucd_ui {
         filename = "$XP_PATH<0>/data/ucd/avs.inp";
      };
   };

   MODS.external_edges external_edges {
      in_field => <-.Read_UCD.field;
   };

//---------------------------------------------------------------------------//

   MODS.scat_to_unif scat_to_unif {
      in_field => <-.Read_UCD.field;
      Scat2UnifParam {
         dim1 = 25;
         dim2 = 10;
         dim3 = 30;
         search_dist1 => 1;
         search_dist2 => 1;
         search_dist3 => 1;
         interp_order = 1;
         null_value => 24.;
      };
   };

   MODS.orthoslice orthoslice {
      in_field => <-.scat_to_unif.out_fld;
      OrthoSliceParam {
         plane => 0;
         axis = 2;
      };
      obj {
         Modes {
            mode = {0,2,0,0,0};
         };
      };
   };

//---------------------------------------------------------------------------//

   XP_COMM_PROJ.Sct2Unif.Sct2UnifMods.field2scat field2scat {
      in_fld => <-.Read_UCD.field;
   };

   double+Port fill_value = 0.;

   XP_COMM_PROJ.Sct2Unif.Sct2UnifMods.sct2unif sct2unif {
      in_fld => <-.field2scat;
      fill_value => <-.fill_value;
   };

   MODS.orthoslice orthoslice#1 {
      in_field => <-.sct2unif.out_fld;
      OrthoSliceParam {
         plane => 31;
         axis = 2;
      };
      obj {
         Modes {
            mode = {0,2,0,0,0};
         };
      };
   };

};


};


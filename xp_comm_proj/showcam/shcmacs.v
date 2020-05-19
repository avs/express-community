
flibrary ShowcamMacs <compile_subs=0> {

   // Functional Macro that takes a glyph input, calculates the appropriate xform
   // and outputs the transformed glyph as a mesh and a DataObject
   macro showcam {
      ilink camera_in;
      ilink glyph_in;

      XP_COMM_PROJ.Showcam.ShowcamMods.prep_cam_xform prep_cam_xform {
         camera_in => <-.camera_in;
      };
      MODS.set_xform set_xform {
         in_field1 => <-.glyph_in;
         in_field2 => <-.prep_cam_xform.mesh_out;
      };

      olink out_fld => .set_xform.out_fld;
      olink out_obj => .set_xform.out_obj;
   };



   // Example Application that shows how showcam can be used
   // to display the camera position of one view in another
   APPS.MultiWindowApp ShowcamEg {

      GDM.Uviewer3D Uviewer3D<NEx=33.,NEy=121.> {
         Scene {
            Top {
               child_objs<NEportLevels={4,1}> => {<-.<-.<-.Read_Geom.geom};
               Xform<NEportLevels={0,4}> {
                  mat = {
                     0.992879,-0.00198178,0.119115,0.,
                     0.108787,0.422614,-0.899758,0.,
                     -0.0485565,0.906308,0.41982,0.,
                     0.,0.,0.,1.
                  };
               };
            };
            Camera {
               Camera<NEportLevels={0,4}> {
                  from = {5.76523,1.61586,5.74038};
                  perspec = 1;
                  front = 1.;
                  up = {-0.389588,0.911068,0.134819};
                  scale = 0.9999993443;
                  auto_norm = "None";
               };
               CameraXform {
                  mat = {
                     0.60425,-0.389588,0.695057,0.,
                     0.363325,0.911067,0.194807,0.,
                     -0.709139,0.134819,0.69206,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {0.,0.,-8.29463};
               };
            };
         };

         Scene_Selector {
            input_views => {Scene.View.View,<-.CameraScene.View.View};
         };

         GDM.Uscene3D CameraScene {
            Top {
               child_objs<NEportLevels={4,1}> => {<-.<-.<-.CameraGroupObject.obj,<-.<-.<-.showcam.out_obj};
               Xform {
                  mat = {
                     0.557995,-0.0675218,0.144674,0.,
                     0.0907948,0.566806,-0.085652,0.,
                     -0.131324,0.10498,0.555501,0.,
                     0.,0.,0.,1.
                  };
               };
            };
            Camera {
               Camera {
                  auto_norm = "None";
                  perspec = 1;
                  front = 1.;
                  from = {0.,0.,13.5311};
               };
               CameraXform {
                  xlate = {0.,0.,-13.5311};
               };
            };
         };
      };


      MODS.Read_Geom Read_Geom<NEx=33.,NEy=22.> {
         read_geom_ui {
            filename = "$XP_PATH<0>/data/geom/teapot.geo";
         };
      };


      // Geometric Camera Glyph.  Could also use other glyphs if desired.
      XP_COMM_PROJ.Showcam.ShowcamMods.CameraGlyph CameraGlyph<NEx=297.,NEy=121.>;

      // Translate camera glyph to camera position.
      XP_COMM_PROJ.Showcam.ShowcamMacs.showcam showcam<NEx=242.,NEy=198.> {
         camera_in => <-.Uviewer3D.Scene.Camera.Camera;
         glyph_in => <-.CameraGlyph.out_fld;
      };


      // Objects in the first scene may also be transformed.
      // Before they can be displayed in the CameraScene the original objects
      // must be transformed by the same transformation.
      // This is done by grouping all display objects together and applying
      // the extracted scene transform.

      // NOTE: If other objects are added to the first Scene they should also be
      //       added to this GroupObject
      GDM.GroupObject CameraGroupObject<NEx=33.,NEy=198.> {
         child_objs => {<-.Read_Geom.geom};
         &Xform<NEportLevels={2,1}> => <-.Uviewer3D.Scene.Top.Xform;
      };

   };

};


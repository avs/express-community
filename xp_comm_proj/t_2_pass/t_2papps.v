
flibrary Transparency2PassApps <compile_subs=0> {

   // example application
   APPS.SingleWindowApp TransparencyEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  switch(<-.<-.<-.transparency_type,
                         <-.<-.<-.Transparency1Pass.out_obj,
                         <-.<-.<-.Transparency2Pass.out_obj,
                         <-.<-.<-.ReverseTransparency2Pass.out_obj)
               };
            };
         };
      };


      MODS.Read_Geom Read_Geom {
         read_geom_ui {
            filename = "$XP_PATH<0>/data/geom/teapot.geo";
         };
      };

      int+OPort   transparency_type = 2;
      float+OPort transparency = 0.4;
   

      XP_COMM_PROJ.Transparency2Pass.Transparency2PassMacs.TransparencyEgUI TransparencyEgUI {
         transparency_type => <-.transparency_type;
         transparency => <-.transparency;
      };


      XP_COMM_PROJ.Transparency2Pass.Transparency2PassMacs.Transparency1Pass Transparency1Pass {
         in_fld => <-.Read_Geom.field;
         transparency => <-.transparency;
      };
      XP_COMM_PROJ.Transparency2Pass.Transparency2PassMacs.Transparency2Pass Transparency2Pass {
         in_fld => <-.Read_Geom.field;
         transparency => <-.transparency;
      };
      XP_COMM_PROJ.Transparency2Pass.Transparency2PassMacs.ReverseTransparency2Pass ReverseTransparency2Pass {
         in_fld => <-.Read_Geom.field;
         transparency => <-.transparency;
      };
   };



   // object ordering example application
   APPS.SingleWindowApp TransparencyOrderingEg {

      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  switch(<-.<-.<-.object_order, <-.<-.<-.Transparency2Pass#1.out_obj, <-.<-.<-.Transparency2Pass#2.out_obj),
                  switch(<-.<-.<-.object_order, <-.<-.<-.Transparency2Pass#2.out_obj, <-.<-.<-.Transparency2Pass#1.out_obj)
               };
            };
         };
      };

      int+OPort   object_order = 1;
      float+OPort transparency = 0.75;


      MODS.Read_Geom Read_Geom {
         read_geom_ui {
            filename = "$XP_PATH<0>/data/geom/teapot.geo";
         };
      };


      XP_COMM_PROJ.Transparency2Pass.Transparency2PassMacs.TransparencyOrderingEgUI TransparencyOrderingEgUI {
         object_order => <-.object_order;
         transparency => <-.transparency;
      };

      XP_COMM_PROJ.Transparency2Pass.Transparency2PassMacs.Transparency2Pass Transparency2Pass#1 {
         in_fld => <-.Read_Geom.field;
         transparency => <-.transparency;
      };

      MODS.scale scale {
         in_field => <-.Read_Geom.field;
         ScaleParam {
            scale_x = 2.;
            scale_y = 2.;
            scale_z = 2.;
         };
      };

      XP_COMM_PROJ.Transparency2Pass.Transparency2PassMacs.Transparency2Pass Transparency2Pass#2 {
         in_fld => <-.scale.out_fld;
         transparency => <-.transparency;
      };
   };

};


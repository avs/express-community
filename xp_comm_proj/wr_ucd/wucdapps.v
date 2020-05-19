
flibrary WriteUCDApps <compile_subs=0> {

   APPS.SingleWindowApp WriteUCDEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.isosurface.out_obj
               };
            };
         };
      };

      MODS.Read_Volume Read_Volume {
         read_volume_ui {
            filename = "$XP_PATH<0>/data/volume/hydrogen.dat";
         };
      };
      MODS.bounds bounds {
         in_field => <-.Read_Volume.field;
      };
      MODS.downsize downsize {
         in_field => <-.Read_Volume.field;
      };
      MODS.isosurface isosurface {
         in_field => <-.downsize.out_fld;
      };

      XP_COMM_PROJ.WriteUCD.WriteUCDMacs.write_ucd write_ucd {
         in_fld => <-.isosurface.out_fld;
      };
   };


   APPS.ModuleStack WriteUCDTest {
      MODS.Read_UCD Read_UCD {
         read_ucd_ui {
            filename = "$XP_PATH<0>/data/ucd/bluntfin.inp";
         };
      };
      MODS.combine_vect combine_vect {
         in_field => <-.Read_UCD.field;
         CombineVectParam {
            components = {1,2,3};
         };
      };
      MODS.combine_comp combine_comp {
         in_field1 => <-.Read_UCD.field;
         in_field2 => <-.combine_vect.out_fld;
         CombCompParam {
            comp1 = {0,4};
            comp2 = {0};
         };
      };
      XP_COMM_PROJ.WriteUCD.WriteUCDMacs.write_ucd write_ucd {
         in_fld => <-.combine_comp.out_fld;
         filename = "$XP_PATH<1>/test_comb.inp";
      };
   };



   APPS.SingleWindowApp WriteUCD_ARR_Eg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.isosurface.out_obj,
                  <-.<-.<-.orthoslice.out_obj
               };
            };
         };
      };

      MODS.Read_Volume Read_Volume {
         read_volume_ui {
            filename = "$XP_PATH<0>/data/volume/hydrogen.dat";
         };
      };
      MODS.bounds bounds {
         in_field => <-.Read_Volume.field;
      };
      MODS.downsize downsize {
         in_field => <-.Read_Volume.field;
      };
      MODS.isosurface isosurface {
         in_field => <-.downsize.out_fld;
      };
      MODS.orthoslice orthoslice {
         in_field => <-.downsize.out_fld;
      };

      XP_COMM_PROJ.WriteUCD.WriteUCDMacs.write_ucd_ARR write_ucd_ARR {
         in_flds => {
            <-.bounds.out_fld,
            <-.isosurface.out_fld,
            <-.orthoslice.out_fld
         };
         filename = "$XP_PATH<1>/test_arr.inp";
      };

   };



   APPS.SingleWindowApp ReadUCDEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_UCD.out_obj
               };
            };
         };
      };


      MODS.Read_UCD Read_UCD {
         read_ucd_ui {
            filename = "$XP_PATH<1>/test.inp";
         };
      };
   };

};


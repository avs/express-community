
flibrary Scl2TexApps
{

   APPS.SingleWindowApp Scalar2TextureEg {

      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Scalar2Texture.out_obj,
                  <-.<-.<-.isoline.out_obj
               };
            };
         };
      };

      MODS.Read_UCD Read_UCD {
         read_ucd_ui {
            filename = "$XP_PATH<0>/data/ucd/yoke.inp";
         };
      };

      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<1>/xp_comm_proj/scl2tex/rainbow.jpg";
            };
         };
      };

      MODS.extract_scalar extract_scalar {
         in_field => <-.Read_UCD.field;
         ExtractScalarParam {
            vector = 2;
            component = 0;
         };
      };

      XP_COMM_PROJ.Scalar2Texture.Scl2TexMacs.Scalar2Texture Scalar2Texture {
         in_fld => <-.extract_scalar.out_fld;
         in_image => <-.Read_Image.field;
      };

      MODS.external_faces external_faces {
         in_field => <-.extract_scalar.out_fld;
      };

      MODS.isoline isoline {
         in_field => <-.external_faces.out_fld;
         IsoParam {
            level_min => -100;
            level_max => 100;
            color = 0;
         };
      };

   };



   Scalar2TextureEg s2tComparisonEg {

      Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.BreakableLink1.out,
                  <-.<-.<-.BreakableLink2.out,
                  <-.<-.<-.isoline.out_obj
               };
            };
         };
      };

      XP_COMM_PROJ.Common.UIWidgets.IAC_StandardUI Scalar2TextureEgUI {
         panel {
            parent+IPort3;
            message = "Select ComparisonEg control panel.";
         };
         mainTitleLabel {
            label = "Texture/Colormap Toggle";
         };
         UIbutton Toggle_Coloring {
            parent => <-.panel;
            do+OPort3;
         };
      };

      XP_COMM_PROJ.Common.LogicWidgets.Toggle Toggle {
         trigger => <-.Scalar2TextureEgUI.Toggle_Coloring.do;
      };

      XP_COMM_PROJ.Common.NetworkWidgets.BreakableLink BreakableLink1 {
         in => <-.Scalar2Texture.out_obj;
         connect => <-.Toggle.value_true;
         disconnect => <-.Toggle.value_false;
         BreakLinkCore {
            out => in;
         };
      };
      XP_COMM_PROJ.Common.NetworkWidgets.BreakableLink BreakableLink2 {
         in => <-.extract_scalar.out_obj;
         connect => <-.Toggle.value_false;
         disconnect => <-.Toggle.value_true;
         BreakLinkCore {
            out => in;
         };
      };

   };

};


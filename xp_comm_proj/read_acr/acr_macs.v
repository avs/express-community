
flibrary ACR_NEMA_Macs {

  //   UI_acr_nema
  //   Read_acr_nema 
  //   Read_ACR_NEMA_Eg
 
     macro UI_acr_nema {
        UImod_panel UImod_panel {
           option {
              set = 1;
           };
           height = 984;
           message = "This module reads ACR NEMA files of type SPI";
           title => "Read ACR NEMA";
        };
        UIfileDialog UIfileDialog {
           visible => <-.UIbutton.do;
           filename => <-.file_name;
           x = 8;
           y = 326;
           width = 300;
           height = 390;
           cancel = 1;
           searchPattern => "*.acr-nema";
        };
        UIbutton UIbutton {
           parent => <-.UImod_panel;
           label => "Browse Files";
           y => ((<-.UIlabel_file_dialog.height + <-.UIlabel_file_dialog.y) + 5);
           x = 30;
        };
        UIlabel UIlabel_title {
           parent => <-.UImod_panel;
           label => "Reads ACR NEMA files of type SPI";
           y = 10;
           x = 10;
           width = 220;
           message = "This module reads ACR NEMA files of type SPI";
        };
        UIlabel UIlabel_file_dialog {
           parent => <-.UImod_panel;
           label => "File Name";
           y => ((<-.UIlabel_title.height + <-.UIlabel_title.y) + 10);
        };
        UItext UItext_filename {
           parent => <-.UImod_panel;
           text => <-.file_name;
           y => ((<-.UIbutton.height + <-.UIbutton.y) + 5);
           x = 5;
           width => (<-.UImod_panel.width - 20);
        };
        string file_name<NEportLevels={1,2}>;
     };

     macro Read_acr_nema {
       XP_COMM_PROJ.Read_ACR_NEMA.ACR_NEMA_Mods.read_acr_nema read_acr_nema {
           filename => <-.UI_acr_nema.file_name;
        };
        XP_COMM_PROJ.Read_ACR_NEMA.ACR_NEMA_Macs.UI_acr_nema UI_acr_nema;
        FLD_MAP.uniform_scalar_field uniform_scalar_field<NEx=341.,NEy=363.> {
           mesh {
              in_dims => <-.<-.read_acr_nema.dims_out;
           };
           data {
              in_data => <-.<-.read_acr_nema.short_array;
           };
           out<NEportLevels={0,3}>;
           obj<NEportLevels={1,3}>;
        };
     };

     APPS.SingleWindowApp Read_ACR_NEMA_Eg<NEdisplayMode="maximized"> {
        UI {
           shell {
              x = 88;
              y = 224;
           };
        };
        XP_COMM_PROJ.Read_ACR_NEMA.ACR_NEMA_Macs.Read_acr_nema Read_acr_nema<NEx=253.,NEy=99.> {
           UI_acr_nema {
              file_name = "xp_comm_proj/read_acr/image.acr-nema";
              UIfileDialog {
                 ok = 1;
                 dirMaskCache = "xp_comm_proj/read_acr/*.acr-nema";
              };
           };
        };
        GDM.Uviewer2D Uviewer2D<NEx=407.,NEy=363.> {
           Scene {
              Top {
                 child_objs => {
                    <-.<-.<-.Read_acr_nema.uniform_scalar_field.obj};
                   };
           };
        };
     };

};

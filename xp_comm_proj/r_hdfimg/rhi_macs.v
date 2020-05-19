
flibrary ReadHDFImageMacs <libdeps="GD"> {

  // ReadHDFImageUI
  //   UI Macro
  //   Components for parameters of the ReadHDFImage module.

  macro ReadHDFImageUI {
    UImod_panel UImod_panel<NEx=341.,NEy=55.> {
      title = "ReadHDFImage";
    };

    XP_COMM_PROJ.ReadHDFImage.ReadHDFImageMods.ReadHDFImageParams 
        &ReadHDFImageParams <NEx=99.,NEy=55.,NEportLevels={2,1}>;

    UIlabel label_select <NEx=11.,NEy=165.> {
      y = 0;
      width = 250;
      parent => <-.UImod_panel;
      label => "Select HDF Image File";
      alignment = 0;
    };

    UItext hdfi_file <NEx=22.,NEy=209.> {
      parent => <-.UImod_panel;
      y = 24;
      width = 170;
      height = 40;
      text<NEportLevels={2,2}> => <-.ReadHDFImageParams.filename;
    };

    UIbutton browse_hdfi_file <NEx=44.,NEy=275.> {
      parent => <-.UImod_panel;
      x => ((hdfi_file.x + hdfi_file.width) + 5);
      y => hdfi_file.y;
      width = 75;
      label => "Browse...";
    };

    UIfileDialog hdfi_file_browser <NEx=44.,NEy=319.> {
      visible => <-.browse_hdfi_file.do;
      parent => <-.UImod_panel;
      filename <NEportLevels={2,2}> => <-.ReadHDFImageParams.filename;
    };
  };

  // ReadHDFImage
  //   Functional Macro

  macro ReadHDFImage {

    XP_COMM_PROJ.ReadHDFImage.ReadHDFImageMods.ReadHDFImageParams
        &ReadHDFImageParams <NEx=44.,NEy=11.,NEportLevels={2,1}>;

    XP_COMM_PROJ.ReadHDFImage.ReadHDFImageMods.ReadHDFImageField
        ReadHDFImageField <NEx=253.,NEy=44.> {
      ReadHDFImageParams => <-.ReadHDFImageParams;
    };

    GDM.DataObject DataObject <NEx=330.,NEy=99.> {
      in => <-.ReadHDFImageField.out;
    };

    link field <NEportLevels={1,2},NEx=99.,NEy=165.> => .ReadHDFImageField.out;
    link obj <NEportLevels={1,2},NEx=297.,NEy=165.> => .DataObject.obj;

  };

  // read_hdf_image
  //   User Macro

  macro read_hdf_image {
    XP_COMM_PROJ.ReadHDFImage.ReadHDFImageMods.ReadHDFImageParams
        ReadHDFImageParams <NEx=22.,NEy=22.>;

    XP_COMM_PROJ.ReadHDFImage.ReadHDFImageMacs.ReadHDFImageUI
        ReadHDFImageUI <NEx=264.,NEy=77.> {
      ReadHDFImageParams => <-.ReadHDFImageParams;
      UImod_panel {
        title => name_of(<-.<-.<-);
      };
    };

    XP_COMM_PROJ.ReadHDFImage.ReadHDFImageMacs.ReadHDFImage
        ReadHDFImage <NEx=220.,NEy=132.> {
      ReadHDFImageParams => <-.ReadHDFImageParams;
    };

    link field <NEportLevels={1,2},NEx=66.,NEy=198.> => .ReadHDFImage.field;

    link obj <NEportLevels={1,2},NEx=297.,NEy=198.> => .ReadHDFImage.obj;

  };

  APPS.SingleWindowApp ReadHDFImageEg {
    UI {
      shell {
        x = 53;
        y = 151;
      };

      Modules {
        IUI {
          optionList {
            cmdList => {
              <-.<-.<-.<-.read_hdf_image.ReadHDFImageUI.UImod_panel.option
            };
            selectedItem = 0;
          };
        };
      };
    };

    GDM.Uviewer2D Uviewer2D<NEx=600,NEy=450> {
      Scene {
        Top {
          child_objs => {
            <-.<-.<-.read_hdf_image.obj
          };
        };

        View {
          View {
            trigger = 1;
          };
        };
      };
    };

    XP_COMM_PROJ.ReadHDFImage.ReadHDFImageMacs.read_hdf_image
        read_hdf_image <NEx=154.,NEy=110.> {

      ReadHDFImageParams {
        filename = "./xp_comm_proj/r_hdfimg/image.hdf";
      };

      ReadHDFImageUI {
        UImod_panel {
          option {
            set = 1;
          };
        };

        hdfi_file_browser {
          x = 56;
          y = 626;
          width = 300;
          height = 390;
          ok = 1;
          dirMaskCache = "./xp_comm_proj/r_hdfimg/*";
        };
      };
    };
  };
};

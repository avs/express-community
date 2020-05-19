
// This file contains the following objects:
//
//   ReadMovieBYUUI      - UI for loading BYU files
//   ReadMovieOffsetUI   - UI for loading a single offset file
//   ReadMovieOffsetsUI  - UI for loading offset files and animating them
//   ReadMovieBYU        - Functional macro that reads in BYU files
//   ReadMovieOffset     - Functional macro that reads in a single offset file
//   ReadMovieOffsets    - Functional macro that reads offset files and iterates through them
//   read_movie_byu      - User macro encapsulating ReadMovieBYU and ReadMovieBYUUI
//   read_movie_offset   - User macro encapsulating ReadMovieOffset and ReadMovieOffsetUI
//   read_movie_offsets  - User macro encapsulating ReadMovieOffsets and ReadMovieOffsetUIs
//   read_movie_animate  - User macro encapsulating read_movie_animate and ReadMovieBYUApplyOffset module
//   ReadMovieBYUEg      - Example application demonstrating reading of BYU file and animation.

flibrary ReadMovieBYUMacs {

  // ReadMovieBYUUI
  //   UI Macro for read_movie_byu

  macro ReadMovieBYUUI {
    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMods.ReadMovieBYUParams
      &ReadMovieBYUParams <NEportLevels={2,1},NEx=187.,NEy=120.>;

    UI.UImod_panel panel <NEx=330.,NEy=20.> {
      title = "ReadMovieBYU";
      message = "Select Read BYU control panel.";
      parent<NEportLevels={2,0}>;
    };

    UIlabel UCD_Filename {
      parent => <-.panel;
      y = 0;
      width => parent.width;
      alignment = 0;
      label = "Read MOVIE-BYU File Name";
      color {
        backgroundColor = "blue";
        foregroundColor = "white";
      };
    };

    UItext file_name {
      parent => panel;
      y => UCD_Filename.y + UCD_Filename.height + 5;
      text => <-.ReadMovieBYUParams.filename;
      width = 170;
      showLastPosition = 1;
    };

    UIbutton visible {
      parent => panel;
      x => file_name.x + file_name.width + 5;
      y => file_name.y;
      width = 75;
      height => <-.file_name.height;
      label = "Browse...";
    };
	
    UIfileSB file_browser {
      GMOD.copy_on_change copy_on_change {
        trigger => <-.<-.visible.do; 
        input => <-.<-.visible.do;
        output => <-.visible;
      };
      title = "Read Movie BYU Filename";
      searchPattern = "$XP_PATH<1>/data/*.byu";
      filename => <-.ReadMovieBYUParams.filename;
    };
  };


  // ReadMovieOffsetUI
  //   UI Macro for read_movie_offset

  macro ReadMovieOffsetUI {
    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMods.ReadMovieOffsetParams
      &ReadMovieOffsetParams <NEportLevels={2,1},NEx=187.,NEy=120.>;

    UI.UImod_panel panel <NEx=330.,NEy=20.> {
      title = "ReadMovieOffset";
      message = "Select Read Offset control panel.";
      parent<NEportLevels={2,0}>;
    };

    UIlabel UCD_Filename {
      parent => <-.panel;
      y = 0;
      width => parent.width;
      alignment = 0;
      label = "Read MOVIE-BYU Offset File Name";
      color {
        backgroundColor = "blue";
        foregroundColor = "white";
      };
    };
	
    UItext file_name {
      parent => panel;
      y => UCD_Filename.y + UCD_Filename.height + 5;
      text => <-.ReadMovieOffsetParams.filename;
      width = 170;
      showLastPosition = 1;
    };

    UIbutton visible {
      parent => panel;
      x => file_name.x + file_name.width + 5;
      y => file_name.y;
      width = 75;
      height => <-.file_name.height;
      label = "Browse...";
    };
	
    UIfileSB file_browser {
      GMOD.copy_on_change copy_on_change {
        trigger => <-.<-.visible.do; 
        input => <-.<-.visible.do;
        output => <-.visible;
      };
      title = "Read Offset Filename";
      searchPattern = "$XP_PATH<1>/data/*.";
      filename => <-.ReadMovieOffsetParams.filename;
    };
  };


  // ReadMovieBYU
  //   Functional macro (Field I/O)

  macro ReadMovieBYU {
    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMods.ReadMovieBYUParams
        &ReadMovieBYUParams <NEx=20.,NEy=20.,NEportLevels={2,1}>;

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMods.ReadMovieBYUFile
        ReadMovieBYUFile <NEx=100.,NEy=250.> {
      ReadMovieBYUParams => <-.ReadMovieBYUParams;
    };

    GDM.DataObject DataObject {
      in => <-.ReadMovieBYUFile.out;
      Obj {
      	name => name_of(<-.<-.<-);
      };
    };

    link out <NEportLevels={1,2},NEx=100.,NEy=420.> => ReadMovieBYUFile.out;
    link obj <NEportLevels={1,2},NEx=100.,NEy=450.> => DataObject.obj;
  };


  // ReadMovieOffset
  //   Functional macro (Field I/O)

  macro ReadMovieOffset {
    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMods.ReadMovieOffsetParams
        &ReadMovieOffsetParams <NEx=20.,NEy=20.,NEportLevels={2,1}>;

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMods.ReadMovieOffsetFile
        ReadMovieOffsetFile <NEx=100.,NEy=250.> {
      filename => <-.ReadMovieOffsetParams.filename;
    };

    link out <NEportLevels={1,2},NEx=100.,NEy=420.> => ReadMovieOffsetFile.out;
  };



  // ReadMovieOffsets
  //   Functional macro
  // (read a whole series of offset files one at a time)

  macro ReadMovieOffsets {
    XP_COMM_PROJ.Common.IOWidgets.IndexedFileParams &IndexedFileParams<NEx=22.,NEy=66.,export=1,NEportLevels={2,1}>;

    XP_COMM_PROJ.Common.UIWidgets.VideoLoopParams &VideoLoopParams<NEx=121.,NEy=22.,export=1,NEportLevels={2,1}>;

    XP_COMM_PROJ.Common.IOWidgets.AnimFilename AnimFilename<NEx=132.,NEy=132.> {
      VideoLoopParams => <-.VideoLoopParams;
      IndexedFileParams => <-.IndexedFileParams;
    };

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMods.ReadMovieOffsetFile ReadMovieOffsetFile<NEx=132.,NEy=187.> {
      filename => <-.AnimFilename.filename;
    };

    link out <NEportLevels={1,2},NEx=132.,NEy=242.> => ReadMovieOffsetFile.out;
  };



  // read_movie_byu
  //   User Macro (ReadMovieBYUParams, ReadMovieBYU and ReadMovieBYUUI)

  macro read_movie_byu {
    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMods.ReadMovieBYUParams
        ReadMovieBYUParams <NEx=300.,NEy=50.>;

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMacs.ReadMovieBYU
        ReadMovieBYU <NEx=100.,NEy=250.> {
      &ReadMovieBYUParams => <-.ReadMovieBYUParams;
    };

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMacs.ReadMovieBYUUI
        ReadMovieBYUUI <NEx=300.,NEy=250.> {
      &ReadMovieBYUParams => <-.ReadMovieBYUParams;
      panel {
        title => name_of(<-.<-.<-);
        parent <NEportLevels={4,1}>;
      };
    };

    link out <NEportLevels={1,2},NEx=100.,NEy=400.> => ReadMovieBYU.out;
    link obj <NEportLevels={1,2},NEx=100.,NEy=400.> => ReadMovieBYU.obj;
  };


  // read_movie_offset
  //   User Macro (ReadMovieOffsetParams, ReadMovieOffset and ReadMovieOffsetUI)

  macro read_movie_offset {
    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMods.ReadMovieOffsetParams
        ReadMovieOffsetParams <NEx=300.,NEy=50.>;

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMacs.ReadMovieOffset
        ReadMovieOffset <NEx=100.,NEy=250.> {
      ReadMovieOffsetParams => <-.ReadMovieOffsetParams;
    };

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMacs.ReadMovieOffsetUI
        ReadMovieOffsetUI <NEx=300.,NEy=250.> {
      &ReadMovieOffsetParams => <-.ReadMovieOffsetParams;
      panel {
        title => name_of(<-.<-.<-);
        parent <NEportLevels={4,1}>;
      };
    };

    link out <NEportLevels={1,2},NEx=100.,NEy=400.> => ReadMovieOffset.out;
  };


  // read_movie_offsets
  //   User Macro (VideoLoopParams, IndexedFileParams, AnimFilenameUI,
  //               CreateIndexedFilenameUI, AnimFilename and ReadMovieOffsetFile)

  macro read_movie_offsets {

    XP_COMM_PROJ.Common.UIWidgets.VideoLoopParams VideoLoopParams<NEx=275.,NEy=44.> {
      start_val = 1;
      incr = 1;
    };

    XP_COMM_PROJ.Common.IOWidgets.IndexedFileParams IndexedFileParams<NEx=44.,NEy=44.> {
      dir = "xp_comm_proj/rdmovbyu/";
      filename_stub = "cube";
      extension = "";
      index_width = 3;
    };

    XP_COMM_PROJ.Common.IOWidgets.CreateIndexedFilenameUI CreateIndexedFilenameUI<NEx=22.,NEy=121.> {
      IndexedFileParams => <-.IndexedFileParams;
    };

    XP_COMM_PROJ.Common.IOWidgets.AnimFilenameUI AnimFilenameUI<NEx=374.,NEy=121.> {
      VideoLoopParams => <-.VideoLoopParams;
    };

    XP_COMM_PROJ.Common.IOWidgets.AnimFilename AnimFilename<NEx=209.,NEy=121.> {
      VideoLoopParams => <-.VideoLoopParams;
      IndexedFileParams => <-.IndexedFileParams;
    };

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMods.ReadMovieOffsetFile ReadMovieOffsetFile<NEx=209.,NEy=176.> {
      filename => <-.AnimFilename.filename;
    };

    link out <NEportLevels={1,2},NEx=209.,NEy=231.> => ReadMovieOffsetFile.out;
  };



  // read_movie_apply_offset
  //   User Macro (read_movie_offset and ReadMovieBYUApplyOffset)
  //   Takes a set of field data and applies the offsets in the file specified by the user
  //   Outputs the offset field data and a renderable DataObject

  macro read_movie_apply_offset {

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMacs.read_movie_offset read_movie_offset<NEx=22.,NEy=99.> {
      ReadMovieOffsetUI {
         panel {
            parent<NEportLevels={5,0}>;
         };
      };
    };

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMods.ReadMovieBYUApplyOffset ReadMovieBYUApplyOffset<NEx=165.,NEy=209.> {
      in => <-.in;
      offset_data => <-.read_movie_offset.out;
      scale => <-.scale;
    };

   link in<NEportLevels={2,1},NEx=143.,NEy=55.>;
   float scale<NEportLevels={2,1},NEx=319.,NEy=33.> = 1;
   link out<NEportLevels={1,2},NEx=77.,NEy=275.> => .ReadMovieBYUApplyOffset.out;
   link obj<NEportLevels={1,2},NEx=286.,NEy=275.> => .ReadMovieBYUApplyOffset.out_obj;
  };



  // read_movie_animate
  //   User Macro (read_movie_offsets and ReadMovieBYUApplyOffset)
  //   Takes a set of field data and then repeatedly applies the offsets found in the offset files
  //   Outputs the offset field data and a renderable DataObject

  macro read_movie_animate {

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMacs.read_movie_offsets read_movie_offsets<NEx=22.,NEy=99.> {
      CreateIndexedFilenameUI {
         panel {
            parent<NEportLevels={5,0}>;
         };
      };
      AnimFilenameUI {
         panel {
            parent<NEportLevels={5,0}>;
         };
      };
    };

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMods.ReadMovieBYUApplyOffset ReadMovieBYUApplyOffset<NEx=165.,NEy=165.> {
      in => <-.in;
      offset_data => <-.read_movie_offsets.out;
      scale => <-.scale;
    };

   link in<NEportLevels={2,1},NEx=121.,NEy=33.>;
   float scale<NEportLevels={2,1},NEx=286.,NEy=33.> = 1;
   link out<NEportLevels={1,2},NEx=77.,NEy=220.> => .ReadMovieBYUApplyOffset.out;
   link obj<NEportLevels={1,2},NEx=319.,NEy=220.> => .ReadMovieBYUApplyOffset.out_obj;
  };



  // ReadMovieBYUEg
  //   Example Application (SingleWindowApp)
  //   Illustrates loading a BYU file and then animating it using the read_movie_animate macro

  APPS.SingleWindowApp ReadMovieBYUEg <NEdisplayMode="maximized"> {

    GDM.Uviewer3D Uviewer3D<NEx=432,NEy=240> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.read_movie_animate.obj};

            Xform {
               ocenter = {0.5,0.5,0.5};
               dcenter = {2.,2.,2.};
               mat = {
                  4.,0.,0.,0.,0.,4.,0.,0.,0.,0.,4.,0.,0.,0.,0.,1.
               };
               xlate = {-2.,-2.,-2.};
               center = {0.5,0.5,0.5};
            };

         };
         Camera {
            Camera {
               auto_norm = "None";
            };
         };
      };
    };

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMacs.read_movie_byu read_movie_byu<NEx=429.,NEy=33.>;

    XP_COMM_PROJ.ReadMovieBYU.ReadMovieBYUMacs.read_movie_animate read_movie_animate<NEx=429.,NEy=121.> {
      in => <-.read_movie_byu.out;

      read_movie_offsets {
         VideoLoopParams {
            end_val = 8;
         };
      };
    };

  };

};

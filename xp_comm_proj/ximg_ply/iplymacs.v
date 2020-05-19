
flibrary AVSImagePlayerMacs <compile_subs=0> {

   macro ImagePlayerUI {

      UImod_panel Image_Player<NEx=440.,NEy=44.> {
         parent<NEportLevels={4,0}>;
      };

      XP_COMM_PROJ.AVSImagePlayer.AVSImagePlayerMods.ImagePlayerParams &image_player_param<NEportLevels={2,0}>;

      macro Images<NEx=33.,NEy=418.> {
         link file_format<NEportLevels={2,1},NEx=66.,NEy=55.> => <-.image_player_param.file_format;
         link nb_images<NEportLevels={2,1},NEx=242.,NEy=55.> => <-.image_player_param.nb_images;
         link first_image<NEportLevels={2,1},NEx=429.,NEy=55.> => <-.image_player_param.first_image;
         link step<NEportLevels={2,1},NEx=616.,NEy=55.> => <-.image_player_param.step;
         UIframe UIframe<NEx=121.,NEy=154.> {
            parent<NEportLevels={3,0}> => <-.<-.Image_Player;
            y = 0;
            width => parent.clientWidth;
            height = 155;
         };
         UIlabel Files_to_read<NEx=341.,NEy=187.> {
            parent => <-.UIframe;
            y = 0;
            width => parent.clientWidth;
         };
         UIlabel File_Format<NEx=209.,NEy=253.> {
            parent => <-.UIframe;
            y = 24;
            alignment = "right";
            width => 115;
         };
         UItext UItext<NEx=440.,NEy=253.> {
            parent => <-.UIframe;
            text<NEportLevels={2,2}> => <-.file_format;
            y = 24;
            x => (parent.clientWidth / 2);
         };
         UIlabel Number_of_Images<NEx=209.,NEy=319.> {
            parent => <-.UIframe;
            y = 54;
            alignment = "right";
            width => 115;
         };
         UIfield UIfield<NEx=440.,NEy=319.> {
            parent => <-.UIframe;
            value => <-.nb_images;
            y = 54;
            x => (parent.clientWidth / 2);
            min = 1.;
            mode = "integer";
         };
         UIlabel First_Image<NEx=209.,NEy=385.> {
            parent => <-.UIframe;
            y = 84;
            alignment = "right";
            width => 115;
         };
         UIlabel Step<NEx=209.,NEy=451.> {
            parent => <-.UIframe;
            y = 114;
            alignment = "right";
            width => 115;
         };
         UIfield UIfield#1<NEx=440.,NEy=385.> {
            parent => <-.UIframe;
            value<NEportLevels={2,2}> => <-.first_image;
            y = 84;
            x => (parent.clientWidth / 2);
            min = 0.;
            mode = "integer";
         };
         UIfield UIfield#2<NEx=440.,NEy=451.> {
            parent => <-.UIframe;
            value<NEportLevels={2,2}> => <-.step;
            y = 114;
            x => (parent.clientWidth / 2);
            min = 1.;
            mode = "integer";
         };
      };

      macro Param<NEx=209.,NEy=418.> {
         link access_mode<NEportLevels={2,1},NEx=33.,NEy=22.> => <-.image_player_param.access_mode;
         link run_mode<NEportLevels={2,1},NEx=209.,NEy=22.> => <-.image_player_param.run_mode;
         link fps<NEportLevels={2,1},NEx=385.,NEy=22.> => <-.image_player_param.fps;
         link nsec_before<NEportLevels={2,1},NEx=550.,NEy=22.> => <-.image_player_param.nsec_before;
         link nsec_after<NEportLevels={2,1},NEx=649.,NEy=66.> => <-.image_player_param.nsec_after;
         UIoption Disk<NEx=297.,NEy=187.>;
         UIoptionMenu Access_Mode<NEx=132.,NEy=187.> {
            parent => <-.UIframe;
            cmdList => {<-.Disk,<-.Memory};
            selectedItem<NEportLevels={2,2}> => <-.access_mode;
            x = 39;
            y = 24;
         };
         UIoption Memory<NEx=473.,NEy=187.> {
            set = 1;
         };
         UIframe UIframe<NEx=110.,NEy=77.> {
            parent<NEportLevels={3,0}> => <-.<-.Image_Player;
            y = 155;
            width => parent.clientWidth;
            height = 192;
            clientWidth = 244;
         };
         UIlabel Animation_parameters<NEx=286.,NEy=132.> {
            parent => <-.UIframe;
            y = 0;
            width => parent.clientWidth;
         };
         UIoption Forward<NEx=297.,NEy=253.>;
         UIoptionMenu Run_Mode<NEx=132.,NEy=253.> {
            parent => <-.UIframe;
            cmdList => {<-.Forward,
               <-.Cont_Forward,<-.Backwards,<-.Cont_Backwards};
            selectedItem<NEportLevels={2,2}> => <-.run_mode;
            x = 25;
            y = 54;
         };
         UIoption Cont_Forward<NEx=473.,NEy=253.> {
            set = 1;
         };
         UIoption Backwards<NEx=297.,NEy=297.>;
         UIoption Cont_Backwards<NEx=473.,NEy=297.>;
         UIlabel Nsec_Before<NEx=121.,NEy=418.> {
            parent => <-.UIframe;
            y = 121;
            alignment = "right";
            width => 115;
         };
         UIfield UIfield#1<NEx=297.,NEy=418.> {
            parent => <-.UIframe;
            value => <-.nsec_before;
            y = 121;
            x => (parent.clientWidth / 2);
            min = 0.;
            mode = "integer";
         };
         UIlabel Nsec_After<NEx=121.,NEy=473.> {
            parent => <-.UIframe;
            y = 151;
            alignment = "right";
            width => 115;
         };
         UIfield UIfield#2<NEx=297.,NEy=473.> {
            parent => <-.UIframe;
            value<NEportLevels={2,2}> => <-.nsec_after;
            y = 151;
            x => (parent.clientWidth / 2);
            min = 0.;
            mode = "integer";
         };
         UIlabel Frames_per_sec<NEx=121.,NEy=352.> {
            parent => <-.UIframe;
            y = 91;
            alignment = "right";
            width => 115;
         };
         UIfield UIfield<NEx=297.,NEy=352.> {
            parent => <-.UIframe;
            value<NEportLevels={2,2}> => <-.fps;
            y = 91;
            x => (parent.clientWidth / 2);
            min = 0.1;
            max = 100.;
            mode = "real";
            decimalPoints = 1;
         };
      };

      macro Control<NEx=385.,NEy=418.> {
         link image_no<NEportLevels={2,1},NEx=231.,NEy=44.> => <-.image_player_param.count;
         link real_fps<NEportLevels={2,1},NEx=407.,NEy=44.> => <-.image_player_param.real_fps;
         link nb_images<NEportLevels={2,1},NEx=550.,NEy=44.> => <-.image_player_param.nb_images;
         UIframe UIframe<NEx=55.,NEy=44.> {
            parent<NEportLevels={3,0}> => <-.<-.Image_Player;
            y = 347;
            width => parent.clientWidth;
            height = 145;
         };
         UIlabel Animation_control<NEx=187.,NEy=121.> {
            parent => <-.UIframe;
            y = 0;
            width => parent.clientWidth;
         };
         UIslider Image_Number<NEx=187.,NEy=187.> {
            parent => <-.UIframe;
            value => <-.image_no;
            y = 24;
            min = 0.;
            max => (<-.nb_images - 1);
            width => parent.clientWidth;
            increment = 5.;
            mode = "integer";
         };
         UIbutton Run<NEx=110.,NEy=253.> {
            parent => <-.UIframe;
            x => (((parent.clientWidth - .width) - .width) / 3);
            y = 84;
         };
         UIbutton Stop<NEx=319.,NEy=253.> {
            parent => <-.UIframe;
            x => (((((parent.clientWidth - .width) - <-.Run.width) / 3) + <-.Run.x) + <-.Run.width);
            y = 84;
         };
         UIlabel UIlabel<NEx=198.,NEy=352.> {
            parent => <-.UIframe;
            label => <-.real_fps;
            y = 115;
            width => parent.clientWidth;
         };
         link run_do<NEportLevels={1,3},NEx=44.,NEy=429.> => .Run.do;
         link stop_do<NEportLevels={1,3},NEx=407.,NEy=418.> => .Stop.do;
      };

      GMOD.parse_v parse_v<NEx=44.,NEy=99.> {
         v_commands = "count=0;";
         trigger => <-.image_player_param.access_mode;
         relative => <-.image_player_param;
      };
   };


   macro imagePlayer {
      ImagePlayerUI ImagePlayerUI {
         image_player_param => <-.ImagePlayerParams;
         Image_Player {
	         title => name_of(<-.<-.<-);
         };
      };

      XP_COMM_PROJ.AVSImagePlayer.AVSImagePlayerMods.AVSImagePlayer ImagePlayer<NEx=407.,NEy=264.> {
         ImagePlayerParams => <-.ImagePlayerParams;
         trigger_run => <-.ImagePlayerUI.Control.run_do;
         trigger_stop => <-.ImagePlayerUI.Control.stop_do;
      };

      XP_COMM_PROJ.AVSImagePlayer.AVSImagePlayerMods.ImagePlayerParams ImagePlayerParams<NEx=55.,NEy=44.,NEportLevels={0,1}> {
         file_format = "data/Imag%04d.x";
         nb_images = 10;
         first_image = 0;
         step = 1;
         access_mode = 1;
         run_mode = 0;
         fps = 5.;
         nsec_before = 1;
         nsec_after = 1;
         real_fps = "";
         count = 0;
      };

      link out<NEportLevels={1,2}> => ImagePlayer.out;
      link obj<NEportLevels={1,2}> => ImagePlayer.obj;
   };


   APPS.SingleWindowApp AVSImagePlayerEg<NEdisplayMode="maximized"> {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.imagePlayer.obj};
            };
         };
      };

      XP_COMM_PROJ.AVSImagePlayer.AVSImagePlayerMacs.imagePlayer imagePlayer {
         ImagePlayerParams {
            file_format = "xp_comm_proj/ximg_ply/data/Globe%d.x";
         };
      };
   };

};


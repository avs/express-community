flibrary CineMods<build_dir="xp_comm_proj/cine",
				out_hdr_file="gen.h"> {
  //
  // module: Animator
  // purpose: Capture and playback frames from an Express viewer.
  // synopsis: 
  //     mode 0: supports interactive capture of images as user pans/zooms or
  //       otherwise updates display. This mode will capture frames, as long 
  //       as capture is true, whenever the view is updated. It will play these 
  //       frames back whenever playback is true, using frame_in to determine 
  //       which frame is visible.
  //
  //     mode 1: supports caching of frames under control of CineControl or 
  //       some other loop mechanism. While capture is true, this mode will 
  //       look for changes in frame_in, and determine whether that frame is
  //       currently cached. If so it will update the view, if not it will
  //       update frame_out to reflect the value of frame_in.
  //
  module Image_Cache<src_file="imgcache.c"> {
    omethod+notify_inst create() = "CacheCreate";
    omethod+req set_mode(.mode+notify+read+write+req
                         ) = "CacheSetMode";
    omethod+req set_capture(.capture+notify+read+req,
                            .playback+nonotify+write
                            ) = "CacheCapture";
    omethod+req set_playback(.playback+notify+read+req,
                             .capture+nonotify+write
                             ) = "CachePlayback";
    omethod+req set_frame(.frame_in+notify+read+req,
                          .frame_out+nonotify+write
                          ) = "CacheSetFrame";
    omethod+req proc_trigger(.view+notify+read,
                             .frame_out+nonotify+write,
                             .num_frames+nonotify+write
                             ) = "CacheProcessTrigger";
    omethod+notify_deinst delete() = "CacheDestroy";
    GDview_templ &view<NEportLevels={2,0}>;
    enum mode<NEportLevels={2,0}> {
      choices = {"interactive","loop"};
    } = "interactive";
    Boolean capture<NEportLevels={2,0}>;
    Boolean playback<NEportLevels={2,0}>;
    int frame_in<NEportLevels={2,0}>;
    int frame_out<NEportLevels={0,2}>;
    int num_frames<NEportLevels={0,2}>;
    ptr hidden<NEvisible=0> = 0;
  };

  // 
  // module: Cine_Control
  // purpose: 
  //
  //
  module Cine_Control<src_file="cinecntl.c"> {
    omethod+notify_inst create() = "CreateControl";
    omethod+req update(.mode+read+notify+req,
                       .rate+read+notify+req,
                       .start+read+notify+req,
                       .end+read+notify+req,
                       .run+read+notify+req,
                       .shell_visible+read+notify,
                       .value+write,
                       .true_rate+write
                       ) = "UpdateControl";
    omethod+notify_deinst destroy() = "DeleteControl";
    int mode<NEportLevels={2,0}>;
    float rate<NEportLevels={2,0}>;
    int start<NEportLevels={2,0}>;
    int end<NEportLevels={2,0}>;
    Boolean run<NEportLevels={2,0}>;
    int value<NEportLevels={0,2}>;
    Boolean shell_visible;
    float true_rate;
    ptr hidden<NEvisible=0> = 0;
  };
  
     
   group CineParams<NEportLevels={0,1}> {
   	   enum+Port2 emode {
   	   	choices = {"interactive","loop"};
   	   };
           int+Port2 mode;
           float+Port2 rate;
           int+Port2 start;
           int+Port2 end;
           int+Port2 capture;
           int+Port2 playback;
           float+Port2 true_rate;
   };
   
};

macro VLIvolume_renderUI {

  VLIVolumeInfo &VLIvol_info<NEportLevels={2,1}>;

  UImod_panel UImod_panel {
    title => "VLIvolume_render";
  };
  
  VLIvolume_scrollUI VLIvolume_scrollUI {
    parent => <-.UImod_panel;
    VLIvol_info => <-.VLIvol_info;
  };

};

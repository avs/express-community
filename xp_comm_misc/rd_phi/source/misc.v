
library rp_misc <compile_subs=0> {

	macro ReadPhoenicsUI<NEx=132.,NEy=33.> {
	   UImod_panel mod_panel<NEx=33.,NEy=22.> {
		  width = 290;
		  height = 780;
		  title => name_of(<-.<-.<-,1);
	   };
	   UItext phi_text<NEx=143.,NEy=110.> {
		  parent => <-.UIframe;
		  text<NEportLevels={2,3},export=2> => <-.phi_dialog.filename;
		  x = 10;
		  y => (<-.phi_label.y + <-.phi_label.height);
		  width = 160;
	   };
	   UIbutton phi_browse<NEx=308.,NEy=110.> {
		  parent => <-.UIframe;
		  height = 30;
		  label => "Browse";
		  y => <-.phi_text.y;
		  x => ((<-.phi_text.x + <-.phi_text.width) + 5);
		  width = 80;
	   };
	   UIfileDialog phi_dialog<NEx=473.,NEy=110.> {
		  visible => <-.phi_browse.do;
		  parent => <-.mod_panel;
		  cancel = 1;
		  searchPattern = "*";
	   };
	   UItext xyz_text<NEx=143.,NEy=165.> {
		  parent => <-.UIframe;
		  x = 10;
		  y => (<-.xyz_label.y + <-.xyz_label.height);
		  width = 160;
		  text<NEportLevels={2,3},export=2> => <-.xyz_dialog.filename;
	   };
	   UIbutton xyz_browse<NEx=308.,NEy=165.> {
		  parent => <-.UIframe;
		  height = 30;
		  label => "Browse";
		  y => <-.xyz_text.y;
		  x => ((<-.xyz_text.x + <-.xyz_text.width) + 5);
		  width = 80;
	   };
	   UIfileDialog xyz_dialog<NEx=484.,NEy=165.> {
		  visible => <-.xyz_browse.do;
		  parent => <-.mod_panel;
		  cancel = 1;
		  searchPattern = "*";
	   };
	   UItext pat_text<NEx=143.,NEy=220.> {
		  parent => <-.UIframe;
		  x = 10;
		  y => (<-.pat_label.y + <-.pat_label.height);
		  width = 160;
		  text<NEportLevels={2,3},export=2> => <-.pat_dialog.filename;
	   };
	   UIbutton pat_browse<NEx=308.,NEy=220.> {
		  parent => <-.UIframe;
		  height = 30;
		  label => "Browse";
		  y => <-.pat_text.y;
		  x => ((<-.pat_text.x + <-.pat_text.width) + 5);
		  width = 80;
		  visible;
	   };
	   UIfileDialog pat_dialog<NEx=484.,NEy=220.> {
		  visible => <-.pat_browse.do;
		  parent => <-.mod_panel;
		  cancel = 1;
		  searchPattern = "*";
	   };
	   UItoggle phi_toggle<NEx=616.,NEy=110.> {
		  parent => <-.UIframe;
		  label => "Binary";
		  set<NEportLevels={0,3}> = 0;
		  y => (<-.phi_text.y + <-.phi_text.height);
		  x = 10;
	   };
	   UItoggle xyz_toggle<NEx=616.,NEy=165.> {
		  parent => <-.UIframe;
		  label => "Binary";
		  y => (<-.xyz_text.y + <-.xyz_text.height);
		  x = 10;
		  set<NEportLevels={0,3}> = 0;
	   };
	   UIlabel phi_label<NEx=0.,NEy=110.> {
		  parent => <-.UIframe;
		  label => "PHI Filename";
		  x = 10;
		  y = 10;
		  alignment = "left";
	   };
	   UIlabel xyz_label<NEx=0.,NEy=165.> {
		  parent => <-.UIframe;
		  label => "XYZ Filename";
		  x = 10;
		  y => (<-.phi_label.y + 80);
		  alignment = "left";
	   };
	   UIlabel pat_label<NEx=0.,NEy=220.> {
		  parent => <-.UIframe;
		  label => "PATGEO Filename";
		  x = 10;
		  y => (<-.xyz_label.y + 80);
		  alignment = "left";
		  width = 200;
	   };
	   UIbutton Read_Files<NEx=308.,NEy=275.> {
		  parent => <-.UIframe;
		  do<NEportLevels={0,3}>;
		  y => (<-.pat_label.y + 100);
		  x => ((.parent.width - .width) / 2);
	   };
	   UIframe UIframe<NEx=209.,NEy=44.> {
		  parent => <-.mod_panel;
		  y = 100;
		  width => (.parent.clientWidth - 20);
		  height => ((<-.Read_Files.y + <-.Read_Files.height) + 100);
	   };
	   UIlabel cham_image<NEx=473.,NEy=44.> {
		  parent => <-.mod_panel;
		  height => 100;
		  y = 0;
		  width => .parent.width;
		  labelPixmap {
			 filename = "data/cham.x";
			 fileType = "avs_image";
		  };
	   };
	   UIlabel message_label {
		  parent => <-.UIframe;
		  width => .parent.width;
		  y => (.parent.height - 50);
		  label = "";
	   };
	};

	macro Read_Phoenics<NEx=143.,NEy=55.> {

	   WORKSPACE_1.ReadPhoenicsUI ReadPhoenicsUI<NEx=55.,NEy=22.> {
          message_label.label => <-.<-.read_phoenics.message;
	   };
	   WORKSPACE_2.read_phoenics read_phoenics<NEx=418.,NEy=99.> {
		  trigger => <-.ReadPhoenicsUI.Read_Files.do;
		  phi_name => <-.ReadPhoenicsUI.phi_text.text;
		  xyz_name => <-.ReadPhoenicsUI.xyz_text.text;
		  pat_name => <-.ReadPhoenicsUI.pat_text.text;
		  phi_bin => <-.ReadPhoenicsUI.phi_toggle.set;
		  xyz_bin => <-.ReadPhoenicsUI.xyz_toggle.set;
		  title<NEportLevels={0,3}>;
	   };
	   link out_fld<NEportLevels={1,2},NEx=165.,NEy=165.> => read_phoenics.out_fld;
	};


};

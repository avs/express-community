flibrary OverlayMacs  {
  
   macro OverlayUI {
     XP_COMM_PROJ.Overlay.OverlayMods.OverlayParams &OverlayParams<NEx=319.,NEy=55.,NEportLevels={2,0}>;

     UImod_panel Overlay<NEx=22.,NEy=110.> {
       option {
      	 set = 1;
       };
     };
     UIlabel Mask<NEx=11.,NEy=176.> {
       parent => <-.Overlay;
       x=50;
     };
     UIslider MaskA<NEx=154.,NEy=176.> {
       parent => <-.Overlay;
       min = 0.;
       max = 255.;
       value<NEportLevels={2,2}> => <-.OverlayParams.maskA;
       title="A";
       mode = "integer";
       increment = 1.;
     };
     UIslider MaskR<NEx=297.,NEy=176.> {
       parent => <-.Overlay;
       min = 0.;
       max = 255.;
       value<NEportLevels={2,2}> => <-.OverlayParams.maskR;
       title="R";
       mode = "integer";
       increment = 1.;
     };
     UIslider MaskG<NEx=440.,NEy=176.> {
       parent => <-.Overlay;
       min = 0.;
       max = 255.;
       value<NEportLevels={2,2}> => <-.OverlayParams.maskG;
       title="G";
       mode = "integer";
       increment = 1.;
     };
     UIslider MaskB<NEx=583.,NEy=176.> {
       parent => <-.Overlay;
       min = 0.;
       max = 255.;
       value<NEportLevels={2,2}> => <-.OverlayParams.maskB;
       title="B";
       mode = "integer";
       increment = 1.;
     };
     UIlabel Background<NEx=66.,NEy=253.> {
       parent => <-.Overlay;
       x=50;
     };
     UIslider BkgndA<NEx=242.,NEy=253.> {
       parent => <-.Overlay;
       min = 0.;
       max = 255.;
       value<NEportLevels={2,2}> => <-.OverlayParams.bkgndA;
       title="A";
       mode = "integer";
       increment = 1.;
     };
     UIslider BkgndR<NEx=385.,NEy=253.> {
       parent => <-.Overlay;
       min = 0.;
       max = 255.;
       value<NEportLevels={2,2}> => <-.OverlayParams.bkgndR;
       title="R";
       mode = "integer";
       increment = 1.;
     };
     UIslider BkgndG<NEx=528.,NEy=253.> {
       parent => <-.Overlay;
       min = 0.;
       max = 255.;
       value<NEportLevels={2,2}> => <-.OverlayParams.bkgndG;
       title="G";
       mode = "integer";
       increment = 1.;
     };
     UIslider BkgndB<NEx=671.,NEy=253.> {
       parent => <-.Overlay;
       min = 0.;
       max = 255.;
       value<NEportLevels={2,2}> => <-.OverlayParams.bkgndB;
       title="B";
       mode = "integer";
       increment = 1.;
     };
   };


   macro OverlayImages {
     ilink image1<NEx=99.,NEy=110.>;
     ilink image2<NEx=99.,NEy=154.>;

     XP_COMM_PROJ.Overlay.OverlayMods.OverlayParams &OverlayParams<NEx=275.,NEy=77.,NEportLevels={2,1}>;

     XP_COMM_PROJ.Overlay.OverlayMods.OverlayNodeData OverlayNodeData<NEx=286.,NEy=198.> {
       image1 => <-.image1;
       image2 => <-.image2;
       OverlayParams => <-.OverlayParams;
     };

     olink out<NEx=143.,NEy=352.> => .OverlayNodeData.compos;
   };


   macro overlayImages<NEx=231.,NEy=187.> {
     ilink in1<NEx=88.,NEy=66.>;
     ilink in2<NEx=88.,NEy=33.>;

     XP_COMM_PROJ.Overlay.OverlayMods.OverlayParams OverlayParams<NEx=374.,NEy=44.,NEportLevels={0,1}>;
     XP_COMM_PROJ.Overlay.OverlayMacs.OverlayUI OverlayUI<NEx=484.,NEy=154.> {
       OverlayParams => <-.OverlayParams;
     };
     XP_COMM_PROJ.Overlay.OverlayMacs.OverlayImages OverlayImages<NEx=231.,NEy=154.> {
       OverlayParams => <-.OverlayParams;
       image1 => <-.in1;
       image2 => <-.in2;
     };

     GDM.DataObject DataObject<NEx=231.,NEy=220.> {
       in => <-.OverlayImages.out;
     };

     olink out<NEx=44.,NEy=275.> => .OverlayImages.out;
     olink obj<NEx=231.,NEy=275.> => .DataObject.obj;
   };

   
   APPS.SingleWindowApp OverlayEg {
     UI {
       Modules {
      	 IUI {
	         optionList {
	            selectedItem = 2;
	         };
	       };
       };
       shell {
      	 x = 301;
	       y = 89;
       };
     };
     GDM.Uviewer2D Uviewer2D<NEx=165.,NEy=220.> {
       Scene {
      	 View {
	         View {
	            trigger = 1;
	         };
	       };
	       Top {
	         child_objs => {<-.<-.<-.overlayImages.obj};
      	   Xform {
	           ocenter = {199.5,89.5,0.};
	           dcenter = {4.,1.79449,0.};
	         };
	       };
       };
     };
     MODS.Read_Image Background_Image<NEx=77.,NEy=33.> {
       read_image_ui {
      	 file_browser {
	         filename = "$XP_ROOT/data/image/mandrill.x";
	       };
       };
     };
     MODS.Read_Image Foreground_Image<NEx=231.,NEy=33.> {
       read_image_ui {
	      file_browser {
	         filename = "./xp_comm_proj/overlay/mvc.tif";
	      };
       };
     };

     XP_COMM_PROJ.Overlay.OverlayMacs.overlayImages overlayImages<NEx=165.,NEy=132.> {
       in1 => <-.Background_Image.field;
       in2 => <-.Foreground_Image.field;
       OverlayParams{
         maskA=255;
	      maskR=0;
	      maskG=0;
	      maskB=255;
	      bkgndA=0;
	      bkgndR=0;
	      bkgndG=0;
	      bkgndB=0;
       };
     };
   };

};


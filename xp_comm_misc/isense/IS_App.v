APPS.SingleWindowApp SingleWindowApp {
   UI {
      shell {
         x = 72;
         y = 43;
      };
      Modules {
         IUI {
            optionList {
               selectedItem = 0;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D<NEx=432,NEy=240> {
      Scene_Editor {
         View_Editor {
            InfoPanel {
               curobj_sel {
                  width = 238;
                  height = 245;
                  x = 0;
                  y = 0;
               };
            };
         };
      };
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Read_Geom.geom};
         };
      };
   };
   macro IS_GUI<NEx=209.,NEy=22.,NEwidth=275.,NEheight=165.> {
      UImod_panel InterSense_Control<NEx=22.,NEy=22.>;
      UIbutton Check_Status<NEx=22.,NEy=165.> {
         parent => <-.InterSense_Control;
         y = 110;
         x = 140;
      };
      UItoggle Enable_Polling<NEx=22.,NEy=121.> {
         parent => <-.InterSense_Control;
         y = 120;
         x = 10;
      };
      UIfield Xrot<NEx=187.,NEy=77.> {
         parent => <-.InterSense_Control;
         y = 150;
		 min = -180;
		 max = 180;
		 mode = 2;
		 decimalPoints = 3;
		 value => <-.<-.ISWrapper.ISWrapperMod.Xrot;
      };
      UIfield Yrot<NEx=187.,NEy=121.> {
         parent => <-.InterSense_Control;
         y = 180;
		 min = -180;
		 max = 180;
		 mode = 2;
		 decimalPoints = 3;
		 value => <-.<-.ISWrapper.ISWrapperMod.Yrot;
      };
      UIfield Zrot<NEx=187.,NEy=165.> {
         parent => <-.InterSense_Control;
         y = 210;
		 min = -180;
		 max = 180;
		 mode = 2;
		 decimalPoints = 3;
		 value => <-.<-.ISWrapper.ISWrapperMod.Zrot;
      };
      UIfield Xtrans<NEx=341.,NEy=77.> {
         parent => <-.InterSense_Control;
         y = 150;
         x = 140;
		 min = -10;
		 max = 10;
		 mode = 2;
		 decimalPoints = 3;
		 value => <-.<-.ISWrapper.ISWrapperMod.Xtrans;
      };
      UIfield Ytrans<NEx=341.,NEy=121.> {
         parent => <-.InterSense_Control;
         y = 180;
         x = 140;
		 min = -10;
		 max = 10;
		 mode = 2;
		 decimalPoints = 3;
		 value => <-.<-.ISWrapper.ISWrapperMod.Ytrans;
      };
      UIfield Ztrans<NEx=341.,NEy=165.> {
         parent => <-.InterSense_Control;
         y = 210;
         x = 140;
		 min = -10;
		 max = 10;
		 mode = 2;
		 decimalPoints = 3;
		 value => <-.<-.ISWrapper.ISWrapperMod.Ztrans;
      };
      UIbutton Open_Device<NEx=187.,NEy=33.> {
         parent => <-.InterSense_Control;
         y = 10;
      };
      UIbutton Close_Device<NEx=341.,NEy=33.> {
         parent => <-.InterSense_Control;
         height = 26;
         x = 140;
         y = 10;
      };
   };
   ISWrapper ISWrapper<NEx=209.,NEy=110.> {
      ISWrapperMod<NEx=143.,NEy=143.> {
         OpenDeviceTrigger => <-.<-.IS_GUI.Open_Device.do;
         CloseDeviceTrigger => <-.<-.IS_GUI.Close_Device.do;
         ShowStatsTrigger => <-.<-.IS_GUI.Check_Status.do;
         UpdateXformTrigger => <-.timer.Val;
      };
      GMOD.timer timer<NEx=143.,NEy=55.> {
         TimeStep = 0;
         Active => <-.<-.IS_GUI.Enable_Polling.set;
      };
   };
   MODS.Read_Geom Read_Geom<NEx=385.,NEy=22.> {
      ACread_geom {
         out_field<NEdisplayMode="open"> {
            xform<NEportLevels={0,4}>;
         };
      };
      DataObject<NEscalingFactor=1.,NExOffset=0.,NEyOffset=0.> {
         Obj<NEx=374.,NEy=132.,NEdisplayMode="open">;
      };
      field<NEx=726.,NEy=165.>;
   };
   GDM.GDxform_edit GDxform_edit<NEx=385.,NEy=110.> {
      xform => <-.Read_Geom.ACread_geom.out_field.xform;
	  x_rot => <-.ISWrapper.ISWrapperMod.Xrot;
	  y_rot => <-.ISWrapper.ISWrapperMod.Yrot;
	  z_rot => <-.ISWrapper.ISWrapperMod.Zrot;
	  x_trans => <-.ISWrapper.ISWrapperMod.Xtrans;
	  y_trans => <-.ISWrapper.ISWrapperMod.Ytrans;
	  z_trans => <-.ISWrapper.ISWrapperMod.Ztrans;
   };
};

flibrary Bicolor {
   GDM.DataObjectNoTexture DVbicolor_surface {
      float h<NEportLevels={2,0}> = 0.;
      float s<NEportLevels={2,0}> = 0.;
      float v<NEportLevels={2,0}> = 0.4;
      GMOD.hsv_to_rgb HSVToRGB {
         h => <-.h;
         s => <-.s;
         v => <-.v;
      };
      int Reverse<NEportLevels={2,0}> = 0;
      Props {
         cull => switch((Reverse==0)+1,"Back","Front");
      };
      GDM.GroupObject GroupObject {
         child_objs => {<-.Obj#1,<-.Obj};
         Top {
            xform_mode = "Parent";
            pickable = 0;
            name = "BicolorSurface";
         };
         Props {
            jitter = 1;
         };
         obj<NEportLevels={1,2}>;
      };
      Obj.name => name_of(<-,1);
      GDM.DefaultObject Obj#1 {
         input => <-.in;
         dmap => <-.Datamap#1;
         xform => in.xform;
         props => <-.Props#1;
         modes => <-.Modes;
         xform_mode = "Parent";
         name => ("rear_" + <-.Obj.name);
         pickable = 0;
      };
      DefaultProps Props#1 {
         cull => switch((Reverse==0)+1,"Front","Back");
         col  => { <-.HSVToRGB.r, <-.HSVToRGB.g, <-.HSVToRGB.b };
      };
      DefaultLinear Datamap#1<NEportLevels={0,1}> {
         dataMin<NEportLevels={2,2}> = 0.;
         dataMax<NEportLevels={2,2}> = 0.;
         DatamapValue = {{v4=><-.<-.v,v3=><-.<-.s,v2=><-.<-.h,},{v4=><-.<-.v,v3=><-.<-.s,v2=><-.<-.h,}};
      };
      obj<NEy=451.,NEx=385.,NEportLevels={1,2}> => .GroupObject.obj;
      child_objs<NEportLevels={1,1}>;
   };
   
   macro BicolorSurface<NEhelpContextID=1,NEhelpFile="STM3/BicolorSurface.html"> {
      link in<NEportLevels={2,1},NEx=22.,NEy=33.>;
      DVbicolor_surface BicolorSurface<NEx=143.,NEy=77.>{
         in => <-.in;
		 h => <-.UI.ColorEditor.Dials.HDial.value;
		 s => <-.UI.ColorEditor.Dials.SDial.value;
		 v => <-.UI.ColorEditor.Dials.VDial.value;
      };
      macro UI<NEx=286.,NEy=143.> {
         UImod_panel panel<NEx=44.,NEy=33.> {
            title => name_of(<-.<-.<-,1);
            parent<NEportLevels={2,0}>;
         };
         macro ColorEditor<NEx=209,NEy=66> {
            link   parent<NEx=55,NEy=33,NEportLevels={2,1}> => <-.panel;
            int    x = 0;
            int    y = 0;
            int+nres    width => parent.clientWidth;
            string label<NEportLevels=1> = "Back Surface Color";
            UIlabel Title<NEx=506,NEy=132> {
               parent => <-.parent;
               x      =  5;
               y      => <-.y + 4;  
               width  => <-.width - 10;
               label  => <-.label;
               alignment = 1;
               color {
                  foregroundColor = "white";
                  backgroundColor = "blue";
               };
            };
            GMOD.hsv_to_rgb HSVToRGB<NEx=209,NEy=297> {
               h => <-.Dials.HDial.value;
               s => <-.Dials.SDial.value;
               v => <-.Dials.VDial.value;
            };
         	VUIColorPanel ColorEcho {
               parent => <-.parent;
               x      =  4;
               y      => <-.Title.y + <-.Title.height + 2;
               width  => <-.width - 8;
               height => <-.Title.height;
               color  => { <-.HSVToRGB.r, <-.HSVToRGB.g, <-.HSVToRGB.b };
            }; 
         
            group Dials {
               UIframe DialFrame<NEx=341,NEy=231> {
                  parent => <-.<-.parent;
                  x      =  4;
                  y      => <-.<-.ColorEcho.y + <-.<-.ColorEcho.height;
                  width  => <-.<-.width - 8;
                  height = 170;
               };
         
               int tw<NEvisible=1> => <-.width - 8;
               int w3<NEvisible=1> => (tw-16) / 3;
         
               VUIDial HDial<NEx=77,NEy=231> {
                  parent  => <-.DialFrame;
                  x       =  0;
                  y       =  0;
                  width   => <-.w3;
                  height  => <-.DialFrame.clientHeight;
                  title   = "Hue";
                  min     = 0.0;
                  max     = 1.0;
                  value<NEportLevels={2,2}> = 0.;
                  immediate<NEportLevels={2,1}> = 1;
               };
               HDial SDial<NEx=231,NEy=231,NEvisible=1> {
                  x     => <-.HDial.x + <-.HDial.width + 4;
                  title = "Saturation";
                  value<NEportLevels={2,2}> = 0.;
               };
               HDial VDial<NEx=385,NEy=231,NEvisible=1> {
                  x     => <-.SDial.x + <-.SDial.width + 4;
                  title = "Value";
                  value<NEportLevels={2,2}> = 0.4;
               };
            };
         };
         VUIToggle VUIToggle<NEx=209.,NEy=121.> {
            parent => <-.panel;
			   y+nres => <-.ColorEditor.Dials.DialFrame.y + <-.ColorEditor.Dials.DialFrame.height + 3;
			   width => parent.clientWidth-6;
			   x = 3;
            set<NEportLevels={1,1}> => BicolorSurface.Reverse;
            label = "Reverse side";
         };
      };
      link obj<NEportLevels={1,2},NEx=136.,NEy=174.> => BicolorSurface.obj;
   };
};

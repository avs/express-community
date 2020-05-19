
// vr2_macs.v
// James S Perrin
// Manchester Visualistion Centre Time-stamp: <00/02/23 11:30:56 zzcgujp>

flibrary VolRender2Macs {

   // new DataObject set up to volume render
   // applying the datamap over a given range
   macro VolRender2DataObject {
      group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255> {
         GDxform_templ &xform;
         method render;
      };
      imlink child_objs;
      DefaultProps Props;
      DefaultModes Modes<NEportLevels={0,2}> {
         mode = {0,0,0,3,0};
      };
      VolRenderMinMax MinMax {
         input => in;
         /* default values */
         min_value = 0.0;
         max_value = 255.0;
      };
      DATAMAPS.VolumeRender VolRenderDatamap {
         dataMin => <-.MinMax.min_value;
         dataMax => <-.MinMax.max_value;
         int rangesMin => min_array[index];
         int rangePosition => mid_array[index];
         int rangesMax => max_array[index];
         /* use user define range (UI) */
         DataRange = {
            {
               selectValues=1
            },
            {
               selectValues=1
            }
         };
         /* ratio based on define range (UI) */
         ratio => ((DataRange[0].UIMaxValue - DataRange[0].UIMinValue) / (DataRange[1].UIMaxValue - DataRange[0].UIMinValue));

         // grayscale ARGB default
         /*
         currentColorModel = 1;
         DatamapValue = {
            {
               v2=0.,v3=0.,v4=0,,
            },,,
            {
               v2=1.,v3=1.,v4=1.,,
            }
         };
         */
      };
      DefaultPickInfo PickInfo;
      GD.AltObject BoundsAltObj {
         alt_in => <-.in;
         Modes = {1,1,1,1,2};
      };
      DefaultObject Obj<NEportLevels={0,1}> {
         dmap => VolRenderDatamap;
         xform => in.xform;
         props => Props;
         modes => Modes;
         objects => child_objs;
         use_altobj = 1;
         altobj => <-.BoundsAltObj.obj;
         pick_info => PickInfo;
         name => name_of(<-.<-.<-);
         input => <-.in;
      };
      olink obj<NEportLevels={1,3}> => Obj;
   };




   // Modified standard volume render
   //  UI to set total range of datamap to be applied
   //  add toggle for alternative obj
   //  small bug fix in UI
   MODS.volume_render volume_render2 {
      VolRender2DataObject DataObject {
         in => in_mesh;
      };
      pos_slider {
         min => <-.DataObject.VolRenderDatamap.rangesMin;
         max => <-.DataObject.VolRenderDatamap.rangesMax;
         value => <-.DataObject.VolRenderDatamap.rangePosition;
         height => SliderFrame.height;
         SliderFrame {
            height => 3*<-.Slider.height+15;
         };
         SliderField {
            y = 10;
         };
         UIslider rangesMin_slider {
            parent => <-.SliderFrame;
            x = 0;
            y => <-.Slider.y + <-.Slider.height + 4;
            width => parent.clientWidth - 2;
            value => <-.<-.DataObject.VolRenderDatamap.rangesMin;
            min => <-.<-.DataObject.VolRenderDatamap.dataMin;
            max => <-.<-.DataObject.VolRenderDatamap.dataMax;
            mode = "integer";
            title = "Ranges Minimum";
         };
         UIslider rangesMax_slider {
            parent => <-.SliderFrame;
            x = 0;
            y => <-.rangesMin_slider.y + <-.rangesMin_slider.height + 4;
            width => parent.clientWidth  - 2;
            value => <-.<-.DataObject.VolRenderDatamap.rangesMax;
            min => <-.<-.DataObject.VolRenderDatamap.dataMin;
            max => <-.<-.DataObject.VolRenderDatamap.dataMax;
            mode = "integer";
            title = "Ranges Maximum";
         };
      };
      
      UItoggle alt_object {
         parent => <-.UImod_panel;
         x = 0;
         y => <-.max_slider.y + <-.max_slider.height + 4;
         width => parent.clientWidth;
         set => <-.DataObject.Obj.use_altobj;
         label = "Alternative Object";
      };
      
   };



   APPS.MultiWindowApp VolRender2Eg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.volume_render.DataObject.obj};
            };
            View {
               View {
                  renderer => "Software";
               };
            };
         };
         Scene_Selector {
            input_views => {Scene.View.View, <-.Vol2_Scene.View.View};
         };
         GDM.Uscene3D Vol2_Scene {
            Top {
               child_objs<NEportLevels={4,1}> => {
                  <-.<-.<-.volume_render2.DataObject.obj};
            };
            View {
               View {
                  renderer => "Software";
               };
            };
         };
      };


      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/hydrogen.fld";
         };
      };

      MODS.data_math data_math {
         in_field1 => <-.Read_Field.field;
         expres = "(#1*8)-1024";
         value_type = 2;
      };


      MODS.volume_render volume_render {
         in_mesh => <-.data_math.out_fld;

         DataObject {
            VolRenderDatamap {
               mid_array[3] = {127.5, 2047.5, 900.0 };
            };
         };
      };

      XP_COMM_PROJ.VolRender2.VolRender2Macs.volume_render2 volume_render2 {
         in_mesh => <-.data_math.out_fld;

         DataObject {
            VolRenderDatamap {
               min_array[3] = {0.0, 0.0, -1024.0};
               mid_array[3] = {127.5, 2047.5, 900.0 };
               max_array[3] = {255.0, 4095.0, 1024.0};
            };
         };
      };

   };

};


//
// example applications for AVS/Express Visit interface
//
// Th.Eickermann & W.Frings (October 2000)
//
//
//                  Visit - Visualization Interface Toolkit 
//                  Seap  - Service Announcement Protocol
//
//   Copyright (C) 2000, Forschungszentrum Juelich GmbH, Federal Republic of
//   Germany. All rights reserved.
//
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//     - Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//
//     - Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     - Any publications that result from the use of this software shall
//       reasonably refer to the Research Centre's development.
//
//     - All advertising materials mentioning features or use of this software
//       must display the following acknowledgement:
//
//           This product includes software developed by Forschungszentrum
//           Juelich GmbH, Federal Republic of Germany.
//
//     - Forschungszentrum Juelich GmbH is not obligated to provide the user with
//       any support, consulting, training or assistance of any kind with regard
//       to the use, operation and performance of this software or to provide
//       the user with any updates, revisions or new versions.
//
//
//   THIS SOFTWARE IS PROVIDED BY FORSCHUNGSZENTRUM JUELICH GMBH "AS IS" AND ANY
//   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//   DISCLAIMED. IN NO EVENT SHALL FORSCHUNGSZENTRUM JUELICH GMBH BE LIABLE FOR
//   ANY SPECIAL, DIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
//   RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
//   CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
//   CONNECTION WITH THE ACCESS, USE OR PERFORMANCE OF THIS SOFTWARE.
//
//
//


flibrary VisitApps<compile_subs=0> {

   APPS.ModuleStack VisitSimpleEg {

      XP_COMM_PROJ.Visit.VisitMacs.visitserver visitserver {
         VisitServerParams {
            Listen = 1;
            SeapService = "vserv";
            SeapPasswd = "demo_passwd";
         };
         IdDescriptions => {"read","write"};
      };

      XP_COMM_PROJ.Visit.VisitMacs.multiplexer multiplexer {
         In => <-.visitserver.Trigger;
      };


      macro VariablesWritten {
         double+OPort2 TimeStamp = 3.1415;
         int+OPort2 n1 = 3;
         int+OPort2 n2 = 1;
         int+OPort2 n3 = 1;
         int+OPort2 n4 = 1;
         int+OPort2 DataInt[3] => {1,2,3};
         byte+OPort2 DataByte[3] => {1,2,3};
         short+OPort2 DataShort[3] => {1,2,3};
         double+OPort2 DataDouble[3] => {1.,2.,3.};
         string+OPort2 DataString = "Test of visitwriter";
         int+OPort2 DataIntScalar = 4711;
         byte+OPort2 DataByteScalar = 248;
         short+OPort2 DataShortScalar = 41;
         double+OPort2 DataDoubleScalar = 2.71;
      };

      XP_COMM_PROJ.Visit.VisitMacs.visitwriter visitwriter {
         SockID => <-.visitserver.SockID;
         Trigger => <-.multiplexer.Out2;

         TimeStamp => <-.VariablesWritten.TimeStamp;
         n1 => <-.VariablesWritten.n1;
         n2 => <-.VariablesWritten.n2;
         n3 => <-.VariablesWritten.n3;
         n4 => <-.VariablesWritten.n4;
         DataInt => <-.VariablesWritten.DataInt;
         DataByte => <-.VariablesWritten.DataByte;
         DataShort => <-.VariablesWritten.DataShort;
         DataDouble => <-.VariablesWritten.DataDouble;
         DataString => <-.VariablesWritten.DataString;
         DataIntScalar => <-.VariablesWritten.DataIntScalar;
         DataByteScalar => <-.VariablesWritten.DataByteScalar;
         DataShortScalar => <-.VariablesWritten.DataShortScalar;
         DataDoubleScalar => <-.VariablesWritten.DataDoubleScalar;
      };

      XP_COMM_PROJ.Visit.VisitMacs.visitreader visitreader {
         SockID => <-.visitserver.SockID;
         Trigger => <-.multiplexer.Out1;
      };

      macro VariablesRead {
         double+IPort2 TimeStamp => <-.visitreader.TimeStamp;
         int+IPort2 n1 => <-.visitreader.n1;
         int+IPort2 n2 => <-.visitreader.n2;
         int+IPort2 n3 => <-.visitreader.n3;
         int+IPort2 n4 => <-.visitreader.n4;
         int+IPort2 DataInt[] => <-.visitreader.DataInt;
         byte+IPort2 DataByte[] => <-.visitreader.DataByte;
         short+IPort2 DataShort[] => <-.visitreader.DataShort;
         double+IPort2 DataDouble[] => <-.visitreader.DataDouble;
         string+IPort2 DataString => <-.visitreader.DataString;
         int+IPort2 DataIntScalar => <-.visitreader.DataIntScalar;
         byte+IPort2 DataByteScalar => <-.visitreader.DataByteScalar;
         short+IPort2 DataShortScalar => <-.visitreader.DataShortScalar;
         double+IPort2 DataDoubleScalar => <-.visitreader.DataDoubleScalar;
      };
   };




   APPS.SingleWindowApp VisitGoLEg {
        GDM.Uviewer3D Uviewer3D {
           Scene {
              Top {
                 child_objs => {
                    <-.<-.<-.glyph.out_obj,
                    <-.<-.<-.bounds.out_obj,
                    <-.<-.<-.Axis3D.out_obj
                 };
                 Xform {
                    mat = {
                       0.,0.,-1.,0.,
                       0.,1.,0.,0.,
                       1.,0.,0.,0.,
                       0.,0.,0.,1.
                    };
                 };
              };
              Camera {
                 Camera {
                    perspec = 1;
                    front = 1.;
                 };
              };
           };
        };


        macro VisitGoLCommunication {
           ilink steeringparams => <-.VisitGoLUI.steeringparams;
           ilink insertpos => <-.VisitGoLUI.insertpos;
           
           XP_COMM_PROJ.Visit.VisitMacs.visitserver visitserver {
              VisitServerParams {
                 Listen = 1;
                 SeapService = "cgol";
                 SeapPasswd = "dcgol";
              };
              IdDescriptions => {"field","2","status","4","5","6","params","ack","insertpos"};
           };

           XP_COMM_PROJ.Visit.VisitMods.Multiplexer Multiplexer {
              Inval => <-.visitserver.Trigger;
           };

           XP_COMM_PROJ.Visit.VisitMods.VisitWriter visitwriter_params {
              SockID => <-.visitserver.SockID;
              Trigger => <-.Multiplexer.Out7;
              DataInt => <-.steeringparams;
              n1 = 5;
              n2 = 1;
              n3 = 1;
              n4 = 1;
           };

           XP_COMM_PROJ.Visit.VisitMods.VisitWriter visitwriter_insert {
              SockID => <-.visitserver.SockID;
              Trigger => <-.Multiplexer.Out9;
              DataInt => <-.insertpos;
              n1 = 5;
              n2 = 1;
              n3 = 1;
              n4 = 1;
           };

           XP_COMM_PROJ.Visit.VisitMods.VisitReader visitreader_data {
              SockID => <-.visitserver.SockID;
              Trigger => <-.Multiplexer.Out1;
              n1 = 8;
              n2 = 8;
              n3 = 8;
              n4 = 1;
           };
           XP_COMM_PROJ.Visit.VisitMods.VisitReader visitreader_ack {
              SockID => <-.visitserver.SockID;
              Trigger => <-.Multiplexer.Out8;
           };
           XP_COMM_PROJ.Visit.VisitMods.VisitReader visitreader_info {
              SockID => <-.visitserver.SockID;
              Trigger => <-.Multiplexer.Out3;
           };


           FLD_MAP.concat_3_arrays concat_3_arrays {
              in1 => {<-.visitreader_data.n1};
              in2 => {<-.visitreader_data.n2};
              in3 => {<-.visitreader_data.n3};
           };

           FLD_MAP.uniform_scalar_field uniform_scalar_field {
              mesh {
                 in_dims => <-.<-.concat_3_arrays.out;
              };
              data {
                 in_data => <-.<-.visitreader_data.DataInt;
                 out {
                    node_data = {
                       {
                          null_flag=1,null_value=0,,,,id=1
                       }
                    };
                 };
              };
           };

           olink acknowledge => .visitreader_ack.DataInt;
           olink info => .visitreader_info.DataString;

           olink out_dims => .concat_3_arrays.out;
           olink out_fld => .uniform_scalar_field.out;
           olink out_obj => .uniform_scalar_field.obj;
        };


        macro VisitGoLUI {
           int &ack<NEportLevels={2,1}>[] => <-.VisitGoLCommunication.acknowledge;
           int &field_dim<NEportLevels={2,1}>[3] => <-.VisitGoLCommunication.out_dims;
           string &info<NEportLevels={2,1}> => <-.VisitGoLCommunication.info;

           UImod_panel VisitGoLModPanel {
              parent<NEportLevels={3,0}>;
              title = "Game of Life";
              message = "Select Game of Life Control Panel";
           };

           UIlabel VisitGoLTitle {
              parent => <-.VisitGoLModPanel;
              label = "Game of Life Options";
              alignment = "left";
              x = 0;
              y = 0;
              width => parent.width;
              color {
                 backgroundColor = "blue";
                 foregroundColor = "white";
              };
           };

           UIlabel VisitGoLStatus {
              parent => <-.VisitGoLModPanel;
              label+IPort2 => <-.info;
              x = 5;
              y => <-.VisitGoLTitle.y + <-.VisitGoLTitle.height + 10;
              width => parent.width - 10;
              color {
                 foregroundColor = "white";
                 backgroundColor = "darkgreen";
              };
              fontAttributes {
                 height = 10;
              };
              alignment = "left";
           };


           int col1pos = 0;
           int col1width => (.VisitGoLModPanel.width / 2.) - 2;

           int col2pos => (.VisitGoLModPanel.width / 2.) + 2;
           int col2width => .col1width;


           /* Start of First Column of UI Components */

           UItoggle PauseToggle {
              parent => <-.VisitGoLModPanel;
              label = "Pause";
              x => <-.col1pos;
              y => <-.VisitGoLStatus.y + <-.VisitGoLStatus.height + 10;
              width => <-.col1width;
           };


           UItoggle Insert_runner {
              parent => <-.VisitGoLModPanel;
              do+Port2;
              set+Port2 => ack[1];
              label = "Insert runner";
              x => <-.col1pos;
              y => <-.PauseToggle.y + <-.PauseToggle.height + 20;
              width => <-.col1width;
              fontAttributes {
                 height = 10;
              };
           };
           UIdial Direction {
              parent => <-.VisitGoLModPanel;
              value = 1.;
              min = -3.;
              max = 3.;
              mode = "integer";
              valuePerRev = 0.;
              style = 0;
              title = "Direction";
              x => <-.col1pos;
              y => <-.Insert_runner.y + <-.Insert_runner.height + 5;
              width => <-.col1width;
              height => width;
              fontAttributes {
                 height = 10;
              };
           };
           UItoggle Insert_xy_runner {
              parent => <-.VisitGoLModPanel;
              do+Port2;
              set+Port2 => ack[2];
              label = "Insert XY runner";
              x => <-.col1pos;
              y => <-.Direction.y + <-.Direction.height + 5;
              width => <-.col1width;
              fontAttributes {
                 height = 10;
              };
           };

           UItoggle Random_generate {
              parent => <-.VisitGoLModPanel;
              do+Port2;
              set+Port2 => ack[3];
              label = "Generate Random";
              x => <-.col1pos;
              y => <-.Percent.y + 15;
              width => <-.col1width;
              fontAttributes {
                 height = 10;
              };
           };


           /* Start of Second Column of UI Components */

           UItoggle StopToggle {
              parent => <-.VisitGoLModPanel;
              label = "Stop";
              set+Port2 => ack[0];
              x => <-.col2pos;
              y => <-.PauseToggle.y;
              width => <-.col2width;
           };

           UIslider Insert_pos_x {
              parent => <-.VisitGoLModPanel;
              value+Port2 = 4.;
              max+IPort2 => <-.field_dim[0];
              mode = "integer";
              title = "Insert pos x";
              x => <-.col2pos;
              y => <-.StopToggle.y + <-.StopToggle.height + 20;
              width => <-.col2width;
              fontAttributes {
                 height = 10;
              };
           };
           UIslider Insert_pos_y {
              parent => <-.VisitGoLModPanel;
              value+Port2 = 4.;
              max+IPort2 => <-.field_dim[1];
              mode = "integer";
              title = "Insert pos y";
              x => <-.col2pos;
              y => <-.Insert_pos_x.y + <-.Insert_pos_x.height + 5;
              width => <-.col2width;
              fontAttributes {
                 height = 10;
              };
           };
           UIslider Insert_pos_z {
              parent => <-.VisitGoLModPanel;
              value+Port2 = 4.;
              max+IPort2 => <-.field_dim[2];
              mode = "integer";
              title = "Insert pos z";
              x => <-.col2pos;
              y => <-.Insert_pos_y.y + <-.Insert_pos_y.height + 5;
              width => <-.col2width;
              fontAttributes {
                 height = 10;
              };
           };


           UIslider Percent {
              parent => <-.VisitGoLModPanel;
              value = 10.;
              min = 0.;
              max = 100.;
              mode = "integer";
              title = "Percent";
              decimalPoints = 0;
              fontAttributes {
                 height = 10;
              };
              x => <-.col2pos;
              y => <-.Insert_pos_z.y + <-.Insert_pos_z.height + 25;
              width => <-.col2width;
           };


           int steeringparams<NEportLevels={1,2}>[] => concat_array(.PauseToggle.set, .StopToggle.set, .Insert_runner.set, .Insert_xy_runner.set, .Random_generate.set);
           int insertpos<NEportLevels={1,2}>[] => concat_array(.Insert_pos_x.value, .Insert_pos_y.value, .Insert_pos_z.value, .Direction.value, .Percent.value);
        };



        MODS.crop crop {
           in_field => <-.VisitGoLCommunication.out_fld;
           CropParam {
              min = {1,1,1};
              max<NEportLevels={3,2}> => (<-.<-.VisitGoLCommunication.out_dims - 2);
           };
        };

        GEOMS.Diamond3D Diamond3D;

        MODS.glyph glyph {
           in_field => <-.crop.out_fld;
           in_glyph => <-.Diamond3D.out_fld;
           GlyphParam {
              vector = 0;
              scale = 0.63;
              normalize = 1;
           };
           GlyphUI {
              scale_slider {
                 min = 0.;
                 max = 1.;
                 decimalPoints = 2;
              };
           };
           obj {
              Obj {
                 cache_size = 40;
              };
              Modes {
                 mode = {0,1,4,0,0};
              };
              Datamap {
                 DataRange = {
                    {
                       UIMaxValue=31.01,DataMaxValue=31.01,UIMinValue=31.,DataMinValue=31.,,,,,,,,,
                    }
                 };
                 DatamapValue = {
                    {
                       v4=0.,v3=0.,v2=1.,
                    },
                    {
                       v3=0.37,,
                    }
                 };
                 currentColorModel = 1;
              };
           };
        };

        MODS.bounds bounds {
           in_field => <-.crop.out_fld;
           BoundsParam {
              imin = 1;
              jmin = 1;
              kmin = 1;
           };
           obj {
              Datamap {
                 DataRange = {
                    {
                       UIMaxValue=1.,,,,,,,,,,,,
                    }
                 };
              };
              Modes {
                 mode = {0,2,1,0,0};
              };
           };
        };

        GEOMS.Axis3D Axis3D {
           in_field => <-.crop.out_fld;
           x_axis_param {
              step = 10.;
              ndig = 0;
              minor_ticks = 0;
              off_anno = 0.2;
           };
           y_axis_param {
              step = 10.;
              ndig = 0;
              minor_ticks = 0;
              off_anno = 0.2;
           };
           z_axis_param {
              step = 10.;
              ndig = 0;
              minor_ticks = 0;
              off_anno = 0.2;
           };
        };

   };


};


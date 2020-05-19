//
// macros for AVS/Express Visit interface
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


flibrary VisitMacs<compile_subs=0> {


   macro VisitServerUI {
      XP_COMM_PROJ.Visit.VisitMods.VisitServerParams &VisitServerParams<NEportLevels={2,1}>;
      int    &Action<NEportLevels={2,1}>;
      string &Status<NEportLevels={2,1}>;

      UImod_panel VisitServerModPanel {
         parent<NEportLevels={4,0}>;
         title = "Visit Server";
         message = "Select Visit Server Control Panel";
      };

      UIlabel VisitStatusLabel {
         parent => <-.VisitServerModPanel;
         label = "Visit Status";
         alignment = "left";
         x = 0;
         y = 0;
         width => parent.width;
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };


      UItoggle ListenToggle {
         parent => <-.VisitServerModPanel;
         label = "active (listen to socket)";
         set => <-.VisitServerParams.Listen;
         x = 3;
         y => <-.VisitStatusLabel.y + <-.VisitStatusLabel.height + 15;
         width => parent.width - 6;
      };

      string+Port actionscolors[] => {"red","yellow","green","blue"};
      string+Port curr_actioncolor => actionscolors[.Action];

      UIlabel StatusLabel {
         parent => <-.VisitServerModPanel;
         label => <-.Status;
         x = 3;
         y => <-.ListenToggle.y + <-.ListenToggle.height + 5;
         width => parent.width;
         alignment = "left";
         color {
            foregroundColor+IPort3 => <-.<-.curr_actioncolor;
            backgroundColor = "black";
         };
         fontAttributes {
            height = 12;
            family = "courier";
            weight = "medium";
            slant = "regular";
            set_width = "narrow";
         };
      };


      UIlabel VisitParametersLabel {
         parent => <-.VisitServerModPanel;
         label = "Visit Parameters";
         alignment = "left";
         x = 0;
         y => <-.StatusLabel.y + <-.StatusLabel.height + 15;
         width => parent.width;
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };


      UIlabel InterfaceLabel {
         parent => <-.VisitServerModPanel;
         label = "Interface";
         x = 0;
         y => <-.InterfaceUIfield.y + 5;
         width => parent.width / 4.;
         alignment = "left";
      };
      UItext InterfaceUIfield {
         parent => <-.VisitServerModPanel;
         text+Port2 => <-.VisitServerParams.Interface;
         x => parent.width / 4. + 20;
         y => <-.VisitParametersLabel.y + <-.VisitParametersLabel.height + 10;
         width => ((3. * parent.width) / 4.) - 25;
      };

      UIlabel SEAPserviceLabel {
         parent => <-.VisitServerModPanel;
         label = "Service";
         x = 0;
         y => <-.SEAPserviceUIfield.y + 5;
         width => parent.width / 4.;
         alignment = "left";
      };
      UItext SEAPserviceUIfield {
         parent => <-.VisitServerModPanel;
         text+Port2 => <-.VisitServerParams.SeapService;
         x => parent.width / 4. + 20;
         y => <-.InterfaceUIfield.y + <-.InterfaceUIfield.height + 10;
         width => ((3. * parent.width) / 4.) - 25;
      };

      UIlabel SEAPpasswdLabel {
         parent => <-.VisitServerModPanel;
         label = "Password";
         x = 0;
         y => <-.SEAPpasswdUIfield.y + 5;
         width => parent.width / 4.;
         alignment = "left";
      };
      UItext SEAPpasswdUIfield {
         parent => <-.VisitServerModPanel;
         text+Port2 => <-.VisitServerParams.SeapPasswd;
         x => parent.width / 4. + 20;
         y => <-.SEAPserviceUIfield.y + <-.SEAPserviceUIfield.height + 10;
         width => ((3. * parent.width) / 4.) - 25;
      };
   };



   macro VisitServerFunc {
      XP_COMM_PROJ.Visit.VisitMods.VisitServerParams &VisitServerParams<NEportLevels={2,1}>;
      string &IdDescriptions<NEportLevels={2,1}>[];

      XP_COMM_PROJ.Visit.VisitMods.VisitServer VisitServer {
         Port           => <-.VisitServerParams.Port;
         Interface      => <-.VisitServerParams.Interface;
         Listen         => <-.VisitServerParams.Listen;
         SeapService    => <-.VisitServerParams.SeapService;
         SeapPasswd     => <-.VisitServerParams.SeapPasswd;
         IdDescriptions => <-.IdDescriptions;
      };

      olink SockID  => .VisitServer.SockID;
      olink Action  => .VisitServer.Action;
      olink Trigger => .VisitServer.Trigger;
      olink Status  => .VisitServer.Status;
   };



   macro visitserver {
      XP_COMM_PROJ.Visit.VisitMods.VisitServerParams VisitServerParams {
         Port = 0;
         Listen = 0;
         Interface = "*";
         SeapService = "";
         SeapPasswd = "";
      };

      string+OPort IdDescriptions[] = {};

      VisitServerFunc VisitServerFunc {
         VisitServerParams => <-.VisitServerParams;
         IdDescriptions => <-.IdDescriptions;
      };
      VisitServerUI VisitServerUI {
         VisitServerParams => <-.VisitServerParams;
         Action => <-.VisitServerFunc.Action;
         Status => <-.VisitServerFunc.Status;
      };


      olink SockID  => .VisitServerFunc.SockID;
      olink Trigger => .VisitServerFunc.Trigger;
   };



   macro visitreader {
      ilink SockID;
      ilink Trigger;
      
      XP_COMM_PROJ.Visit.VisitMods.VisitReader VisitReader {
         SockID => <-.SockID;
         Trigger => <-.Trigger;
      };

      olink TimeStamp => .VisitReader.TimeStamp;
      olink n1 => .VisitReader.n1;
      olink n2 => .VisitReader.n2;
      olink n3 => .VisitReader.n3;
      olink n4 => .VisitReader.n4;
      olink DataInt => .VisitReader.DataInt;
      olink DataByte => .VisitReader.DataByte;
      olink DataShort => .VisitReader.DataShort;
      olink DataDouble => .VisitReader.DataDouble;
      olink DataString => .VisitReader.DataString;
      olink DataIntScalar => .VisitReader.DataIntScalar;
      olink DataByteScalar => .VisitReader.DataByteScalar;
      olink DataShortScalar => .VisitReader.DataShortScalar;
      olink DataDoubleScalar => .VisitReader.DataDoubleScalar;
   };



   macro visitwriter {
      ilink SockID;
      ilink Trigger;
      ilink TimeStamp;
      ilink n1;
      ilink n2;
      ilink n3;
      ilink n4;
      ilink DataInt;
      ilink DataByte;
      ilink DataShort;
      ilink DataDouble;
      ilink DataString;
      ilink DataIntScalar;
      ilink DataByteScalar;
      ilink DataShortScalar;
      ilink DataDoubleScalar;

      XP_COMM_PROJ.Visit.VisitMods.VisitWriter VisitWriter {
         SockID => <-.SockID;
         Trigger => <-.Trigger;
         TimeStamp => <-.TimeStamp;
         DataInt => <-.DataInt;
         DataByte => <-.DataByte;
         DataShort => <-.DataShort;
         DataDouble => <-.DataDouble;
         DataString => <-.DataString;
         DataIntScalar => <-.DataIntScalar;
         DataByteScalar => <-.DataByteScalar;
         DataShortScalar => <-.DataShortScalar;
         DataDoubleScalar => <-.DataDoubleScalar;
         n1 => <-.n1;
         n2 => <-.n2;
         n3 => <-.n3;
         n4 => <-.n4;
      };
   };


   macro multiplexer {
      ilink In;

      XP_COMM_PROJ.Visit.VisitMods.Multiplexer Multiplexer {
         Inval => <-.In;
      };

      olink Out1 => .Multiplexer.Out1;
      olink Out2 => .Multiplexer.Out2;
      olink Out3 => .Multiplexer.Out3;
      olink Out4 => .Multiplexer.Out4;
      olink Out5 => .Multiplexer.Out5;
      olink Out6 => .Multiplexer.Out6;
      olink Out7 => .Multiplexer.Out7;
      olink Out8 => .Multiplexer.Out8;
      olink Out9 => .Multiplexer.Out9;
      olink Out10 => .Multiplexer.Out10;
   };

};


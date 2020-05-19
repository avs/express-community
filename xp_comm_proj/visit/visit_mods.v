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


flibrary VisitMods <build_dir="xp_comm_proj/visit",
                    link_files="-L/usr/local/visit/lib -lvisit",
                    hdr_dirs="/usr/local/visit/include",
                    out_hdr_file="gen.h">
{

      group+OPort VisitServerParams {
         int+Port2    Port;
         string+Port2 Interface;
         int+Port2    Listen;
         string+Port2 SeapService;
         string+Port2 SeapPasswd;
      };


      module Multiplexer<src_file="Multiplexer.c"> {
         int+IPort2 Inval;

         omethod+req Multiplex(
            Inval+read+notify+req,
            Out1+write,
            Out2+write,
            Out3+write,
            Out4+write,
            Out5+write,
            Out6+write,
            Out7+write,
            Out9+write,
            Out10+write
         ) = "Multiplex";

         int+OPort2 Out1;
         int+OPort2 Out2;
         int+OPort2 Out3;
         int+OPort2 Out4;
         int+OPort2 Out5;
         int+OPort2 Out6;
         int+OPort2 Out7;
         int+OPort2 Out8;
         int+OPort2 Out9;
         int+OPort2 Out10;
      };


      module VisitServer<src_file="VisitServer.c"> {
         int+IPort2    Port        = 0;
         string+IPort2 Interface   = "*";
         int+IPort2    Listen      = 0;
         string+IPort2 SeapService = "VISIT_AVS";
         string+IPort2 SeapPasswd  = "demo";

         string+IPort2 IdDescriptions[];

         int+IPort2 updateStatus = 1;

         ptr internal;

         omethod+notify_inst+req VisitServer_inst(
            Port+read+req,
            Interface+read+req,
            SeapService+read+req,
            SeapPasswd+read+req,
            Listen+read+req,
            Action+write,
            Status+write
         ) = "VisitServer_inst";
         omethod+notify_deinst VisitServer_deinst(
            Action+write,
            Status+write
         ) = "VisitServer_deinst";

         omethod+req SeapUpdateService(
            SeapService+read+notify+req,
            Action+write,
            Status+write
         ) = "SeapUpdateService";
         omethod+req SeapUpdatePasswd(
            SeapPasswd+read+notify+req,
            Action+write,
            Status+write
         ) = "SeapUpdatePasswd";
         omethod+req SeapUpdateInterface(
            Interface+read+notify+req,
            Action+write,
            Status+write
         ) = "SeapUpdateInterface";
         omethod+req ListenUpdate(
            Listen+read+notify+req,
            Action+write,
            Status+write
         ) = "ListenUpdate";
         omethod+req IdDescriptionsUpdate(
            IdDescriptions+read+notify
         ) = "IdDescriptionsUpdate";

         int+OPort2    SockID = -1;
         int+OPort2    Trigger = 0;
         int+OPort2    Action = 0;
         string+OPort2 Status = "<Init>";
      };


      module VisitReader<src_file="VisitReader.c"> {
         int+IPort2 SockID = -1;
         int+IPort2 Trigger = 0;

         omethod+notify_inst VisitReader_inst(
            TimeStamp+write,
            n1+read+write,
            n2+read+write,
            n3+read+write,
            n4+read+write,
            DataIntSize+write,
            DataInt+write,
            DataShortSize+write,
            DataShort+write,
            DataByteSize+write,
            DataByte+write,
            DataDoubleSize+write,
            DataDouble+write,
            DataString+write,
            DataIntScalar+write,
            DataByteScalar+write,
            DataShortScalar+write,
            DataDoubleScalar+write
         ) = "VisitReader_inst";

         omethod+req VisitReader_read(
            SockID+read+req,
            Trigger+read+notify+req,
            TimeStamp+write,
            n1+write,
            n2+write,
            n3+write,
            n4+write,
            DataIntSize+write,
            DataInt+write,
            DataByteSize+write,
            DataByte+write,
            DataShortSize+write,
            DataShort+write,
            DataDoubleSize+write,
            DataDouble+write,
            DataString+write,
            DataIntScalar+write,
            DataShortScalar+write,
            DataByteScalar+write,
            DataDoubleScalar+write
         ) = "VisitReader_read";

         double+OPort2 TimeStamp;

         int+OPort2 n1 = -1;
         int+OPort2 n2 = -1;
         int+OPort2 n3 = -1;
         int+OPort2 n4 = -1;
         int o1 = -1;
         int o2 = -1;
         int o3 = -1;
         int o4 = -1;
         int s1 = -1;
         int s2 = -1;
         int s3 = -1;
         int s4 = -1;

         int DataIntSize = 0;
         int+OPort2 DataInt[DataIntSize];
         int DataByteSize = 0;
         byte+OPort2 DataByte[DataByteSize];
         int DataShortSize = 0;
         short+OPort2 DataShort[DataShortSize];
         int DataDoubleSize = 0;
         double+OPort2 DataDouble[DataDoubleSize];
         string+OPort2 DataString;
         int+OPort2 DataIntScalar;
         byte+OPort2 DataByteScalar;
         short+OPort2 DataShortScalar;
         double+OPort2 DataDoubleScalar;
      };


      module VisitWriter<src_file="VisitWriter.c"> {
         int+IPort2    SockID = -1;
         int+IPort2    Trigger = 0;
         double+IPort2 TimeStamp = 0.;

         int+IPort2 DataInt[];
         byte+IPort2 DataByte[];
         short+IPort2 DataShort[];
         double+IPort2 DataDouble[];
         string+IPort2 DataString = "";
         int+IPort2 DataIntScalar = -1;
         byte+IPort2 DataByteScalar = -1;
         short+IPort2 DataShortScalar = -1;
         double+IPort2 DataDoubleScalar = -1.;

         int+IPort2 n1 = -1;
         int+IPort2 n2 = -1;
         int+IPort2 n3 = -1;
         int+IPort2 n4 = -1;
         int o1 = -1;
         int o2 = -1;
         int o3 = -1;
         int o4 = -1;
         int s1 = -1;
         int s2 = -1;
         int s3 = -1;
         int s4 = -1;

         omethod+req VisitWriter_read(
            SockID+read+req,
            Trigger+read+notify+req,
            TimeStamp+read,
            n1+read,
            n2+read,
            n3+read,
            n4+read,
            o1+read,
            o2+read,
            o3+read,
            o4+read,
            s1+read,
            s2+read,
            s3+read,
            s4+read,
            DataInt+read,
            DataByte+read,
            DataShort+read,
            DataDouble+read,
            DataString+read,
            DataIntScalar+read,
            DataByteScalar+read,
            DataShortScalar+read,
            DataDoubleScalar+read
         ) = "VisitWriter_read";
      };

};


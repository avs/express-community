
flibrary InstrumentationMods <build_dir="xp_comm_proj/instment",
                              out_src_file = "gen.cxx",
                              out_hdr_file = "gen.hxx">
{

  /*
   * global attribute & parameter block with default settings
   */
  group+OPort instr_params {
     int+Port2    timerOn = 0;
     string+Port2 timerFile = "$XP_PATH<1>/avs.log";
  };


  /*
   * Usage
   *
   * Synopsis           : reports time, elapsed time memory usage to the
   *                      specified file
   */

  module usage<src_file="usage.cxx"> {
     string filename;
     string message;
     prim startTrigger;
     prim outTrigger;

     ptr timePtr = 0;
 
     cxxmethod+req init(startTrigger+notify, // the only thing with notify
                        timePtr+read+write);

     cxxmethod+req output(filename+read+req,
                          message+read,
                          outTrigger+notify,  // the only thing with notify
                          timePtr+read);
  };



  /*
   *  gtrace - gated ARR trace control
   *
   * Synopsis           : reports OM memory usage by toggling the ARRset_trace mode
   */

  module gtrace<src_file="gtrace.cxx"> {
     int mode = 0;
     cxxmethod+req update( mode+read+notify); // the only thing with notify
  };


  /*
   * Print VCP 
   *
   * Synopsis           : prints a message to stdout - in the VCP window
   */

  module prvcp<src_file="prvcp.cxx"> {
     string message;
     prim Trigger;
 
     cxxmethod+req update(message+read,
                          Trigger+notify); // the only thing with notify
  };



  /*
   * obj_stats 
   *
   * Synopsis           : reports application object counts
   *                      specified log file
   */

  module obj_stats<src_file="objstats.cxx"> {
     string filename;
     string message;
     int depth = 1;
     prim Trigger;
 
     cxxmethod+req update(filename+read+req,
                          message+read,
                          depth+read,
                          Trigger+notify); // the only thing with notify
  };


  /*
   * special version of parse_v
   */
/*
  GMOD.parse_v parseV {
       on_inst = 0;
  };
*/

};


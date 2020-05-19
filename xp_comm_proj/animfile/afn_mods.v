
flibrary AnimFilenameMods <cxx_name=""> {

  // define the parameter block groups

  group AnimFilenameParams <NEportLevels={0,1}> {
    string pre <NEportLevels={2,2}> = "";
    string post <NEportLevels={2,2}> = "";
    int precision <NEportLevels={2,2}> = 1;
  };

  group AnimLoopParams <NEportLevels={0,1}> {
    boolean reset <NEportLevels={2,2}> = 0;
    boolean run <NEportLevels={2,2}> = 0;
    boolean cycle <NEportLevels={2,2}> = 0;
    int start_val <NEportLevels={2,2}> = 0;
    int end_val <NEportLevels={2,2}> = 1;
    int incr <NEportLevels={2,2}> = 1;
  };

  module AnimFilenameGen {
    AnimFilenameParams &AnimFilenameParams<NEportLevels={2,1}>;
    string pre => AnimFilenameParams.pre;
    string post => AnimFilenameParams.post;
    int number<NEportLevels={2,1}>;
    int precision => AnimFilenameParams.precision;
    string format => str_format("%%s%%.%dd%%s",.precision);
    string output<NEportLevels={0,2}> =>
        str_format(.format,.pre,.number,.post);
  };

  GMOD.loop AnimFilenameLoop {
    AnimLoopParams &AnimLoopParams <NEportLevels={2,1}>;
    reset <NEportLevels={0,0}> => AnimLoopParams.reset;
    run <NEportLevels={0,0}> => AnimLoopParams.run;
    cycle <NEportLevels={0,0}> => AnimLoopParams.cycle;
    start_val <NEportLevels={0,0}> => AnimLoopParams.start_val;
    end_val <NEportLevels={0,0}> => AnimLoopParams.end_val;
    incr <NEportLevels={0,0}> => AnimLoopParams.incr;
  };
};



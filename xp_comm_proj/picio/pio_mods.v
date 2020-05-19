
//
// Copyright (C) Chris Pudney, The University of Western Australia, 1997.
// All rights reserved.
// 
// Permission to use, copy, modify and distribute this software and its
// documentation only for the purposes of teaching and research is hereby
// granted without fee, provided that the above copyright notice and this
// permission notice appear in all copies of this software/documentation and
// that you do not sell the software.  No commercial use or distribution
// of the software is permitted without the consent of the copyright
// owners.  Commercial licensing is available by contacting the author(s).
//
// THIS SOFTWARE/DOCUMENTATION IS PROVIDED WITH NO WARRANTY, EXPRESS OR
// IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTY OF MERCHANTABILITY OR
// FITNESS FOR A PARTICULAR PURPOSE.
//

flibrary PICioMods<compile_subs=0> {

  module Read_PIC_core<data_import_module=1> {
    file+IPort2 &FILE;

    //FIXME: Code currently performs byte reordering even when it is
    //       not necessary.  Ideally detect endian nature and only
    //       reorder when necessary.

    int bytes[6] => cache(file_obj_bin(.FILE,0,0));
    int+OPort2 dims[3]  => { 
                             (bytes[0] + bytes[1]*256),
                             (bytes[2] + bytes[3]*256),
                             (bytes[4] + bytes[5]*256)
                           };

    byte+OPort2 data[prod(dims)] => cache(file_obj_bin(.FILE,76,0));
  };

};


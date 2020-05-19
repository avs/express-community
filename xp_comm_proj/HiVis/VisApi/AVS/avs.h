// Technische Universität München - Peter.Breitling@in.tum.de
// Please read the file LICENSE for conditions associated with this file.

#define visPlatAVSEXPRESS

//#include "user.h"
#include "express.h"

#include "avs/om.h"

#ifdef __cplusplus
#include "avs/omx.hxx"
#endif

#ifdef __cplusplus
#include "avs/gd.h"
#endif

#ifdef __cplusplus
#include "fld/Xfld.h"
#endif

#define visAVS_Grid_Struct 0
#define visAVS_Grid_Unif   1
#define visAVS_Grid_Rect   2

/* -----------------------------------------------------------------------------
   AVS/Express HiVis Module prototypes
   -------------------------------------------------------------------------- */

//class HiVis_HiVisMods_HiVis_Grid;

/* -----------------------------------------------------------------------------
   FieldAccessLink
   ---------------------------------------------------------------------------*/

class HiVis_Grid_Link;
class HiVis_HiVisMods_HiVis_Grid;

class visGrid;
class visAdaptiveGrid;
class visStructuredGrid;
class visUnstructuredGrid;
class visGridLink {
  friend class HiVis_HiVisMods_HiVis_Grid;
public:
  visGridLink(HiVis_HiVisMods_HiVis_Grid *hiVisFieldAccess);
  virtual ~visGridLink();

  visAdaptiveGrid *castToAdaptiveGrid();
  visStructuredGrid *castToStructuredGrid();
  visUnstructuredGrid *castToUnstructuredGrid();

  visGrid *myFieldAccess;
  HiVis_HiVisMods_HiVis_Grid *myHiVisFieldAccess;
};

//  ERRverror("", ERR_NO_HEADER | ERR_INFO, 

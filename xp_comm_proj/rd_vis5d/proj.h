
//
//	Map projections and vertical coordinate systems:
//
#define PROJ_GENERIC          0  /* No specific units */
#define PROJ_LINEAR           1  /* Cylindrical-Equidistant (old vis5d) */
#define PROJ_LAMBERT          2  /* Lambert conformal */
#define PROJ_STEREO           3  /* Stereographic */
#define PROJ_ROTATED          4  /* Rotated equidistant */
#define PROJ_MERCATOR         5  /* Mercator */
#define PROJ_LAST			  5  /* Last projection number */

#define VERT_GENERIC          0 /* No specific units */
#define VERT_EQUAL_KM         1 /* Equally spaced in kilometers */
#define VERT_NONEQUAL_KM      2 /* Non-equally spaced in kilometers */
#define VERT_NONEQUAL_MB      3 /* Non-equally spaced in millibars */
#define VERT_LAST             3 /* Last vertical spacin code */

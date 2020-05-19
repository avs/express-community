/*********************************************************************

  this is the generic include file for procedure scope

*********************************************************************/


/*********************************************************************

  version information

*********************************************************************/

/* AVSmessage stuff */
char LFNC_version_info[2][128] = {

  "version 0",
  "version 1"

};


/* charcter manipulation array */
char chuge[1024];

char *cstart, *cfinal;

/* finction name */
char LFNC_function_name[128] = "";

/* environment debug flag */
char *debug_env = getenv( "XP_DEBUG" );



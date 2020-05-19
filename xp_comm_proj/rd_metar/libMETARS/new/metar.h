@@METAR.H
#ifndef METARX
#define METARX
 
/********************************************************************/
/*                                                                  */
/*  Title:         METAR H                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          19 Jan 1996                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      METAR Decoder Header File.                       */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
 
#include <local h>     /* standard header file */
 
 
/*********************************************/
/*                                           */
/* RUNWAY VISUAL RANGE STRUCTURE DECLARATION */
/*       AND VARIABLE TYPE DEFINITION        */
/*                                           */
/*********************************************/
 
typedef struct runway_VisRange {
   char runway_designator[6];
   bool vrbl_visRange;
   bool below_min_RVR;
   bool above_max_RVR;
   int  visRange;
   int  Max_visRange;
   int  Min_visRange;
}  Runway_VisRange;
 
/***********************************************/
/*                                             */
/* DISPATCH VISUAL RANGE STRUCTURE DECLARATION */
/*       AND VARIABLE TYPE DEFINITION          */
/*                                             */
/***********************************************/
 
typedef struct dispatch_VisRange {
   bool vrbl_visRange;
   bool below_min_DVR;
   bool above_max_DVR;
   int  visRange;
   int  Max_visRange;
   int  Min_visRange;
}  Dispatch_VisRange;
 
/*****************************************/
/*                                       */
/* CLOUD CONDITION STRUCTURE DECLARATION */
/*      AND VARIABLE TYPE DEFINITION     */
/*                                       */
/*****************************************/
 
typedef struct cloud_Conditions {
   char cloud_type[5];
   char cloud_hgt_char[4];
   char other_cld_phenom[4];
   int  cloud_hgt_meters;
}  Cloud_Conditions;
 
/*****************************************/
/*                                       */
/* WIND GROUP DATA STRUCTURE DECLARATION */
/*      AND VARIABLE TYPE DEFINITION     */
/*                                       */
/*****************************************/
 
typedef struct windstruct {
   char windUnits[ 4 ];
   bool windVRB;
   int windDir;
   int windSpeed;
   int windGust;
} WindStruct;
 
/*****************************************/
/*                                       */
/* RECENT WX GROUP STRUCTURE DECLARATION */
/*      AND VARIABLE TYPE DEFINITION     */
/*                                       */
/*****************************************/
 
typedef struct recent_wx {
   char Recent_weather[ 5 ];
   int  Bhh;
   int  Bmm;
   int  Ehh;
   int  Emm;
} Recent_Wx;
 
/***************************************/
/*                                     */
/* DECODED METAR STRUCTURE DECLARATION */
/*     AND VARIABLE TYPE DEFINITION    */
/*                                     */
/***************************************/
 
typedef struct decoded_METAR {
   char synoptic_cloud_type[ 6 ];
   char snow_depth_group[ 6 ];
   char codeName[ 6 ];
   char stnid[5];
   char horiz_vsby[5];
   char dir_min_horiz_vsby[3];
   char vsby_Dir[ 3 ];
   char WxObstruct[10][8];
   char autoIndicator[5];
   char VSBY_2ndSite_LOC[10];
   char SKY_2ndSite_LOC[10];
   char SKY_2ndSite[10];
   char SectorVsby_Dir[ 3 ];
   char ObscurAloft[ 12 ];
   char ObscurAloftSkyCond[ 12 ];
   char VrbSkyBelow[ 4 ];
   char VrbSkyAbove[ 4 ];
   char LTG_DIR[ 3 ];
   char CloudLow;
   char CloudMedium;
   char CloudHigh;
   char CIG_2ndSite_LOC[10];
   char VIRGA_DIR[3];
   char TornadicType[15];
   char TornadicLOC[10];
   char TornadicDIR[4];
   char TornadicMovDir[3];
   char CHINO_LOC[6];
   char VISNO_LOC[6];
   char PartialObscurationAmt[2][7];
   char PartialObscurationPhenom[2][12];
   char SfcObscuration[6][10];
   char charPrevailVsby[12];
   char charVertVsby[10];
   char TS_LOC[3];
   char TS_MOVMNT[3];
 
   bool Indeterminant3_6HrPrecip;
   bool Indeterminant_24HrPrecip;
   bool CIGNO;
   bool SLPNO;
   bool ACFTMSHP;
   bool NOSPECI;
   bool FIRST;
   bool LAST;
   bool SunSensorOut;
   bool AUTO;
   bool COR;
   bool NIL_rpt;
   bool CAVOK;
   bool RVRNO;
   bool A_altstng;
   bool Q_altstng;
   bool VIRGA;
   bool VOLCASH;
   bool GR;
   bool CHINO;
   bool VISNO;
   bool PNO;
   bool PWINO;
   bool FZRANO;
   bool TSNO;
   bool DollarSign;
   bool PRESRR;
   bool PRESFR;
   bool Wshft_FROPA;
   bool OCNL_LTG;
   bool FRQ_LTG;
   bool CNS_LTG;
   bool CG_LTG;
   bool IC_LTG;
   bool CC_LTG;
   bool CA_LTG;
   bool DSNT_LTG;
   bool AP_LTG;
   bool VcyStn_LTG;
   bool OVHD_LTG;
   bool LightningVCTS;
   bool LightningTS;
 
   int  TornadicDistance;
   int  ob_hour;
   int  ob_minute;
   int  ob_date;
   int minWnDir;
   int maxWnDir;
   int VertVsby;
   int temp;
   int dew_pt_temp;
   int QFE;
   int hectoPasc_altstng;
   int char_prestndcy;
   int minCeiling;
   int maxCeiling;
   int WshfTime_hour;
   int WshfTime_minute;
   int min_vrbl_wind_dir;
   int max_vrbl_wind_dir;
   int PKWND_dir;
   int PKWND_speed;
   int PKWND_hour;
   int PKWND_minute;
   int SKY_2ndSite_Meters;
   int Ceiling;
   int Estimated_Ceiling;
   int SNINCR;
   int SNINCR_TotalDepth;
   int SunshineDur;
   int ObscurAloftHgt;
   int VrbSkyLayerHgt;
   int Num8thsSkyObscured;
   int CIG_2ndSite_Meters;
   int snow_depth;
   int BTornadicHour;
   int BTornadicMinute;
   int ETornadicHour;
   int ETornadicMinute;
 
 
   float SectorVsby;
   float WaterEquivSnow;
   float VSBY_2ndSite;
   float prevail_vsbySM;
   float prevail_vsbyM;
   float prevail_vsbyKM;
   float prestndcy;
   float precip_amt;
   float precip_24_amt;
   float maxtemp;
   float mintemp;
   float max24temp;
   float min24temp;
   float minVsby;
   float maxVsby;
   float hourlyPrecip;
   float TWR_VSBY;
   float SFC_VSBY;
   float Temp_2_tenths;
   float DP_Temp_2_tenths;
   float SLP;
   float GR_Size;
 
   double inches_altstng;
 
   Runway_VisRange RRVR[12];
   Dispatch_VisRange DVR;
   Recent_Wx ReWx[3];
   WindStruct winData;
   Cloud_Conditions cldTypHgt[6];
 
}  Decoded_METAR;
 
#define MAXWXSYMBOLS 10       /*-- NOT TO EXCEED 10 PRES. WX GRPS --*/
#define MAXTOKENS    500      /*--  RPT NOT TO EXCEED 500 GRPS   --*/
 
 
#endif
@@LOCAL.H
/*********************************************************************/
/*                                                                   */
/*  Title: local h                                                   */
/*  Updated: 10 June 1996                                            */
/*  Organization: W/OSO242 - Graphics and Display Section            */
/*  Language: C/370                                                  */
/*                                                                   */
/*  Abstract:                                                        */
/*  This header file provides all function definitions necessary for */
/*  the OSO242 C function library.                                   */
/*                                                                   */
/*********************************************************************/
 
#ifndef locallib_defined
#define locallib_defined
 
 
 
/*****************/
/* Include Files */
/*****************/
 
#include <assert.h>
#include <ctype.h>
/* #include <ctest.h>   Includes IBM specific debugging libraries  */
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 
 
 
/********************/
/* Standard Defines */
/********************/
 
#define FALSE        0                 /* boolean value */
#define MAXINT       INT_MAX           /* maximum integer */
#define MININT       INT_MIN           /* minimum integer */
#define MAXNEG       INT_MIN           /* minimum integer */
#define NO           FALSE             /* boolean value */
#define TRUE         1                 /* boolean value */
#define TRUNCATED    -1                /* indicates truncation */
#define YES          TRUE              /* boolean value */
 
 
/*****************/
/* Macro defines */
/*****************/
 
#define ABS(x)       (((x) < 0) ? -(x) : (x))
#define clearscrn    system("CLRSCRN")
#define assgndev(d, v) v = 0x##d
#define DIM(a)       (sizeof(a) / sizeof(a[0]))
#define FOREVER      for(;;)           /* endless loop */
#define getln(s, n)  ((fgets(s, n, stdin)==NULL) ? EOF : strlen(s))
#define IMOD(i, j)   (((i) % (j)) < 0 ? ((i) % (j))+(j) : ((i) % (j)))
#define IN_RANGE(n, lo, hi) ((lo) <= (n) && (n) <= (hi))
#define LOOPDN(r, n) for ((r) = (n)+1; --(r) > 0;)
#define MAX(x, y)    (((x) < (y)) ? (y) : (x))
#define max(x, y)    (((x) < (y)) ? (y) : (x))
#define MIN(x, y)    (((x) < (y)) ? (x) : (y))
#define min(x, y)    (((x) < (y)) ? (x) : (y))
#define STREQ(s, t)  (strcmp(s, t) == 0)
#define STRGT(s, t)  (strcmp(s, t) > 0)
#define STRLT(s, t)  (strcmp(s, t) < 0)
#define STRNEQ(s, t, l) (strncmp(s, t, l) == 0)
#define STRNGT(s, t, l) (strncmp(s, t, l) > 0)
#define STRNLT(s, t, l) (strncmp(s, t, l) < 0)
#define SWAP(a,b,t)  ((t) = (a), (a) = (b), (b) = (t))
 
 
/*********************************************************************/
/*                                                                   */
/* Memory allocation debugging routines                              */
/*                                                                   */
/*********************************************************************/
 
#ifdef MEMDEBUG
 
void *mallocx(size_t, char *, int);
void *callocx(size_t, size_t, char *, int);
void *reallocx(void *, size_t, char *, int);
void freex(void *, char *, int);
 
#define malloct(x) mallocx((x), __FILE__, __LINE__)
#define calloct(x, y) callocx((x), (y), __FILE__, __LINE__)
#define realloct(x, y) reallocx((x), (y), __FILE__, __LINE__)
#define freet(x) freex((x), __FILE__, __LINE__)
 
#define malloc malloct
#define calloc calloct
#define realloc realloct
#define free freet
 
#endif
 
 
 
/*********************************************************************/
/*                                                                   */
/* General typedefs                                                  */
/*                                                                   */
/*********************************************************************/
 
typedef unsigned char byte;
 
typedef unsigned short int bool;
 
typedef unsigned short int Devaddr;
 
typedef struct diskaddr {
   int cylinder;
   int track;
   int record;
} Diskaddr;
 
 
typedef struct record_id {
 
   char id[8];
   time_t write_timestamp;
 
} Record_ID;
 
 
typedef struct location {
 
   union {
      unsigned bsn;
      char cs[9];
      unsigned short msn;
   } loc;
 
   unsigned location_is_bsn:1,
            location_is_cs:1,
            location_is_msn:1;
 
} Location;
 
 
 
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*                                                                   */
/* Functions specific defines, typedefs, and structures              */
/*                                                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
 
 
 
/*********************************************************************/
/*                                                                   */
/* Function prototype and structure(s) used in -                     */
/*                                                                   */
/* bldstree - Build station information tree                         */
/* delstree - Delete station information tree                        */
/* getstinf - Get station information from tree                      */
/*                                                                   */
/*********************************************************************/
 
typedef struct stn_info_node {
     int key;
     int block;
     int station;
     int latitude;
     int longitude;
     int elev;
     struct stn_info_node * right;
     struct stn_info_node * left;
} Stninfo;
 
struct stn_info_node *bldstree(void);
void delstree(struct stn_info_node *);
struct stn_info_node *getstinf(struct stn_info_node *,
                               int,
                               int);
 
 
 
/*********************************************************************/
/*                                                                   */
/* Function prototype and structure(s) used in -                     */
/*                                                                   */
/* capqread - Read bulletins from CAPQ chain                         */
/*                                                                   */
/*********************************************************************/
 
typedef struct CAPQ_data {
   char * bulletin;
   int bulletin_length;
   char * WMO_heading;
   char * AFOS_pil;
   char * current_CAPQ_end_address;
   int start_offset;
   int record_count;
   int end_offset;
   char * bulletin_address;
   int input_line;
   int receive_line;
   int receive_hour;
   int receive_minute;
   int CAPQ_day;
   int CAPQ_hour;
   int CAPQ_minute;
   int rc;
   char flag1;
   char flag2;
} CAPQdata;
 
struct CAPQ_data * capqread (char *, ...);
 
 
 
/*********************************************************************/
/*                                                                   */
/* Function prototype and structure(s) used in -                     */
/*                                                                   */
/* mdadread - Read bulletins from MDAD chain                         */
/*                                                                   */
/*********************************************************************/
 
typedef struct MDAD_data {
   char * bulletin;
   int bulletin_length;
   char * WMO_heading;
   char * AFOS_pil;
   char * current_MDAD_end_address;
   int start_offset;
   int record_count;
   int end_offset;
   char * bulletin_address;
   int input_line;
   int receive_line;
   int receive_hour;
   int receive_minute;
   int MDAD_year;
   int MDAD_month;
   int MDAD_day;
   int MDAD_hour;
   int MDAD_minute;
   int rc;
   int part_number;
   int number_of_parts;
   char MDAD_flag;
   char flag1;
   char flag2;
   char flag3;
   char MDAD_flag2;
} MDADdata;
 
MDADdata * mdadread (char *, ...);
MDADdata * mdupread (char *, ...);
MDADdata * mdadrd2 (char *, ...);
 
MDADdata * mdadscan (char *, ...);
MDADdata * mdupscan (char *, ...);
 
void mdadinpt ( MDADdata *, char, bool );
 
char * mdadnxtr ( void );
char * mdadnxtk ( void );
 
 
#define MDAD_HISTORY_LIMIT 39
 
typedef
struct MDAD_history_entry
{
   unsigned short hour;
   unsigned short minute;
   Diskaddr MDAD_chain_addr;
}
MDAD_History_Entry;
 
typedef
struct MDAD_history
{
   MDAD_History_Entry history_array[MDAD_HISTORY_LIMIT];
}
MDAD_History;
 
MDAD_History *mdadhist ( void );
 
 
/*********************************************************************/
/*                                                                   */
/* Function prototype and structure(s) used in -                     */
/*                                                                   */
/* gethdgi - Get bulletin heading information                        */
/*                                                                   */
/*********************************************************************/
 
typedef struct bltn_heading_info {
    int bltn_day;
    int bltn_hour;
    int bltn_min;
    int rtd_present;
    int cor_present;
    int amd_present;
    char * first_report;
    char TTAAii??(7??);
    char CCCC??(5??);
    char amd_seq;
    char cor_seq;
    char rtd_seq;
} Abbrevhdg;
 
Abbrevhdg *gethdgi(char * );
 
 
 
/*********************************************************************/
/*                                                                   */
/* Function prototype and structure(s) used in -                     */
/*                                                                   */
/* getime  - Get current system time                                 */
/* suspend - Delay execution until specified minute boundary         */
/*                                                                   */
/*********************************************************************/
 
 
typedef struct tm_struct{
   int hour;
   int min;
} Stime;
 
Stime *gettime(void);
int suspend(Stime *, int);
int timediff(Stime *, Stime *);
#define timecmp timediff
 
 
 
/*********************************************************************/
/*                                                                   */
/* Function prototype and structure(s) used in -                     */
/*                                                                   */
/* rdtaend - Specify rdtaread Ending Address                         */
/* rdtaread - Read From RGTR Data Tank                               */
/* rdtastrt - Specify rdtaread Starting Address                      */
/* rdtatend - Specify rdtaread End Time                              */
/* rdtatnke - Specify rdtaread Ending Address                        */
/* rdtarstr - Specify rdtaread Start Time                            */
/*                                                                   */
/*********************************************************************/
 
typedef struct rgtrdata {
   Diskaddr forward_chain;
   Diskaddr bulletin_addr;
   int receive_line;
   int receive_day;
   Stime receive_time;
   Stime RGTR_time;
   int length;
   char *bulletin;
   char datatype;
} RGTRdata;
 
int rdtaend(char, Diskaddr *);
int rdtaread(RGTRdata *);
int rdtastrt(char, Diskaddr *);
int rdtatend (char, Stime *);
int rdtatnke(char);
int rdtatstr(char, Stime *);
void rdtainit(void);
 
 
 
/*********************************************************************/
/*                                                                   */
/*  Typedefs and function prototypes for bulletin and report parsing */
/*  functions.                                                       */
/*                                                                   */
/*********************************************************************/
 
 
 
typedef struct rptNode {
   char *rptPtr;
   int rptLength;
   struct rptNode* next;
} RptNode;
 
 
typedef struct synpBltn {
   Abbrevhdg heading;
   short int day;
   short int hour;
   int reportCount;
   RptNode *rptList;
   bool valid;
} SynpBltn;
 
 
typedef struct shipBltn {
   Abbrevhdg heading;
   int reportCount;
   RptNode *rptList;
   bool valid;
} ShipBltn;
 
 
typedef struct tepiBltn {
   Abbrevhdg heading;
   int reportCount;
   RptNode *rptList;
   bool valid;
} TePiBltn;
 
 
typedef struct drftBltn {
   Abbrevhdg heading;
   int reportCount;
   RptNode *rptList;
   bool valid;
} DrftBltn;
 
 
typedef struct airpBltn {
   Abbrevhdg heading;
   int reportCount;
   RptNode *rptList;
   bool valid;
} AirpBltn;
 
 
typedef struct amdrBltn {
   Abbrevhdg heading;
   short int day;
   short int hour;
   int reportCount;
   RptNode *rptList;
   bool valid;
} AmdrBltn;
 
 
typedef struct bthyBltn {
   Abbrevhdg heading;
   int reportCount;
   RptNode *rptList;
   bool valid;
} BthyBltn;
 
 
typedef struct tescBltn {
   Abbrevhdg heading;
   int reportCount;
   RptNode *rptList;
   bool valid;
} TescBltn;
 
 
typedef struct tracBltn {
   Abbrevhdg heading;
   int reportCount;
   RptNode *rptList;
   bool valid;
} TracBltn;
 
 
typedef struct climBltn {
   Abbrevhdg heading;
   int reportCount;
   int month;
   int year;
   RptNode *rptList;
   bool valid;
} ClimBltn;
 
 
typedef struct clmtBltn {
   Abbrevhdg heading;
   int reportCount;
   int month;
   int year;
   RptNode *rptList;
   bool valid;
} ClmtBltn;
 
 
typedef struct metBltn {
   Abbrevhdg heading;
   int reportCount;
   RptNode *rptList;
   bool valid;
   short int day;              /* -1 indicates missing/invalid */
   short int hour;             /* -1 indicates missing/invalid */
   short int min;              /* -1 indicates missing/invalid */
} MetBltn;
 
 
typedef struct saoBltn {
   Abbrevhdg heading;
   int reportCount;
   RptNode *rptList;
   bool valid;
} SAOBltn;
 
 
typedef struct prBltn {
   Abbrevhdg heading;
   int reportCount;
   RptNode *rptList;
   bool valid;
} PRBltn;
 
 
typedef struct tafBltn {
   Abbrevhdg heading;
   int reportCount;
   RptNode *rptList;
   bool valid;
} TafBltn;
 
 
typedef struct arepBltn{
   Abbrevhdg heading;
   int reportCount;
   RptNode *rptList;
   bool valid;
}ArepBltn;
 
typedef struct metrRptP {
   char locind[4];
   int groupCount;
   short int day;             /* -1 indicates missing or invalid */
   short int hour;            /* -1 indicates missing or invalid */
   short int min;             /* -1 indicates missing or invalid */
   bool valid;
} MetrRptP;
 
 
typedef struct saoRptP {
   char locind[4];
   int groupCount;
   short int hour;            /* -1 indicates missing or invalid */
   short int min;             /* -1 indicates missing or invalid */
   bool valid;
} SAORptP;
 
 
typedef struct prRptP {
   char locind[4];
   int groupCount;
   short int hour;            /* -1 indicates missing or invalid */
   short int min;             /* -1 indicates missing or invalid */
   bool valid;
} PRRptP;
 
 
typedef struct tafRptP {
   char locind[4];
   int groupCount;
   short int YY;
   short int GG;
   short int validPeriod;
   bool ammendment;
   bool correction;
   bool valid;
} TafRptP;
 
 
typedef struct synpRptP {
   short int II;
   short int iii;
   int groupCount;
   bool valid;
} SynpRptP;
 
 
typedef struct climRptP {
   short int II;
   short int iii;
   int groupCount;
   bool valid;
} ClimRptP;
 
 
typedef struct clmtRptP {
   short int II;
   short int iii;
   int groupCount;
   bool valid;
} ClmtRptP;
 
 
typedef struct tepiRptP {
   short int II;
   short int iii;
   short int YY;
   short int GG;
   short int quad;
   short int ulatitude;
   short int ulongitude;
   int msquare;
   int latitude;
   int longitude;
   int groupCount;
   char callsign[15];
   char type;
   char part;
   bool valid;
} TePiRptP;
 
 
SynpBltn *pbsynp(char *);
ShipBltn *pbship(char *);
TePiBltn *pbtepi(char *);
DrftBltn *pbdrft(char *);
AirpBltn *pbairp(char *);
AmdrBltn *pbamdr(char *);
BthyBltn *pbbthy(char *);
TescBltn *pbtesc(char *);
TracBltn *pbtrac(char *);
ClimBltn *pbclim(char *);
ClmtBltn *pbclmt(char *);
MetBltn  *pbmetr(char *);
MetBltn  *pbspec(char *);
TafBltn  *pbtaf(char *);
TafBltn  *pbtaf2(char *);
TafBltn  *pbtaf3(char *);
TafBltn  *pbtaf4(char *);
SAOBltn  *pbsao(char *);
PRBltn   *pbpirep(char *);
ArepBltn *pbairep(char *);
 
SynpRptP *prpsynp(char *, int);
TePiRptP *prptepi(char *, int);
ClimRptP *prpclim(char *, int);
ClmtRptP *prpclmt(char *, int);
MetrRptP *prpmetr(char *, int);
TafRptP  *prptaf(char *, int);
TafRptP  *prptaf2(char *, int);
TafRptP  *prptaf3(char *, int);
TafRptP  *prptaf4(char *, int);
SAORptP  *prpsao(char *, int);
PRRptP   *prppirep(char *, int);
 
 
 
 
/*********************************************************************/
/*                                                                   */
/*  Structures and Function Prototypes for RRN physical I/O          */
/*                                                                   */
/*********************************************************************/
 
 
typedef struct RRN_device {
 
   char name[44],
        ownerid[8];
 
   unsigned short dev_addr,
                  base_cylinder,
                  base_track,
                  base_record,
                  max_cylinder,
                  max_track,
                  max_record,
                  records_per_track,
                  tracks_per_cylinder,
                  record_length;
 
} RRN_Device;
 
 
bool readrrn(char *device_name,
             unsigned int rrn,
             void *input_buffer,
             unsigned int read_count);
 
bool writerrn(char *device_name,
              unsigned int rrn,
              void *output_buffer,
              unsigned int write_count);
 
RRN_Device *devinfo(char *device_name);
 
bool valid_dn(char *device_name);
 
 
 
/*********************************************************************/
/*                                                                   */
/*  Function prototype for string value test functions.              */
/*                                                                   */
/*********************************************************************/
 
 
int sisalnum(char *);
int sisalpha(char *);
int siscntrl(char *);
int sisdigit(char *);
int sisgraph(char *);
int sislower(char *);
int sisprint(char *);
int sispunct(char *);
int sisspace(char *);
int sisupper(char *);
int sisxdigi(char *);
 
int nisalnum(char *, int);
int nisalpha(char *, int);
int niscntrl(char *, int);
int nisdigit(char *, int);
int nisgraph(char *, int);
int nislower(char *, int);
int nisprint(char *, int);
int nispunct(char *, int);
int nisspace(char *, int);
int nisupper(char *, int);
int nisxdigi(char *, int);
 
char *nxtalnum(char *);
char *nxtalpha(char *);
char *nxtcntrl(char *);
char *nxtdigit(char *);
char *nxtgraph(char *);
char *nxtlower(char *);
char *nxtprint(char *);
char *nxtpunct(char *);
char *nxtspace(char *);
char *nxtupper(char *);
char *nxtxdigi(char *);
 
char *lstalnum(char *, int);
char *lstalpha(char *, int);
char *lstcntrl(char *, int);
char *lstdigit(char *, int);
char *lstgraph(char *, int);
char *lstlower(char *, int);
char *lstprint(char *, int);
char *lstpunct(char *, int);
char *lstspace(char *, int);
char *lstupper(char *, int);
char *lstxdigi(char *, int);
 
 
/*********************************************************************/
/*                                                                   */
/*  Enumeration type and declaration for code form identification    */
/*  function                                                         */
/*                                                                   */
/*********************************************************************/
 
 
typedef
enum codeform {AIREP, AMDAR, ARFOR, ARMET, BATHY, CLIMAT, CLIMAT_SHIP,
               CLIMAT_TEMP, CLIMAT_TEMP_SHIP, CODAR, DRIFTER, FC,
               HYFOR, IAC, IAC_FLEET, ICEAN, METAR, PILOT, PILOT_MOBILE,
               PILOT_SHIP, RECCO, ROCOB, ROCOB_SHIP, ROFOR, SAO, PIREP,
               SATEM, SATOB, SHIP, SPECI, SYNOP, TAF, TEMP, TEMP_DROP,
               TEMP_MOBILE, TEMP_SHIP, TESAC, TRACKOB, WAVEOB,
               UNKNOWN_FORM, TEMP_A, TEMP_B, TEMP_C, TEMP_D,
               TEMP_DROP_A, TEMP_DROP_B, TEMP_DROP_C, TEMP_DROP_D,
               TEMP_MOBILE_A, TEMP_MOBILE_B, TEMP_MOBILE_C,
               TEMP_MOBILE_D, TEMP_SHIP_A, TEMP_SHIP_B, TEMP_SHIP_C,
               TEMP_SHIP_D, PILOT_A, PILOT_B, PILOT_C, PILOT_D,
               PILOT_MOBILE_A, PILOT_MOBILE_B, PILOT_MOBILE_C,
               PILOT_MOBILE_D, PILOT_SHIP_A, PILOT_SHIP_B,
               PILOT_SHIP_C, PILOT_SHIP_D }
CodeForm;
 
CodeForm idcode(char *);
 
char *codename(CodeForm);
CodeForm name2cf ( char * );
 
 
 
/********************************************************************/
/*                                                                  */
/*  Additional Bulletin and Report Parsing Structures and Routines  */
/*                                                                  */
/********************************************************************/
 
typedef struct mespBltn {
   Abbrevhdg heading;
   int reportCount;
   RptNode *rptList;
   CodeForm type;
   bool valid;
   short int day;              /* -1 indicates missing/invalid */
   short int hour;             /* -1 indicates missing/invalid */
   short int min;              /* -1 indicates missing/invalid */
} MeSpBltn;
 
 
typedef struct mespRptP {
   char locind[4];
   int groupCount;
   CodeForm type;
   char *rptStart;
   int rptLen;
   short int day;             /* -1 indicates missing or invalid */
   short int hour;            /* -1 indicates missing or invalid */
   short int min;             /* -1 indicates missing or invalid */
   bool valid;
} MeSpRptP;
 
 
MeSpBltn  *pbmesp(char *);
 
MeSpRptP *prpmesp(char *, int);
 
MeSpBltn  *tpbmesp(char *);
 
MeSpRptP *tprpmesp(char *, int);
 
 
 
/*********************************************************************/
/*                                                                   */
/*  String manipulation functions                                    */
/*                                                                   */
/*********************************************************************/
 
 
char *strnlf(char *, size_t);
char *strnmid(char *, size_t, size_t);
char *strnrt(char *, size_t);
char *strrstr(char *, char *);
char *strcentr(char *, size_t);
char *strdel(char *, char *, size_t);
char *strins(char *, char *, char *);
char *strljust(char * , size_t);
char *strltrim(char *, char *);
char *strmrplc(char *, char *, char *);
char *strocat(char *, char *);
char *strrpt(char *, char *, size_t);
char *strrjust(char *, size_t);
char *strrplc(char * , char *, char *);
char *strrtrim(char * , char *);
char *strtrim(char *, char *);
char *strvcat(char *, char *, ...);
 
 
 
/*********************************************************************/
/*                                                                   */
/*  Bulletin Generator declarations                                  */
/*                                                                   */
/*********************************************************************/
 
typedef bool (*ParseBltnFnPtr) ( char *bltn,
                                 char **rptPtr,
                                 char *bbbTypePtr,
                                 char **prefixPtr,
                                 short *YYPtr,
                                 short *GGPtr,
                                 char *bltnTypePtr,
                                 char **headingPtr );
 
void cbltngen ( ParseBltnFnPtr fnPtr,
                char *filename,
                Devaddr *historyDevice,
                Diskaddr *historyAddr,
                unsigned * bltnInCountPtr,
                unsigned * bltnOutCountPtr,
                unsigned * rptOutCountPtr );
 
void tbltngen ( ParseBltnFnPtr fnPtr,
                char *filename,
                Devaddr *historyDevice,
                Diskaddr *historyAddr,
                unsigned * bltnInCountPtr,
                unsigned * bltnOutCountPtr,
                unsigned * rptOutCountPtr );
 
typedef bool (*ParseBltnFnPtrX) ( char *bltn,
                                 char **rptPtr,
                                 char *bbbTypePtr,
                                 char **prefixPtr,
                                 short *YYPtr,
                                 short *GGPtr,
                                 short *ggPtr,
                                 char *bltnTypePtr,
                                 char **headingPtr );
 
void xbltngen ( ParseBltnFnPtrX fnPtr,
                char *filename,
                Devaddr *historyDevice,
                Diskaddr *historyAddr,
                unsigned * bltnInCountPtr,
                unsigned * bltnOutCountPtr,
                unsigned * rptOutCountPtr );
 
void dbltngen ( ParseBltnFnPtrX fnPtr,
                char *filename,
                Devaddr *historyDevice,
                Diskaddr *historyAddr,
                unsigned * bltnInCountPtr,
                unsigned * bltnOutCountPtr,
                unsigned * rptOutCountPtr );
 
typedef bool (*OParseBltnFnPtr) ( char *bltn,
                                  char **rptPtr,
                                  char *bbbTypePtr,
                                  char **prefixPtr,
                                  short *YYPtr,
                                  short *GGPtr,
                                  char *bltnTypePtr,
                                  char **headingPtr,
                                  char **ccccPtr );
 
void obltngen ( OParseBltnFnPtr fnPtr,
                char *filename,
                Devaddr *historyDevice,
                Diskaddr *historyAddr,
                unsigned * bltnInCountPtr,
                unsigned * bltnOutCountPtr,
                unsigned * rptOutCountPtr );
 
 
void pbltngen ( OParseBltnFnPtr fnPtr,
                char *filename,
                Devaddr *historyDevice,
                Diskaddr *historyAddr,
                unsigned * bltnInCountPtr,
                unsigned * bltnOutCountPtr,
                unsigned * rptOutCountPtr );
 
 
void sbltngen ( OParseBltnFnPtr fnPtr,
                char *filename,
                Devaddr *historyDevice,
                Diskaddr *historyAddr,
                unsigned * bltnInCountPtr,
                unsigned * bltnOutCountPtr,
                unsigned * rptOutCountPtr );
 
 
void ebltngen ( ParseBltnFnPtr fnPtr,
                char *filename,
                Devaddr *historyDevice,
                Diskaddr *historyAddr,
                unsigned * bltnInCountPtr,
                unsigned * bltnOutCountPtr,
                unsigned * rptOutCountPtr );
 
 
/*********************************************************************/
/*                                                                   */
/*  Typedefs and function prototypes for retrieving information from */
/*  switching directory.                                             */
/*                                                                   */
/*********************************************************************/
 
typedef struct sw_dir_info_rec {
  char wmo_header[11];
  char AFOS_pil[10];
  char multiple_line;
  short int line_num;
  short int recvd_line;
  char flag1;
  char flag2;
  char flag3;
  char class;
  short int domestic_cat_num;
  char afos_tmp;
  char ccb[2];
  char region_addr;
  short int output_line_count;
  unsigned short trans_line[128];
  char change_date[3];
  char dir_flags;
  Diskaddr history_file_addr;
  char birth_date[3];
} SwDirInfo;
 
 
SwDirInfo *rtswdir(char *, int);
SwDirInfo *rtpswdir(void);
SwDirInfo *rtnswdir(void);
 
 
 
 
 
/*********************************************************************/
/*                                                                   */
/*  General local functions                                          */
/*                                                                   */
/*********************************************************************/
 
 
int itoc(int, char *, int);
 
int antoi(char *, int);
 
float antof(char *, int);
 
void errmsg(char *, ...);
 
void logmsg(char *, ...);
 
void opermsg(char *, ...);
 
int lmsg(const char *, const char *, ...);
int emsg(const char *, const char *, ...);
int omsg(const char *, const char *, ...);
 
#pragma linkage(ASCTOEB, OS)
void ASCTOEB(char *, int);
 
#pragma linkage(EAXLATE, OS)
void EAXLATE(char *, int);
 
#pragma linkage(PASCTOEB, OS)
void PASCTOEB(char *, int);
 
char **bldhdarr(char *);
 
void dalchdar(char **);
 
#pragma linkage(CCAPREAD, OS)
void *CCAPREAD(char *, int);
 
#pragma linkage(CCAPWRIT, OS)
void CCAPWRIT(char *, char *, int);
 
#pragma linkage(PPTOI, OS)
int PPTOI(char);
 
char itopp(int);
 
int diffmin(int, int, int, int, int, int);
 
char incrseq(char);
 
void nextdate(int *, int *, int *);
 
void prevdate(int *, int *, int *);
 
void rdstaddr(char *, char *);
 
int wrenaddr(char *, char *);
 
int vfydigit (char *, int);
 
int readline(char * , int);
 
int prevjul(int, int);
 
int nextjul(int, int);
 
int fcomppos(fpos_t *, fpos_t *);
 
void lfprint(char *);
 
void flfprint(FILE *, char *);
 
void slfprint(char *, int, char *);
 
void flfnprnt(FILE *, char *, int);
 
void slfnprnt(char *, int, char *, int);
 
int strhash(char *);
 
void reverse(char *);
 
bool itoa(int, char *, int);
 
int getsnn(char * , int);
 
int fgetsnn(char *, int, FILE *);
 
int getreply(char *, char *, int);
 
bool strfit(char *, char *, size_t);
 
bool addrfrm3(char *, Diskaddr *);
 
bool addrfrm5(char *, Diskaddr *);
 
bool addrto3(Diskaddr *, char *);
 
bool addrto5(Diskaddr *, char *);
 
int addrcmp(Diskaddr *, Diskaddr *);
 
void incraddr(Diskaddr *, Diskaddr *, Diskaddr *);
void decraddr(Diskaddr *, Diskaddr *, Diskaddr *);
 
#pragma linkage(readrec, OS)
char *readrec(Diskaddr *, Devaddr *, int, void *);
 
#pragma linkage(writerec, OS)
int writerec(Diskaddr*, Devaddr *, int, void *);
 
char prhold(char *, ...);
 
void dump(char *, int);
 
void fdump(FILE *, char *, int);
 
void fwdump(FILE *, char *, int);
 
char toascii(char);
 
char *strtoas(char *);
 
char *strntoas(char *, int);
 
char toebcdic(char);
 
char *strtoeb(char *);
 
char *strntoeb(char *, int);
 
char *lfind(char *, char *, int, int, int(*)(char *, char *));
 
char *lsearch(char *, char *, int *, int, int(*)(char *, char *));
 
bool strcmpw(char *, char *);
 
int strccnt(char *, int);
 
int strnccnt(char *, int, size_t);
 
int pprt(FILE *, char *, char *, char *, char *, ...);
 
bool pprtbrk(FILE *, char *, char *, char *);
 
bool pprtend(FILE *, char *);
 
bool pprtinit(int, char, char *, char *, char *);
 
char *monthnam(int, char);
 
char *getrec(FILE *, int, char *);
 
bool jtog(int, int, int *, int *, int *);
 
bool gtoj(int, int, int, int *, int *);
 
bool ccap2std(char *, Devaddr *, Diskaddr *);
 
bool std2ccap(Devaddr *, Diskaddr *, char *);
 
char *strupr(char *);
char *strlwr(char *);
char *strdup(char *);
char *strndup(char *, int);
int strcmpi(char *, char *);
 
void *memccpy(void *, void *, int, unsigned);
 
char *rptstrip(char *);
char *rptstrp2(char *);
char *rptfmt(char *);
char *rptfmt2(char *);
char *rptfmti(char *, unsigned short int);
char *rptfmtni(char *, unsigned short int);
 
char *strnstr(char *, char *, size_t);
 
int stregion(int);
int ccregion(char *);
char *rgnname(int);
 
void *memrchr(const void *, int, size_t);
 
bool sysmonms(char *, char *, ...);
bool sysmoncl(char *);
 
short prevndx ( short max, short min, short current );
short nextndx ( short max, short min, short current );
 
time_t extrym ( unsigned day, unsigned hour, unsigned minute );
time_t extrymd ( unsigned hour, unsigned minute );
 
int cmptimet ( time_t t1, time_t t2 );
 
int tfprintf ( FILE *, const char *, ... );
 
bool purgelog ( char *filename, unsigned short delete_age );
 
time_t odbtime ( void );
 
int bltnpcnt ( char *, int );
void bltnpage ( char *, int, int );
 
void rot( char *, unsigned int );
void unrot( char *, unsigned int );
 
void encrypt( char *, char * );
void decrypt( char *, char * );
 
int HEXTOI( char *, int );
 
char **hdgxref( char * );
 
struct tm *zonetime( unsigned short, unsigned short, char );
 
int wordcnt( char * );
int wordcntn( char *, unsigned int );
 
char *word( char *, unsigned int );
char *wordn( char *, unsigned int, unsigned int );
 
char *crlfstrp( char * );
 
bool charcmp( char *, char * );
 
int linecnt( char * );
int linecntn( char *, unsigned int );
 
char *bltline( char *, unsigned int );
char *bltlinen( char *, unsigned int, unsigned int );
 
char *pttoline( char *, unsigned int );
char *pttoword( char *, unsigned int );
 
char *moblrgn(unsigned short,
              unsigned short,
              unsigned short );
 
char *nxtgroup( char * );
 
void prtmdade(struct MDAD_data *);
 
#endif
@@DRVMETAR.C
#include <metar h>
#ifdef SYNOPTIC
 
char *BldSynop( Decoded_METAR * , char * );
/*char *Sec0MeSm(Decoded_METAR *);*/
/*char *Sec1MeSm(Decoded_METAR *, char *);*/
/*char *Sec3MeSm(Decoded_METAR *, char *);*/
/*char *Sec5MeSm(Decoded_METAR *, char *);*/
#endif
 
void prtDMETR( Decoded_METAR *);
int DcdMETAR( char *, Decoded_METAR * );
 
#pragma page(1)
#pragma subtitle(" ")
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         dRVMETAR                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          28 Oct 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      DRVMETAR is a main routine that acts a driver    */
/*                 for testing the METAR Decoder function.          */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 DcdMETAR                                         */
/*                 prtDcdMetar                                      */
/*                 Sec0MTSm                                         */
/*                 Sec1MTSm                                         */
/*                                                                  */
/*  Input:         None                                             */
/*                                                                  */
/*  Output:        None                                             */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
main()
{
   static char *string[] =
  {
 
"KMKG 18022G29KT 3SM BR BKN018 BR 24/22 A2995 RMK A02 VIS 2",
 
 
"KPIT 132351Z 33013KT 4SM +TSRA BR BKN018CB OVC031 12/11 A2977 RMK "
"AO2 PK WND 31041/2305 WSHFT 2300 PRESSRR SLP090 FRQ LTGCGCC OHD "
"TS OHD MOV E CB OHD MOV E 8/3// P0051 60052 T01170111 10222 20122 "
"53030",
 
 
"KCAK 132351Z 28016G22KT 10SM BKN021 OVC030 11/09 A2981 RMK AO2 "
"TSE00RAE10 PRESRR SLP093 TS MOV NE CIG RGD 8/5// P0002 60066 "
"T01110094 10217 20111 51053",
 
"KBUF 132354Z 21007KT 3SM +TSRA BR FEW009 OVC 12/11 A2959 RMK "
"AO2 PRESFR SLP021 8/9// TS ALQDS MOV E OCNL LTGICCCCG P0031 "
"60073 T01170111 10233 20111 50000 0",
 
 
"KPIT 132356Z 32012G21KT 4SM TSRA BR BKN018CB OVC031 12/11 A2978 "
"RMK AO2 WSHFT 2338 PRESFR FRQ LTGCGCC OHD TS OHD MOV E CB OHD MOV "
"E P0001",
 
 
"KCAK 132358Z 28015G22KT 10SM BKN013 OVC023 11/10 A2982 RMK AO2",
 
"KBUF 140001Z 22008KT 3SM +TSRA BR BKN009 BKN013 OVC022 12/12 A2959 "
"RMK AO2 P0003",
 
 
 
 
 
"KRIL 031853Z AUTO 33008KT 10SM SCT022 BKN032 OVC060 07/01 A3004 "
"RMK AO2 SLP157 T00720006 TSNO",
 
 
"METAR KCLE 091657Z COR 35021KT 3SM -PL SHPL VV004 M03/M04 A2964 "
"RMK VIS S M1/4=",
 
"METAR KCLE 091657Z COR 35021KT 3SM -PE SHPE VV004 M03/M04 A2964 "
"RMK VIS S M1/4=",
 
"METAR KCLE 091657Z COR 35021KT 3SM -PE TSPL VV004 M03/M04 A2964 "
"RMK VIS S M1/4=",
 
"METAR KCLE 091657Z COR 35021KT 3SM -PL TSPE VV004 M03/M04 A2964 "
"RMK VIS S M1/4=",
 
 
"KMLB 200450Z 18006KT 7SM OVC100 23/22 A2986 RMK FQT LTGIG W-N",
 
"KMLB 200450Z 18006KT 7SM OVC100 23/22 A2986 RMK FQT LTGIG W-N=",
 
"KMLB 200450Z 18006KT 7SM OVC100 23/22 A2986 RMK FRQ LTGIC NW",
 
"KMLB 200450Z 18006KT 7SM OVC100 23/22 A2986 RMK FRQ LTGCC NW=",
 
 
"SPECI KEKO 151609Z 00000KT 5SM BR FEW003 SCT013 M04/M06 A3018 "
"RMK VIS N-NE M1/4",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/MM A2992",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB MM/M12 A2992",
 
 "METAR KLAX 281156Z AUTO VRB100G135KT 130V210 3 9999 "
 "R15L/0500FT R22L/2700FT R16/1200FT R34/1000V1600FT R01L/P6000FT FC "
 "+TS VCTS FEW/// SCT000 BKN050 SCT150 OVC250 3/M1 A2991 RMK "
 "TORNADO B13 DSNT NE A01 PK WND 18515/45 "
 "WSHFT 1350 FROPA TWR VIS 1 1/2 SFC VIS 1/4 VIS 1/4V1 1/4 "
 "VIS 2 1/2 RWY11 "
 "DVR/1000V1600FT "
 "SHRAB05E30SHSNB20E55 FZDZB1057E1059 CIG 1000V1500 CIG 020 RWY11 "
 "PRESFR PRESRR SLP013 FG FEW/// HZ SCT000 VIS NW 2 1/2 GR 3/4 "
 "VIRGA SE -XRAFG3 CIGE005 BKN014 V OVC "
 "FU BKN020 NOSPECI LAST 8/365 SNINCR 2/10 4/178 "
 "933125 98096 P0125 60225 70565 "
 "T00261015 10369 21026 "
 "404800360 52101 VISNO RWY05 CHINO RWY27 PNO RVRNO "
 "PWINO FZRANO TSNO $",
 
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK SLP046 ESTMD SLP VIS SW-NW 2 "
 "PWINO FZRANO TSNO $",
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK SLP046 ESTMD SLP VIS SW-NW 2 1/2 "
 "PWINO FZRANO TSNO $",
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK SLP046 ESTMD SLP VIS SW-NW 2",
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK SLP046 ESTMD SLP VIS SW-NW 2 1/2=",
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK SLP046 ESTMD SLP VIS SW-NW 2",
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK CIG 003V026 SLP046 ESTMD SLP VIS SW-NW 2",
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK VIS S 2",
 
"SPECI KEKO 151609Z 00000KT 5SM BR FEW003 SCT013 M04/M06 A3018 "
"RMK VIS N-NE 1",
 
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/MM A2992",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB MM/M12 A2992",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB M18/MM A2992",
 
 
 
 
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK CIG 003V026 SLP046 ESTMD SLP VIS SW-NW 2=",
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK VIS S 2=",
 
"SPECI KEKO 151609Z 00000KT 5SM BR FEW003 SCT013 M04/M06 A3018 "
"RMK VIS N-NE 1=",
 
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK SLP046 ESTMD SLP VIS SW-NW 2",
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK CIG 003V026 SLP046 ESTMD SLP VIS SW-NW 2",
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK VIS S 2",
 
"SPECI KEKO 151609Z 00000KT 5SM BR FEW003 SCT013 M04/M06 A3018 "
"RMK VIS N-NE 1",
 
 
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK SLP046 ESTMD SLP VIS SW 2",
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK CIG 003V026 SLP046 ESTMD SLP VIS NW 2",
 
"METAR KAFF 091657Z COR 35021KT 3SM -SG BR VV004 M03/M04 A2964 "
"RMK VIS S 2",
 
"SPECI KEKO 151609Z 00000KT 5SM BR FEW003 SCT013 M04/M06 A3018 "
"RMK VIS NE 1",
 
 
 "KPIT 1935Z 22015G25KT 1/8SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/12 A2992",
 
 "KPIT 1935Z 22015G25KT 6SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB M12/M18 A2992",
 
 "KPIT 1935Z 22015G25KT 8SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB M18/12 A2992",
 
 "KPIT 1935Z 22015G25KT 9SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/M01 A2992",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005TCU BKN010ACSL OVC250CB MM/12 A2992",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/MM A2992",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB MM/M12 A2992",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB M18/MM A2992",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB MM/MM A2992",
 
  "SPECI KGFI 041430Z 18045G56KT M1/4SM R15/0200FT FC +TS VV010 20/18 "
  "A2900 RMK A02A PK WND 18056/28 OCNL LTG AP "
  "RAB15E25TSB20 FCB1430 PRESFR "
  "SLP 701 P 0254 T01990182",
 
 
 "METAR KLAX 281156Z AUTO VRB100G135KT 130V210 3 9999 "
 "R15L/0500FT R22L/2700FT R16/1200FT R34/1000V1600FT R01L/P6000FT FC "
 "+TS VCTS FEW/// SCT000 BKN050 SCT150 OVC250 3/M1 A2991 RMK "
 "TORNADO B13 DSNT NE A01 PK WND 18515/45 "
 "WSHFT 1350 FROPA TWR VIS 1 1/2 SFC VIS 1/4 VIS 1/4V1 1/4 "
 "VIS 2 1/2 RWY11 "
 "DVR/1000V1600FT "
 "SHRAB05E30SHSNB20E55 FZDZB1057E1059 CIG 1000V1500 CIG 020 RWY11 "
 "PRESFR PRESRR SLP013 FG FEW/// HZ SCT000 VIS NW 2 1/2 GR 3/4 "
 "VIRGA SE -XRAFG3 CIGE005 BKN014 V OVC "
 "FU BKN020 NOSPECI LAST 8/365 SNINCR 2/10 4/178 "
 "933125 98096 P0125 60225 70565 "
 "T00261015 10369 21026 "
 "404800360 52101 VISNO RWY05 CHINO RWY27 PNO RVRNO "
 "PWINO FZRANO TSNO $",
 
 "KP88 1919Z 09001KT 14/03 RMK AO / PKWND 002/RNO 158 Z T01440034",
 
 "K40B 1924Z 29004KT 15/M07 RMK AO PKWND 011/RM MV263 T01501072",
 
 
 
 "SPECI KGFI 041430Z COR 18045G56KT  "
 "M1/4SM R15/0200FT R01L/0600V1000FT R01L/M0600FT R27/P6000FT "
 "+FC +TS -FZDZ VV010 04/M02 "
 "A2900 RMK TORNADO B13 6 NE A02A PK WND 18056/28 WSHFT 30 FROPA "
 "TWR VIS 1 1/2 VIS NE 2 1/2 VIS 2 1/2 RWY11 DVR/0600V1000FT "
 "OCNL LTGICCG OVHD RAB15E25 TSB20 FCB1430 TS SE MOV NE GR 1 3/4 "
 "VIRGA SW CIG 005V010 FG SCT000 BKN014 V OVC CB DSNT W "
 "CIG 002 RWY11 PRESFR PRESRR "
 "SLP701 ACFT MSHP NOSPECI SNINCR 2/10 FIRST "
 "P0254 60217 70125 4/021 933036 8/903 98096 T00261015 "
 "11021 21001 401120084 52032RVRNO PWINO PNO FZRANO TSNO "
 "VISNO RWY06 CHINO RWY12 $",
 
 
 
"KPHX 281156Z 12004KT 16KM CLR 15/05 A2996 RMK AOA SLP135 T01500050 "
"10250 20150 53006",
"KFCA 281156Z 30003KT 10SM CLR 06/02 A3009 RMRK AO TNO $ SLP191 "
"T00610023 10167 20056 53003",
"KAST 281156Z 00000KT 10SM BKN095 09/08 A2997 REMARK AOA SLP150 "
"T00940084 10161 20094 52005 ",
"KHVR 281156Z 03003KT 10SM OVC020 09/07 A3010 REMARKS AO TNO ZRNO "
"$ SLP194 T00940073 10156 20089 51005",
 
"KGGW 281156Z 35006KT 5SM BR OVC010 10/09 A3003 RMK AOA $ SLP177 "
"70003 T01000095 10156 20110 53008",
"KELY 1153Z AUTO 14004KT 10SM SCT075 01/M01 A3011 RMK AOA TNO ZRNO "
"SLP171 70001 T00061011 10139 21006 51005",
"KFLG 281156Z 29006KT 10SM CLR 04/M01 A3012 RMK AO TNO SLP147 "
"T00391011 21006 51004",
"KGTF 281156Z 27005KT 7SM BKN080 04/04 A3010 RMK AOA SLP205 "
"T00440045 10117 20039 51006",
"KHLN 281156Z AUTO 27005KT 10SM OVC023 07/05 A3011 RMK AOA OVC V "
"BKN $ SLP202 60000 70001 T00670050 10122 20061 53003",
 
"K13A 1918Z 20011KT 26/M06 RMK AO PKWND 020/RNO 644V264 T02611061",
 
 
   "KGGW 1756Z 33018KT 10SM OVC015 M03/M06 A3041 RMK AOA SLP338 "
   "4/007 60002 T10281055 11028 21072 51009",
   "KPHX 1756Z 130004KT 10SM CLR 18/M03 A3001 RMK AOA SLP154 "
   "T01781033 10178 20067 58007",
   "KFCA 1756Z 29005KT 10SM CLR 05/M11 A3049 RMK AOA TNO SLP352 "
   "T00501111 10050 21044 50004",
   "KAST 1756Z 01006KT 10SM CLR 11/04 A3047 RMK AOA SLP316 "
   "T01110045 10111 20000 50002",
   "KHVR 1756Z 31007KT 5SM -SN SCT011 BKN024 OVC030 M05/M08 A3056 "
   "RMK AOA 933004 "
   "BKN V SCT TNO P0000 $ SLP389 4/015 60002 "
   "T10501077 11050 21078 51010",
   "KELY 1753Z 34010KT 10SM CLR 01/M07 A3022 RMK AOA TNO FZRNO "
   "SLP240 T00111066 10011 21078 58007",
   "KFLG 1756Z 07006KT 10SM CLR 06/M12 A3009 RMK AO TNO FZRNO "
   "SLP178 T00561122 10061 21100 58005",
   "KGTF 1756Z 35010KT 1/2SM -SN FG VV09 M06/M08 A3051 RMK AOA "
   "933004 SFC VSBY 3/4 "
   "P0009 SLP393 60010 T10611077 11044 21067 53013",
   "KHLN 1756Z 35012KT 10SM SCT032 OVC060 M02/M09 A3048 RMK AOA "
   "SLP369 60000 T10171094 11017 21061 53006",
   "KAST 1756Z 01006KT 10SM CLR 11/04 A3047 RMK AOA SLP316 61104 "
   "71235 T01110045 10111 20000 401720056 58002",
  "METAR KLAX 04281156Z AUTO VRB100G135KT 130V210 3 1/2SM "
  "R15L/0500FT R22L/2700FT R16/1200FT R34/1000V1600FT R01L/P6000FT FC "
  "+TS BLPY FEW000 BKN050 SCT150 OVC250 3/M1 A2991 RMK "
  "TORNADO B13 DSNT NE A02 PK WND 18515/45 "
  "WSHFT 1350 FROPA TWR VIS 1 1/2 SFC VIS 1/4 VIS 1/4V1 1/4 "
  "VIS 2 1/2 RWY11 OCNL LTG VCY STN "
  "RAB1030E1145 FZDZE56 BLPYE57 CIG 1000V1500 CIG 020 RWY11 "
  "PRESFR PRESRR SLP013 FG FEW000 VIS NW2 1/2 GR 3/4 "
  "VIRGA SE -XRAFG3 CIGE005 BKN014 V OVC "
  "FU BKN020 NOSPECI LAST 8/365 SNINCR 2/10 4/178 "
  "933125 98096 P0125 60225 70565 "
  "T00261015 10369 21026 "
  "404800360 52101 PWINO FZRANO TSNO $",
 
 
 
  "METAR KGFI 041356Z AUTO 17012KT 130V210 3 1/2SM R15L/0500FT -RA "
  "SCT050 OVC110 26/18 A2991 RMK FUNNEL CLOUDS A02 RAB30 "
  "SLP 101 GR M1/4 VIRGA SCT V BKN P 0010 T02640178",
 
  "METAR KGFI 041356Z AUTO 05008KT 10SM R15L/P6000FT CLR A2991 "
  "RMK WATERSPOUTS VCY STN NW A02 SLP 101 10288 20243 52021 $ ",
 
  "SPECI KGFI 041420Z AUTO 18030KT 3 1/2SM RVRNO TS -RA BKN008 OVC060 "
  "26/22 A2991 RMK A02 RA15TSB20 PRESFR SLP 101 P 0000 T02640218",
 
  "KABE 281900Z NIL",
 
  "METAR KPIT NIL",
 
  "METAR KCLE 04281156Z 170100G135KT 110V180  M1/4SM "
  "R01L/P6000FT +TSSHRA VCFG "
  "BKN025 SCT100 OVC200 M26/ A2991 RMK PK WND 18515/45 A02 "
  "WSHFT 1350 TWR VIS 1 1/2 SFC VIS 1/4 VIS 1/4V1 1/4 "
  "CIG 1000V1500 PRESFR FRQ LTG CG NW "
  "RAB1030E1145 FZDZE56 PRESRR SLP135 GS "
  "T1263 "
  "VIRGA NW 8/365 4/178 P 0125 60225 7//// 70565 10369 21026 "
  "404800360 52101 PWINO FZRANO TSNO $",
 
  "METAR KPHL 040256Z AUTO 170100G135KT 130V210  1/2SM "
  "R15L/0500FT R22L/2700FT R16/1200FT R34/1000V1600FT R01L/P6000FT "
  "FC +TS BKN050 SCT150 OVC250 M26/ A2991 RMK A02 PK WND 185150/1345 "
  "WSHFT 1350 TWR VIS 1 1/2 SFC VIS 1/4 VIS 1/4V1 1/4 LTG DSNT "
  "RAB1030E1145 FZDZE56 CIG 1000V1500 PRESFR PRESRR SLP037 GR 2 3/4 "
  "VIRGA E 8/365 4/178 P 0125 70565 21026 T0263 10369 60225 "
  "404800360 52101 PWINO FZRANO TSNO $",
 
  "SPECI KGFI 041420Z AUTO 18030KT 2 1/2SM RVRNO TS -RA BKN008 "
  "OVC060 25/22 A2991 RMK A02 LTG DSNT W "
  "RAB15TSB20 PRESFR SLP101 P 0000 "
  "254/218",
 
  "METAR KGFI 041356Z AUTO 170100G135KT 130V210 3 1/2SM "
  "R15L/0500FT R22L/2700FT R16/1200FT R34/1000V1600FT R01L/P6000FT "
  "FC +TS BKN050 SCT150 OVC250 M26/ A2991 RMK A02 PK WND 185150/1345 "
  "WSHFT 1350 TWR VIS 1 1/2 SFC VIS 1/4 VIS 1/4V1 1/4 "
  "RAB1030E1145 FZDZE56 CIG 1000V1500 PRESFR PRESRR SLP997 GR M1/4 "
  "VIRGA SE 8/365 4/178 P 0125 6//// 60225 70565 T0263 10369 21026 "
  "404800360 52101 PWINO FZRANO TSNO $",
 
 
 
  "METAR KGFI 041356Z AUTO 170100G135KT 130V210 3 1/2SM "
  "R15L/0500FT R22L/2700FT R16/1200FT R34/1000V1600FT R01L/P6000FT "
  "FC +TS BKN050 SCT150 OVC250 M26/ A2991 RMK A02 PK WND 185150/1345 "
  "WSHFT 1350 TWR VIS 1 1/2 SFC VIS 1/4 VIS 1/4V1 1/4 "
  "RAB1030E1145 FZDZE56 CIG 1000V1500 PRESFR PRESRR SLP997 GR 25 "
  "VIRGA 35 8/365 4/178 P 0125 6//// 60225 70565 T0263 10369 21026 "
  "VIRGA 35 8/365 4/178 P 0125 21026 70565 10369 60225 T0263 21026 "
  "404800360 52101 PWINO FZRANO TSNO $",
 
 
  "METAR KGFI 041356Z AUTO 170100G135KT 130V210 3 1/2SM "
  "R15L/0500FT R22L/2700FT R16/1200FT R34/1000V1600FT R01L/P6000FT "
  "FC +TS BKN050 SCT150 OVC250 3/M1 A2991 RMK A02 PK WND 18515/45 "
  "WSHFT 1350 TWR VIS 1 1/2 SFC VIS 1/4 VIS 1/4V1 1/4 "
  "RAB1030E1145 FZDZE56 CIG 1000V1500 PRESFR PRESRR SLP997 GR 25 "
  "VIRGA 35 8/365 4/178 P 0125 60225 70565 T00261015 10369 21026 "
  "404800360 52101 PWINO FZRANO TSNO $",
 
  "METAR KGFI 041356Z AUTO 170100G135KT 130V210 3 1/2SM "
  "R15L/0500FT R22L/2700FT R16/1200FT R34/1000V1600FT R01L/P6000FT "
  "FC +TS BKN050 SCT150 OVC250 3/M1 A2991 RMK A02 PK WND 185150/1345 "
  "WSHFT 1350 TWR VIS 1 1/2 SFC VIS 1/4 VIS 1/4V1 1/4 "
  "RAB1030E1145 FZDZE56 CIG 1000V1500 PRESFR PRESRR SLP997 GR 25 "
  "VIRGA 35 8/365 4/178 P 0125 60225 70565 T00261015 10369 21026 "
  "404800360 52101 PWINO FZRANO TSNO",
 
 
 
  "METAR KGFI 041356Z AUTO 05008KT 10SM R15L/P6000FT CLR A2991 RMK "
  "A02 SLP 101 10288 20243 52021",
  "SPECI DGFI 041430Z 18045G56KT M1/4SM R15/0200FT FC +TS VV010 20/18 "
  "M20/M18 A2900 RMK A02A PK WND 18056/28 RAB15E25TSB20 FCB1430 PRESFR "
  "SLP 701 P 0254 M199/M182",
 
  "SPECI DGFI 041430Z 18045G56KT M1/4SM R15/0200FT FC +TS VV010 20/18 "
  "M20/M18 A2900 RMK A02A PK WND 18056/28 RAB15E25TSB20 FCB1430 PRESFR "
  "SLP 701 P 0254 M199/182",
 
  "SPECI DGFI 041430Z 18045G56KT M1/4SM R15/0200FT FC +TS VV010 20/18 "
  "M20/M18 A2900 RMK A02A PK WND 18056/28 RAB15E25TSB20 FCB1430 PRESFR "
  "SLP 701 P 0254 199/M182",
 
 "METAR APIT 171755Z AUTO 22015G25KT 1 3/4SM R22L/2700FT R16/1200FT "
 "R34/1000V1600FT R01L/P6000FT R04RR/900FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/16 A2992 "
 "RMK 4/369 58033 6003/ TWELVE 70125 10039 20029 410840112 "
 "PCPN 0009 8/563 WSHFT 1715 PK WND 2032/1725 "
 "CIG 20V25 WND 12V25 TWR VIS 2 1/2 "
 "SFC VIS 1 1/2 VIS 1 1/2V2 SLP875 SGB1213E1225",
 
 "NZWN 1700Z 35030G49KT 320V030 20KM 02 5SC021 7SC046 12/08 "
 "          Q0994.2 TEMPO 6000 RA 5ST012 2CB015 RMK SLP056 "
 "RAE0123",
 
 "SPECI APIT 171755Z 22015G25KT 1 3/4SM R22L/2700FT R16/1200FT "
 "R34/1000V1600FT R01L/P6000FT R04RR/900FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/16 A2992 "
 "RMK 58033 6003/ TWELVE 70125 8/321 10039 20029 410840112 "
 "PCPN 0009 WSHFT 1715 PK WND 2032/1725 "
 "CIG 20V25 WND 12V25 TWR VIS 2 1/2 "
 "SFC VIS 1 1/2 VIS 1 SLP875 FGB1713",
 
 "APIT 1755Z 22015G25KT 1 3/4SM R22L/2700FT R16/1200FT "
 "R34/1000V1600FT R01L/P6000FT R04RR/900FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/16 A2992 "
 "RMK 58033 6003/ TWELVE 70125 10039 20029 410840112 "
 "PCPN 0009 WSHFT 1715 PK WND 2032/1725 "
 "CIG 20V25 WND 12V25 TWR VIS 2 1/2 "
 "SFC VIS 1 1/2 VIS 1V2 SLP875",
 
 
 "APIT 1755Z 22015G25KT 1 3/4SM R22L/2700FT R16/1200FT "
 "R34/1000V1600FT R01L/P6000FT R04RR/900FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/16 A2992 "
 "RMK 58033 6003/ TWELVE 70125 10039 20029 410840112 "
 "PCPN 0009 WSHFT 1715 PK WND 2032/1725 "
 "CIG 20V25 WND 12V25 TWR VIS 2 1/2 "
 "SFC VIS 1 1/2 VIS 1 1/2V2 1/2 SLP875",
 
 "APIT 1755Z 22015G25KT 1 3/4SM R22L/2700FT R16/1200FT "
 "R34/1000V1600FT R01L/P6000FT R04RR/900FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/16 A2992 "
 "RMK 58033 6003/ TWELVE 70125 10039 20029 410840112 "
 "PCPN 0009 WSHFT 1715 PK WND 2032/1725 "
 "CIG 20V25 WND 12V25 TWR VIS 2 1/2 "
 "SFC VIS 1 1/2 VIS 1V2 1/2 SLP875",
 
 
 "EGPF 1720Z 00000KT 9999 -SHRA STC014 SCT020CB BNK024 12/09 "
 "Q1003 NOSIG",
 
 "NZAA 1700Z 03010KT 30KM 03 5CU022 7SC035 11/07 Q1006.5 NOSIG",
 "NZWN 1700Z 35030G49KT 320V030 20KM 02 5SC021 7SC046 12/08 "
 "     Q0994.2 TEMPO 6000 RA 5ST012 2CB015 RMK KAUKAU 30050KT",
 "DGAA 1800Z 22012KT 9999 SCT009 BKN120 25/21 Q1015",
 "DAAT 1830Z 30010KT CAVOK 29/06 Q1019",
 
 "GQPP 1800Z 34023KT 3000 DRSA SKC 24/20 Q1011 NSG",
 "DAAG 1830Z 06006KT 9999 SCT020 25/22 Q1015",
 "DABB 1830Z 04010KT 9999 SCT030TCU SCT033CB 27/18 Q1017",
 "DABC 1830Z 00000KT 9999 SCT026TCU SCT036CB 22/18 Q1020 RETS",
 
 "NZAA 1700Z 03010KT 30KM 03 5CU022 7SC035 11/07 Q1006.5 NOSIG",
 "NZWN 1700Z 35030G49KT 320V030 20KM 02 5SC021 7SC046 12/08 "
 "          Q0994.2 TEMPO 6000 RA 5ST012 2CB015 RMK K",
 "NZWN 1700Z 35030G49KT 320V030 20KM 02 5SC021 7SC046 12/08 "
 "     Q0994.2 TEMPO 6000 RA 5ST012 2CB015 RMK KAUKAU 30050KT",
 "DGAA 1800Z 22012KT 9999 SCT009 BKN120 25/21 Q1015",
 
 "GFLL 1900Z NIL",
 
 "GOOY 1800Z 03006G17KT 340V080 6000 TSRA BKN016 BKN030CB "
 "BKN133 26/23 Q1013 NOSIG",
 
 "GCXO 1930Z 32018KT 8000 SCT003 SCT007 18/16 Q1019",
 
 "APIT 1755Z 22015G25KT 1 3/4SM R22L/2700FT R16/1200FT "
 "R34/1000V1600FT R01L/P6000FT R04RR/900FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/16 A2992 "
 "RMK 58033 6003/ TWELVE 70125 10039 20029 410840112 "
 "PCPN 0009 WSHFT 1715 PK WND 2032/1725 "
 "CIG 20V25 WND 12V25 TWR VIS 2 1/2 "
 "SFC VIS 1 1/2 VIS 1 1/2V2",
 
 "BPIT 1755Z 22015G25KT 1 3/4SM R22L/2700FT R16/1200FT "
 "R34/1000V1600FT R01L/P6000FT R04RR/900FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/16 A2992 "
 "RMK 58033 6003/ TWELVE 70125 10039 20029 410840112 "
 "PCPN 0009 WSHFT 1715 PK WND 2032/1725 "
 "CIG 20V25 WND 12V25 TWR VIS 2 1/2 "
 "SFC VIS 1 1/2 VIS 1V2",
 
 "CPIT 1755Z 22015G25KT 1 3/4SM R22L/2700FT R16/1200FT "
 "R34/1000V1600FT R01L/P6000FT R04RR/900FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/16 A2992 "
 "RMK 58033 6003/ TWELVE 70125 10039 20029 410840112 "
 "PCPN 0009 WSHFT 1715 PK WND 2032/1725 "
 "CIG 20V25 WND 12V25 TWR VIS 2 1/2 "
 "SFC VIS 1 1/2 VIS 1V2 1/2",
 
 "DPIT 1755Z 22015G25KT 1 3/4SM R22L/2700FT R16/1200FT "
 "R34/1000V1600FT R01L/P6000FT R04RR/900FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/16 A2992 "
 "RMK 58033 6003/ TWELVE 70125 10039 20029 410840112 "
 "PCPN 0009 WSHFT 1715 PK WND 2032/1725 "
 "CIG 20V25 WND 12V25 TWR VIS 2 1/2 "
 "SFC VIS 1 1/2 VIS 1 1/2V2 1/2",
 
 "EPIT 1755Z 22015G25KT 1 3/4SM R22L/2700FT R16/1200FT "
 "R34/1000V1600FT R01L/P6000FT R04RR/900FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/16 A2992 "
 "RMK 58033 6003/ TWELVE 70125 10039 20029 410840112 "
 "PCPN 0009 WSHFT 1715 PK WND 2032/1725 "
 "CIG 20V25 WND 12V25 TWR VIS 2 1/2 "
 "SFC VIS 1 1/2 VIS 1/2V3/4",
 
 "FPIT 1755Z 22015G25KT 1 3/4SM R22L/2700FT R16/1200FT "
 "R34/1000V1600FT R01L/P6000FT R04RR/900FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/16 A2992 "
 "RMK 58033 6003/ TWELVE 70125 10039 20029 410840112 "
 "PCPN 0009 WSHFT 1715 PK WND 2032/1725 "
 "CIG 20V25 WND 12V25 TWR VIS 2 1/2 "
 "SFC VIS 1 1/2 VIS 3/4V2 1/2",
 
 "GPIT 1755Z 22015G25KT 1 3/4SM R22L/2700FT R16/1200FT "
 "R34/1000V1600FT R01L/P6000FT R04RR/900FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/16 A2992 "
 "RMK 58033 6003/ TWELVE 70125 10039 20029 410840112 "
 "PCPN 0009 WSHFT 1715 PK WND 2032/1725 "
 "CIG 20V25 WND 12V25 TWR VIS 2 1/2 "
 "SFC VIS 1 1/2 VIS 3/4V3",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB M18/M16 A2992",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB M18/16 A2992",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB 18/M16 A2992",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB MM/M16 A2992",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB MM/16 A2992",
 
 "KPIT 1935Z 22015G25KT 1 3/4SM R22L/2700FT "
 "TSRA -DZ FG +SNPE SCT005 BKN010 OVC250CB M18/MM A2992",
 
 
        NULL};
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   Decoded_METAR Metar;
   Decoded_METAR *Mptr = &Metar;
   int j,
       ErReturn;
   static char *synopRTRN = NULL;
   char bltn_prefix[20];
 
   /***************************************************/
   /* START BODY OF MAIN ROUTINE FOR CALLING DcdMETAR */
   /***************************************************/
 
   j = 0;
 
   while( string[j] != NULL)
   {
            /*-- PRINT INPUT METAR REPORT ----------------------------*/
      printf("\n\nINPUT METAR REPORT:\n\n %s\n\n",string[j] );
 
            /*-- DECODE INPUT REPORT ---------------------------------*/
      if ( (ErReturn = DcdMETAR( string[ j ], Mptr )) != 0 )
         printf("DcdMETAR:  Error Return Number: %d\n",ErReturn);
 
            /*-- PRINT DECODED METAR REPORT ELEMENTS -----------------*/
      printf("\n\nFINAL DECODED PRODUCT...\n\n");
      prtDMETR( Mptr );
#ifdef OLDSTUFF
/************************************************/
/*  Convert Decoded METAR into Synoptic format  */
/************************************************/
 
      printf("Just after call to prtDMETR\n");
 
               sprintf( bltn_prefix, "AAXX YYGGi##," );
               synopRTRN = BldSynop( Mptr, bltn_prefix );
            printf("After BldSynop, SynopRep =:\n%s\n",synopRTRN);
            /**********************************************************/
            /*-- ENCODE SECTION 0 OF THE SYNTHETIC SYNOPTIC REPORT ---*/
            /**********************************************************/
      printf("Just before call to Sec0MeSM\n");
 
      if( (synopRTRN = Sec0MeSm( Mptr )) == NULL )
         printf("Sec0MeSm returned a NULL pointer\n");
      else
         printf("After Sec0MeSm: %s\n",synopRTRN);
 
            /**********************************************************/
            /*-- ENCODE SECTION 1 OF THE SYNTHETIC SYNOPTIC REPORT ---*/
            /**********************************************************/
      if( synopRTRN != NULL )
         synopRTRN = Sec1MeSm( Mptr,synopRTRN );
            printf("After Sec1MeSm: %s\n",synopRTRN);
 
            /**********************************************************/
            /*-- ENCODE SECTION 3 OF THE SYNTHETIC SYNOPTIC REPORT ---*/
            /**********************************************************/
 
      if( synopRTRN != NULL )
         synopRTRN = Sec3MeSm( Mptr,synopRTRN );
            printf("After Sec3MeSm: %s\n",synopRTRN);
 
            /**********************************************************/
            /*-- ENCODE SECTION 5 OF THE SYNTHETIC SYNOPTIC REPORT ---*/
            /**********************************************************/
 
      if( synopRTRN != NULL )
         synopRTRN = Sec5MeSm( Mptr,synopRTRN);
            printf("After Sec5MeSm: %s\n",synopRTRN);
 
            /**********************************************************/
            /*-- PRINT THE ENCODED SYNTHETIC SYNOPTIC REPORT ---------*/
            /**********************************************************/
 
      if( synopRTRN != NULL ) {
         printf("\n\nOutput Synoptic Report:  %s\n\n",synopRTRN);
         free( synopRTRN);
      }
 
#endif
 
      j++;
 
   }
 
}
@@DCDMETAR.C
#pragma comment (compiler)
#pragma comment (date)
#pragma comment (timestamp)
#pragma pagesize(80)
 
 
#include <metar h>     /* standard header file */
 
float fracPart( char * );
void DcdMTRmk( char **, Decoded_METAR * );
void prtDMETR( Decoded_METAR * );
 
#pragma page(1)
#pragma subtitle(" ")
#pragma subtitle("subtitle - Decode METAR report.              ")
/********************************************************************/
/*                                                                  */
/*  Title:         SaveTokenString                                  */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          14 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      SaveTokenString tokenizes the input character    */
/*                 string based upon the delimeter set supplied     */
/*                 by the calling routine.  The elements tokenized  */
/*                 from the input character string are saved in an  */
/*                 array of pointers to characters.  The address of */
/*                 this array is the output from this function.     */
/*                                                                  */
/*  Input:         string - a pointer to a character string.        */
/*                                                                  */
/*                 delimeters - a pointer to a string of 1 or more  */
/*                              characters that are used for token- */
/*                              izing the input character string.   */
/*                                                                  */
/*  Output:        token  - the address of a pointer to an array of */
/*                          pointers to character strings.  The     */
/*                          array of pointers are the addresses of  */
/*                          the character strings that are token-   */
/*                          ized from the input character string.   */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static char **SaveTokenString ( char *string , char *delimeters )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   int NDEX;
   static char *token[ MAXTOKENS ],
        *TOKEN;
 
 
   /*********************************/
   /* BEGIN THE BODY OF THE ROUTINE */
   /*********************************/
 
   /******************************************/
   /* TOKENIZE THE INPUT CHARACTER STRING    */
   /* AND SAVE THE TOKENS TO THE token ARRAY */
   /******************************************/
 
   NDEX = 0;
   TOKEN = strtok( string, delimeters);
 
   if( TOKEN == NULL )
      return NULL;
 
   token[NDEX] = (char *) malloc(sizeof(char)*(strlen(TOKEN)+1));
   strcpy( token[ NDEX ], TOKEN );
 
 
   while ( token[NDEX] != NULL )
   {
      NDEX++;
      TOKEN = strtok( NULL, delimeters );
 
      if( TOKEN != NULL )
      {
         token[NDEX] = (char *)
                              malloc(sizeof(char)*(strlen(TOKEN)+1));
         strcpy( token[NDEX], TOKEN );
      }
      else
         token[ NDEX ] = TOKEN;
 
   }
 
 
   return token;
 
}
#pragma page(1)
#pragma subtitle(" ")
#pragma subtitle("subtitle - Decode METAR report.              ")
/********************************************************************/
/*                                                                  */
/*  Title:         freeTokens                                       */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          14 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      freeTokens frees the storage allocated for the   */
/*                 character strings stored in the token array.     */
/*                                                                  */
/*  Input:         token  - the address of a pointer to an array    */
/*                          of string tokens.                       */
/*                                                                  */
/*                                                                  */
/*  Output:        None.                                            */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static void freeTokens( char **token )
{
   int NDEX;
 
   NDEX = 0;
   while( *(token+NDEX) != NULL )
   {
      free( *(token+NDEX) );
      NDEX++;
   }
   return;
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         InitDcdMETAR                                     */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  InitDcdMETAR initializes every member of the         */
/*             structure addressed by the pointer Mptr.             */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         Mptr - ptr to a decoded_METAR structure.         */
/*                                                                  */
/*  Output:        NONE                                             */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static void InitDcdMETAR( Decoded_METAR *Mptr )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
 
   int i,
       j;
 
 
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
   memset(Mptr->TS_LOC,'\0',3);
   memset(Mptr->TS_MOVMNT,'\0',3);
 
 
   memset(Mptr->TornadicType,'\0',15);
   memset(Mptr->TornadicLOC,'\0',10);
   memset(Mptr->TornadicDIR,'\0',4);
   memset(Mptr->TornadicMovDir,'\0',3);
   Mptr->BTornadicHour = MAXINT;
   Mptr->BTornadicMinute = MAXINT;
   Mptr->ETornadicHour = MAXINT;
   Mptr->ETornadicMinute = MAXINT;
   Mptr->TornadicDistance = MAXINT;
 
   memset( Mptr->autoIndicator,'\0', 5 );
 
   Mptr->RVRNO = FALSE;
   Mptr->GR = FALSE;
   Mptr->GR_Size = (float) MAXINT;
 
   Mptr->CHINO = FALSE;
   memset(Mptr->CHINO_LOC, '\0', 6);
 
   Mptr->VISNO = FALSE;
   memset(Mptr->VISNO_LOC, '\0', 6);
 
   Mptr->PNO = FALSE;
   Mptr->PWINO = FALSE;
   Mptr->FZRANO  = FALSE;
   Mptr->TSNO   = FALSE;
   Mptr->DollarSign  = FALSE;
   Mptr->hourlyPrecip = (float) MAXINT;
 
   Mptr->ObscurAloftHgt = MAXINT;
   memset(Mptr->ObscurAloft, '\0', 12);
   memset(Mptr->ObscurAloftSkyCond, '\0', 12);
 
   memset(Mptr->VrbSkyBelow, '\0', 4);
   memset(Mptr->VrbSkyAbove, '\0', 4);
   Mptr->VrbSkyLayerHgt = MAXINT;
 
   Mptr->SectorVsby = (float) MAXINT;
   memset( Mptr->SectorVsby_Dir, '\0', 3);
 
   memset(Mptr->codeName, '\0', 6);
   memset(Mptr->stnid, '\0', 5);
   Mptr->ob_hour   = MAXINT;
   Mptr->ob_minute = MAXINT;
   Mptr->ob_date   = MAXINT;
 
   memset(Mptr->synoptic_cloud_type, '\0', 6);
 
   Mptr->CloudLow    = '\0';
   Mptr->CloudMedium = '\0';
   Mptr->CloudHigh   = '\0';
 
   memset(Mptr->snow_depth_group, '\0', 6);
   Mptr->snow_depth = MAXINT;
 
   Mptr->Temp_2_tenths    = (float) MAXINT;
   Mptr->DP_Temp_2_tenths = (float) MAXINT;
 
   Mptr->OCNL_LTG      = FALSE;
   Mptr->FRQ_LTG       = FALSE;
   Mptr->CNS_LTG       = FALSE;
   Mptr->CG_LTG        = FALSE;
   Mptr->IC_LTG        = FALSE;
   Mptr->CC_LTG        = FALSE;
   Mptr->CA_LTG        = FALSE;
   Mptr->AP_LTG        = FALSE;
   Mptr->OVHD_LTG      = FALSE;
   Mptr->DSNT_LTG      = FALSE;
   Mptr->VcyStn_LTG    = FALSE;
   Mptr->LightningVCTS = FALSE;
   Mptr->LightningTS   = FALSE;
 
   memset( Mptr->LTG_DIR, '\0', 3);
 
 
   for( i = 0; i < 3; i++)
   {
      memset(Mptr->ReWx[i].Recent_weather, '\0', 5);
 
      Mptr->ReWx[i].Bhh = MAXINT;
      Mptr->ReWx[i].Bmm = MAXINT;
 
      Mptr->ReWx[i].Ehh = MAXINT;
      Mptr->ReWx[i].Emm = MAXINT;
 
   }
 
   Mptr->NIL_rpt = FALSE;
   Mptr->AUTO = FALSE;
   Mptr->COR  = FALSE;
 
   Mptr->winData.windDir = MAXINT;
   Mptr->winData.windSpeed = MAXINT;
   Mptr->winData.windGust = MAXINT;
   Mptr->winData.windVRB  = FALSE;
   memset(Mptr->winData.windUnits, '\0', 4);
 
   Mptr->minWnDir = MAXINT;
   Mptr->maxWnDir = MAXINT;
 
   memset(Mptr->horiz_vsby, '\0', 5);
   memset(Mptr->dir_min_horiz_vsby, '\0', 3);
 
   Mptr->prevail_vsbySM = (float) MAXINT;
   Mptr->prevail_vsbyM  = (float) MAXINT;
   Mptr->prevail_vsbyKM = (float) MAXINT;
 
   memset(Mptr->vsby_Dir, '\0', 3);
 
   Mptr->CAVOK = FALSE;
 
   for ( i = 0; i < 12; i++ )
   {
      memset(Mptr->RRVR[ i ].runway_designator,
              '\0', 6);
 
      Mptr->RRVR[ i ].visRange = MAXINT;
 
      Mptr->RRVR[ i ].vrbl_visRange = FALSE;
      Mptr->RRVR[ i ].below_min_RVR = FALSE;
      Mptr->RRVR[ i ].above_max_RVR = FALSE;
 
 
      Mptr->RRVR[ i ].Max_visRange = MAXINT;
      Mptr->RRVR[ i ].Min_visRange = MAXINT;
   }
 
   Mptr->DVR.visRange = MAXINT;
   Mptr->DVR.vrbl_visRange = FALSE;
   Mptr->DVR.below_min_DVR = FALSE;
   Mptr->DVR.above_max_DVR = FALSE;
   Mptr->DVR.Max_visRange = MAXINT;
   Mptr->DVR.Min_visRange = MAXINT;
 
   for ( i = 0; i < MAXWXSYMBOLS; i++ )
   {
      for( j = 0; j < 8; j++ )
         Mptr->WxObstruct[i][j] = '\0';
   }
 
   /***********************/
   /* PARTIAL OBSCURATION */
   /***********************/
 
   memset( &(Mptr->PartialObscurationAmt[0][0]), '\0', 7 );
   memset( &(Mptr->PartialObscurationPhenom[0][0]), '\0',12);
 
   memset( &(Mptr->PartialObscurationAmt[1][0]), '\0', 7 );
   memset( &(Mptr->PartialObscurationPhenom[1][0]), '\0',12);
 
 
   /***************************************************/
   /* CLOUD TYPE, CLOUD LEVEL, AND SIGNIFICANT CLOUDS */
   /***************************************************/
 
 
   for ( i = 0; i < 6; i++ )
   {
      memset(Mptr->cldTypHgt[ i ].cloud_type,
              '\0', 5);
 
      memset(Mptr->cldTypHgt[ i ].cloud_hgt_char,
              '\0', 4);
 
      Mptr->cldTypHgt[ i ].cloud_hgt_meters = MAXINT;
 
      memset(Mptr->cldTypHgt[ i ].other_cld_phenom,
              '\0', 4);
   }
 
   Mptr->VertVsby = MAXINT;
 
   Mptr->temp = MAXINT;
   Mptr->dew_pt_temp = MAXINT;
   Mptr->QFE = MAXINT;
 
   Mptr->SLPNO = FALSE;
   Mptr->SLP = (float) MAXINT;
 
   Mptr->A_altstng = FALSE;
   Mptr->inches_altstng = (double) MAXINT;
 
   Mptr->Q_altstng = FALSE;
   Mptr->hectoPasc_altstng = MAXINT;
 
   Mptr->char_prestndcy = MAXINT;
   Mptr->prestndcy = (float) MAXINT;
 
   Mptr->precip_amt = (float) MAXINT;
 
   Mptr->precip_24_amt = (float) MAXINT;
   Mptr->maxtemp       = (float) MAXINT;
   Mptr->mintemp       = (float) MAXINT;
   Mptr->max24temp     = (float) MAXINT;
   Mptr->min24temp     = (float) MAXINT;
 
   Mptr->VIRGA         = FALSE;
   memset( Mptr->VIRGA_DIR, '\0', 3 );
 
   Mptr->VOLCASH       = FALSE;
 
   Mptr->minCeiling    = MAXINT;
   Mptr->maxCeiling    = MAXINT;
 
   Mptr->CIG_2ndSite_Meters = MAXINT;
   memset(Mptr->CIG_2ndSite_LOC, '\0', 10 );
 
   Mptr->minVsby = (float) MAXINT;
   Mptr->maxVsby = (float) MAXINT;
   Mptr->VSBY_2ndSite = (float) MAXINT;
   memset(Mptr->VSBY_2ndSite_LOC,'\0',10);
 
   for( i = 0; i < 6; i++ )
      memset (&(Mptr->SfcObscuration[i][0]), '\0', 10);
 
   Mptr->Num8thsSkyObscured = MAXINT;
 
   Mptr->Indeterminant3_6HrPrecip = FALSE;
   Mptr->Indeterminant_24HrPrecip = FALSE;
   Mptr->CIGNO = FALSE;
   Mptr->Ceiling = MAXINT;
   Mptr->Estimated_Ceiling = MAXINT;
 
   Mptr->NOSPECI = FALSE;
   Mptr->LAST    = FALSE;
 
   Mptr->SNINCR = MAXINT;
   Mptr->SNINCR_TotalDepth = MAXINT;
 
   Mptr->WaterEquivSnow = (float) MAXINT;
 
   Mptr->SunshineDur = MAXINT;
   Mptr->SunSensorOut = FALSE;
 
 
   Mptr->WshfTime_hour = MAXINT;
   Mptr->WshfTime_minute = MAXINT;
   Mptr->Wshft_FROPA     = FALSE;
   Mptr->min_vrbl_wind_dir = MAXINT;
   Mptr->max_vrbl_wind_dir = MAXINT;
 
   Mptr->PRESRR        = FALSE;
   Mptr->PRESFR        = FALSE;
 
   Mptr->TWR_VSBY = (float) MAXINT;
   Mptr->SFC_VSBY = (float) MAXINT;
 
   Mptr->PKWND_dir = MAXINT;
   Mptr->PKWND_speed = MAXINT;
   Mptr->PKWND_hour = MAXINT;
   Mptr->PKWND_minute = MAXINT;
 
   return;
 
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         ResetMETARGroup                                  */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  ResetMETARGroup returns a METAR_obGroup enumerated   */
/*             variable that indicates which METAR reporting group  */
/*             might next appear in the METAR report and should be  */
/*             considered for decoding.                             */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         StartGroup - a METAR_obGroup variable that       */
/*                              indicates where or on what group    */
/*                              METAR Decoding began.               */
/*                                                                  */
/*                 SaveStartGroup - a METAR_obGroup variable that   */
/*                                  indicates the reporting group   */
/*                                  in the METAR report that was    */
/*                                  successfully decoded.           */
/*                                                                  */
/*  Output:        A METAR_obGroup variable that indicates which    */
/*                 reporting group in the METAR report should next  */
/*                 be considered for decoding                       */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static int ResetMETARGroup( int StartGroup,
                            int SaveStartGroup )
{
 
   enum METAR_obGroup { codename, stnid, NIL1, COR1, obDateTime, NIL2,
                        AUTO, COR, windData, MinMaxWinDir,
                        CAVOK, visibility,
                        RVR, presentWX, skyCond, tempGroup,
                        altimStng, NotIDed = 99};
 
   if( StartGroup == NotIDed && SaveStartGroup == NotIDed )
      return NotIDed;
   else if( StartGroup == NotIDed && SaveStartGroup != NotIDed &&
            SaveStartGroup != altimStng )
      return (++SaveStartGroup);
   else
      return (++SaveStartGroup);
 
}
 
#pragma page(1)
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         CodedHgt2Meters                                  */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  CodedHgt2Meters converts a coded cloud height into   */
/*             meters.                                              */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         token - a pointer to a METAR report group.       */
/*                 Mptr - a pointer to a decoded_METAR structure.   */
/*                                                                  */
/*  Output:        Cloud height in meters                           */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static int CodedHgt2Meters( char *token, Decoded_METAR *Mptr )
{
   int hgt;
   static int maxhgt = 30000;
 
 
   if( (hgt = atoi(token)) == 999 )
      return maxhgt;
   else
      return (hgt*30);
}
 
#pragma page(1)
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isPartObscur                                     */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  isPartObscur determines whether or not the METAR     */
/*             report element that is passed to it is or is not     */
/*             a partial obscuration indicator for an amount of     */
/*             obscuration.                                         */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         token - the address of a pointer to the group    */
/*                         in the METAR report that isPartObscur    */
/*                         determines is or is not a partial        */
/*                         obscuration indicator.                   */
/*                                                                  */
/*                                                                  */
/*                 Mptr - a pointer to a decoded_METAR structure.   */
/*                                                                  */
/*  Output:        TRUE, if the group is a partial obscuration      */
/*                 indicator and FALSE, if it is not.               */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isPartObscur( char **string, Decoded_METAR *Mptr,
                          int *NDEX )
{
 
   if( *string == NULL )
      return FALSE;
 
   if( strcmp( *string, "FEW///" ) == 0 ||
       strcmp( *string, "SCT///" ) == 0 ||
       strcmp( *string, "BKN///" ) == 0 ||
       strcmp( *string, "FEW000" ) == 0 ||
       strcmp( *string, "SCT000" ) == 0 ||
       strcmp( *string, "BKN000" ) == 0    ) {
      strcpy( &(Mptr->PartialObscurationAmt[0][0]), *string );
      (*NDEX)++;
      string++;
 
      if( *string == NULL )
         return TRUE;
 
      if( strcmp( (*string+3), "///") ) {
          if( strcmp( *string, "FEW000" ) == 0 ||
              strcmp( *string, "SCT000" ) == 0 ||
              strcmp( *string, "BKN000" ) == 0    ) {
            strcpy( &(Mptr->PartialObscurationAmt[1][0]), *string );
            (*NDEX)++;
         }
      }
      else {
         if( strcmp( *string, "FEW///" ) == 0 ||
             strcmp( *string, "SCT///" ) == 0 ||
             strcmp( *string, "BKN///" ) == 0 ) {
            strcpy( &(Mptr->PartialObscurationAmt[1][0]), *string );
            (*NDEX)++;
         }
      }
      return TRUE;
   }
   else
      return FALSE;
}
 
#pragma page(1)
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isCldLayer                                       */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      isCldLayer determines whether or not the         */
/*                 current group has a valid cloud layer            */
/*                 identifier.                                      */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         token - pointer to a METAR report group.         */
/*                                                                  */
/*  Output:        TRUE, if the report group is a valid cloud       */
/*                 layer indicator.                                 */
/*                                                                  */
/*                 FALSE, if the report group is not a valid cloud  */
/*                 layer indicator.                                 */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isCldLayer( char *token )
{
   if( token == NULL )
      return FALSE;
 
   if( strlen(token) < 6 )
      return FALSE;
   else
      return ((strncmp(token,"OVC",3) == 0 ||
               strncmp(token,"SCT",3) == 0 ||
               strncmp(token,"FEW",3) == 0 ||
               strncmp(token,"BKN",3) == 0 ||
               (isdigit(*token) &&
                strncmp(token+1,"CU",2) == 0) ||
               (isdigit(*token) &&
                strncmp(token+1,"SC",2) == 0) ) &&
               nisdigit((token+3),3)) ? TRUE:FALSE;
}
 
#pragma page(1)
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isCAVOK                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          09 May 1996                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      isCAVOK determines whether or not the current    */
/*                 group is a valid CAVOK indicator.                */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         token - pointer to a METAR report group.         */
/*                                                                  */
/*  Output:        TRUE, if the input group is a valid CAVOK        */
/*                 indicator.  FALSE, otherwise.                    */
/*                                                                  */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isCAVOK( char *token, Decoded_METAR *Mptr, int *NDEX )
{
 
   if( token == NULL )
      return FALSE;
 
   if( strcmp(token, "CAVOK") != 0 )
      return FALSE;
   else {
      (*NDEX)++;
      Mptr->CAVOK = TRUE;
      return TRUE;
   }
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         parseCldData                                     */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static void parseCldData( char *token, Decoded_METAR *Mptr, int next)
{
 
 
   if( token == NULL )
      return;
 
   if( strlen(token) > 6 )
      strncpy(Mptr->cldTypHgt[next].other_cld_phenom,token+6,
              (strlen(token)-6));
 
   strncpy(Mptr->cldTypHgt[next].cloud_type,token,3);
 
   strncpy(Mptr->cldTypHgt[next].cloud_hgt_char,token+3,3);
 
   Mptr->cldTypHgt[next].cloud_hgt_meters =
                               CodedHgt2Meters( token+3, Mptr );
 
   return;
}
 
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isSkyCond                                        */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isSkyCond( char **skycond, Decoded_METAR *Mptr,
                        int *NDEX )
{
 
   bool first_layer,
        second_layer,
        third_layer,
        fourth_layer,
        fifth_layer,
        sixth_layer;
   int next;
 
      /********************************************************/
      /* INTERROGATE skycond TO DETERMINE IF "CLR" IS PRESENT */
      /********************************************************/
 
   if( *skycond == NULL )
      return FALSE;
 
 
   if( strcmp(*skycond,"CLR") == 0)
   {
      strcpy(Mptr->cldTypHgt[0].cloud_type,"CLR");
/*
      memset(Mptr->cldTypHgt[0].cloud_hgt_char,'\0',1);
      memset(Mptr->cldTypHgt[0].other_cld_phenom,
              '\0', 1);
*/
      (*NDEX)++;
      return TRUE;
   }
 
      /********************************************************/
      /* INTERROGATE skycond TO DETERMINE IF "SKC" IS PRESENT */
      /********************************************************/
 
   else if( strcmp(*skycond,"SKC") == 0)
   {
      strcpy(Mptr->cldTypHgt[0].cloud_type,"SKC");
/*
      memset(Mptr->cldTypHgt[0].cloud_hgt_char,'\0',1);
      memset(Mptr->cldTypHgt[0].other_cld_phenom,
              '\0', 1);
*/
      (*NDEX)++;
      return TRUE;
   }
 
      /****************************************/
      /* INTERROGATE skycond TO DETERMINE IF  */
      /*    VERTICAL VISIBILITY IS PRESENT    */
      /****************************************/
 
   else if( strncmp(*skycond,"VV",2) == 0
             && strlen(*skycond) == 5 &&
                  nisdigit((*skycond+2),3) )
   {
      Mptr->VertVsby = CodedHgt2Meters( (*skycond+2), Mptr);
      strncpy(Mptr->cldTypHgt[0].cloud_type,*skycond,2);
      (*NDEX)++;
      return TRUE;
   }
 
      /****************************************/
      /* INTERROGATE skycond TO DETERMINE IF  */
      /*    CLOUD LAYER DATA IS PRESENT       */
      /****************************************/
 
   else if( isCldLayer( *skycond ))
   {
      next = 0;
 
      parseCldData( *skycond , Mptr, next );
      first_layer = TRUE;
      next++;
      (++skycond);
 
      if( *skycond == NULL )
         return TRUE;
 
      second_layer = FALSE;
      third_layer = FALSE;
      fourth_layer = FALSE;
      fifth_layer = FALSE;
      sixth_layer = FALSE;
 
 
      if( isCldLayer( *skycond ) && first_layer )
      {
         parseCldData( *skycond, Mptr, next );
         second_layer = TRUE;
         next++;
         (++skycond);
 
         if( *skycond == NULL )
            return TRUE;
 
      }
 
      if( isCldLayer( *skycond ) && first_layer &&
          second_layer )
      {
         parseCldData( *skycond , Mptr, next );
         third_layer = TRUE;
         next++;
         (++skycond);
 
         if( *skycond == NULL )
            return TRUE;
 
      }
 
      if( isCldLayer( *skycond ) && first_layer && second_layer &&
                      third_layer )
      {
         parseCldData( *skycond, Mptr, next );
         fourth_layer = TRUE;
         next++;
         (++skycond);
 
         if( *skycond == NULL )
            return TRUE;
 
      }
 
      if( isCldLayer( *skycond ) && first_layer && second_layer &&
          third_layer && fourth_layer )
      {
         parseCldData( *skycond , Mptr, next );
         fifth_layer = TRUE;
         next++;
         (++skycond);
 
         if( *skycond == NULL )
            return TRUE;
 
      }
 
      if( isCldLayer( *skycond ) && first_layer && second_layer &&
          third_layer && fourth_layer && fifth_layer )
      {
         parseCldData( *skycond , Mptr, next );
         sixth_layer = TRUE;
      }
 
 
 
      if( sixth_layer )
      {
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else if( fifth_layer )
      {
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else if( fourth_layer )
      {
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else if( third_layer )
      {
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else if( second_layer )
      {
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else if( first_layer )
      {
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
 
   }
   else
      return FALSE;
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         prevailVSBY                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static float prevailVSBY( char *visibility )
{
   float Miles_vsby;
   char *temp,
        *Slash_ptr,
        *SM_KM_ptr;
   char numerator[3],
        denominator[3];
 
 
   if( (SM_KM_ptr = strstr( visibility, "SM" )) == NULL )
      SM_KM_ptr = strstr(visibility, "KM");
 
   Slash_ptr = strchr( visibility, '/' );
 
   if( Slash_ptr == NULL )
   {
      temp = (char *) malloc(sizeof(char) *
                          ((SM_KM_ptr - visibility)+1));
      memset( temp, '\0', (SM_KM_ptr-visibility)+1);
      strncpy( temp, visibility, (SM_KM_ptr-visibility) );
      Miles_vsby = (float) (atoi(temp));
      free( temp );
      return Miles_vsby;
   }
   else
   {
      memset(numerator,   '\0', 3);
      memset(denominator, '\0', 3);
 
      strncpy(numerator, visibility, (Slash_ptr - visibility));
 
/*>>>>>>>>>>>>>>>>>>>>>>
      if( (SM_KM_ptr - (Slash_ptr+1)) == 0 )
         strcpy(denominator, "4");
      else
<<<<<<<<<<<<<<<<<<<<<<*/
 
      strncpy(denominator,
              Slash_ptr+1, (SM_KM_ptr - Slash_ptr));
 
      return ( ((float)(atoi(numerator)))/
               ((float)(atoi(denominator))) );
   }
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isVisibility                                     */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
#pragma page(1)
 
static bool isVisibility( char **visblty, Decoded_METAR *Mptr,
                          int *NDEX )
{
   char *achar,
        *astring,
        *save_token;
 
 
   /****************************************/
   /* CHECK FOR VISIBILITY MEASURED <1/4SM */
   /****************************************/
 
   if( *visblty == NULL )
      return FALSE;
 
 
   if( strcmp(*visblty,"M1/4SM") == 0 ||
       strcmp(*visblty,"<1/4SM") == 0 ) {
      Mptr->prevail_vsbySM = 0.0;
      (*NDEX)++;
      return TRUE;
   }
 
   /***********************************************/
   /* CHECK FOR VISIBILITY MEASURED IN KILOMETERS */
   /***********************************************/
 
   if( (achar = strstr(*visblty, "KM")) != NULL )
   {
      if( nisdigit(*visblty,(achar - *visblty)) &&
                        (achar - *visblty) > 0 )
      {
         Mptr->prevail_vsbyKM = prevailVSBY( *visblty );
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
 
   /***********************************/
   /* CHECK FOR VISIBILITY MEASURED   */
   /* IN A FRACTION OF A STATUTE MILE */
   /***********************************/
 
   else if( (achar = strchr( *visblty, '/' )) !=
                    NULL &&
       (astring = strstr( *visblty, "SM")) != NULL )
   {
      if( nisdigit(*visblty,(achar - *visblty))
                     &&
               (achar - *visblty) > 0 &&
               (astring - (achar+1)) > 0 &&
                nisdigit(achar+1, (astring - (achar+1))) )
      {
         Mptr->prevail_vsbySM = prevailVSBY (*visblty);
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
 
   /***********************************/
   /* CHECK FOR VISIBILITY MEASURED   */
   /*     IN WHOLE STATUTE MILES      */
   /***********************************/
 
   else if( (astring = strstr(*visblty,"SM") ) != NULL )
   {
      if( nisdigit(*visblty,(astring - *visblty)) &&
                       (astring- *visblty) > 0 )
      {
         Mptr->prevail_vsbySM = prevailVSBY (*visblty);
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
 
   /***********************************/
   /* CHECK FOR VISIBILITY MEASURED   */
   /* IN WHOLE AND FRACTIONAL STATUTE */
   /*             MILES               */
   /***********************************/
 
   else if( nisdigit( *visblty,
               strlen(*visblty)) &&
                            strlen(*visblty) < 4 )
   {
      save_token = (char *) malloc(sizeof(char)*
                              (strlen(*visblty)+1));
      strcpy(save_token,*visblty);
      if( *(++visblty) == NULL)
      {
         free( save_token );
         return FALSE;
      }
 
      if( (achar = strchr( *visblty, '/' ) ) != NULL &&
          (astring = strstr( *visblty, "SM") ) != NULL  )
      {
         if( nisdigit(*visblty,
                 (achar - *visblty)) &&
                 (achar - *visblty) > 0 &&
                 (astring - (achar+1)) > 0 &&
             nisdigit(achar+1, (astring - (achar+1))) )
         {
            Mptr->prevail_vsbySM = prevailVSBY (*visblty);
            Mptr->prevail_vsbySM +=
                                 (float) (atoi(save_token));
            free( save_token);
 
            (*NDEX)++;
            (*NDEX)++;
 
            return TRUE;
 
         }
         else
            return FALSE;
      }
      else
         return FALSE;
 
   }
 
   /***********************************/
   /* CHECK FOR VISIBILITY MEASURED   */
   /* IN METERS WITH OR WITHOUT DI-   */
   /*     RECTION OF OBSERVATION      */
   /***********************************/
 
   else if( nisdigit(*visblty,4) &&
                strlen(*visblty) >= 4)
   {
      if( strcmp(*visblty+4,"NE") == 0 )
      {
         memset(Mptr->vsby_Dir,'\0',3);
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
      if( strcmp(*visblty+4,"NW") == 0 )
      {
         memset(Mptr->vsby_Dir,'\0',3);
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
      if( strcmp(*visblty+4,"SE") == 0 )
      {
         memset(Mptr->vsby_Dir,'\0',3);
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
      if( strcmp(*visblty+4,"SW") == 0 )
      {
         memset(Mptr->vsby_Dir,'\0',3);
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
      if( strcmp(*visblty+4,"N") == 0 )
      {
         memset(Mptr->vsby_Dir,'\0',3);
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
      if( strcmp(*visblty+4,"S") == 0 )
      {
         memset(Mptr->vsby_Dir,'\0',3);
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
      if( strcmp(*visblty+4,"E") == 0 )
      {
         memset(Mptr->vsby_Dir,'\0',3);
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
      if( strcmp(*visblty+4,"W") == 0 )
      {
         memset(Mptr->vsby_Dir,'\0',3);
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
 
      if( antoi(*visblty,
                  strlen(*visblty)) >= 50 &&
               antoi(*visblty,
                  strlen(*visblty)) <= 500 &&
              (antoi(*visblty,
                  strlen(*visblty)) % 50) == 0 )
      {
         Mptr->prevail_vsbyM =
           (float) (antoi(*visblty,
                       strlen(*visblty)));
         (*NDEX)++;
         return TRUE;
      }
      else if( antoi(*visblty,
                 strlen(*visblty)) >= 500 &&
           antoi(*visblty,
                 strlen(*visblty)) <= 3000 &&
          (antoi(*visblty,
                 strlen(*visblty)) % 100) == 0 )
      {
         Mptr->prevail_vsbyM =
            (float) (antoi(*visblty,
                      strlen(*visblty)));
         (*NDEX)++;
         return TRUE;
      }
      else if( antoi(*visblty,
              strlen(*visblty)) >= 3000 &&
          antoi(*visblty,
              strlen(*visblty)) <= 5000 &&
          (antoi(*visblty,
                  strlen(*visblty)) % 500) == 0 )
      {
         Mptr->prevail_vsbyM =
               (float) (antoi(*visblty,
                    strlen(*visblty)));
         (*NDEX)++;
         return TRUE;
      }
      else if( antoi(*visblty,
            strlen(*visblty)) >= 5000 &&
          antoi(*visblty,
            strlen(*visblty)) <= 9999 &&
          (antoi(*visblty,
            strlen(*visblty)) % 500) == 0 ||
           antoi(*visblty,
            strlen(*visblty)) == 9999 )
      {
         Mptr->prevail_vsbyM =
                (float) (antoi(*visblty,
                     strlen(*visblty)));
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
 
   }
   else
      return FALSE;
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         vrblVsby                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool vrblVsby( char *string1, char *string2,
                      Decoded_METAR *Mptr, int *NDEX )
{
   char buf[ 6 ];
   int numerator,
       denominator;
   char *slash,
        *V_char,
        *temp;
 
   if( string1 == NULL )
      return FALSE;
 
   V_char = strchr(string1,'V');
   slash =  strchr(string1,'/');
 
   if(slash == NULL)
   {
      if(nisdigit(string1,V_char-string1))
      {
         memset(buf, '\0', 6);
         strncpy(buf, string1, V_char-string1);
 
         if( Mptr->minVsby != (float) MAXINT )
            Mptr->minVsby += (float) atoi(buf);
         else
            Mptr->minVsby  = (float) atoi(buf);
 
         memset(buf, '\0', 6);
         strncpy(buf, V_char+1, 5);
         Mptr->maxVsby = (float) atoi(buf);
 
      }
      else
         return FALSE;
   }
   else
   {
      temp = (char *) malloc(sizeof(char)*((V_char-string1)+1));
      memset(temp, '\0', (V_char-string1) +1);
      strncpy(temp, string1, V_char-string1);
      if( Mptr->minVsby != MAXINT )
         Mptr->minVsby += fracPart(temp);
      else
         Mptr->minVsby = fracPart(temp);
 
      free( temp );
 
      if( strchr(V_char+1,'/') != NULL)
         Mptr->maxVsby = fracPart(V_char+1);
      else
         Mptr->maxVsby = (float) atoi(V_char+1);
   }
 
   if( string2 == NULL )
      return TRUE;
   else
   {
      slash = strchr( string2, '/' );
 
      if( slash == NULL )
         return TRUE;
      else
      {
         if( nisdigit(string2,slash-string2) &&
             nisdigit(slash+1,strlen(slash+1)) )
         {
            Mptr->maxVsby += fracPart(string2);
            (*NDEX)++;
         }
         return TRUE;
      }
   }
 
}
 
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isMinMaxWinDir                                   */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isMinMaxWinDir( char *string, Decoded_METAR *Mptr,
     int *NDEX )
{
#define buf_len 50
   char buf[ buf_len ];
   char *V_char;
 
   if( string == NULL )
      return FALSE;
 
   if( (V_char = strchr(string,'V')) == NULL )
      return FALSE;
   else
   {
      if( nisdigit(string,(V_char - string)) &&
               nisdigit(V_char+1,3) )
      {
         memset( buf, '\0', buf_len);
         strncpy( buf, string, V_char - string);
         Mptr->minWnDir = atoi( buf );
 
         memset( buf, '\0', buf_len);
         strcpy( buf, V_char+1 );
         Mptr->maxWnDir = atoi( buf );
 
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isRVR                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isRVR( char *token, Decoded_METAR *Mptr, int *NDEX,
                     int ndex )
{
   char *slashPtr, *FT_ptr;
   char *vPtr;
   int length;
 
   if( token == NULL )
      return FALSE;
 
   if( *token != 'R' || (length = strlen(token)) < 7 ||
        (slashPtr = strchr(token,'/')) == NULL ||
        nisdigit(token+1,2) == FALSE )
      return FALSE;
 
   if( (slashPtr - (token+3)) > 0 )
      if( !nisalpha(token+3,(slashPtr - (token+3))) )
         return FALSE;
 
   if( strcmp(token+(strlen(token)-2),"FT") != 0 )
      return FALSE;
   else
      FT_ptr = token + (strlen(token)-2);
 
   if( strchr(slashPtr+1, 'P' ) != NULL )
      Mptr->RRVR[ndex].above_max_RVR = TRUE;
 
   if( strchr(slashPtr+1, 'M' ) != NULL )
      Mptr->RRVR[ndex].below_min_RVR = TRUE;
 
 
   strncpy(Mptr->RRVR[ndex].runway_designator, token+1,
           (slashPtr-(token+1)));
 
   if( (vPtr = strchr(slashPtr, 'V' )) != NULL )
   {
      Mptr->RRVR[ndex].vrbl_visRange = TRUE;
      Mptr->RRVR[ndex].Min_visRange = antoi(slashPtr+1,
                              (vPtr-(slashPtr+1)) );
      Mptr->RRVR[ndex].Max_visRange = antoi(vPtr+1,
                              (FT_ptr - (vPtr+1)) );
      (*NDEX)++;
      return TRUE;
   }
   else
   {
      if( Mptr->RRVR[ndex].below_min_RVR ||
          Mptr->RRVR[ndex].above_max_RVR    )
         Mptr->RRVR[ndex].visRange = antoi(slashPtr+2,
                           (FT_ptr - (slashPtr+2)) );
      else
         Mptr->RRVR[ndex].visRange = antoi(slashPtr+1,
                           (FT_ptr - (slashPtr+1)) );
 
      (*NDEX)++;
      return TRUE;
   }
 
}
 
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isAltimStng                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isAltimStng( char *token, Decoded_METAR *Mptr, int *NDEX )
{
   char dummy[6];
 
 
   if( token == NULL )
      return FALSE;
 
   if( strlen(token) < 5 )
      return FALSE;
   else
   {
      Mptr->A_altstng = FALSE;
      Mptr->Q_altstng = FALSE;
 
      if( (*token == 'A' || *token == 'Q') &&
           (nisdigit(token+1, strlen(token)-1) ||
            nisdigit(token+1,strlen(token)-3)) )
      {
         if( *token == 'A' )
         {
            Mptr->A_altstng = TRUE;
            Mptr->inches_altstng = atof(token+1) * 0.01;
         }
         else
         {
            Mptr->Q_altstng = TRUE;
 
            if( strchr(token,'.') != NULL)
            {
               memset(dummy, '\0', 6);
               strncpy(dummy,token+1,4);
               Mptr->hectoPasc_altstng = atoi(dummy);
            }
            else
               Mptr->hectoPasc_altstng = atoi(token+1);
         }
 
         (*NDEX)++;
         return TRUE;
 
      }
      return FALSE;
   }
}
 
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isTempGroup                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isTempGroup( char *token, Decoded_METAR *Mptr, int *NDEX)
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   char *slash;
 
   if( token == NULL )
      return FALSE;
 
   if( (slash = strchr(token,'/')) == NULL)
      return FALSE;
   else
   {
      if( charcmp(token,"aa'/'dd") ) {
         Mptr->dew_pt_temp = atoi(slash+1);
         (*NDEX)++;
         return TRUE;
      }
      else if( charcmp(token,"aa'/''M'dd") ) {
         Mptr->dew_pt_temp = atoi(slash+2) * -1;
         (*NDEX)++;
         return TRUE;
      }
      else if( charcmp(token,"dd'/'aa") ) {
         Mptr->temp = antoi(token,(slash-token));
         (*NDEX)++;
         return TRUE;
      }
      else if( charcmp(token,"'M'dd'/'aa") ) {
         Mptr->temp = antoi(token+1,(slash-(token+1))) * -1;
         (*NDEX)++;
         return TRUE;
      }
      else if( nisdigit(token,(slash-token)) &&
           nisdigit(slash+1,strlen(slash+1)) )
      {
         Mptr->temp = antoi(token,(slash-token));
         Mptr->dew_pt_temp = atoi(slash+1);
         (*NDEX)++;
         return TRUE;
      }
      else if( *token == 'M' && nisdigit(token+1,(slash-(token+1)))
                && *(slash+1) != '\0' &&
            *(slash+1) == 'M' && nisdigit(slash+2,strlen(slash+2)) )
      {
         Mptr->temp = antoi(token+1,(slash-(token+1))) * -1;
         Mptr->dew_pt_temp = atoi(slash+2) * -1;
         (*NDEX)++;
         return TRUE;
      }
      else if( *token == 'M' && nisdigit(token+1,(slash-(token+1)))
                 && *(slash+1) != '\0' &&
               nisdigit(slash+1,strlen(slash+1)) )
      {
         Mptr->temp = antoi(token+1,(slash-(token+1))) * -1;
         Mptr->dew_pt_temp = atoi(slash+1);
         (*NDEX)++;
         return TRUE;
      }
      else if( nisdigit(token,(slash - token)) &&
                    *(slash+1) != '\0' &&
                    nisdigit(slash+2,strlen(slash+2)) )
      {
         Mptr->temp = antoi(token,(slash-token));
         Mptr->dew_pt_temp = atoi(slash+2) * -1;
         (*NDEX)++;
         return TRUE;
      }
      else if( nisdigit(token,(slash-token)) &&
           strlen(token) <= 3)
      {
         Mptr->temp = antoi(token,(slash-token));
         (*NDEX)++;
         return TRUE;
      }
      else if( *token == 'M' &&
                   nisdigit(token+1,(slash-(token+1))) &&
                   strlen(token) <= 4)
      {
         Mptr->temp = antoi(token+1,(slash-(token+1))) * -1;
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
 
}
 
 
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isWxToken                                        */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isWxToken( char *token )
{
   int i;
 
   if( token == NULL )
      return FALSE;
   for( i = 0; i < strlen(token); i++ )
   {
      if( !(isalpha(*(token+i)) || *(token+i) == '+' ||
                                   *(token+i) == '-'   ) )
         return FALSE;
   }
   return TRUE;
}
 
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isPresentWX                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isPresentWX( char *token, Decoded_METAR *Mptr,
                        int *NDEX, int *next )
{
   static char *WxSymbols[] = {"BCFG", "BLDU", "BLSA", "BLPY",
          "BLSN", "FZBR", "VCBR", "TSGR", "VCTS",
          "DRDU", "DRSA", "DRSN", "FZFG", "FZDZ", "FZRA",
          "PRFG", "MIFG",
          "SHRA", "SHSN", "SHPE", "SHPL", "SHGS",
          "SHGR",
          "VCFG", "VCFC",
          "VCSS", "VCDS", "TSRA", "TSPE", "TSPL", "TSSN",
          "VCSH", "VCPO", "VCBLDU", "VCBLSA", "VCBLSN",
 
          "BR", "DU",
          "DZ", "DS",
          "FG", "FC", "FU", "GS", "GR", "HZ", "IC",
          "PE", "PL", "PO", "RA",
          "SN", "SG", "SQ", "SA", "SS", "TS",
          "VA",
          "PY", NULL};
 
   int i;
   char *ptr,
        *temp_token,
        *save_token,
        *temp_token_orig;
 
   if( token == NULL)
      return FALSE;
 
   temp_token_orig = temp_token =
        (char *) malloc(sizeof(char)*(strlen(token)+1));
   strcpy(temp_token, token);
   while( temp_token != NULL && (*next) < MAXWXSYMBOLS )
   {
      i = 0;
      save_token = NULL;
 
      if( *temp_token == '+' || *temp_token == '-' )
      {
         save_token = temp_token;
         temp_token++;
      }
 
      while( WxSymbols[i] != NULL )
         if( strncmp(temp_token, WxSymbols[i],
                      strlen(WxSymbols[i])) != 0 )
            i++;
         else
            break;
 
      if( WxSymbols[i] == NULL ) {
         free( temp_token_orig );
         return FALSE;
      }
      else
      {
 
         if( save_token != NULL )
         {
            strncpy( Mptr->WxObstruct[*next], save_token, 1);
            strcpy( (Mptr->WxObstruct[*next])+1,
                              WxSymbols[i]);
            (*next)++;
         }
         else
         {
            strcpy( Mptr->WxObstruct[*next], WxSymbols[i]);
            (*next)++;
         }
 
 
         if( strcmp(temp_token, WxSymbols[i]) != 0)
         {
            ptr = strstr(temp_token, WxSymbols[i]);
            temp_token = ptr + strlen(WxSymbols[i]);
         }
         else
         {
            free( temp_token_orig );
            temp_token = NULL;
            (*NDEX)++;
            return TRUE;
         }
 
      }
 
   }
 
   free( temp_token_orig );
   return FALSE;
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isStnID                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isStnId( char *stnID, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( stnID == NULL )
      return FALSE;
 
#ifdef CMCPRT
   printf("isStnId:  stnID = %s\n",stnID);
#endif
 
   if( strlen(stnID) == 4 )
   {
      if( nisalpha(stnID,1) != 0 && nisalnum(stnID+1,3) != 0 ) {
         strcpy(Mptr->stnid,stnID);
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
   else
      return FALSE;
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isCodeName                                       */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isCodeName( char *codename, Decoded_METAR *Mptr, int *NDEX)
{
   if( codename == NULL )
      return FALSE;
 
   if( strcmp(codename,"METAR") == 0 ||
       strcmp(codename,"SPECI") == 0   )
   {
      strcpy(Mptr->codeName, codename );
      (*NDEX)++;
      return TRUE;
   }
   else
      return FALSE;
 
}
 
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isNIL                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isNIL( char *token, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( token == NULL )
      return FALSE;
 
   if( strcmp(token, "NIL") == 0 )
   {
      Mptr->NIL_rpt = TRUE;
      (*NDEX)++;
      return TRUE;
   }
   else
      return FALSE;
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isAUTO                                           */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isAUTO( char *token, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( token == NULL )
      return FALSE;
 
   if( strcmp(token, "AUTO") == 0 )
   {
      Mptr->AUTO = TRUE;
      (*NDEX)++;
      return TRUE;
   }
   else
      return FALSE;
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isCOR                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          24 Apr 1996                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isCOR ( char *token, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( token == NULL )
      return FALSE;
 
   if( strcmp(token, "COR") == 0 )
   {
      Mptr->COR  = TRUE;
      (*NDEX)++;
      return TRUE;
   }
   else
      return FALSE;
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isTimeUTC                                        */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isTimeUTC( char *UTC, Decoded_METAR *Mptr, int *NDEX )
{
 
   if( UTC == NULL )
      return FALSE;
 
   if( strlen( UTC ) == 4 ) {
      if(nisdigit(UTC,4) ) {
         Mptr->ob_hour = antoi(UTC,2);
         Mptr->ob_minute = antoi(UTC+2,2);
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
   else if( strlen( UTC ) == 6 ) {
      if(nisdigit(UTC,6) ) {
         Mptr->ob_date = antoi(UTC,2);
         Mptr->ob_hour = antoi(UTC+2,2);
         Mptr->ob_minute = antoi(UTC+4,2);
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
   if( strlen( UTC ) == 5 ) {
      if(nisdigit(UTC,4) && (*(UTC+4) == 'Z') ) {
         Mptr->ob_hour = antoi(UTC,2);
         Mptr->ob_minute = antoi(UTC+2,2);
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
   else if( strlen( UTC ) == 7 ) {
      if(nisdigit(UTC,6) && (*(UTC+6) == 'Z') ) {
         Mptr->ob_date = antoi(UTC,2);
         Mptr->ob_hour = antoi(UTC+2, 2);
         Mptr->ob_minute = antoi(UTC+4, 2 );
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
   else
      return FALSE;
}
 
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isWindData                                       */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isWindData( char *wind, Decoded_METAR *Mptr, int *NDEX )
{
 
   char *GustPtr,
        *unitsPtr;
   char dummy[8];
 
   if( wind == NULL )
      return FALSE;
 
   if( strlen(wind) < 7 )
      return FALSE;
 
   memset(dummy,'\0',8);
 
   /***************************************/
   /* CHECK FOR WIND SPEED UNITS OF KNOTS */
   /***************************************/
 
/*
   if( ( unitsPtr = strstr( wind, "KMH" ) ) != NULL )
      strcpy( dummy, "KMH" );
   else if( (unitsPtr = strstr( wind, "MPS") ) != NULL )
      strcpy( dummy, "MPS" );
*/
 
   if( (unitsPtr = strstr( wind, "KT") ) != NULL )
      strcpy( dummy, "KT" );
   else
      return FALSE;
 
   /*****************************************/
   /* CHECK FOR VARIABLE ("VRB") WIND SPEED */
   /*****************************************/
 
   if( charcmp(wind,"'V''R''B'dd'K''T'")) {
      Mptr->winData.windVRB = TRUE;
      Mptr->winData.windSpeed = antoi(wind+3,2);
      memset(Mptr->winData.windUnits, '\0', 4);
      strcpy(Mptr->winData.windUnits,"KT");
      (*NDEX)++;
/*
printf("isWindData:  Passed VRBddKT test - wind = %s\n",wind);
*/
      return TRUE;
   }
 
   if( charcmp(wind,"'V''R''B'ddd'K''T'")) {
      Mptr->winData.windVRB = TRUE;
      Mptr->winData.windSpeed = antoi(wind+3,3);
      memset(Mptr->winData.windUnits, '\0', 4);
      strcpy(Mptr->winData.windUnits,"KT");
      (*NDEX)++;
/*
printf("isWindData:  Passed VRBdddKT test - wind = %s\n",wind);
*/
      return TRUE;
   }
 
   if( charcmp(wind,"'V''R''B'ddd'G'ddd'K''T'")) {
      Mptr->winData.windVRB = TRUE;
      Mptr->winData.windSpeed = antoi(wind+3,3);
      Mptr->winData.windGust = antoi(wind+7,3);
 
      memset(Mptr->winData.windUnits, '\0', 4);
      strcpy(Mptr->winData.windUnits,"KT");
      (*NDEX)++;
      return TRUE;
   }
 
   if( charcmp(wind,"'V''R''B'dd'G'dd'K''T'")) {
      Mptr->winData.windVRB = TRUE;
      Mptr->winData.windSpeed = antoi(wind+3,2);
      Mptr->winData.windGust = antoi(wind+6,2);
 
      memset(Mptr->winData.windUnits, '\0', 4);
      strcpy(Mptr->winData.windUnits,"KT");
      (*NDEX)++;
      return TRUE;
   }
 
   if( charcmp(wind,"'V''R''B'dd'G'ddd'K''T'")) {
      Mptr->winData.windVRB = TRUE;
      Mptr->winData.windSpeed = antoi(wind+3,2);
      Mptr->winData.windGust = antoi(wind+6,3);
 
      memset(Mptr->winData.windUnits, '\0', 4);
      strcpy(Mptr->winData.windUnits,"KT");
      (*NDEX)++;
      return TRUE;
   }
 
   /************************/
   /* CHECK FOR WIND GUSTS */
   /************************/
 
   if( (GustPtr = strchr( wind, 'G' )) != NULL )
   {
/*
printf("isWindData:  Passed 1st GUST test - wind = %s\n",wind);
*/
      if( nisdigit(wind,(GustPtr-wind)) &&
            nisdigit(GustPtr+1,(unitsPtr-(GustPtr+1))) &&
            ((GustPtr-wind) >= 5 && (GustPtr-wind) <= 6) &&
            ((unitsPtr-(GustPtr+1)) >= 2 &&
             (unitsPtr-(GustPtr+1)) <= 3) )
      {
         Mptr->winData.windDir = antoi(wind,3);
 
         Mptr->winData.windSpeed = antoi(wind+3, (GustPtr-(wind+3)));
         Mptr->winData.windGust = antoi(GustPtr+1,(unitsPtr-
                                                    (GustPtr+1)));
         strcpy( Mptr->winData.windUnits, dummy );
/*
printf("isWindData:  Passed 2nd GUST test - wind = %s\n",wind);
*/
         (*NDEX)++;
         return TRUE;
      }
      else {
/*
printf("isWindData:  Failed 2nd GUST test - wind = %s\n",wind);
*/
         return FALSE;
      }
   }
   else if( nisdigit(wind,(unitsPtr-wind)) &&
            ((unitsPtr-wind) >= 5 && (unitsPtr-wind) <= 6) )
   {
      Mptr->winData.windDir = antoi(wind, 3);
 
      Mptr->winData.windSpeed = antoi(wind+3,(unitsPtr-(wind+3)));
      strcpy( Mptr->winData.windUnits, dummy );
      (*NDEX)++;
/*
printf("isWindData:  Passed dddff(f) test - wind = %s\n",wind);
*/
      return TRUE;
   }
   else
      return FALSE;
 
}
#pragma page(1)
#pragma subtitle(" ")
#pragma subtitle("subtitle - Decode METAR report.              ")
/********************************************************************/
/*                                                                  */
/*  Title:         DcdMETAR                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          14 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      DcdMETAR takes a pointer to a METAR report char- */
/*                 acter string as input, decodes the report, and   */
/*                 puts the individual decoded/parsed groups into   */
/*                 a structure that has the variable type           */
/*                 Decoded_METAR.                                   */
/*                                                                  */
/*  Input:         string - a pointer to a METAR report character   */
/*                          string.                                 */
/*                                                                  */
/*  Output:        Mptr   - a pointer to a structure that has the   */
/*                          variable type Decoded_METAR.            */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
 
int DcdMETAR( char *string , Decoded_METAR *Mptr )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
 
   enum METAR_obGroup { codename, stnid, NIL1, COR1, obDateTime, NIL2,
                        AUTO, COR, windData, MinMaxWinDir,
                        CAVOK, visibility,
                        RVR, presentWX, PartialObscur,
                        skyCond, tempGroup,
                        altimStng, NotIDed = 99} StartGroup,
                                      SaveStartGroup,
                                      MetarGroup;
 
   WindStruct *WinDataPtr;
 
   int    ndex,
          NDEX,
          i,
          jkj,
          j;
 
 
   char   **token,
          *delimeters = {" "};
 
   bool IS_NOT_RMKS;
 
/*********************************/
/* BEGIN THE BODY OF THE ROUTINE */
/*********************************/
 
   /********************************************************/
   /* ONLY PARSE OR DECOCODE NON-NULL METAR REPORT STRINGS */
   /********************************************************/
 
   if( string == NULL )
      return 8;
 
 
   /*****************************************/
   /*   INITIALIZE STRUCTURE THAT HAS THE   */
   /*      VARIABLE TYPE Decoded_METAR      */
   /*****************************************/
 
   InitDcdMETAR( Mptr );
 
#ifdef DEBUGZZ
   printf("DcdMETAR: Returned from InitDcdMETAR\n");
#endif
 
 
   /****************************************************/
   /* TOKENIZE AND STORE THE INPUT METAR REPORT STRING */
   /****************************************************/
#ifdef DEBUGZZ
   printf("DcdMETAR: Before start of tokenizing, string = %s\n",
             string);
#endif
 
   token = SaveTokenString( string, delimeters );
 
 
 
   /*********************************************************/
   /* DECODE THE METAR REPORT (POSITIONAL ORDER PRECEDENCE) */
   /*********************************************************/
 
   NDEX = 0;
   MetarGroup = codename;
   IS_NOT_RMKS = TRUE;
 
#ifdef DEBUGZZ
printf("DcdMETAR: token[0] = %s\n",token[0]);
#endif
 
   while( token[NDEX] != NULL && IS_NOT_RMKS ) {
 
#ifdef DEBUGZZ
if( strcmp(token[0],"OPKC") == 0 || strcmp(token[0],"TAPA") == 0 ) {
   printf("DcdMETAR:  token[%d] = %s\n",NDEX,token[NDEX]);
   printf("DcdMETAR: Token[%d] = %s\n",NDEX,token[NDEX]);
   printf("DcdMETAR: MetarGroup = %d\n",MetarGroup);
}
#endif
 
    if( strcmp( token[NDEX], "RMK" ) != 0 ) {
 
      StartGroup = NotIDed;
 
#ifdef DEBUGZZ
if( strcmp(token[0],"OPKC") == 0 || strcmp(token[0],"TAPA") == 0 ) {
   printf("DcdMETAR: StartGroup = %d\n",StartGroup);
   printf("DcdMETAR: SaveStartGroup = %d\n",SaveStartGroup);
}
#endif
 
      /**********************************************/
      /* SET ID_break_CODE TO ITS DEFAULT VALUE OF  */
      /* 99, WHICH MEANS THAT NO SUCCESSFUL ATTEMPT */
      /* WAS MADE TO DECODE ANY METAR CODED GROUP   */
      /* FOR THIS PASS THROUGH THE DECODING LOOP    */
      /**********************************************/
      switch( MetarGroup ) {
         case( codename ):
            if( isCodeName( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = codename;
            MetarGroup = stnid;
            break;
         case( stnid ):
            if( isStnId( token[NDEX], Mptr, &NDEX ) ) {
               SaveStartGroup = StartGroup = stnid;
               MetarGroup = NIL1;
            }
            else {
#ifdef DEBUGZX
printf("DcdMETAR:  token[%d] = %s\n",NDEX,token[NDEX]);
#endif
               freeTokens( token );
               return 12;
            }
            break;
         case( NIL1 ):
            if( isNIL( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = NIL1;
            MetarGroup = COR1;
            break;
         case( COR1 ):
            if( isCOR( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = COR1;
            MetarGroup = obDateTime;
            break;
         case( obDateTime ):
            if( isTimeUTC( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = obDateTime;
            MetarGroup = NIL2;
            break;
         case( NIL2 ):
            if( isNIL( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = NIL2;
            MetarGroup = AUTO;
            break;
         case( AUTO ):
            if( isAUTO( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = AUTO;
            MetarGroup = COR;
            break;
         case( COR ):
            if( isCOR( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = COR;
            MetarGroup = windData;
            break;
         case( windData ):
            if( isWindData( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = windData;
            MetarGroup = MinMaxWinDir;
            break;
         case( MinMaxWinDir ):
            if( isMinMaxWinDir( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = MinMaxWinDir;
            MetarGroup = CAVOK;
            break;
         case( CAVOK ):
            if( isCAVOK( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = CAVOK;
            MetarGroup = visibility;
            break;
         case( visibility ):
            if( isVisibility( &(token[NDEX]), Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = visibility;
            MetarGroup = RVR;
            break;
         case( RVR ):
            ndex = 0;
            MetarGroup = presentWX;
 
            while (isRVR( token[NDEX], Mptr, &NDEX, ndex ) &&
                               ndex < 12 ) {
               ndex++;
               SaveStartGroup = StartGroup = RVR;
               MetarGroup = presentWX;
            }
            break;
         case( presentWX ):
            ndex = 0;
            MetarGroup = skyCond;
 
            while( isPresentWX( token[NDEX], Mptr, &NDEX,
                          &ndex ) && ndex < MAXWXSYMBOLS) {
               SaveStartGroup = StartGroup = presentWX;
               MetarGroup = PartialObscur;
            }
            break;
         case( PartialObscur ):
            if( isPartObscur( &(token[NDEX]), Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = PartialObscur;
            MetarGroup = skyCond;
            break;
         case( skyCond ):
            if( isSkyCond( &(token[NDEX]), Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = skyCond;
            MetarGroup = tempGroup;
            break;
         case( tempGroup ):
            if( isTempGroup( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = tempGroup;
            MetarGroup = altimStng;
            break;
         case( altimStng ):
            if( isAltimStng( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = altimStng;
            MetarGroup = NotIDed;
            break;
         default:
            NDEX++;
/*          MetarGroup = SaveStartGroup;   */
            MetarGroup = ResetMETARGroup( StartGroup,
                                          SaveStartGroup );
            break;
      }
    }
    else
      IS_NOT_RMKS = FALSE;
 
   }
 
 
#ifdef DEBUGZZ
if( strcmp(token[0],"OPKC") == 0 || strcmp(token[0],"TAPA") == 0 ) {
   printf("DcdMETAR:  while loop exited, Token[%d] = %s\n",
                  NDEX,token[NDEX]);
}
#endif
                                     /******************************/
                                     /* DECODE GROUPS FOUND IN THE */
                                     /*  REMARKS SECTION OF THE    */
                                     /*       METAR REPORT         */
                                     /******************************/
#ifdef PRTMETAR
printf("DCDMETAR:  Print DECODED METAR, before leaving "
       "DCDMETAR Routine, but before possible call to DcdMTRmk\n\n");
prtDMETR( Mptr );
#endif
 
   if( token[NDEX] != NULL )
      if( strcmp( token[NDEX], "RMK" ) == 0 )
         DcdMTRmk( token, Mptr );
 
#ifdef PRTMETAR
printf("DCDMETAR:  Print DECODED METAR, after possible DcdMTRmk "
       "call\n\n");
prtDMETR( Mptr );
#endif
 
                           /****************************************/
   freeTokens( token );    /* FREE THE STORAGE ALLOCATED FOR THE   */
                           /* ARRAY USED TO HOLD THE METAR REPORT  */
                           /*                GROUPS                */
                           /****************************************/
   return 0;
 
}
@@DCDMTRMK.C
#include <metar h>
 
#define SKY1_len 50
float fracPart( char * );
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isTS_LOC                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          06 May 1996                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:   Identify the input character string as a thunder-   */
/*              storm location.  If the input string is a thunder-  */
/*              storm location, then return TRUE.  Otherwise,       */
/*              return FALSE.                                       */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         string    - a pointer to a pointer to a charac-  */
/*                             ter string from a METAR report.      */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string is a thunderstorm     */
/*                        location.                                 */
/*                 FALSE - the input string is not a thunderstorm   */
/*                         location.                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isTS_LOC( char **string, Decoded_METAR *Mptr,
                           int *NDEX )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   int i;
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
   /*******************************************/
   /* COMPARE THE INPUT CHARACTER STRING WITH */
   /* VALID AUTOMATED STATION CODE TYPE.  IF  */
   /* A MATCH IS FOUND, RETURN TRUE.  OTHER-  */
   /*           WISE, RETURN FALSE            */
   /*******************************************/
 
   if( *string == NULL )
      return FALSE;
 
   i = 0;
 
   if( strcmp( *string, "TS") != 0 )
      return FALSE;
   else {
      string++;
 
      if( *string == NULL )
         return FALSE;
 
      if(    strcmp(*string,"N")  == 0  ||
             strcmp(*string,"NE") == 0  ||
             strcmp(*string,"NW") == 0  ||
             strcmp(*string,"S")  == 0  ||
             strcmp(*string,"SE") == 0  ||
             strcmp(*string,"SW") == 0  ||
             strcmp(*string,"E")  == 0  ||
             strcmp(*string,"W")  == 0   ) {
         strcpy( Mptr->TS_LOC, *string );
         (*NDEX)++;
         (*NDEX)++;
         string++;
 
         if( *string == NULL )
            return TRUE;
 
         if( strcmp( *string, "MOV" ) == 0 ) {
            string++;
 
            if( *string == NULL ) {
               (*NDEX)++;
               return TRUE;
            }
 
            if(    strcmp(*string,"N")  == 0  ||
                   strcmp(*string,"NE") == 0  ||
                   strcmp(*string,"NW") == 0  ||
                   strcmp(*string,"S")  == 0  ||
                   strcmp(*string,"SE") == 0  ||
                   strcmp(*string,"SW") == 0  ||
                   strcmp(*string,"E")  == 0  ||
                   strcmp(*string,"W")  == 0   ) {
               strcpy( Mptr->TS_MOVMNT, *string );
               (*NDEX)++;
               (*NDEX)++;
               string++;
               return TRUE;
            }
         }
         else
            return TRUE;
 
      }
      else
         return FALSE;
 
   }
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isDVR                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isDVR( char *token, Decoded_METAR *Mptr, int *NDEX )
{
   char *slashPtr, *FT_ptr;
   char *vPtr;
   int length;
 
   if( token == NULL )
      return FALSE;
 
   if( (length = strlen( token )) < 4 )
      return FALSE;
 
   if( strncmp( token, "DVR", 3 ) != 0 )
      return FALSE;
 
   if( *(slashPtr = token+3) != '/' ) {
      (*NDEX)++;
      return FALSE;
   }
 
   if( strcmp(token+(strlen(token)-2),"FT") != 0 )
      return FALSE;
   else
      FT_ptr = token + (strlen(token)-2);
 
   if( strchr(slashPtr+1, 'P' ) != NULL )
      Mptr->DVR.above_max_DVR = TRUE;
 
   if( strchr(slashPtr+1, 'M' ) != NULL )
      Mptr->DVR.below_min_DVR = TRUE;
 
 
   if( (vPtr = strchr(slashPtr, 'V' )) != NULL )
   {
      Mptr->DVR.vrbl_visRange = TRUE;
      Mptr->DVR.Min_visRange = antoi(slashPtr+1,
                              (vPtr-(slashPtr+1)) );
      Mptr->DVR.Max_visRange = antoi(vPtr+1,
                              (FT_ptr - (vPtr+1)) );
      (*NDEX)++;
      return TRUE;
   }
   else
   {
      if( Mptr->DVR.below_min_DVR ||
          Mptr->DVR.above_max_DVR    )
         Mptr->DVR.visRange = antoi(slashPtr+2,
                           (FT_ptr - (slashPtr+2)) );
      else
         Mptr->DVR.visRange = antoi(slashPtr+1,
                           (FT_ptr - (slashPtr+1)) );
 
      (*NDEX)++;
      return TRUE;
   }
 
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isRADAT                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          07 Nov 1996                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:   Determines whether or not the input string is       */
/*              the 'RADAT' group elevation indicator.  If it is,   */
/*              then skip past the 'RADAT' indicator and also the   */
/*              next group which is the RADAT elevation informa-    */
/*              tion.                                               */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         string - the address of a pointer to a charac-   */
/*                          ter string that may or may not be the   */
/*                          RADAT group.                            */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if a RADAT group is found.                */
/*                 FALSE - if no RADAT group is found.              */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isRADAT( char **string, Decoded_METAR *Mptr,
                             int *NDEX )
{
   if( strcmp( *string, "RADAT" ) != 0 )
      return FALSE;
   else {
 
      (*NDEX)++;
      (++string);
 
      if( *string == NULL )
         return TRUE;
      else {
         (*NDEX)++;
         (++string);
 
         return TRUE;
      }
 
   }
 
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isTornadicActiv                                  */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:   Determines whether or not the input character       */
/*              string is signals the beginning of TORNADIC         */
/*              ACTIVITY data.  If it is, then interrogate subse-   */
/*              quent report groups for time, location, and movement*/
/*              of tornado.  Return TRUE, if TORNADIC ACTIVITY is   */
/*              found.  Otherwise, return FALSE.                    */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         string - the address of a pointer to a charac-   */
/*                          ter string that may or may not signal   */
/*                          TORNADIC ACTIVITY.                      */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if TORNADIC ACTIVITY is found.            */
/*                 FALSE - if no TORNADIC ACTIVITY is found.        */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isTornadicActiv( char **string, Decoded_METAR *Mptr,
                             int *NDEX )
{
   int saveNdex,
       TornadicTime;
   bool Completion_flag;
   char *B_stringPtr,
        *E_stringPtr;
 
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
 
   saveNdex = *NDEX;
 
   B_stringPtr = NULL;
   E_stringPtr = NULL;
 
   if( *string == NULL )
      return FALSE;
 
   if( !( strcmp(*string, "TORNADO")         == 0 ||
          strcmp(*string, "TORNADOS")        == 0 ||
          strcmp(*string, "TORNADOES")       == 0 ||
          strcmp(*string, "WATERSPOUT")      == 0 ||
          strcmp(*string, "WATERSPOUTS")     == 0 ||
          strcmp(*string, "FUNNEL")     == 0  ) )
         return FALSE;
   else {
      if( strcmp(*string, "FUNNEL") == 0 ) {
         (++string);
 
         if( *string == NULL )
            return FALSE;
 
         if( !(strcmp(*string,"CLOUD") == 0 ||
               strcmp(*string,"CLOUDS") == 0 ) ) {
            (*NDEX)++;
            return FALSE;
         }
         else
               strcpy(Mptr->TornadicType,"FUNNEL CLOUD");
      }
      else {
         strcpy(Mptr->TornadicType, *string);
         (*NDEX)++;
         (++string);
      }
 
      Completion_flag = FALSE;
 
      if( *string == NULL )
         return FALSE;
 
      while( !Completion_flag ) {
 
/*       printf("isTornadicActivity:  current *string = %s\n",
                        *string);    */
 
         if( *(*string) =='B' || *(*string) == 'E') {
            if( *(*string) == 'B' ) {
               B_stringPtr = *string;
               E_stringPtr = strchr((*string)+1,'E');
            }
            else {
               B_stringPtr = strchr((*string)+1,'B');
               E_stringPtr = *string;
            }
/*
         if( B_stringPtr != NULL )
            printf("isTornadicActivity:  B_stringPtr = %x\n",
                        B_stringPtr);
         else
            printf("isTornadicActivity:  B_stringPtr = NULL\n");
 
         if( E_stringPtr != NULL )
            printf("isTornadicActivity:  E_stringPtr = %x\n",
                        E_stringPtr);
         else
            printf("isTornadicActivity:  E_stringPtr = NULL\n");
*/
            if( B_stringPtr != NULL && E_stringPtr == NULL ) {
               if( nisdigit((*string)+1, strlen((*string)+1)) &&
                     strlen((*string)+1) <= 4 ) {
                  TornadicTime = antoi((*string)+1,
                                      strlen((*string)+1));
                  if( TornadicTime > 99 ) {
                     Mptr->BTornadicHour = TornadicTime / 100;
                     Mptr->BTornadicMinute = TornadicTime % 100;
                     (*NDEX)++;
                     (++string);
                  }
                  else {
                     Mptr->BTornadicHour = TornadicTime;
                     (*NDEX)++;
                     (++string);
                  }
               }
               else {
                  (*NDEX)++;
                  (++string);
               }
            }
            else if( B_stringPtr == NULL && E_stringPtr != NULL ) {
               if( nisdigit((*string)+1,strlen((*string)+1)) &&
                        strlen((*string)+1) <= 4 ) {
                  TornadicTime = antoi((*string)+1,
                                     strlen((*string)+1));
                  if( TornadicTime > 99 ) {
                     Mptr->ETornadicHour = TornadicTime / 100;
                     Mptr->ETornadicMinute = TornadicTime % 100;
                     (*NDEX)++;
                     (++string);
                  }
                  else {
                     Mptr->ETornadicHour = TornadicTime;
                     (*NDEX)++;
                     (++string);
                  }
               }
               else {
                  (*NDEX)++;
                  (++string);
               }
            }
            else {
/*          printf("isTornadicActivity:  B_stringPtr != NULL"
                   " and E_stringPtr != NULL\n");  */
               if( nisdigit((B_stringPtr+1),(E_stringPtr -
                                     (B_stringPtr+1)))) {
                  TornadicTime = antoi(( B_stringPtr+1),
                                     (E_stringPtr-(B_stringPtr+1)));
                  if( TornadicTime > 99 ) {
                     Mptr->BTornadicHour = TornadicTime / 100;
                     Mptr->BTornadicMinute = TornadicTime % 100;
                     (*NDEX)++;
                     (++string);
                  }
                  else {
                     Mptr->BTornadicHour = TornadicTime;
                     (*NDEX)++;
                     (++string);
                  }
 
                  TornadicTime = antoi(( E_stringPtr+1),
                                        strlen(E_stringPtr+1));
 
                  if( TornadicTime > 99 ) {
                     Mptr->ETornadicHour = TornadicTime / 100;
                     Mptr->ETornadicMinute = TornadicTime % 100;
                     (*NDEX)++;
                     (++string);
                  }
                  else {
                     Mptr->ETornadicHour = TornadicTime;
                     (*NDEX)++;
                     (++string);
                  }
               }
               else {
                  (*NDEX)++;
                  (++string);
               }
            }
         }
         else if( nisdigit(*string, strlen(*string))) {
            (++string);
 
            if( *string == NULL )
               return FALSE;
 
            if(  strcmp(*string,"N")  == 0  ||
                 strcmp(*string,"NE") == 0  ||
                 strcmp(*string,"NW") == 0  ||
                 strcmp(*string,"S")  == 0  ||
                 strcmp(*string,"SE") == 0  ||
                 strcmp(*string,"SW") == 0  ||
                 strcmp(*string,"E")  == 0  ||
                 strcmp(*string,"W")  == 0   ) {
                 (--string);
                 Mptr->TornadicDistance = antoi(*string,
                                  strlen(*string));
                 (*NDEX)++;
                 (++string);
            }
            else {
               (--string);
 
               if( saveNdex == *NDEX )
                  return FALSE;
               else
                  return TRUE;
            }
 
         }
         else if(strcmp(*string,"DSNT")  == 0 ||
                 strcmp(*string,"VC")    == 0 ||
                 strcmp(*string,"VCY")   == 0 ) {
            if( strcmp(*string,"VCY") == 0 ||
                  strcmp(*string,"VC") == 0  ) {
               (++string);
 
               if( *string == NULL )
                  return FALSE;
 
               if( strcmp(*string,"STN") == 0 ){
                  strcpy(Mptr->TornadicLOC,"VC STN");
                  (*NDEX)++;
                  (*NDEX)++;
                  (++string);
               }
               else {
                  strcpy(Mptr->TornadicLOC,"VC");
                  (*NDEX)++;
               }
            }
            else {
               strcpy(Mptr->TornadicLOC,"DSNT");
               (*NDEX)++;
               (++string);
            }
         }
         else if(strcmp(*string,"N")  == 0  ||
                 strcmp(*string,"NE") == 0  ||
                 strcmp(*string,"NW") == 0  ||
                 strcmp(*string,"S")  == 0  ||
                 strcmp(*string,"SE") == 0  ||
                 strcmp(*string,"SW") == 0  ||
                 strcmp(*string,"E")  == 0  ||
                 strcmp(*string,"W")  == 0   ) {
            strcpy(Mptr->TornadicDIR, *string);
            (*NDEX)++;
            (++string);
         }
         else if( strcmp(*string, "MOV" ) == 0 ) {
            (*NDEX)++;
            (++string);
 
            if( *string == NULL )
               return FALSE;
 
            if(   strcmp(*string, "N")  == 0  ||
                  strcmp(*string, "S")  == 0  ||
                  strcmp(*string, "E")  == 0  ||
                  strcmp(*string, "W")  == 0  ||
                  strcmp(*string, "NE")  == 0 ||
                  strcmp(*string, "NW")  == 0 ||
                  strcmp(*string, "SE")  == 0 ||
                  strcmp(*string, "SW")  == 0     ) {
               strcpy( Mptr->TornadicMovDir, *string );
               (*NDEX)++;
               (++string);
 
            }
         }
         else
            Completion_flag = TRUE;
      }
 
      if( saveNdex == *NDEX )
         return FALSE;
      else
         return TRUE;
 
   }
 
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isPartObscur                                     */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:   Determine whether or not the input character string */
/*              is a partial obscuration phenomenon.  If a partial  */
/*              obscuration is found, then take the preceding group */
/*              as the obscuring phenomenon.  If a partial obscura- */
/*              tion is found, then return TRUE.  Otherwise, return */
/*              false.                                              */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         string - the address of a pointer to a group     */
/*                          in a METAR report that may or may not   */
/*                          be a partial obscuration indicator.     */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string is a partial obscura- */
/*                        tion.                                     */
/*                 FALSE - if the input string is not a partial ob- */
/*                         scuration.                               */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isPartObscur( char **string, Decoded_METAR *Mptr,
                          int ndex, int *NDEX )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   int i;
 
   static char *phenom[] = {"-DZ", "DZ", "+DZ",
   "FZDZ", "-RA", "RA", "+RA",
   "SHRA", "TSRA", "FZRA", "-SN", "SN", "+SN", "DRSN",
   "SHSN", "TSSN", "-SG", "SG", "+SG", "IC", "-PE", "PE", "+PE",
   "SHPE", "TSPE", "GR", "SHGR", "TSGR", "GS", "SHGS", "TSGS", "-GS",
   "+GS", "TS", "VCTS", "-TSRA", "TSRA", "+TSRA", "-TSSN", "TSSN",
   "+TSSN", "-TSPE", "TSPE", "+TSPE", "-TSGS", "TSGS", "+TSGS",
   "VCSH", "-SHRA", "+SHRA", "-SHSN", "+SHSN", "-SHPE", "+SHPE",
   "-SHGS", "+SHGS", "-FZDZ", "+FZDZ", "-FZRA", "+FZRA", "FZFG",
   "+FZFG", "BR", "FG", "VCFG", "MIFG", "PRFG", "BCFG", "FU",
   "VA", "DU", "DRDU", "BLDU", "SA", "DRSA", "BLSA", "HZ",
   "BLPY", "BLSN", "+BLSN", "VCBLSN", "BLSA", "+BLSA",
   "VCBLSA", "+BLDU", "VCBLDU", "PO", "VCPO", "SQ", "FC", "+FC",
   "VCFC", "SS", "+SS", "VCSS", "DS", "+DS", "VCDS", NULL};
 
 
#ifdef DEBUGXX
   printf("isPartObscur:  Routine Entered...\n");
   printf("isPartObscur:  *string = %s\n",*string);
   if( Mptr->PartialObscurationAmt[ndex][0] != '\0' ) {
      printf("PartialObscurationAmt = %s\n",
                &(Mptr->PartialObscurationAmt[ndex][0]));
      if( strcmp( *string, "FEW///" ) == 0 ||
          strcmp( *string, "SCT///" ) == 0 ||
          strcmp( *string, "BKN///" ) == 0 ||
          strcmp( *string, "FEW000" ) == 0 ||
          strcmp( *string, "SCT000" ) == 0 ||
          strcmp( *string, "BKN000" ) == 0   ) {
 
          --string;
         printf("isPartObscur:  Preceding group = %s\n",
                  *string);
         ++string;
      }
   }
#endif
 
   if( *string == NULL )
      return FALSE;
 
   if( strcmp( *string, "FEW///" ) == 0 ||
       strcmp( *string, "SCT///" ) == 0 ||
       strcmp( *string, "BKN///" ) == 0 ||
       strcmp( *string, "FEW000" ) == 0 ||
       strcmp( *string, "SCT000" ) == 0 ||
       strcmp( *string, "BKN000" ) == 0   ) {
      if( Mptr->PartialObscurationAmt[ndex][0] == '\0' )
      {
         (*NDEX)++;
         return FALSE;
      }
      else {
         if( strcmp( *string,
                     &(Mptr->PartialObscurationAmt[ndex][0]) ) == 0 )
         {
            --string;
 
            if( *string == NULL )
               return FALSE;
 
            i = 0;
            while( phenom[i] != NULL ) {
               if( strcmp( *string, phenom[i] ) == 0 ) {
                  strcpy(&(Mptr->PartialObscurationPhenom[ndex][0]),
                         *string);
 
                  (*NDEX)++;
                  return TRUE;
               }
               else
                  i++;
            }
 
            (*NDEX)++;
            return FALSE;
 
         }
         else {
            (*NDEX)++;
            return FALSE;
         }
 
      }
 
   }
   else
      return FALSE;
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isA0indicator                                    */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:   Identify the input character string as an automated */
/*              station code type.  If the input character string   */
/*              is an automated station code type, then return      */
/*              TRUE.  Otherwise, return FALSE.                     */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         indicator - a pointer to a character string      */
/*                             that may or may not be an ASOS       */
/*                             automated station code type.         */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string matches one of the    */
/*                        valid ASOS automated station indicators.  */
/*                 FALSE - the input string did not match one of the*/
/*                        valid ASOS automated station indicators.  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isA0indicator( char *indicator, Decoded_METAR *Mptr,
                           int *NDEX )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   char *autoFlag[] = {"A01", "A01A", "A02", "A02A", "AOA",
                       "A0A", "AO1", "AO1A", "AO2", "AO2A", NULL};
   int i;
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
   /*******************************************/
   /* COMPARE THE INPUT CHARACTER STRING WITH */
   /* VALID AUTOMATED STATION CODE TYPE.  IF  */
   /* A MATCH IS FOUND, RETURN TRUE.  OTHER-  */
   /*           WISE, RETURN FALSE            */
   /*******************************************/
 
   if( indicator == NULL )
      return FALSE;
 
   i = 0;
 
   while( autoFlag[ i ] != NULL )
   {
      if( strcmp( indicator, autoFlag[ i ]) == 0 )
      {
         (*NDEX)++;
         strcpy(Mptr->autoIndicator, indicator);
         return TRUE;
      }
      i++;
   }
 
   return FALSE;
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isPeakWind                                       */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of peak wind.                                        */
/*                                                                  */
/*                                                                  */
/*  Input:         string - the addr of a ptr to a character string */
/*                             that may or may not be the indicator */
/*                             for a peak wind data group.          */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as peak wind.                 */
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as peak wind.            */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isPeakWind( char **string, Decoded_METAR *Mptr,
                        int *NDEX )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   char buf[ 6 ];
   char *slash;
   int temp;
   bool PK_WND_FLAG;
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
 
 
   /******************************************************/
   /* IF THE CURRENT AND NEXT GROUPS ARE "PK WND", THEN  */
   /* DETERMINE WHETHER OR NOT THE GROUP THAT FOLLOWS IS */
   /* A VALID PK WND GROUP.  IF IT IS, THEN DECODE THE   */
   /* GROUP AND RETURN TRUE.  OTHERWISE, RETURN FALSE.   */
   /******************************************************/
 
   PK_WND_FLAG = TRUE;
 
   if( *string == NULL )
      return FALSE;
 
 
   if( !(strcmp(*string,"PK") == 0 ||
          strcmp(*string,"PKWND") == 0 ) )
      return FALSE;
   else
      (++string);
 
   if( *string == NULL )
      return FALSE;
 
   if( strcmp(*string,"WND") == 0 )
      (++string);
   else
      PK_WND_FLAG = FALSE;
 
   if( *string == NULL )
      return FALSE;
 
   if( (slash = strchr(*string,'/')) == NULL ) {
                              /********************************/
                              /* INVALID PEAK WIND. BUMP PAST */
                              /* PK AND WND GROUP AND RETURN  */
                              /*             FALSE.           */
                              /********************************/
      (*NDEX)++;
 
      if( PK_WND_FLAG )
         (*NDEX)++;
 
      return FALSE;
   }
   else if( strlen(*string) >= 8 && strlen(*string) <= 11 &&
             nisdigit(slash+1,strlen(slash+1)) &&
             nisdigit(*string, (slash - *string)) &&
             (slash - *string) <= 6 )
   {
      memset( buf, '\0', 4);
      strncpy( buf, *string, 3 );
      Mptr->PKWND_dir = atoi( buf );
 
      memset( buf, '\0', 4);
      strncpy( buf, *string+3, slash-(*string+3) );
      Mptr->PKWND_speed =  atoi( buf );
 
      memset( buf, '\0', 5);
      strcpy( buf, slash+1 );
      temp             =  atoi( buf );
 
      if( temp > 99 )
      {
         Mptr->PKWND_hour = atoi(buf)/100;
         Mptr->PKWND_minute = (atoi(buf)) % 100;
      }
      else
         Mptr->PKWND_minute =  atoi( buf );
                              /********************************/
                              /* VALID PEAK WIND FOUND.  BUMP */
                              /* PAST PK, WND, AND PEAK WIND  */
                              /* GROUPS AND RETURN TRUE.      */
                              /********************************/
      (*NDEX)++;
      (*NDEX)++;
 
      if( PK_WND_FLAG )
         (*NDEX)++;
 
      return TRUE;
   }
   else
      return FALSE;
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isWindShift                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of wind shift and frontal passage, if included.      */
/*                                                                  */
/*                                                                  */
/*  Input:         string - the addr of a ptr to a character string */
/*                           that may or may not be the indicator   */
/*                           for a wind shift data group.           */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as wind shift.                */
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as wind shift.           */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isWindShift( char **string, Decoded_METAR *Mptr,
                        int *NDEX)
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   int temp;
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
 
   /****************************************************/
   /* IF THE CURRENT GROUP IS "WSHFT", THEN DETERMINE  */
   /* WHETHER OR NOT THE GROUP THAT FOLLOWS IS A VALID */
   /* WSHFT GROUP.  IF IT IS, THEN DECODE THE GROUP    */
   /* GROUP AND RETURN TRUE.  OTHERWISE, RETURN FALSE. */
   /****************************************************/
 
   if( *string == NULL )
      return FALSE;
 
   if( strcmp( *string, "WSHFT" ) != 0 )
      return FALSE;
   else
      (++string);
 
   if( *string == NULL )
      return FALSE;
 
   if( nisdigit(*string,strlen(*string)) && strlen(*string) <= 4)
   {
      temp = atoi( *string );
 
      if( temp > 100 )
      {
         Mptr->WshfTime_hour = (atoi(*string))/100;
         Mptr->WshfTime_minute = (atoi(*string)) % 100;
      }
      else
         Mptr->WshfTime_minute = (atoi(*string)) % 100;
 
      (++string);
 
      if( *string == NULL )
         return FALSE;
 
 
      if( **string == '\0') {
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else if( strcmp( *string, "FROPA") == 0 )
      {
         Mptr->Wshft_FROPA = TRUE;
                              /********************************/
                              /* VALID WIND SHIFT FOUND. BUMP */
                              /* PAST WSHFT, WSHFT GROUP, AND */
                              /* FROPA GROUPS AND RETURN TRUE.*/
                              /********************************/
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else {
                              /********************************/
                              /* VALID WIND SHIFT FOUND. BUMP */
                              /* PAST WSHFT AND WSHFT GROUP   */
                              /*       AND RETURN TRUE.       */
                              /********************************/
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
   }
   else {
                              /**********************************/
                              /* INVALID WIND SHIFT FOUND. BUMP */
                              /* PAST WSHFT AND RETURN FALSE.   */
                              /********************************/
      (*NDEX)++;
      return FALSE;
   }
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isTowerVsby                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of tower visibility.                                 */
/*                                                                  */
/*                                                                  */
/*  Input:         string - the addr of a ptr to a character string */
/*                          that may or may not be the indicator    */
/*                          for tower visibility.                   */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as tower visibility.          */
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as tower visibility      */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isTowerVsby( char **token, Decoded_METAR *Mptr, int *NDEX)
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   char *slash;
   float T_vsby;
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
 
   /****************************************************************/
   /* IF THE CURRENT AND NEXT GROUPS ARE "TWR VIS", THEN DETERMINE */
   /* WHETHER OR NOT THE GROUP(S) THAT FOLLOWS IS(ARE) A VALID     */
   /* TOWER VISIBILITY  GROUP.  IF IT IS, THEN DECODE THE GROUP    */
   /* GROUP AND RETURN TRUE.  OTHERWISE, RETURN FALSE.             */
   /****************************************************************/
 
   if( *token == NULL )
      return FALSE;
 
   if(strcmp(*token,"TWR") != 0)
      return FALSE;
   else
      (++token);
 
   if( *token == NULL )
      return FALSE;
 
   if( strcmp(*token,"VIS") != 0) {
      (*NDEX)++;
      return FALSE;
   }
   else
      (++token);
 
   if( *token == NULL )
      return FALSE;
 
   if( nisdigit(*token,
              strlen(*token)))
   {
      Mptr->TWR_VSBY = (float) atoi(*token);
      (++token);
      if( *token != NULL )
      {
         if( (slash = strchr(*token, '/'))
                             != NULL )
         {
            if( nisdigit(slash+1,strlen(slash+1)) &&
                         nisdigit(*token,
                             (slash-*token)))
            {
               T_vsby = fracPart(*token);
               Mptr->TWR_VSBY += T_vsby;
               (*NDEX)++;
               (*NDEX)++;
               (*NDEX)++;
               (*NDEX)++;
               return TRUE;
            }
            else {
               (*NDEX)++;
               (*NDEX)++;
               (*NDEX)++;
               return TRUE;
            }
 
         }
         else {
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
         }
      }
      else {
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
 
   }
   else if( (slash = strchr(*token, '/'))
                             != NULL )
   {
      if( nisdigit(slash+1,strlen(slash+1)) &&
                         nisdigit(*token,
                             (slash-*token)))
      {
         Mptr->TWR_VSBY = fracPart(*token);
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else {
         (*NDEX)++;
         (*NDEX)++;
         return FALSE;
      }
   }
   else {
      (*NDEX)++;
      (*NDEX)++;
      return FALSE;
   }
 
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isSurfaceVsby                                    */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of surface visibility.                               */
/*                                                                  */
/*                                                                  */
/*  Input:         string - the addr of a ptr to a character string */
/*                          that may or may not be the indicator    */
/*                          for surface visibility.                 */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as surface visibility.        */
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as surface visibility.   */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isSurfaceVsby( char **token, Decoded_METAR *Mptr,
                           int *NDEX)
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   char *slash;
   float S_vsby;
 
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
 
   /****************************************************************/
   /* IF THE CURRENT AND NEXT GROUPS ARE "SFC VIS", THEN DETERMINE */
   /* WHETHER OR NOT THE GROUP(S) THAT FOLLOWS IS(ARE) A VALID     */
   /* SURFACE VISIBILITY  GROUP.  IF IT IS, THEN DECODE THE GROUP  */
   /* GROUP AND RETURN TRUE.  OTHERWISE, RETURN FALSE.             */
   /****************************************************************/
 
   if( *token == NULL )
      return FALSE;
 
   if(strcmp(*token,"SFC") != 0)
      return FALSE;
   else
      (++token);
 
   if( strcmp(*token,"VIS") != 0) {
      (*NDEX)++;
      return FALSE;
   }
   else
      (++token);
 
 
   if( *token == NULL )
      return FALSE;
 
 
   if( nisdigit(*token,
              strlen(*token)))
   {
      Mptr->SFC_VSBY = (float) atoi(*token);
      (++token);
      if( *token != NULL )
      {
         if( (slash = strchr(*token, '/'))
                             != NULL )
         {
            if( nisdigit(slash+1,strlen(slash+1)) &&
                         nisdigit(*token,
                             (slash-*token)))
            {
               S_vsby = fracPart(*token);
               Mptr->SFC_VSBY += S_vsby;
               (*NDEX)++;
               (*NDEX)++;
               (*NDEX)++;
               (*NDEX)++;
               return TRUE;
            }
            else {
               (*NDEX)++;
               (*NDEX)++;
               (*NDEX)++;
               return TRUE;
            }
 
         }
         else {
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
         }
      }
      else {
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
 
   }
   else if( (slash = strchr(*token, '/'))
                             != NULL )
   {
      if( nisdigit(slash+1,strlen(slash+1)) &&
                         nisdigit(*token,
                             (slash-*token)))
      {
         Mptr->SFC_VSBY = fracPart(*token);
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else {
         (*NDEX)++;
         (*NDEX)++;
         return FALSE;
      }
   }
   else {
      (*NDEX)++;
      (*NDEX)++;
      return FALSE;
   }
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isVariableVsby                                   */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          21 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of variable prevailing visibility.                   */
/*                                                                  */
/*                                                                  */
/*  Input:         string - the addr of a ptr to a character string */
/*                          that may or may not be the indicator    */
/*                          for variable prevailing visibility.     */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as variable prevailing vsby.  */
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as variable prevailing   */
/*                         vsby.                                    */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isVariableVsby( char **string, Decoded_METAR *Mptr,
                              int *NDEX )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   char *slash,
        *slash1,
        *slash2,
        buf[ 5 ],
        *V_char;
   float minimumVsby,
         maximumVsby;
 
 
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
 
   /***************************************************/
   /* IF THE CURRENT GROUP IS  "VIS", THEN DETERMINE  */
   /* WHETHER OR NOT THE GROUPS THAT FOLLOW ARE VALID */
   /* VARIABLE PREVAILING VSBY.  IF THEY ARE, THEN    */
   /* DECODE THE GROUPS AND RETURN TRUE.  OTHERWISE,  */
   /* RETURN FALSE.                                   */
   /***************************************************/
 
   if( *string == NULL )
      return FALSE;
 
   if( strcmp(*string, "VIS") != 0 )
      return FALSE;
   else
      (++string);
 
   if( *string == NULL )
      return FALSE;
 
   if( !((V_char = strchr(*string, 'V')) != NULL ||
         nisdigit(*string,strlen(*string))) )
      return FALSE;
   else if( nisdigit(*string,strlen(*string)) ) {
      minimumVsby = (float) atoi(*string);
      (++string);
 
      if( *string == NULL )
         return FALSE;
 
      if( (V_char = strchr(*string,'V')) == NULL )
         return FALSE;
      else {
         if( (slash = strchr(*string,'/')) == NULL )
            return FALSE;
         else {
            if( nisdigit(*string,(slash - *string)) &&
                  nisdigit(slash+1,(V_char-(slash+1))) &&
                  nisdigit(V_char+1,strlen(V_char+1)) ) {
               if( (V_char - *string) > 4 )
                  return FALSE;
               else {
                  memset(buf,'\0',5);
                  strncpy(buf,*string,(V_char - *string));
                  Mptr->minVsby = minimumVsby + fracPart(buf);
                  maximumVsby = (float) atoi(V_char+1);
               }
 
               (++string);
 
               if( *string == NULL )
                  return FALSE;
 
               if( (slash = strchr(*string,'/')) == NULL ) {
                  Mptr->maxVsby = maximumVsby;
                  (*NDEX)++;
                  (*NDEX)++;
                  (*NDEX)++;
                  return TRUE;
               }
               else if( nisdigit(*string,(slash - *string)) &&
                           nisdigit(slash+1, strlen(slash+1)) ) {
                  Mptr->maxVsby = maximumVsby + fracPart(*string);
                  (*NDEX)++;
                  (*NDEX)++;
                  (*NDEX)++;
                  (*NDEX)++;
                  return TRUE;
               }
               else {
                  Mptr->maxVsby = maximumVsby;
                  (*NDEX)++;
                  (*NDEX)++;
                  (*NDEX)++;
                  return TRUE;
               }
            }
            else
               return FALSE;
         }
      }
   }
   else {
      if( (V_char = strchr(*string,'V')) == NULL )
         return FALSE;
      if(nisdigit(*string,(V_char - *string)) &&
            nisdigit(V_char+1,strlen(V_char+1)) ) {
         Mptr->minVsby = (float) antoi(*string,(V_char - *string));
         maximumVsby = (float) atoi(V_char+1);
 
         (++string);
 
         if( *string == NULL )
            return FALSE;
 
         if( (slash = strchr(*string,'/')) == NULL ) {
            Mptr->maxVsby = maximumVsby;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
         }
         else if( nisdigit(*string, (slash - *string)) &&
                     nisdigit(slash+1,strlen(slash+1)) ) {
            Mptr->maxVsby = maximumVsby + fracPart( *string );
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
         }
         else {
            Mptr->maxVsby = maximumVsby;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
         }
      }
      else {
         if( (slash2 = strchr(V_char+1,'/')) == NULL &&
               (slash1 = strchr(*string,'/')) == NULL )
            return FALSE;
         else if( slash1 == NULL )
            return FALSE;
         else if( slash == slash2 )
            return FALSE;
         else if( nisdigit(*string,(slash1 - *string)) &&
                     nisdigit((slash1+1),(V_char-(slash1+1))) ) {
            if( (V_char - *string) > 4 )
               return FALSE;
            else {
               memset(buf,'\0',5);
               strncpy(buf,*string,(V_char - *string));
               minimumVsby = fracPart(buf);
            }
            if( slash2 == NULL) {
               if( nisdigit(V_char+1, strlen(V_char+1)) ) {
                  maximumVsby = (float) atoi(V_char+1);
 
                  (++string);
 
                  if( *string == NULL )
                     return FALSE;
 
                  if( (slash = strchr(*string,'/')) == NULL ) {
                     Mptr->minVsby = minimumVsby;
                     Mptr->maxVsby = maximumVsby;
                     (*NDEX)++;
                     (*NDEX)++;
                     return TRUE;
                  }
                  else if( nisdigit(*string,(slash-*string)) &&
                         nisdigit((slash+1),strlen(slash+1)) ) {
                     Mptr->minVsby = minimumVsby;
                     Mptr->maxVsby = maximumVsby +
                                        fracPart(*string);
                     (*NDEX)++;
                     (*NDEX)++;
                     (*NDEX)++;
                     return TRUE;
                  }
                  else{
                     Mptr->minVsby = minimumVsby;
                     Mptr->maxVsby = maximumVsby;
                     (*NDEX)++;
                     (*NDEX)++;
                     return TRUE;
                  }
               }
               else
                  return FALSE;
            }
            else {
               if( nisdigit(V_char+1,(slash2-V_char+1)) &&
                     nisdigit((slash2+1),strlen(slash2+1)) ) {
                  Mptr->minVsby = minimumVsby;
                  Mptr->maxVsby = fracPart(V_char+1);
                  (*NDEX)++;
                  (*NDEX)++;
                  return TRUE;
               }
               else
                  return FALSE;
            }
         }
      }
   }
   return FALSE;
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isVsby2ndSite                                    */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of visibility at a secondary site.                   */
/*                                                                  */
/*                                                                  */
/*  Input:         token  - the addr of a ptr to a character string */
/*                          that may or may not be the indicator    */
/*                          for visibility at a secondary site.     */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as visibility at a 2ndry site.*/
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as visibility at a 2ndry */
/*                         site.                                    */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 nisalnum                                         */
/*                 fracPart                                         */
/*                 nisdigit                                         */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isVsby2ndSite( char **token, Decoded_METAR *Mptr,
                           int *NDEX)
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   char *slash;
   float S_vsby,
         VSBY_2ndSite;
 
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
 
   /***************************************************/
   /* IF THE CURRENT GROUP IS  "VIS", THEN DETERMINE  */
   /* WHETHER OR NOT THE GROUPS THAT FOLLOW ARE VALID */
   /* VSBILITY AT A 2NDRY SITE.  IF THEY ARE, THEN    */
   /* DECODE THE GROUPS AND RETURN TRUE.  OTHERWISE,  */
   /* RETURN FALSE.                                   */
   /***************************************************/
 
   if( *token == NULL )
      return FALSE;
 
   if(strcmp(*token,"VIS") != 0)
      return FALSE;
   else
      (++token);
 
   if( *token == NULL )
      return FALSE;
 
   if( nisdigit(*token,
              strlen(*token)))
   {
      VSBY_2ndSite = (float) atoi(*token);
      (++token);
      if( *token != NULL )
      {
         if( (slash = strchr(*token, '/'))
                             != NULL )
         {
            if( nisdigit(slash+1,strlen(slash+1)) &&
                         nisdigit(*token,
                             (slash-*token)))
            {
               S_vsby = fracPart(*token);
 
               (++token);
 
               if( *token == NULL )
                  return FALSE;
 
               if( strncmp( *token, "RWY", 3 ) == 0) {
                  if( nisalnum( *token, strlen(*token) ) ) {
                     strcpy(Mptr->VSBY_2ndSite_LOC, *token);
                     Mptr->VSBY_2ndSite = VSBY_2ndSite + S_vsby;
                     (*NDEX)++;
                     (*NDEX)++;
                     (*NDEX)++;
                     (*NDEX)++;
                     return TRUE;
                  }
                  else
                     return FALSE;
               }
               else
                  return FALSE;
            }
            else {
               if( strncmp( *token, "RWY", 3 ) == 0) {
                  if( nisalnum( *token, strlen(*token) ) ) {
                     strcpy(Mptr->VSBY_2ndSite_LOC, *token);
                     Mptr->VSBY_2ndSite = VSBY_2ndSite;
                     (*NDEX)++;
                     (*NDEX)++;
                     (*NDEX)++;
                     return TRUE;
                  }
                  else
                     return FALSE;
               }
               else
                  return FALSE;
            }
 
         }
         else {
            if( strncmp( *token, "RWY", 3 ) == 0) {
               if( nisalnum( *token, strlen(*token) ) ) {
                  strcpy(Mptr->VSBY_2ndSite_LOC, *token);
                  Mptr->VSBY_2ndSite = VSBY_2ndSite;
                  (*NDEX)++;
                  (*NDEX)++;
                  (*NDEX)++;
                  return TRUE;
               }
               else
                  return TRUE;
            }
            else
               return FALSE;
         }
      }
      else {
         if( strncmp( *token, "RWY", 3 ) == 0) {
            if( nisalnum( *token, strlen(*token) ) ) {
               strcpy(Mptr->VSBY_2ndSite_LOC, *token);
               Mptr->VSBY_2ndSite = VSBY_2ndSite;
               (*NDEX)++;
               (*NDEX)++;
               (*NDEX)++;
               return TRUE;
            }
            else
               return FALSE;
         }
         else
            return FALSE;
      }
 
   }
   else if( (slash = strchr(*token, '/'))
                             != NULL )
   {
      if( nisdigit(slash+1,strlen(slash+1)) &&
                         nisdigit(*token,
                             (slash-*token)))
      {
         VSBY_2ndSite = fracPart(*token);
         if( strncmp( *(++token), "RWY", 3 ) == 0) {
            if( nisalnum( *token, strlen(*token) ) ) {
               Mptr->VSBY_2ndSite = VSBY_2ndSite;
               strcpy(Mptr->VSBY_2ndSite_LOC, *token);
               (*NDEX)++;
               (*NDEX)++;
               (*NDEX)++;
               return TRUE;
            }
            else
               return FALSE;
         }
         else
            return FALSE;
      }
      else
         return FALSE;
   }
   else
      return FALSE;
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isLTGfreq                                        */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of lightning.                                        */
/*                                                                  */
/*                                                                  */
/*  Input:        string  - the addr of a ptr to a character string */
/*                          that may or may not be the indicator    */
/*                          for lightning.                          */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as lightning.                 */
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as lightning.            */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 NONE.                                            */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 09 May 1996:  Software modified to properly      */
/*                               decode lightning types.            */
/*                                                                  */
/*                                                                  */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
bool static isLTGfreq( char **string, Decoded_METAR *Mptr, int *NDEX )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   bool LTG_FREQ_FLAG,
        LTG_TYPE_FLAG,
        LTG_LOC_FLAG,
        LTG_DIR_FLAG,
        TYPE_NOT_FOUND;
   char *temp;
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
 
 
   /***************************************************/
   /* IF THE CURRENT GROUP IS  "LTG", THEN DETERMINE  */
   /* WHETHER OR NOT THE PREVIOUS GROUP AS WELL AS    */
   /* GROUPS THAT FOLLOW ARE VALID LIGHTNING REPORT   */
   /* PARAMETERS.  IF THEY ARE, THEN DECODE THE       */
   /* GROUPS AND RETURN TRUE.  OTHERWISE, RETURN      */
   /*                   FALSE.                        */
   /***************************************************/
 
   if( *string == NULL )
      return FALSE;
 
   if( strcmp(*string,"VCTS") == 0 ) {
      Mptr->LightningVCTS = TRUE;
      (++string);
      (*NDEX)++;
      return TRUE;
   }
 
   if( *string == NULL )
      return FALSE;
 
   if( strncmp( *string, "LTG", 3 ) != 0 ) {
      return FALSE;
   }
   else {
 
      if( *string == NULL )
         return FALSE;
 
      (--string);
 
 
      LTG_FREQ_FLAG = FALSE;
                        /*-- CHECK FOR LIGHTNING FREQUENCY -----------*/
      if( strcmp( *string, "OCNL" ) == 0 ) {
         Mptr->OCNL_LTG = TRUE;
         LTG_FREQ_FLAG = TRUE;
      }
      else if( strcmp( *string, "FRQ" ) == 0 ) {
         Mptr->FRQ_LTG = TRUE;
         LTG_FREQ_FLAG = TRUE;
      }
      else if( strcmp( *string, "CONS" ) == 0 ) {
         Mptr->CNS_LTG = TRUE;
         LTG_FREQ_FLAG = TRUE;
      }
 
 
      (++string);
 
      if( *string == NULL )
         return FALSE;
 
 
      if( strcmp( *string, "LTG") == 0 ) {
         (++string);
 
         if( *string == NULL )
            return FALSE;
 
         (*NDEX)++;
 
         LTG_LOC_FLAG = FALSE;
                        /*-- CHECK FOR LIGHTNING LOCATION ------------*/
         if( strcmp( *string, "DSNT" ) == 0 ) {
            Mptr->DSNT_LTG = TRUE;
            LTG_LOC_FLAG = TRUE;
         }
         else if( strcmp( *string, "AP" ) == 0 ) {
            Mptr->AP_LTG = TRUE;
            LTG_LOC_FLAG = TRUE;
         }
         else if( strcmp( *string, "VCY" ) == 0 ||
                  strcmp( *string, "VC"  ) == 0 ) {
            Mptr->VcyStn_LTG = TRUE;
            LTG_LOC_FLAG = TRUE;
         }
         else if( strcmp( *string, "OVHD" ) == 0 ||
                  strcmp( *string, "OHD"  ) == 0 ) {
            Mptr->OVHD_LTG = TRUE;
            LTG_LOC_FLAG = TRUE;
         }
 
         if( LTG_LOC_FLAG )
            (++string);
 
         if( *string == NULL ) {
            if( LTG_LOC_FLAG )
               (*NDEX)++;
            return TRUE;
         }
 
         LTG_DIR_FLAG = FALSE;
                        /*-- CHECK FOR LIGHTNING DIRECTION -----------*/
         if( strcmp( *string, "N" ) == 0 ||
             strcmp( *string, "NE" ) == 0 ||
             strcmp( *string, "NW" ) == 0 ||
             strcmp( *string, "S" ) == 0 ||
             strcmp( *string, "SE" ) == 0 ||
             strcmp( *string, "SW" ) == 0 ||
             strcmp( *string, "E" ) == 0 ||
             strcmp( *string, "W" ) == 0    ) {
            strcpy( Mptr->LTG_DIR, *string);
            LTG_DIR_FLAG = TRUE;
         }
 
 
         if( LTG_LOC_FLAG )
            (*NDEX)++;
         if( LTG_DIR_FLAG )
            (*NDEX)++;
 
         return TRUE;
      }
      else {
 
         LTG_TYPE_FLAG = FALSE;
                        /*-- CHECK FOR LIGHTNING TYPE ----------------*/
         TYPE_NOT_FOUND = FALSE;
         temp = (*string) + 3;
         while( *temp != '\0' && !TYPE_NOT_FOUND ) {
            if( strncmp( temp, "CG", 2 ) == 0 ) {
               Mptr->CG_LTG = TRUE;
               LTG_TYPE_FLAG = TRUE;
               temp++;
               temp++;
            }
            else if( strncmp( temp, "IC", 2 ) == 0 ) {
               Mptr->IC_LTG = TRUE;
               LTG_TYPE_FLAG = TRUE;
               temp++;
               temp++;
            }
            else if( strncmp( temp, "CC", 2 ) == 0 ) {
               Mptr->CC_LTG = TRUE;
               LTG_TYPE_FLAG = TRUE;
               temp++;
               temp++;
            }
            else if( strncmp( temp, "CA", 2 ) == 0 ) {
               Mptr->CA_LTG = TRUE;
               LTG_TYPE_FLAG = TRUE;
               temp++;
               temp++;
            }
            else
               TYPE_NOT_FOUND = TRUE;
         }
 
         (++string);
 
         if( *string == NULL ) {
            (*NDEX)++;
            return TRUE;
         }
/*       else
            (*NDEX)++;   TURNED OFF 07-24-97  */
 
         LTG_LOC_FLAG = FALSE;
                        /*-- CHECK FOR LIGHTNING LOCATION ------------*/
         if( strcmp( *string, "DSNT" ) == 0 ) {
            Mptr->DSNT_LTG = TRUE;
            LTG_LOC_FLAG = TRUE;
         }
         else if( strcmp( *string, "AP" ) == 0 ) {
            Mptr->AP_LTG = TRUE;
            LTG_LOC_FLAG = TRUE;
         }
         else if( strcmp( *string, "VCY" ) == 0 ||
                  strcmp( *string, "VC"  ) == 0 ) {
            Mptr->VcyStn_LTG = TRUE;
            LTG_LOC_FLAG = TRUE;
         }
         else if( strcmp( *string, "OVHD" ) == 0 ) {
            Mptr->OVHD_LTG = TRUE;
            LTG_LOC_FLAG = TRUE;
         }
 
         if( LTG_LOC_FLAG )
            (++string);
 
         if( *string == NULL ) {
            if( LTG_LOC_FLAG )
               (*NDEX)++;
            if( LTG_TYPE_FLAG )
               (*NDEX)++;
            return TRUE;
         }
 
         LTG_DIR_FLAG = FALSE;
                        /*-- CHECK FOR LIGHTNING DIRECTION -----------*/
         if( strcmp( *string, "N" ) == 0 ||
             strcmp( *string, "NE" ) == 0 ||
             strcmp( *string, "NW" ) == 0 ||
             strcmp( *string, "S" ) == 0 ||
             strcmp( *string, "SE" ) == 0 ||
             strcmp( *string, "SW" ) == 0 ||
             strcmp( *string, "E" ) == 0 ||
             strcmp( *string, "W" ) == 0    ) {
            strcpy( Mptr->LTG_DIR, *string);
            LTG_DIR_FLAG = TRUE;
         }
 
 
         if( LTG_TYPE_FLAG )
            (*NDEX)++;
         if( LTG_LOC_FLAG )
            (*NDEX)++;
         if( LTG_DIR_FLAG )
            (*NDEX)++;
 
         if( !(LTG_TYPE_FLAG) &&     /*  Added on 02/23/98 to prevent */
             !(LTG_LOC_FLAG)  &&     /*  infinite looping when 'LTG'  */
             !(LTG_DIR_FLAG)    )    /*  is present in the input, but */
            (*NDEX)++;               /*  all other related parameters */
                                     /*  are missing or invalid       */
         return TRUE;
      }
   }
}
 
 
#pragma comment (compiler)
#pragma comment (date)
#pragma comment (timestamp)
#pragma pagesize(80)
 
#include <metar h>     /* standard header file */
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isRecentWx                                       */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             recent weather.                                      */
/*                                                                  */
/*  Input:         token  - the addr of a ptr to a character token */
/*                          that may or may not be a recent weather */
/*                          group.                                  */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the i*NDEX */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the i*NDEX   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input token (and possibly subse-  */
/*                        quent groups) are decoded as recent wx.   */
/*                 FALSE - if the input token (and possibly subse- */
/*                         quent groups) are not decoded as recent  */
/*                         wx.                                      */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isRecentWX( char **token, Decoded_METAR *Mptr,
                        int *NDEX )
{
   static char *phenom[] = {"FCB", "-DZB", "DZB", "+DZB",
   "FZDZB", "-RAB", "RAB", "+RAB",
   "SHRAB", "TSRAB", "FZRAB", "-SNB",
   "SNB", "+SNB", "DRSNB", "BLSNB",
   "SHSNB", "TSSNB", "-SGB", "SGB",
   "+SGB", "ICB", "-PEB", "PEB", "+PEB",
   "SHPEB", "TSPEB", "GRB", "SHGRB",
   "TSGRB", "GSB", "SHGSB", "TSGSB", "-GSB",
   "+GSB", "TSB", "VCTSB", "-TSRAB",
   "TSRAB", "+TSRAB", "-TSSNB", "TSSNB",
   "+TSSNB", "-TSPEB", "TSPEB", "+TSPEB",
   "-TSGSB", "TSGSB", "+TSGSB",
   "VCSHB", "-SHRAB", "+SHRAB", "-SHSNB",
   "+SHSNB", "-SHPEB", "+SHPEB",
   "-SHGSB", "+SHGSB", "-FZDZB", "+FZDZB",
   "-FZRAB", "+FZRAB", "FZFGB",
   "+FZFGB", "BRB", "FGB", "VCFGB", "MIFGB",
   "PRFGB", "BCFGB", "FUB",
   "VAB", "DUB", "DRDUB", "BLDUB", "SAB",
   "DRSAB", "BLSAB", "HZB",
   "BLPYB", "BLSNB", "+BLSNB", "VCBLSNB",
   "BLSAB", "+BLSAB",
   "VCBLSAB", "+BLDUB", "VCBLDUB", "POB",
   "VCPOB", "SQB", "FCB", "+FCB",
   "VCFCB", "SSB", "+SSB", "VCSSB", "DSB",
   "+DSB", "VCDSB",
 
 
   "FCE", "-DZE", "DZE", "+DZE",
   "FZDZE", "-RAE", "RAE", "+RAE",
   "SHRAE", "TSRAE", "FZRAE", "-SNE",
   "SNE", "+SNE", "DRSNE", "BLSNE",
   "SHSNE", "TSSNE", "-SGE", "SGE",
   "+SGE", "ICE", "-PEE", "PEE", "+PEE",
   "SHPEE", "TSPEE", "GRE", "SHGRE",
   "TSGRE", "GSE", "SHGSE", "TSGSE", "-GSE",
   "+GSE", "TSE", "VCTSE", "-TSRAE",
   "TSRAE", "+TSRAE", "-TSSNE", "TSSNE",
   "+TSSNE", "-TSPEE", "TSPEE", "+TSPEE",
   "-TSGSE", "TSGSE", "+TSGSE",
   "VCSHE", "-SHRAE", "+SHRAE", "-SHSNE",
   "+SHSNE", "-SHPEE", "+SHPEE",
   "-SHGSE", "+SHGSE", "-FZDZE", "+FZDZE",
   "-FZRAE", "+FZRAE", "FZFGE",
   "+FZFGE", "BRE", "FGE", "VCFGE", "MIFGE",
   "PRFGE", "BCFGE", "FUE",
   "VAE", "DUE", "DRDUE", "BLDUE", "SAE",
   "DRSAE", "BLSAE", "HZE",
   "BLPYE", "BLSNE", "+BLSNE", "VCBLSNE",
   "BLSAE", "+BLSAE",
   "VCBLSAE", "+BLDUE", "VCBLDUE", "POE",
   "VCPOE", "SQE", "FCE", "+FCE",
   "VCFCE", "SSE", "+SSE", "VCSSE", "DSE",
   "+DSE", "VCDSE", "4-Zs"};
 
   int i,
       beg_hour,
       beg_min,
       end_hour,
       end_min;
 
   char *temp,
        *free_temp,
        *numb_char,
        *C_char;
 
 
   if( *token == NULL )
      return FALSE;
 
 
   if( (free_temp = temp = (char *) malloc(sizeof(char) *
             (strlen(*token) + 1))) == NULL ) {
      return FALSE;
   }
   else
      strcpy(temp,*token);
 
 
 
   while ( *temp != '\0' ) {
/*
printf("isRecentWX:  JUST inside while-loop, *NDEX = %d\n",*NDEX);
printf("isRecentWX:  JUST inside while-loop, temp = %s\n",temp);
*/
      i = 0;
 
      beg_hour = beg_min = end_hour = end_min = MAXINT;
 
      while( strncmp(temp, phenom[i],strlen(phenom[i])) != 0 &&
                    strcmp(phenom[i],"4-Zs") != 0 )
         i++;
 
      if( strcmp(phenom[i],"4-Zs") != 0 ) {
 
         C_char = (strlen(phenom[i]) - 1) + temp;
         numb_char = C_char + 1;
 
         if( *numb_char == '\0')
            return FALSE;
 
         if( nisdigit(numb_char,4) && strlen(numb_char) >= 4) {
            if( *C_char == 'B' ) {
               beg_hour = antoi( numb_char, 2 );
               beg_min = antoi( numb_char+2,2 );
               temp = numb_char+4;
 
               if( *NDEX < 3 ) {
                  Mptr->ReWx[*NDEX].Bmm = beg_min;
                  Mptr->ReWx[*NDEX].Bhh = beg_hour;
               }
 
               temp = numb_char + 4;
 
               if( *(numb_char+4) == 'E' ) {
                  numb_char += 5;
                  if( nisdigit(numb_char,4) &&
                              strlen(numb_char) >= 4 ) {
                     end_hour = antoi( numb_char, 2 );
                     end_min = antoi( numb_char+2,2 );
                     temp = numb_char+4;
 
                     if( *NDEX < 3 ) {
                        Mptr->ReWx[*NDEX].Emm = end_min;
                        Mptr->ReWx[*NDEX].Ehh = end_hour;
                     }
 
                     temp = numb_char + 4;
 
                  }
                  else if( nisdigit(numb_char,2) &&
                            strlen(numb_char) >= 2 ) {
                     end_min = antoi( numb_char,2 );
 
                     if( *NDEX < 3 ) {
                        Mptr->ReWx[*NDEX].Emm = end_min;
                     }
                     temp = numb_char+2;
                  }
 
               }
 
               if( *NDEX < 3 ) {
                  strncpy(Mptr->ReWx[*NDEX].Recent_weather,
                             phenom[i], (strlen(phenom[i])-1) );
                  (*NDEX)++;
               }
               if( *temp == '\0' ) {
                  free( free_temp );
                  return TRUE;
               }
 
            }
            else {
               end_hour = antoi( numb_char, 2 );
               end_min = antoi( numb_char+2,2 );
 
               temp = numb_char + 4;
 
               if( *NDEX < 3 ) {
                  Mptr->ReWx[*NDEX].Emm = end_min;
                  Mptr->ReWx[*NDEX].Ehh = end_hour;
 
               }
 
               temp = numb_char+4;
 
               if( *(numb_char+4) == 'B' ) {
                  numb_char += 5;
                  if( nisdigit(numb_char,4) &&
                             strlen(numb_char) >= 4 ) {
 
                     beg_hour = antoi(numb_char,2);
                     beg_min  = antoi(numb_char+2,2);
                     temp = numb_char + 4;
 
                     if( *NDEX < 3 ) {
                        Mptr->ReWx[*NDEX].Bmm = beg_min;
                        Mptr->ReWx[*NDEX].Bhh = beg_hour;
 
                     }
 
                     temp = numb_char+4;
                  }
                  else if( nisdigit(numb_char,2) &&
                           strlen(numb_char) >= 2 ) {
                     beg_min = antoi( numb_char,2 );
 
                     if( *NDEX < 3 ) {
                        Mptr->ReWx[*NDEX].Bmm = beg_min;
                     }
 
                     temp = numb_char+2;
                  }
 
               }
 
               if( *NDEX < 3 ) {
                  strncpy(Mptr->ReWx[*NDEX].Recent_weather,
                          phenom[i], (strlen(phenom[i])-1) );
                  (*NDEX)++;
               }
 
               if( *temp == '\0' ) {
                  free( free_temp );
                  return TRUE;
               }
 
            }
 
         }
         else if(nisdigit(numb_char,2) && strlen(numb_char) >= 2 ) {
            if( *C_char == 'B' ) {
               beg_min = antoi( numb_char,2 );
 
               if( *NDEX < 3 ) {
                  Mptr->ReWx[*NDEX].Bmm = beg_min;
 
 
               }
 
               temp = numb_char+2;
 
               if( *(numb_char+2) == 'E' ) {
                  numb_char += 3;
                  if( nisdigit(numb_char,4) &&
                           strlen(numb_char) >= 4 ) {
                     end_hour = antoi( numb_char,2 );
                     end_min = antoi( numb_char+2,2 );
 
                     if( *NDEX < 3 ) {
                        Mptr->ReWx[*NDEX].Emm = end_min;
                        Mptr->ReWx[*NDEX].Ehh = end_hour;
 
 
                     }
 
                     temp = numb_char+4;
                  }
                  else if( nisdigit(numb_char,2) &&
                             strlen(numb_char) >= 2 ) {
                     end_min = antoi( numb_char,2 );
 
                     if( *NDEX < 3 )
                        Mptr->ReWx[*NDEX].Emm = end_min;
 
 
                     temp = numb_char+2;
                  }
               }
               if( *NDEX < 3 ) {
                  strncpy( Mptr->ReWx[*NDEX].Recent_weather,
                           phenom[i], (strlen(phenom[i])-1) );
 
                  (*NDEX)++;
               }
               if( *temp == '\0' ) {
                  free( free_temp );
                  return TRUE;
               }
            }
            else {
               end_min = antoi( numb_char, 2 );
 
               if( *NDEX < 3 )
                  Mptr->ReWx[*NDEX].Emm = end_min;
 
               temp = numb_char+2;
 
               if( *(numb_char+2) == 'B' ) {
                  numb_char += 3;
                  if( nisdigit(numb_char,4) &&
                               strlen(numb_char) >= 4 ) {
                     beg_hour = antoi( numb_char,2 );
                     beg_min = antoi( numb_char+2,2 );
 
                     if( *NDEX < 3 ) {
                        Mptr->ReWx[*NDEX].Bmm = beg_min;
                        Mptr->ReWx[*NDEX].Bhh = beg_hour;
 
                     }
 
                     temp = numb_char+4;
                  }
                  else if( nisdigit(numb_char,2) &&
                             strlen(numb_char) >= 2 ) {
                     beg_min = antoi( numb_char,2 );
 
                     if( *NDEX < 3 )
                        Mptr->ReWx[*NDEX].Bmm = beg_min;
 
 
                     temp = numb_char+2;
                  }
 
               }
               if( *NDEX < 3 ) {
                  strncpy( Mptr->ReWx[*NDEX].Recent_weather,
                           phenom[i], (strlen(phenom[i])-1) );
                  (*NDEX)++;
               }
               if( *temp == '\0' ) {
                  free( free_temp );
                  return TRUE;
               }
 
            }
 
         }
         else {
            free( free_temp );
 
            if( *NDEX > 0 && *NDEX < 3 )
               return TRUE;
            else
               return FALSE;
         }
 
      }
      else {
         free( free_temp );
         return FALSE;
      }
 
   }
 
}
 
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isVariableCIG                                    */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          21 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      isVariableCIG determines whether or not the      */
/*                 current group in combination with the next       */
/*                 one or more groups is a report of variable       */
/*                 ceiling.                                         */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*  Input:         token - a pointer to an array of METAR report    */
/*                           groups.                                */
/*                 Mptr - a pointer to a decoded_METAR structure    */
/*                 NDEX - the index value of the current METAR      */
/*                        report group array element.               */
/*                                                                  */
/*  Output:        TRUE, if the token is currently pointing to      */
/*                 METAR report group(s) that a report of vari-     */
/*                 ble ceiling.                                     */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isVariableCIG( char **token, Decoded_METAR *Mptr,
                           int *NDEX )
{
   char *V_char;
 
   if( *token == NULL )
      return FALSE;
 
   if( strcmp(*token, "CIG") != 0 )
      return FALSE;
   else
      (++token);
 
   if( *token == NULL )
      return FALSE;
 
   if( (V_char = strchr(*token,'V')) != NULL ) {
      if( nisdigit(*token, (V_char - *token)) &&
            nisdigit( V_char+1, strlen(V_char+1)) ) {
         Mptr->minCeiling = antoi(*token, (V_char - *token));
         Mptr->maxCeiling = atoi(V_char+1);
 
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
   else
      return FALSE;
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isCeil2ndSite                                    */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      isCeil2ndSite determines whether or not the      */
/*                 current group in combination with the next       */
/*                 one or more groups is a report of a ceiling      */
/*                 at a secondary site.                             */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*  Input:         token - a pointer to an array of METAR report    */
/*                           groups.                                */
/*                 Mptr - a pointer to a decoded_METAR structure    */
/*                 NDEX - the index value of the current METAR      */
/*                        report group array element.               */
/*                                                                  */
/*  Output:        TRUE, if the token is currently pointing to      */
/*                 METAR report group(s) that are reporting         */
/*                 ceiling at a secondary site.                     */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isCIG2ndSite( char **token, Decoded_METAR *Mptr,
                           int *NDEX)
{
   int CIG2ndSite;
 
   if( (*token) == NULL )
      return FALSE;
 
   if(strcmp(*token,"CIG") != 0)
      return FALSE;
   else
      (++token);
 
   if( (*token) == NULL )
      return FALSE;
 
   if( strlen(*token) != 3 )
      return FALSE;
 
   if( nisdigit(*token,3) )
   {
      CIG2ndSite = atoi(*token ) * 10;
 
      if( strncmp(*(++token),"RY",2) != 0)
         return FALSE;
      else {
         strcpy(Mptr->CIG_2ndSite_LOC, *token );
         Mptr->CIG_2ndSite_Meters = CIG2ndSite;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
   }
   else
      return FALSE;
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isPRESFR                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isPRESFR( char *string, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( string == NULL )
      return FALSE;
 
   if( strcmp(string, "PRESFR") != 0 )
      return FALSE;
   else {
      Mptr->PRESFR = TRUE;
      (*NDEX)++;
      return TRUE;
   }
 
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isPRESRR                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isPRESRR( char *string, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( string == NULL )
      return FALSE;
 
   if( strcmp(string, "PRESRR") != 0 )
      return FALSE;
   else {
      Mptr->PRESRR = TRUE;
      (*NDEX)++;
      return TRUE;
   }
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isSLP                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isSLP( char **token, Decoded_METAR *Mptr, int *NDEX )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   int pressure,
       ndex;
 
   /*************************/
   /* BEGIN BODY OF ROUTINE */
   /*************************/
 
   if( *token == NULL )
      return FALSE;
 
   if( strcmp(*token, "SLPNO") == 0 ) {
      Mptr->SLPNO = TRUE;
      (*NDEX)++;
      return TRUE;
   }
 
 
   if( strncmp(*token, "SLP", 3) != 0 )
      return FALSE;
   else
   {
      if( strncmp(*token, "SLP", 3) == 0 &&
                  strcmp(*token,"SLP") != 0 )
      {
         if( nisdigit( *token+3, 3) )
         {
            pressure = atoi(*token+3);
 
            if(pressure >= 550 )
               Mptr->SLP = ((float) pressure)/10. + 900.;
            else
               Mptr->SLP = ((float) pressure)/10. + 1000.;
            (*NDEX)++;
            return TRUE;
         }
         else
            return FALSE;
      }
      else
      {
         (++token);
 
         if( *token == NULL )
            return FALSE;
 
         if( nisdigit( *token, 3) )
         {
            pressure = atoi(*token);
 
            if(pressure >= 550 )
               Mptr->SLP = ((float) pressure)/10. + 900.;
            else
               Mptr->SLP = ((float) pressure)/10. + 1000.;
 
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
         }
         else
            return FALSE;
      }
 
   }
 
}
#pragma page(1)
static bool isSectorVsby( char **string, Decoded_METAR *Mptr,
                          int  *NDEX )
{
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   int result,
       tempstrlen = 20;
 
   float vsby;
   char  dd[3],
         temp[20],
         *slash;
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
   if( *string == NULL )
      return FALSE;
 
   memset( dd, '\0', 3 );
 
   if( strcmp(*string, "VIS") != 0 )
      return FALSE;
   else {
      ++string;
 
      if( *string == NULL )
         return FALSE;
 
      if( strncmp(*string,"NE", 2) == 0 )
         strncpy(dd,*string,2);
      else if( strncmp(*string,"SE",2) == 0 )
         strncpy(dd,*string,2);
      else if( strncmp(*string,"NW",2) == 0 )
         strncpy(dd,*string,2);
      else if( strncmp(*string,"SW",2) == 0 )
         strncpy(dd,*string,2);
      else if( strncmp(*string,"N",1) == 0 )
         strncpy(dd,*string,1);
      else if( strncmp(*string,"E",1) == 0 )
         strncpy(dd,*string,1);
      else if( strncmp(*string,"S",1) == 0 )
         strncpy(dd,*string,1);
      else if( strncmp(*string,"W",1) == 0 )
         strncpy(dd,*string,1);
      else
         return FALSE;
 
      (++string);
      if( *string == NULL )
         return FALSE;
/*
printf("DCDMTRMK result = %d\n",
                 strspn(*string,"0123456789/M"));
*/
      if( (result = strspn(*string,"0123456789/M")) == 0 )
         return FALSE;
      else if(nisdigit(*string,result) )
         vsby = antoi(*string,result);
      else if(result >= tempstrlen-1)
         return FALSE;
      else {
         memset( temp, '\0', tempstrlen );
         strncpy(temp, *string, result);
/*
printf("DCDMTRMK temp = %s\n",temp);
*/
         if( strcmp(temp, "M1/4") == 0) {
            strcpy(Mptr->SectorVsby_Dir,dd);
            Mptr->SectorVsby = 0.0;
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
         }
         if( strchr(temp,'M') != NULL )
            return FALSE;
         if( (slash = strchr(temp,'/')) == NULL )
            return FALSE;
         else if(nisdigit(temp,(slash-temp)) &&
                  nisdigit(slash+1,strlen(slash+1)) ) {
            vsby = fracPart(temp);
            if(vsby > 0.875)
               return FALSE;
            else {
               Mptr->SectorVsby = vsby;
               strcpy(Mptr->SectorVsby_Dir,dd);
               (*NDEX)++;
               (*NDEX)++;
               (*NDEX)++;
               return TRUE;
            }
 
         }
 
      }
 
 
      (++string);
      if( *string == NULL ) {
         Mptr->SectorVsby = vsby;
         strcpy(Mptr->SectorVsby_Dir,dd);
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else if( (result = strspn(*string,"0123456789/")) == 0 ) {
         Mptr->SectorVsby = vsby;
         strcpy(Mptr->SectorVsby_Dir,dd);
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else if( (slash = strchr(*string,'/')) == NULL ) {
         Mptr->SectorVsby = vsby;
         strcpy(Mptr->SectorVsby_Dir,dd);
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else {
         if( fracPart(*string) > 0.875 ) {
            Mptr->SectorVsby = vsby;
            strcpy(Mptr->SectorVsby_Dir,dd);
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
         }
         else {
            vsby += fracPart(*string);
            Mptr->SectorVsby = vsby;
            strcpy(Mptr->SectorVsby_Dir,dd);
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
         }
 
      }
 
   }
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isGR                                             */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isGR( char **string, Decoded_METAR *Mptr, int *NDEX)
{
   char *slash;
 
   if( *string == NULL )
      return FALSE;
 
   if( strcmp(*string, "GS") == 0 ) {
      Mptr->GR = TRUE;
      (*NDEX)++;
      return TRUE;
   }
 
 
   if( strcmp(*string, "GR") != 0 )
      return FALSE;
   else {
      (++string);
 
      if( *string == NULL )
         return FALSE;
 
      if( (slash = strchr( *string, '/' )) != NULL ) {
         if( strcmp( *string, "M1/4" ) == 0 ) {
            Mptr->GR_Size = 1./8.;
            Mptr->GR = TRUE;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
         }
         else if( nisdigit( *string, (slash - *string) ) &&
               nisdigit( slash+1, strlen(slash+1)) ) {
            Mptr->GR_Size = fracPart( *string );
            Mptr->GR = TRUE;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
         }
         else {
            Mptr->GR = TRUE;
            (*NDEX)++;
            return TRUE;
         }
      }
      else if( nisdigit( *string, strlen(*string) ) ) {
         Mptr->GR_Size = antoi( *string, strlen(*string) );
         Mptr->GR = TRUE;
 
         (++string);
 
         if( *string == NULL )
            return FALSE;
 
         if( (slash = strchr( *string, '/' )) != NULL ) {
            if( nisdigit( *string, (slash - *string) ) &&
                     nisdigit( slash+1, strlen(slash+1)) ) {
               Mptr->GR_Size += fracPart( *string );
               (*NDEX)++;
               (*NDEX)++;
               (*NDEX)++;
               return TRUE;
            }
            else {
               (*NDEX)++;
               (*NDEX)++;
               return TRUE;
            }
         }
         else {
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
         }
      }
      else {
         Mptr->GR = TRUE;
         (*NDEX)++;
         return TRUE;
      }
   }
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isVIRGA                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isVIRGA( char **string, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( *string == NULL )
      return FALSE;
 
   if( strcmp(*string, "VIRGA") != 0 )
      return FALSE;
   else {
      Mptr->VIRGA = TRUE;
      (*NDEX)++;
 
      (++string);
 
      if( *string == NULL )
         return FALSE;
 
 
      if( strcmp( *string, "N" ) == 0 ||
          strcmp( *string, "S" ) == 0 ||
          strcmp( *string, "E" ) == 0 ||
          strcmp( *string, "W" ) == 0 ||
          strcmp( *string, "NE" ) == 0 ||
          strcmp( *string, "NW" ) == 0 ||
          strcmp( *string, "SE" ) == 0 ||
          strcmp( *string, "SW" ) == 0    ) {
         strcpy(Mptr->VIRGA_DIR, *string);
         (*NDEX)++;
      }
      return TRUE;
   }
 
}
 
#pragma page(1)
static bool isSfcObscuration( char *string, Decoded_METAR *Mptr,
                              int *NDEX )
{
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   static char *WxSymbols[] = {"BCFG", "BLDU", "BLSA", "BLPY",
          "DRDU", "DRSA", "DRSN", "DZ", "DS", "FZFG", "FZDZ", "FZRA",
          "FG", "FC", "FU", "GS", "GR", "HZ", "IC", "MIFG",
          "PE", "PO", "RA", "SHRA", "SHSN", "SHPE", "SHGS",
          "SHGR", "SN", "SG", "SQ", "SA", "SS", "TSRA",
          "TSSN", "TSPE", "TSGS", "TSGR", "TS",
          "VCSH", "VCPO", "VCBLDU", "VCBLSA", "VCBLSN",
          "VCFG", "VCFC","VA", NULL};
   int i,
       ndex;
   char *numLoc,
        ww[12],
        *temp;
 
   bool IS_NOT_FOUND;
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
   if( string == NULL )
      return FALSE;
 
   memset( ww, '\0', sizeof(ww) );
 
   if( strlen(string) < 4 )
      return FALSE;
 
   if( strncmp(string, "-X",2 ) != 0 )
      return FALSE;
 
   if( !(nisdigit(string+(strlen(string)-1), 1)) )
      return FALSE;
   else {
      temp = string + 2;
      strncpy( ww, temp, (strlen(string)-2) );
 
      ndex = 0;
      temp = ww;
      numLoc = temp + (strlen(temp) - 1 );
 
      while( temp < numLoc && ndex < 6 ) {
         i = 0;
 
         IS_NOT_FOUND = TRUE;
 
         while( WxSymbols[i] != NULL && IS_NOT_FOUND ) {
            if( strncmp( WxSymbols[i], temp, strlen(WxSymbols[i]))
                 != 0 )
               i++;
            else
               IS_NOT_FOUND = FALSE;
         }
 
         if( WxSymbols[i] == NULL ) {
            (*NDEX)++;
            return FALSE;
         }
         else {
            strcpy(&(Mptr->SfcObscuration[ndex][0]),WxSymbols[i]);
            temp += strlen(WxSymbols[i]);
            ndex++;
         }
 
      }
 
      if( ndex > 0 ) {
         Mptr->Num8thsSkyObscured = antoi( numLoc,1 );
         (*NDEX)++;
         return TRUE;
      }
      else {
         (*NDEX)++;
         return FALSE;
      }
 
   }
 
}
 
#pragma page(1)
static bool isCeiling( char *string, Decoded_METAR *Mptr, int *NDEX )
{
 
   if( string == NULL )
      return FALSE;
 
   if( !(strncmp(string,"CIG",3) == 0 && strlen(string) >= 5) )
      return FALSE;
   else {
      if( strcmp(string, "CIGNO") == 0 ) {
         Mptr->CIGNO = TRUE;
         (*NDEX)++;
         return TRUE;
      }
      else if( strlen( string+3 ) == 3 ) {
         if( nisdigit(string+3, strlen(string+3)) &&
                    strlen(string+3) == 3 ) {
            Mptr->Ceiling = atoi(string+3) * 100;
            (*NDEX)++;
            return TRUE;
         }
         else
            return FALSE;
      }
      else if( strlen(string+3) == 4 ) {
         if( *(string+3) == 'E' && nisdigit(string+4,3) ) {
            Mptr->Estimated_Ceiling = antoi(string+4,3) * 100;
            (*NDEX)++;
            return TRUE;
         }
         else
            return FALSE;
      }
      else
         return FALSE;
 
   }
 
}
#pragma page(1)
static bool isVrbSky( char **string, Decoded_METAR *Mptr, int *NDEX )
{
   static char *cldPtr[] = {"FEW", "SCT", "BKN", "OVC", NULL };
   bool IS_NOT_FOUND;
   int i;
   char SKY1[ SKY1_len ];
 
 
   if( *string == NULL )
      return FALSE;
 
 
   memset( SKY1, '\0', SKY1_len );
   i = 0;
   IS_NOT_FOUND = TRUE;
 
   while( cldPtr[i] != NULL && IS_NOT_FOUND ) {
#ifdef DEBUGQQ
   printf("isVrbSky: *string = %s cldPtr[%d] = %s\n",
                            *string,i,cldPtr[i]);
#endif
      if( strncmp(*string, cldPtr[i], strlen(cldPtr[i])) != 0 )
         i++;
      else
         IS_NOT_FOUND = FALSE;
   }
 
   if( cldPtr[i] == NULL )
      return FALSE;
   else {
#ifdef DEBUGQQ
   printf("isVrbSky: *string = %s = cldPtr[%d] = %s\n",
                            *string,i,cldPtr[i]);
#endif
      strcpy( SKY1, cldPtr[i] );
 
      (++string);
 
      if( *string == NULL )
         return FALSE;
 
 
      if( strcmp(*string, "V") != 0 )
         return FALSE;
      else {
         (++string);
 
         if( *string == NULL )
            return FALSE;
 
         i = 0;
         IS_NOT_FOUND = TRUE;
         while( cldPtr[i] != NULL && IS_NOT_FOUND ) {
#ifdef DEBUGQQ
   printf("isVrbSky: *string = %s cldPtr[%d] = %s\n",
                            *string,i,cldPtr[i]);
#endif
            if( strncmp(*string, cldPtr[i], strlen(cldPtr[i])) != 0 )
               i++;
            else
               IS_NOT_FOUND = FALSE;
         }
 
         if( cldPtr[i] == NULL ) {
            (*NDEX)++;
            (*NDEX)++;
            return FALSE;
         }
         else {
            if(strlen(SKY1) == 6 ) {
               if( nisdigit(SKY1+3,3)) {
                  strncpy(Mptr->VrbSkyBelow,SKY1,3);
                  strcpy(Mptr->VrbSkyAbove,cldPtr[i]);
                  Mptr->VrbSkyLayerHgt = antoi(SKY1+3,3)*100;
                  (*NDEX)++;
                  (*NDEX)++;
                  (*NDEX)++;
                  return TRUE;
               }
               else {
                  (*NDEX)++;
                  (*NDEX)++;
                  (*NDEX)++;
                  return TRUE;
               }
            }
            else {
               strcpy(Mptr->VrbSkyBelow,SKY1);
               strcpy(Mptr->VrbSkyAbove,cldPtr[i]);
               (*NDEX)++;
               (*NDEX)++;
               (*NDEX)++;
               return TRUE;
            }
 
         }
 
      }
 
   }
 
}
 
#pragma page(1)
static bool isObscurAloft( char **string, Decoded_METAR *Mptr,
                           int *NDEX )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   static char *WxSymbols[] = {"BCFG", "BLDU", "BLSA", "BLPY",
          "DRDU", "DRSA", "DRSN", "DZ", "DS", "FZFG", "FZDZ", "FZRA",
          "FG", "FC", "FU", "GS", "GR", "HZ", "IC", "MIFG",
          "PE", "PO", "RA", "SHRA", "SHSN", "SHPE", "SHGS",
          "SHGR", "SN", "SG", "SQ", "SA", "SS", "TSRA",
          "TSSN", "TSPE", "TSGS", "TSGR", "TS",
          "VCSH", "VCPO", "VCBLDU", "VCBLSA", "VCBLSN",
          "VCFG", "VCFC","VA", NULL};
   int i;
   char *saveTemp,
        *temp;
 
   bool IS_NOT_FOUND;
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
   if( *string == NULL )
      return FALSE;
 
   saveTemp = temp = *string;
 
   if( *temp == '\0' )
      return FALSE;
 
   while( *temp != '\0' ) {
      i = 0;
 
      IS_NOT_FOUND = TRUE;
 
      while( WxSymbols[i] != NULL && IS_NOT_FOUND ) {
         if( strncmp(temp,WxSymbols[i],strlen(WxSymbols[i])) != 0 )
            i++;
         else
            IS_NOT_FOUND = FALSE;
      }
 
      if( WxSymbols[i] == NULL ) {
         return FALSE;
      }
      else
         temp += strlen(WxSymbols[i]);
   }
 
   (++string);
 
   if( *string == NULL )
      return FALSE;
 
   if( strlen(*string) != 6 )
      return FALSE;
   else {
      if((strncmp(*string,"FEW",3) == 0 ||
          strncmp(*string,"SCT",3) == 0 ||
          strncmp(*string,"BKN",3) == 0 ||
          strncmp(*string,"OVC",3) == 0  ) &&
                 (nisdigit(*string+3,3) &&
                  strcmp(*string+3,"000") != 0  )) {
         strcpy(Mptr->ObscurAloft,saveTemp);
         strncpy(Mptr->ObscurAloftSkyCond, *string,3);
         Mptr->ObscurAloftHgt = atoi(*string+3)*100;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else {
         (*NDEX)++;
         return TRUE;
      }
 
   }
 
}
#pragma page(1)
static bool isNOSPECI( char *string, Decoded_METAR *Mptr, int *NDEX )
{
 
   if( string == NULL )
      return FALSE;
 
   if( strcmp(string,"NOSPECI") != 0 )
      return FALSE;
   else {
      Mptr->NOSPECI = TRUE;
      (*NDEX)++;
      return TRUE;
   }
}
#pragma page(1)
static bool isLAST( char *string, Decoded_METAR *Mptr, int *NDEX )
{
 
   if( string == NULL )
      return FALSE;
 
   if( strcmp(string,"LAST") != 0 )
      return FALSE;
   else {
      Mptr->LAST = TRUE;
      (*NDEX)++;
      return TRUE;
   }
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isSynopClouds                                    */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isSynopClouds( char *token, Decoded_METAR *Mptr,
                           int *NDEX )
{
 
 
   if( token == NULL )
      return FALSE;
 
   if(strlen(token) != 5)
      return FALSE;
 
   if( *token == '8' &&
       *(token+1) == '/'  &&
       ((*(token+2) <= '9' && *(token+2) >= '0') || *(token+2) == '/')
                          &&
       ((*(token+3) <= '9' && *(token+3) >= '0') || *(token+3) == '/')
                          &&
       ((*(token+4) <= '9' && *(token+4) >= '0') || *(token+4) == '/'))
   {
      strcpy(Mptr->synoptic_cloud_type,token);
 
      Mptr->CloudLow    = *(token+2);
      Mptr->CloudMedium = *(token+3);
      Mptr->CloudHigh   = *(token+4);
 
      (*NDEX)++;
      return TRUE;
   }
   else
      return FALSE;
}
 
#pragma page(1)
static bool isSNINCR( char **string, Decoded_METAR *Mptr, int *NDEX )
{
 
   char *slash;
 
   if( *string == NULL )
      return FALSE;
 
   if( strcmp( *string, "SNINCR") != 0 )
      return FALSE;
   else {
      (++string);
 
      if( *string == NULL )
         return FALSE;
 
 
      if( (slash = strchr(*string,'/')) == NULL ) {
         (*NDEX)++;
         return FALSE;
      }
      else if( nisdigit (*string,(slash-*string)) &&
                 nisdigit(slash+1,strlen(slash+1)) ) {
         Mptr->SNINCR = antoi(*string,(slash-*string));
         Mptr->SNINCR_TotalDepth = antoi(slash+1,strlen(slash+1));
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else {
         (*NDEX)++;
         return FALSE;
      }
 
   }
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isSnowDepth                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isSnowDepth( char *token, Decoded_METAR *Mptr,
                         int *NDEX )
{
 
   if( token == NULL )
      return FALSE;
 
   if(strlen(token) != 5)
      return FALSE;
 
   if( *token == '4' &&
       *(token+1) == '/'  &&
       nisdigit( (token+2),3) )
   {
      strcpy(Mptr->snow_depth_group,token);
      Mptr->snow_depth = antoi(token+2,3);
      (*NDEX)++;
      return TRUE;
   }
   else
      return FALSE;
}
 
#pragma page(1)
static bool isWaterEquivSnow( char *string,
                               Decoded_METAR *Mptr,
                               int *NDEX )
{
 
   if( string == NULL )
      return FALSE;
 
   if( strlen(string) != 6 )
      return FALSE;
   else if( !(nisdigit(string,6)) )
      return FALSE;
   else if( strncmp(string, "933", 3) != 0 )
      return FALSE;
   else {
      Mptr->WaterEquivSnow = ((float) atoi(string+3))/10.;
      (*NDEX)++;
      return TRUE;
   }
 
}
#pragma page(1)
static bool isSunshineDur( char *string, Decoded_METAR *Mptr,
                           int *NDEX )
{
 
   if( string == NULL )
      return FALSE;
 
   if( strlen(string) != 5 )
      return FALSE;
   else if( strncmp(string, "98", 2) != 0 )
      return FALSE;
   else if(nisdigit(string+2,3)) {
      Mptr->SunshineDur = atoi(string+2);
      (*NDEX)++;
      return TRUE;
   }
   else if( strncmp(string+2, "///", 3) == 0 ) {
      Mptr->SunSensorOut = TRUE;
      (*NDEX)++;
      return TRUE;
   }
   else
      return FALSE;
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isHourlyPrecip                                   */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isHourlyPrecip( char **string, Decoded_METAR *Mptr,
                            int *NDEX)
{
 
 
   if( *string == NULL )
      return FALSE;
 
   if( !(strcmp(*string, "P") == 0 || charcmp(*string, "'P'dddd") ||
                  charcmp(*string, "'P'ddd") ) )
      return FALSE;
   else if( strcmp(*string, "P") != 0 ) {
      if( nisdigit((*string+1), strlen(*string+1)) ) {
         Mptr->hourlyPrecip = ((float)
                                 atoi(*string+1)) * 0.01;
         (*NDEX)++;
         return TRUE;
      }
      else {
         (*NDEX)++;
         return FALSE;
      }
   }
   else {
 
      (++string);
 
      if( *string == NULL )
         return FALSE;
 
 
      if( nisdigit(*string,strlen(*string)) ) {
         Mptr->hourlyPrecip =  ((float)
                        atoi(*string)) * 0.01;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else {
         (*NDEX)++;
         return FALSE;
      }
   }
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isP6Precip                                       */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isP6Precip( char *string, Decoded_METAR *Mptr,
                        int *NDEX )
{
 
   if( string == NULL )
      return FALSE;
 
   if( strlen(string) != 5 )
      return FALSE;
 
 
   if( charcmp(string,"'6'dddd") ||
         charcmp(string,"'6''/''/''/''/'") ) {
      if( strcmp(string+1, "////") == 0 ) {
         Mptr->precip_amt = (float) MAXINT;
         Mptr->Indeterminant3_6HrPrecip = TRUE;
         (*NDEX)++;
         return TRUE;
      }
      else {
         Mptr->precip_amt = ((float) atoi(string+1)) / 100;
         (*NDEX)++;
         return TRUE;
      }
   }
   else
      return FALSE;
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isP24Precip                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isP24Precip( char *string, Decoded_METAR *Mptr,
                        int *NDEX )
{
 
   if( string == NULL )
      return FALSE;
 
   if( strlen(string) != 5 )
      return FALSE;
 
   if( charcmp(string,"'7'dddd") ||
         charcmp(string,"'7''/''/''/''/'") ) {
      if( strcmp(string+1, "////") == 0 ) {
         Mptr->precip_24_amt = (float) MAXINT;
         Mptr->Indeterminant_24HrPrecip = TRUE;
         (*NDEX)++;
         return TRUE;
      }
      else {
         Mptr->precip_24_amt = ((float) atoi(string+1)) / 100.;
         (*NDEX)++;
         return TRUE;
      }
   }
   else
      return FALSE;
 
}
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isTTdTenths                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          16 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isTTdTenths( char *token, Decoded_METAR *Mptr, int *NDEX)
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   bool returnFlag = FALSE;
   float sign;
 
   if( token == NULL )
      return FALSE;
 
   if( *token != 'T' )
      return FALSE;
   else if( !(strlen(token) == 5 || strlen(token) == 9) )
      return FALSE;
   else
   {
      if( (*(token+1) == '0' || *(token+1) == '1') &&
                 nisdigit(token+2,3) )
      {
         if( *(token+1) == '0' )
            sign = 0.1;
         else
            sign = -0.1;
 
         Mptr->Temp_2_tenths = sign * ((float) antoi(token+2,3));
         returnFlag = TRUE;
      }
      else
        return FALSE;
 
      if( (*(token+5) == '0' || *(token+5) == '1') &&
                 nisdigit(token+6,3) )
      {
         if( *(token+5) == '0' )
            sign = 0.1;
         else
            sign = -0.1;
 
         Mptr->DP_Temp_2_tenths = sign * ((float) atoi(token+6));
         (*NDEX)++;
         return TRUE;
 
      }
      else
      {
         if( returnFlag )
         {
            (*NDEX)++;
            return TRUE;
         }
         else
            return FALSE;
      }
   }
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isMaxTemp                                        */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isMaxTemp(char *string, Decoded_METAR *Mptr, int *NDEX)
{
   char buf[ 6 ];
 
   if( string == NULL )
      return FALSE;
 
   if(strlen(string) != 5 )
      return FALSE;
   else if(*string == '1' && (*(string+1) == '0' ||
                              *(string+1) == '1' ||
                              *(string+1) == '/'   ) &&
          (nisdigit((string+2),3) ||
            strncmp(string+2,"///",3) == 0) )
   {
      if(nisdigit(string+2,3))
      {
         memset(buf,'\0',6);
         strncpy(buf,string+2,3);
         Mptr->maxtemp = ( (float) atoi(buf))/10.;
 
         if( *(string+1) == '1' )
            Mptr->maxtemp *= (-1.0);
 
         (*NDEX)++;
         return TRUE;
      }
      else
      {
         Mptr->maxtemp = (float) MAXINT;
         (*NDEX)++;
         return TRUE;
      }
   }
   else
      return FALSE;
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isMinTemp                                        */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isMinTemp(char *string, Decoded_METAR *Mptr, int *NDEX)
{
   char buf[ 6 ];
 
   if( string == NULL )
      return FALSE;
 
   if(strlen(string) != 5 )
      return FALSE;
   else if(*string == '2' && (*(string+1) == '0' ||
                              *(string+1) == '1' ||
                              *(string+1) == '/'   ) &&
          (nisdigit((string+2),3) ||
              strncmp(string+2,"///",3) == 0) )
   {
      if(nisdigit(string+2,3))
      {
         memset(buf,'\0',6);
         strncpy(buf,string+2,3);
         Mptr->mintemp = ( (float) atoi(buf) )/10.;
 
         if( *(string+1) == '1' )
            Mptr->mintemp *= (-1.0);
         (*NDEX)++;
         return TRUE;
      }
      else
      {
         Mptr->mintemp = (float) MAXINT;
         (*NDEX)++;
         return TRUE;
      }
   }
   else
      return FALSE;
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isT24MaxMinTemp                                  */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
static bool isT24MaxMinTemp( char *string, Decoded_METAR *Mptr,
                             int *NDEX )
{
   char buf[ 6 ];
 
   if( string == NULL )
      return FALSE;
 
   if( strlen(string) != 9 )
      return FALSE;
   else if( (*string == '4' && (*(string+1) == '0' ||
                                *(string+1) == '1' ||
                                *(string+1) == '/')     &&
             (nisdigit((string+2),3) || strncmp(string+2,"///",3)))
                              &&
             ((*(string+5) == '0' || *(string+5) == '1' ||
              *(string+5) == '/') &&
              (nisdigit((string+6),3) ||
               strncmp(string+6,"///",3) == 0 )) )
   {
      if(nisdigit(string+1,4) && (*(string+1) == '0' ||
                                  *(string+1) == '1')   )
      {
         memset(buf, '\0', 6);
         strncpy(buf, string+2, 3);
         Mptr->max24temp = ( (float) atoi( buf ) )/10.;
 
         if( *(string+1) == '1' )
            Mptr->max24temp *= -1.;
      }
      else
         Mptr->max24temp = (float) MAXINT;
 
 
      if(nisdigit(string+5,4) && (*(string+5) == '0' ||
                                  *(string+5) == '1' )  )
      {
         memset(buf, '\0', 6);
         strncpy(buf, string+6, 3);
         Mptr->min24temp = ( (float) atoi(buf) )/10.;
 
         if( *(string+5) == '1' )
            Mptr->min24temp *= -1.;
      }
      else
         Mptr->min24temp = (float) MAXINT;
 
      (*NDEX)++;
      return TRUE;
 
   }
   else
      return FALSE;
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isPtendency                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isPtendency(char *string, Decoded_METAR *Mptr, int *NDEX)
{
   char buf[ 6 ];
 
   if( string == NULL )
      return FALSE;
 
   if(strlen(string) != 5)
      return FALSE;
   else if(*string == '5' && ('0' <= *(string+1) <= '8') &&
             (nisdigit(string+2,3) || strncmp(string+2,"///",3)
                                             == 0) )
   {
      if( !(nisdigit(string+2,3)) )
      {
         memset(buf,'\0',6);
         strncpy(buf,(string+1),1);
         Mptr->char_prestndcy = atoi(buf);
         (*NDEX)++;
         return TRUE;
      }
      else
      {
         memset(buf,'\0',6);
         strncpy(buf,(string+1),1);
         Mptr->char_prestndcy = atoi(buf);
 
         Mptr->prestndcy = ((float) atoi(string+2)) * 0.1;
 
         (*NDEX)++;
         return TRUE;
      }
 
   }
   else
      return FALSE;
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isPWINO                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isPWINO( char *string, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( string == NULL )
      return FALSE;
 
 
   if( strcmp(string, "PWINO") != 0 )
      return FALSE;
   else {
      Mptr->PWINO = TRUE;
      (*NDEX)++;
      return TRUE;
   }
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isPNO                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isPNO( char *string, Decoded_METAR *Mptr, int *NDEX)
{
 
 
   if( string == NULL )
      return FALSE;
 
   if( strcmp(string, "PNO") != 0 )
      return FALSE;
   else {
      Mptr->PNO = TRUE;
      (*NDEX)++;
      return TRUE;
   }
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isRVRNO                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isRVRNO( char *string, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( string == NULL )
      return FALSE;
 
   if( strcmp(string, "RVRNO") != 0 )
      return FALSE;
   else {
      Mptr->RVRNO = TRUE;
      (*NDEX)++;
      return TRUE;
   }
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isCHINO                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isCHINO( char **string, Decoded_METAR *Mptr, int *NDEX)
{
 
 
   if( *string == NULL )
      return FALSE;
 
   if( strcmp(*string, "CHINO") != 0 )
      return FALSE;
   else
      string++;
 
   if( *string == NULL )
      return FALSE;
 
   if( strlen(*string) <= 2 ) {
      (*NDEX)++;
      return FALSE;
   }
   else {
      if( strncmp( *string, "RY", 2 ) == 0 &&
            nisdigit(*string+2,strlen(*string+2)) ) {
         Mptr->CHINO = TRUE;
         strcpy(Mptr->CHINO_LOC, *string);
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else {
         (*NDEX)++;
         return FALSE;
      }
   }
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isVISNO                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isVISNO( char **string, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( *string == NULL )
      return FALSE;
 
   if( strcmp(*string, "VISNO") != 0 )
      return FALSE;
   else
      string++;
 
   if( *string == NULL )
      return FALSE;
 
   if( strlen(*string) <= 2 ) {
      (*NDEX)++;
      return FALSE;
   }
   else {
      if( strncmp( *string, "RY", 2 ) == 0 &&
            nisdigit(*string+2,strlen(*string+2))) {
         Mptr->VISNO = TRUE;
         strcpy(Mptr->VISNO_LOC, *string);
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else {
         (*NDEX)++;
         return FALSE;
      }
   }
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isFZRANO                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isFZRANO( char *string, Decoded_METAR *Mptr, int *NDEX)
{
 
 
   if( string == NULL )
      return FALSE;
 
   if( strcmp(string, "FZRANO") != 0 )
      return FALSE;
   else {
      Mptr->FZRANO = TRUE;
      (*NDEX)++;
      return TRUE;
   }
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isTSNO                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isTSNO( char *string, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( string == NULL )
      return FALSE;
 
   if( strcmp(string, "TSNO") != 0 )
      return FALSE;
   else {
      Mptr->TSNO = TRUE;
      (*NDEX)++;
      return TRUE;
   }
 
}
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         isDollarSign                                 */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
static bool isDollarSign( char *indicator, Decoded_METAR *Mptr,
                              int *NDEX )
{
 
   if( indicator == NULL )
      return FALSE;
 
   if( strcmp(indicator,"$") != 0 )
      return FALSE;
   else
   {
      (*NDEX)++;
      Mptr->DollarSign = TRUE;
      return TRUE;
   }
}
 
#pragma page(1)
#pragma subtitle(" ")
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         DcdMTRmk                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      DcdMTRmk takes a pointer to a METAR              */
/*                 report and parses/decodes data elements from     */
/*                 the remarks section of the report.               */
/*                                                                  */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         token - the address of a pointer to a METAR      */
/*                         report character string.                 */
/*                 Mptr  - a pointer to a structure of the vari-    */
/*                         able type Decoded_METAR.                 */
/*                                                                  */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
void DcdMTRmk( char **token, Decoded_METAR *Mptr )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   int TornadicActvty = 0, A0indicator = 0,
       peakwind = 0, windshift = 0, towerVsby = 0, surfaceVsby = 0,
       variableVsby = 0, LTGfreq = 0,
       TS_LOC = 0,
       recentWX = 0, variableCIG = 0, PRESFR = 0,
       Vsby2ndSite = 0, CIG2ndSite = 0,
       PRESRR = 0, SLP = 0, PartObscur = 0,
       SectorVsby = 0, GR = 0, Virga = 0,
       SfcObscur = 0, Ceiling = 0, VrbSkyCond = 0, ObscurAloft = 0,
       NoSPECI = 0, Last = 0, SynopClouds = 0, Snincr = 0,
       SnowDepth = 0, WaterEquivSnow = 0, SunshineDur = 0,
       hourlyPrecip = 0, P6Precip = 0, P24Precip = 0,
       TTdTenths = 0, MaxTemp = 0, MinTemp = 0, T24MaxMinTemp = 0,
       Ptendency = 0, PWINO = 0,
       FZRANO = 0, TSNO = 0, maintIndicator = 0, CHINO = 0, RVRNO = 0,
       VISNO = 0, PNO = 0, DVR = 0;
 
   int  NDEX,
        ndex,
        i;
   char *slash,
        *tokenX,
        *V_char,
        *temp_token;
 
   bool extra_token,
        IS_NOT_RMKS;
 
   float T_vsby;
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
   NDEX = 0;
 
   /*************************************************/
   /* LOCATE THE START OF THE METAR REMARKS SECTION */
   /*************************************************/
 
   IS_NOT_RMKS = TRUE;
 
   while( token[ NDEX ] != NULL && IS_NOT_RMKS) {
#ifdef DEBUGZZ
   printf("DcdMTRmk:  token[%d] = %s\n",NDEX,token[NDEX]);
#endif
      if( strcmp(token[ NDEX ], "RMK") != 0 )
         NDEX++;
      else
         IS_NOT_RMKS = FALSE;
   }
 
   /***********************************************/
   /* IF THE METAR REPORT CONTAINS NO REMARKS     */
   /* SECTION, THEN RETURN TO THE CALLING ROUTINE */
   /***********************************************/
 
   if( token[ NDEX ] != NULL ) {
#ifdef DEBUGZZ
   printf("DcdMTRmk:  RMK found, token[%d] = %s\n",
                   NDEX,token[NDEX]);
#endif
      NDEX++;
#ifdef DEBUGZZ
   printf("DcdMTRmk:  Bump NDEX, token[%d] = %s\n",
                   NDEX,token[NDEX]);
#endif
   }
   else {
#ifdef DEBUGZZ
   printf("DcdMTRmk:  No RMK found.  NULL ptr encountered\n");
#endif
      return;
   }
   /*****************************************/
   /* IDENTIFY AND VALIDATE REMARKS SECTION */
   /*   DATA GROUPS FOR PARSING/DECODING    */
   /*****************************************/
 
   while(token[NDEX] != NULL) {
 
#ifdef DEBUGZZ
   printf("DcdMTRmk:  DECODE RMKS: token[%d] = %s\n",NDEX,token[NDEX]);
#endif
 
      isRADAT( &(token[NDEX]), Mptr, &NDEX );
 
      if( isTornadicActiv( &(token[NDEX]), Mptr, &NDEX ) ) {
         TornadicActvty++;
         if( TornadicActvty > 1 ) {
            memset(Mptr->TornadicType,'\0',15);
            memset(Mptr->TornadicLOC,'\0',10);
            memset(Mptr->TornadicDIR,'\0',4);
            Mptr->BTornadicHour = MAXINT;
            Mptr->BTornadicMinute = MAXINT;
            Mptr->ETornadicHour = MAXINT;
            Mptr->ETornadicMinute = MAXINT;
         }
      }
      else if( isA0indicator( token[NDEX], Mptr, &NDEX ) ) {
         A0indicator++;
         if( A0indicator > 1 )
            memset(Mptr->autoIndicator,'\0',5);
      }
      else if( isPeakWind( &(token[NDEX]), Mptr, &NDEX ) ) {
         peakwind++;
         if( peakwind > 1 ) {
            Mptr->PKWND_dir = MAXINT;
            Mptr->PKWND_speed = MAXINT;
            Mptr->PKWND_hour = MAXINT;
            Mptr->PKWND_minute = MAXINT;
         }
      }
      else if( isWindShift( &(token[NDEX]), Mptr, &NDEX ) ) {
         windshift++;
         if( windshift > 1 ) {
            Mptr->WshfTime_hour = MAXINT;
            Mptr->WshfTime_minute = MAXINT;
         }
      }
      else if( isTowerVsby( &(token[NDEX]), Mptr, &NDEX ) ) {
         towerVsby++;
         if( towerVsby > 1 )
            Mptr->TWR_VSBY = (float) MAXINT;
      }
      else if( isSurfaceVsby( &(token[NDEX]), Mptr, &NDEX ) ) {
         surfaceVsby++;
         if( surfaceVsby > 1 )
            Mptr->SFC_VSBY = (float) MAXINT;
      }
      else if( isVariableVsby( &(token[NDEX]), Mptr, &NDEX ) ) {
         variableVsby++;
         if( variableVsby > 1 ) {
            Mptr->minVsby = (float) MAXINT;
            Mptr->maxVsby = (float) MAXINT;
         }
      }
      else if( isVsby2ndSite( &(token[NDEX]), Mptr, &NDEX ) ) {
         Vsby2ndSite++;
         if( Vsby2ndSite > 1 ) {
            Mptr->VSBY_2ndSite = (float) MAXINT;
            memset(Mptr->VSBY_2ndSite_LOC,'\0',10);
         }
      }
      else if( isLTGfreq( &(token[NDEX]), Mptr, &NDEX ) ) {
         LTGfreq++;
         if( LTGfreq > 1 ) {
            Mptr->OCNL_LTG = FALSE;
            Mptr->FRQ_LTG = FALSE;
            Mptr->CNS_LTG = FALSE;
            Mptr->CG_LTG = FALSE;
            Mptr->IC_LTG = FALSE;
            Mptr->CC_LTG = FALSE;
            Mptr->CA_LTG = FALSE;
            Mptr->DSNT_LTG = FALSE;
            Mptr->OVHD_LTG = FALSE;
            Mptr->VcyStn_LTG = FALSE;
            Mptr->LightningVCTS = FALSE;
            Mptr->LightningTS = FALSE;
            memset(Mptr->LTG_DIR,'\0',3 );
         }
      }
      else if( isTS_LOC( &(token[NDEX]), Mptr, &NDEX ) ) {
         TS_LOC++;
         if( TS_LOC > 1 ) {
            memset(Mptr->TS_LOC, '\0', 3);
            memset(Mptr->TS_MOVMNT, '\0', 3);
         }
      }
      else if( isRecentWX( &(token[NDEX]), Mptr, &recentWX ) ) {
         NDEX++;
      }
      else if( isVariableCIG( &(token[NDEX]), Mptr, &NDEX ) ) {
         variableCIG++;
         if( variableCIG > 1) {
            Mptr->minCeiling = MAXINT;
            Mptr->maxCeiling = MAXINT;
         }
      }
      else if( isCIG2ndSite( &(token[NDEX]), Mptr, &NDEX ) ) {
         CIG2ndSite++;
         if( CIG2ndSite > 1) {
            Mptr->CIG_2ndSite_Meters = MAXINT;
            memset( Mptr->CIG_2ndSite_LOC, '\0', 10);
         }
      }
      else if( isPRESFR( token[NDEX], Mptr, &NDEX ) ) {
         PRESFR++;
         if( PRESFR > 1 )
            Mptr->PRESFR = FALSE;
      }
      else if( isPRESRR( token[NDEX], Mptr, &NDEX ) ) {
         PRESRR++;
         if( PRESRR > 1 )
            Mptr->PRESRR = FALSE;
      }
      else if( isSLP( &(token[NDEX]), Mptr, &NDEX ) ) {
         SLP++;
         if( SLP > 1 )
            Mptr->SLP = (float) MAXINT;
      }
      else if( isPartObscur( &(token[NDEX]), Mptr, PartObscur,
               &NDEX ) ) {
         PartObscur++;
         if( PartObscur > 2 ) {
            memset(&(Mptr->PartialObscurationAmt[0][0]), '\0', 7 );
            memset(&(Mptr->PartialObscurationPhenom[0][0]),'\0',12 );
 
            memset(&(Mptr->PartialObscurationAmt[1][0]), '\0', 7 );
            memset(&(Mptr->PartialObscurationPhenom[1][0]),'\0',12 );
         }
      }
      else if( isSectorVsby( &(token[NDEX]), Mptr, &NDEX ) ) {
         SectorVsby++;
         if( SectorVsby > 1 ) {
            Mptr->SectorVsby = (float) MAXINT;
            memset(Mptr->SectorVsby_Dir, '\0', 3);
         }
      }
      else if( isGR( &(token[NDEX]), Mptr, &NDEX ) ) {
         GR++;
         if( GR > 1 ) {
            Mptr->GR_Size = (float) MAXINT;
            Mptr->GR = FALSE;
         }
      }
      else if( isVIRGA( &(token[NDEX]), Mptr, &NDEX ) ) {
         Virga++;
         if( Virga > 1 ) {
            Mptr->VIRGA = FALSE;
            memset(Mptr->VIRGA_DIR, '\0', 3);
         }
      }
      else if( isSfcObscuration( token[NDEX], Mptr, &NDEX ) ) {
         SfcObscur++;
         if( SfcObscur > 1 ) {
            for( i = 0; i < 6; i++ ) {
               memset(&(Mptr->SfcObscuration[i][0]), '\0', 10);
               Mptr->Num8thsSkyObscured = MAXINT;
            }
         }
      }
      else if( isCeiling( token[NDEX], Mptr, &NDEX ) ) {
         Ceiling++;
         if( Ceiling > 1 ) {
            Mptr->CIGNO = FALSE;
            Mptr->Ceiling = MAXINT;
            Mptr->Estimated_Ceiling = FALSE;
         }
      }
      else if( isVrbSky( &(token[NDEX]), Mptr, &NDEX ) ) {
         VrbSkyCond++;
         if( VrbSkyCond > 1 ) {
            memset(Mptr->VrbSkyBelow, '\0', 4);
            memset(Mptr->VrbSkyAbove, '\0', 4);
            Mptr->VrbSkyLayerHgt = MAXINT;
         }
      }
      else if( isObscurAloft( &(token[NDEX]), Mptr, &NDEX ) ) {
         ObscurAloft++;
         if( ObscurAloft > 1 ) {
            Mptr->ObscurAloftHgt = MAXINT;
            memset( Mptr->ObscurAloft, '\0', 12 );
            memset( Mptr->ObscurAloftSkyCond, '\0', 12 );
         }
      }
      else if( isNOSPECI( token[NDEX], Mptr, &NDEX ) ) {
         NoSPECI++;
         if( NoSPECI > 1 )
            Mptr->NOSPECI = FALSE;
      }
      else if( isLAST( token[NDEX], Mptr, &NDEX ) ) {
         Last++;
         if( Last > 1 )
            Mptr->LAST = FALSE;
      }
      else if( isSynopClouds( token[NDEX], Mptr, &NDEX ) ) {
         SynopClouds++;
         if( SynopClouds > 1 ) {
            memset( Mptr->synoptic_cloud_type, '\0', 6 );
            Mptr->CloudLow    = '\0';
            Mptr->CloudMedium = '\0';
            Mptr->CloudHigh   = '\0';
         }
      }
      else if( isSNINCR( &(token[NDEX]), Mptr, &NDEX ) ) {
         Snincr++;
         if( Snincr > 1 ) {
            Mptr->SNINCR = MAXINT;
            Mptr->SNINCR_TotalDepth = MAXINT;
         }
      }
      else if( isSnowDepth( token[NDEX], Mptr, &NDEX ) ) {
         SnowDepth++;
         if( SnowDepth > 1 ) {
            memset( Mptr->snow_depth_group, '\0', 6 );
            Mptr->snow_depth = MAXINT;
         }
      }
      else if( isWaterEquivSnow( token[NDEX], Mptr, &NDEX ) ) {
         WaterEquivSnow++;
         if( WaterEquivSnow > 1 )
            Mptr->WaterEquivSnow = (float) MAXINT;
      }
      else if( isSunshineDur( token[NDEX], Mptr, &NDEX ) ) {
         SunshineDur++;
         if( SunshineDur > 1 ) {
            Mptr->SunshineDur = MAXINT;
            Mptr->SunSensorOut = FALSE;
         }
      }
      else if( isHourlyPrecip( &(token[NDEX]), Mptr, &NDEX ) ) {
         hourlyPrecip++;
         if( hourlyPrecip > 1 )
            Mptr->hourlyPrecip = (float) MAXINT;
      }
      else if( isP6Precip( token[NDEX], Mptr, &NDEX ) ) {
         P6Precip++;
         if( P6Precip > 1 )
            Mptr->precip_amt = (float) MAXINT;
      }
      else if( isP24Precip( token[NDEX], Mptr, &NDEX ) ) {
         P24Precip++;
         if( P24Precip > 1 )
            Mptr->precip_24_amt = (float) MAXINT;
      }
      else  if( isTTdTenths( token[NDEX], Mptr, &NDEX ) ) {
         TTdTenths++;
         if( TTdTenths > 1 ) {
            Mptr->Temp_2_tenths = (float) MAXINT;
            Mptr->DP_Temp_2_tenths = (float) MAXINT;
         }
      }
      else if( isMaxTemp( token[NDEX], Mptr, &NDEX ) ) {
         MaxTemp++;
         if( MaxTemp > 1 )
            Mptr->maxtemp = (float) MAXINT;
      }
      else if( isMinTemp( token[NDEX], Mptr, &NDEX ) ) {
         MinTemp++;
         if( MinTemp > 1 )
            Mptr->mintemp = (float) MAXINT;
      }
      else if( isT24MaxMinTemp( token[NDEX],
                                          Mptr, &NDEX ) ) {
         T24MaxMinTemp++;
         if( T24MaxMinTemp > 1 ) {
            Mptr->max24temp = (float) MAXINT;
            Mptr->min24temp = (float) MAXINT;
         }
      }
      else if( isPtendency( token[NDEX], Mptr, &NDEX ) ) {
         Ptendency++;
         if( Ptendency > 1 ) {
            Mptr->char_prestndcy = MAXINT;
            Mptr->prestndcy = (float) MAXINT;
         }
      }
      else if( isPWINO( token[NDEX], Mptr, &NDEX ) ) {
         PWINO++;
         if( PWINO > 1 )
            Mptr->PWINO = FALSE;
      }
      else if( isFZRANO( token[NDEX], Mptr, &NDEX ) ) {
         FZRANO++;
         if( FZRANO > 1 )
            Mptr->FZRANO = FALSE;
      }
      else if( isTSNO( token[NDEX], Mptr, &NDEX ) ) {
         TSNO++;
         if( TSNO > 1 )
            Mptr->TSNO = FALSE;
      }
      else if( isDollarSign( token[NDEX], Mptr, &NDEX ) ) {
         maintIndicator++;
         if( maintIndicator > 1 )
            Mptr->DollarSign = FALSE;
      }
      else if( isRVRNO( token[NDEX], Mptr, &NDEX ) ) {
         RVRNO++;
         if( RVRNO > 1 )
            Mptr->RVRNO = FALSE;
      }
      else if( isPNO( token[NDEX], Mptr, &NDEX ) ) {
         PNO++;
         if( PNO > 1 )
            Mptr->PNO = FALSE;
      }
      else if( isVISNO( &(token[NDEX]), Mptr, &NDEX ) ) {
         VISNO++;
         if( VISNO > 1 ) {
            Mptr->VISNO = FALSE;
            memset(Mptr->VISNO_LOC, '\0', 6);
         }
      }
      else if( isCHINO( &(token[NDEX]), Mptr, &NDEX ) ) {
         CHINO++;
         if( CHINO > 1 ) {
            Mptr->CHINO = FALSE;
            memset(Mptr->CHINO_LOC, '\0', 6);
         }
      }
      else if( isDVR( token[NDEX], Mptr, &NDEX ) ) {
         DVR++;
         if( DVR > 1 ) {
            Mptr->DVR.Min_visRange = MAXINT;
            Mptr->DVR.Max_visRange = MAXINT;
            Mptr->DVR.visRange = MAXINT;
            Mptr->DVR.vrbl_visRange = FALSE;
            Mptr->DVR.below_min_DVR = FALSE;
            Mptr->DVR.above_max_DVR = FALSE;
         }
      }
      else
         NDEX++;
 
   }
 
   return;
}
@@PRTDMETR.C
#include <metar h>
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         prtDMETR                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  prtDMETR    prints, in order of the ASOS METAR       */
/*             format, all non-initialized members of the structure */
/*             addressed by the Decoded_METAR pointer.              */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         Mptr - ptr to a decoded_METAR structure.         */
/*                                                                  */
/*  Output:        NONE                                             */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
void prtDMETR( Decoded_METAR *Mptr )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   int i;
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
   printf("\n\n\n/*******************************************/\n");
   printf("/*    THE DECODED METAR REPORT FOLLOWS     */\n");
   printf("/*******************************************/\n\n");
 
   if( Mptr->codeName[ 0 ] != '\0' )
      printf("REPORT CODE NAME    : %s\n",Mptr->codeName);
 
   if( Mptr->stnid[ 0 ] != '\0' )
      printf("STATION ID          : %s\n",Mptr->stnid);
 
   if( Mptr->ob_date != MAXINT )
      printf("OBSERVATION DAY     : %d\n",Mptr->ob_date);
 
   if( Mptr->ob_hour != MAXINT )
      printf("OBSERVATION HOUR    : %d\n",Mptr->ob_hour);
 
   if( Mptr->ob_minute != MAXINT )
      printf("OBSERVATION MINUTE  : %d\n",Mptr->ob_minute);
 
   if( Mptr->NIL_rpt )
      printf("NIL REPORT          : TRUE\n");
 
   if( Mptr->AUTO )
      printf("AUTO REPORT         : TRUE\n");
 
   if( Mptr->COR )
      printf("CORRECTED REPORT    : TRUE\n");
 
   if( Mptr->winData.windVRB )
      printf("WIND DIRECTION VRB  : TRUE\n");
 
   if( Mptr->winData.windDir != MAXINT )
      printf("WIND DIRECTION      : %d\n",Mptr->winData.windDir);
 
   if( Mptr->winData.windSpeed != MAXINT )
      printf("WIND SPEED          : %d\n",Mptr->winData.windSpeed);
 
   if( Mptr->winData.windGust != MAXINT )
      printf("WIND GUST           : %d\n",Mptr->winData.windGust);
 
   if( Mptr->winData.windUnits[ 0 ] != '\0' )
      printf("WIND UNITS          : %s\n",Mptr->winData.windUnits);
 
   if( Mptr->minWnDir != MAXINT )
      printf("MIN WIND DIRECTION  : %d\n",Mptr->minWnDir);
 
   if( Mptr->maxWnDir != MAXINT )
      printf("MAX WIND DIRECTION  : %d\n",Mptr->maxWnDir);
 
   if( Mptr->prevail_vsbyM != (float) MAXINT )
      printf("PREVAIL VSBY (M)    : %f\n",Mptr->prevail_vsbyM);
 
   if( Mptr->prevail_vsbyKM != (float) MAXINT )
      printf("PREVAIL VSBY (KM)   : %f\n",Mptr->prevail_vsbyKM);
 
   if( Mptr->prevail_vsbySM != (float) MAXINT )
      printf("PREVAIL VSBY (SM)   : %.3f\n",Mptr->prevail_vsbySM);
/*
   if( Mptr->charPrevailVsby[0] != '\0' )
      printf("PREVAIL VSBY (CHAR) : %s\n",Mptr->charPrevailVsby);
*/
   if( Mptr->vsby_Dir[ 0 ] != '\0' )
      printf("VISIBILITY DIRECTION: %s\n",Mptr->vsby_Dir);
 
   if( Mptr->RVRNO )
      printf("RVRNO               : TRUE\n");
 
   for ( i = 0; i < 12; i++ )
   {
      if( Mptr->RRVR[i].runway_designator[0] != '\0' )
         printf("RUNWAY DESIGNATOR   : %s\n",
                 Mptr->RRVR[i].runway_designator);
 
      if( Mptr->RRVR[i].visRange != MAXINT )
         printf("R_WAY VIS RANGE (FT): %d\n",
                 Mptr->RRVR[i].visRange);
 
      if( Mptr->RRVR[i].vrbl_visRange )
         printf("VRBL VISUAL RANGE   : TRUE\n");
 
      if( Mptr->RRVR[i].below_min_RVR )
         printf("BELOW MIN RVR       : TRUE\n");
 
      if( Mptr->RRVR[i].above_max_RVR )
         printf("ABOVE MAX RVR       : TRUE\n");
 
      if( Mptr->RRVR[i].Max_visRange != MAXINT )
         printf("MX R_WAY VISRNG (FT): %d\n",
                 Mptr->RRVR[i].Max_visRange);
 
      if( Mptr->RRVR[i].Min_visRange != MAXINT )
         printf("MN R_WAY VISRNG (FT): %d\n",
                 Mptr->RRVR[i].Min_visRange);
 
   }
 
 
   if( Mptr->DVR.visRange != MAXINT )
      printf("DISPATCH VIS RANGE  : %d\n",
              Mptr->DVR.visRange);
 
   if( Mptr->DVR.vrbl_visRange )
      printf("VRBL DISPATCH VISRNG: TRUE\n");
 
   if( Mptr->DVR.below_min_DVR )
      printf("BELOW MIN DVR       : TRUE\n");
 
   if( Mptr->DVR.above_max_DVR )
      printf("ABOVE MAX DVR       : TRUE\n");
 
   if( Mptr->DVR.Max_visRange != MAXINT )
      printf("MX DSPAT VISRNG (FT): %d\n",
              Mptr->DVR.Max_visRange);
 
   if( Mptr->DVR.Min_visRange != MAXINT )
      printf("MN DSPAT VISRNG (FT): %d\n",
              Mptr->DVR.Min_visRange);
 
 
   i = 0;
   while ( Mptr->WxObstruct[i][0] != '\0' && i < MAXWXSYMBOLS )
   {
      printf("WX/OBSTRUCT VISION  : %s\n",
         Mptr->WxObstruct[i] );
      i++;
   }
 
   if( Mptr->PartialObscurationAmt[0][0] != '\0' )
      printf("OBSCURATION AMOUNT  : %s\n",
            &(Mptr->PartialObscurationAmt[0][0]));
 
   if( Mptr->PartialObscurationPhenom[0][0] != '\0' )
      printf("OBSCURATION PHENOM  : %s\n",
            &(Mptr->PartialObscurationPhenom[0][0]));
 
 
   if( Mptr->PartialObscurationAmt[1][0] != '\0' )
      printf("OBSCURATION AMOUNT  : %s\n",
            &(Mptr->PartialObscurationAmt[1][0]));
 
   if( Mptr->PartialObscurationPhenom[1][0] != '\0' )
      printf("OBSCURATION PHENOM  : %s\n",
            &(Mptr->PartialObscurationPhenom[1][0]));
 
   i = 0;
   while ( Mptr->cldTypHgt[ i ].cloud_type[0] != '\0' &&
                     i < 6 )
   {
      if( Mptr->cldTypHgt[ i ].cloud_type[0] != '\0' )
         printf("CLOUD COVER         : %s\n",
            Mptr->cldTypHgt[ i ].cloud_type);
 
      if( Mptr->cldTypHgt[ i ].cloud_hgt_char[0] != '\0' )
         printf("CLOUD HGT (CHARAC.) : %s\n",
            Mptr->cldTypHgt[ i ].cloud_hgt_char);
 
      if( Mptr->cldTypHgt[ i ].cloud_hgt_meters != MAXINT)
         printf("CLOUD HGT (METERS)  : %d\n",
            Mptr->cldTypHgt[ i ].cloud_hgt_meters);
 
      if( Mptr->cldTypHgt[ i ].other_cld_phenom[0] != '\0' )
         printf("OTHER CLOUD PHENOM  : %s\n",
            Mptr->cldTypHgt[ i ].other_cld_phenom);
 
      i++;
 
   }
 
   if( Mptr->temp != MAXINT )
      printf("TEMP. (CELSIUS)     : %d\n", Mptr->temp);
 
   if( Mptr->dew_pt_temp != MAXINT )
      printf("D.P. TEMP. (CELSIUS): %d\n", Mptr->dew_pt_temp);
 
   if( Mptr->A_altstng )
      printf("ALTIMETER (INCHES)  : %.2f\n",
         Mptr->inches_altstng );
 
   if( Mptr->Q_altstng )
      printf("ALTIMETER (PASCALS) : %d\n",
         Mptr->hectoPasc_altstng );
 
   if( Mptr->TornadicType[0] != '\0' )
      printf("TORNADIC ACTVTY TYPE: %s\n",
         Mptr->TornadicType );
 
   if( Mptr->BTornadicHour != MAXINT )
      printf("TORN. ACTVTY BEGHOUR: %d\n",
         Mptr->BTornadicHour );
 
   if( Mptr->BTornadicMinute != MAXINT )
      printf("TORN. ACTVTY BEGMIN : %d\n",
         Mptr->BTornadicMinute );
 
   if( Mptr->ETornadicHour != MAXINT )
      printf("TORN. ACTVTY ENDHOUR: %d\n",
         Mptr->ETornadicHour );
 
   if( Mptr->ETornadicMinute != MAXINT )
      printf("TORN. ACTVTY ENDMIN : %d\n",
         Mptr->ETornadicMinute );
 
   if( Mptr->TornadicDistance != MAXINT )
      printf("TORN. DIST. FROM STN: %d\n",
         Mptr->TornadicDistance );
 
   if( Mptr->TornadicLOC[0] != '\0' )
      printf("TORNADIC LOCATION   : %s\n",
         Mptr->TornadicLOC );
 
   if( Mptr->TornadicDIR[0] != '\0' )
      printf("TORNAD. DIR FROM STN: %s\n",
         Mptr->TornadicDIR );
 
   if( Mptr->TornadicMovDir[0] != '\0' )
      printf("TORNADO DIR OF MOVM.: %s\n",
         Mptr->TornadicMovDir );
 
 
   if( Mptr->autoIndicator[0] != '\0' )
         printf("AUTO INDICATOR      : %s\n",
                          Mptr->autoIndicator);
 
   if( Mptr->PKWND_dir !=  MAXINT )
      printf("PEAK WIND DIRECTION : %d\n",Mptr->PKWND_dir);
   if( Mptr->PKWND_speed !=  MAXINT )
      printf("PEAK WIND SPEED     : %d\n",Mptr->PKWND_speed);
   if( Mptr->PKWND_hour !=  MAXINT )
      printf("PEAK WIND HOUR      : %d\n",Mptr->PKWND_hour);
   if( Mptr->PKWND_minute !=  MAXINT )
      printf("PEAK WIND MINUTE    : %d\n",Mptr->PKWND_minute);
 
   if( Mptr->WshfTime_hour != MAXINT )
      printf("HOUR OF WIND SHIFT  : %d\n",Mptr->WshfTime_hour);
   if( Mptr->WshfTime_minute != MAXINT )
      printf("MINUTE OF WIND SHIFT: %d\n",Mptr->WshfTime_minute);
   if( Mptr->Wshft_FROPA != FALSE )
      printf("FROPA ASSOC. W/WSHFT: TRUE\n");
 
   if( Mptr->TWR_VSBY != (float) MAXINT )
      printf("TOWER VISIBILITY    : %.2f\n",Mptr->TWR_VSBY);
   if( Mptr->SFC_VSBY != (float) MAXINT )
      printf("SURFACE VISIBILITY  : %.2f\n",Mptr->SFC_VSBY);
 
   if( Mptr->minVsby != (float) MAXINT )
      printf("MIN VRBL_VIS (SM)   : %.4f\n",Mptr->minVsby);
   if( Mptr->maxVsby != (float) MAXINT )
      printf("MAX VRBL_VIS (SM)   : %.4f\n",Mptr->maxVsby);
 
   if( Mptr->VSBY_2ndSite != (float) MAXINT )
      printf("VSBY_2ndSite (SM)   : %.4f\n",Mptr->VSBY_2ndSite);
   if( Mptr->VSBY_2ndSite_LOC[0] != '\0' )
      printf("VSBY_2ndSite LOC.   : %s\n",
                   Mptr->VSBY_2ndSite_LOC);
 
 
   if( Mptr->OCNL_LTG )
      printf("OCCASSIONAL LTG     : TRUE\n");
 
   if( Mptr->FRQ_LTG )
      printf("FREQUENT LIGHTNING  : TRUE\n");
 
   if( Mptr->CNS_LTG )
      printf("CONTINUOUS LTG      : TRUE\n");
 
   if( Mptr->CG_LTG )
      printf("CLOUD-GROUND LTG    : TRUE\n");
 
   if( Mptr->IC_LTG )
      printf("IN-CLOUD LIGHTNING  : TRUE\n");
 
   if( Mptr->CC_LTG )
      printf("CLD-CLD LIGHTNING   : TRUE\n");
 
   if( Mptr->CA_LTG )
      printf("CLOUD-AIR LIGHTNING : TRUE\n");
 
   if( Mptr->AP_LTG )
      printf("LIGHTNING AT AIRPORT: TRUE\n");
 
   if( Mptr->OVHD_LTG )
      printf("LIGHTNING OVERHEAD  : TRUE\n");
 
   if( Mptr->DSNT_LTG )
      printf("DISTANT LIGHTNING   : TRUE\n");
 
   if( Mptr->LightningVCTS )
      printf("L'NING W/I 5-10(ALP): TRUE\n");
 
   if( Mptr->LightningTS )
      printf("L'NING W/I 5 (ALP)  : TRUE\n");
 
   if( Mptr->VcyStn_LTG )
      printf("VCY STN LIGHTNING   : TRUE\n");
 
   if( Mptr->LTG_DIR[0] != '\0' )
      printf("DIREC. OF LIGHTNING : %s\n", Mptr->LTG_DIR);
 
 
 
   i = 0;
   while( i < 3 && Mptr->ReWx[ i ].Recent_weather[0] != '\0' )
   {
      printf("RECENT WEATHER      : %s",
                  Mptr->ReWx[i].Recent_weather);
 
      if( Mptr->ReWx[i].Bhh != MAXINT )
         printf(" BEG_hh = %d",Mptr->ReWx[i].Bhh);
      if( Mptr->ReWx[i].Bmm != MAXINT )
         printf(" BEG_mm = %d",Mptr->ReWx[i].Bmm);
 
      if( Mptr->ReWx[i].Ehh != MAXINT )
         printf(" END_hh = %d",Mptr->ReWx[i].Ehh);
      if( Mptr->ReWx[i].Emm != MAXINT )
         printf(" END_mm = %d",Mptr->ReWx[i].Emm);
 
      printf("\n");
 
      i++;
   }
 
   if( Mptr->minCeiling != MAXINT )
      printf("MIN VRBL_CIG (FT)   : %d\n",Mptr->minCeiling);
   if( Mptr->maxCeiling != MAXINT )
      printf("MAX VRBL_CIG (FT))  : %d\n",Mptr->maxCeiling);
 
   if( Mptr->CIG_2ndSite_Meters != MAXINT )
      printf("CIG2ndSite (FT)     : %d\n",Mptr->CIG_2ndSite_Meters);
   if( Mptr->CIG_2ndSite_LOC[0] != '\0' )
      printf("CIG @ 2nd Site LOC. : %s\n",Mptr->CIG_2ndSite_LOC);
 
   if( Mptr->PRESFR )
      printf("PRESFR              : TRUE\n");
   if( Mptr->PRESRR )
      printf("PRESRR              : TRUE\n");
 
   if( Mptr->SLPNO )
      printf("SLPNO               : TRUE\n");
 
   if( Mptr->SLP != (float) MAXINT )
      printf("SLP (hPa)           : %.1f\n", Mptr->SLP);
 
   if( Mptr->SectorVsby != (float) MAXINT )
      printf("SECTOR VSBY (MILES) : %.2f\n", Mptr->SectorVsby );
 
   if( Mptr->SectorVsby_Dir[ 0 ] != '\0' )
      printf("SECTOR VSBY OCTANT  : %s\n", Mptr->SectorVsby_Dir );
 
   if( Mptr->TS_LOC[ 0 ] != '\0' )
      printf("THUNDERSTORM LOCAT. : %s\n", Mptr->TS_LOC );
 
   if( Mptr->TS_MOVMNT[ 0 ] != '\0' )
      printf("THUNDERSTORM MOVMNT.: %s\n", Mptr->TS_MOVMNT);
 
   if( Mptr->GR )
      printf("GR (HAILSTONES)     : TRUE\n");
 
   if( Mptr->GR_Size != (float) MAXINT )
      printf("HLSTO SIZE (INCHES) : %.3f\n",Mptr->GR_Size);
 
   if( Mptr->VIRGA )
      printf("VIRGA               : TRUE\n");
 
   if( Mptr->VIRGA_DIR[0] != '\0' )
      printf("DIR OF VIRGA FRM STN: %s\n", Mptr->VIRGA_DIR);
 
   for( i = 0; i < 6; i++ ) {
      if( Mptr->SfcObscuration[i][0] != '\0' )
         printf("SfcObscuration      : %s\n",
                   &(Mptr->SfcObscuration[i][0]) );
   }
 
   if( Mptr->Num8thsSkyObscured != MAXINT )
      printf("8ths of SkyObscured : %d\n",Mptr->Num8thsSkyObscured);
 
   if( Mptr->CIGNO )
      printf("CIGNO               : TRUE\n");
 
   if( Mptr->Ceiling != MAXINT )
      printf("Ceiling (ft)        : %d\n",Mptr->Ceiling);
 
   if( Mptr->Estimated_Ceiling != MAXINT )
      printf("Estimated CIG (ft)  : %d\n",Mptr->Estimated_Ceiling);
 
   if( Mptr->VrbSkyBelow[0] != '\0' )
      printf("VRB SKY COND BELOW  : %s\n",Mptr->VrbSkyBelow);
 
   if( Mptr->VrbSkyAbove[0] != '\0' )
      printf("VRB SKY COND ABOVE  : %s\n",Mptr->VrbSkyAbove);
 
   if( Mptr->VrbSkyLayerHgt != MAXINT )
      printf("VRBSKY COND HGT (FT): %d\n",Mptr->VrbSkyLayerHgt);
 
   if( Mptr->ObscurAloftHgt != MAXINT )
      printf("Hgt Obscur Aloft(ft): %d\n",Mptr->ObscurAloftHgt);
 
   if( Mptr->ObscurAloft[0] != '\0' )
      printf("Obscur Phenom Aloft : %s\n",Mptr->ObscurAloft);
 
   if( Mptr->ObscurAloftSkyCond[0] != '\0' )
      printf("Obscur ALOFT SKYCOND: %s\n",Mptr->ObscurAloftSkyCond);
 
 
   if( Mptr->NOSPECI )
      printf("NOSPECI             : TRUE\n");
 
   if( Mptr->LAST )
      printf("LAST                : TRUE\n");
 
   if( Mptr->synoptic_cloud_type[ 0 ] != '\0' )
      printf("SYNOPTIC CLOUD GROUP: %s\n",Mptr->synoptic_cloud_type);
 
   if( Mptr->CloudLow != '\0' )
      printf("LOW CLOUD CODE      : %c\n",Mptr->CloudLow);
 
   if( Mptr->CloudMedium != '\0' )
      printf("MEDIUM CLOUD CODE   : %c\n",Mptr->CloudMedium);
 
   if( Mptr->CloudHigh != '\0' )
      printf("HIGH CLOUD CODE     : %c\n",Mptr->CloudHigh);
 
   if( Mptr->SNINCR != MAXINT )
      printf("SNINCR (INCHES)     : %d\n",Mptr->SNINCR);
 
   if( Mptr->SNINCR_TotalDepth != MAXINT )
      printf("SNINCR(TOT. INCHES) : %d\n",Mptr->SNINCR_TotalDepth);
 
   if( Mptr->snow_depth_group[ 0 ] != '\0' )
      printf("SNOW DEPTH GROUP    : %s\n",Mptr->snow_depth_group);
 
   if( Mptr->snow_depth != MAXINT )
      printf("SNOW DEPTH (INCHES) : %d\n",Mptr->snow_depth);
 
   if( Mptr->WaterEquivSnow != (float) MAXINT )
      printf("H2O EquivSno(inches): %.2f\n",Mptr->WaterEquivSnow);
 
   if( Mptr->SunshineDur != MAXINT )
      printf("SUNSHINE (MINUTES)  : %d\n",Mptr->SunshineDur);
 
   if( Mptr->SunSensorOut )
      printf("SUN SENSOR OUT      : TRUE\n");
 
   if( Mptr->hourlyPrecip != (float) MAXINT )
      printf("HRLY PRECIP (INCHES): %.2f\n",Mptr->hourlyPrecip);
 
   if( Mptr->precip_amt != (float) MAXINT)
      printf("3/6HR PRCIP (INCHES): %.2f\n",
         Mptr->precip_amt);
 
   if( Mptr->Indeterminant3_6HrPrecip )
      printf("INDTRMN 3/6HR PRECIP: TRUE\n");
 
   if( Mptr->precip_24_amt !=  (float) MAXINT)
      printf("24HR PRECIP (INCHES): %.2f\n",
         Mptr->precip_24_amt);
 
   if( Mptr->Indeterminant_24HrPrecip )
      printf("INDTRMN 24 HR PRECIP: TRUE\n");
 
   if( Mptr->Temp_2_tenths != (float) MAXINT )
      printf("TMP2TENTHS (CELSIUS): %.1f\n",Mptr->Temp_2_tenths);
 
   if( Mptr->DP_Temp_2_tenths != (float) MAXINT )
      printf("DPT2TENTHS (CELSIUS): %.1f\n",Mptr->DP_Temp_2_tenths);
 
   if( Mptr->maxtemp !=  (float) MAXINT)
      printf("MAX TEMP (CELSIUS)  : %.1f\n",
         Mptr->maxtemp);
 
   if( Mptr->mintemp !=  (float) MAXINT)
      printf("MIN TEMP (CELSIUS)  : %.1f\n",
         Mptr->mintemp);
 
   if( Mptr->max24temp !=  (float) MAXINT)
      printf("24HrMAXTMP (CELSIUS): %.1f\n",
         Mptr->max24temp);
 
   if( Mptr->min24temp !=  (float) MAXINT)
      printf("24HrMINTMP (CELSIUS): %.1f\n",
         Mptr->min24temp);
 
   if( Mptr->char_prestndcy != MAXINT)
      printf("CHAR PRESS TENDENCY : %d\n",
         Mptr->char_prestndcy );
 
   if( Mptr->prestndcy != (float) MAXINT)
      printf("PRES. TENDENCY (hPa): %.1f\n",
         Mptr->prestndcy );
 
   if( Mptr->PWINO )
      printf("PWINO               : TRUE\n");
 
   if( Mptr->PNO )
      printf("PNO                 : TRUE\n");
 
   if( Mptr->CHINO )
      printf("CHINO               : TRUE\n");
 
   if( Mptr->CHINO_LOC[0] != '\0' )
      printf("CHINO_LOC           : %s\n",Mptr->CHINO_LOC);
 
   if( Mptr->VISNO )
      printf("VISNO               : TRUE\n");
 
   if( Mptr->VISNO_LOC[0] != '\0' )
      printf("VISNO_LOC           : %s\n",Mptr->VISNO_LOC);
 
   if( Mptr->FZRANO )
      printf("FZRANO              : TRUE\n");
 
   if( Mptr->TSNO )
      printf("TSNO                : TRUE\n");
 
   if( Mptr->DollarSign)
      printf("DOLLAR $IGN INDCATR : TRUE\n");
 
   if( Mptr->horiz_vsby[ 0 ] != '\0' )
      printf("HORIZ VISIBILITY    : %s\n",Mptr->horiz_vsby);
 
   if( Mptr->dir_min_horiz_vsby[ 0 ] != '\0' )
      printf("DIR MIN HORIZ VSBY  : %s\n",Mptr->dir_min_horiz_vsby);
 
   if( Mptr->CAVOK )
      printf("CAVOK               : TRUE\n");
 
 
   if( Mptr->VertVsby != MAXINT )
      printf("Vert. Vsby (meters) : %d\n",
                  Mptr->VertVsby );
 
   if( Mptr->charVertVsby[0] != '\0' )
      printf("Vert. Vsby (CHAR)   : %s\n",
                  Mptr->charVertVsby );
 
   if( Mptr->QFE != MAXINT )
      printf("QFE                 : %d\n", Mptr->QFE);
 
   if( Mptr->VOLCASH )
      printf("VOLCANIC ASH        : TRUE\n");
 
   if( Mptr->min_vrbl_wind_dir != MAXINT )
      printf("MIN VRBL WIND DIR   : %d\n",Mptr->min_vrbl_wind_dir);
   if( Mptr->max_vrbl_wind_dir != MAXINT )
      printf("MAX VRBL WIND DIR   : %d\n",Mptr->max_vrbl_wind_dir);
 
 
   printf("\n\n\n");
 
 
   return;
 
}
@@FRACPART.C
#include <metar h>
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("subtitle - description                       ")
/********************************************************************/
/*                                                                  */
/*  Title:         fracPart                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          13 Jun 1995                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      Convert a character string fraction into a       */
/*                 decimal (floating point) number.                 */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         string - a pointer to a character string frac-   */
/*                          tion.                                   */
/*  Output:        A decimal (floating point) number.               */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
float fracPart( char *string )
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   char buf[ 6 ],
        *slash;
 
   float numerator,
         denominator;
 
   /*************************/
   /* START BODY OF ROUTINE */
   /*************************/
 
   slash = strchr(string, '/');
 
   memset(buf , '\0', 6);
   strncpy( buf, string, slash-string);
 
   numerator = (float) atoi(buf);
 
   memset(buf , '\0', 6);
   strcpy( buf, slash+1);
 
   denominator = (float) atoi(buf);
 
   if( denominator == 0.0 )
      return ((float) MAXINT);
   else
      return (numerator/denominator);
 
}
@@STSPACK2.C
#pragma comment (compiler)
#pragma comment (date)
#pragma comment (timestamp)
#pragma pagesize(80)
 
#include <local h>     /* standard header file */
 
#pragma page(1)
#pragma subtitle(" ")
#pragma subtitle("stspack2 - Local string test functions       ")
/********************************************************************/
/*                                                                  */
/*  Title:         stspack2                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          05 Oct 1992                                      */
/*  Programmer:    ALLAN DARLING                                    */
/*  Language:      C/2                                              */
/*                                                                  */
/*  Abstract:      The stspack2 package contains functions to       */
/*                 perform the isalnum through isxdigit functions   */
/*                 on strings.  The functions come in four forms:   */
/*                 those that test NULL delimited strings and are   */
/*                 named in the form sxxxxxxx, those that test at   */
/*                 most n characters and are named in the form      */
/*                 nxxxxxxx, those that search forward in a string  */
/*                 and are named in the form nxtyyyyy, and those    */
/*                 that search backward in a string and are named   */
/*                 in the form lstyyyyy.                            */
/*                                                                  */
/*                 The xxxxxxx is the name of the test applied to   */
/*                 each character in the string, such as isalpha,   */
/*                 thus a function to test a NULL delimited string  */
/*                 an return a nonzero value if all characters in   */
/*                 the string are digits is named sisdigit.         */
/*                                                                  */
/*                 The yyyyy is the name of the test applied to     */
/*                 characters in a string, minus the 'is' prefix.   */
/*                 Thus a function to find the next digit in a NULL */
/*                 delimited string and return a pointer to it is   */
/*                 named nxtdigit.                                  */
/*                                                                  */
/*                 The only exception to the naming rule is for the */
/*                 functions that test for hexadecimal digits.      */
/*                 These are named sisxdigi, nisxdigi, nxtxdigi,    */
/*                 and lstxdigi because of the eight character      */
/*                 function name limitation.                        */
/*                                                                  */
/*                 The nxxxxxxx class of functions will test up to  */
/*                 n characters or the first NULL character         */
/*                 encountered, whichever comes first.  For all     */
/*                 classes of functions, the string sentinal is     */
/*                 not included in the test.                        */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 isalnum, isalpha, iscntrl, isdigit, isgraph,     */
/*                 islower, isprint, ispunct, isspace, isupper,     */
/*                 isxdigit.                                        */
/*                                                                  */
/*  Input:         For sxxxxxxx class functions, a pointer to a     */
/*                 NULL delimited character string.                 */
/*                                                                  */
/*                 For nxtyyyyy class functions, a pointer to a     */
/*                 NULL delimited character string.                 */
/*                                                                  */
/*                 for nxxxxxxx class functions, a pointer to a     */
/*                 character array, and a positive, nonzero integer.*/
/*                                                                  */
/*                 for lstyyyyy class functions, a pointer to a     */
/*                 character array, and a positive, nonzero integer.*/
/*                                                                  */
/*  Output:        A nonzero value if the test is true for all      */
/*                 characters in the string, a zero value otherwise.*/
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
int nisalnum(char *s, int n) {
 
   for (; *s && n; s++, n--)
 
      if (!isalnum(*s))
         return (0);
 
   return (1);
 
} /* end nisalnum */
 
 
int nisalpha(char *s, int n) {
 
   for (; *s && n; s++, n--)
 
      if (!isalpha(*s))
         return (0);
 
   return (1);
 
} /* end nisalpha */
 
 
int niscntrl(char *s, int n) {
 
   for (; *s && n; s++, n--)
 
      if (!iscntrl(*s))
         return (0);
 
   return (1);
 
} /* end niscntrl */
 
 
int nisdigit(char *s, int n) {
 
   for (; *s && n; s++, n--)
 
      if (!isdigit(*s))
         return (0);
 
   return (1);
 
} /* end nisdigit */
 
 
int nisgraph(char *s, int n) {
 
   for (; *s && n; s++, n--)
 
      if (!isgraph(*s))
         return (0);
 
   return (1);
 
} /* end nisgraph */
 
 
int nislower(char *s, int n) {
 
   for (; *s && n; s++, n--)
 
      if (!islower(*s))
         return (0);
 
   return (1);
 
} /* end nislower */
 
 
int nisprint(char *s, int n) {
 
   for (; *s && n; s++, n--)
 
      if (!isprint(*s))
         return (0);
 
   return (1);
 
} /* end nisprint */
 
 
int nispunct(char *s, int n) {
 
   for (; *s && n; s++, n--)
 
      if (!ispunct(*s))
         return (0);
 
   return (1);
 
} /* end nispunct */
 
 
int nisspace(char *s, int n) {
 
   for (; *s && n; s++, n--)
 
      if (!isspace(*s))
         return (0);
 
   return (1);
 
} /* end nisspace */
 
 
int nisupper(char *s, int n) {
 
   for (; *s && n; s++, n--)
 
      if (!isupper(*s))
         return (0);
 
   return (1);
 
} /* end nisupper */
 
 
int nisxdigi(char *s, int n) {
 
   for (; *s && n; s++, n--)
 
      if (!isxdigit(*s))
         return (0);
 
   return (1);
 
} /* end nisxdigi */
 
#pragma page(1)
@@STSPACK3.C
#pragma comment (compiler)
#pragma comment (date)
#pragma comment (timestamp)
#pragma pagesize(80)
 
#include <local h>     /* standard header file */
 
#pragma page(1)
#pragma subtitle(" ")
#pragma subtitle("stspack3 - Local string test functions       ")
/********************************************************************/
/*                                                                  */
/*  Title:         stspack3                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          05 Oct 1992                                      */
/*  Programmer:    ALLAN DARLING                                    */
/*  Language:      C/2                                              */
/*                                                                  */
/*  Abstract:      The stspack3 package contains functions to       */
/*                 perform the isalnum through isxdigit functions   */
/*                 on strings.  The functions come in four forms:   */
/*                 those that test NULL delimited strings and are   */
/*                 named in the form sxxxxxxx, those that test at   */
/*                 most n characters and are named in the form      */
/*                 nxxxxxxx, those that search forward in a string  */
/*                 and are named in the form nxtyyyyy, and those    */
/*                 that search backward in a string and are named   */
/*                 in the form lstyyyyy.                            */
/*                                                                  */
/*                 The xxxxxxx is the name of the test applied to   */
/*                 each character in the string, such as isalpha,   */
/*                 thus a function to test a NULL delimited string  */
/*                 an return a nonzero value if all characters in   */
/*                 the string are digits is named sisdigit.         */
/*                                                                  */
/*                 The yyyyy is the name of the test applied to     */
/*                 characters in a string, minus the 'is' prefix.   */
/*                 Thus a function to find the next digit in a NULL */
/*                 delimited string and return a pointer to it is   */
/*                 named nxtdigit.                                  */
/*                                                                  */
/*                 The only exception to the naming rule is for the */
/*                 functions that test for hexadecimal digits.      */
/*                 These are named sisxdigi, nisxdigi, nxtxdigi,    */
/*                 and lstxdigi because of the eight character      */
/*                 function name limitation.                        */
/*                                                                  */
/*                 The nxxxxxxx class of functions will test up to  */
/*                 n characters or the first NULL character         */
/*                 encountered, whichever comes first.  For all     */
/*                 classes of functions, the string sentinal is     */
/*                 not included in the test.                        */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 isalnum, isalpha, iscntrl, isdigit, isgraph,     */
/*                 islower, isprint, ispunct, isspace, isupper,     */
/*                 isxdigit.                                        */
/*                                                                  */
/*  Input:         For sxxxxxxx class functions, a pointer to a     */
/*                 NULL delimited character string.                 */
/*                                                                  */
/*                 For nxtyyyyy class functions, a pointer to a     */
/*                 NULL delimited character string.                 */
/*                                                                  */
/*                 for nxxxxxxx class functions, a pointer to a     */
/*                 character array, and a positive, nonzero integer.*/
/*                                                                  */
/*                 for lstyyyyy class functions, a pointer to a     */
/*                 character array, and a positive, nonzero integer.*/
/*                                                                  */
/*  Output:        A nonzero value if the test is true for all      */
/*                 characters in the string, a zero value otherwise.*/
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
char *nxtalnum(char *s) {
 
   for (; !isalnum(*s) && *s; s++) ;
 
   if (*s)
      return (s);
   else
      return (NULL);
 
} /* end nxtalnum */
 
 
char *nxtalpha(char *s) {
 
   for (; !isalpha(*s) && *s; s++) ;
 
   if (*s)
      return (s);
   else
      return (NULL);
 
} /* end nxtalpha */
 
 
char *nxtcntrl(char *s) {
 
   for (; !iscntrl(*s) && *s; s++) ;
 
   if (*s)
      return (s);
   else
      return (NULL);
 
} /* end nxtcntrl */
 
 
char *nxtdigit(char *s) {
 
   for (; !isdigit(*s) && *s; s++) ;
 
   if (*s)
      return (s);
   else
      return (NULL);
 
} /* end nxtdigit */
 
 
char *nxtgraph(char *s) {
 
   for (; !isgraph(*s) && *s; s++) ;
 
   if (*s)
      return (s);
   else
      return (NULL);
 
} /* end nxtgraph */
 
 
char *nxtlower(char *s) {
 
   for (; !islower(*s) && *s; s++) ;
 
   if (*s)
      return (s);
   else
      return (NULL);
 
} /* end nxtlower */
 
 
char *nxtprint(char *s) {
 
   for (; !isprint(*s) && *s; s++) ;
 
   if (*s)
      return (s);
   else
      return (NULL);
 
} /* end nxtprint */
 
 
char *nxtpunct(char *s) {
 
   for (; !ispunct(*s) && *s; s++) ;
 
   if (*s)
      return (s);
   else
      return (NULL);
 
} /* end nxtpunct */
 
 
char *nxtspace(char *s) {
 
   for (; !isspace(*s) && *s; s++) ;
 
   if (*s)
      return (s);
   else
      return (NULL);
 
} /* end nxtspace */
 
 
char *nxtupper(char *s) {
 
   for (; !isupper(*s) && *s; s++) ;
 
   if (*s)
      return (s);
   else
      return (NULL);
 
} /* end nxtupper */
 
 
char *nxtxdigi(char *s) {
 
   for (; !isxdigit(*s) && *s; s++) ;
 
   if (*s)
      return (s);
   else
      return (NULL);
 
} /* end nxtxdigi */
 
 
#pragma page(1)
@@ANTOI.C
#pragma comment(compiler)
#pragma comment(date)
#pragma comment(timestamp)
 
#include <stdlib h>
 
#pragma title("antoi - char array to integer")
#pragma pagesize (80)
 
#pragma page(1)
/********************************************************************/
/*                                                                  */
/*  Title:         antoi                                            */
/*  Date:          Jan 28, 1991                                     */
/*  Organization:  W/OSO242 - Graphics and Display Section          */
/*  Programmer:    Allan Darling                                    */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      This function will convert a character array     */
/*                 (string) of length (len) into an integer.        */
/*                 The integer is created via a call to the         */
/*                 function atoi.  This function extends the        */
/*                 functionality of atoi by removing the            */
/*                 requirement for a sentinal delimited string      */
/*                 as input.                                        */
/*                                                                  */
/*  Input: - Pointer to an array of characters.                     */
/*         - Integer indicating the number of character to include  */
/*           in the conversion.                                     */
/*                                                                  */
/*  Output:- An integer corresponding to the value in the character */
/*           array or MAXNEG (-2147483648) if the function is       */
/*           unable to acquire system storage.                      */
/*                                                                  */
/*  Modification History:                                           */
/*                 None                                             */
/*                                                                  */
/********************************************************************/
 
int antoi(char * string, int len)
{
 
    /*******************/
    /* local variables */
    /*******************/
 
    char * tmpstr;
    int i,
        retval;
 
 
    /*****************/
    /* function body */
    /*****************/
 
    tmpstr = malloc((len+1) * sizeof(char));
 
    if (tmpstr == NULL) return (-2147483648);
 
    for (i = 0; i < len; i++)
       tmpstr[i] = string[i];
 
    tmpstr[len] = '\0';
 
    retval = atoi(tmpstr);
 
    free(tmpstr);
 
    return(retval);
 
} /* end antoi */
 
#pragma page(1)
@@CHARCMP.C
#pragma comment (compiler)
#pragma comment (date)
#pragma comment (timestamp)
#pragma pagesize(80)
 
#include <local h>     /* standard header file */
 
#pragma subtitle(" ")
#pragma page(1)
#pragma subtitle("charcmp - characters compare with patterns  ")
/********************************************************************/
/*                                                                  */
/*  Title:         charcmp                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          12 Dec 1995                                      */
/*  Programmer:    CINDY L. CHONG                                   */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      This function will compare each character in the */
/*                 string match with each character in the pattern  */
/*                 which is made up of characters.   The str can    */
/*                 be longer than the pattern.                      */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         str is a pointer to char                         */
/*                 pattern is a pointer to char                     */
/*                                                                  */
/*  Output:        Return true if str matches pattern,              */
/*                 otherwise, return false                          */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
#pragma page(1)
 
bool charcmp(char *str, char *pattern)
{
 
 
   /**********************************************************/
   /* Loop while str and pattern is not equal to null, then  */
   /* inscreases str and pattern by one                      */
   /**********************************************************/
 
   for (; *pattern != '\0'; pattern++)
   {
      if (*str == '\0')
         return FALSE;
 
      /************************************************************/
      /* If pattern match str, then increase str and jump out the */
      /* case and read next char of the str and pattern           */
      /************************************************************/
 
      if ( isspace(*pattern) )
         pattern = nxtalpha(pattern);
 
      switch( *pattern )
      {
         case 'c':
            if ( !isalnum(*str++) )
            {
               return FALSE;
            }
            break;
 
         case 'a':
            if ( !isalpha(*str) )
            {
               return FALSE;
            }
            str++;
            break;
 
         case 'n':
            if ( !iscntrl(*str++) )
            {
               return FALSE;
            }
            break;
 
         case 'd':
            if ( !isdigit(*str) )
            {
               return FALSE;
            }
            str++;
            break;
 
         case 'g':
            if ( !isgraph(*str++) )
            {
               return FALSE;
            }
            break;
 
         case 'i':
            if ( !islower(*str++) )
            {
               return FALSE;
            }
            break;
 
         case 'p':
            if ( !isprint(*str++) )
            {
               return FALSE;
            }
            break;
 
         case 't':
            if ( !ispunct(*str++) )
            {
               return FALSE;
            }
            break;
 
         case 'w':
            if ( !isspace(*str++) )
            {
               return FALSE;
            }
            break;
 
         case 'u':
            if ( !isupper(*str++) )
            {
               return FALSE;
            }
            break;
 
         case 's':
            if (*str++ != ' ')
            {
               return FALSE;
            }
            break;
 
         case 'm':
            if ( !isspace(*str) )
            {
               return FALSE;
            }
            else
            {
               while ( isspace(*str) )
                  str++;
            }
            break;
 
         case '\'':
            pattern++;
            if (*pattern != *str)
            {
               return FALSE;
            }
            pattern++;
            str++;
            break;
 
         default:
            return FALSE;
 
      } /* end switch */
 
   } /* end for */
 
   return (TRUE);
}
 

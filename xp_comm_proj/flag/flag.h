/*              @(#)$RCSfile: flag.h,v $ $Revision: 1.1 $ $AVS$ $Date: 1997/10/02 15:23:51 $     */

/*
 * flag.h
 */

/***************************************************************************
Copyright (C) 1992, 1993
by Kubota Pacific Computer Inc.  All Rights Reserved.
This program is a trade secret of Kubota Pacific Computer Inc. and
it is not to be reproduced, published, disclosed to others, copied,
adapted, distributed, or displayed without the prior authorization
of Kubota Pacific Computer Inc.  Licensee agrees to attach or embed
this Notice on all copies of the program, including partial copies
or modified versions thereof.
***************************************************************************/

/* vector operations */

void vecmulc (
    double a,
    double *v,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (v)[qxg]*=(a);
}

void vecaddc (
    double a,
    double *v,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (v)[qxg]+=(a);
}

void vecsubc (
    double a,
    double *v,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (v)[qxg]-=(a);
}

void vecdivc (
    double a,
    double *v,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (v)[qxg]/=(a);
}

void veccopyc (
    double a,
    double *v,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (v)[qxg]=(a);
}


void vecmul (
    double *u,
    double *v,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (v)[qxg]*=(u)[qxg];
}

void vecadd (
    double *u,
    double *v,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (v)[qxg]+=(u)[qxg];
}

void vecsub (
    double *u,
    double *v,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (v)[qxg]-=(u)[qxg];
}

void vecdiv (
    double *u,
    double *v,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (v)[qxg]/=(u)[qxg];
}

void veccopy (
    double *u,
    double *v,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (v)[qxg]=(u)[qxg];
}


void vecmulc3 (
    double a,
    double *v,
    double *o,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]*(a);
}

void vecaddc3 (
    double a,
    double *v,
    double *o,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]+(a);
}

void vecsubc3 (
    double a,
    double *v,
    double *o,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]-(a);
}

void vecdivc3 (
    double a,
    double *v,
    double *o,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]/(a);
}


void vecmul3 (
    double *u,
    double *v,
    double *o,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]*(u)[qxg];
}

void vecadd3 (
    double *u,
    double *v,
    double *o,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]+(u)[qxg];
}

void vecsub3 (
    double *u,
    double *v,
    double *o,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]-(u)[qxg];
}

void vecdiv3 (
    double *u,
    double *v,
    double *o,
    int n)
{
	int qxg;
	
	for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]/(u)[qxg];
}



/* prototypes */

void vecmulc  (double, double *, int);
void vecaddc  (double, double *, int);
void vecsubc  (double, double *, int);
void vecdivc  (double, double *, int);
void veccopyc  (double, double *, int);
void vecmul  (double *, double *, int);
void vecadd  (double *, double *, int);
void vecsub  (double *, double *, int);
void vecdiv  (double *, double *, int);
void veccopy  (double *, double *, int);
void vecmulc3  (double, double *, double *, int);
void vecaddc3  (double, double *, double *, int);
void vecsubc3  (double, double *, double *, int);
void vecdivc3  (double, double *, double *, int);
void vecmul3  (double *, double *, double *, int);
void vecadd3  (double *, double *, double *, int);
void vecsub3  (double *, double *, double *, int);
void vecdiv3  (double *, double *, double *, int);

int make_flag_model_group (void);
void update_flag_callback (void);
void forceflag  (void);
void force_horizontal  (void);
void force_vertical  (void);
void force_diagonal  (void);
void externalforces  (void);
void force_wind  (void);
void moveflag  (void);
void init_sqrt  (void);
void init_wind  (void);
void init_flag  (void);
int createflag (void);
void calc_wind  (void);



/***************************************************************************
Copyright (C) 1992, 1993
by Kubota Pacific Computer Inc.  All Rights Reserved.
This program is a trade secret of Kubota Pacific Computer Inc. and
it is not to be reproduced, published, disclosed to others, copied,
adapted, distributed, or displayed without the prior authorization
of Kubota Pacific Computer Inc.  Licensee agrees to attach or embed
this Notice on all copies of the program, including partial copies
or modified versions thereof.
***************************************************************************/

typedef struct
{
   float  light_intensities[3];

   char  *flag_texture_file;
   int    flag_color;
   int    flag_motion;
   int    flag_release[2];
   float  flag_wind[2];
   float  flag_backg_rgb[3];
   int    flag_reptype;
   int    flag_reset;

   int    timer;

   float  flag_strength;
}
Ui;

#define LIGHT_AMB       0
#define LIGHT_DIR       1
#define LIGHT_SPEC      2

#define RELEASE_TOP     0
#define RELEASE_BOTTOM  1

#define MOTION_NONE     0
#define MOTION_COARSE   1
#define MOTION_REG      2
#define MOTION_FINE     3

#define COLOR_SOLID     0
#define COLOR_VELOCITY  1
#define COLOR_FORCE     2
#define COLOR_FORCEMAG  3
#define COLOR_TEXTURE   4

#ifndef FALSE
# define FALSE 0
#endif

#ifndef TRUE
# define TRUE  1
#endif


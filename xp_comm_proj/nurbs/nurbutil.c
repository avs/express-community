/*
 * Nurbs Demo, Manchester University
 * Revised from AVS5 to Express by
 * Ian Curington, August, 1995
 *
 * Revision: 13 Oct 1995 ianc - added control point struct
 *           30 Oct 1995 ianc - added ptr object to nurbs_struct
 *                              utilities only
 */

/*-----------------------------------------------------------------------------
    (C)Copyright 1995, The University of Manchester, United Kingdom

    This program is produced by the Computer Graphics Unit, Manchester
    Computing, the Victoria University of Manchester.

    This program has been included for its instructional value. It has been 
    tested with care but is not guaranteed for any particular purpose. Neither
    the authors, nor the University of Manchester offer any warranties or 
    representations, nor do they accept any liabilities with respect to this 
    program. 

    This program must not be used for commmercial gain without the written 
    permission of the authors. 

    Fenqiang Lin, W T Hewitt
    Computer Graphics Unit
    Manchester Computing
    University of Manchester
    Manchester M13 9PL

    e'mail: cgu-info@mcc.ac.uk
    tel:    0161 275 6095
    fax:    0161 275 6040    
    
-----------------------------------------------------------------------------*/


#include <stdio.h>
#include <math.h>

#include "avs/om.h"
#include "avs/err.h"
#include "avs/gd_def.h"

#include "nurbs.h"

int
nurbs_util(OMobj_id object, OMevent_mask event_mask, int seq_num)
{
	return 0;
}
    /*******************************/
    /***  Function Declarations  ***/
    /*******************************/

/* *****************************************/
/* NURBS evaluation functions              */
/* *****************************************/

/* NURBS_PCH_SIZE --- calculate the size of Qnurbs_pch */

int nrb_pch_size (int ku,int nu,int kv,int nv)
                          /* order in u direction */
                          /* number of control points in u */
                          /* order in v direction */
                          /* number of control points in v */
{
    return (sizeof (Qnurbs_pch) + 
	    (nu + ku + nv + kv + 4 * nu * nv - 1) * sizeof(Qfloat));
}    

/* NURBS_PCH_DATA --- assign nrb pch data to the given parameters */

void nrb_pch_data (Qnurbs_pch *pch,int *ku,int *nu,int *kv,int *nv,Qfloat **tu,Qfloat **tv,Qpoint4 **p)
                /* nrb patch record */
                 /* order in u direction */
                  /* number of control points in u */
                  /* order in v direction */
                  /* number of control points in v */
                /* u knots vector */
                /* v knots vector */
                  /* control points */
{
    *ku = pch->ku;
    *nu = pch->nu;
    *kv = pch->kv;
    *nv = pch->nv;
    *tu = &pch->d[0];
    *tv = &pch->d[pch->ku + pch->nu];
    *p  = (Qpoint4 *) &pch->d[pch->ku + pch->nu + pch->kv + pch->nv];
}       

/* NRB_DUMP_PCH: dump nrb patch data in the proper form */

void nrb_dump_pch (char *name,int dbkey,Qnurbs_pch *pch_data)
                /* comment name */
                /* related DB key */
                /* nrb patch data */
{
    Qfloat   *tu, *tv;
    Qpoint4 *pcp;
    int     ku, nu, kv, nv, i, j, jj;

    if (pch_data == NULL) return;

    nrb_pch_data (pch_data, &ku, &nu, &kv, &nv, &tu, &tv, &pcp);
    /* dump nurbs pch data */
    printf ("NURBS_PCH %s DATA ====================>\n", name);
    printf ("DATA BASE KEY = %d\n", dbkey);
    printf ("U: ORDER = %d, CPT NO = %d\n", ku, nu);
    printf ("V: ORDER = %d, CPT NO = %d\n", kv, nv);
    printf ("U KNOTS VECTOR ==>\n");
    for (i = 0; i < ku + nu; i++) 
	printf ("   %d ==> %f\n", i, tu[i]);
    printf ("V KNOTS VECTOR ==>\n");
    for (i = 0; i < kv + nv; i++) 
	printf ("   %d ==> %f\n", i, tv[i]);
    printf ("4D CONTROL POINTS ==>\n");
    for (i = 0; i < nv; i++)
    for (j = 0; j < nu; j++) {
	jj = i * nu + j;
	printf ("%f, %f, %f, %f,\n", 
		pcp[jj].x, pcp[jj].y, pcp[jj].z, pcp[jj].w);
    }
}

/* NRB_READ_PCH: read nrb patch data from a given file */

Qnurbs_pch *nrb_read_pch(char *filename)
{
    FILE       *fp;
    int        i, j, k;
    char       line[MAXLINE];
    Qnurbs_pch *pch;    
    int        ku, nu, kv, nv;
    Qfloat     *tu, *tv;
    Qpoint4    *cp;

    if (filename == NULL) return(NULL); 
    if ((fp = fopen(filename, "r")) == NULL) return(NULL); 

    /* skip first two lines of nurbs pch data */
    fgets (line, MAXLINE, fp);
    fgets (line, MAXLINE, fp);

    /* get order and number of control points in both U and V directions */
    fscanf (fp, "U: ORDER = %d, CPT NO = %d\n", &ku, &nu);
    fscanf (fp, "V: ORDER = %d, CPT NO = %d\n", &kv, &nv);

    /* allocate memory for a NURBS surface */
    pch = (Qnurbs_pch *) malloc (nrb_pch_size (ku, nu, kv, nv));
    pch->ku = ku; 
    pch->nu = nu; 
    pch->kv = kv; 
    pch->nv = nv;
    nrb_pch_data (pch, &pch->ku, &pch->nu, &pch->kv, &pch->nv, &tu, &tv, &cp);
 
    /* skip the line */
    fgets (line, MAXLINE, fp);
    /* get U knot vector */
    for (i = 0; i < ku + nu; i++) 
	fscanf (fp, "   %d ==> %f\n", &j, &tu[i]);

    /* skip the line */
    fgets (line, MAXLINE, fp);
    /* get V knot vector */
    for (i = 0; i < kv + nv; i++) 
	fscanf (fp, "   %d ==> %f\n", &j, &tv[i]);

    /* skip the line */
    fgets (line, MAXLINE, fp);
    /* get 4D control points */
    for (i = 0; i < nv; i++)
    for (j = 0; j < nu; j++) {
	k = i * nu + j;
	fscanf (fp, "%f, %f, %f, %f,\n", 
		&cp[k].x, &cp[k].y, &cp[k].z, &cp[k].w);
    }                                                      

    /* close the data file */
    fclose(fp);
    
    return(pch);
}

/* NRB_INTERV_KNOTS : find the unique index,  
                      default case: t[i] <= tx < t[i + 1] -> right open 
		      forced  case: t[i] < tx <= t[i + 1] -> left open 
*/

void nrb_interv_knots (int k,int n,Qfloat tx,Qfloat *knots,int *index)
{ 
    int i, l0, l1;

    if (tx <= knots[k-1])    i = k - 1;
    else if (tx >= knots[n]) i = n - 1;
    else {
        l0 = k-1;
        l1 = n;
	while (l0 <= l1) {
	    i = (l0 + l1) / 2;
	    if (knots[i+1] <= tx) l0 = i + 1;
	    else if (tx < knots[i]) l1 = i - 1;
	    else break;
        }
    }
    *index = i;
}

/* NRB_CORE --- core evaluation routine for NURBS                         */

void nrb_core1(Qfloat  *a,Qfloat  *b,int m,int ld,Qpoint4 *rp)
                           /* array of (t - tk[i]) (m) at t   */
                           /* array of (tk[i] - t) (m) at t   */
                            /* (order - 1) of NURBS arc or pch */
                        /* indication of results (as mode) */
                          /* input: point arrary (m) at t    */
                                      /* output: results of evaluation   */ 
{
    int j, l, l1, mj, lj;

    for (j = 0; j < m; j++) {
        for (l = 0; l <= m-j-1; l++) {
            lj = l + j ;
            l1 = l + 1 ;
            rp[l].x = (a[lj] * rp[l1].x + b[l] * rp[l].x) / (a[lj] + b[l]);
            rp[l].y = (a[lj] * rp[l1].y + b[l] * rp[l].y) / (a[lj] + b[l]);
            rp[l].z = (a[lj] * rp[l1].z + b[l] * rp[l].z) / (a[lj] + b[l]);
            rp[l].w = (a[lj] * rp[l1].w + b[l] * rp[l].w) / (a[lj] + b[l]);
        }
    }

    if(ld > m) {
        for (l = m + 1; l <= ld; l++) {
            rp[l].x = 0.0;
            rp[l].y = 0.0;
            rp[l].z = 0.0;
            rp[l].w = 0.0;
        }
        ld = m;
    }
    for (j = 1; j <= ld; j++) {
        for (l = ld; l >= j; l--) {
	    mj = m + 1 - j;
            lj = l - j;
            l1 = l - 1;
            rp[l].x = (rp[l].x - rp[l1].x) / (b[lj] / mj);
            rp[l].y = (rp[l].y - rp[l1].y) / (b[lj] / mj);
            rp[l].z = (rp[l].z - rp[l1].z) / (b[lj] / mj);
            rp[l].w = (rp[l].w - rp[l1].w) / (b[lj] / mj);
        }
    }
}

void nrb_core2(Qfloat *a,Qfloat *b,int m,int ld,Qpoint4 *rp)
                           /* array of (t - tk[i]) (m) at t   */
                           /* array of (tk[i] - t) (m) at t   */
                            /* (order - 1) of NURBS arc or pch */
                           /* indication of results (as mode) */
                          /* input: point arrary (m) at t    */
{
    int j, l, l1, mj, lj;

    for (j = 1; j <= m; j++) {
	for (l = 0; l <= m-j; l++) {
            mj = m - j - l;
            lj = m - l - 1;
            l1 = l + 1 ;
            rp[l].w = (b[mj] * rp[l1].w + a[lj] * rp[l].w) / (a[lj] + b[mj]);
            rp[l].x = (b[mj] * rp[l1].x + a[lj] * rp[l].x) / (a[lj] + b[mj]);
            rp[l].y = (b[mj] * rp[l1].y + a[lj] * rp[l].y) / (a[lj] + b[mj]);
            rp[l].z = (b[mj] * rp[l1].z + a[lj] * rp[l].z) / (a[lj] + b[mj]);
        }
    }
    
    if(ld > m) {
        for (l = m + 1; l <= ld; l++) {
            rp[l].x = 0.0;
            rp[l].y = 0.0;
            rp[l].z = 0.0;
            rp[l].w = 0.0;
        }
        ld = m;
    }
    for (j = 1; j <= ld; j++) {
        for (l = ld; l >= j; l--) {
	    mj = m + 1 - j;
            lj = m - l + j - 1;
            l1  = l - 1;
            rp[l].w = -(rp[l].w - rp[l1].w) / (a[lj] / mj);
            rp[l].x = -(rp[l].x - rp[l1].x) / (a[lj] / mj);
            rp[l].y = -(rp[l].y - rp[l1].y) / (a[lj] / mj);
            rp[l].z = -(rp[l].z - rp[l1].z) / (a[lj] / mj);
        }
    }
}


/* NRB_PCH_EVAL :  evaluate a NURBS patch by given u and v              */

void nrb_pch_eval (Qnurbs_pch *pch,Qfloat u,Qfloat v,int mode,Qpoint3 *res)
                     /* nrb patch data to be evaluated  */
                     /* u parameter                     */
                     /* v parameter                     */
                     /* indication of results:          */
                     /* 0 -> point only to be required  */
                     /* 1 -> point and first derivative */
                     /* 2 -> point,1st & 2nd derivative */
                     /* evaluation results :            */
                     /* res[0] -> Coord. of patch point */
                     /* res[1] -> 1st derivative in u   */
                     /* res[2] -> 1st derivative in v   */
                     /* res[3] -> 2nd derivative in u   */
                     /* res[4] -> mix deriv. with u & v */
                     /* res[5] -> 2nd derivative in v   */
                     /* res[6] -> mix deriv. with v & u */
{
    static  int ir[4] = {1, 3, 6, 10};
    Qfloat  *tu, *tv, *w;
    Qpoint4 *p, rp1[MAX_ORDER][MAX_ORDER], rp2[MAX_ORDER][MAX_ORDER];
    Qfloat  au[MAX_ORDER], bu[MAX_ORDER], av[MAX_ORDER], bv[MAX_ORDER];
    int     nu, ku, nv, kv, mu, mv, ld;
    int     iu, iv, i, j, l0, l1, mj, ioff;


    if (mode > 3) mode = 3;
    ld = mode;
    nrb_pch_data (pch, &ku, &nu, &kv, &nv, &tu, &tv, &p);
    mu = ku - 1;
    mv = kv - 1;
    
    nrb_interv_knots (ku, nu, u, tu, &iu);
    nrb_interv_knots (kv, nv, v, tv, &iv);
    for (j = 0; j < mu; j++) {
	au[j] = u - tu[iu-mu+j+1];
	bu[j] = tu[iu+j+1] - u;
    }
    for (j = 0; j < mv; j++) {
	av[j] = v - tv[iv-mv+j+1];
	bv[j] = tv[iv+j+1] - v;
    } 
    for (l0 = 0; l0 <= mv; l0++) {
	if (bv[0] > av[mv-1]) l1 = iv - mv + l0;
	else                  l1 = iv - l0;
	ioff = l1 * nu + iu;
        if (bu[0] > au[mu-1]) {
            for (j = 0; j <= mu; j++) {
		mj = ioff - mu + j;
		rp1[l0][j] = p[mj];
            }
	    nrb_core1 (au, bu, mu, ld, &rp1[l0][0]);
        } else {
	    for (j = 0; j <= mu; j++) {
		mj = ioff - j;
		rp1[l0][j] = p[mj];
	    }
	    nrb_core2 (au, bu, mu, ld, &rp1[l0][0]);
        }
    }    	

    for (l0 = 0; l0 <= ld; l0++) {
        for (j = 0; j <= mv; j++) rp2[l0][j] = rp1[j][l0];
	if (bv[0] > av[mv-1])
	    nrb_core1 (av, bv, mv, ld, &rp2[l0][0]);
        else
            nrb_core2 (av, bv, mv, ld, &rp2[l0][0]);
    }

    for (i = 0; i < ir[ld]; i++) {
        switch (i) {        
        case 0:             /* Coord. of patch point */
	    res[0].x = rp2[0][0].x / rp2[0][0].w;
	    res[0].y = rp2[0][0].y / rp2[0][0].w;
	    res[0].z = rp2[0][0].z / rp2[0][0].w;
	    break;
        case 1:             /* 1st derivative in u */
	    res[1].x = (rp2[1][0].x - rp2[1][0].w * res[0].x) / rp2[0][0].w;
	    res[1].y = (rp2[1][0].y - rp2[1][0].w * res[0].y) / rp2[0][0].w;
	    res[1].z = (rp2[1][0].z - rp2[1][0].w * res[0].z) / rp2[0][0].w;
	    break;
        case 2:             /* 1st derivative in v */
	    res[2].x = (rp2[0][1].x - rp2[0][1].w * res[0].x) / rp2[0][0].w;
	    res[2].y = (rp2[0][1].y - rp2[0][1].w * res[0].y) / rp2[0][0].w;
	    res[2].z = (rp2[0][1].z - rp2[0][1].w * res[0].z) / rp2[0][0].w;
	    break;
        case 3:             /* 2nd derivative in u */
	    res[3].x = (rp2[2][0].x - 2 * rp2[1][0].w * res[1].x 
                     - rp2[2][0].w * res[0].x ) / rp2[0][0].w;
	    res[3].y = (rp2[2][0].y - 2 * rp2[1][0].w * res[1].y 
                     - rp2[2][0].w * res[0].y ) / rp2[0][0].w;
	    res[3].z = (rp2[2][0].z - 2 * rp2[1][0].w * res[1].z 
                     - rp2[2][0].w * res[0].z ) / rp2[0][0].w;
            break;
        case 4:             /* mix deriv. with u & v */
	    res[4].x = (rp2[1][1].x - rp2[1][1].w * res[0].x 
                     - rp2[1][0].w * res[2].x - rp2[0][1].w * res[1].x) 
	             / rp2[0][0].w;
	    res[4].y = (rp2[1][1].y - rp2[1][1].w * res[0].y 
                     - rp2[1][0].w * res[2].y - rp2[0][1].w * res[1].y) 
	             / rp2[0][0].w;
	    res[4].z = (rp2[1][1].z - rp2[1][1].w * res[0].z 
                     - rp2[1][0].w * res[2].z - rp2[0][1].w * res[1].z) 
	             / rp2[0][0].w;
	    break;
        case 5:             /* 2nd derivative in v */
	    res[5].x = (rp2[0][2].x - 2 * rp2[0][1].w * res[2].x 
                     - rp2[0][2].w * res[0].x ) / rp2[0][0].w;
	    res[5].y = (rp2[0][2].y - 2 * rp2[0][1].w * res[2].y 
                     - rp2[0][2].w * res[0].y ) / rp2[0][0].w;
	    res[5].z = (rp2[0][2].z - 2 * rp2[0][1].w * res[2].z 
                     - rp2[0][2].w * res[0].z ) / rp2[0][0].w;
            break;
        }
    }
}


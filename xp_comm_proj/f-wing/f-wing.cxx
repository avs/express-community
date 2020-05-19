#include <stdio.h>
#include <math.h>

#include "./xp_comm_proj/f-wing/gen.hxx"

#define ARR(_ar,i,j,k) _ar[(grid*grid*(k) + grid*(j) + (i))]
#define ALLOCCUBE( _ar,_size) _ar=(double*)malloc(_size*_size*_size*sizeof(double))

// Uncomment the following line to enable the display of debugging information
#define DEBUG

#ifdef DEBUG
#include <sys/types.h>
#include <time.h>
#endif

#ifdef DEBUG
#define PRINT(A) { fprintf(stdout, A); fflush(stdout);}
#else
#define PRINT(A) { }
#endif


void vortvel(int i,int j,int k,double l,double m,double n,double theta,double beta,double gama,double *vortx,double *vorty,double *vortz);


int
FiniteWing_FiniteWingMods_FiniteWingCore::update(OMevent_mask event_mask, int seq_num)
{
   // Angle_of_Attack (OMXdouble read req notify)
   // Wing_span (OMXdouble read req notify)

   // Output_x (OMXdouble_array write)
   int Output_x_size;
   double *Output_x_arr;

   // Output (OMXdouble_array write)
   int Output_y_size;
   double *Output_y_arr;

   // Output (OMXdouble_array write)
   int Output_z_size;
   double *Output_z_arr;


   double aoa = (double)Angle_of_Attack;
   double ws  = (double)Wing_span;
   int gs = (int)Grid_size;

   /***********************/
   /* Function's Body     */
   /***********************/
   //ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: FiniteWing_FiniteWingMods_FiniteWingCore::update\n");

   PRINT("Starting finitewing update\n");

#ifdef DEBUG
   time_t t0, t1;
   t0=time(0);
#endif

   int i,j,k,posx,posy,posz,grid,gridmax,b,brel;
   double gama,u,pi,deg,alpha,beta,theta,l,m,n,c,ratio;
   double oldl,oldn,vortx,vorty,vortz,dist;

   // maximum size of grid
   gridmax=51;
   // number of grid points
   grid=gs;
   if ((grid % 2)==0)
     grid++;

   Grid_size = grid;


   Output_x_arr = (double *)Output_x.ret_array_ptr(OM_GET_ARRAY_WR,&Output_x_size);
   if (!Output_x_arr) {
      // Output_x_arr is not valid, don't run code
      return 0;
   }

   Output_y_arr = (double *)Output_y.ret_array_ptr(OM_GET_ARRAY_WR,&Output_y_size);
   if (!Output_y_arr) {
      // Output_y_arr is not valid, don't run code
      ARRfree(Output_x_arr);
      return 0;
   }

   Output_z_arr = (double *)Output_z.ret_array_ptr(OM_GET_ARRAY_WR,&Output_z_size);
   if (!Output_z_arr) {
      // Output_z_arr is not valid, don't run code
      ARRfree(Output_x_arr);
      ARRfree(Output_y_arr);
      return 0;
   }

   PRINT("Arrays allocated\n");

   double *ux, *uy, *uz, *velx, *vely, *velz, *vbx, * vby, *vbz;
   double *vpx, *vpy, *vpz, *vnx, *vny, *vnz, *vboldx, *vboldy, *vboldz;
   double *vpoldx, *vpoldy, *vpoldz, *vnoldx, *vnoldy, *vnoldz;

   ALLOCCUBE(ux, grid);ALLOCCUBE(uy, grid);ALLOCCUBE(uz, grid);
   ALLOCCUBE(velx, grid);ALLOCCUBE(vely, grid);ALLOCCUBE(velz, grid);
   ALLOCCUBE(vbx, grid);ALLOCCUBE(vby, grid);ALLOCCUBE(vbz, grid);
   ALLOCCUBE(vpx, grid);ALLOCCUBE(vpy, grid);ALLOCCUBE(vpz, grid);
   ALLOCCUBE(vnx, grid);ALLOCCUBE(vny, grid);ALLOCCUBE(vnz, grid);
   ALLOCCUBE(vboldx, grid);ALLOCCUBE(vboldy, grid);ALLOCCUBE(vboldz, grid);
   ALLOCCUBE(vpoldx, grid);ALLOCCUBE(vpoldy, grid);ALLOCCUBE(vpoldz, grid);
   ALLOCCUBE(vnoldx, grid);ALLOCCUBE(vnoldy, grid);ALLOCCUBE(vnoldz, grid);

  //calculate ratio between gridsize and maximum gridsize
  ratio=float(grid-1)/float(gridmax-1);

  //position of bound vortex
  posx=((grid-1)/4);      // quater grid-1
  posy=(grid-1)/2;        // half grid-1
  posz=(grid-1)/2;        // half grid-1

  for (k=0;k<grid;k++)
  {
      for (j=0;j<grid;j++)
      {
         for (i=0;i<grid;i++)
         {
            ARR(vbx, i,j,k)=0.0;
            ARR(vby, i,j,k)=0.0;
            ARR(vbz, i,j,k)=0.0;
            ARR(vpx, i,j,k)=0.0;
            ARR(vpy, i,j,k)=0.0;
            ARR(vpz, i,j,k)=0.0;
            ARR(vnx, i,j,k)=0.0;
            ARR(vny, i,j,k)=0.0;
            ARR(vnz, i,j,k)=0.0;
         }
      }
  } 

  // define fixed variables
  pi=4.0*atan(1.0);
  // free-stream velocity
  u=1.0;
  // chord length
  c=4;   

  // changable variables
  // wing-span  
  b = ws;
  if (b*ratio-int(b*ratio)<0.5)
    brel=int(b*ratio);
  else if (b*ratio-int(b*ratio)>=0.5)
    brel=int(b*ratio+1);
  if ((brel % 2) == 1)
    brel++;

  // angle of attack;
  deg = aoa;
  alpha=deg*pi/180;


  // calculate gama
  gama=u*pi*c*alpha;

  // distance from vortex centre at which conditions to avoid infinity are imposed
  dist=0.5;

#ifdef DEBUG
  fprintf(stdout, "Wing-span = %d   AOA = %f\n", b, deg);
  fprintf(stdout, "Grid-size= %d Ratio=%f brel=%d\n", grid, ratio, brel);
  fprintf(stdout, "Position of the bound vortex is i=%d j=%d k=%d\n", posx, posy, posz); 
  fprintf(stdout, "alpha = %f   gama = %f\n", alpha, gama);
  fflush(stdout);
#endif

  // z-loop
  for (k=0;k<grid;k++)
  {
      // j-loop
      for (j=0;j<grid;j++)
      {
         // i-loop
         for (i=0;i<grid;i++)
         {
     	      // uniform flow
	         // velocity in x-direction
            ARR(ux, i,j,k)=u;
            // velocity in y-direction
            ARR(uy, i,j,k)=0.0;
            // velocity in z-direction
            ARR(uz, i,j,k)=0.0; 

            // vortex flow
            // bound vortex
            // angle of vortexline
            beta=pi/2;
            theta=pi/2;

            // position of vortex line: i=l, j=m, k=n
            l=posx;
            n=posz;

            // m-loop; locate positions of vortices
            for (m = posy-brel/2; m <= posy+brel/2; m++)
            {
               ARR(vboldx, i,j,k)=ARR(vbx, i,j,k);
               ARR(vboldy, i,j,k)=ARR(vby, i,j,k);
               ARR(vboldz, i,j,k)=ARR(vbz, i,j,k);

               vortvel(i,j,k,l,m,n,theta,beta,gama,&vortx,&vorty,&vortz);

               // velocity in x-direction
               ARR(vbx, i,j,k)=vortx;
               // velocity in y-direction   
               ARR(vby, i,j,k)=vorty;
               // velocity in z-direction
               ARR(vbz, i,j,k)=vortz;

               // update vortex flow
               ARR(vbx, i,j,k)=ARR(vbx, i,j,k)+ARR(vboldx, i,j,k);
               ARR(vby, i,j,k)=ARR(vby, i,j,k)+ARR(vboldy, i,j,k);
               ARR(vbz, i,j,k)=ARR(vbz, i,j,k)+ARR(vboldz, i,j,k);

               // avoid infinity at vortex point
               if (fabs(i-l)<dist && fabs(j-m)<dist && fabs(k-n)<dist) {
                  ARR(vbx, i,j,k)=gama*(i-l)/(pi*pow(3*dist*dist,(3.0/2.0)));
                  ARR(vby, i,j,k)=0.0;
                  ARR(vbz, i,j,k)=-gama*(k-n)/(pi*pow(3*dist*dist,(3.0/2.0)));
               }

	         }
	         // end m-loop

            // initial values for oldl and oldn
            oldl=l;
            oldn=n;

            // trailing vortices
            // angle of the vortexline
            theta=0;

            // position of the vortexline: i=l,j=m,k=n
            // l-loop; locate positions of vortices
            for (l=posx;l<grid;l++)
            {
               n=-0.00006*gama*pow(l,3.0)+0.0037*gama*pow(l,2.0)-0.0788*gama*l+0.2571*gama+posz;

               if (oldn-n!=0.0)
                  beta=pi+atan((l-oldl)/(n-oldn));
               else if (oldn-n==0.0) 
                  beta=pi/2;

               // negative trailing vortex
               m=posy-brel/2;
               ARR(vnoldx, i,j,k)=ARR(vnx, i,j,k);
               ARR(vnoldy, i,j,k)=ARR(vny, i,j,k);
               ARR(vnoldz, i,j,k)=ARR(vnz, i,j,k);

               vortvel(i,j,k,l,m,n,theta,beta,gama,&vortx,&vorty,&vortz);

               // velocity in x-direction
               ARR(vnx, i,j,k)=-vortx;
               // velocity in y-direction   
               ARR(vny, i,j,k)=-vorty;
               // velocity in z-direction
               ARR(vnz, i,j,k)=-vortz;

               // update vortex flow
               ARR(vnx, i,j,k)=ARR(vnx, i,j,k)+ARR(vnoldx, i,j,k);
               ARR(vny, i,j,k)=ARR(vny, i,j,k)+ARR(vnoldy, i,j,k);
               ARR(vnz, i,j,k)=ARR(vnz, i,j,k)+ARR(vnoldz, i,j,k);

               // avoid infinity at vortex point
               if (fabs(i-l)<dist && fabs(j-m)<dist && fabs(k-n)<dist) {
                  ARR(vnx, i,j,k)=gama*cos(beta)*(i-l)/(pi*pow(3*dist*dist,3.0/2.0));
                  ARR(vny, i,j,k)=gama*(sin(beta)-cos(beta))*(j-m)/(pi*pow(3*dist*dist,3.0/2.0));
                  ARR(vnz, i,j,k)=-gama*sin(beta)*(k-n)/(pi*pow(3*dist*dist,3/2));
               }

               // positive trailing vortex
               m=posy+brel/2;

               ARR(vpoldx, i,j,k)=ARR(vpx, i,j,k);
               ARR(vpoldy, i,j,k)=ARR(vpy, i,j,k);
               ARR(vpoldz, i,j,k)=ARR(vpz, i,j,k);

               vortvel(i,j,k,l,m,n,theta,beta,gama,&vortx,&vorty,&vortz);

               // velocity in x-direction
               ARR(vpx, i,j,k)=vortx;
               // velocity in y-direction   
               ARR(vpy, i,j,k)=vorty;
               // velocity in z-direction
               ARR(vpz, i,j,k)=vortz;

               // update vortex flow
               ARR(vpx, i,j,k)=ARR(vpx, i,j,k)+ARR(vpoldx, i,j,k);
               ARR(vpy, i,j,k)=ARR(vpy, i,j,k)+ARR(vpoldy, i,j,k);
               ARR(vpz, i,j,k)=ARR(vpz, i,j,k)+ARR(vpoldz, i,j,k);
                 
               // avoid infinity at vortex point
               if (fabs(i-l)<dist && fabs(j-m)<dist && fabs(k-n)<dist) {
                  ARR(vpx, i,j,k)=-gama*cos(beta)*(i-l)/(pi*pow(3*dist*dist,3.0/2.0));
                  ARR(vpy, i,j,k)=-gama*(sin(beta)-cos(beta))*(j-m)/(pi*pow(3*dist*dist,3.0/2.0));
                  ARR(vpz, i,j,k)=gama*sin(beta)*(k-n)/(pi*pow(3*dist*dist,3/2));
               }

               oldl=l;
               oldn=n;
            }
            // end l-loop 

            // calculate velocity field
            // velocity in x-direction
            ARR(velx, i,j,k)=ARR(vbx, i,j,k)+ARR(vpx, i,j,k)+ARR(vnx, i,j,k)+ARR(ux, i,j,k);
      	   // velocity in y-direction
            ARR(vely, i,j,k)=ARR(vby, i,j,k)+ARR(vpy, i,j,k)+ARR(vny, i,j,k)+ARR(uy, i,j,k);
            // velocity in z-direction
            ARR(velz, i,j,k)=ARR(vbz, i,j,k)+ARR(vpz, i,j,k)+ARR(vnz, i,j,k)+ARR(uz, i,j,k);

            Output_x_arr[grid*grid*k + grid*j + i]= ARR(velx, i,j,k);
            Output_y_arr[grid*grid*k + grid*j + i]= ARR(vely, i,j,k);
            Output_z_arr[grid*grid*k + grid*j + i]= ARR(velz, i,j,k);
         }
         // end x-loop
      }
      // end y-loop
   }
   // end z-loop

   free(ux);free(uy);free(uz);free(velx);free(vely);free(velz);
   free(vbx);free(vby);free(vbz);free(vpy);free(vpy);free(vpz);
   free(vnx);free(vny);free(vnz);free(vboldx);free(vboldy);
   free(vboldz);free(vpoldx);free(vpoldy);free(vpoldz);
   free(vnoldx);free(vnoldy);free(vnoldz);

   ARRfree(Output_x_arr);
   ARRfree(Output_y_arr);
   ARRfree(Output_z_arr);

   // Set transformation outputs
   probe_x = 0;
   probe_y = 0;
   probe_z = posz;
   probe_scale = grid-1;
   wing_x = posx;
   wing_y = posy-brel/2;
   wing_z = posz;
   wing_scale = brel;


#ifdef DEBUG
   // calulcate time
   t1=time(0);
   fprintf(stdout, "Ending finitewing update\n");
   fprintf(stdout, "Time = %d seconds\n\n", t1-t0);
   fflush(stdout);   
#endif

   // return 1 for success
   return 1;
}



void vortvel(int i,int j,int k,double l,double m,double n,double theta,double beta,double gama,double *vortx,double *vorty,double *vortz)
{
   double pi,A;
  
   pi=4.0*atan(1.0);
   A=gama/(4.0*pi*pow(pow((i-l),2.0)+pow((j-m),2.0)+pow((k-n),2.0),(3.0/2.0)));

   *vortx=A*((k-n)*sin(beta)*sin(theta)-(j-m)*cos(beta));
   *vorty=A*(-(k-n)*sin(beta)*cos(theta)+(i-l)*cos(beta));
   *vortz=A*((j-m)*sin(beta)*cos(theta)-(i-l)*sin(beta)*sin(theta));
}


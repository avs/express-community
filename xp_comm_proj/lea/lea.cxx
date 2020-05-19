
#include "gen.h"
#include "avs/gd_def.h"
#include "lea_lib-1.0/src/advection_master.h"

struct static_vars_t {
	int initialized;
	int doInitialSteps;
	int oldnx;
	int oldny;
	int dimvx;
	int dimvy;
};


int
LEACore::OnInstance(OMevent_mask event_mask, int seq_num)
{

   /***********************/
   /* Function's Body     */
   /***********************/
  //ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: LEA_mod::OnInstance\n");

   AdvectionMaster *lea = new AdvectionMaster;
   OMset_ptr_val((OMobj_id)local_ptr, (void*)lea, 0);
   OMset_ptr_val((OMobj_id)vx_ptr   , (void*)0, 0);
   OMset_ptr_val((OMobj_id)vy_ptr   , (void*)0, 0);
   OMset_ptr_val((OMobj_id)vmag_ptr , (void*)0, 0);

	struct static_vars_t *sv = new struct static_vars_t;
	sv->initialized = 0;
	sv->doInitialSteps = 0;
	sv->oldnx = 0;
	sv->oldny = 0;
	sv->dimvx = 0;
	sv->dimvy = 0;

   OMset_ptr_val((OMobj_id)static_vars, (void*)sv, 0);

   // return 1 for success
   return(1);
}


int
LEACore::OnDeInstance(OMevent_mask event_mask, int seq_num)
{

   /***********************/
   /* Function's Body     */
   /***********************/
  //ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: LEA_mod::OnDeInstance\n");

   AdvectionMaster *lea;
   float *vx, *vy, *vmag;

   OMget_ptr_val((OMobj_id)local_ptr, (void **)&lea, 0);
   OMget_ptr_val((OMobj_id)vx_ptr   , (void **)&vx, 0);
   OMget_ptr_val((OMobj_id)vy_ptr   , (void **)&vy, 0);
   OMget_ptr_val((OMobj_id)vmag_ptr , (void **)&vmag, 0);

   struct static_vars_t *sv;
   OMget_ptr_val((OMobj_id)static_vars, (void **)&sv, 0);
   delete sv;

   delete lea;
   delete [] vx;
   delete [] vy;
   delete [] vmag;
 
   // return 1 for success
   return(1);
}


int
LEACore::Compute(OMevent_mask event_mask, int seq_num)
{
   // nx (OMXint read notify)
   // ny (OMXint read notify)

   typedef unsigned char byte;

	// fix the problem with the static variables
    struct static_vars_t *sv;
	OMget_ptr_val((OMobj_id)static_vars, (void **)&sv, 0);
	if (!sv) return(0);

   int recompute = 0;


   AdvectionMaster *lea;
   OMget_ptr_val((OMobj_id)local_ptr, (void **)&lea, 0);
   if (!lea) return(0);
   if(!vf.nnodes.valid_obj() || !vf.ndim.valid_obj()) return 0;

   if (!sv->initialized || nx.changed(seq_num) || ny.changed(seq_num) || 
       integrationStepMax.changed(seq_num) || vf.changed (seq_num) ) {
     //cout << "SetResolution()"<<endl;
     
     //static int oldnx;
     //static int oldny;

     // Compute aspect ratio
     int *dims  = (int*) vf.dims.ret_array_ptr(OM_GET_ARRAY_RD);
     float aspectRatio = (dims&&dims[0] ? float(dims[0])/float(dims[1]) : 1);
     if (dims) ARRfree((char*) dims);
     
     if      (nx.changed(seq_num)||vf.changed (seq_num)) 
       ny = int(float(nx)/aspectRatio);
     else if (ny.changed(seq_num)) 
       nx = int(float(ny)*aspectRatio);
 
     if (nx!= sv->oldnx || ny!= sv->oldny || integrationStepMax.changed(seq_num)) {
       lea->SetResolution(nx, ny, integrationStepMax);
       recompute = 1;
       sv->doInitialSteps = 1;
       sv->oldnx = nx;
       sv->oldny = ny;
     }
   }

   if (!sv->initialized || integrationStep.changed(seq_num) || 
       swapPercentage.changed(seq_num) ||  blendCoef.changed(seq_num)) {
     //cout << "SetAdvectionOptions()"<<endl;
     lea->SetAdvectionOptions(integrationStep, swapPercentage, blendCoef);
     recompute = 1;
   }

   if (!sv->initialized || licOn.changed(seq_num)) {
     //cout << "SetLicOn()" << endl;
     lea->SetLicOn(licOn);
     recompute = 1;
   }

   if (!sv->initialized || kernelLength.changed(seq_num) || 
       kernelShape.changed(seq_num) || 
       intensityCorrectionOn.changed(seq_num) || 
       normalizationOn.changed(seq_num)) {
     //cout << "SetLicOptions()" <<endl;
     lea->SetLicOptions(kernelLength, kernelShape, intensityCorrectionOn, 
			normalizationOn);
     recompute = 1;
   }
  
   if (!sv->initialized || maskOn.changed(seq_num)) {
     //cout << "SetVelocityMaskOn()" << endl;
     lea->SetVelocityMaskOn(maskOn);
     recompute = 1;
   }

   if (!sv->initialized || velocityMaskVersion.changed(seq_num) || 
       alphaOn.changed(seq_num)) {
     //cout << "SetVelocityMaskOptions()" << endl;
     lea->SetVelocityMaskOptions(velocityMaskVersion, alphaOn);
     recompute = 1;
   }
   
   
   if (!sv->initialized || vf.changed(seq_num) || vmax.changed(seq_num)) {
     
     int nnodes     = vf.nnodes;
     int nnode_data = vf.nnode_data;
     int veclen     = (nnode_data ? vf.node_data[0].veclen : 0);
     
     float *vfdata   = 0;
     float *vfdata_u = 0;
     float *vfdata_v = 0;
     float magmax    = vmax;

     // Verify that vector field vf is correctly initialized
     // It can be either a single node data with 2 components u v
     // either a double node data of one component for u and v
     // determine magmax if not yet defined
     if (nnode_data == 1 && veclen == 2) {
       
       vfdata = (float*) vf.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RD);

       if ((float)vmax == 0) {
	 float *max_uv, *min_uv, min_u, min_v, max_u, max_v;
	 max_uv=(float*)vf.node_data[0].max_vec.ret_array_ptr(OM_GET_ARRAY_RD);
	 min_uv=(float*)vf.node_data[0].min_vec.ret_array_ptr(OM_GET_ARRAY_RD);
	 // play with squares to get rid of sign
	 min_u = min_uv[0]*min_uv[0];
	 min_v = min_uv[1]*min_uv[1];
	 max_u = max_uv[0]*max_uv[0];
	 max_v = max_uv[1]*max_uv[1];
	 // find the maximum square value for each component
	 if (min_u > max_u) max_u = min_u;
	 if (min_v > max_v) max_v = min_v;
	 // magmax is the norm. max_u and max_v are already squared
	 magmax = sqrt(max_u + max_v);	 
	 
	 //cout << "magmax = " << (float)magmax << endl;
	 
	 if (max_uv) ARRfree((char*) max_uv);
	 if (min_uv) ARRfree((char*) min_uv);
       }

     } else if (nnode_data == 2 && veclen == 1) {
       
       vfdata_u=(float*) vf.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RD);
       vfdata_v=(float*) vf.node_data[1].values.ret_array_ptr(OM_GET_ARRAY_RD);
       
       if ((float)vmax == 0) {
	 float *max_uv0,*min_uv0,*max_uv1,*min_uv1,min_u,min_v,max_u,max_v;
       max_uv0 =(float*)vf.node_data[0].max_vec.ret_array_ptr(OM_GET_ARRAY_RD);
       min_uv0 =(float*)vf.node_data[0].min_vec.ret_array_ptr(OM_GET_ARRAY_RD);
       max_uv1 =(float*)vf.node_data[1].max_vec.ret_array_ptr(OM_GET_ARRAY_RD);
       min_uv1 =(float*)vf.node_data[1].min_vec.ret_array_ptr(OM_GET_ARRAY_RD);
	 // play with squares to get rid of sign
	 min_u = min_uv0[0]*min_uv0[0];
	 min_v = min_uv1[0]*min_uv1[0];
	 max_u = max_uv0[0]*max_uv0[0];
	 max_v = max_uv1[0]*max_uv1[0];
	 // find the maximum square value for each component
	 if (min_u > max_u) max_u = min_u;
	 if (min_v > max_v) max_v = min_v;
	 // magmax is the norm. max_u and max_v are already squared
	 magmax = sqrt(max_u + max_v); 

	 //cout << "magmax = " << (float)magmax << endl;

	 if (max_uv0) ARRfree((char*) max_uv0);
	 if (min_uv0) ARRfree((char*) min_uv0);
	 if (max_uv1) ARRfree((char*) max_uv1);
	 if (min_uv1) ARRfree((char*) min_uv1);
       }
     } else if (nnode_data == 0 && veclen == 0) {
       // no error reported: triggered without input
       return 0;
     } else {
       ERRverror("",ERR_NO_HEADER | ERR_PRINT, "vf should have nnode_data==1 and veclen==2\nor nnode_data==2 and veclen==1\n(it has: nnode_data = %d; veclen = %d)\n", nnode_data, veclen);
       return(0);
     }
     
     //static int   dimvx,dimvy;
     //static float *vx,*vy,*vmag;
     
     // retrieve velocity array pointers
     float *vx,*vy,*vmag;
     OMget_ptr_val((OMobj_id)vx_ptr   , (void**)&vx, 0);
     OMget_ptr_val((OMobj_id)vy_ptr   , (void**)&vy, 0);
     OMget_ptr_val((OMobj_id)vmag_ptr , (void**)&vmag, 0);

     int *dims  = (int*) vf.dims.ret_array_ptr(OM_GET_ARRAY_RD);
     
     // check if necessary to reallocate arrays
     if (sv->dimvx != dims[0] || sv->dimvy != dims[1] || !vx || !vy || !vmag) {
       //cout << "Array reallocation!" <<endl;
       sv->dimvx = dims[0];
       sv->dimvy = dims[1];
       delete [] vx;
       delete [] vy;
       delete [] vmag;
       vx   = new float[nnodes];
       vy   = new float[nnodes];
       vmag = new float[nnodes];
       OMset_ptr_val((OMobj_id)vx_ptr   , (void*)vx, 0);
       OMset_ptr_val((OMobj_id)vy_ptr   , (void*)vy, 0);
       OMset_ptr_val((OMobj_id)vmag_ptr , (void*)vmag, 0);
     }
     if (dims) ARRfree((char*) dims);
     
     // fill velocity array from vfdata or vfdata_u and vfdata_v
     int i;
     float valvx,valvy,valmag;
     
     if (nnode_data == 1 && veclen == 2) {
       
       for (i=0; i<nnodes; i++) {
	 valvx = vfdata[i*2  ];
	 valvy = vfdata[i*2+1];
	 valmag= sqrt(valvx*valvx + valvy*valvy);

	 if (valmag>=magmax) {
	   vx[i]   = valvx / valmag;
	   vy[i]   = valvy / valmag;
	   vmag[i] = 1;
	 } else {
	   vx[i]   = valvx / magmax;
	   vy[i]   = valvy / magmax;
	   vmag[i] = valmag/ magmax;;
	 }
       }
       
     } else if (nnode_data == 2 && veclen == 1) {
       
       for (i=0; i<nnodes; i++) {
	 valvx = vfdata_u[i];
	 valvy = vfdata_v[i];
	 valmag= sqrt(valvx*valvx + valvy*valvy);

	 if (valmag>=magmax) {
	   vx[i]   = valvx / valmag;
	   vy[i]   = valvy / valmag;
	   vmag[i] = 1;
	 } else {
	   vx[i]   = valvx / magmax;
	   vy[i]   = valvy / magmax;
	   vmag[i] = valmag/ magmax;;
	 }
       }
      
     }
     
     //cout << "SetCurrentVectorField()" << endl;

     lea->SetCurrentVectorField(vx, vy, vmag, sv->dimvx, sv->dimvy);
     
     // Release pointers on AVS structures
     if (vfdata)   ARRfree((char*) vfdata);
     if (vfdata_u) ARRfree((char*) vfdata_u);
     if (vfdata_v) ARRfree((char*) vfdata_v);
     
     recompute = 1;
   }
   
   //--- Compute and sets output ---
   if (trigger && (trigger.changed(seq_num) ||  recompute)) {
     //cout << "Compute()" << endl;

     if (sv->doInitialSteps && nInitialSteps>0) {
       lea->SetLicOn(0);
       lea->SetVelocityMaskOn(0);
       for (int i=0; i<nInitialSteps; i++) {
	 lea->Compute();
       }
       sv->doInitialSteps = 0;
     } else if (nStepsAtOnce > 1) {
       // Run internal advection nStepsAtOnce times
       lea->SetLicOn(0);
       lea->SetVelocityMaskOn(0);
       for (int i=0; i<nStepsAtOnce-1; i++) {
	 lea->Compute();
       }
     }
     // Run internal advection a last time with correct options
     lea->SetLicOn(licOn);
     lea->SetVelocityMaskOn(maskOn);
     lea->Compute();
    
     // Fill output image structure
     int nbComponents;
     float *src_img = lea->GetOutput(nbComponents);
     
     lea_img.nnodes = nx*ny;
     int* dims = (int *)lea_img.dims.ret_array_ptr(OM_GET_ARRAY_WR);
     dims[0] = nx;
     dims[1] = ny;
     if (dims) ARRfree((char*) dims);
     
     float *points = (float *)lea_img.points.ret_array_ptr(OM_GET_ARRAY_WR);
     
     float *min_vec, *max_vec;
     min_vec = (float *)vf.coordinates.min_vec.ret_array_ptr(OM_GET_ARRAY_RD);
     max_vec = (float *)vf.coordinates.max_vec.ret_array_ptr(OM_GET_ARRAY_RD);
     points[0] = min_vec[0];
     points[1] = min_vec[1];
     points[2] = max_vec[0];
     points[3] = max_vec[1];
     if (min_vec) ARRfree((char *) min_vec);
     if (max_vec) ARRfree((char *) max_vec);
     if (points)  ARRfree((char *) points);
     
     lea_img.nnode_data = 1;
     lea_img.node_data[0].veclen = 4;
     lea_img.node_data[0].id = GD_RGB_DATA_ID;
     byte *dest_img = (byte *)lea_img.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
     
     int i, is, id;
     switch (nbComponents) {
     case 1: // intensity values
       for (i=0; i<nx*ny; i++) {
	 id = i*4;
	 dest_img[id]   = 255;
	 dest_img[id+1] = byte( src_img[i]*255.0 );
	 dest_img[id+2] = byte( src_img[i]*255.0 );
	 dest_img[id+3] = byte( src_img[i]*255.0 );
       }
       break;
     case 2: // intensity-alpha values
       for (i=0; i<nx*ny; i++) {
	 is = i*2;
	 id = i*4;
	 dest_img[id]   = byte( src_img[is+1]*255.0 );
	 dest_img[id+1] = byte( src_img[is]*255.0 );
	 dest_img[id+2] = byte( src_img[is]*255.0 );
	 dest_img[id+3] = byte( src_img[is]*255.0 );
       }
       break;
     }
     if (dest_img) ARRfree((byte*) dest_img);
   }

   sv->initialized = 1;

   // return 1 for success
   return(1);
}

/*
 * Interface code for the routine:
 * "Contrast Limited Adaptive Histogram Equalization"
 * by Karel Zuiderveld, karel@cv.ruu.nl
 * in "Graphics Gems IV", Academic Press, 1994
 *
 * The module accepts 8 bit one band image in the standard
 * Express IPimage format.
 *
 * The output is again the image in the same format with contrast changed.
 *
 * The level parameter control the level of sharpening:
 *
 *	  0  - no contrast limitation, maximum contrast enhancement
 *
 *  0-1 - contrast reduction
 *   1  - no change to image
 *  1-5 - contrast enhancement
 *
 *
 *	Express i/f author:		Mario Valle
 *							AVS Italy
 *
 *  CLAHE routine author:	Karel Zuiderveld, Computer Vision Research Group,
 *							Utrecht, The Netherlands (karel@cv.ruu.nl)
 *
 *	Version:				1.0 29-August-2000
 *                   1.1 7-March-2001
 *
 */

#define XP_WIDE_API

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <malloc.h>

#include "gen.hxx"

extern "C" {
int CLAHE (unsigned char* pImage, unsigned int uiXRes, unsigned int uiYRes,
		  unsigned char Min, unsigned char Max, unsigned int uiNrX, unsigned int uiNrY,
		  unsigned int uiNrBins, float fCliplimit);
}


int
EnhanceContrast_EnhanceContrastMods_EnhanceContrastCore::Enhance(OMevent_mask event_mask, int seq_num)
{
	// img (OMXbyte_array read req notify)
	unsigned char *img_arr;
	
	// out (OMXbyte_array write)
	unsigned char *out_arr;
	
	xp_long i, j, k;
	bool need_copy = false;
	unsigned char *work_area;


	//
	//	Get the input image and dimensions
	//
	img_arr = (unsigned char *)img.ret_array_ptr(OM_GET_ARRAY_RD);
	if (!img_arr) return 0;

	// These dims are ints even under x64 as they are IP objects
	int *in_dims_arr = (int *)in_dims.ret_array_ptr(OM_GET_ARRAY_RD);
	if (!in_dims_arr) {
		ARRfree(img_arr);
		return 0;
	}


	//ERRverror("EnhanceContrastCore::Enhance", ERR_ERROR, "started");
	//cout << "in Enhance: called from EnhanceContrastCore module\n";

	//
	//	Compute number of context regions and padding eventually needed
	//
	xp_long ctx[2];
	xp_long pad[2];

	for(i=0; i < 2; i++)
	{
		for(j=16; j >= 2; j--)
		{
			if(in_dims_arr[i] % j == 0) break;
		}

		if(j < 2)
		{
			xp_long curr_err = 2000;
			xp_long curr_ctx = 0;
			for(j=2; j <= 16; j++)
			{
				xp_long err = in_dims_arr[i] - (in_dims_arr[i]/j+1)*j;
				if(err < curr_err)
				{
					curr_ctx = j;
					curr_err = err;
				}
			}
			ctx[i] = curr_ctx+1;
			pad[i] = curr_err;
			need_copy = true;
		}
		else
		{
			ctx[i] = j;
			pad[i] = 0;
		}
	}

	//
	//	Get the output image
	//
	out_arr = (unsigned char *)out.ret_array_ptr(OM_GET_ARRAY_WR);
	if (!out_arr) {
		ARRfree(in_dims_arr);
		ARRfree(img_arr);
		return 0;
	}

	//
	//	Create the local copy for the working routine
	//
	if(need_copy)
	{
		work_area = (unsigned char *)malloc((in_dims_arr[0]+pad[0])*(in_dims_arr[1]+pad[1]));
		for(j=0; j < in_dims_arr[1]; j++)
		{
			for(i=0; i < in_dims_arr[0]; i++)
			{
				work_area[j*(in_dims_arr[0]+pad[0])+i] = img_arr[j*in_dims_arr[0]+i];
			}
			for(k=0; k < pad[0]; k++) out_arr[j*(in_dims_arr[0]+pad[0])+k+in_dims_arr[0]] = 0;
		}
		for(k=0; k < (in_dims_arr[0]+pad[0])*pad[1]; k++) work_area[in_dims_arr[1]*(in_dims_arr[0]+pad[0])+k] = 0;
	}
	else
	{
		work_area = out_arr;
		for(i=0; i < in_dims_arr[0]*in_dims_arr[1]; i++)
		{
			work_area[i] = img_arr[i];
		}
	}
	ARRfree(img_arr);

	//
	//	Histogram enhancement routine
	//
	// This functions is 32bit only
	int sts = CLAHE(work_area,
					(int)(in_dims_arr[0]+pad[0]), (int)(in_dims_arr[1]+pad[1]),
					0, 255, 
					(int)ctx[0], (int)ctx[1],
					256, (float)level);

	if (sts)
	{
		ERRverror("ContrastEnhancer::Enhance",ERR_ERROR,"CLAHE routine failure. Status = %d", sts);
		if(need_copy) free(work_area);
		ARRfree(out_arr);
		ARRfree(in_dims_arr);

		return 0;
	}

	//
	//	If needed copy the workarea to the output array removing the padding
	//
	if(need_copy)
	{
		for(j=0; j < in_dims_arr[1]; j++)
		{
			for(i=0; i < in_dims_arr[0]; i++)
			{
				out_arr[j*in_dims_arr[0]+i] = work_area[j*(in_dims_arr[0]+pad[0])+i];
			}
		}
		free(work_area);
	}

	//
	//	All done
	//
	ARRfree(out_arr);
	ARRfree(in_dims_arr);

	return 1;
}


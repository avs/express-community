
//
//	Distance measurement functions. 
//

//
//	Returns the squared distance between a point and a segment
//
float point_segment_squared_dist(short px, short py, short segstartx, short segstarty, short segendx, short segendy)
{
	//	float MinPointLineSegment (const Point3& p, const Line3& seg, float& t)

	float mx = (float)segendx - (float)segstartx;
	float my = (float)segendy - (float)segstarty;

    //diff = p - b;
	float diffx = (float)px - (float)segstartx;
	float diffy = (float)py - (float)segstarty;

    //float t = Dot(m, diff);
	float t = mx*diffx+my*diffy;

    if(t > 0.0f)
    {
		//Dot(m, m);
        float mDotm = mx*mx+my*my;
        if(t >= mDotm)
        {
            //diff = diff - m;
			diffx -= mx;
			diffy -= my;
        }
        else
        {
            t /= mDotm;

            //diff = diff - t*m;
			diffx -= (t*mx);
			diffy -= (t*my);
        }
    }

    //return Dot(diff, diff);
	return diffx*diffx+diffy*diffy;
}

//
//	Returns the index of the starting point of the segment nearer to a given point
//
int find_nearest_segment(short *pt, int npt, short x, short y)
{
	float currdist = 1e20F;
	int   curridx  = 0;  // just to be sure

	for(int i=0; i < npt-1; i++)
	{
		float dist = point_segment_squared_dist(x, y, pt[2*i+0], pt[2*i+1], pt[2*i+2], pt[2*i+3]);

		if(dist < currdist)
		{
			currdist = dist;
			curridx  = i;
		}
	}

	return curridx;
}

//
//	Returns the index of the point nearer to a given point
//
int find_nearest_point(short *pt, int npt, short x, short y)
{
	float currdist = 1e20F;
	int   curridx  = 0;  // just to be sure

	for(int i=0; i < npt; i++)
	{
		float dx = (float)pt[2*i+0] - (float)x;
		float dy = (float)pt[2*i+1] - (float)y;
		float dist = dx*dx+dy*dy;

		if(dist < currdist)
		{
			currdist = dist;
			curridx  = i;
		}
	}

	return curridx;
}


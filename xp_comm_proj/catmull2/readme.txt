
	Polyline smoothing using Catmull-Rom splines
	--------------------------------------------

	This kind of spline passes through all the points with
	C1 (continuous first parametric derivative) and
	G1 (slopes equal at joints) continuity.


	Files
	-----

	ctmlmods.v		interface object and code
	ctml_mod.cxx

	catmull.cxx		spline computation object
	catmull.hxx

	ctmlmacs.v		user macros and usage examples


	CatmullSplineCore module parameters
	-----------------------------------
 
	input:
	
		points:		an array of type float with the coordinates
				of the nodes.
				The dimensions are: [n_points][nspace]
				nspace must be either 2 or 3
				n_points must be at least 2 (for open splines)
				or 3 (for closed splines)
				The first and last points can be coincident.

		line_type:	0 closed spline; 1 open spline

		spline_type:	0 catmull-rom spline;
				Currently only Catmull-Rom splines are supported

		smoothness:	'Smoothness' of calculated spline.
				As the parameter approaches 0 the generated spline
				approaches the original polyline.

		points_added:	number of points added between two input points.


	output:

		out:		array of resulting points coordinates

		out_size:	number of resulting points


	Changes from Version 1.0 of Catmull-Rom Smoother
	------------------------------------------------

	- rewritten in C++

	- bugs corrected for open line first and last points.

	- now works correctly on closed lines.

	- added a smoothness parameter for the resulting spline
	  (The spline can be straightened to a line if this parameter approaches 0
	   or it can be transformed to a baroque line if smoothness is > 1).

	- changed the old smooth parameter to points_added between two original points


	Future
	------

	- forward difference algorithm for the output computation.
	- line parametrization by arc length


	Author
	------
	Mario Valle mario@avs.it on March 19th, 2000
	Version 2.0


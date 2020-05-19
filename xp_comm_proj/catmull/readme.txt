
	Polyline smoothing using Catmull-Rom splines
	--------------------------------------------

	This spline passes through all the points with
	C1 continuity.


	Files
	-----

	catmull.v	interface object and code
	catmull.c

	spline.c	spline computation
	spline.h

	test.v		an usage example


	CatmullRomSpline module parameters
	----------------------------------

	input:
	
		points:		an array of float with the coordinates
				of the nodes.
				Format: {X0, Y0, X1, Y1 ... } or
					{X0, Y0, Z0, X1, Y1, Z1 ... }

		nspace:		nspace for input points (default: 2)
		step:		number of steps for the output spline
				approximation (default: 10)
		line_type:	0 closed spline; 1 open spline

	output:

		out:		output polyline mesh
		obj:		renderable version of the output mesh


	Usage
	-----

	- create a new project
	- load catmull.v into Workspace 1
	- put catmull.c, spline.c and spline.h in the project directory
	- select the catmull_rom_spline_module object and compile
	- load the sample application test.v


	Future
	------

	- forward difference algorithm for the output computation.
	- check for coincident nodes.


	Author
	------
	Mario Valle mario@avs.it on December 7th, 1997



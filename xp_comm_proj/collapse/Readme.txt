
Collapse
--------

	This module takes a 1, 2 or 3D values array from an uniform field
	and collapses it along the specified axis. The input field data can have any
	veclen desidered.

	There are various kind of collapsing operation controlled by the enum parameter operations:

	"collapse" (0)
		sums the values elementwise along the collapsing axis.

	"slice"	(1)
		orthoslices along the collapsing axis at specified plane (first component of
		plane parameter).

	"collapse_except" (2) 
		sums the values elementwise along the collapsing axis except specified planes.

	"collapse_only" (3)
		sum elementwise along the collapsing axis only the specified planes.



Collapse Field
--------------

	Adds to Collapse the input and output fields.


Usage
-----

	In my project I have a 3D vector uniform field representing vibrational results of
	a car test.
	The three dimensions where:
		Input frequency
		Output mode amplitude
		Car structural element
	Collapse helped me to answer the following questions:
		- Contour of total response (sum of all modes)
		- What happens to total response removing a structural element
		- For each test frequency and each structural element the mode distribution.

	The application CollapseFieldEg contains a small test application that accepts
	only one plane.
	The application CollapseFieldPlanesEg contains a small test application that accepts
	an array of planes.
	


Author
------
	Mario Valle - Advanced Visual Syetems Srl
	08-may-2000


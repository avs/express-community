
AVS/Express Star-CD Format Reader
=================================

Notes:
------

1. The reader is designed to work with version 3000 of Star CD and has been
   tested against version 3.1 of AVS/Express

2. An AVS/Express project has been supplied, designed to work with both
   the Dev. and Viz. editions. The project contains the following:
   
   apps/	- contains a simple application to test the reader
   avsenv	- edit this file to reflect your local installation of express
   src/		- source files for the reader module
   v/		- v source for the reader and associated macros
   supersonicSolid }  - Example data files
   transExample    }
   makefile	- makefile to generate project

3. This module is *unsupported*. However, comments and bug reports are
   welcome. Please refer to the README file in the src/ directory for
   details.
   
4. The file src/readStar.f contains paramaters defining the size of mesh etc.
   These may need to be increased if the data set is large, and the project
   recompiled. If this is the case, error messages outlining which parameters
   must be increased will be sent to the VCP window when the data is read in.
   
5. The modules exist in the express process which must be compiled for your
   target platform. To do this, in the top-level directory where this file
   resides, type 'make'. You may need to alter the Fortran record length in
   the file src/readStar.f at line 117.
   
6. The express process must be started from the directory containing the data
   files you wish to read. For example, for the supersonicSolid test case, do
   the following:
   
   cd supersonicSolid
   ../bin/s2/express      (assuming Solaris target platform)
   
   (load application apps/simple.v)
   (enter 'as_2d' in the 'case' type in, and press 'OK to load')

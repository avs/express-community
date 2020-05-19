//----------------------------------------------------------------------
//	File:		ann_test.cc
//	Programmer:	Sunil Arya and David Mount
//	Last modified:	03/04/98 (Release 0.1)
//	Description:	test program for ANN (approximate nearest neighbors)
//
//----------------------------------------------------------------------
// Copyright (c) 1997-1998 University of Maryland and Sunil Arya and David
// Mount.  All Rights Reserved.
// 
// This software and related documentation is part of the 
// Approximate Nearest Neighbor Library (ANN).
// 
// Permission to use, copy, and distribute this software and its 
// documentation is hereby granted free of charge, provided that 
// (1) it is not a component of a commercial product, and 
// (2) this notice appears in all copies of the software and
//     related documentation. 
// 
// The University of Maryland (U.M.) and the authors make no representations
// about the suitability or fitness of this software for any purpose.  It is
// provided "as is" without express or implied warranty.
//----------------------------------------------------------------------

#include <fstream.h>			// file I/O
#include <string.h>			// string ops
#include <time.h>			// clock
#include <math.h>			// math routines

#include <ANN/ANNx.h>			// ANN declarations
#include <ANN/ANNperf.h>		// performance evaluation

#include "rand.h"			// random point generation

#ifndef CLOCKS_PER_SEC			// define clocks-per-second if needed
#define CLOCKS_PER_SEC          1000000
#endif

//----------------------------------------------------------------------
// ann_test
//
// This program is a driver for testing and evaluating the ANN library
// for computing approximate nearest neighbors.  It allows the user to
// generate data and query sets of various sizes, dimensions, and
// distributions, to build kd- and bbd-trees of various types, and then
// run queries and outputting various performance statistics.
//
// Overview:
// ---------
// The test program is run as follows:
// 
// 	ann_test < test_input > test_output
//
// where the test_input file contains a list of directives as described
// below.
//
// Basic operations:
// -----------------
// The test program can perform the following operations.  How these
// operations are performed depends on the options which are described
// later:
//
//    output_label		Output a label to the output file.
//    gen_data_pts		Generate a set of random data points from
//				some point distribution.
//    read_data_pts <file>	Read a set of data points from file <file>.
//    build_ann			Generate an approximate nearest neighbor for
//				the current data set, using the selected
//				splitting rules.
//    gen_query_pts		Generate a set of random query points from
//				some point distribution.
//    read_query_pts <file>	Read a set of query points from file <file>
//    run_queries <string>	Apply nearest neighbor searching to the
//				query points using the approximate nearest
//				neighbor structure and the given search
//				strategy.  Possible strategies are:
//				    standard = standard kd-tree search
//				    priority = priority search
//    dump <file>		Dump the current structure to given file.
//				(The dump format is explained further in
//				the source file kd_tree.cc.)
//
//
// Options:
// --------
// How these operations are performed depends on a set of options.
// If an option is not specified, a default value is used. An option
// retains its value until it is set again.  String inputs are not
// enclosed in quotes, and must contain no embedded white space (sorry,
// this is C++'s convention).
//
// Options affecting search tree structure:
// ----------------------------------------
//	split_rule <type>	Type of splitting rule to use in building
//				the search tree.  Choices are:
//				    standard	= standard optimized kd-tree
//				    fair	= fair split
//				    midpt	= midpoint split
//				    sl_midpt	= sliding midpt split
//				    sl_fair	= sliding fair split
//				    suggest	= authors' choice for best
//				The default is "suggest".  See the file
//				kd_split.cc for more detailed information.
//
//	shrink_rule <type>	Type of shrinking rule to use in building
//				a bd-tree data structure.  If "none" is
//				given, then no shrinking is performed and
//				the result is a kd-tree.  Choices are:
//				    none	= perform no shrinking
//				    simple	= simple shrinking
//				    centroid	= centroid shrinking
//				    suggest	= authors' choice for best
//				The default is "none".  See the file
//				bd_tree.cc for more information.
//	bucket_size <int>	Bucket size, that is, the maximum number of
//				points stored in each leaf node.
//
// Options affecting data and query points:
// ----------------------------------------
//	dim <int>		Dimension of space.
//	seed <int>		Seed for random number generation.
// 	data_size <int>		Number of data points.  When reading data
//				points from a file, this indicates the
//				maximum number of points for storage
//				allocation.
//	query_size <int>	Number of query points.
//	std_dev <float>		Standard deviation (used in gauss and
//				clus_gauss distributions). Default = 1.
//	corr_coef <float>	Correlation coefficient (used in co-gauss
//				and co_lapace distributions). Default = 0.05.
//	colors <int>		Number of color classes (clusters) (used
//				in the clus_gauss distribution).  Default = 5.
//	distribution <string>	Type of input distribution
//				    uniform	= uniform over cube [-1,1]^d.
//				    gauss	= Gaussian with mean 0
//				    laplace	= Laplacian, mean 0 and var 1
//				    co_gauss	= correlated Gaussian
//				    co_laplace	= correlated Laplacian
//				    clus_gauss	= clustered Gaussian
//				    par_lines	= uniform on 2 parallel lines
//				See the file rand.cc for further information.
//
// Options affecting nearest neighbor search:
// ------------------------------------------
//	epsilon <float>		Error bound for approx. near neigh. search.
//	near_neigh <int>	Number of nearest neighbors to compute.
//	max_pts_visit <int>	Maximum number of points to visit before
//				terminating.  (Used in applications where
//				real-time performance is important.)
//				(Default = 0, which means no limit.)
//
// Options affection general program behavior:
// -------------------------------------------
//	stats <string>		Level of statistics output
//				    silent	 = no output,
//				    exec_time	+= execution time only
//				    prep_stats	+= preprocessing statistics
//				    query_stats	+= query performance stats
//				    query_res	+= results of queries
//				    show_pts	+= show the data points
//				    show_struct += print search structure
//	validate <string>	Validate experiment and compute average
//				error.  Since validation causes exact
//				nearest neighbors to be computed by the
//				brute force method, this can take a long
//				time.  Valid arguments are:
//				    on		= turn validation on
//				    off		= turn validation off
//	true_near_neigh <int>	Number of true nearest neighbors to compute.
//				When validating, we compute the difference
//				in rank between each reported nearest neighbor
//				and the true nearest neighbor of the same
//				rank.  Thus it is necessary to compute a
//				few more true nearest neighbors.  By default
//				we compute 10 more than near_neigh.  With
//				this option the exact number can be set.
//				(Used only when validating.)
//
// Example:
// --------
//  output_label test_run_0	(output label for this run)
//    validate off		(do not perform validation)
//    dim 16			(points in dimension 16)
//    stats query_stats		(output performance statistics for queries)
//    seed 121212		(random number seed)
//    data_size 1000
//    distribution uniform
//  gen_data_pts		(1000 uniform data points in dim 16)
//    query_size 100
//    std_dev 0.05
//    distribution clus_gauss
//  gen_query_pts		(100 points in 10 clusters with std_dev 0.05)
//    bucket_size 2
//    split_rule standard
//    shrink_rule none
//  build_ann			(kd-tree; standard split, bucket size 2)
//    epsilon 0.1
//    near_neigh 5
//    max_pts_visit 100		(stop search if more than 100 points seen)
//  run_queries standard	(run queries; 5 nearest neighbors, 10% error)
//    data_size 500
//  read_data_pts data.in	(read up to 500 points from file data.in)
//    split_rule sl_midpt
//    shrink_rule simple
//  build_ann			(bd-tree; simple shrink, sliding midpoint split)
//    epsilon 0
//  run_queries	priority	(run same queries; 0 allowable error)
//
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//  Constants
//------------------------------------------------------------------------

const int	STRING_LEN 	= 500;		// max string length
const double	ERR 		= 0.00001;	// epsilon (for float compares)

//------------------------------------------------------------------------
//  Enumerated values and conversions
//------------------------------------------------------------------------

typedef enum {DATA, QUERY} PtType;	// point types

//------------------------------------------------------------------------
//  Statistics output levels
//------------------------------------------------------------------------

typedef enum {			// stat levels
	SILENT,				// no output
	EXEC_TIME,			// just execution time
	PREP_STATS,			// preprocessing info
	QUERY_STATS,			// query performance
	QUERY_RES,			// query results
	SHOW_PTS,			// show data points
	SHOW_STRUCT,			// show tree structure
	N_STAT_LEVELS}			// number of levels
	StatLev;

const char stat_table[N_STAT_LEVELS][STRING_LEN] = {
	"silent",			// SILENT
	"exec_time",			// EXEC_TIME
	"prep_stats",			// PREP_STATS
	"query_stats",			// QUERY_STATS
	"query_res",			// QUERY_RES
	"show_pts",			// SHOW_PTS
	"show_struct"};			// SHOW_STRUCT

//------------------------------------------------------------------------
//  Distributions
//------------------------------------------------------------------------

typedef enum {			// distributions
	UNIFORM,			// uniform over cube [-1,1]^d.
	GAUSS,				// Gaussian with mean 0
	LAPLACE,			// Laplacian, mean 0 and var 1
	CO_GAUSS,			// correlated Gaussian
	CO_LAPLACE,			// correlated Laplacian
	CLUS_GAUSS,			// clustered Gaussian
	PAR_LINES,			// uniform on 2 parallel lines
	N_DISTRIBS}
	Distrib;

const char distr_table[N_STAT_LEVELS][STRING_LEN] = {
	"uniform",			// UNIFORM
	"gauss",			// GAUSS
	"laplace",			// LAPLACE
	"co_gauss",			// CO_GAUSS
	"co_laplace",			// CO_LAPLACE
	"clus_gauss",			// CLUS_GAUSS
	"par_lines"};			// PAR_LINES

//------------------------------------------------------------------------
//  Splitting rules for kd-trees (see ANN.h for types)
//------------------------------------------------------------------------

const int N_SPLIT_RULES = 6;
const char split_table[N_SPLIT_RULES][STRING_LEN] = {
	"standard",			// standard optimized kd-tree
	"fair",				// fair split
	"midpt",			// midpoint split
	"sl_midpt",			// sliding midpt split
	"sl_fair",			// sliding fair split
	"suggest"};			// authors' choice for best

//------------------------------------------------------------------------
//  Shrinking rules for bd-trees (see ANN.h for types)
//------------------------------------------------------------------------

const int N_SHRINK_RULES = 4;
const char shrink_table[N_SHRINK_RULES][STRING_LEN] = {
	"none",				// perform no shrinking (kd-tree)
	"simple",			// simple shrinking
	"centroid",			// centroid shrinking
	"suggest"};			// authors' choice for best

//----------------------------------------------------------------------
//  Short utility functions
//	Error - general error routine
//	printPoint - print a point to standard output
//	lookUp - look up a name in table and return index
//----------------------------------------------------------------------

void Error(				// error routine
    char		*msg,		// error message
    ANNerr		level)		// abort afterwards
{
    if (level == ANNabort) {
	cerr << "ann_test: ERROR------->" << msg << "<-------------ERROR\n";
	exit(1);
    }
    else {
	cerr << "ann_test: WARNING----->" << msg << "<-------------WARNING\n";
    }
}

void printPoint(			// print point
    ANNpoint		p,		// the point
    int			dim)		// the dimension
{
    cout << "[";
    for (int i = 0; i < dim; i++) {
	cout << p[i];
	if (i < dim-1) cout << ",";
    }
    cout << "]";
}

int lookUp(				// look up name in table
    const char	*arg,			// name to look up
    const char	(*table)[STRING_LEN],	// name table
    int		size)			// table size
{
    int i;
    for (i = 0; i < size; i++) {
	if (!strcmp(arg, table[i])) return i;
    }
    return i;
}

//------------------------------------------------------------------------
// Function declarations
//------------------------------------------------------------------------

void generatePts(			// generate data/query points
    ANNpointArray	&pa,		// point array (returned)
    int			n,		// number of points
    PtType		type);		// point type

void readPts(				// read data/query points from file
    ANNpointArray	&pa,		// point array (returned)
    int			&n,		// number of points
    char		*file_nm,	// file name
    PtType		type);		// point type (DATA, QUERY)

void doValidation();			// perform validation
void getTrueNN();			// compute true nearest neighbors

void treeStats(				// print statistics on kd- or bd-tree
    ostream		&out,		// output stream
    ANNbool		verbose);	// print stats

//------------------------------------------------------------------------
//  Default execution parameters
//------------------------------------------------------------------------
const int	extra_nn	= 10;		// how many extra true nn's?

const int	def_dim		= 2;		// def dimension
const int	def_n_color	= 5;		// def number of colors
const int	def_data_size	= 100;		// def data size
const int	def_query_size	= 100;		// def number of queries
const Distrib	def_distr	= UNIFORM;	// def distribution
const double	def_std_dev	= 1.00;		// def standard deviation
const double	def_corr_coef	= 0.05;		// def correlation coef
const int	def_bucket_size = 1;		// def bucket size
const double	def_epsilon	= 0.0;		// def error bound
const int	def_near_neigh	= 1;		// def number of near neighbors
const int	def_max_visit	= 0;		// def number of points visited
						// def number of true nn's
const int	def_true_nn	= def_near_neigh + extra_nn;
const int	def_seed	= 0;		// def seed for random numbers
const ANNbool	def_validate	= ANNfalse;	// def validation flag
						// def statistics output level
const StatLev	def_stats	= QUERY_STATS;
const ANNsplitRule				// def splitting rule
		def_split	= ANN_KD_SUGGEST;
const ANNshrinkRule				// def shrinking rule
		def_shrink	= ANN_BD_NONE;

//------------------------------------------------------------------------
//  Global variables - Execution options
//------------------------------------------------------------------------

int		dim; 			// dimension
int		n_color;		// number of colors
int		data_size; 		// data size
int		query_size; 		// number of queries
Distrib		distr; 			// distribution
double		corr_coef; 		// correlation coef
double		std_dev; 		// standard deviation
int		bucket_size; 		// bucket size
double		epsilon; 		// error bound
int		near_neigh; 		// number of near neighbors
int		max_pts_visit; 		// max number of points to visit
int		true_nn; 		// number of true nn's
ANNbool		validate; 		// validation flag
StatLev		stats; 			// statistics output level
ANNsplitRule  	split; 			// splitting rule
ANNshrinkRule	shrink; 		// shrinking rule

//------------------------------------------------------------------------
//  More globals - pointers to dynamically allocated arrays and structures
//
//	It is assumed that all these values are set to NULL when nothing
//	is allocated.
//
//	data_pts, query_pts		the data and query points
//	the_tree			points to the kd- or bd-tree for
//					nearest neighbor searching.
//	apx_nn_idx, apx_dists		record approximate near neighbor
//					indices and distances
//	true_nn_idx, true_dists		record true near neighbor
//					indices and distances
//
//	The approximate and true nearest neighbor results are stored
//	in: apx_nn_idx, apx_dists, and true_nn_idx, true_dists.
//	They are really flattened 2-dimensional arrays.  Each of these
//	arrays consists of query_size blocks, each of which contains
//	near_neigh (or true_nn) entries, one for each of the nearest
//	neighbors for a given query point.
//------------------------------------------------------------------------

ANNpointArray	data_pts;		// data points
ANNpointArray	query_pts;		// query points
ANNbd_tree	*the_tree;		// kd- or bd-tree search structure
ANNidxArray	apx_nn_idx;		// storage for near neighbor indices
ANNdistArray	apx_dists;		// storage for near neighbor distances
ANNidxArray	true_nn_idx;		// true near neighbor indices
ANNdistArray	true_dists;		// true near neighbor distances

ANNbool		valid_dirty;		// validation is no longer valid

//------------------------------------------------------------------------
//  Initialize global parameters
//------------------------------------------------------------------------

void initGlobals()
{
    dim			= def_dim;		// init execution parameters
    n_color		= def_n_color;
    data_size		= def_data_size;
    query_size		= def_query_size;
    distr		= def_distr;
    corr_coef		= def_corr_coef;
    std_dev		= def_std_dev;
    n_color		= def_n_color;
    bucket_size		= def_bucket_size;
    epsilon		= def_epsilon;
    near_neigh		= def_near_neigh;
    max_pts_visit	= def_max_visit;
    true_nn		= def_true_nn;
    valid_dirty 	= ANNtrue;		// (validation must be done)
    validate		= def_validate;
    stats		= def_stats;
    split		= def_split;
    shrink		= def_shrink;
    idum		= -def_seed;		// init. global seed for ran0()

    data_pts	= NULL;				// initialize storage pointers
    query_pts	= NULL;
    the_tree	= NULL;
    apx_nn_idx	= NULL;
    apx_dists	= NULL;
    true_nn_idx	= NULL;
    true_dists	= NULL;
}

//------------------------------------------------------------------------
// main program - driver
//	The main program reads input options, invokes the necessary
//	routines to process them.
//------------------------------------------------------------------------

int main()
{
    long	clock0;				// clock time
    char	directive[STRING_LEN];		// input directive
    char	arg[STRING_LEN];		// all-purpose argument

    initGlobals();				// initialize global values

    //--------------------------------------------------------------------
    //  Main input loop
    //--------------------------------------------------------------------
						// read input directive
    while (cin >> directive) {
	//----------------------------------------------------------------
	//  Read options
	//----------------------------------------------------------------
	if (!strcmp(directive,"dim")) {
	    cin >> dim;
	}
	else if (!strcmp(directive,"colors")) {
	    cin >> n_color;
	}
	else if (!strcmp(directive,"std_dev")) {
	    cin >> std_dev;
	}
	else if (!strcmp(directive,"corr_coef")) {
	    cin >> corr_coef;
	}
	else if (!strcmp(directive, "data_size")) {
	    cin >> data_size;
	}
	else if (!strcmp(directive,"query_size")) {
	    cin >> query_size;
	}
	else if (!strcmp(directive,"bucket_size")) {
	    cin >> bucket_size;
	}
	else if (!strcmp(directive,"epsilon")) {
	    cin >> epsilon;
	}
	else if (!strcmp(directive,"max_pts_visit")) {
	    cin >> max_pts_visit;
	}
	else if (!strcmp(directive,"near_neigh")) {
	    cin >> near_neigh;
	    true_nn = near_neigh + extra_nn;	// also reset true near neighs
	    valid_dirty = ANNtrue;		// validation must be redone
	}
	else if (!strcmp(directive,"true_near_neigh")) {
	    cin >> true_nn;
	    valid_dirty = ANNtrue;		// validation must be redone
	}
	//----------------------------------------------------------------
	//  seed option
	//	The seed is reset by setting the global idum to the
	//	negation of the seed value.  See rand.cc.
	//----------------------------------------------------------------
	else if (!strcmp(directive,"seed")) {
	    cin >> idum;
	    idum = -idum;
	}
	//----------------------------------------------------------------
	//  validate option
	//----------------------------------------------------------------
	else if (!strcmp(directive,"validate")) {
	    cin >> arg;				// input argument
	    if (!strcmp(arg, "on")) {
	    	validate = ANNtrue;
		cout << "validate = on   "
		     << "(Warning: this may slow execution time.)\n";
	    }
	    else if (!strcmp(arg, "off")) {
	    	validate = ANNfalse;
	    }
	    else {
		Error("validate argument must be \"on\" or \"off\"", ANNabort);
	    }
	}
	//----------------------------------------------------------------
	//  distribution option
	//----------------------------------------------------------------
	else if (!strcmp(directive,"distribution")) {
	    cin >> arg;				// input name and translate
	    distr = (Distrib) lookUp(arg, distr_table, N_DISTRIBS);
	    if (distr >= N_DISTRIBS) {		// not something we recognize
		Error("Unknown distribution level", ANNabort);
	    }
	}
	//----------------------------------------------------------------
	//  stats option
	//----------------------------------------------------------------
	else if (!strcmp(directive,"stats")) {
	    cin >> arg;				// input name and translate
	    stats = (StatLev) lookUp(arg, stat_table, N_STAT_LEVELS);
	    if (stats >= N_STAT_LEVELS) {	// not something we recognize
		Error("Unknown statistics level", ANNabort);
	    }
	    if (stats > SILENT)
	    	cout << "stats = " << arg << "\n";
	}
	//----------------------------------------------------------------
	//  split_rule option
	//----------------------------------------------------------------
	else if (!strcmp(directive,"split_rule")) {
	    cin >> arg;				// input split_rule name
	    split = (ANNsplitRule) lookUp(arg, split_table, N_SPLIT_RULES);
	    if (split >= N_SPLIT_RULES) {	// not something we recognize
		Error("Unknown splitting rule", ANNabort);
	    }
	}
	//----------------------------------------------------------------
	//  shrink_rule option
	//----------------------------------------------------------------
	else if (!strcmp(directive,"shrink_rule")) {
	    cin >> arg;				// input split_rule name
	    shrink = (ANNshrinkRule) lookUp(arg, shrink_table, N_SHRINK_RULES);
	    if (shrink >= N_SHRINK_RULES) {	// not something we recognize
		Error("Unknown shrinking rule", ANNabort);
	    }
	}
	//----------------------------------------------------------------
	//  label operation
	//----------------------------------------------------------------
	else if (!strcmp(directive,"output_label")) {
	    cin >> arg;
	    if (stats > SILENT)
	    	cout << "<" << arg << ">\n";
	}
	//----------------------------------------------------------------
	//  gen_data_pts operation
	//----------------------------------------------------------------
	else if (!strcmp(directive,"gen_data_pts")) {
	    generatePts(			// generate data points
		data_pts,			// data points
		data_size,			// number of data points
		DATA);				// data points
	    valid_dirty = ANNtrue;		// validation must be redone
	}
	//----------------------------------------------------------------
	//  gen_query_pts operation
	//----------------------------------------------------------------
	else if (!strcmp(directive,"gen_query_pts")) {
	    generatePts(			// generate data points
		query_pts,			// point array
		query_size,			// number of points
		QUERY);				// query points
	    valid_dirty = ANNtrue;		// validation must be redone
	}
	//----------------------------------------------------------------
	//  read_data_pts operation
	//----------------------------------------------------------------
	else if (!strcmp(directive,"read_data_pts")) {
	    cin >> arg;				// input file name
	    readPts(
		data_pts,			// point array
		data_size,			// number of points
		arg,				// file name
		DATA);				// data points
	    valid_dirty = ANNtrue;		// validation must be redone
	}
	//----------------------------------------------------------------
	//  read_query_pts operation
	//----------------------------------------------------------------
	else if (!strcmp(directive,"read_query_pts")) {
	    cin >> arg;				// input file name
	    readPts(
		query_pts,			// point array
		query_size,			// number of points
		arg,				// file name
		QUERY);				// query points
	    valid_dirty = ANNtrue;		// validation must be redone
	}
	//----------------------------------------------------------------
	//  build_ann operation
	//	We always invoke the constructor for bd-trees.  Note
	//	that when the shrinking rule is NONE (which is true by
	//	default), then this constructs a kd-tree.
	//----------------------------------------------------------------
	else if (!strcmp(directive,"build_ann")) {
	    //------------------------------------------------------------
	    //  Build the tree
	    //------------------------------------------------------------
	    if (the_tree != NULL) {		// tree exists already
		delete the_tree;		// get rid of it
	    }
	    clock0 = clock();			// start time

	    the_tree = new ANNbd_tree(		// build it
		    data_pts,			// the data points
		    data_size,			// number of points
		    dim,			// dimension of space
		    bucket_size,		// maximum bucket size
		    split,			// splitting rule
		    shrink);			// shrinking rule

	    //------------------------------------------------------------
	    //  Print summary
	    //------------------------------------------------------------
	    long prep_time = clock() - clock0;	// end of prep time

	    if (stats > SILENT) {
	    	cout << "[Build ann-structure:\n";
		cout << "  split_rule    = " << split_table[split] << "\n";
		cout << "  shrink_rule   = " << shrink_table[shrink] << "\n";
	    	cout << "  data_size     = " << data_size << "\n";
	    	cout << "  dim           = " << dim << "\n";
	    	cout << "  bucket_size   = " << bucket_size << "\n";

		if (stats >= EXEC_TIME) {	// output processing time
		    cout << "  process_time  = "
			 << double(prep_time)/CLOCKS_PER_SEC << " sec\n";
		}

		if (stats >= PREP_STATS)	// output or check tree stats
		    treeStats(cout, ANNtrue);	// print tree stats
		else
		    treeStats(cout, ANNfalse);	// check stats

		if (stats >= SHOW_STRUCT) {	// print the whole tree
		    cout << "  (Structure Contents:\n";
	    	    the_tree->Print(ANNfalse, cout);
		    cout << "  )\n";
		}
	    	cout << "]\n";
	    }
	}
	//----------------------------------------------------------------
	//  dump operation
	//----------------------------------------------------------------
	else if (!strcmp(directive,"dump")) {
	    cin >> arg;				// input file name
	    if (the_tree == NULL) {		// no tree
		Error("Cannot dump.  No tree has been built yet", ANNwarn);
	    }
	    else {				// there is a tree
						// try to open file
		ofstream out_dump_file(arg);
		if (!out_dump_file)
		    Error("Cannot open dump file", ANNabort);
						// dump the tree and points
		the_tree->Dump(ANNtrue, out_dump_file);
		if (stats > SILENT) {
	    	    cout << "(Tree has been dumped to file " << arg << ")\n";
		}
	    }
	}
	//----------------------------------------------------------------
	//  run_queries operation
	//	This section does all the query processing.  It consists
	//	of the following subsections:
	//
	//	**  input the argument (standard or priority) and output
	//	    the header describing the essential information.
	//	**  allocate space for the results to be stored.
	//	**  run the queries by invoking the appropriate search
	//	    procedure on the query points.  Print nearest neighbor
	//	    if requested.
	//	**  print final summaries
	//----------------------------------------------------------------
	else if (!strcmp(directive,"run_queries")) {

	    //------------------------------------------------------------
	    //  Input arguments and print summary
	    //------------------------------------------------------------
	    enum {STANDARD, PRIORITY} method;

	    cin >> arg;				// input argument
	    if (!strcmp(arg, "standard")) {
	    	method = STANDARD;
	    }
	    else if (!strcmp(arg, "priority")) {
	    	method = PRIORITY;
	    }
	    else {
		Error("Search type must be \"standard\" or \"priority\"",
			ANNabort);
	    }
	    if (data_pts == NULL || query_pts == NULL) {
		Error("Either data set and query set not constructed", ANNabort);
	    }
	    if (the_tree == NULL) {
		Error("No search tree built.", ANNabort);
	    }

	    //------------------------------------------------------------
	    //  Set up everything
	    //------------------------------------------------------------

	    #ifdef PERF				// performance only
		reset_stats(data_size);		// reset statistics
	    #endif

	    clock0 = clock();			// start time
						// deallocate existing storage
	    if (apx_nn_idx != NULL) delete [] apx_nn_idx;
	    if (apx_dists  != NULL) delete [] apx_dists;
						// allocate apx answer storage
	    apx_nn_idx = new int[near_neigh*query_size];
	    apx_dists  = new ANNdist[near_neigh*query_size];

	    annMaxPtsVisit(max_pts_visit);	// set max points to visit

	    //------------------------------------------------------------
	    //  Run the queries
	    //------------------------------------------------------------
						// pointers for current query
	    int		 *curr_nn_idx = apx_nn_idx;
	    ANNdistArray  curr_dists  = apx_dists;

	    for (int i = 0; i < query_size; i++) {
		#ifdef PERF
		    reset_counts();		// reset counters
		#endif

		switch (method) {
		case STANDARD:			// standard search
		    the_tree->annkSearch(
			query_pts[i],		// query point
			near_neigh,		// number of near neighbors
			curr_nn_idx,		// nearest neighbors (returned)
	    		curr_dists,		// distance (returned)
			epsilon);		// error bound
		    break;
		case PRIORITY:			// priority search
		    the_tree->annkPriSearch(
			query_pts[i],		// query point
			near_neigh,		// number of near neighbors
			curr_nn_idx,		// nearest neighbors (returned)
	    		curr_dists,		// distance (returned)
			epsilon);		// error bound
		    break;
		}
		curr_nn_idx += near_neigh;	// increment current pointers
		curr_dists  += near_neigh;

		#ifdef PERF
		    update_stats();		// update stats
		#endif
	    }

	    long query_time = clock() - clock0;	// end of query time

	    if (validate) {			// validation requested
		if (valid_dirty) getTrueNN();	// get true near neighbors
		doValidation();			// validate
	    }

	    //------------------------------------------------------------
	    //  Print summaries
	    //------------------------------------------------------------
	
	    if (stats > SILENT) {
	    	cout << "[Run Queries:\n";
	    	cout << "  query_size    = " << query_size << "\n";
	    	cout << "  dim           = " << dim << "\n";
	    	cout << "  search_method = " << arg << "\n";
	    	cout << "  epsilon       = " << epsilon << "\n";
	    	cout << "  near_neigh    = " << near_neigh << "\n";
		if (max_pts_visit)
		    cout << "  max_pts_visit = " << max_pts_visit << "\n";
		if (validate)
	    	    cout << "  true_nn       = " << true_nn << "\n";

		if (stats >= EXEC_TIME) {	// print exec time summary
		    cout << "  query_time    = " <<
			double(query_time)/(query_size*CLOCKS_PER_SEC)
			 << " sec/query";
		    #ifdef PERF
			cout << " (biased by perf measurements)";
		    #endif
		    cout << "\n";
		}

		if (stats >= QUERY_STATS) {	// output performance stats
		    #ifdef PERF
	    		print_stats(validate);
	    	    #else
			cout << "  (Performance statistics unavailable.)\n";
	    	    #endif
		}

		if (stats >= QUERY_RES) {	// output results
		    cout << "  (Query Results:\n";
		    cout << "    Pt\tANN\tDist\n";
		    curr_nn_idx = apx_nn_idx;
		    curr_dists  = apx_dists;
		    for (int i = 0; i < query_size; i++) {
			cout << "    " << i;
			for (int j = 0; j < near_neigh; j++) {
			    cout << "\t" << curr_nn_idx[j]
				 << "\t" << ANN_ROOT(curr_dists[j]) << "\n";
			}
			curr_nn_idx += near_neigh;// incr current pointers
			curr_dists  += near_neigh;
		    }
		    cout << "  )\n";
		}
	    	cout << "]\n";
	    }
	}
	//----------------------------------------------------------------
	//  Unknown directive
	//----------------------------------------------------------------
	else {
	    Error("Unknown directive", ANNabort);
	}
    }
    //--------------------------------------------------------------------
    //  End of input loop (deallocate stuff that was allocated)
    //--------------------------------------------------------------------
    if (the_tree  != NULL) { delete the_tree;   the_tree  = NULL; }
    if (data_pts != NULL)  { annDeallocPts(data_pts);  data_pts  = NULL; }
    if (query_pts != NULL) { annDeallocPts(query_pts); query_pts = NULL; }
	
	return 0;
}

//------------------------------------------------------------------------
// generatePts - call appropriate routine to generate points of a
//	given distribution.
//------------------------------------------------------------------------

void generatePts(
    ANNpointArray	&pa,		// point array (returned)
    int			n,		// number of points
    PtType		type)		// point type
{
    if (pa != NULL) annDeallocPts(pa);		// get rid of any old points
    pa = annAllocPts(n, dim);			// allocate point storage

    switch (distr) {
	case UNIFORM:				// uniform over cube [-1,1]^d.
	    uniform_pts(pa, n, dim);
	    break;
	case GAUSS:				// Gaussian with mean 0
	    gauss_pts(pa, n, dim, std_dev);
	    break;
	case LAPLACE:				// Laplacian, mean 0 and var 1
	    laplace_pts(pa, n, dim);
	    break;
	case CO_GAUSS:				// correlated Gaussian
	    co_gauss_pts(pa, n, dim, corr_coef);
	    break;
	case CO_LAPLACE:			// correlated Laplacian
	    co_laplace_pts(pa, n, dim, corr_coef);
	    break;
	case CLUS_GAUSS:			// clustered Gaussian
	    clus_gauss_pts(pa, n, dim, n_color, ANNfalse, std_dev);
	    break;
	case PAR_LINES:				// uniform on 2 parallel lines
	    par_lines_pts(pa, n, dim);
	    break;
	default:
	    Error("INTERNAL ERROR: Unknown distribution", ANNabort);
	    break;
    }

    if (stats > SILENT) {
	if(type == DATA) cout << "[Generating Data Points:\n";
	else             cout << "[Generating Query Points:\n";
    	cout << "  size          = " << n << "\n";
    	cout << "  dim           = " << dim << "\n";
    	cout << "  distribution  = " << distr_table[distr] << "\n";
	if (idum < 0)
	    cout << "  seed          = " << idum << "\n";
	if (distr == GAUSS || distr == CLUS_GAUSS)
    	    cout << "  std_dev       = " << std_dev << "\n";
	if (distr == CO_GAUSS || distr == CO_LAPLACE)
    	    cout << "  corr_coef     = " << corr_coef << "\n";
	if (distr == CLUS_GAUSS)
    	    cout << "  colors        = " << n_color << "\n";
    }
						// want to see points?
    if ((type == DATA  && stats >= SHOW_PTS) ||
	(type == QUERY && stats >= QUERY_RES)) {
	if(type == DATA) cout << "(Data Points:\n";
	else             cout << "(Query Points:\n";
	for (int i = 0; i < n; i++) {
	    cout << "    " << i << "\t";
	    printPoint(pa[i], dim);
	    cout << "\n";
	}
	cout << "  )\n";
    }
    cout << "]\n";
}

//------------------------------------------------------------------------
// readPts - read a collection of data or query points.
//------------------------------------------------------------------------

void readPts(
    ANNpointArray	&pa,		// point array (returned)
    int			&n,		// number of points
    char		*file_nm,	// file name
    PtType		type)		// point type (DATA, QUERY)
{
    int i;
    //--------------------------------------------------------------------
    //  Open input file and read points
    //--------------------------------------------------------------------
    ifstream in_file(file_nm);			// try to open data file
    if (!in_file)
	Error("Cannot open input data/query file", ANNabort);
						// allocate storage for points
    if (pa != NULL) annDeallocPts(pa);		// get rid of old points
    pa = annAllocPts(n, dim);

    for (i = 0; i < n; i++) {			// read the data
	if (!(in_file >> pa[i][0])) break;
	for (int d = 1; d < dim; d++) {
	    in_file >> pa[i][d];
	}
    }

    char ignore_me;				// character for EOF test
    in_file >> ignore_me;			// try to get one more character
    if (!in_file.eof()) {			// exhausted space before eof
	if (type == DATA) 
	    Error("`data_size' too small. Input file truncated.", ANNwarn);
	else
	    Error("`query_size' too small. Input file truncated.", ANNwarn);
    }
    n = i;					// number of points read

    //--------------------------------------------------------------------
    //  Print summary
    //--------------------------------------------------------------------
    if (stats > SILENT) {
    	if (type == DATA) {
	    cout << "[Read Data Points:\n";
	    cout << "  data_size  = " << n << "\n";
	}
    	else {
	    cout << "[Read Query Points:\n";
	    cout << "  query_size  = " << n << "\n";
	}
	cout << "  file_name  = " << file_nm << "\n";
    	cout << "  dim        = " << dim << "\n";
						// print if results requested
	if ((type == DATA && stats >= SHOW_PTS) ||
	    (type == QUERY && stats >= QUERY_RES)) {
	    cout << "  (Points:\n";
	    for (i = 0; i < n; i++) {
		cout << "    " << i << "\t";
		printPoint(pa[i], dim);
		cout << "\n";
	    }
	    cout << "  )\n";
	}
    	cout << "]\n";
    }
}

//------------------------------------------------------------------------
//  getTrueNN
//	Computes the true nearest neighbors.  For purposes of validation,
//	this intentionally done in a rather dumb (but safe way), by
//	invoking the brute-force search.
//
//	The number of true nearest neighbors is somewhat larger than
//	the number of nearest neighbors.  This is so that the validation
//	can determine the expected difference in element ranks.
//
//	This procedure is invoked just prior to running queries.  Since
//	the operation takes a long time, it is performed only if needed.
//	In particular, once generated, it will be regenerated only if
//	new query or data points are generated, or if the requested number
//	of true near neighbors or approximate near neighbors has changed.
//------------------------------------------------------------------------

void getTrueNN()			// compute true nearest neighbors
{
    if (stats > SILENT) {
	cout << "(Computing true nearest neighbors for validation.)\n";
    }
						// deallocate existing storage
    if (true_nn_idx != NULL) delete [] true_nn_idx;
    if (true_dists  != NULL) delete [] true_dists;

    if (true_nn > data_size) {			// cannot get more true nn than points
	true_nn = data_size;
    }

    true_nn_idx = new int[true_nn*query_size];	// allocate true answer storage
    true_dists  = new ANNdist[true_nn*query_size];

    ANNidxArray	 curr_nn_idx = true_nn_idx;	// current locations in arrays
    ANNdistArray curr_dists = true_dists;

						// allocate search structure
    ANNbruteForce *the_brute = new ANNbruteForce(data_pts, data_size, dim);
						// compute nearest neighbors
    for (int i = 0; i < query_size; i++) {
	the_brute->annkSearch(			// compute true near neighbors
			query_pts[i],		// query point
			true_nn,		// number of nearest neighbors
			curr_nn_idx,		// where to put indices
			curr_dists);		// where to put distances
	curr_nn_idx += true_nn;			// increment nn index pointer
	curr_dists  += true_nn;			// increment nn dist pointer
    }
    delete the_brute;				// delete brute-force struct
    valid_dirty = ANNfalse;			// validation good for now
}

//------------------------------------------------------------------------
//  doValidation
//	Compares the approximate answers to the k-nearest neighbors
//	against the true nearest neighbors (computed earlier).  It is
//	assumed that the true nearest neighbors and indices have been
//	computed earlier.
//
//	We compute two quantities:
//
//	Average Error
//	The relative error between the distance to a reported nearest
//	neighbor and the true nearest neighbor (of the same rank),
//
//	Rank Error
//	The difference in rank between the reported nearest neighbor and
//	its position (if any) among the true nearest neighbors.  If we
//	cannot find this point among the true nearest neighbors, then
//	it assumed that the rank of the true nearest neighbor is true_nn+1.
//
//	Because of the possibility of duplicate distances, this is computed
//	as follows.  For the j-th reported nearest neighbor, we count the
//	number of true nearest neighbors that are at least this close.  Let
//	this be rnk.  Then the rank error is max(0, j-rnk).  (In the code
//	below, j is an array index and so the first item is 0, not 1.  Thus
//	we take max(0, j+1-rnk) instead.)
//
//------------------------------------------------------------------------

void doValidation()			// perform validation
{
    int		 *curr_apx_idx = apx_nn_idx;	// approx index pointer
    ANNdistArray  curr_apx_dst = apx_dists;	// approx distance pointer
    int		 *curr_tru_idx = true_nn_idx;	// true index pointer
    ANNdistArray  curr_tru_dst = true_dists;	// true distance pointer
    int i, j;
    if (true_nn < near_neigh) {
	Error("Cannot validate with fewer true near neighbors than actual", ANNabort);
    }

    for (i = 0; i < query_size; i++) {		// validate each query
	//--------------------------------------------------------------------
	//	Compute average errors
	//--------------------------------------------------------------------
    	for (j = 0; j < near_neigh; j++) {
	    double avgErr = 0.0;		// average error
						// true i-th smallest distance
	    double true_dist = ANN_ROOT(curr_tru_dst[j]);
						// reported i-th smallest
	    double rept_dist = ANN_ROOT(curr_apx_dst[j]);
						// better than optimum?
	    if (rept_dist < true_dist*(1-ERR)) {
		Error("INTERNAL ERROR: True nearest neighbor incorrect", ANNabort);
	    }

	    if (true_dist == 0.0) {		// let's not divide by zero
		if (rept_dist != 0.0) avgErr = MAXDOUBLE;
		else		      avgErr = 0.0;
	    }
	    else {
		avgErr = (rept_dist - true_dist) / ((double) true_dist);
	    }

	    if (avgErr > epsilon && max_pts_visit == 0) {
		Error("INTERNAL ERROR: Average error exceeds epsilon", ANNabort);
	    }
	    #ifdef PERF
		average_err += avgErr;		// update statistics error
	    #endif
	}
	//--------------------------------------------------------------------
	//	Compute rank errors  (only needed for perf measurements)
	//--------------------------------------------------------------------
	#ifdef PERF
	    for (j = 0; j < near_neigh; j++) {
		double rnkErr = 0.0;		// rank error
						// reported j-th distance
		ANNdist rept_dist = curr_apx_dst[j];
		int rnk = 0;			// compute rank of this item
		while (rnk < true_nn && curr_tru_dst[rnk] <= rept_dist) rnk++;
		if (j+1-rnk > 0) rnkErr = (double) (j+1-rnk);
		rank_err += rnkErr;		// update average rank error
	    }
	#endif
	curr_apx_idx += near_neigh;
	curr_apx_dst += near_neigh;
	curr_tru_idx += true_nn;		// increment current pointers
	curr_tru_dst += true_nn;
    }
}

//----------------------------------------------------------------------
//  treeStats
//	Computes a number of statistics related to kd_trees and
//	bd_trees.  These statistics are printed if in verbose mode,
//	and otherwise they are only printed if they are deemed to
//	be outside of reasonable operating bounds.
//----------------------------------------------------------------------

#define log2(x) (log(x)/log(2.0))		// log base 2

void treeStats(
    ostream	&out,				// output stream
    ANNbool	verbose)			// print stats
{
    const int   MIN_PTS     = 20;		// min no. pts for checking
    const float MAX_FRAC_TL = 0.50;		// max frac of triv leaves
    const float MAX_AVG_AR  = 20;		// max average aspect ratio

    ANNkdStats st;				// statistics structure

    the_tree->getStats(st);			// get statistics
						// total number of nodes
    int n_nodes = st.n_lf + st.n_spl + st.n_shr;
						// should be O(n/bs)
    int opt_n_nodes = (int) (2*(float(st.n_pts)/st.bkt_size));
    int too_many_nodes = 10*opt_n_nodes;
    if (st.n_pts >= MIN_PTS && n_nodes > too_many_nodes) {
	out << "-----------------------------------------------------------\n";
	out << "Warning: The tree has more than 10x as many nodes as points.\n";
	out << "You may want to consider a different split or shrink method.\n";
	out << "-----------------------------------------------------------\n";
	verbose = ANNtrue;
    }
						// fraction of trivial leaves
    float frac_tl = (st.n_lf == 0 ? 0 : ((float) st.n_tl)/ st.n_lf);
    if (st.n_pts >= MIN_PTS && frac_tl > MAX_FRAC_TL) {
	out << "-----------------------------------------------------------\n";
	out << "Warning: A significant fraction of leaves contain no points.\n";
	out << "You may want to consider a different split or shrink method.\n";
	out << "-----------------------------------------------------------\n";
	verbose = ANNtrue;
    }
						// depth should be O(dim*log n)
    int too_many_levels = (int) (2.0 * st.dim * log2((double) st.n_pts));
    int opt_levels = (int) log2(double(st.n_pts)/st.bkt_size);
    if (st.n_pts >= MIN_PTS && st.depth > too_many_levels) {
	out << "-----------------------------------------------------------\n";
	out << "Warning: The tree is more than 2x as deep as (dim*log n).\n";
	out << "You may want to consider a different split or shrink method.\n";
	out << "-----------------------------------------------------------\n";
	verbose = ANNtrue;
    }
						// average leaf aspect ratio
    if (st.n_pts >= MIN_PTS && st.avg_ar > MAX_AVG_AR) {
	out << "-----------------------------------------------------------\n";
	out << "Warning: Average aspect ratio of cells is quite large.\n";
	out << "This may slow queries depending on the point distribution.\n";
	out << "-----------------------------------------------------------\n";
	verbose = ANNtrue;
    }

    //------------------------------------------------------------------
    //  Print summaries if requested
    //------------------------------------------------------------------
    if (verbose) {				// output statistics
	out << "  (Structure Statistics:\n";
	out << "    n_nodes          = " << n_nodes
	    << " (opt = " << opt_n_nodes
	    << ", best if < " << too_many_nodes << ")\n"
	    << "        n_leaves     = " << st.n_lf
	    << " (" << st.n_tl << " contain no points)\n"
	    << "        n_splits     = " << st.n_spl << "\n"
	    << "        n_shrinks    = " << st.n_shr << "\n";
	out << "    empty_leaves     = " << frac_tl*100
	    << " percent (best if < " << MAX_FRAC_TL*100 << " percent)\n";
	out << "    depth            = " << st.depth
	    << " (opt = " << opt_levels
	    << ", best if < " << too_many_levels << ")\n";
	out << "    avg_aspect_ratio = " << st.avg_ar
	    << " (best if < " << MAX_AVG_AR << ")\n";
	out << "  )\n";
    }
}

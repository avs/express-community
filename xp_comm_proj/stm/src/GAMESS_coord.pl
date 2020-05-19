#!/app/perl/sgi/bin/perl
#
# Usage
#   GAMESS_coord.pl -f GAMESS-log-file-name  
#
# Oct. 20, 1997
# Significant modifications by Theresa Windus to add creation of 
#  different files for each coordinate set and to split the
#  original file name to just get the prefix part of the basename.
#  Also changed the way the file is read in.
#
# Original by Ken Flurchick
#
use File::Basename;
use Getopt::Long;

my $ret = GetOptions ("f|file_name:s");
my $file_name = $opt_f || die "Usage: $0 -f file_name\n";

#
open (INFILE,  "<$file_name") || die "Could not read the $file_name\n";
my $base  = basename($file_name, '');

@fnpat = split /\./, $base;
$count = split /\./, $base;

$file_name = $fnpat[0];
$i=1;

while ($i < ($count - 1)) {
  $file_name = $file_name . '.' . $fnpat[$i];
  $i++;
}

#
$count = 0;
$start = 0;
$num = 1;
#
#
LINE: while($Line = <INFILE>){ 		# read GAMESS log file line by line
	chomp $Line;
	if($Line =~/COORDINATES \(/){
		if($Line =~/GRADIENT/){
			$Line = <INFILE>;
			$Line = <INFILE>;
			$Line = <INFILE>;
			$Line = <INFILE>;
			$Line = <INFILE>;
                        while($Line = <INFILE>){
                          chomp $Line;
                            if($Line ne ""){
			      if($start == 0) {
                	  	$count++;
				if ($count <= 9) {
				  $COORD_file = $file_name . ".00" . $count . '.xyz';
				}
				elsif ($count <= 99) {
				  $COORD_file = $file_name . ".0" . $count . '.xyz';
				}
				elsif ($count <=999) {
				  $COORD_file = $file_name . $count . '.xyz';
				}
				else {
				  die "This script only handles < 1000 coordinate sets\n";
				}
				open (OUTFILE, ">$COORD_file") || 
				  die "Could not overwrite the $COORD_file\n";
				$start ++;
			      }
                              print $Line, "\n";
                              print OUTFILE $Line, "\n";
                            }
                            else {
			      $start = 0;
			      close(OUTFILE);
                              next LINE;
                             }
                        }
		}
		else {
			print $Line, "\n";
			$Line = <INFILE>;
			chomp $Line;
			print $Line, "\n";
			while($Line = <INFILE>){
			  chomp $Line;
			  if($Line ne ""){
			    if($start == 0) {
                	      $count++;
				if ($count <= 9) {
				  $COORD_file = $file_name . ".00" . $count . '.xyz';
				}
				elsif ($count <= 99) {
				  $COORD_file = $file_name . ".0" . $count . '.xyz';
				}
				elsif ($count <=999) {
				  $COORD_file = $file_name . $count . '.xyz';
				}
				else {
				  die "This script only handles < 1000 coordinate sets\n";
				}
				open (OUTFILE, ">$COORD_file") || 
				  die "Could not overwrite the $COORD_file\n";
			      $start ++;
			    }
			    print $Line, "\n";
			    print OUTFILE "  ", $num, " ", $Line, "\n";
			    $num++;
			  }
			  else {
			    $start = 0;
			    close(OUTFILE);
			    next LINE;
			  }
			}
		}
	}
}
close(INFILE);

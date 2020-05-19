#!/bin/perl
sub mmAbout {
  print "Usage: noc2oc <source.noc> <dest.oc>\n";
}

if ($#ARGV < 1 || $#ARGV > 1) {
  &mmAbout();
  exit(1);
}

sub readFile {
    local($file) = "";
    if (!$doNone) {
	open(FILE, "<$_[0]") || 
	    die ("can't open file for reading: $!");
	while (<FILE>) {
	    $file .= $_;
	}
	close (FILE);
    }
    return $file;
}

sub writeFile {
    if (!$doNone) {
	open (FILE, ">$_[0]") ||
	    die ("can't open file for writing: $!\n");
	print FILE $_[1];
	close (FILE);
    }
}

$file = "Octree\n(";
$file .= &readFile($ARGV[0]);
$file =~ s/!\s+!\s*//g;
$file =~ s/1\(/\(/g;
$file .= ")\n";
&writeFile($ARGV[1], $file);

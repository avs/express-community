###
# author: Peter Breitling (breitling@acm.org)
###

#      7 6 5 4 3 2 1 0
#$hash{'0,0,0,0,0,0,0,0'} = "";
#$hash{'0,0,0,0,0,0,0,1'} = "0,2,1";
#$hash{'0,0,0,0,0,0,1,1'} = "1,4,2,1,3,4";
#$hash{'0,0,0,0,1,0,0,1'} = "0,2,1,3,5,7";
#$hash{'1,0,0,0,0,0,0,1'} = "0,2,1,7,b,a";
#$hash{'0,0,0,0,0,1,1,1'} = "3,4,5,2,5,4,2,6,5";
#$hash{'1,0,0,0,0,0,1,1'} = "1,4,2,1,3,4,7,b,a";
#$hash{'0,1,0,0,1,0,0,1'} = "0,2,1,3,5,7,6,9,b";
#$hash{'0,0,0,0,1,1,1,1'} = "2,6,4,4,6,7";
#$hash{'1,0,0,0,0,1,1,1'} = "3,4,5,2,5,4,2,6,5,7,b,a";
#$hash{'0,1,1,0,1,0,0,1'} = "0,2,1,3,5,7,4,a,8,6,9,b";
#$hash{'0,0,0,1,0,1,1,1'} = "3,4,5,4,6,5,4,8,6,6,8,9";
#$hash{'0,1,0,0,0,1,1,1'} = "2,9,4,4,9,5,4,5,3,5,9,b";
#$hash{'1,1,0,0,0,0,1,1'} = "1,4,2,1,3,4,6,9,7,7,9,a";
#$hash{'0,0,0,1,1,0,1,1'} = "4,5,7,4,8,5,1,5,8,1,8,9";

#      7 6 5 4 3 2 1 0
$hash{'0,0,0,0,0,0,0,0'} = "";
$hash{'0,1,0,0,0,0,0,0'} = "9,b,6";
$hash{'1,1,0,0,0,0,0,0'} = "6,9,a,a,7,6";
$hash{'0,1,1,0,0,0,0,0'} = "9,b,6,4,a,8";
$hash{'1,0,0,0,1,1,0,0'} = "b,a,6,6,a,1,1,a,3";
$hash{'1,1,0,0,1,1,0,0'} = "9,a,3,9,3,1";
$hash{'1,0,0,1,1,1,0,0'} = "2,8,9,b,a,6,6,a,1,1,a,3";
$hash{'0,1,1,0,1,0,0,1'} = "6,9,b,0,2,1,8,4,a,3,5,7";
$hash{'0,1,0,0,1,1,0,1'} = "0,2,9,0,9,b,0,b,3,3,b,7";
$hash{'1,0,0,0,1,1,0,1'} = "2,6,b,2,b,3,2,3,0,3,b,a";
$hash{'0,1,0,0,0,0,1,0'} = "9,b,6,0,3,4";
$hash{'1,1,0,0,0,0,1,0'} = "9,a,6,a,7,6,0,3,4";
$hash{'1,0,0,1,0,0,1,0'} = "2,8,9,a,7,b,0,3,4";
$hash{'0,1,0,1,1,0,1,0'} = "0,5,4,4,5,7,2,8,6,8,b,6";
$hash{'0,1,0,0,1,1,1,0'} = "1,9,b,4,1,b,4,0,1,4,b,7";

%final;

sub invert {
  local(%hash) = @_;
  local(%final);
  foreach $i (keys %hash) {
    @con = split ',', $hash{$i};
    @ncon = ();
    $n = ($#con+1) / 3;
    if ($n < 4) {
      for ($j = 0; $j < $n; $j++) {
	$ncon[$j * 3 + 0] = $con[$j * 3 + 0];
	$ncon[$j * 3 + 1] = $con[$j * 3 + 2];
	$ncon[$j * 3 + 2] = $con[$j * 3 + 1];
      }
      @index = split ',', $i;
      for ($j = 0; $j < 8; $j++) {
	if ($index[$j] == 0) {
	  $index[$j] = 1;
	} else {
	  $index[$j] = 0;
	}
      }
      $i = join ',', @index;
      $final{$i} = join ',', @ncon;
    }
  }
  %final;
}

sub mirrorX {
  local(%hash) = @_;
  local(%final);
  foreach $i (keys %hash) {
    @con = split ',', $hash{$i};
    @ncon = ();
    $n = ($#con+1) / 3;
    for ($j = 0; $j < $n; $j++) {
      $ncon[$j * 3 + 0] = $con[$j * 3 + 0];
      $ncon[$j * 3 + 1] = $con[$j * 3 + 2];
      $ncon[$j * 3 + 2] = $con[$j * 3 + 1];
    }
    $nConS = join ',', @ncon;
    $nConS =~ tr /1324679a/314276a9/;
    @index = split ',', $i;
    $nIndex[0] = $index[1]; $nIndex[1] = $index[0];
    $nIndex[2] = $index[3]; $nIndex[3] = $index[2];
    $nIndex[4] = $index[5]; $nIndex[5] = $index[4];
    $nIndex[6] = $index[7]; $nIndex[7] = $index[6];
    $i = join ',', @nIndex;
    $final{$i} = $nConS;
  }  
  %final;
}

sub rotX {
  local(%hash) = @_;
  foreach $i (keys %hash) {
    @con = split ',', $hash{$i};
    @ncon = @con;
    $nConS = join ',', @ncon;
    $nConS =~ tr /129634a708b5/29614a738b50/;
    @index = split ',', $i;
    $nIndex[0] = $index[2]; $nIndex[4] = $index[0];
    $nIndex[6] = $index[4]; $nIndex[2] = $index[6];
    $nIndex[1] = $index[3]; $nIndex[5] = $index[1];
    $nIndex[7] = $index[5]; $nIndex[3] = $index[7];
    $i = join ',', @nIndex;
    $final{$i} = $nConS;
  }  
  %final;
}

sub rotY {
  local(%hash) = @_;
  foreach $i (keys %hash) {
    @con = split ',', $hash{$i};
    @ncon = @con;
    $nConS = join ',', @ncon;
    $nConS =~ tr /028456b719a3/28406b759a31/;
    @index = split ',', $i;
    $nIndex[0] = $index[1]; $nIndex[1] = $index[5];
    $nIndex[5] = $index[4]; $nIndex[4] = $index[0];
    $nIndex[2] = $index[3]; $nIndex[3] = $index[7];
    $nIndex[7] = $index[6]; $nIndex[6] = $index[2];
    $i = join ',', @nIndex;
    $final{$i} = $nConS;
  }  
  %final;
}

sub addToFinal {
  local(%hash) = @_;
  foreach $i (keys %hash) {
    if (! exists $final{$i}) {
      $final{$i} = $hash{$i};
    }
  }
}

&addToFinal(%hash);
&addToFinal(&invert(%final));
&addToFinal(&rotX(&rotX(%final)));
&addToFinal(&rotY(&rotY(%final)));
&addToFinal(&rotX(&rotY(%final)));
&addToFinal(&rotX(&rotX(%final)));
&addToFinal(&rotY(&rotY(%final)));
&addToFinal(&rotX(&rotY(%final)));
&addToFinal(&rotX(&rotX(%final)));
&addToFinal(&rotY(&rotY(%final)));
&addToFinal(&rotX(&rotY(%final)));
&addToFinal(&rotX(&rotX(%final)));
&addToFinal(&rotY(&rotY(%final)));
&addToFinal(&rotX(&rotY(%final)));
#&addToFinal(&mirrorX(%final));

#print keys(%final) . "\n";
#exit(1);

{
  print "\n/*\n";
  print " * Isosurface structure in 2D and 3D for efficient polygon generation.\n";
  print " *\n";
  print " * Generated by IsoFaceGen.pl at " .
    localtime(time) , "\n";
  print " */\n";
  
print <<'EOF'

static int myIsoPrim2DVertex2Edge[2*4] = {
  0,1, 0,2, 1,3, 2,3
};

static int myIsoPrim2DVertex2Axis[4] = {
  0, 1, 1, 0
};

static float myIsoPrim2DVertex2Offset[2*4] = {
  0,0, 0,0, 1,0, 0,1
};

static int myIsoPrim2D[8*16] = {
  // 0000:
  0, -1,-1,-1,-1, -1,-1,-1,
  // 0001:
  1,  0, 1,-1,-1, -1,-1,-1,
  // 0010:
  1,  0, 2,-1,-1, -1,-1,-1,
  // 0011:
  1,  1, 2,-1,-1, -1,-1,-1,
  // 0100:
  1,  1, 3,-1,-1, -1,-1,-1,
  // 0101:
  1,  0, 3,-1,-1, -1,-1,-1,
  // 0110:
  2,  0, 1, 2, 3, -1,-1,-1,
  // 0111:
  1,  2, 3,-1,-1, -1,-1,-1,
  // 1000:
  1,  2, 3,-1,-1, -1,-1,-1,
  // 1001:
  2,  0, 1, 2, 3, -1,-1,-1,
  // 1010:
  1,  0, 3,-1,-1, -1,-1,-1,
  // 1011:
  1,  1, 3,-1,-1, -1,-1,-1,
  // 1100:
  1,  1, 2,-1,-1, -1,-1,-1,
  // 1101:
  1,  0, 2,-1,-1, -1,-1,-1,
  // 1110:
  1,  0, 1,-1,-1, -1,-1,-1,
  // 1111:
  0, -1,-1,-1,-1, -1,-1,-1,
};

static int myIsoPrim3DVertex2Edge[2*12] = {
  0,1, 0,2, 0,4, 1,3, 1,5, 2,3, 2,6, 3,7, 4,5, 4,6, 5,7, 6,7
};

static int myIsoPrim3DVertex2Axis[12] = {
  0, 1, 2, 1, 2, 0, 2, 2, 0, 1, 1, 0
};

static float myIsoPrim3DVertex2Offset[3*12] = {
  0,0,0, 0,0,0, 0,0,0, 1,0,0, 
  1,0,0, 0,1,0, 0,1,0, 1,1,0,
  0,0,1, 0,0,1, 1,0,1, 0,1,1
};

static int myIsoPrim3D[16*256] = {
EOF
}
foreach $i (sort keys %final) {
  $s = $i; $s =~ s/,//g;
  print '  // ' . $s . ":\n";
  @con = split ',', $final{$i};
  $n = ($#con + 1) / 3;
  print "  " . $n . ",  ";
  for ($j = 0; $j < 12; $j ++) {
    if ($j <= $#con) {
      $x = "0x0$con[$j]";
      print "0x$con[$j],";
#sprintf("%.2d", $x) . ",";
    } else {
      print " -1,";
    }
  }
  print " -1,-1,-1,\n";
}
print "};\n";


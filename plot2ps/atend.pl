#!/usr/local/bin/perl
# this perl script moves the bounding box to the beginning of a eps file.
unshift(@ARGV, '-') if $#ARGV < $[;
while ($ARGV = shift)
{ open(ARGV, $ARGV);
  read (ARGV, $data, 2**23);	# 8 meg file size limit
  open(ARGVOUT, ">$ARGV");
  if ($data =~ m/%%BoundingBox: *\(atend\)/)
  { if ($data =~ m/%%BoundingBox: *\d+ +\d+ +\d+ +\d+/)
    { $bbox = $&;
      $data =~ s/$bbox//;
      $data =~ s/%%BoundingBox: *\(atend\)/$bbox/;}}
  print ARGVOUT $data;
  close ARGVOUT;}
# local variables:
# compile-command: "./atend.pl - <spline.ps"
# end:

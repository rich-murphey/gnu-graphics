dir ../tek2plot
dir ../getopt
#br main
#br read_file
#br array_min
#br array_max
#br cont
br write_plot
#br find_tick
run +debug  -bM -a 10 100 -f 12 -g 2 -d a -m 1 -S 2 .02 -T  .03 -L "top label" -X "x label" -Y "y label" -u .1 -r .1  -w .8 -h .8 -x 100 1100 -y 0 1 <../test/spline.y.dat >spline.plt

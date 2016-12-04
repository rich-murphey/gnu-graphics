#!/bin/csh
../graph/graph -S -y l <sqrt >asdf
xplot -xrm "xplot*font:7x13" <asdf
plot2plot -ascii-output <asdf
exit 0


echo 1 2 1 |../graph/graph -a -S +debug >asdf
plot2plot -ascii-output <asdf
xplot -xrm "xplot*font:7x13" <asdf
exit 0

echo 0 1 0 1 >9asdf
cat 9asdf
../graph/graph +debug -a .10 +margin .1 -- 9asdf|xplot
exit 0
../graph/graph -g 2 -T .03 -a 1 1 -y .0001 .008 .001 -x l 1 100 2 -S -L "SPLINE DATA" -X "X AXIS" -Y "Y AXIS" spline.y.dat |xplot
#../graph/graph -a 1 1 -y .0001 .008 .001 -x l 1 100 -S -L "SPLINE DATA" -X "X AXIS" -Y "Y AXIS" spline.y.dat |../plot2plot/plot2plot -ascii-output

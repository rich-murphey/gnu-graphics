# This is the make file for the regression tests
all : test
test : graph.test tek2plot.test plot2tek.test plot2ps.test plot2fig.test plot2plot.test

.PHONY : clean clean-local test expected.output

expected.output : Makefile \
graph.expected.output tek2plot.expected.output \
plot2tek.expected.output plot2ps.expected.output \
plot2fig.expected.output plot2plot.expected.output
	chmod -w $^
#----------------------------------------------------------------
graph.test : spline.plt graph.expected.output
	-../plot2plot/plot2plot +output-as-ascii <spline.plt >graph.test
	@echo
	@echo -n The regression test for graph ""
	-@cmp -s graph.expected.output graph.test && echo PASSED. \
	    || echo FAILED.
	@echo

spline.plt : ../plot2plot/plot2plot ../graph/graph
	-../graph/graph -bM -a 10 100 -f 12 -g 2 -d a -m 2 -S 2 .02 -T \
            .03 -L "top label" -X "x label" -Y "y label" -u .1 -r .1 \
            -w .8 -h .8 -x 100 1100 -y 0 1 <spline.y.dat >spline.plt

graph.expected.output :
	-../graph/graph -bM -a 10 100 -f 12 -g 2 -d a -m 2 -S 2 .02 -T \
            .03 -L "top label" -X "x label" -Y "y label" -u .1 -r .1 \
            -w .8 -h .8 -x 100 1100 -y 0 1 <spline.y.dat \
            | ../plot2plot/plot2plot +output-as-ascii >graph.expected.output

#----------------------------------------------------------------
tek2plot.test: ../tek2plot/tek2plot tek2plot.expected.output spline.tek
	../tek2plot/tek2plot <spline.tek >tek2plot.test
	@echo
	@echo -n The regression test for tek2plot ""
	-@cmp -s tek2plot.expected.output tek2plot.test && echo PASSED. \
	    || echo FAILED.
	@echo

tek2plot.expected.output :
	../tek2plot/tek2plot <spline.tek >tek2plot.expected.output

spline.tek : spline.plt ../plot2tek/plot2tek
# note that if TERM=xterm, plot2tek generates addional xterm specific output
# which is included in later regression tests.  They will fail without it.
	TERM=xterm; export TERM; ../plot2tek/plot2tek <spline.plt >spline.tek

#----------------------------------------------------------------
plot2tek.test : ../plot2tek/plot2tek plot2tek.expected.output spline.plt
	-TERM=xterm; export TERM; ../plot2tek/plot2tek <spline.plt >plot2tek.test
	@echo
	@echo -n The regression test for plot2tek ""
	-@cmp -s plot2tek.expected.output plot2tek.test && echo PASSED. \
	    || echo FAILED.
	@echo

plot2tek.expected.output :
	-TERM=xterm; export TERM; ../plot2tek/plot2tek <spline.plt >plot2tek.expected.output

plot2ps.test : ../plot2ps/plot2ps plot2ps.expected.output spline.plt
	-../plot2ps/plot2ps <spline.plt >plot2ps.test
	@echo
	@echo -n The regression test for plot2ps ""
	-@cmp -s plot2ps.expected.output plot2ps.test && echo PASSED. \
	    || echo FAILED.
	@echo

plot2ps.expected.output :
	-../plot2ps/plot2ps <spline.plt >plot2ps.expected.output

plot2fig.test : ../plot2fig/plot2fig plot2fig.expected.output spline.plt
	-../plot2fig/plot2fig <spline.plt >plot2fig.test
	@echo
	@echo -n The regression test for plot2fig ""
	-@cmp -s plot2fig.expected.output plot2fig.test && echo PASSED. \
	    || echo FAILED.
	@echo

plot2fig.expected.output :
	-../plot2fig/plot2fig <spline.plt >plot2fig.expected.output

plot2plot.test : ../plot2plot/plot2plot plot2plot.expected.output spline.plt
	-../plot2plot/plot2plot +output-as-ascii <spline.plt >plot2plot.test
	@echo
	@echo -n The regression test for plot2plot ""
	-@cmp -s plot2plot.expected.output plot2plot.test && echo PASSED. \
	    || echo FAILED.
	@echo

plot2plot.expected.output :
	-../plot2plot/plot2plot +output-as-ascii  <spline.plt >plot2plot.expected.output
#----------------------------------------------------------------
../graph/graph ../tek2plot/tek2plot ../xplot/xplot ../plot2plot/plot2plot \
    ../plot2tek/plot2tek ../plot2ps/plot2ps :
	cd ..; make all

clean-local :
	-rm *.test *.diff spline.dat spline.tek spline.plt *~

look : graph.test tek2plot.test plot2ps.test plot2fig.test plot2plot.test
	../plot2plot/plot2plot -ascii-input <graph.test |../xplot/xplot
	../xplot/xplot <tek2plot.test

#----------------------------------------------------------------
diff : Makefile \
graph.diff tek2plot.diff \
plot2tek.diff plot2ps.diff \
plot2fig.diff plot2plot.diff

graph.diff : graph.expected.output graph.test
	-diff -cb graph.expected.output graph.test >$@
tek2plot.diff : tek2plot.expected.output tek2plot.test
	-diff -cb tek2plot.expected.output tek2plot.test >$@
plot2tek.diff : plot2tek.expected.output plot2tek.test
	-diff -cb plot2tek.expected.output plot2tek.test >$@
plot2ps.diff : plot2ps.expected.output plot2ps.test
	-diff -cb plot2ps.expected.output plot2ps.test >$@
plot2fig.diff : plot2fig.expected.output plot2fig.test
	-diff -cb plot2fig.expected.output plot2fig.test >$@
plot2plot.diff : plot2plot.expected.output plot2plot.test
	-diff -cb plot2plot.expected.output plot2plot.test >$@
#----------------------------------------------------------------
report : clean
	make test diff >$@
	cat *.diff >>$@

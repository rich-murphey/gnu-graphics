
lib_LTLIBRARIES = libgps.la
libgps_la_SOURCES = getname.c getnum.c message.c prnum.c prhelp.c rdvec.c setprecn.c
libgps_la_LDFLAGS = -version-info 0:0:0
libgps_adir = ../include

bin_PROGRAMS = cor cusum gas hilo log mean mod pair power prod qsort rand rank round siline total var atob btoa abs

cor_SOURCES = cor.c
cusum_SOURCES = cusum.c
gas_SOURCES = gas.c
hilo_SOURCES = hilo.c
log_SOURCES = log.c
mean_SOURCES = mean.c
mod_SOURCES = mod.c
pair_SOURCES = pair.c
power_SOURCES = power.c
prod_SOURCES = prod.c
qsort_SOURCES = qsort.c
rand_SOURCES = rand.c
rank_SOURCES = rank.c
round_SOURCES = round.c
siline_SOURCES = siline.c
total_SOURCES = total.c
var_SOURCES = var.c
atob_SOURCES = atob.c
btoa_SOURCES = btoa.c
abs_SOURCES = abs.c

LDADD = libgps.la -lm
AM_CPPFLAGS = -I$(srcdir)/../include

LN_S=@LN_S@

LINK_CEIL_TO ?= abs

install-exec-hook:
	(cd ${bindir}; for FILE in ceil floor exp cos sin tan acos asin atan gamma ;do $(LN_S) $(LINK_HELLO_TO) $(FILE) ;done)
	(cd ${bindir}; $(LN_S) cor lreg)
	(cd ${bindir}; $(LN_S) root power)

uninstall-hook:
	(cd ${bindir}; for FILE in ceil floor exp cos sin tan acos asin atan gamma ;do $(RM) $(FILE) ;done)
	(cd ${bindir}; $(RM) lreg)
	(cd ${bindir}; $(RM) power)

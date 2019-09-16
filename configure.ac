## aclocal && autoheader && automake --force-missing --add-missing && autoconf && ./configure && make clean && make all

## sudo apt install -y autoconf libtool
## libtoolize --force
## make maintainer-clean
## aclocal
## autoheader
## automake --add-missing
# for i in $(find . -type l); do cp -p $i asdf; mv asdf $i ;done
## find . -type l
## autoconf
## ./configure
## make clean
## make all

AC_INIT([GNU_Graphics],[0.18],[rich@murphey.org])
AM_INIT_AUTOMAKE([-Wall])
AM_PROG_AR
AC_PROG_CC
LT_INIT([disable-shared])
AC_CONFIG_HEADERS([config.h:config.hin])
AC_CONFIG_FILES([
Makefile
graph/Makefile
libgetopt/Makefile
libplot/Makefile
tek2plot/Makefile
xplot/Makefile
dist-stat/Makefile
double/Makefile
plot2fig/Makefile
plot2plot/Makefile
plot2ps/Makefile
plot2tek/Makefile
test/Makefile
])
AC_CHECK_FUNCS([floor])
AC_CHECK_FUNCS([isascii])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([pow])
AC_CHECK_FUNCS([re_comp])
AC_CHECK_FUNCS([sqrt])
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strrchr])
AC_CHECK_HEADERS([float.h])
AC_CHECK_HEADERS([getopt.h])
AM_CONDITIONAL(BUILD_GETOPT, ["$HAVE_GETOPT_H" == 1])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([malloc.h])
AC_CHECK_HEADERS([stddef.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([strings.h])
AC_CHECK_HEADERS([sys/file.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([values.h])
AC_CHECK_HEADER_STDBOOL
AC_HEADER_STDC
AC_FUNC_ALLOCA
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_PATH_X
AC_PREREQ
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_OUTPUT

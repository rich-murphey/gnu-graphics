#!/bin/sh
#!/bin/sh -xv
f=/tmp/atend$$
trap "rm -f $f;exit" 0 1 2 3 15
cat >$f
bb=`awk '/^%%BoundingBox: [0-9]/{print}' <$f`
if [ -n "${bb}" ]; then
	awk "\
\$1~/%%BoundingBox:/{if(\$2~/atend/) print \"${bb}\";}\
\$1!~/%%BoundingBox:/{print;}\
" <$f
else
	cat $f
fi
/bin/rm -rf $f

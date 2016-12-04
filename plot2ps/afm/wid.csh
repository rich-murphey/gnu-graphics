#!/bin/csh -f
foreach i ( Courier-Bold Courier-BoldOblique Courier-Oblique Courier Helvetica-Bold Helvetica-BoldOblique Helvetica-Oblique Helvetica Symbol Times-Bold Times-BoldItalic Times-Italic Times-Roman )
  echo
  echo "int *${i}-width = {"
  awk -f wid.awk </usr/local/lib/ps/${i}.afm
  echo "0};"
  echo
end

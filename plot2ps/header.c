/* The following is a postscript prologue which defines a set of
   macros and constants which are used in rendering graphics on the
   page.

   This postscirpt prologue was written by John Interrante.  For more
   information see the Interviews distribution on inverviews.stanford.edu. */

char *header[6] =
{"\
\n\
%%EndComments\n\
50 dict begin /arrowHeight 8 def /arrowWidth 4 def /none null def\n\
/numGraphicParameters 17 def /stringLimit 65535 def /Begin{save\n\
numGraphicParameters dict begin}def /End{end restore}def /SetB{dup type\n\
/nulltype eq{pop false /brushRightArrow idef false /brushLeftArrow idef true\n\
/brushNone idef}{/brushDashOffset idef /brushDashArray idef 0 ne\n\
/brushRightArrow idef 0 ne /brushLeftArrow idef /brushWidth idef false\n\
/brushNone idef}ifelse}def /SetCFg{/fgblue idef /fggreen idef /fgred idef}def\n\
/SetCBg{/bgblue idef /bggreen idef /bgred idef}def /SetF{/printSize idef\n\
/printFont idef}def /SetP{dup type /nulltype eq{pop true /patternNone idef}{\n\
/patternGrayLevel idef patternGrayLevel -1 eq{/patternString idef}if false\n\
/patternNone idef}ifelse}def /BSpl{0 begin storexyn newpath n 1 gt{0 0 0 0 0 0\n\
1 1 true subspline n 2 gt{0 0 0 0 1 1 2 2 false subspline 1 1 n 3 sub{/i exch\n\
def i 1 sub dup i dup i 1 add dup i 2 add dup false subspline}for n 3 sub dup\n\
n 2 sub dup n 1 sub dup 2 copy false subspline}if n 2 sub dup n 1 sub dup 2\n\
copy 2 copy false subspline patternNone not brushLeftArrow not brushRightArrow\n\
not and and{ifill}if brushNone not{istroke}if 0 0 1 1 leftarrow n 2 sub dup n\n\
1 sub dup rightarrow}if end}dup 0 4 dict put def /Circ{newpath 0 360 arc\n\
patternNone not{ifill}if brushNone not{istroke}if}def /CBSpl{0 begin dup 2 gt{\n\
storexyn newpath n 1 sub dup 0 0 1 1 2 2 true subspline 1 1 n 3 sub{/i exch\n\
def i 1 sub dup i dup i 1 add dup i 2 add dup false subspline}for n 3 sub dup\n\
n 2 sub dup n 1 sub dup 0 0 false subspline n 2 sub dup n 1 sub dup 0 0 1 1\n\
false subspline patternNone not{ifill}if brushNone not{istroke}if}{Poly}\n\
ifelse end}dup 0 4 dict put def /Elli{0 begin newpath 4 2 roll translate scale\n\
0 0 1 0 360 arc patternNone not{ifill}if brushNone not{istroke}if end}dup 0 1\n\
dict put def /Line{0 begin 2 storexyn newpath x 0 get y 0 get moveto x 1 get y\n\
1 get lineto brushNone not{istroke}if 0 0 1 1 leftarrow 0 0 1 1 rightarrow end\n\
}dup 0 4 dict put def /MLine{0 begin storexyn newpath n 1 gt{x 0 get y 0 get\n\
moveto 1 1 n 1 sub{/i exch def x i get y i get lineto}for patternNone not\n\
brushLeftArrow not brushRightArrow not and and{ifill}if brushNone not{istroke}\n\
if 0 0 1 1 leftarrow n 2 sub dup n 1 sub dup rightarrow}if end}dup 0 4 dict\n\
put def /Poly{3 1 roll newpath moveto -1 add{lineto}repeat closepath\n\
patternNone not{ifill}if brushNone not{istroke}if}def /Rect{0 begin /t exch\n\
def /r exch def /b exch def /l exch def newpath l b moveto l t lineto r t\n\
lineto r b lineto closepath patternNone not{ifill}if brushNone not{istroke}if\n\
end}dup 0 4 dict put def /Text{ishow}def /idef{dup where{pop pop pop}{exch def\n\
}ifelse}def /ifill{0 begin gsave patternGrayLevel -1 ne{fgred bgred fgred sub\n\
patternGrayLevel mul add fggreen bggreen fggreen sub patternGrayLevel mul add\n\
fgblue bgblue fgblue sub patternGrayLevel mul add setrgbcolor eofill}{eoclip\n\
originalCTM setmatrix pathbbox /t exch def /r exch def /b exch def /l exch def\n\
/w r l sub ceiling cvi def /h t b sub ceiling cvi def /imageByteWidth w 8 div\n\
ceiling cvi def /imageHeight h def bgred bggreen bgblue setrgbcolor eofill\n\
fgred fggreen fgblue setrgbcolor w 0 gt h 0 gt and{l b translate w h scale w h\n\
true[w 0 0 h neg 0 h]{patternproc}imagemask}if}ifelse grestore end}dup 0 8\n\
dict put def /istroke{gsave brushDashOffset -1 eq{[]0 setdash 1 setgray}{\n\
brushDashArray brushDashOffset setdash fgred fggreen fgblue setrgbcolor}\n\
ifelse brushWidth setlinewidth originalCTM setmatrix stroke grestore}def\n\
/ishow{0 begin gsave fgred fggreen fgblue setrgbcolor /fontDict printFont\n\
findfont printSize scalefont dup setfont def /descender fontDict begin 0[\n\
FontBBox]1 get FontMatrix end transform exch pop def /vertoffset 0 descender\n\
sub printSize sub printFont /Courier ne printFont /Courier-Bold ne and{1 add}\n\
if def{0 vertoffset moveto show /vertoffset vertoffset printSize sub def}\n\
forall grestore end}dup 0 3 dict put def /patternproc{0 begin\n\
/patternByteLength patternString length def /patternHeight patternByteLength 8\n\
mul sqrt cvi def /patternWidth patternHeight def /patternByteWidth\n\
patternWidth 8 idiv def /imageByteMaxLength imageByteWidth imageHeight mul\n\
stringLimit patternByteWidth sub min def /imageMaxHeight imageByteMaxLength\n\
imageByteWidth idiv patternHeight idiv patternHeight mul patternHeight max def\n\
/imageHeight imageHeight imageMaxHeight sub store /imageString imageByteWidth\n\
imageMaxHeight mul patternByteWidth add string def 0 1 imageMaxHeight 1 sub{\n\
/y exch def /patternRow y patternByteWidth mul patternByteLength mod def\n\
/patternRowString patternString patternRow patternByteWidth getinterval def\n\
/imageRow y imageByteWidth mul def 0 patternByteWidth imageByteWidth 1 sub{/x\n\
exch def imageString imageRow x add patternRowString putinterval}for}for\n\
imageString end}dup 0 12 dict put def /min{dup 3 2 roll dup 4 3 roll lt{exch}\n\
if pop}def /max{dup 3 2 roll dup 4 3 roll gt{exch}if pop}def /arrowhead{0\n\
begin transform originalCTM itransform /taily exch def /tailx exch def\n\
transform originalCTM itransform /tipy exch def /tipx exch def /dy tipy taily\n\
sub def /dx tipx tailx sub def /angle dx 0 ne dy 0 ne or{dy dx atan}{90}\n\
ifelse def gsave originalCTM setmatrix tipx tipy translate angle rotate\n\
newpath 0 0 moveto arrowHeight neg arrowWidth 2 div lineto arrowHeight neg\n\
arrowWidth 2 div neg lineto closepath patternNone not{originalCTM setmatrix\n\
/padtip arrowHeight 2 exp 0.25 arrowWidth 2 exp mul add sqrt brushWidth mul\n\
arrowWidth div def /padtail brushWidth 2 div def tipx tipy translate angle\n\
rotate padtip 0 translate arrowHeight padtip add padtail add arrowHeight div\n\
dup scale arrowheadpath ifill}if brushNone not{originalCTM setmatrix tipx tipy\n\
translate angle rotate arrowheadpath istroke}if grestore end}dup 0 9 dict put\n\
def /arrowheadpath{newpath 0 0 moveto arrowHeight neg arrowWidth 2 div lineto\n\
arrowHeight neg arrowWidth 2 div neg lineto closepath}def /leftarrow{0 begin y\n\
exch get /taily exch def x exch get /tailx exch def y exch get /tipy exch def\n\
x exch get /tipx exch def brushLeftArrow{tipx tipy tailx taily arrowhead}if\n\
end}dup 0 4 dict put def /rightarrow{0 begin y exch get /tipy exch def x exch\n\
get /tipx exch def y exch get /taily exch def x exch get /tailx exch def\n\
brushRightArrow{tipx tipy tailx taily arrowhead}if end}dup 0 4 dict put def\n\
/midpoint{0 begin /y1 exch def /x1 exch def /y0 exch def /x0 exch def x0 x1\n\
add 2 div y0 y1 add 2 div end}dup 0 4 dict put def /thirdpoint{0 begin /y1\n\
exch def /x1 exch def /y0 exch def /x0 exch def x0 2 mul x1 add 3 div y0 2 mul\n\
y1 add 3 div end}dup 0 4 dict put def /subspline{0 begin /movetoNeeded exch\n\
def y exch get /y3 exch def x exch get /x3 exch def y exch get /y2 exch def x\n\
exch get /x2 exch def y exch get /y1 exch def x exch get /x1 exch def y exch\n\
get /y0 exch def x exch get /x0 exch def x1 y1 x2 y2 thirdpoint /p1y exch def\n\
/p1x exch def x2 y2 x1 y1 thirdpoint /p2y exch def /p2x exch def x1 y1 x0 y0\n\
thirdpoint p1x p1y midpoint /p0y exch def /p0x exch def x2 y2 x3 y3 thirdpoint\n\
p2x p2y midpoint /p3y exch def /p3x exch def movetoNeeded{p0x p0y moveto}if\n\
p1x p1y p2x p2y p3x p3y curveto end}dup 0 17 dict put def /storexyn{/n exch\n\
def /y n array def /x n array def n 1 sub -1 0{/i exch def y i 3 2 roll put x\n\
i 3 2 roll put}for}def\n\
%%EndProlog\n\
",""};

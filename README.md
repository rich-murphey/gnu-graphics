## Gnu Graphics

GNU graphics is a set of programs for plotting scientific
data.  The program `graph` reads data files and writes a stream of
plotting commands in a device independent format refered to below as a
GNU plot file.  The various `plot` programs provide display
plot files on tektronix 4010, postscript, and X window system
compatible output devices.

`graph` reads both ascii and binary data files and writes a plot
file with or without axes and labels.  You can specify labels and ranges
for the axes, and you can set the sizes and position of the plot on the
page.  Each invocation of graph produces a plot with single set of axes
and data.  You can place an arbitrary number of plots on the page by
concatenating the plot output of several invocations.

`plot2ps` is a utility for converting plot files into PostScript.
The `plot2ps` utility reads plotting commands from named files or
the standard input and writes PostScript to the standard output.  You
can then print the PostScript output on a printer, or edit it using the
`idraw` graphics editor.  You can also easily include
the output in LaTeX documents using `dvips` and the LaTeX command
`psfig`.

## Authors

Rich Murphey wrote this in 1990-1991 produce the graphics for his
dissertation. He assigned the copyright to the Free Software
Foundation.  It was later extended and renamed 'Gnu Plotutils'.

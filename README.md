Opag - a command line option parser generator
=============================================


What is it?
-----------

Opag, which stands for `Option Parser Generator', is a program to
generate C or C++ code which parses command line options. It takes as
input a description of the command line options you want your program
to support. The output is a C or C++ function which finds these options
in an array of strings (typically `argv', the second parameter of the
`main' function) and sets variables accordingly. Short (single
character) as well as long (GNU style) options are supported.

It is also possbile to associate a help string with each option. Opag
uses these to create a macro which expands to a string containing a
nicely formatted description of the options.

Opag is Free Software, released under the GNU General Public License.


Installation
------------

The file `INSTALL' contains general installation instructions. If you
have not yet read it, please do so now.

Opag uses itself to build the files `options.c' and `options.h' from
`options.opag' (in the `src' directory). Therefore, if you want to
modify `options.opag', you must already have an opag executable. The
`configure' script tries to find such an executable, but you can also
specify its location with the option `--with-opag'. Please type
`./configure --help' for more information.


Home page
---------

The opag home page used to be located at
<http://www.zero-based.org/software/opag/>, but this site is no longer
exists.

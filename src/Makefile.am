## Process this file with automake to produce Makefile.in.
## This file is part of opag, an option parser generator.
## Copyright (C) 2003, 2004 Martin Dickopp
##
## This file is free software; the copyright holder gives unlimited
## permission to copy and/or distribute it, with or without
## modifications, as long as this notice is preserved.
##
## This file is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY, to the extent permitted by law; without
## even the implied warranty of MERCHANTABILITY or FITNESS FOR A
## PARTICULAR PURPOSE.

bin_PROGRAMS = opag

opag_SOURCES = chartype.h fileread.c fileread.h opag.c opag.h options.c options.h output.c parse.c stdinc.h \
               util.c util.h
opag_LDADD = @LIBOBJS@

BUILT_SOURCES = options.c options.h
EXTRA_DIST = options.opag

options.h: options.opag
options.c: options.h options.opag
	@if test "@OPAG@" = missing; then \
	  echo "You have modified \`options.opag', but you do not seem to have the \`opag'"; \
	  echo "program installed on your system. Do you need to specify the \`--with-opag'"; \
	  echo "option to the \`configure' script?"; \
	  exit 1; \
	fi
	@OPAG@ --gcc $(srcdir)/options.opag options.c options.h

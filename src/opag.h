/* This file is part of opag, an option parser generator.
   Copyright (C) 2003, 2005 Martin Dickopp

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
   USA.  */

#ifndef HDR_OPAG
#define HDR_OPAG 1


/* Flags associated with an option definition (bitmask).  */
#define FLAG_OPTARG 0x01                           /* Optional argument.  */
#define FLAG_REQARG 0x02                           /* Required argument.  */
#define FLAG_RETURN 0x04                           /* Return immediately.  */
#define FLAGS_ARG   (FLAG_OPTARG | FLAG_REQARG)    /* Optional or required argument.  */


/* Definition of a preprocessor directive.  */
struct pp_directive {
    const char *str;        /* Directive string.  */
    size_t len;             /* Length of directive string.  */
    unsigned int lineno;    /* Line number.  */
};

/* Definition of a single option.  */
struct opt {
    char short_option;      /* Short option character, or '\0' if not present.  */
    char *long_option;      /* Long option name.  */
    char *varname;          /* Variable name of the option.  */
    char *macroname;        /* Macro name of the option.  */
    char *help_string;      /* String to be used as help text, or a null pointer if not present.  */
    char *arg_string;       /* String to be used as argument name, or a null pointer if not present.  */
    size_t opt_str_len;     /* Length of long option name plus argument string, or 0 if no help text is present.  */
    struct pp_directive *pp_directives;    /* List of preprocessor directives.  */
    size_t pp_directives_len;              /* Length of list of preprocessor directives.  */
    unsigned int flags;
};


/* Representation of the input file after parsing.  */
struct parsed_infile {
    const char *header;        /* Text above option definitions.  */
    size_t header_len;         /* Length of text above option definitions.  */
    const char *footer;        /* Text below option definitions.  */
    size_t footer_len;         /* Length of text below option declarations.  */
    struct opt *opt;           /* Option definitions.  */
    size_t num_opt;            /* Number of option definitions.  */
    size_t max_opt_str_len;    /* Maximal length of long option name plus argument string.  */
};


/* Parse input file buffer.  */
extern void parse_file (const char *infilename, char *buffer, char *buffer_end,
                        struct parsed_infile *pf) gcc_attr_nonnull (());

/* Create header and code file.  */
extern void output_code (const char *cfilename, const char *hfilename,
                         const struct parsed_infile *pf) gcc_attr_nonnull (());


#endif

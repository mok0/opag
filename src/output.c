/* This file is part of opag, an option parser generator.
   Copyright (C) 2003, 2004, 2005 Martin Dickopp

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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "stdinc.h"

#include <stdio.h>

#if HAVE_ERRNO_H
# include <errno.h>
#endif
#ifndef errno
extern int errno;
#endif

#include "util.h"
#include "chartype.h"
#include "opag.h"
#include "options.h"



static int generate_header (FILE *f, const char *filename_macro, const struct parsed_infile *pf) gcc_attr_nonnull (());
static int generate_code (FILE *f, const struct parsed_infile *pf) gcc_attr_nonnull (());
static void write_pp_directives (FILE *f, const struct opt *opt, int flag_closing) gcc_attr_nonnull (());
static void write_help_text (FILE *f, const struct opt *opt,
                             size_t left_column, size_t max_column) gcc_attr_nonnull (());
static void write_c_string (FILE *f, const char *s, size_t max_chars) gcc_attr_nonnull (());
static size_t common_prefix_len (const char *a, const char *b) gcc_attr_pure gcc_attr_nonnull (());
static int compare_long_opt (const void *a, const void *b) gcc_attr_pure gcc_attr_nonnull (());
static int compare_short_opt (const void *a, const void *b) gcc_attr_pure gcc_attr_nonnull (());



/* Create header and code file.  */
void
output_code (const char *const cfilename, const char *hfilename, const struct parsed_infile *const pf)
{
    if (hfilename != 0)
    {
        char *filename_macro;

        /* Determine filename macro.  */
        {
            size_t i, len;
            const char *read_ptr;
            char *write_ptr;

            if ((read_ptr = strrchr (hfilename, '/')) == 0 || *++read_ptr == '/')
                read_ptr = hfilename;

            len = strlen (read_ptr);
            if (len > 2 && hfilename [len - 1] == 'h' && hfilename [len - 2] == '.')
                len -= 2;

            filename_macro = write_ptr = xmalloc (len + 1);

            for (i = 0; i < len; ++i)
            {
                if (ISALNUM ((unsigned char)*read_ptr))
                    *write_ptr++ = TOUPPER ((unsigned char)*read_ptr);
                else
                    *write_ptr++ = '_';
                ++read_ptr;
            }

            *write_ptr = '\0';
        }

        /* Generate header file.  */
        {
            FILE *const f = fopen (hfilename, "w");

            if (f == 0 || generate_header (f, filename_macro, pf) == -1 || fclose (f) == EOF)
            {
                fprintf (stderr, "%s: %s: %s\n", invocation_name, hfilename, strerror (errno));
                exit (2);
            }
        }

        free (filename_macro);
    }


    /* Generate code file.  */
    {
        FILE *const f = cfilename != 0 ? fopen (cfilename, "w") : stdout;

        if (f == 0 || generate_code (f, pf) == -1 || (cfilename != 0 && fclose (f) == EOF))
        {
            fprintf (stderr, "%s: %s: %s\n", invocation_name, cfilename != 0 ? cfilename : "STDOUT", strerror (errno));
            exit (2);
        }
    }
}



/* Generate header file.  Return zero on success, or -1 if a file error occurs.  */
static int
generate_header (FILE *const f, const char *const filename_macro, const struct parsed_infile *const pf)
{
    /* Identify this program.  */
    fprintf (f, "\
/* This file has been generated with " PACKAGE_STRING ".  */\n\n\
#ifndef HDR_%s\n\
#define HDR_%s 1\n", filename_macro, filename_macro);


    /* Namespace.  */
    if (opt_namespace)
        fprintf (f, "\nnamespace %s {\n", arg_namespace);


    /* Structure declaration.  */
    if (opt_struct)
        fprintf (f, "\nstruct %s {\n", arg_struct);


    /* Declarations of option variables.  */
    {
        size_t i;
        for (i = 0; i < pf->num_opt; ++i)
        {
            putc ('\n', f);

            write_pp_directives (f, &pf->opt [i], 0);

            if (pf->opt [i].short_option != '\0')
                fprintf (f, "/* Set to 1 if option --%s (-%c) has been specified.  */\n",
                         pf->opt [i].long_option, pf->opt [i].short_option);
            else
                fprintf (f, "/* Set to 1 if option --%s has been specified.  */\n",
                         pf->opt [i].long_option);

            if (opt_struct)
                fprintf (f, "unsigned int %s%s : 1;\n", arg_option_prefix, pf->opt [i].varname);
            else
                fprintf (f, "extern char %s%s;\n", arg_option_prefix, pf->opt [i].varname);

            write_pp_directives (f, &pf->opt [i], 1);
        }
    }


    /* Declarations of argument variables.  */
    {
        size_t i;
        for (i = 0; i < pf->num_opt; ++i)
            if (pf->opt [i].flags & FLAGS_ARG)
            {
                putc ('\n', f);

                write_pp_directives (f, &pf->opt [i], 0);

                if (pf->opt [i].short_option != '\0')
                    fprintf (f, "/* Argument to option --%s (-%c)",
                             pf->opt [i].long_option, pf->opt [i].short_option);
                else
                    fprintf (f, "/* Argument to option --%s", pf->opt [i].long_option);

                if (pf->opt [i].flags & FLAG_OPTARG)
                    fputs (", or a null pointer if no argument", f);

                if (opt_struct)
                    fprintf (f, ".  */\nconst char *%s%s;\n", arg_argument_prefix, pf->opt [i].varname);
                else
                    fprintf (f, ".  */\nextern const char *%s%s;\n", arg_argument_prefix, pf->opt [i].varname);

                write_pp_directives (f, &pf->opt [i], 1);
            }
    }


    /* End of structure declaration.  */
    if (opt_struct)
        fputs ("\n};\n", f);


    /* Function prototype.  */
    if (opt_cxx)
        fprintf (f, "\n\
/* Parse command line options.  Return index of first non-option argument.\n\
   Throw a %s exception if an error is encountered.  */\n",
                 opt_exception_name ? arg_exception_name : "::std::runtime_error");
    else
        fputs ("\n\
/* Parse command line options.  Return index of first non-option argument,\n\
   or -1 if an error is encountered.  */\n", f);

    if (opt_struct)
    {
        if (opt_cxx)
        {
            if (opt_gcc)
                fprintf (f, "\
#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))\n\
extern int %s (%s *options, int argc, char **argv) __attribute__ ((nonnull));\n\
#else\n\
extern int %s (%s *options, int argc, char **argv);\n\
#endif",
                         opt_function_name ? arg_function_name : "parse_options", arg_struct,
                         opt_function_name ? arg_function_name : "parse_options", arg_struct);
            else
                fprintf (f, "\
extern int %s (%s *options, int argc, char **argv);",
                         opt_function_name ? arg_function_name : "parse_options", arg_struct);
        }
        else
        {
            if (opt_gcc)
                fprintf (f, "\
#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))\n\
extern int %s (struct %s *options, const char *program_name, int argc, char **argv) __attribute__ ((nonnull));\n\
#else\n\
extern int %s (struct %s *options, const char *program_name, int argc, char **argv);\n\
#endif",
                         opt_function_name ? arg_function_name : "parse_options", arg_struct,
                         opt_function_name ? arg_function_name : "parse_options", arg_struct);
            else
                fprintf (f, "\
extern int %s (struct %s *options, const char *program_name, int argc, char **argv);",
                         opt_function_name ? arg_function_name : "parse_options", arg_struct);
        }
    }
    else
    {
        if (opt_cxx)
        {
            if (opt_gcc)
                fprintf (f, "\
#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))\n\
extern int %s (int argc, char **argv) __attribute__ ((nonnull));\n\
#else\n\
extern int %s (int argc, char **argv);\n\
#endif",
                         opt_function_name ? arg_function_name : "parse_options",
                         opt_function_name ? arg_function_name : "parse_options");
            else
                fprintf (f, "\
extern int %s (int argc, char **argv);",
                         opt_function_name ? arg_function_name : "parse_options");
        }
        else
        {
            if (opt_gcc)
                fprintf (f, "\
#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))\n\
extern int %s (const char *program_name, int argc, char **argv) __attribute__ ((nonnull));\n\
#else\n\
extern int %s (const char *program_name, int argc, char **argv);\n\
#endif",
                         opt_function_name ? arg_function_name : "parse_options",
                         opt_function_name ? arg_function_name : "parse_options");
            else
                fprintf (f, "\
extern int %s (const char *program_name, int argc, char **argv);",
                         opt_function_name ? arg_function_name : "parse_options");
        }
    }


    /* End of namespace.  */
    if (opt_namespace)
        fputs ("\n\n}", f);


    fputs ("\n\n#endif\n",f );

    return ferror (f) ? -1 : 0;
}



/* Generate code file.  Return zero on success, or -1 if a file error occurs.  */
static int
generate_code (FILE *const f, const struct parsed_infile *const pf)
{
    int flag_error_long_opt_ambiguous = 0;
    int flag_error_missing_arg_long = 0;
    int flag_error_unexpec_arg_long = 0;
    int flag_error_missing_arg_short = 0;



    /* Identify this program.  */
    fputs ("/* This file has been generated with " PACKAGE_STRING ".  */\n", f);


    /* Header.  */
    if (pf->header_len > 0 && fwrite (pf->header, 1, pf->header_len, f) != pf->header_len)
        return -1;


    /* GNU libc include.  */
    if (opt_glibc)
        fputs ("\
#if defined(__GLIBC__) && __GLIBC__ >= 2\n\
# ifndef _GNU_SOURCE\n\
#  define _GNU_SOURCE 1\n\
# endif\n\
# include <getopt.h>\n\
#endif\n\n", f);


    /* Error text macros.  */
    if (opt_cxx)
        fputs ("\
#ifndef STR_ERR_UNKNOWN_LONG_OPT\n\
# define STR_ERR_UNKNOWN_LONG_OPT \\\n\
  ::std::string (\"unrecognized option `--\").append (option).append (1, '\\'')\n\
#endif\n\n\
#ifndef STR_ERR_LONG_OPT_AMBIGUOUS\n\
# define STR_ERR_LONG_OPT_AMBIGUOUS \\\n\
  ::std::string (\"option `--\").append (option).append (\"' is ambiguous\")\n\
#endif\n\n\
#ifndef STR_ERR_MISSING_ARG_LONG\n\
# define STR_ERR_MISSING_ARG_LONG \\\n\
  ::std::string (\"option `--\").append (option).append (\"' requires an argument\")\n\
#endif\n\n\
#ifndef STR_ERR_UNEXPEC_ARG_LONG\n\
# define STR_ERR_UNEXPEC_ARG_LONG \\\n\
  ::std::string (\"option `--\").append (option).append (\"' doesn't allow an argument\")\n\
#endif\n\n\
#ifndef STR_ERR_UNKNOWN_SHORT_OPT\n\
# define STR_ERR_UNKNOWN_SHORT_OPT \\\n\
  ::std::string (\"unrecognized option `-\").append (1, *option).append (1, '\\'')\n\
#endif\n\n\
#ifndef STR_ERR_MISSING_ARG_SHORT\n\
# define STR_ERR_MISSING_ARG_SHORT \\\n\
  ::std::string (\"option `-\").append (1, *option).append (\"' requires an argument\")\n\
#endif\n", f);
    else if (opt_nls_error_text)
        fprintf (f, "\
#ifndef STR_ERR_UNKNOWN_LONG_OPT\n\
# define STR_ERR_UNKNOWN_LONG_OPT   %s(\"%%s: unrecognized option `--%%s'\\n\")\n\
#endif\n\n\
#ifndef STR_ERR_LONG_OPT_AMBIGUOUS\n\
# define STR_ERR_LONG_OPT_AMBIGUOUS %s(\"%%s: option `--%%s' is ambiguous\\n\")\n\
#endif\n\n\
#ifndef STR_ERR_MISSING_ARG_LONG\n\
# define STR_ERR_MISSING_ARG_LONG   %s(\"%%s: option `--%%s' requires an argument\\n\")\n\
#endif\n\n\
#ifndef STR_ERR_UNEXPEC_ARG_LONG\n\
# define STR_ERR_UNEXPEC_ARG_LONG   %s(\"%%s: option `--%%s' doesn't allow an argument\\n\")\n\
#endif\n\n\
#ifndef STR_ERR_UNKNOWN_SHORT_OPT\n\
# define STR_ERR_UNKNOWN_SHORT_OPT  %s(\"%%s: unrecognized option `-%%c'\\n\")\n\
#endif\n\n\
#ifndef STR_ERR_MISSING_ARG_SHORT\n\
# define STR_ERR_MISSING_ARG_SHORT  %s(\"%%s: option `-%%c' requires an argument\\n\")\n\
#endif\n",
                 arg_nls_error_text, arg_nls_error_text, arg_nls_error_text,
                 arg_nls_error_text, arg_nls_error_text, arg_nls_error_text);
    else
        fputs ("\
#ifndef STR_ERR_UNKNOWN_LONG_OPT\n\
# define STR_ERR_UNKNOWN_LONG_OPT   \"%s: unrecognized option `--%s'\\n\"\n\
#endif\n\n\
#ifndef STR_ERR_LONG_OPT_AMBIGUOUS\n\
# define STR_ERR_LONG_OPT_AMBIGUOUS \"%s: option `--%s' is ambiguous\\n\"\n\
#endif\n\n\
#ifndef STR_ERR_MISSING_ARG_LONG\n\
# define STR_ERR_MISSING_ARG_LONG   \"%s: option `--%s' requires an argument\\n\"\n\
#endif\n\n\
#ifndef STR_ERR_UNEXPEC_ARG_LONG\n\
# define STR_ERR_UNEXPEC_ARG_LONG   \"%s: option `--%s' doesn't allow an argument\\n\"\n\
#endif\n\n\
#ifndef STR_ERR_UNKNOWN_SHORT_OPT\n\
# define STR_ERR_UNKNOWN_SHORT_OPT  \"%s: unrecognized option `-%c'\\n\"\n\
#endif\n\n\
#ifndef STR_ERR_MISSING_ARG_SHORT\n\
# define STR_ERR_MISSING_ARG_SHORT  \"%s: option `-%c' requires an argument\\n\"\n\
#endif\n", f);


    /* Help text macros.  */
    if (pf->max_opt_str_len > 0)
    {
        const size_t left_column = pf->max_opt_str_len + 12 < 40 ? pf->max_opt_str_len + 12 : 40;
        size_t last_opt_with_help_string = 0;

        {
            size_t i;
            for (i = 0; i < pf->num_opt; ++i)
                if (pf->opt [i].help_string != 0)
                {
                    if (opt_nls_help_strings)
                        fprintf (f, "\n#define STR_HELP_%s %s(\"\\\n", pf->opt [i].macroname, arg_nls_help_strings);
                    else
                        fprintf (f, "\n#define STR_HELP_%s \"\\\n", pf->opt [i].macroname);
                    write_help_text (f, &pf->opt [i], left_column, 79);
                    fputs (opt_nls_help_strings ? "\")\n" : "\"\n", f);
                    last_opt_with_help_string = i;
                }
        }

        if (opt_nls_help_text)
            fprintf (f, "\n#define STR_HELP %s(\"\\\n", arg_nls_help_text);
        else
            fputs ("\n#define STR_HELP \"\\\n", f);

        {
            size_t i;
            for (i = 0; i < pf->num_opt; ++i)
                if (pf->opt [i].help_string != 0)
                {
                    write_help_text (f, &pf->opt [i], left_column, 79);
                    fputs (i == last_opt_with_help_string ? opt_nls_help_text ? "\")\n" : "\"\n" : "\\\n", f);
                }
        }
    }


    /* Namespace.  */
    if (opt_namespace)
        fprintf (f, "\nnamespace %s {\n", arg_namespace);


    /* Structure declaration.  */
    if (opt_struct)
        fprintf (f, "\nstruct %s {\n", arg_struct);


    /* Definitions of option variables.  */
    {
        size_t i;
        for (i = 0; i < pf->num_opt; ++i)
        {
            putc ('\n', f);

            write_pp_directives (f, &pf->opt [i], 0);

            if (pf->opt [i].short_option != '\0')
                fprintf (f, "/* Set to 1 if option --%s (-%c) has been specified.  */\n",
                         pf->opt [i].long_option, pf->opt [i].short_option);
            else
                fprintf (f, "/* Set to 1 if option --%s has been specified.  */\n",
                         pf->opt [i].long_option);

            if (opt_struct)
                fprintf (f, "unsigned int %s%s : 1;\n", arg_option_prefix, pf->opt [i].varname);
            else
                fprintf (f, "char %s%s;\n", arg_option_prefix, pf->opt [i].varname);

            write_pp_directives (f, &pf->opt [i], 1);
        }
    }


    /* Definitions of argument variables.  */
    {
        size_t i;
        for (i = 0; i < pf->num_opt; ++i)
            if (pf->opt [i].flags & FLAGS_ARG)
            {
                putc ('\n', f);

                write_pp_directives (f, &pf->opt [i], 0);

                if (pf->opt [i].short_option != '\0')
                    fprintf (f, "/* Argument to option --%s (-%c)",
                             pf->opt [i].long_option, pf->opt [i].short_option);
                else
                    fprintf (f, "/* Argument to option --%s", pf->opt [i].long_option);

                if (pf->opt [i].flags & FLAG_OPTARG)
                    fputs (", or a null pointer if no argument", f);

                if (opt_struct)
                    fprintf (f, ".  */\nconst char *%s%s;\n", arg_argument_prefix, pf->opt [i].varname);
                else
                    fprintf (f, ".  */\nconst char *%s%s;\n", arg_argument_prefix, pf->opt [i].varname);

                write_pp_directives (f, &pf->opt [i], 1);
            }
    }


    /* End of structure declaration.  */
    if (opt_struct)
        fputs ("\n};\n", f);


    /* Function definition.  */
    if (opt_cxx)
        fprintf (f, "\n\
/* Parse command line options.  Return index of first non-option argument.\n\
   Throw a %s exception if an error is encountered.  */\n",
                 opt_exception_name ? arg_exception_name : "::std::runtime_error");
    else
        fputs ("\n\
/* Parse command line options.  Return index of first non-option argument,\n\
   or -1 if an error is encountered.  */\n", f);

    if (opt_struct)
    {
        if (opt_cxx)
            fprintf (f, "\
int %s (%s *const options, const int argc, char **const argv)\n{\n",
                     opt_function_name ? arg_function_name : "parse_options", arg_struct);
        else
            fprintf (f, "\
int %s (struct %s *const options, const char *const program_name, const int argc, char **const argv)\n{\n",
                     opt_function_name ? arg_function_name : "parse_options", arg_struct);
    }
    else
    {
        if (opt_cxx)
            fprintf (f, "\
int %s (const int argc, char **const argv)\n{\n",
                     opt_function_name ? arg_function_name : "parse_options");
        else
            fprintf (f, "\
int %s (const char *const program_name, const int argc, char **const argv)\n{\n",
                     opt_function_name ? arg_function_name : "parse_options");
    }


    /* Assign option names to variables, unless the option takes an optional argument.  */
    if (opt_glibc)
        fputs ("#if !defined(__GLIBC__) || __GLIBC__ < 2\n", f);

    {
        size_t i;
        for (i = 0; i < pf->num_opt; ++i)
            if (!(pf->opt [i].flags & FLAG_OPTARG))
            {
                write_pp_directives (f, &pf->opt [i], 0);
                fprintf (f, "  static const char *const optstr__%s = \"%s\";\n",
                         pf->opt [i].varname, pf->opt [i].long_option);
                write_pp_directives (f, &pf->opt [i], 1);
            }
    }

    fputs ("  int i = 0;\n", f);

    if (opt_glibc)
        fputs ("#endif\n", f);


    /* Initialize option and argument variables.  */
    {
        size_t i;
        for (i = 0; i < pf->num_opt; ++i)
        {
            write_pp_directives (f, &pf->opt [i], 0);
            fprintf (f, "  %s%s%s = 0;\n", opt_struct ? "options->" : "" , arg_option_prefix, pf->opt [i].varname);
            write_pp_directives (f, &pf->opt [i], 1);
        }
    }

    {
        size_t i;
        for (i = 0; i < pf->num_opt; ++i)
            if (pf->opt [i].flags & FLAGS_ARG)
            {
                write_pp_directives (f, &pf->opt [i], 0);
                fprintf (f, "  %s%s%s = 0;\n", opt_struct ? "options->" : "" , arg_argument_prefix,
                         pf->opt [i].varname);
                write_pp_directives (f, &pf->opt [i], 1);
            }
    }


    /* Code to parse options with GNU libc.  */
    if (opt_glibc)
    {
        fputs ("\
#if defined(__GLIBC__) && __GLIBC__ >= 2\n\
  argv [0] = (char *)program_name;\n\
  optind = 1;\n\
  while (1)\n\
  {\n\
    static const struct option options [] = {\n", f);


        /* Sort options by short option character.  */
        qsort (pf->opt, pf->num_opt, sizeof *pf->opt, compare_short_opt);


        /* GNU libc option definitions.  */
        {
            int optnum = 256;
            size_t i;

            for (i = 0; i < pf->num_opt; ++i)
            {
                write_pp_directives (f, &pf->opt [i], 0);

                fprintf (f, "      {\"%s\", %s_argument, 0, ", pf->opt [i].long_option,
                         (pf->opt [i].flags & FLAG_REQARG ? "required"
                          : pf->opt [i].flags & FLAG_OPTARG ? "optional" : "no"));

                if (pf->opt [i].short_option != '\0')
                    fprintf (f, "'%c'},\n", pf->opt [i].short_option);
                else
                    fprintf (f, "%d},\n", optnum++);

                write_pp_directives (f, &pf->opt [i], 1);
            }
        }


        /* GNU libc code.  */
        fputs ("\
      {0, 0, 0, 0}};\n\
    const int opt = getopt_long (argc, argv, \"", f);


        /* GNU libc short option definitions.  */
        {
            size_t i;
            for (i = 0; i < pf->num_opt; ++i)
                if (pf->opt [i].short_option != '\0')
                {
                    if (pf->opt [i].pp_directives_len > 0)
                    {
                        if (i == 0 || pf->opt [i - 1].pp_directives_len == 0)
                            fputs ("\"\n", f);
                        write_pp_directives (f, &pf->opt [i], 0);
                        fputs ("                                             \"", f);
                    }

                    putc (pf->opt [i].short_option, f);
                    if (pf->opt [i].flags & FLAGS_ARG)
                        putc (':', f);
                    if (pf->opt [i].flags & FLAG_OPTARG)
                        putc (':', f);

                    if (pf->opt [i].pp_directives_len > 0)
                    {
                        fputs ("\"\n", f);
                        write_pp_directives (f, &pf->opt [i], 1);
                        if (i + 1 == pf->num_opt || pf->opt [i + 1].pp_directives_len == 0)
                            fputs ("                                             \"", f);
                    }
                }
        }


        /* GNU libc code.  */
        fputs ("\", options, (int *)0);\n\
    switch (opt)\n\
    {\n\
     case -1:\n\
      return optind;\n", f);


        /* GNU libc option switch.  */
        {
            ssize_t question_mark = -1;
            int optnum = 256;
            size_t i;

            for (i = 0; i < pf->num_opt; ++i)
            {
                write_pp_directives (f, &pf->opt [i], 0);

                if (pf->opt [i].short_option != '\0')
                {
                    if (pf->opt [i].short_option != '?')
                        fprintf (f, "     case '%c':\n", pf->opt [i].short_option);
                    else
                        question_mark = (ssize_t)i;
                }
                else
                    fprintf (f, "     case %d:\n", optnum++);

                fprintf (f, "      %s%s%s = 1;\n", opt_struct ? "options->" : "", arg_option_prefix,
                         pf->opt [i].varname);

                if (pf->opt [i].flags & FLAGS_ARG)
                    fprintf (f, "      %s%s%s = optarg;\n", opt_struct ? "options->" : "", arg_argument_prefix,
                             pf->opt [i].varname);

                fputs (pf->opt [i].flags & FLAG_RETURN ? "      return optind;\n" : "      break;\n", f);

                write_pp_directives (f, &pf->opt [i], 1);
            }

            if (question_mark >= 0)
            {
                fputs ("     default:\n", f);

                write_pp_directives (f, &pf->opt [question_mark], 0);

                fprintf (f, "\
      if (opt == '?' && optopt == '?')\n\
      {\n\
        %s%s%s = 1;\n", opt_struct ? "options->" : "", arg_option_prefix, pf->opt [question_mark].varname);

                if (pf->opt [question_mark].flags & FLAGS_ARG)
                    fprintf (f, "        %s%s%s = optarg;\n", opt_struct ? "options->" : "", arg_argument_prefix,
                             pf->opt [question_mark].varname);

                fputs (pf->opt [question_mark].flags & FLAG_RETURN
                       ? "        return optind;\n" : "        break;\n", f);

                fputs ("\
      }\n\
      else\n", f);

                write_pp_directives (f, &pf->opt [question_mark], 1);

                fputs ("        return -1;\n", f);
            }
            else
                fputs ("     default:\n      return -1;\n", f);
        }


        /* GNU libc code.  */
        fputs ("\
    }\n\
  }\n\
#else\n", f);
    }


    /* Parsing code.  */
    fprintf (f, "\
  while (++i < argc)\n\
  {\n\
    const char *option = argv [i];\n\
    if (*option != '-')\n\
      return i;\n\
    else if (*++option == '\\0')\n\
      return i;\n\
    else if (*option == '-')\n\
    {\n\
      const char *argument;\n\
      %ssize_t option_len;\n\
      ++option;\n\
      if ((argument = %sstrchr (option, '=')) == option)\n", opt_cxx ? "::std::" : "", opt_cxx ? "::std::" : "");

    if (opt_cxx)
        fprintf (f, "        throw %s (STR_ERR_UNKNOWN_LONG_OPT);\n",
                 opt_exception_name ? arg_exception_name : "::std::runtime_error");
    else
        fputs ("        goto error_unknown_long_opt;\n", f);

    fprintf (f, "\
      else if (argument == 0)\n\
        option_len = %sstrlen (option);\n\
      else\n\
        option_len = argument++ - option;\n\
      switch (*option)\n\
      {\n\
       case '\\0':\n\
        return i + 1;\n", opt_cxx ? "::std::" : "");


    /* Sort options by long option name.  */
    qsort (pf->opt, pf->num_opt, sizeof *pf->opt, compare_long_opt);


    /* Long option parsing.  */
    {
        char first_character = '\0';
        size_t last_prefix_len = 0, i;

        for (i = 0; i < pf->num_opt; ++i)
        {
            size_t prefix_len, next_prefix_len;

            if (first_character != *pf->opt [i].long_option)
            {
                first_character = *pf->opt [i].long_option;
                if (i > 0)
                {
                    if (opt_cxx)
                        fprintf (f, "        throw %s (STR_ERR_UNKNOWN_LONG_OPT);\n",
                                 opt_exception_name ? arg_exception_name : "::std::runtime_error");
                    else
                        fputs ("        goto error_unknown_long_opt;\n", f);
                }
                fprintf (f, "       case '%c':\n", first_character);
            }

            write_pp_directives (f, &pf->opt [i], 0);

            fputs ("        if (", f);

            if (i < pf->num_opt - 1)
                next_prefix_len = common_prefix_len (pf->opt [i].long_option, pf->opt [i + 1].long_option);
            else
                next_prefix_len = 0;
            prefix_len = next_prefix_len > last_prefix_len ? next_prefix_len : last_prefix_len;
            last_prefix_len = next_prefix_len;

            if (pf->opt [i].long_option [1] != '\0')
            {
                if (pf->opt [i].flags & FLAG_OPTARG)
                    fprintf (f, "%sstrncmp (option + 1, \"%s\", option_len - 1) == 0)\n        {\n",
                             opt_cxx ? "::std::" : "", pf->opt [i].long_option + 1);
                else
                    fprintf (f, "%sstrncmp (option + 1, optstr__%s + 1, option_len - 1) == 0)\n        {\n",
                             opt_cxx ? "::std::" : "", pf->opt [i].varname);
            }
            else
                fputs ("option_len == 1)\n        {\n", f);
            if (prefix_len > 0 && pf->opt [i].long_option [1] != '\0')
            {
                fprintf (f, "          if (option_len %s %u)\n",
                         strlen (pf->opt [i].long_option) == prefix_len ? "<" : "<=", (unsigned int)prefix_len);
                if (opt_cxx)
                    fprintf (f, "            throw %s (STR_ERR_LONG_OPT_AMBIGUOUS);\n",
                             opt_exception_name ? arg_exception_name : "::std::runtime_error");
                else
                    fputs ("            goto error_long_opt_ambiguous;\n", f);
                flag_error_long_opt_ambiguous = 1;
            }

            if (pf->opt [i].flags & FLAG_OPTARG)
                fprintf (f, "          %s%s%s = argument;\n", opt_struct ? "options->" : "", arg_argument_prefix,
                         pf->opt [i].varname);
            else if (pf->opt [i].flags & FLAG_REQARG)
            {
                fprintf (f, "\
          if (argument != 0)\n\
            %s%s%s = argument;\n\
          else if (++i < argc)\n\
            %s%s%s = argv [i];\n\
          else\n\
          {\n\
            option = optstr__%s;\n",
                         opt_struct ? "options->" : "", arg_argument_prefix, pf->opt [i].varname,
                         opt_struct ? "options->" : "", arg_argument_prefix, pf->opt [i].varname, pf->opt [i].varname);
                if (opt_cxx)
                    fprintf (f, "\
            throw %s (STR_ERR_MISSING_ARG_LONG);\n\
          }\n", opt_exception_name ? arg_exception_name : "::std::runtime_error");
                else
                    fputs ("\
            goto error_missing_arg_long;\n\
          }\n", f);
                flag_error_missing_arg_long = 1;
            }
            else
            {
                fprintf (f, "\
          if (argument != 0)\n\
          {\n\
            option = optstr__%s;\n", pf->opt [i].varname);
                if (opt_cxx)
                    fprintf (f, "\
            throw %s (STR_ERR_UNEXPEC_ARG_LONG);\n\
          }\n", opt_exception_name ? arg_exception_name : "::std::runtime_error");
                else
                    fputs ("\
            goto error_unexpec_arg_long;\n\
          }\n", f);
                flag_error_unexpec_arg_long = 1;
            }

            fprintf (f, "          %s%s%s = 1;\n", opt_struct ? "options->" : "", arg_option_prefix,
                     pf->opt [i].varname);
            fputs (pf->opt [i].flags & FLAG_RETURN
                   ? "          return i + 1;\n        }\n" : "          break;\n        }\n", f);

            write_pp_directives (f, &pf->opt [i], 1);
        }
    }


    /* Parsing code.  */
    fputs ("       default:\n", f);

    if (opt_cxx)
        fprintf (f, "        throw %s (STR_ERR_UNKNOWN_LONG_OPT);\n",
                 opt_exception_name ? arg_exception_name : "::std::runtime_error");
    else
        fputs ("\
       error_unknown_long_opt:\n\
        fprintf (stderr, STR_ERR_UNKNOWN_LONG_OPT, program_name, option);\n\
        return -1;\n", f);

    if (!opt_cxx && flag_error_long_opt_ambiguous)
        fputs ("\
       error_long_opt_ambiguous:\n\
        fprintf (stderr, STR_ERR_LONG_OPT_AMBIGUOUS, program_name, option);\n\
        return -1;\n", f);

    if (!opt_cxx && flag_error_missing_arg_long)
        fputs ("\
       error_missing_arg_long:\n\
        fprintf (stderr, STR_ERR_MISSING_ARG_LONG, program_name, option);\n\
        return -1;\n", f);

    if (!opt_cxx && flag_error_unexpec_arg_long)
        fputs ("\
       error_unexpec_arg_long:\n\
        fprintf (stderr, STR_ERR_UNEXPEC_ARG_LONG, program_name, option);\n\
        return -1;\n", f);

    fputs ("\
      }\n\
    }\n\
    else\n\
      do\n\
      {\n\
        switch (*option)\n\
        {\n", f);


    /* Sort options by short option character.  */
    qsort (pf->opt, pf->num_opt, sizeof *pf->opt, compare_short_opt);


    /* Short option parsing.  */
    {
        size_t i;
        for (i = 0; i < pf->num_opt; ++i)
            if (pf->opt [i].short_option != '\0')
            {
                write_pp_directives (f, &pf->opt [i], 0);

                fprintf (f, "         case '%c':\n", pf->opt [i].short_option);

                if (pf->opt [i].flags & FLAG_OPTARG)
                    fprintf (f, "\
          if (option [1] != '\\0')\n\
          {\n\
            %s%s%s = option + 1;\n\
            option = \"\\0\";\n\
          }\n\
          else\n\
            %s%s%s = 0;\n",
                             opt_struct ? "options->" : "", arg_argument_prefix, pf->opt [i].varname,
                             opt_struct ? "options->" : "", arg_argument_prefix, pf->opt [i].varname);
                else if (pf->opt [i].flags & FLAG_REQARG)
                {
                    fprintf (f, "\
          if (option [1] != '\\0')\n\
            %s%s%s = option + 1;\n\
          else if (++i < argc)\n\
            %s%s%s = argv [i];\n\
          else\n",
                             opt_struct ? "options->" : "", arg_argument_prefix, pf->opt [i].varname,
                             opt_struct ? "options->" : "", arg_argument_prefix, pf->opt [i].varname);
                    if (opt_cxx)
                        fprintf (f, "\
            throw %s (STR_ERR_MISSING_ARG_SHORT);\n\
          option = \"\\0\";\n", opt_exception_name ? arg_exception_name : "::std::runtime_error");
                    else
                        fputs ("\
            goto error_missing_arg_short;\n\
          option = \"\\0\";\n", f);
                    flag_error_missing_arg_short = 1;
                }

                fprintf (f, "          %s%s%s = 1;\n", opt_struct ? "options->" : "", arg_option_prefix,
                         pf->opt [i].varname);
                fputs (pf->opt [i].flags & FLAG_RETURN ? "          return i + 1;\n" : "          break;\n", f);

                write_pp_directives (f, &pf->opt [i], 1);
            }
    }


    /* Parsing code.  */
    fputs ("         default:\n", f);

    if (opt_cxx)
        fprintf (f, "          throw %s (STR_ERR_UNKNOWN_SHORT_OPT);\n",
                 opt_exception_name ? arg_exception_name : "::std::runtime_error");
    else
        fputs ("\
          fprintf (stderr, STR_ERR_UNKNOWN_SHORT_OPT, program_name, *option);\n\
          return -1;\n", f);

    if (!opt_cxx && flag_error_missing_arg_short)
        fputs ("\
         error_missing_arg_short:\n\
          fprintf (stderr, STR_ERR_MISSING_ARG_SHORT, program_name, *option);\n\
          return -1;\n", f);

    fputs ("\
        }\n\
      } while (*++option != '\\0');\n\
  }\n\
  return i;\n", f);


    /* GNU libc code.  */
    if (opt_glibc)
        fputs ("#endif\n", f);


    /* End of function.  */
    fputs ("}\n", f);


    /* End of namespace.  */
    if (opt_namespace)
        fputs ("\n}\n", f);


    /* Footer.  */
    if (pf->footer_len > 0 && fwrite (pf->footer, 1, pf->footer_len, f) != pf->footer_len)
        return -1;


    return ferror (f) ? -1 : 0;
}



/* Output preprocessor directives associated with an option.  */
static void
write_pp_directives (FILE *const f, const struct opt *const opt, const int flag_closing)
{
    size_t i;

    for (i = 0; i < opt->pp_directives_len; ++i)
        if (flag_closing)
            fputs ("#endif\n", f);
        else
        {
            fputs ("#if", f);
            fwrite (opt->pp_directives [i].str, 1, opt->pp_directives [i].len, f);
            putc ('\n', f);
        }
}



/* Output a help text string.  The enclosing quotes are not written by this function.  */
static void
write_help_text (FILE *const f, const struct opt *const opt, size_t left_column, const size_t max_column)
{
    size_t column;


    if (opt->short_option != '\0')
        fprintf (f, "  -%c, --%s", opt->short_option, opt->long_option);
    else
        fprintf (f, "      --%s", opt->long_option);

    if (opt->arg_string != 0)
        write_c_string (f, opt->arg_string, 0);


    column = opt->opt_str_len + 8;

    if (column + 4 > left_column)
    {
        fputs ("\\n\\\n", f);
        column = 0;
    }

    while (column++ < left_column)
        putc (' ', f);


    column = left_column;
    left_column += 2;

    {
        const char *ptr = opt->help_string;
        while (1)
        {
            if (strlen (ptr) > max_column - column)
            {
                const char *ptr1 = ptr + (max_column - column);

                while (ptr1 > ptr && *--ptr1 != ' ');

                if (ptr1 > ptr)
                {
                    size_t len = ptr1 - ptr;
                    while (len > 0 && ptr [len - 1] == ' ')
                        --len;
                    write_c_string (f, ptr, len);
                    ptr = ptr1 + 1;
                }
                else
                {
                    ptr1 = ptr + (max_column - column);
                    write_c_string (f, ptr, ptr1 - ptr);
                    ptr = ptr1;
                }

                fputs ("\\n\\\n", f);
                {
                    size_t i;
                    for (i = 0; i < left_column; ++i)
                        putc (' ', f);
                }

                column = left_column;
            }
            else
            {
                fputs (ptr, f);
                break;
            }
        }
    }


    fputs ("\\n", f);
}



/* Output a C string. If `max_chars' is zero, output the whole string,
   otherwise don't output more than `max_chars' characters.  */
static void
write_c_string (FILE *const f, const char *s, const size_t max_chars)
{
    size_t n = 0;

    while (*s != '\0' && (max_chars == 0 || n < max_chars))
    {
        if (*s == '\\')
            fputs ("\\\\", f);
        else if (*s == '"')
            fputs ("\\\"", f);
        else
            putc (*s, f);

        ++s;
        ++n;
    }
}




/* Length of the common prefix of two strings.  */
static size_t
common_prefix_len (const char *a, const char *b)
{
    size_t n = 0;

    while (*a != '\0' && *a == *b)
    {
        ++n;
        ++a;
        ++b;
    }

    return n;
}



/* Compare the long option names of two option definitions.  */
static int
compare_long_opt (const void *const a, const void *const b)
{
    return strcmp (((const struct opt *)a)->long_option, ((const struct opt *)b)->long_option);
}



/* Compare the short option characters of two option definitions.
   Definitions without short option sort at the end.  */
static int
compare_short_opt (const void *const a, const void *const b)
{
    const struct opt *const a1 = a;
    const struct opt *const b1 = b;

    if (a1->short_option == '\0')
    {
        if (b1->short_option == '\0')
            return compare_long_opt (a, b);
        else
            return 1;
    }
    else
    {
        if (b1->short_option == '\0')
            return -1;
        else
            return (unsigned char)a1->short_option - (unsigned char)b1->short_option;
    }
}

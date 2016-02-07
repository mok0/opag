/* This file is part of opag, an option parser generator.
   Copyright (C) 2003, 2004 Martin Dickopp

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

#include "util.h"
#include "fileread.h"
#include "opag.h"
#include "options.h"

/* Name of this program.  */
#define PROGRAM_NAME PACKAGE_TARNAME



/* Text to display in response to the --help command line option.  */
extern const char *const usage_text;

/* Text to display in response to the --version command line option.  */
extern const char *const version_text;



int
main (int argc, char *argv [])
{
    const char *infilename = 0, *cfilename = 0, *hfilename = 0;
    struct filebuffer infilebuffer;
    struct parsed_infile parsed_file;



    /* Store program invocation name.  */
    invocation_name = argv [0] != 0 && *(argv [0]) != '\0' ? argv [0] : PROGRAM_NAME;


    /* Parse command line arguments.  */
    {
        int i;

        if ((i = parse_options (invocation_name, argc, argv)) == -1)
            goto error_try_help;

        if (opt_help)
        {
            printf (usage_text, invocation_name);
            exit (0);
        }

        if (opt_version)
        {
            fputs (version_text, stdout);
            exit (0);
        }

        if (opt_glibc && opt_cxx)
        {
            fprintf (stderr, "%s: options `--glibc' and `--cxx' are mutually exclusive\n", invocation_name);
            exit (2);
        }

        if (opt_struct)
        {
            if (arg_struct != 0)
            {
                if (!c_identifier (arg_struct))
                {
                    fprintf (stderr, "%s: `%s' is not a valid %s identifier\n", invocation_name, arg_struct,
                             opt_cxx ? "C++" : "C");
                    exit (2);
                }
            }
            else
                arg_struct = "options";
        }

        if (opt_function_name)
        {
            if (!c_identifier (arg_function_name))
            {
                fprintf (stderr, "%s: `%s' is not a valid %s identifier\n", invocation_name, arg_function_name,
                         opt_cxx ? "C++" : "C");
                exit (2);
            }
        }

        if (opt_nls_error_text)
        {
            if (opt_cxx)
            {
                fprintf (stderr, "%s: option `--nls-error-text' cannot be used with `--cxx'\n", invocation_name);
                exit (2);
            }

            if (arg_nls_error_text != 0)
            {
                if (!c_identifier (arg_nls_error_text))
                {
                    fprintf (stderr, "%s: `%s' is not a valid C identifier\n", invocation_name, arg_nls_error_text);
                    exit (2);
                }
            }
            else
                arg_nls_error_text = "gettext";
        }

        if (opt_nls_help_strings)
        {
            if (opt_cxx)
            {
                fprintf (stderr, "%s: option `--nls-help-strings' cannot be used with `--cxx'\n", invocation_name);
                exit (2);
            }

            if (arg_nls_help_strings != 0)
            {
                if (!c_identifier (arg_nls_help_strings))
                {
                    fprintf (stderr, "%s: `%s' is not a valid C identifier\n", invocation_name, arg_nls_help_strings);
                    exit (2);
                }
            }
            else
                arg_nls_help_strings = "gettext";
        }

        if (opt_nls_help_text)
        {
            if (opt_cxx)
            {
                fprintf (stderr, "%s: option `--nls-help-text' cannot be used with `--cxx'\n", invocation_name);
                exit (2);
            }

            if (arg_nls_help_text != 0)
            {
                if (!c_identifier (arg_nls_help_text))
                {
                    fprintf (stderr, "%s: `%s' is not a valid C identifier\n", invocation_name, arg_nls_help_text);
                    exit (2);
                }
            }
            else
                arg_nls_help_text = "gettext";
        }

        if (opt_exception_name)
        {
            if (!opt_cxx)
            {
                fprintf (stderr, "%s: option `--exception-name' cannot be used without `--cxx'\n", invocation_name);
                exit (2);
            }

            if (!cxx_scoped_identifier (arg_exception_name))
            {
                fprintf (stderr, "%s: `%s' is not a valid C++ typename\n", invocation_name, arg_exception_name);
                exit (2);
            }
        }

        if (opt_namespace)
        {
            if (!opt_cxx)
            {
                fprintf (stderr, "%s: option `--namespace' cannot be used without `--cxx'\n", invocation_name);
                exit (2);
            }

            if (!c_identifier (arg_namespace))
            {
                fprintf (stderr, "%s: `%s' is not a valid C++ identifier\n", invocation_name, arg_namespace);
                exit (2);
            }
        }

        if (opt_option_prefix)
        {
            if (!c_identifier (arg_option_prefix))
            {
                fprintf (stderr, "%s: `%s' is not a valid %s identifier\n", invocation_name, arg_option_prefix,
                         opt_cxx ? "C++" : "C");
                exit (2);
            }
        }
        else
            arg_option_prefix = "opt_";

        if (opt_argument_prefix)
        {
            if (!c_identifier (arg_argument_prefix))
            {
                fprintf (stderr, "%s: `%s' is not a valid %s identifier\n", invocation_name, arg_argument_prefix,
                         opt_cxx ? "C++" : "C");
                exit (2);
            }
        }
        else
            arg_argument_prefix = "arg_";

        if ((opt_option_prefix || opt_argument_prefix) && strcmp (arg_option_prefix, arg_argument_prefix) == 0)
        {
            fprintf (stderr, "%s: option and argument variable prefixes must differ\n", invocation_name);
            exit (2);
        }

        if (i < argc)
        {
            if (strcmp (argv [i], "-") != 0)
                infilename = argv [i];

            if (++i < argc)
            {
                if (strcmp (argv [i], "-") != 0)
                    cfilename = argv [i];

                if (++i < argc)
                {
                    hfilename = argv [i];

                    if (++i < argc)
                    {
                        fprintf (stderr, "%s: unrecognized argument `%s'\n", invocation_name, argv [i]);
                      error_try_help:
                        fprintf (stderr, "Try `%s --help' for more information.\n", invocation_name);
                        exit (2);
                    }
                }
            }
        }
    }


    /* Read input file.  */
    file_read (infilename, &infilebuffer);


    /* Parse input file.  */
    parse_file (infilename != 0 ? infilename : "STDIN", infilebuffer.ptr, infilebuffer.ptr + infilebuffer.len,
                &parsed_file);


    /* Create option parsing code and header.  */
    output_code (cfilename, hfilename, &parsed_file);


    exit (0);
}

/* This file has been generated with opag 0.8.0.  */
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


#ifndef STR_ERR_UNKNOWN_LONG_OPT
# define STR_ERR_UNKNOWN_LONG_OPT   "%s: unrecognized option `--%s'\n"
#endif

#ifndef STR_ERR_LONG_OPT_AMBIGUOUS
# define STR_ERR_LONG_OPT_AMBIGUOUS "%s: option `--%s' is ambiguous\n"
#endif

#ifndef STR_ERR_MISSING_ARG_LONG
# define STR_ERR_MISSING_ARG_LONG   "%s: option `--%s' requires an argument\n"
#endif

#ifndef STR_ERR_UNEXPEC_ARG_LONG
# define STR_ERR_UNEXPEC_ARG_LONG   "%s: option `--%s' doesn't allow an argument\n"
#endif

#ifndef STR_ERR_UNKNOWN_SHORT_OPT
# define STR_ERR_UNKNOWN_SHORT_OPT  "%s: unrecognized option `-%c'\n"
#endif

#ifndef STR_ERR_MISSING_ARG_SHORT
# define STR_ERR_MISSING_ARG_SHORT  "%s: option `-%c' requires an argument\n"
#endif

#define STR_HELP_GLIBC "\
  -g, --glibc                           generate code that makes use of GNU\n\
                                          libc functionality if available\n"

#define STR_HELP_GCC "\
  -G, --gcc                             generate code that makes use of GCC\n\
                                          functionality if available\n"

#define STR_HELP_CXX "\
  -C, --cxx                             generate C++ code instead of C code\n"

#define STR_HELP_STRUCT "\
  -s, --struct[=TAG]                    use struct with tag TAG (default:\n\
                                          `options') instead of global\n\
                                          variables\n"

#define STR_HELP_FUNCTION_NAME "\
  -f, --function-name=IDENTIFIER        use IDENTIFIER as function name\n\
                                          instead of `parse_options'\n"

#define STR_HELP_NLS_ERROR_TEXT "\
      --nls-error-text[=IDENTIFIER]     when generating C code, wrap each\n\
                                          error text macro in call to function\n\
                                          IDENTIFIER (default: `gettext')\n"

#define STR_HELP_NLS_HELP_STRINGS "\
      --nls-help-strings[=IDENTIFIER]\n\
                                        when generating C code, wrap each help\n\
                                          string macro in call to function\n\
                                          IDENTIFIER (default: `gettext')\n"

#define STR_HELP_NLS_HELP_TEXT "\
      --nls-help-text[=IDENTIFIER]      when generating C code, wrap complete\n\
                                          help string macro in call to\n\
                                          function IDENTIFIER (default:\n\
                                          `gettext')\n"

#define STR_HELP_EXCEPTION_NAME "\
  -e, --exception-name=TYPENAME         when generating C++ code, throw\n\
                                          TYPENAME instead of\n\
                                          `::std::runtime_error'\n"

#define STR_HELP_NAMESPACE "\
  -n, --namespace=IDENTIFIER            put generated C++ code into namespace\n\
                                          IDENTIFIER\n"

#define STR_HELP_OPTION_PREFIX "\
  -O, --option-prefix=PREFIX            prefix option variables with PREFIX\n\
                                          instead of `opt_'\n"

#define STR_HELP_ARGUMENT_PREFIX "\
  -A, --argument-prefix=PREFIX          prefix argument variables with PREFIX\n\
                                          instead of `arg_'\n"

#define STR_HELP_HELP "\
      --help                            display this help text and exit\n"

#define STR_HELP_VERSION "\
      --version                         display version information and exit\n"

#define STR_HELP "\
  -g, --glibc                           generate code that makes use of GNU\n\
                                          libc functionality if available\n\
  -G, --gcc                             generate code that makes use of GCC\n\
                                          functionality if available\n\
  -C, --cxx                             generate C++ code instead of C code\n\
  -s, --struct[=TAG]                    use struct with tag TAG (default:\n\
                                          `options') instead of global\n\
                                          variables\n\
  -f, --function-name=IDENTIFIER        use IDENTIFIER as function name\n\
                                          instead of `parse_options'\n\
      --nls-error-text[=IDENTIFIER]     when generating C code, wrap each\n\
                                          error text macro in call to function\n\
                                          IDENTIFIER (default: `gettext')\n\
      --nls-help-strings[=IDENTIFIER]\n\
                                        when generating C code, wrap each help\n\
                                          string macro in call to function\n\
                                          IDENTIFIER (default: `gettext')\n\
      --nls-help-text[=IDENTIFIER]      when generating C code, wrap complete\n\
                                          help string macro in call to\n\
                                          function IDENTIFIER (default:\n\
                                          `gettext')\n\
  -e, --exception-name=TYPENAME         when generating C++ code, throw\n\
                                          TYPENAME instead of\n\
                                          `::std::runtime_error'\n\
  -n, --namespace=IDENTIFIER            put generated C++ code into namespace\n\
                                          IDENTIFIER\n\
  -O, --option-prefix=PREFIX            prefix option variables with PREFIX\n\
                                          instead of `opt_'\n\
  -A, --argument-prefix=PREFIX          prefix argument variables with PREFIX\n\
                                          instead of `arg_'\n\
      --help                            display this help text and exit\n\
      --version                         display version information and exit\n"

/* Set to 1 if option --glibc (-g) has been specified.  */
char opt_glibc;

/* Set to 1 if option --gcc (-G) has been specified.  */
char opt_gcc;

/* Set to 1 if option --cxx (-C) has been specified.  */
char opt_cxx;

/* Set to 1 if option --struct (-s) has been specified.  */
char opt_struct;

/* Set to 1 if option --function-name (-f) has been specified.  */
char opt_function_name;

/* Set to 1 if option --nls-error-text has been specified.  */
char opt_nls_error_text;

/* Set to 1 if option --nls-help-strings has been specified.  */
char opt_nls_help_strings;

/* Set to 1 if option --nls-help-text has been specified.  */
char opt_nls_help_text;

/* Set to 1 if option --exception-name (-e) has been specified.  */
char opt_exception_name;

/* Set to 1 if option --namespace (-n) has been specified.  */
char opt_namespace;

/* Set to 1 if option --option-prefix (-O) has been specified.  */
char opt_option_prefix;

/* Set to 1 if option --argument-prefix (-A) has been specified.  */
char opt_argument_prefix;

/* Set to 1 if option --help has been specified.  */
char opt_help;

/* Set to 1 if option --version has been specified.  */
char opt_version;

/* Argument to option --struct (-s), or a null pointer if no argument.  */
const char *arg_struct;

/* Argument to option --function-name (-f).  */
const char *arg_function_name;

/* Argument to option --nls-error-text, or a null pointer if no argument.  */
const char *arg_nls_error_text;

/* Argument to option --nls-help-strings, or a null pointer if no argument.  */
const char *arg_nls_help_strings;

/* Argument to option --nls-help-text, or a null pointer if no argument.  */
const char *arg_nls_help_text;

/* Argument to option --exception-name (-e).  */
const char *arg_exception_name;

/* Argument to option --namespace (-n).  */
const char *arg_namespace;

/* Argument to option --option-prefix (-O).  */
const char *arg_option_prefix;

/* Argument to option --argument-prefix (-A).  */
const char *arg_argument_prefix;

/* Parse command line options.  Return index of first non-option argument,
   or -1 if an error is encountered.  */
int parse_options (const char *const program_name, const int argc, char **const argv)
{
  static const char *const optstr__glibc = "glibc";
  static const char *const optstr__gcc = "gcc";
  static const char *const optstr__cxx = "cxx";
  static const char *const optstr__function_name = "function-name";
  static const char *const optstr__exception_name = "exception-name";
  static const char *const optstr__namespace = "namespace";
  static const char *const optstr__option_prefix = "option-prefix";
  static const char *const optstr__argument_prefix = "argument-prefix";
  static const char *const optstr__help = "help";
  static const char *const optstr__version = "version";
  int i = 0;
  opt_glibc = 0;
  opt_gcc = 0;
  opt_cxx = 0;
  opt_struct = 0;
  opt_function_name = 0;
  opt_nls_error_text = 0;
  opt_nls_help_strings = 0;
  opt_nls_help_text = 0;
  opt_exception_name = 0;
  opt_namespace = 0;
  opt_option_prefix = 0;
  opt_argument_prefix = 0;
  opt_help = 0;
  opt_version = 0;
  arg_struct = 0;
  arg_function_name = 0;
  arg_nls_error_text = 0;
  arg_nls_help_strings = 0;
  arg_nls_help_text = 0;
  arg_exception_name = 0;
  arg_namespace = 0;
  arg_option_prefix = 0;
  arg_argument_prefix = 0;
  while (++i < argc)
  {
    const char *option = argv [i];
    if (*option != '-')
      return i;
    else if (*++option == '\0')
      return i;
    else if (*option == '-')
    {
      const char *argument;
      size_t option_len;
      ++option;
      if ((argument = strchr (option, '=')) == option)
        goto error_unknown_long_opt;
      else if (argument == 0)
        option_len = strlen (option);
      else
        option_len = argument++ - option;
      switch (*option)
      {
       case '\0':
        return i + 1;
       case 'a':
        if (strncmp (option + 1, optstr__argument_prefix + 1, option_len - 1) == 0)
        {
          if (argument != 0)
            arg_argument_prefix = argument;
          else if (++i < argc)
            arg_argument_prefix = argv [i];
          else
          {
            option = optstr__argument_prefix;
            goto error_missing_arg_long;
          }
          opt_argument_prefix = 1;
          break;
        }
        goto error_unknown_long_opt;
       case 'c':
        if (strncmp (option + 1, optstr__cxx + 1, option_len - 1) == 0)
        {
          if (argument != 0)
          {
            option = optstr__cxx;
            goto error_unexpec_arg_long;
          }
          opt_cxx = 1;
          break;
        }
        goto error_unknown_long_opt;
       case 'e':
        if (strncmp (option + 1, optstr__exception_name + 1, option_len - 1) == 0)
        {
          if (argument != 0)
            arg_exception_name = argument;
          else if (++i < argc)
            arg_exception_name = argv [i];
          else
          {
            option = optstr__exception_name;
            goto error_missing_arg_long;
          }
          opt_exception_name = 1;
          break;
        }
        goto error_unknown_long_opt;
       case 'f':
        if (strncmp (option + 1, optstr__function_name + 1, option_len - 1) == 0)
        {
          if (argument != 0)
            arg_function_name = argument;
          else if (++i < argc)
            arg_function_name = argv [i];
          else
          {
            option = optstr__function_name;
            goto error_missing_arg_long;
          }
          opt_function_name = 1;
          break;
        }
        goto error_unknown_long_opt;
       case 'g':
        if (strncmp (option + 1, optstr__gcc + 1, option_len - 1) == 0)
        {
          if (option_len <= 1)
            goto error_long_opt_ambiguous;
          if (argument != 0)
          {
            option = optstr__gcc;
            goto error_unexpec_arg_long;
          }
          opt_gcc = 1;
          break;
        }
        if (strncmp (option + 1, optstr__glibc + 1, option_len - 1) == 0)
        {
          if (option_len <= 1)
            goto error_long_opt_ambiguous;
          if (argument != 0)
          {
            option = optstr__glibc;
            goto error_unexpec_arg_long;
          }
          opt_glibc = 1;
          break;
        }
        goto error_unknown_long_opt;
       case 'h':
        if (strncmp (option + 1, optstr__help + 1, option_len - 1) == 0)
        {
          if (argument != 0)
          {
            option = optstr__help;
            goto error_unexpec_arg_long;
          }
          opt_help = 1;
          return i + 1;
        }
        goto error_unknown_long_opt;
       case 'n':
        if (strncmp (option + 1, optstr__namespace + 1, option_len - 1) == 0)
        {
          if (option_len <= 1)
            goto error_long_opt_ambiguous;
          if (argument != 0)
            arg_namespace = argument;
          else if (++i < argc)
            arg_namespace = argv [i];
          else
          {
            option = optstr__namespace;
            goto error_missing_arg_long;
          }
          opt_namespace = 1;
          break;
        }
        if (strncmp (option + 1, "ls-error-text", option_len - 1) == 0)
        {
          if (option_len <= 4)
            goto error_long_opt_ambiguous;
          arg_nls_error_text = argument;
          opt_nls_error_text = 1;
          break;
        }
        if (strncmp (option + 1, "ls-help-strings", option_len - 1) == 0)
        {
          if (option_len <= 9)
            goto error_long_opt_ambiguous;
          arg_nls_help_strings = argument;
          opt_nls_help_strings = 1;
          break;
        }
        if (strncmp (option + 1, "ls-help-text", option_len - 1) == 0)
        {
          if (option_len <= 9)
            goto error_long_opt_ambiguous;
          arg_nls_help_text = argument;
          opt_nls_help_text = 1;
          break;
        }
        goto error_unknown_long_opt;
       case 'o':
        if (strncmp (option + 1, optstr__option_prefix + 1, option_len - 1) == 0)
        {
          if (argument != 0)
            arg_option_prefix = argument;
          else if (++i < argc)
            arg_option_prefix = argv [i];
          else
          {
            option = optstr__option_prefix;
            goto error_missing_arg_long;
          }
          opt_option_prefix = 1;
          break;
        }
        goto error_unknown_long_opt;
       case 's':
        if (strncmp (option + 1, "truct", option_len - 1) == 0)
        {
          arg_struct = argument;
          opt_struct = 1;
          break;
        }
        goto error_unknown_long_opt;
       case 'v':
        if (strncmp (option + 1, optstr__version + 1, option_len - 1) == 0)
        {
          if (argument != 0)
          {
            option = optstr__version;
            goto error_unexpec_arg_long;
          }
          opt_version = 1;
          return i + 1;
        }
       default:
       error_unknown_long_opt:
        fprintf (stderr, STR_ERR_UNKNOWN_LONG_OPT, program_name, option);
        return -1;
       error_long_opt_ambiguous:
        fprintf (stderr, STR_ERR_LONG_OPT_AMBIGUOUS, program_name, option);
        return -1;
       error_missing_arg_long:
        fprintf (stderr, STR_ERR_MISSING_ARG_LONG, program_name, option);
        return -1;
       error_unexpec_arg_long:
        fprintf (stderr, STR_ERR_UNEXPEC_ARG_LONG, program_name, option);
        return -1;
      }
    }
    else
      do
      {
        switch (*option)
        {
         case 'A':
          if (option [1] != '\0')
            arg_argument_prefix = option + 1;
          else if (++i < argc)
            arg_argument_prefix = argv [i];
          else
            goto error_missing_arg_short;
          option = "\0";
          opt_argument_prefix = 1;
          break;
         case 'C':
          opt_cxx = 1;
          break;
         case 'G':
          opt_gcc = 1;
          break;
         case 'O':
          if (option [1] != '\0')
            arg_option_prefix = option + 1;
          else if (++i < argc)
            arg_option_prefix = argv [i];
          else
            goto error_missing_arg_short;
          option = "\0";
          opt_option_prefix = 1;
          break;
         case 'e':
          if (option [1] != '\0')
            arg_exception_name = option + 1;
          else if (++i < argc)
            arg_exception_name = argv [i];
          else
            goto error_missing_arg_short;
          option = "\0";
          opt_exception_name = 1;
          break;
         case 'f':
          if (option [1] != '\0')
            arg_function_name = option + 1;
          else if (++i < argc)
            arg_function_name = argv [i];
          else
            goto error_missing_arg_short;
          option = "\0";
          opt_function_name = 1;
          break;
         case 'g':
          opt_glibc = 1;
          break;
         case 'n':
          if (option [1] != '\0')
            arg_namespace = option + 1;
          else if (++i < argc)
            arg_namespace = argv [i];
          else
            goto error_missing_arg_short;
          option = "\0";
          opt_namespace = 1;
          break;
         case 's':
          if (option [1] != '\0')
          {
            arg_struct = option + 1;
            option = "\0";
          }
          else
            arg_struct = 0;
          opt_struct = 1;
          break;
         default:
          fprintf (stderr, STR_ERR_UNKNOWN_SHORT_OPT, program_name, *option);
          return -1;
         error_missing_arg_short:
          fprintf (stderr, STR_ERR_MISSING_ARG_SHORT, program_name, *option);
          return -1;
        }
      } while (*++option != '\0');
  }
  return i;
}


/* Text to display in response to the --help command line option.  */
const char *const usage_text = "\
Usage: %s [OPTION]... [INPUT-FILE [CODE-OUTPUT-FILE [HEADER-OUTPUT-FILE]]]\n\
Generate C code to parse command line options.\n\n\
Options:\n" STR_HELP "\n\
Please report bugs to <" PACKAGE_BUGREPORT ">.\n";

/* Text to display in response to the --version command line option.  */
const char *const version_text =
PACKAGE_STRING "\n\
Copyright (C) 2002, 2003, 2004, 2005 Martin Dickopp\n\n\
This program is free software; it may be copied and/or modified under the\n\
terms of the GNU General Public License version 2 or (at your option) any\n\
later version; see the file COPYING in the source package for details. There\n\
is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR\n\
PURPOSE.\n";

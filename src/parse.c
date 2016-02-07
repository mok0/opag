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

#include "util.h"
#include "chartype.h"
#include "opag.h"



/* Parse input file buffer.  */
void
parse_file (const char *const infilename, char *buffer, char *const buffer_end, struct parsed_infile *const pf)
{
    unsigned int lineno = 1;



    /* Find header delimiter.  */
    pf->header = buffer;

    {
        char *beginning_of_line = buffer;
        enum {BEGINNING_OF_LINE, PERCENT1, PERCENT2, OTHER} state = BEGINNING_OF_LINE;

        while (buffer < buffer_end)
        {
            switch (*buffer++)
            {
              case '\n':
                ++lineno;
                if (state == PERCENT2)
                    goto parsed_header_delim;
                state = BEGINNING_OF_LINE;
                beginning_of_line = buffer;
                break;

              case '%':
                switch (state)
                {
                  case BEGINNING_OF_LINE:
                    state = PERCENT1;
                    break;

                  case PERCENT1:
                    state = PERCENT2;
                    break;

                  default:
                    state = OTHER;
                }
                break;

              case ' ':
              case '\t':
                if (state != PERCENT2)
                    state = OTHER;
                break;

              default:
                state = OTHER;
            }
        }

        goto error_no_option_def;

      parsed_header_delim:
        pf->header_len = beginning_of_line - pf->header;
    }


    /* Parse option definitions.  */
    {
        size_t size_pp_directives = 8, pp_directives_len = 0;
        struct pp_directive *pp_directives = xmalloc (size_pp_directives * sizeof *pp_directives);

        size_t size_opt = 8;

        pf->footer_len = 0;
        pf->opt = xmalloc (size_opt * sizeof *pf->opt);
        pf->num_opt = 0;
        pf->max_opt_str_len = 0;

        while (buffer < buffer_end)
        {
            /* Test for footer deliminiter.  */
            if (*buffer == '%' && buffer + 1 < buffer_end && buffer [1] == '%')
            {
                char *ptr = buffer + 2;

                while (ptr < buffer_end && ISBLANK (*ptr))
                    ++ptr;
                if (ptr >= buffer_end || *ptr == '\n')
                {
                    if (ptr < buffer_end)
                    {
                        pf->footer = ptr + 1;
                        pf->footer_len = buffer_end - pf->footer;
                    }

                    break;
                }
            }


            /* Initialize option definition.  */
            if (pf->num_opt >= size_opt)
                pf->opt = xrealloc (pf->opt, (size_opt *= 2) * sizeof *pf->opt);

            pf->opt [pf->num_opt].short_option = '\0';
            pf->opt [pf->num_opt].help_string = 0;
            pf->opt [pf->num_opt].arg_string = 0;
            pf->opt [pf->num_opt].opt_str_len = 0;
            pf->opt [pf->num_opt].flags = 0;


            /* Parse preprocessor directives.  */
            if (*buffer == '#'
                && ((buffer + 3 < buffer_end
                     && buffer [1] == 'i' && buffer [2] == 'f' && ISBLANK ((unsigned char)buffer [3]))
                    || (buffer + 6 < buffer_end
                        && buffer [1] == 'i' && buffer [2] == 'f' && buffer [3] == 'd' && buffer [4] == 'e'
                        && buffer [5] == 'f' && ISBLANK ((unsigned char)buffer [6]))
                    || (buffer + 6 < buffer_end
                        && buffer [1] == 'e' && buffer [2] == 'n' && buffer [3] == 'd' && buffer [4] == 'i'
                        && buffer [5] == 'f' && (ISBLANK ((unsigned char)buffer [6]) || buffer [6] == '\n'))))
            {
                if (buffer [1] == 'i')
                {
                    /* #if or #ifdef directive.  */
                    if (pp_directives_len >= size_pp_directives)
                        pp_directives = xrealloc (pp_directives, (size_pp_directives *= 2) * sizeof *pp_directives);

                    pp_directives [pp_directives_len].str = buffer += 3;
                    pp_directives [pp_directives_len].len = 0;
                    pp_directives [pp_directives_len].lineno = lineno;
                    while (buffer < buffer_end && *buffer++ != '\n')
                        ++pp_directives [pp_directives_len].len;

                    ++pp_directives_len;
                    ++lineno;
                    continue;
                }
                else
                {
                    /* #endif directive.  */
                    if (pp_directives_len > 0)
                        --pp_directives_len;
                    else
                        fprintf (stderr, "%s: %s:%u: warning: `#endif' without matching `#if'/`#ifdef' ignored\n",
                                 invocation_name, infilename, lineno);

                    buffer += 6;
                    goto skip_remaining_line;
                }
            }


            /* Parse short option character.  */
            if (ISALNUM ((unsigned char)*buffer) || *buffer == '#' || *buffer == '?' || *buffer == '@')
            {
                size_t i;

                for (i = 0; i < pf->num_opt; ++i)
                    if (pf->opt [i].short_option == *buffer)
                    {
                        fprintf (stderr, "%s: %s:%u: short option character '%c' not unique\n",
                                 invocation_name, infilename, lineno, *buffer);
                        goto skip_remaining_line;
                    }

                pf->opt [pf->num_opt].short_option = *buffer++;
            }
            else if (!ISBLANK ((unsigned char)*buffer) && *buffer != '\n')
            {
                if (ISPRINT ((unsigned char)*buffer))
                    fprintf (stderr, "%s: %s:%u: invalid short option character '%c'\n",
                             invocation_name, infilename, lineno, *buffer);
                else
                    fprintf (stderr, "%s: %s:%u: invalid short option character '\\%o'\n",
                             invocation_name, infilename, lineno, (unsigned char)*buffer);

                goto skip_remaining_line;
            }


            /* Parse long option name.  */
            while (buffer < buffer_end && ISBLANK ((unsigned char)*buffer))
                ++buffer;
            if (buffer >= buffer_end || *buffer == '\n')
            {
                if (pf->opt [pf->num_opt].short_option != '\0')
                {
                    fprintf (stderr, "%s: %s:%u: long option missing\n",
                             invocation_name, infilename, lineno);
                }

                goto skip_remaining_line;
            }

            {
                char *ptr = buffer;
                size_t len, i;

                while (ptr < buffer_end && !ISBLANK (*ptr) && *ptr != '\n')
                    ++ptr;
                len = ptr - buffer + 1;

                pf->opt [pf->num_opt].long_option = xmalloc (3 * len);
                memcpy (pf->opt [pf->num_opt].long_option, buffer, len - 1);
                pf->opt [pf->num_opt].long_option [len - 1] = '\0';

                ptr = pf->opt [pf->num_opt].long_option;
                if (!ISALNUM ((unsigned char)*ptr))
                    goto invalid_long_option;
                while (*++ptr != '\0')
                    if (!ISALNUM ((unsigned char)*ptr) && *ptr != '-')
                    {
                      invalid_long_option:
                        fprintf (stderr, "%s: %s:%u: invalid long option `%s'\n",
                                 invocation_name, infilename, lineno, pf->opt [pf->num_opt].long_option);
                        free (pf->opt [pf->num_opt].long_option);
                        goto skip_remaining_line;
                    }

                pf->opt [pf->num_opt].varname = pf->opt [pf->num_opt].long_option + len;
                memcpy (pf->opt [pf->num_opt].varname, pf->opt [pf->num_opt].long_option, len);

                ptr = pf->opt [pf->num_opt].varname;
                while (*ptr != '\0')
                {
                    if (*ptr == '-')
                        *ptr = '_';
                    else
                        *ptr = TOLOWER (*ptr);
                    ++ptr;
                }

                for (i = 0; i < pf->num_opt; ++i)
                    if (strcmp (pf->opt [pf->num_opt].varname, pf->opt [i].varname) == 0)
                    {
                        fprintf (stderr, "%s: %s:%u: long option `%s' not unique\n",
                                 invocation_name, infilename, lineno, pf->opt [pf->num_opt].long_option);
                        free (pf->opt [pf->num_opt].long_option);
                        goto skip_remaining_line;
                    }

                pf->opt [pf->num_opt].macroname = pf->opt [pf->num_opt].varname + len;
                memcpy (pf->opt [pf->num_opt].macroname, pf->opt [pf->num_opt].varname, len);

                ptr = pf->opt [pf->num_opt].macroname;
                while (*ptr != '\0')
                {
                    *ptr = TOUPPER (*ptr);
                    ++ptr;
                }

                buffer += len - 1;
            }


            /* Parse help string.  */
            while (buffer < buffer_end && ISBLANK ((unsigned char)*buffer))
                ++buffer;
            if (buffer >= buffer_end || *buffer == '\n')
                goto parsed_option_def;

            if (*buffer == '"')
            {
                const char *read_ptr;
                char *write_ptr;

                pf->opt [pf->num_opt].help_string = ++buffer;

                while (buffer < buffer_end && (*buffer != '"' || buffer [-1] == '\\') && *buffer != '\n')
                    ++buffer;

                if (buffer >= buffer_end || *buffer == '\n')
                {
                    fprintf (stderr, "%s: %s:%u: warning: unterminated help string ignored\n",
                             invocation_name, infilename, lineno);
                    pf->opt [pf->num_opt].help_string = 0;
                    goto parsed_option_def;
                }

                *buffer++ = '\0';

                /* Decode C string.  */
                read_ptr = write_ptr = pf->opt [pf->num_opt].help_string;
                while (*read_ptr != '\0')
                {
                    if (*read_ptr == '\\')
                    {
                        if (*++read_ptr == '\\')
                            *write_ptr++ = '\\';
                        else if (*read_ptr == '"')
                            *write_ptr++ = '"';
                        else if ('0' <= *read_ptr && *read_ptr <= '7')
                        {
                            int value = *read_ptr - '0';
                            if ('0' <= read_ptr [1] && read_ptr [1] <= '7')
                            {
                                value = value * 8 + (*++read_ptr - '0');
                                if ('0' <= read_ptr [1] && read_ptr [1] <= '7')
                                    value = value * 8 + (*++read_ptr - '0');
                            }
                            if (value < 256 && !ISCNTRL (value))
                                *write_ptr++ = value;
                            else
                                fprintf (stderr, "%s: %s:%u: warning: escape sequence '\\%o' in help string ignored\n",
                                         invocation_name, infilename, lineno, value);
                        }
                        else
                            fprintf (stderr, "%s: %s:%u: warning: escape sequence '\\%c' in help string ignored\n",
                                     invocation_name, infilename, lineno, (unsigned char)*read_ptr);
                    }
                    else if (!ISCNTRL ((unsigned char)*read_ptr))
                        *write_ptr++ = *read_ptr;
                    else
                        fprintf (stderr, "%s: %s:%u: warning: control character '\\%o' in help string ignored\n",
                                 invocation_name, infilename, lineno, (unsigned char)*read_ptr);
                    ++read_ptr;
                }
                *write_ptr = '\0';

                if (*pf->opt [pf->num_opt].help_string == '\0')
                {
                    fprintf (stderr, "%s: %s:%u: warning: empty help string ignored\n",
                             invocation_name, infilename, lineno);
                    pf->opt [pf->num_opt].help_string = 0;
                }
            }


            /* Parse flags.  */
            while (1)
            {
                while (buffer < buffer_end && ISBLANK ((unsigned char)*buffer))
                    ++buffer;
                if (buffer >= buffer_end || *buffer == '\n')
                    goto parsed_flags;

                {
                    char *const ptr = buffer;

                    while (buffer < buffer_end && !ISBLANK ((unsigned char)*buffer) && *buffer != '\n')
                        ++buffer;

                    if (buffer - ptr == 6)
                    {
                        if (memcmp (ptr, "optarg", 6) == 0)
                        {
                            if (pf->opt [pf->num_opt].flags & FLAG_REQARG)
                            {
                                fprintf (stderr, "%s: %s:%u: warning: flags `reqarg' and `optarg' "
                                         "are mutually exclusive\n", invocation_name, infilename, lineno);
                                pf->opt [pf->num_opt].flags &= ~FLAG_REQARG;
                            }
                            pf->opt [pf->num_opt].flags |= FLAG_OPTARG;
                            continue;
                        }
                        else if (memcmp (ptr, "reqarg", 6) == 0)
                        {
                            if (pf->opt [pf->num_opt].flags & FLAG_OPTARG)
                            {
                                fprintf (stderr, "%s: %s:%u: warning: flags `optarg' and `reqarg' "
                                         "are mutually exclusive\n", invocation_name, infilename, lineno);
                                pf->opt [pf->num_opt].flags &= ~FLAG_OPTARG;
                            }
                            pf->opt [pf->num_opt].flags |= FLAG_REQARG;
                            continue;
                        }
                        else if (memcmp (ptr, "return", 6) == 0)
                        {
                            pf->opt [pf->num_opt].flags |= FLAG_RETURN;
                            continue;
                        }
                    }

                    fprintf (stderr, "%s: %s:%u: warning: invalid flag `", invocation_name, infilename, lineno);
                    fwrite (ptr, 1, buffer - ptr, stderr);
                    fputs ("' ignored\n", stderr);
                }
            }


            /* Separate argument name from help text.  */
          parsed_flags:
            if (pf->opt [pf->num_opt].help_string != 0)
            {
                pf->opt [pf->num_opt].opt_str_len = strlen (pf->opt [pf->num_opt].long_option);

                if (pf->opt [pf->num_opt].flags & FLAGS_ARG)
                {
                    char *ptr = pf->opt [pf->num_opt].help_string;

                    if (*ptr == ' ')
                    {
                        while (*ptr == ' ')
                            ++ptr;
                        if (*ptr != '\0')
                            pf->opt [pf->num_opt].help_string = ptr;
                    }
                    else
                    {
                        char *ptr1;

                        pf->opt [pf->num_opt].arg_string = ptr;

                        while (*ptr != ' ' && *ptr != '\0')
                            ++ptr;
                        ptr1 = ptr;
                        while (*ptr == ' ')
                            ++ptr;
                        if (ptr > ptr1 && *ptr != '\0')
                        {
                            pf->opt [pf->num_opt].arg_string = pf->opt [pf->num_opt].help_string;
                            pf->opt [pf->num_opt].help_string = ptr;
                            *ptr1 = '\0';

                            if (ISALNUM ((unsigned char)*pf->opt [pf->num_opt].arg_string)
                                || *pf->opt [pf->num_opt].arg_string == '-')
                                *--pf->opt [pf->num_opt].arg_string = ' ';
                        }
                        else
                            pf->opt [pf->num_opt].arg_string = 0;

                        if (pf->opt [pf->num_opt].arg_string != '\0')
                            pf->opt [pf->num_opt].opt_str_len += strlen (pf->opt [pf->num_opt].arg_string);
                    }
                }

                if (pf->opt [pf->num_opt].opt_str_len > pf->max_opt_str_len)
                    pf->max_opt_str_len = pf->opt [pf->num_opt].opt_str_len;
            }


          parsed_option_def:
            if (pp_directives_len > 0)
            {
                pf->opt [pf->num_opt].pp_directives = xmalloc (pp_directives_len * sizeof *pp_directives);
                memcpy (pf->opt [pf->num_opt].pp_directives, pp_directives, pp_directives_len * sizeof *pp_directives);
            }
            pf->opt [pf->num_opt].pp_directives_len = pp_directives_len;

            ++pf->num_opt;

          skip_remaining_line:
            while (buffer < buffer_end && *buffer++ != '\n');
            ++lineno;
        }


        /* Warn about unclosed preprocessor directives.  */
        {
            size_t i;
            for (i = 0; i < pp_directives_len; ++i)
            {
                fprintf (stderr, "%s: %s:%u: warning: `#if", invocation_name, infilename, pp_directives [i].lineno);
                fwrite (pp_directives [i].str, 1, pp_directives [i].len, stderr);
                fputs ("' has no matching `#endif'\n", stderr);
            }
        }

        free (pp_directives);

        if (pf->num_opt == 0)
        {
          error_no_option_def:
            fprintf (stderr, "%s: %s: no option definitions\n", invocation_name, infilename);
            exit (1);
        }
    }
}

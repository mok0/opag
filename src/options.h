/* This file has been generated with opag 0.8.0.  */

#ifndef HDR_OPTIONS
#define HDR_OPTIONS 1

/* Set to 1 if option --glibc (-g) has been specified.  */
extern char opt_glibc;

/* Set to 1 if option --gcc (-G) has been specified.  */
extern char opt_gcc;

/* Set to 1 if option --cxx (-C) has been specified.  */
extern char opt_cxx;

/* Set to 1 if option --struct (-s) has been specified.  */
extern char opt_struct;

/* Set to 1 if option --function-name (-f) has been specified.  */
extern char opt_function_name;

/* Set to 1 if option --nls-error-text has been specified.  */
extern char opt_nls_error_text;

/* Set to 1 if option --nls-help-strings has been specified.  */
extern char opt_nls_help_strings;

/* Set to 1 if option --nls-help-text has been specified.  */
extern char opt_nls_help_text;

/* Set to 1 if option --exception-name (-e) has been specified.  */
extern char opt_exception_name;

/* Set to 1 if option --namespace (-n) has been specified.  */
extern char opt_namespace;

/* Set to 1 if option --option-prefix (-O) has been specified.  */
extern char opt_option_prefix;

/* Set to 1 if option --argument-prefix (-A) has been specified.  */
extern char opt_argument_prefix;

/* Set to 1 if option --help has been specified.  */
extern char opt_help;

/* Set to 1 if option --version has been specified.  */
extern char opt_version;

/* Argument to option --struct (-s), or a null pointer if no argument.  */
extern const char *arg_struct;

/* Argument to option --function-name (-f).  */
extern const char *arg_function_name;

/* Argument to option --nls-error-text, or a null pointer if no argument.  */
extern const char *arg_nls_error_text;

/* Argument to option --nls-help-strings, or a null pointer if no argument.  */
extern const char *arg_nls_help_strings;

/* Argument to option --nls-help-text, or a null pointer if no argument.  */
extern const char *arg_nls_help_text;

/* Argument to option --exception-name (-e).  */
extern const char *arg_exception_name;

/* Argument to option --namespace (-n).  */
extern const char *arg_namespace;

/* Argument to option --option-prefix (-O).  */
extern const char *arg_option_prefix;

/* Argument to option --argument-prefix (-A).  */
extern const char *arg_argument_prefix;

/* Parse command line options.  Return index of first non-option argument,
   or -1 if an error is encountered.  */
#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
extern int parse_options (const char *program_name, int argc, char **argv) __attribute__ ((nonnull));
#else
extern int parse_options (const char *program_name, int argc, char **argv);
#endif

#endif

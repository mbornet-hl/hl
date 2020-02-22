/* ============================================================================
 * Copyright (C) 2015-2019, Martial Bornet
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   (C) Copyright Martial Bornet, 2015-2020.
 *
 *   Author       :     Martial BORNET (MB) - 3rd of January, 2015
 *
 *   Description  :     String colorization
 *
 *   File         :     cr_main.c
 *
 *   @(#)  [MB] cr_main.c Version 1.90 du 20/02/22 - 
 *
 * Sources from the original hl command are available on :
 * https://github.com/mbornet-hl/hl
 *
 *   Functions in this file :
 *   ~~~~~~~~~~~~~~~~~~~~~~~~
 *   - cr_list2argv
 *   - cr_lists2argv
 *   - cr_read_config_file
 *   - cr search_config
 *   - cr_read_config_files
 *   - cr_new_config
 *   - cr_new_arg
 *   - cr_new_args
 *   - cr_new_ptrs
 *   - cr_new_re_desc
 *   - cr_needs_arg
 *   - cr_get_config
 *   - cr_dump_ptrs
 *   - cr_dump_args
 *   - cr_getopt
 *   - cr_set_args
 *   - cr_add_to_list
 *   - cr_clear_marker_flags
 *   - cr_add_regexp
 *   - main
 *   - cr_usage
 *   - cr_display_args_list
 *   - cr_display_args
 *   - cr_display_config
 *   - cr_init_list
 *   - cr_add_config
 *   - cr_add_arg
 *   - cr_free_re
 *   - cr_marker2color
 *   - cr_set_desc
 *   - cr_set_desc_alt
 *   - cr_read_input
 *   - cr_start_color
 *   - cr_end_color
 *   - cr_init_desc
 *   - cr_same_colors
 *   - cr_disp_line
 *   - cr_get_deflt_alt_col
 * ============================================================================
 */

#if defined(HL_BACKTRACE)
#define _GNU_SOURCE
#endif    /* HL_BACKTRACE */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <glob.h>
#if defined(HL_BACKTRACE)
#include <signal.h>
#include <execinfo.h>
#endif    /* HL_BACKTRACE */

#include "cr_epri.h"

#define   X0     fprintf(stderr, "%s(%d)\n", __FILE__, __LINE__);
#define   X      if (G.debug) fprintf(stderr, "%s(%d)\n", __FILE__, __LINE__);
#define   SC     if (G.debug) fprintf(stderr, "==> %s(%d) Start color : [%c]\n", __FILE__, __LINE__, _c);
#define   NC     if (G.debug) fprintf(stderr, "==> %s(%d) No color    : [%c]\n", __FILE__, __LINE__, _c);
#define   EC     if (G.debug) fprintf(stderr, "==> %s(%d) End color\n", __FILE__, __LINE__);

#define   inline /* empty : for compilers that do not know the inline directive */

/******************************************************************************

                              CR_LIST2ARGV

******************************************************************************/
void cr_list2argv(struct cr_config *config)
{
     char                     **_argv;
     struct cr_arg             *_arg, *_old_arg;
     int                        _size;

     _size          = sizeof(char **) * (config->argc + 2);

     config->argv   = (char **) malloc(_size);
     if (config->argv == 0) {
          fprintf(stderr, cr_err_malloc, G.prgname);
          exit(1);
     }
     bzero(config->argv, _size);

     for (_argv = config->argv + 1, _arg = config->extract; _arg != 0; ) {
          *_argv++       = _arg->value;
          _old_arg       = _arg;
          _arg           = _arg->next;
          free(_old_arg);
     }

     *_argv              = 0;
     config->extract     =
     config->insert      = 0;
}

/******************************************************************************

                              CR_LISTS2ARGV

******************************************************************************/
void cr_lists2argv(struct cr_configs *configs)
{
     struct cr_config         *_config;

     for (_config = configs->extract; _config != 0; _config = _config->next) {
          cr_list2argv(_config);
     }
}

/******************************************************************************

                              CR_READ_CONFIG_FILE

******************************************************************************/
void cr_read_config_file(char *cfg_file)
{
     if (access(cfg_file, 0) != 0) {
#if 0
          fprintf(stderr, "%s: config file \"%s\" does not exist !\n",
                  G.prgname, cfg_file);
          exit(1);
#else
          return;
#endif
     }

     if ((yyin = fopen(cfg_file, "r")) == NULL) {
          fprintf(stderr, "%s: cannot open \"%s\" !\n",
                  G.prgname, cfg_file);
          perror("fopen");
          exit(1);
     }

     G.cfg_filename           = strdup(cfg_file);
     yylex();

//     cr_lists2argv(&G.configs);
}

/******************************************************************************

                              CR_SEARCH_CONFIG

******************************************************************************/
void cr_search_config(char *dir)
{
     int                       _flags = 0, _size, _i;
     glob_t                    _globbuf;
     char                     *_var_conf_glob, *_val_conf_glob, *_val, *_pattern, *_s;

     _var_conf_glob           = CR_ENV_CONF_GLOB;
     if ((_val_conf_glob = getenv(_var_conf_glob)) == 0) {
//          fprintf(stderr, "%s is undefined\n", _var_conf_glob);
          _val_conf_glob      = CR_DEFLT_CONF_GLOB;
     }
//X0
//   fprintf(stderr, "CONF_GLOB = \"%s\"\n", _val_conf_glob);

//X0
     _size                    = strlen(dir) + strlen(_val_conf_glob);
     if ((_pattern = malloc(_size)) == 0) {
          fprintf(stderr, "%s: malloc error !\n", G.prgname);
          exit(1);
     }
//X0

     for (_val = strdup(_val_conf_glob); (_s = strtok(_val, ":")) != 0; _val = 0) {
          sprintf(_pattern, "%s/%s", dir, _s);
//fprintf(stderr, "pattern = [%s]\n", _pattern);

          switch (glob(_pattern, _flags, NULL, &_globbuf)) {  
       
          case GLOB_NOSPACE:  
               fprintf(stderr, "Memoire insuffisante !\n");  
               exit(1);  
               break;  
       
          case GLOB_ABORTED:  
               fprintf(stderr, "Erreur de lecture !\n");  
               exit(1);  
               break;  
       
          case GLOB_NOMATCH:  
               fprintf(stderr, "Pas de correspondance !\n");  
               break; 
       
          default:  
               for (_i = 0; _globbuf.gl_pathv[_i] != NULL; _i++) { 
//                  printf("%5d %s\n", _i, _globbuf.gl_pathv[_i]); 
                    cr_read_config_file(_globbuf.gl_pathv[_i]);
               } 
          }
     }
}

/******************************************************************************

                              CR_READ_CONFIG_FILES

******************************************************************************/
void cr_read_config_files(void)
{
     int                       _size;
     char                     *_home, *_cfg_file;
     char                     *_var_conf = CR_ENV_CONF,
                              *_val_conf, *_val, *_s;
     struct stat               _st_buf;

     if (G.config_file_read) {
          return;
     }

     if ((_val_conf = getenv(_var_conf)) == 0) {
//          fprintf(stderr, "%s is undefined\n", _var_conf);

          if ((_home = getenv("HOME")) == 0) {
               fprintf(stderr, "%s: HOME variable undefined !\n", G.prgname);
               exit(1);
          }

          _size     = strlen(_home) + 1 + sizeof(CR_CONFIG_FILENAME);
          if ((_cfg_file = malloc(_size)) == NULL) {
               fprintf(stderr, cr_err_malloc, G.prgname);
               exit(1);
          }

          sprintf(_cfg_file, "%s/%s", _home, CR_CONFIG_FILENAME);

          cr_read_config_file(_cfg_file);
          cr_read_config_file(CR_DEFLT_CONFIG_FILE);
     }
     else {
          for (_val = strdup(_val_conf); (_s = strtok(_val, ":")) != 0; _val = 0) {
               if (stat(_s, &_st_buf) < 0) {
                    fprintf(stderr, "Cannot stat \"%s\"\n", _s);
               }
               else {
                    switch (_st_buf.st_mode & S_IFMT) {

                    case S_IFDIR:
                         fprintf(stderr, "%s : directory\n", _s);
                         cr_search_config(_s);
                         break;

                    case S_IFREG:
                         fprintf(stderr, "%s : file\n", _s);
                         cr_read_config_file(_s);
                         break;

                    default:
                         fprintf(stderr, "%s : IGNORED\n", _s);
                         break;
                    }
               }
          }
     }

     cr_lists2argv(&G.configs);

     G.config_file_read  = TRUE;
}

/******************************************************************************

                         CR_NEW_CONFIG

******************************************************************************/
CR_NEW(config)

/******************************************************************************

                         CR_NEW_ARG

******************************************************************************/
CR_NEW(arg)

/******************************************************************************

                         CR_NEW_ARGS

******************************************************************************/
CR_NEW(args)

/******************************************************************************

                         CR_NEW_PTRS

******************************************************************************/
CR_NEW(ptrs)

/******************************************************************************

                         CR_NEW_RE_DESC

******************************************************************************/
CR_NEW(re_desc)

/******************************************************************************

                         CR_NEW_COLOR

******************************************************************************/
CR_NEW(color)

/******************************************************************************

                         CR_CREATE_COLOR

******************************************************************************/
struct cr_color *cr_create_color(int color, int intensity)
{
     // XXX : for Alternate option
     struct cr_color     *_color;

     _color              = cr_new_color();
     _color->col_num     = color;
     _color->intensity   = intensity;
     _color->out         = stdout;      // XXX : TO CHANGE !!!

     return _color;
}

/******************************************************************************

                         CR_DUMP_COLOR

******************************************************************************/
void cr_dump_color(struct cr_color *col)
{
     printf("COLOR %p : color = %3d), intensity = %d\n",
            col, col->col_num, col->intensity);
}

/******************************************************************************

                         CR_NEEDS_ARG

******************************************************************************/
bool cr_needs_arg(char opt, struct cr_args *args)
{
     int                  _i;

     for (_i = 0; args->opts[_i] != 0; _i++) {
          if (args->opts[_i] != opt) continue;
          if (args->opts[_i + 1] == '!') {
               return TRUE;
          }
          else {
               break;
          }
     }

     return FALSE;
}

/******************************************************************************

                         CR_SPECIAL_OPT

******************************************************************************/
bool cr_special_opt(char opt, struct cr_args *args)
{
     // for Alternate option
     int                  _i;

     for (_i = 0; args->opts[_i] != 0; _i++) {
          if (args->opts[_i] != opt) continue;
          if (args->opts[_i + 1] == '{') {
               return TRUE;
          }
          else {
               break;
          }
     }

     return FALSE;
}

/******************************************************************************

                              CR_GET_CONFIG

******************************************************************************/
struct cr_config *cr_get_config(char *config_name, struct cr_args *args)
{
     struct cr_config         *_config;

     if (!G.config_file_read) {
          cr_read_config_files();
     }

     for (_config = args->configs->extract; _config != 0;
          _config = _config->next) {
          if (!strcmp(config_name, _config->name)) {
               break;
          }
     }

     return _config;
}

/******************************************************************************

                              CR_DUMP_PTRS

******************************************************************************/
void cr_dump_ptrs(struct cr_ptrs *ptrs)
{
     printf("PTRS :\n");
     printf("     curr_arg  = \"%s\"\n", ptrs->curr_arg);
     printf("     curr_idx  = %d\n", ptrs->curr_idx);
     printf("     next_arg  = \"%s\"\n", ptrs->next_arg);
     printf("     prev      = %p\n", ptrs->prev);
     printf("     config    = %p\n", ptrs->config);
}

/******************************************************************************

                              CR_DUMP_ARGS

******************************************************************************/
void cr_dump_args(struct cr_args *args)
{
     printf("ARGS :\n");
     printf("    opts      = \"%s\"\n", args->opts);
     printf("    optarg    = \"%s\"\n", args->optarg);
     printf("    curr_ptrs = %p\n", args->curr_ptrs);

     cr_dump_ptrs(args->curr_ptrs);
}

/******************************************************************************

                         CR_IS_SELECTOR

******************************************************************************/
int cr_is_selector(char c)
{
     int             _retcode;

     if (strchr(G.selector_string, (int) c) != NULL) {
          _retcode  = TRUE;
     }
     else {
          _retcode  = FALSE;
     }

     return _retcode;
}

/******************************************************************************

                         CR_IS_INTENSITY

******************************************************************************/
int cr_is_intensity(char c)
{
     int             _retcode;

     if (strchr(G.intensity_string, (int) c) != NULL) {
          _retcode  = TRUE;
     }
     else {
          _retcode  = FALSE;
     }

     return _retcode;
}

/******************************************************************************

                         CR_IS_A_COLOR

******************************************************************************/
int cr_is_a_color(char c)
{
     int             _retcode;

     if (strchr(G.color_string, (int) c) != NULL) {
          _retcode  = TRUE;
     }
     else {
          _retcode  = FALSE;
     }

     return _retcode;
}

/******************************************************************************

                         CR_DECODE_COLOR

******************************************************************************/
struct cr_color *cr_decode_color(char c, int intensity)
{
     struct cr_color          *_color;

     switch (c) {

     case 'r':
          _color    = cr_create_color(CR_RED, intensity);
          break;

     case 'g':
          _color    = cr_create_color(CR_GREEN, intensity);
          break;

     case 'y':
          _color    = cr_create_color(CR_YELLOW, intensity);
          break;

     case 'b':
          _color    = cr_create_color(CR_BLUE, intensity);
          break;

     case 'm':
          _color    = cr_create_color(CR_MAGENTA, intensity);
          break;

     case 'c':
          _color    = cr_create_color(CR_CYAN, intensity);
          break;

     case 'w':
          _color    = cr_create_color(CR_WHITE, intensity);
          break;

     case 'R':
          _color    = cr_create_color(CR_RED_REV, intensity);
          break;

     case 'G':
          _color    = cr_create_color(CR_GREEN_REV, intensity);
          break;

     case 'Y':
          _color    = cr_create_color(CR_YELLOW_REV, intensity);
          break;

     case 'B':
          _color    = cr_create_color(CR_BLUE_REV, intensity);
          break;

     case 'M':
          _color    = cr_create_color(CR_MAGENTA_REV, intensity);
          break;

     case 'C':
          _color    = cr_create_color(CR_CYAN_REV, intensity);
          break;

     case 'W':
          _color    = cr_create_color(CR_WHITE_REV, intensity);
          break;

     case 'n':
          _color    = cr_create_color(CR_NO_COLOR, intensity);
          break;

     default:
          /* Syntax error
             ~~~~~~~~~~~~ */
          fprintf(stderr, cr_err_syntax, G.prgname, __func__, __FILE__, __LINE__);
          exit(1);
          break;
     }

     return _color;
}

/******************************************************************************

                              CR_DECODE_ALTERNATE

******************************************************************************/
struct cr_re_desc *cr_decode_alternate(struct cr_args *args)
{
     // for Alternate option
     int                       _state, _curr_col_idx, _selector, _lg, _size;
     char                      _c, _next_char, *_regexp;
     struct cr_ptrs           *_ptrs;
     struct cr_color         **_alt_colors;
     struct cr_re_desc        *_re;
     int                       _error, _i;
     char                      _errbuf[256], *_p;

//X
     _re                      = cr_new_re_desc();
     _re->cflags              = G.cflags;
     _re->col.col_num         = 0;
     _re->col.intensity       = 0;
     _re->col.out             = G.out;
     _re->max_sub             = 1;
     _re->alt_idx             = 0;
     _re->idx_regex_select    = 0;
     _re->matching_str        = NULL;
     _re->change_on_diff      = TRUE;
     G.out                    = stdout;

     _state                   = CR_STATE_INITIAL;
     _curr_col_idx            = 0;
     _selector                = 0;

     _regexp                  = NULL;

     if ((_ptrs = args->curr_ptrs) == NULL) {
          _lg                           = 3;
     }
     else {
          _lg                      = strlen(_ptrs->curr_arg);
          if (_lg < 3) {
               _lg                      = 3;
          }
     }

     /* Allocate memory for the color descriptors
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _size                    = (_lg + 1) * sizeof(struct cr_color **);
     if ((_alt_colors = (struct cr_color **) malloc(_size)) == NULL) {
          fprintf(stderr, cr_err_malloc, G.prgname);
          exit(1);
     }
     _re->alt_cols            = _alt_colors;

     for (_i = 0; _i < (_lg + 1); _i++) {
          _alt_colors[_i]          = NULL;
     }

     /* Decode options
        ~~~~~~~~~~~~~~ */
     for (_c = 0 ; ; ) {
          if (!(_ptrs = args->curr_ptrs)) {
               /* No more argument to treat
                  ~~~~~~~~~~~~~~~~~~~~~~~~~ */
               CR_DEBUG("NO MORE ARGS.\n");
               break;
          }

          _c        = _ptrs->curr_arg[_ptrs->curr_idx];
          CR_DEBUG("==> OPTION : '%c'\n", _c);
          _ptrs->curr_idx++;
          _next_char     = _ptrs->curr_arg[_ptrs->curr_idx];
          args->optarg      = 0;

          if (_next_char == 0) {
               CR_DEBUG("    No more 1 letter option\n");
               CR_DEBUG("    Current arg = %p \"%s\"\n", _ptrs->curr_arg, _ptrs->curr_arg);
               if (*(_ptrs->curr_argv + 1) == 0) {
                    /* No regexp for this option
                       ~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    CR_DEBUG("No regexp for this option => use default one\n");
                    _regexp             = CR_DEFLT_ALT_REGEXP;
                    args->curr_ptrs     = _ptrs->prev;
                    free(_ptrs);
               }
               else {
                    _ptrs->curr_argv++;
                    _ptrs->curr_arg     = *_ptrs->curr_argv;
                    _ptrs->curr_idx     = 0;
                    CR_DEBUG("  Next arg  = %p \"%s\"\n", _ptrs->curr_arg, _ptrs->curr_arg);
                    if (_ptrs->curr_arg[0] != '-') {
                         /* Regexp found !
                            ~~~~~~~~~~~~~~ */
                         _regexp             = _ptrs->curr_arg;
                         _ptrs->curr_argv++;
                         _ptrs->curr_arg     = *_ptrs->curr_argv;
//                         args->curr_ptrs     = _ptrs;  // XXX !?!?!?!?
//cr_dump_args(args);
                    }
                    else {
                         /* No regexp : use default one
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _regexp             = CR_DEFLT_ALT_REGEXP;
                    }
               }
          }

          switch (_state) {

          case CR_STATE_INITIAL:
               if (cr_is_selector(_c)) {
                    _selector      = _c - '0';
                    _state         = CR_STATE_W_SEPARATOR;
               }
               else if (_c == ',') {
                    _selector      = 0;
                    _state         = CR_STATE_W_INTENSITY;
               }
               else {
                    /* Syntax error
                       ~~~~~~~~~~~~ */
                    fprintf(stderr, cr_err_syntax, G.prgname, __func__, __FILE__, __LINE__);
                    exit(1);
                    break;
               }
               break;

          case CR_STATE_W_SEPARATOR:
               if (_c == ',') {
                    _state         = CR_STATE_W_INTENSITY;
               }
               else {
                    /* Syntax error
                       ~~~~~~~~~~~~ */
                    fprintf(stderr, cr_err_syntax, G.prgname, __func__, __FILE__, __LINE__);
                    exit(1);
                    break;
               }
               break;

          case CR_STATE_W_INTENSITY:
               if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    _state         = CR_STATE_W_COLOR;
               }
               else if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
//cr_dump_color(_alt_colors[_curr_col_idx - 1]);
                    _state         = CR_STATE_W_END;
               }
               else {
                    fprintf(stderr, cr_err_syntax, G.prgname, __func__, __FILE__, __LINE__);
                    exit(1);
               }
               break;

          case CR_STATE_W_COLOR:
               if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
//cr_dump_color(_alt_colors[_curr_col_idx - 1]);
               }
               _state         = CR_STATE_W_END;
               break;

          case CR_STATE_W_END:
               if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    _state         = CR_STATE_W_COLOR;
               }
               else if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
//cr_dump_color(_alt_colors[_curr_col_idx - 1]);
                    _state         = CR_STATE_W_END;
               }
               else {
                    fprintf(stderr, cr_err_syntax, G.prgname, __func__, __FILE__, __LINE__);
                    exit(1);
               }
               break;

          default:
               fprintf(stderr, cr_err_syntax, G.prgname, __func__, __FILE__, __LINE__);
               exit(1);
               break;
          }

          if (_regexp != NULL) {
               /* Regex initialized : get out of the loop
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               break;
          }
     }

//printf("END OF LOOP : regexp initialized : [%s]\n", _regexp);
//cr_dump_args(args);
     _alt_colors[_curr_col_idx++]  = NULL;
//printf("%s %s(%d) : selector = %d\n", __func__, __FILE__, __LINE__, _selector);

     if (_alt_colors[0] == NULL) {
          _alt_colors[0]           = G.deflt_alt_col_1;
     }
     if (_alt_colors[1] == NULL) {
          _alt_colors[1]           = G.deflt_alt_col_2;
     }
     if (_regexp == NULL) {
          _regexp                  = CR_DEFLT_ALT_REGEXP;
     }

     /* Count number of possible sub strings
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_p = _regexp; (*_p); _p++) {
          if (*_p == '(') {
               _re->max_sub++;
          }
     }

     _selector++;   // '0' => regexp number 1
     _re->idx_regex_select    = _selector;
     _re->regex[0]            = _regexp;
     _re->regex[1]            = NULL;

     if (G.debug) {
          struct cr_color     *_color;

          printf("Selector  = %d\n",   _re->idx_regex_select);
          printf("regex[0]  = [%s]\n", _re->regex[0]);
          printf("regex[1]  = %p\n",   _re->regex[1]);
          for (_i = 0; ; _i++) {
               _color              = _re->alt_cols[_i];
               if (_color == NULL) {
                    break;
               }
               printf("Color[%d]  = (%d, %d)  (%p)\n", _i, _color->intensity, _color->col_num, _color);
          }
     }

     if ((_error = regcomp(&_re->reg[0], _regexp, _re->cflags)) != 0) {
          (void) regerror(_error, &_re->reg[0], _errbuf, sizeof(_errbuf));
          fprintf(stderr, "%s: regcomp error for \"%s\" : %s\n",
                  G.prgname, _regexp, _errbuf);
          exit(1);
     }

     return _re;
}

/******************************************************************************

                              CR_GETOPT

******************************************************************************/
int cr_getopt(struct cr_args *args)
{
     /* Pointers always point to the arg or option to treat
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     char                 _c, *_arg, *_config_name, _next_char;
     struct cr_ptrs      *_ptrs, *_new_ptrs;
     struct cr_config    *_config;

     for ( ; ; ) {
          if (!(_ptrs = args->curr_ptrs)) {
               /* No more argument to treat
                  ~~~~~~~~~~~~~~~~~~~~~~~~~ */
               CR_DEBUG("NO MORE ARGS.\n");
               return -1;
          }

          if (*(_ptrs->curr_argv) == 0) {
               /* No more argument for this level
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               CR_DEBUG("No more args for this level\n");
               args->curr_ptrs     = _ptrs->prev;
               free(_ptrs);
               continue;
          }

          CR_DEBUG("Current arg = %p \"%s\" idx = %d\n",
                   _ptrs->curr_arg, _ptrs->curr_arg, _ptrs->curr_idx);

          if (_ptrs->curr_idx == 0) {
               /* Treatment of a new argument
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (_ptrs->curr_arg[0] != '-') {
                    fprintf(stderr, "%s: argument with no associated option (\"%s\") ! \n",
                            G.prgname, _ptrs->curr_arg);
                    cr_dump_args(args);
                    exit(1);
               }
               else {
                    /* Continuation of the treatment of an argument
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _ptrs->curr_idx++;
                    if (_ptrs->curr_arg[1] != '-') {
                         /* Continuation of the one letter options
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         break;
                    }
                    else {
                         /* Configuration name
                            ~~~~~~~~~~~~~~~~~~ */
                         _config_name        = _ptrs->curr_arg + 2;
                         CR_DEBUG("==> CONFIG : \"%s\"\n", _config_name);
                         _ptrs->curr_argv++;
                         _ptrs->curr_idx     = 0;
                         _ptrs->curr_arg     = *_ptrs->curr_argv;
                         CR_DEBUG("Apres incrementation pointeur CONFIG :\n");
                         CR_DEBUG("Current arg = %p \"%s\"\n", _ptrs->curr_arg, _ptrs->curr_arg);

                         _config             = cr_get_config(_config_name, args);
                         if (_config == 0) {
                                  fprintf(stderr, "%s: undefined configuration (%s) !\n",
                                      G.prgname, _config_name);
                              exit(1);
                         }

                         /* Detection of recursive loop
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         if (_config->visited) {
                              fprintf(stderr, "%s: configuration loop for \"%s\" !\n",
                                      G.prgname, _config->name);
                              exit(1);
                         }
                         _config->visited    = TRUE;

                         _new_ptrs                = cr_new_ptrs();
                         _new_ptrs->prev          = args->curr_ptrs;
                         _new_ptrs->argc          = _config->argc;
                         _new_ptrs->argv          = _config->argv;
                         _new_ptrs->curr_argv     = _new_ptrs->argv + 1;
                         _new_ptrs->curr_arg      = _new_ptrs->argv[1];
                         _new_ptrs->config        = _config;          // Usefullness ???
                         args->curr_ptrs          = _new_ptrs;

                         continue;
                    }
               }
          }
          else {
               break;
          }
     }

     /* Options
        ~~~~~~~ */
     _c        = _ptrs->curr_arg[_ptrs->curr_idx];
     CR_DEBUG("==> OPTION : '%c'\n", _c);
     _ptrs->curr_idx++;
     _next_char     = _ptrs->curr_arg[_ptrs->curr_idx];
     if (cr_needs_arg(_c, args)) {
          CR_DEBUG("    (argument needed)\n");
          _arg      = *(_ptrs->curr_argv + 1);
          if (_arg == 0 || _arg[0] == '-' || _next_char != 0) {
               fprintf(stderr, "%s: missing argument for \"-%c\" !\n",
                       G.prgname, _c);
               cr_dump_args(args);
               exit(1);
          }

          args->optarg      = _arg;
          CR_DEBUG("    OPTARG = \"%s\"\n", args->optarg);
          _ptrs->curr_argv++;
          _ptrs->curr_arg     = *_ptrs->curr_argv;
     }
     else if (cr_special_opt(_c, args)) {
          args->special_opt        = TRUE;
     }
     else {
          CR_DEBUG("    no argument needed.\n");
          args->optarg      = 0;
     }

     if (_next_char == 0) {
          CR_DEBUG("    No more 1 letter option\n");
          CR_DEBUG("    Current arg = %p \"%s\"\n", _ptrs->curr_arg, _ptrs->curr_arg);
          if (*(_ptrs->curr_argv + 1) == 0) {
               /* No more argument for this level
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               CR_DEBUG("No more args for this level\n");
               args->curr_ptrs     = _ptrs->prev;
               free(_ptrs);
          }
          else {
               _ptrs->curr_argv++;
               _ptrs->curr_arg     = *_ptrs->curr_argv;
               _ptrs->curr_idx     = 0;
               CR_DEBUG("  Next arg  = %p \"%s\"\n", _ptrs->curr_arg, _ptrs->curr_arg);
          }
     }

     return _c;
}

/******************************************************************************

                              CR_SET_ARGS

******************************************************************************/
struct cr_args *cr_set_args(int argc, char **argv, char *opts,
                            struct cr_configs *configs)
{
     struct cr_args           *_args;
     struct cr_ptrs           *_ptrs;

     _args               = cr_new_args();
     _ptrs               = cr_new_ptrs();

     _ptrs->argc         = argc;
     _ptrs->argv         = argv + 1;
     _ptrs->curr_argv    = _ptrs->argv;
     _ptrs->curr_arg     = argv[1];
     _ptrs->curr_idx     = 0;
     _ptrs->next_arg     = NULL;
     _ptrs->prev         = NULL;
     _ptrs->config       = NULL;

     _args->opts         = opts;
     _args->optarg       = NULL;
     _args->curr_ptrs    = _ptrs;
     _args->configs      = configs;

     return _args;
}

/******************************************************************************

                         CR_ADD_TO_LIST

******************************************************************************/
void cr_add_to_list(struct cr_re_desc *re)
{
     if (G.extract_RE == 0) {
          G.extract_RE   =
          G.insert_RE    = re;
     }
     else {
          G.insert_RE->next   = re;
          G.insert_RE         = re;
     }
}

/******************************************************************************

                         CR_CLEAR_MARKER_FLAGS

******************************************************************************/
inline void cr_clear_marker_flags(void)
{
     G.begin_specified        = FALSE;
     G.end_specified          = FALSE;
}

/******************************************************************************

                         CR_ADD_REGEXP

******************************************************************************/
void cr_add_regexp(int color, char *regexp)
{
     struct cr_re_desc        *_re;
     int                       _error;
     char                      _errbuf[256], *_p;

     if (!G.end_specified) {
          _re                      = cr_new_re_desc();
     
          _re->regex[0]            = regexp;
          _re->cflags              = G.cflags;
          _re->col.col_num         = color;
          _re->col.intensity       = G.intensity;
          _re->col.out             = G.out;
          _re->max_sub             = 1;
          G.out                    = stdout;

          /* Count number of possible sub strings
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          for (_p = regexp; (*_p); _p++) {
               if (*_p == '(') {
                    _re->max_sub++;
               }
          }

          if ((_error = regcomp(&_re->reg[0], regexp, _re->cflags)) != 0) {
               (void) regerror(_error, &_re->reg[0], _errbuf, sizeof(_errbuf));
               fprintf(stderr, "%s: regcomp error for \"%s\" : %s\n",
                       G.prgname, regexp, _errbuf);
               exit(1);
          }

          cr_add_to_list(_re);
          G.last_RE                = _re;
          G.last_color             = color;
     }
     else {
          _re                      = G.last_RE;
          _re->regex[1]            = regexp;

          if (_re->regex[1]) {
               if (!strcmp(_re->regex[0], _re->regex[1])) {
                    _re->begin_is_end   = TRUE;
               }
               if (regcomp(&_re->reg[1], regexp, _re->cflags) != 0) {
                    fprintf(stderr, "%s: regcomp error for \"%s\" !\n", G.prgname, regexp);
                    exit(1);
               }
          }

          cr_clear_marker_flags();
          G.last_RE                = 0;
          G.last_color             = 0;
     }
}

/******************************************************************************

                         CR_GET_DEFLT_ALT_COL

******************************************************************************/
struct cr_color *cr_get_deflt_alt_col(char *env_var_name, int deflt_intensity,
                                      int deflt_color)
{
     struct cr_color          *_color = NULL;
     char                     *_env_val, _c;
     int                       _intensity, _lg;

     if ((_env_val = getenv(env_var_name)) == 0) {
          /* Environment variable undefined :
           * use default values
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          _color         = cr_decode_color(deflt_color, deflt_intensity);
     }
     else {
          /* Decode the color string
             ~~~~~~~~~~~~~~~~~~~~~~~ */
          _lg            = strlen(_env_val);
          if (_lg == 0 || _lg > 2) {
               /* Invalid color specifier : use default values
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               fprintf(stderr,
                       "Invalid color specifier %s=\"%s\", using default\n",
                       env_var_name, _env_val);
               _color         = cr_decode_color(deflt_color, deflt_intensity);
          }
          else {
               _c             = _env_val[0];
               if (cr_is_intensity(_c)) {
                    /* Intensity : OK
                       ~~~~~~~~~~~~~~ */
                    _intensity     = _c - '0';
                    _c             = _env_val[1];
                    if (cr_is_a_color(_c)) {
                         /* Intensity : OK, color : OK
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _color         = cr_decode_color(_c, _intensity);
                    }
                    else {
                         /* Intensity : OK, color : default
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _color         = cr_decode_color(deflt_color,
                                                          _intensity);
                    }
               }
               else {
                    /* Intensity KO : maybe intensity is absent
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    if (cr_is_a_color(_c)) {
                         /* Intensity : default, color : OK
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _color         = cr_decode_color(_c, deflt_intensity);
                    }
                    else {
                         /* No intensity, invalid color :
                          * use default values
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         fprintf(stderr,
                                 "Invalid color specifier %s=\"%s\", using default\n",
                                 env_var_name, _env_val);
                         _color         = cr_decode_color(deflt_color,
                                                          deflt_intensity);
                    }
               }
          }
     }

     return _color;
}

/******************************************************************************

                         CR_INIT_DEFLT_ALT_COL

******************************************************************************/
void cr_init_deflt_alt_col()
{
     G.deflt_alt_col_1   = cr_get_deflt_alt_col(CR_ENV_DEFLT_ALTERNATE_1,
                                      CR_DEFLT_ALT_INTENSITY_1,
                                      CR_DEFLT_ALT_COLOR_1);

     G.deflt_alt_col_2   = cr_get_deflt_alt_col(CR_ENV_DEFLT_ALTERNATE_2,
                                      CR_DEFLT_ALT_INTENSITY_2,
                                      CR_DEFLT_ALT_COLOR_2);
}

#if defined(HL_BACKTRACE)
/******************************************************************************

                              PRINT_TRACE

******************************************************************************/
void print_trace(int signum)
{
#define   MAX_FUNCTS               (1600)
     void            *_array[MAX_FUNCTS];
     size_t           _num_addr, _max_addr;
     char           **_strings;
     int              _i;

     printf("%s: received signal %d.\n", __func__, signum);

     _max_addr      = sizeof(_array)/sizeof(_array[0]);
     printf("Max addresses number = %u\n", (unsigned int) _max_addr);

     _num_addr      = backtrace(_array, _max_addr);
     printf("size = %u\n", (unsigned int) _num_addr);

     _strings       = backtrace_symbols(_array, _num_addr);
     printf("%u currently active functions :\n", (unsigned int) _num_addr);

     for (_i = 0; _i < _num_addr; _i++) {
          printf("===> %s\n", _strings[_i]);
     }

     free(_strings);

     exit(1);
}
#endif    /* HL_BACKTRACE */

/******************************************************************************

                              MAIN

******************************************************************************/
int main(int argc, char *argv[])
{
     int                       _opt, _i, _j, _lg, _argc;
     struct cr_args           *_args;
     struct cr_re_desc        *_re;
     char                     *_env_var_name, *_env_deflt, *_deflt_color_opt,
                              **_argv, *_argv_deflt[4];

#if defined(HL_BACKTRACE)
     sighandler_t              _previous_handler;
     
     if ((_previous_handler = signal(SIGSEGV, print_trace)) == SIG_ERR) {
          fprintf(stderr, "%s: signal() returned an error !\n", argv[0]);
          exit(1);
     }

#endif    /* HL_BACKTRACE */
     G.prgname           = argv[0];
     G.selector_string   = CR_SELECTOR_STRING;
     G.color_string      = CR_COLORS_STRING;
     G.intensity_string  = CR_INTENSITY_STRING;
     G.out               = stdout;

     switch (argc) {

     case 1:
          cr_usage(FALSE);
          break;

     case 2:
          if (argv[1][0] != '-') {
               /* Only one argument (the regular expression)
                * => get the default color in the environment variable
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _env_var_name  = CR_ENV_DEFLT;
               if ((_env_deflt = getenv(_env_var_name)) == 0) {
#if 0
                    fprintf(stderr, "%s: minimal syntax but \"%s\" is undefined !\n",
                            G.prgname, _env_var_name);
                    exit(1);
#else
                    _env_deflt     = CR_DEFLT_COLOR;
#endif
               }
               _lg       = strlen(_env_deflt);
               if ((_deflt_color_opt = malloc(_lg + 2)) == 0) {
                    fprintf(stderr, cr_err_malloc, G.prgname);
                    exit(1);
               }
               sprintf(_deflt_color_opt, "-%s", _env_deflt);

               _argv_deflt[0] = argv[0];
               _argv_deflt[1] = _deflt_color_opt;
               _argv_deflt[2] = argv[1];
               _argv_deflt[3] = NULL;

               _argc          = 3;
               _argv          = _argv_deflt;
          }
          else {
               _argc          = argc;
               _argv          = argv;
          }
          break;

     default:
          _argc          = argc;
          _argv          = argv;
          break;
     }

     cr_init_list();
     G.intensity    = CR_DEFLT_INTENSITY;

     cr_init_deflt_alt_col();

     cr_clear_marker_flags();

     /* Decoding of arguments
        ~~~~~~~~~~~~~~~~~~~~~ */
     _args               = cr_set_args(_argc, _argv,
                                       "hHuVvEr!g!y!b!m!c!w!R!G!Y!B!M!C!W!n!DLdei1234%.!A{I{",
                                       &G.configs);
     while ((_opt = cr_getopt(_args)) != -1) {
          switch (_opt) {
          case 'h':
               cr_usage(FALSE);
               break;

          case 'H':
               cr_read_config_files();
               cr_usage(TRUE);
               break;

          case 'E':
               G.out          = stderr;
               break;

          case 'd' :
               G.debug        = TRUE;
               break;

          case 'D' :
               G.disp_regex   = TRUE;
               break;

          case 'L' :
               G.disp_lex     = TRUE;
               break;

          case 'r':
               cr_add_regexp(CR_RED, _args->optarg);
               break;

          case 'g':
               cr_add_regexp(CR_GREEN, _args->optarg);
               break;

          case 'y':
               cr_add_regexp(CR_YELLOW, _args->optarg);
               break;

          case 'b':
               cr_add_regexp(CR_BLUE, _args->optarg);
               break;

          case 'm':
               cr_add_regexp(CR_MAGENTA, _args->optarg);
               break;

          case 'c':
               cr_add_regexp(CR_CYAN, _args->optarg);
               break;

          case 'w':
               cr_add_regexp(CR_WHITE, _args->optarg);
               break;

          case 'R':
               cr_add_regexp(CR_RED_REV, _args->optarg);
               break;

          case 'G':
               cr_add_regexp(CR_GREEN_REV, _args->optarg);
               break;

          case 'Y':
               cr_add_regexp(CR_YELLOW_REV, _args->optarg);
               break;

          case 'B':
               cr_add_regexp(CR_BLUE_REV, _args->optarg);
               break;

          case 'M':
               cr_add_regexp(CR_MAGENTA_REV, _args->optarg);
               break;

          case 'C':
               cr_add_regexp(CR_CYAN_REV, _args->optarg);
               break;

          case 'W':
               cr_add_regexp(CR_WHITE_REV, _args->optarg);
               break;

          case 'n':
               cr_add_regexp(CR_NO_COLOR, _args->optarg);
               break;

          case 'e':
               G.cflags  |= REG_EXTENDED;
               break;

          case 'i':
               G.cflags  |= REG_ICASE;
               break;

          case 'u':
               setvbuf(stdout, (char *) 0, _IONBF, 0);
               break;

          case 'v':
               G.verbose++;
               break;

          case 'V':
               fprintf(stderr, "%s: version %s\n", G.prgname, "1.90");
               exit(1);
               break;

          case '1':
          case '2':
          case '3':
          case '4':
               G.intensity    = _opt - '0';
               break;

          case '%':
               /* Begin marker
                  ~~~~~~~~~~~~ */
               if (G.begin_specified) {
                    fprintf(stderr, "%s: begin marker without end marker !\n",
                            G.prgname);
                    exit(1);
               }
               G.begin_specified    = TRUE;
               break;

          case '.':
               /* End marker
                  ~~~~~~~~~~ */
               if (!G.begin_specified) {
                    fprintf(stderr, "%s: end marker without begin marker !\n",
                            G.prgname);
                    exit(1);
               }
               G.end_specified      = TRUE;
               cr_add_regexp(G.last_color, _args->optarg);
               break;

          case 'A':
               G.cflags  |= REG_EXTENDED;
               if (_args->special_opt) {
                    _re                 = cr_decode_alternate(_args);
                    _re->change_on_diff = TRUE;
                    cr_add_to_list(_re);
               }
               break;

          case 'I':
               G.cflags  |= REG_EXTENDED;
               if (_args->special_opt) {
                    _re                 = cr_decode_alternate(_args);
                    _re->change_on_diff = FALSE;
                    cr_add_to_list(_re);
               }
               break;

          default:
               fprintf(stderr, "%s: unknown option '%c' !\n", G.prgname, _opt);
               cr_usage(FALSE);
               break;
          }
     }

     if (G.disp_regex) {
          for (_i = 0, _re = G.extract_RE; _re != NULL; _re = _re->next) {
               if (_re->alt_cols == NULL) {
                    printf("[%2d] ", ++_i);
                    cr_start_color(&_re->col);
                    printf("%s", _re->regex[0]);
                    cr_end_color(&_re->col);
                    printf("\n");
                    if (_re->regex[1]) {
                         printf("     => ");
                         cr_start_color(&_re->col);
                         printf("%s", _re->regex[1]);
                         cr_end_color(&_re->col);
                         printf("\n");
                    }
               }
               else {
                    struct cr_color     *_color;

                    _i++;
                    for (_j = 0; ; _j++) {
                         _color              = _re->alt_cols[_j];
                         if (_color == NULL) {
                              break;
                         }

                         printf("[%2d.%d] ", _i, _j);
                         cr_start_color(_re->alt_cols[_j]);
                         printf("%s", _re->regex[0]);
                         cr_end_color(_re->alt_cols[_j]);
                         printf("\n");
                    }
               }
          }
     }

     cr_read_input();

     cr_free_RE();
     return 0;
}

/******************************************************************************

                         CR_USAGE

******************************************************************************/
void cr_usage(bool disp_config)
{
     char                     *_env_var,           *_env_val,
                              *_env_var1,          *_env_var2,
                              *_env_val1,          *_env_val2,
                              *_env_var_conf,      *_env_val_conf,
                              *_env_var_conf_glob, *_env_val_conf_glob,
                              *_msg,               *_undefined,
						 _deflt_alt_1[4],	  _deflt_alt_2[4],
						 _deflt_conf[128];

     fprintf(stderr, "%s: version %s\n", G.prgname, "1.90");
     fprintf(stderr, "Usage: %s [-h|-H|-V|-[[%%.]eiuvdDEL1234][-[rgybmcwRGYBMCWnAI] regexp ...][--config_name ...] ]\n",
             G.prgname);
     fprintf(stderr, "  -h  : help\n");
     fprintf(stderr, "  -H  : help + configuration names\n");
     fprintf(stderr, "  -V  : version\n");
     fprintf(stderr, "  -v  : verbose\n");
     fprintf(stderr, "  -u  : do not bufferize output on stdout\n");
     fprintf(stderr, "  -e  : extended regular expressions\n");
     fprintf(stderr, "  -i  : ignore case\n");
     fprintf(stderr, "  -E  : print on stderr\n");
     fprintf(stderr, "  -r  : red\n");
     fprintf(stderr, "  -g  : green\n");
     fprintf(stderr, "  -y  : yellow\n");
     fprintf(stderr, "  -b  : blue\n");
     fprintf(stderr, "  -m  : magenta\n");
     fprintf(stderr, "  -c  : cyan\n");
     fprintf(stderr, "  -w  : white\n");
     fprintf(stderr, "  -R  : red     (reverse video)\n");
     fprintf(stderr, "  -G  : green   (reverse video)\n");
     fprintf(stderr, "  -Y  : yellow  (reverse video)\n");
     fprintf(stderr, "  -B  : blue    (reverse video)\n");
     fprintf(stderr, "  -M  : magenta (reverse video)\n");
     fprintf(stderr, "  -C  : cyan    (reverse video)\n");
     fprintf(stderr, "  -W  : white   (reverse video)\n");
     fprintf(stderr, "  -n  : never colorize\n");
     fprintf(stderr, "  -%%c : specifies the beginning of a range colorized in color 'c'\n");
     fprintf(stderr, "  -.  : specifies the end of the previous range\n");
     fprintf(stderr, "  -d  : debug\n");
     fprintf(stderr, "  -D  : display regular expressions\n");
     fprintf(stderr, "  -L  : lex debug\n");
     fprintf(stderr, "  -1  : color brightness (half-bright)\n");
     fprintf(stderr, "  -2  : color brightness (normal : default)\n");
     fprintf(stderr, "  -3  : color brightness (bright)\n");
     fprintf(stderr, "  -4  : color brightness (underscore)\n");
     fprintf(stderr, "  -A  : alternate colors when string matched by selection regex changes\n");
     fprintf(stderr, "  -I  : alternate colors when string matched by selection regex does not change\n");
     fprintf(stderr, "        Syntax for alternate options : -{A|I}[[s],c1c2...cn]\n");
     fprintf(stderr, "         where s is a number from 0 to 9 indicating the selection regexp number,\n");
     fprintf(stderr, "         and c1, c2, ... cn are color specifiers to use\n");
     fprintf(stderr, "        Alternate colors implies extended regular expressions (-e)\n");

     _env_var            = CR_ENV_DEFLT;
     _env_var1           = CR_ENV_DEFLT_ALTERNATE_1;
     _env_var2           = CR_ENV_DEFLT_ALTERNATE_2;
     _env_var_conf       = CR_ENV_CONF;
     _env_var_conf_glob  = CR_ENV_CONF_GLOB;
     _undefined          = "Environment variable %s is undefined. Default value = \"%s\".\n";
     _msg                = "Environment variable %-14s = \"%s\"\n";

     if ((_env_val = getenv(_env_var)) == NULL) {
          fprintf(stderr, _undefined, _env_var, CR_DEFLT_COLOR);
     }
     else {
          fprintf(stderr, _msg, _env_var, _env_val);
     }
     if ((_env_val1 = getenv(_env_var1)) == NULL) {
		sprintf(_deflt_alt_1, "%d%c", CR_DEFLT_ALT_INTENSITY_1, CR_DEFLT_ALT_COLOR_1);
          fprintf(stderr, _undefined, _env_var1, _deflt_alt_1);
     }
     else {
          fprintf(stderr, _msg, _env_var1, _env_val1);
     }
     if ((_env_val2 = getenv(_env_var2)) == NULL) {
		sprintf(_deflt_alt_2, "%d%c", CR_DEFLT_ALT_INTENSITY_2, CR_DEFLT_ALT_COLOR_2);
          fprintf(stderr, _undefined, _env_var2, _deflt_alt_2);
     }
     else {
          fprintf(stderr, _msg, _env_var2, _env_val2);
     }
     if ((_env_val_conf = getenv(_env_var_conf)) == NULL) {
		sprintf(_deflt_conf, "~/%s:%s", CR_CONFIG_FILENAME, CR_DEFLT_CONFIG_FILE);
          fprintf(stderr, _undefined, _env_var_conf, _deflt_conf);
     }
     else {
          fprintf(stderr, _msg, _env_var_conf, _env_val_conf);
     }
     if ((_env_val_conf_glob = getenv(_env_var_conf_glob)) == NULL) {
          fprintf(stderr, _undefined, _env_var_conf_glob, CR_DEFLT_CONF_GLOB);
     }
     else {
          fprintf(stderr, _msg, _env_var_conf_glob, _env_val_conf_glob);
     }

     if (disp_config) {
          cr_display_config();
     }
     exit(1);
}

/******************************************************************************

                         CR_DISPLAY_ARGS_LIST

******************************************************************************/
void cr_display_args_list(struct cr_config *config)
{
     struct cr_arg            *_arg;

     for (_arg = config->extract; _arg != 0; _arg = _arg->next) {
          fprintf(stderr, "      %s\n", _arg->value);
     }
     fprintf(stderr, "\n");
}

/******************************************************************************

                         CR_DISPLAY_ARGS

******************************************************************************/
void cr_display_args(struct cr_config *config)
{
     char                    **_argv;

     for (_argv = config->argv + 1; *_argv != 0; _argv++) {
          fprintf(stderr, "      %s\n", *_argv);
     }
     fprintf(stderr, "\n");
}

/******************************************************************************

                         CR_DISPLAY_CONFIG

******************************************************************************/
void cr_display_config(void)
{
     struct cr_config         *_config;

     fprintf(stderr, "  Configurations :\n");
     for (_config = G.configs.extract; _config != 0; _config = _config->next) {
		switch (G.verbose) {

		case 0:
			fprintf(stderr, "    --%s\n", _config->name);
			break;

		case	1:
			fprintf(stderr, "%-*s : %s\n",
				   CR_SZ_CFG_FILE, _config->config_file, _config->name);
			break;

		default:
			fprintf(stderr, "%-*s : %s\n",
				   CR_SZ_CFG_FILE, _config->config_file, _config->name);
			cr_display_args(_config);
			break;
		}
     }
}

/******************************************************************************

                         CR_INIT_LIST

******************************************************************************/
void cr_init_list(void)
{
     int                       _i;

     for (_i = 0; _i < CR_NB_COLORS; _i++) {
          G.list[_i]     = -1;
     }
     G.idx_list     = 0;
}

/******************************************************************************

                         CR_ADD_CONFIG

******************************************************************************/
void cr_add_config(struct cr_config *config)
{
     if (G.configs.insert == 0) {
          G.configs.insert    =
          G.configs.extract   = config;
     }
     else {
          G.configs.insert->next   = config;
          G.configs.insert         = config;
     }
}

/******************************************************************************

                         CR_ADD_ARG

******************************************************************************/
void cr_add_arg(struct cr_arg *arg)
{
     struct cr_config         *_config;

     assert(G.configs.insert != 0);

     _config   = G.configs.insert;
     if(_config->insert == 0) {
          _config->insert     =
          _config->extract    = arg;
     }
     else {
          _config->insert->next    = arg;
          _config->insert          = arg;
     }

     _config->argc++;

}

/******************************************************************************

                         CR_FREE_RE

******************************************************************************/
void cr_free_RE(void)
{
     struct cr_re_desc        *_re;

     for (_re = G.extract_RE; _re != NULL; _re = _re->next) {
          regfree(&_re->reg[0]);
     }
}

/******************************************************************************

                         CR_MARKER2COLOR

******************************************************************************/
void cr_marker2color( struct cr_re_desc *re)
{
     int                  _i = 0, _curr_level;
     struct cr_col_desc  *_desc;

    _curr_level          = re->curr_level;
//fprintf(stderr, "[%2d] >>> curr_level : %d\n", _i, _curr_level);
     for (_i = 0, _desc = G.desc; _i < G.length; _i++, _desc++) {
          switch (_desc->marker) {
          case  1:
               _curr_level++;
//fprintf(stderr, "[%2d] +++ curr_level : %d\n", _i, _curr_level);
               break;

          case -1:
               if (_curr_level > 0) {
                    _curr_level--;
               }
//fprintf(stderr, "[%2d] --- curr_level : %d\n", _i, _curr_level);
               break;

          case  0:
//fprintf(stderr, "[%2d] === curr_level : %d\n", _i, _curr_level);
               break;

          default:
               fprintf(stderr, "%s: internal error\n", G.prgname);
               exit(1);
               break;
          }
          if (!_desc->used) {
               if (_curr_level > 0) {
//                  fprintf(stderr, "==> [%2d] [%c] Color set. Col num = %d\n",
//                          _i, G.line[_i], re->col.col_num);
                    _desc->used    = TRUE;
                    _desc->col     = &re->col;
               }
               else {
//                  fprintf(stderr, "==> [%2d] [%c] NO COLOR.\n", _i, G.line[_i]);
               }
          }
          else {
//             fprintf(stderr, "==> [%2d] [%c] Already colorized.\n", _i, G.line[_i]);
          }
     }

     re->curr_level = _curr_level;
//fprintf(stderr, "[%2d] <<< curr_level : %d\n", _i, _curr_level);
}

/******************************************************************************

                         CR_SET_DESC

******************************************************************************/
void cr_set_desc(struct cr_re_desc *re, int offset, int s, int e, int marker)
{
     int                  _i;
     struct cr_col_desc  *_desc;

//fprintf(stderr, "SET_DESC : s = %3d  e = %3d  marker = %d\n", s, e, marker);
     if (re->regex[1] == 0) {
          /* RE descriptor does not define a range
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          for (_i = s, _desc = &G.desc[offset + s]; _i <= e; _i++, _desc++) {
               if (!_desc->used) {
                    _desc->used    = TRUE;
                    _desc->col     = &re->col;
               }
          }
     }
     else {
          switch (marker) {

          case  1:
               _desc          = &G.desc[offset + s];
               break;

          case -1:
               _desc          = &G.desc[offset + e + 1];    // XXX : ATTENTION AU SEGV
               break;

          default:
               fprintf(stderr, "%s: erreur interne, marker = %d\n",
                       G.prgname, marker);
               exit(1);
          }

          _desc->marker  = marker;
     }
}

/******************************************************************************

                         CR_SET_DESC_ALT

******************************************************************************/
void cr_set_desc_alt(struct cr_re_desc *re, int offset, int s, int e, struct cr_color *col)
{
     int                  _i;
     struct cr_col_desc  *_desc;

     /* RE descriptor does not define a range
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_i = s, _desc = &G.desc[offset + s]; _i <= e; _i++, _desc++) {
          if (!_desc->used) {
               _desc->used    = TRUE;
               _desc->col     = col;
          }
     }
}

/******************************************************************************

                         CR_READ_INPUT

******************************************************************************/
void cr_read_input(void)
{
     int                       _i, _j, _n, _s = 0, _e = 0, _off, _idx_last,
                               _marker = 0, _search_no;
     struct cr_re_desc        *_re;
     size_t                    _nmatch;
     regmatch_t                _pmatch[CR_SIZE + 1];
     int                       _eflags = 0;

     char                      _debug_str[CR_SIZE + 1];
     char                      _matching_str[CR_SIZE + 1];
     bool                      _change;

     _nmatch        = sizeof(_pmatch) / sizeof(_pmatch[0]);


     for (_n = sizeof(G.line); fgets(G.line, _n, stdin) != 0; _n = sizeof(G.line)) {
          /* Reset of color descriptors
             ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          cr_init_desc();

          G.length       = strlen(G.line);
          _idx_last      = G.length - 1;
          if (G.line[_idx_last] == '\n') {
               G.line[_idx_last]   = 0;
               G.newline           = TRUE;
          }
          else {
               G.newline           = FALSE;
          }

          if (G.debug) {
               fprintf(stderr, "LENGTH : %4d\n", G.length);
               fprintf(stderr, "LINE   : [%s] :\n", G.line);
          }

//X
          /* Loop on regexp
             ~~~~~~~~~~~~~~ */
          for (_re = G.extract_RE; _re != NULL; _re = _re->next) {
               /* Loop on BEGIN / END regexp
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               for (_i = 0; _i < 2; _i++) {
//X
                    if (_re->alt_cols == NULL) {
//X
                         if (_re->regex[_i]) {
                              if (_i == 1 && _re->begin_is_end) {
                                   break;
                              }
                              /* Search for multiple matches on the line
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                              for (_off = 0, _eflags = 0;
                                   _off < G.length &&
                                   regexec(&_re->reg[_i], G.line + _off, _nmatch, _pmatch,
                                   _eflags) == 0; _off += _e + 1, _eflags = REG_NOTBOL) {

                                   if (G.debug) {
                                        fprintf(stderr, "  Match for [%s] // [%s]\n",
                                                G.line + _off, _re->regex[_i]);
                                        fprintf(stderr, "    LINE : [%s] :\n", G.line + _off);
                                   }

                                   /* Loop on substrings
                                      ~~~~~~~~~~~~~~~~~~ */
                                   for (_j = 0; _j < _re->max_sub; _j++) {
                                        if (_j == 0 && _pmatch[1].rm_so != -1) {
                                             continue;
                                        }

                                        _s   = _pmatch[_j].rm_so;
                                        _e   = _pmatch[_j].rm_eo - 1;

                                        if (G.debug) {
                                             strncpy(_debug_str,
                                                     G.line + _off + _s, _e - _s + 1);
                                             _debug_str[_e -_s + 1]   = 0;
                                             fprintf(stderr,
                                                     "    OFFSET = %3d : %3d => %3d [%s] [%s]\n",
                                                     _off, _s, _e, _re->regex[_i], _debug_str);
                                        }

                                        if (!_re->begin_is_end) {
                                             if (_i == 0)   _marker   =  1;
                                             else           _marker   = -1;
                                        }
                                        else {
                                             _re->inside_zone    = !_re->inside_zone;
                                             if (_re->inside_zone)    _marker   =  1;
                                             else                     _marker   = -1;
                                        }
                                        if (_s >= 0) {
                                             cr_set_desc(_re, _off, _s, _e, _marker);
                                        }

                                        if (G.debug) {
                                             fprintf(stderr, "cr_set_desc : %d, %d => %d, %d\n",
                                                     _off, _s, _e, _marker);
                                             fprintf(stderr, "\n");
                                        }
                                   }

                                   /* To handle empty strings
                                      ~~~~~~~~~~~~~~~~~~~~~~~ */
                                   if (_e < 0) {
                                        _e   = 0;
                                   }
                              }

                              if (G.debug) {
                                   fprintf(stderr, "  NO MATCH for [%s] // [%s]\n",
                                           G.line + _off, _re->regex[_i]);
                              }
                         }
                    }
                    else {
                         if (G.debug) {
                              printf("ALTERNATE COLORS ...\n");
                         }

                         /* Search for multiple matches on the line
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _search_no     = 1;
                         for (_off = 0, _eflags = 0;
                              _off < G.length &&
                              regexec(&_re->reg[_i], G.line + _off, _nmatch, _pmatch,
                              _eflags) == 0; _off += _e + 1, _eflags = REG_NOTBOL, _search_no++) {

                              if (G.debug) {
                                   fprintf(stderr, "  Search %3d : Match for [%s] // [%s] _i = %d\n",
                                           _search_no, G.line + _off, _re->regex[_i], _i);
                                   fprintf(stderr, "  Search %3d :  LINE : [%s] :\n", _search_no, G.line + _off);
                              }

                              _change        = FALSE;
                              if ((_search_no == 1) && (_re->idx_regex_select < _re->max_sub)) {
                                   /* Check if the string matching the selection regexp has changed
                                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                   _j   = _re->idx_regex_select;
                                   if (G.debug) {
                                        printf("Search %3d : Selection regexp number = %d\n", _search_no, _j);
                                   }

                                   _s   = _pmatch[_j].rm_so;
                                   _e   = _pmatch[_j].rm_eo - 1;

                                   if (G.debug) {
                                        strncpy(_debug_str, G.line + _off + _s, _e - _s + 1);
                                        _debug_str[_e -_s + 1]   = 0;
                                        fprintf(stderr, "    OFFSET = %3d : %3d => %3d [%s] [%s] _j = %d\n",
                                                _off, _s, _e, _re->regex[_i], _debug_str, _j);
                                   }

                                   strncpy(_matching_str, G.line + _off + _s, _e - _s + 1);
                                   _matching_str[_e -_s + 1]   = 0;
                                   if (G.debug) {
                                        printf("String matching the selection regexp : [%s]\n", _matching_str);
                                   }
                                   if (_re->matching_str == NULL) {
                                        if (G.debug) {
                                             printf("NO MATCHING STRING STORED YET : storing [%s]\n", _matching_str);
                                        }

                                        /* No previous match for the selection regexp
                                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                        _re->matching_str   = strdup(_matching_str);
                                   }
                                   else {
                                        if (G.debug) {
                                             printf("Comparing [%s] and [%s] ...\n", _re->matching_str, _matching_str);
                                        }

                                        if (!strcmp(_re->matching_str, _matching_str)) {
                                             /* Current match is identical to the previous match
                                                ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                             if (G.debug) {
                                                  printf("IDENTICAL STRINGS\n");
                                             }

                                             if (!_re->change_on_diff) {
                                                  if (G.debug) {
                                                       printf("CHANGE !!!\n");
                                                  }
                                                  _change   = TRUE;
                                             }
                                             else {
                                                  if (G.debug) {
                                                       printf("No change !!!\n");
                                                  }
                                             }
                                        }
                                        else {
                                             /* Current match differs from the previous match
                                                ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                             if (G.debug) {
                                                  printf("DIFFERENT STRINGS\n");
                                             }
                                             free(_re->matching_str);
                                             _re->matching_str   = strdup(_matching_str);

                                             if (_re->change_on_diff) {
                                                  if (G.debug) {
                                                       printf("CHANGE !!!\n");
                                                  }
                                                  _change   = TRUE;
                                             }
                                             else {
                                                  if (G.debug) {
                                                       printf("No change !!!\n");
                                                  }
                                             }
                                        }
                                   }
                              }

                              if (_change) {
                                   /* Color change : select next color
                                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                   _re->alt_idx++;
                                   if (_re->alt_cols[_re->alt_idx] == NULL) {
                                        _re->alt_idx   = 0;
                                   }

                                   if (G.debug) {
                                        printf("***** CHANGE COLOR ***** : alt_idx = %d\n", _re->alt_idx);
                                   }
                              }

                              /* Loop on substrings
                                 ~~~~~~~~~~~~~~~~~~ */
                              for (_j = 0; _j < _re->max_sub; _j++) {
                                   if (G.debug) {
                                        printf("%s %s(%d) : _j = %d\n", __func__, __FILE__, __LINE__, _j);
                                   }

                                   if (_j == 0 && _pmatch[1].rm_so != -1) {
//                                 if (_pmatch[1].rm_so != -1) {
                                        continue;
                                   }

                                   _s   = _pmatch[_j].rm_so;
                                   _e   = _pmatch[_j].rm_eo - 1;

                                   if (G.debug) {
                                        strncpy(_debug_str,
                                                G.line + _off + _s, _e - _s + 1);
                                        _debug_str[_e -_s + 1]   = 0;
                                        fprintf(stderr,
                                                "    OFFSET = %3d : %3d => %3d [%s] [%s] _j = %d\n",
                                                _off, _s, _e, _re->regex[_i], _debug_str, _j);
                                   }

                                   if (_s >= 0) {
                                        cr_set_desc_alt(_re, _off, _s, _e, _re->alt_cols[_re->alt_idx]);

                                        if (G.debug) {
                                             cr_dump_color(_re->alt_cols[_re->alt_idx]);
                                             fprintf(stderr, "cr_set_desc_alt : offset = %d, [%d => %d], col = %d\n",
                                                     _off, _s, _e, _re->alt_cols[_re->alt_idx]->col_num);
                                             fprintf(stderr, "\n");
                                        }
                                   }
                              }

                              /* To handle empty strings
                                 ~~~~~~~~~~~~~~~~~~~~~~~ */
                              if (_e < 0) {
                                   _e   = 0;
                              }
                         }
                         break;
                    }

                    if (G.debug) {
                         fprintf(stderr, "\n");
                    }
               }

               if (_re->regex[1]) {
                    cr_marker2color(_re);
               }
          
               if (G.debug) {
                    fprintf(stderr, "\n");
               }
          }

          cr_disp_line();
     }
}

/******************************************************************************

                         CR_START_COLOR

******************************************************************************/
void cr_start_color(struct cr_color *col)
{
     int                       _col_num;
     FILE                     *_out;

     if (col)  _out = col->out;
     else      _out = stdout;

     _col_num       = col->col_num;

     if (_col_num > CR_WHITE) {
          if (_col_num != CR_NO_COLOR) {
               /* Reverse video
                  ~~~~~~~~~~~~~ */
               switch (col->intensity) {

               case 1:
                    fprintf(_out, "\033[%sm", cr_best_fg[_col_num - 9][0]);
                    fprintf(_out, "\033[48;5;%dm", cr_col_codes[_col_num - 9][0]);
                    break;

               case 2:
                    fprintf(_out, "\033[%sm", cr_best_fg[_col_num - 9][1]);
                    fprintf(_out, "\033[48;5;%dm", cr_col_codes[_col_num - 9][1]);
                    break;

               case 3:
                    fprintf(_out, "\033[%sm", cr_best_fg[_col_num - 9][2]);
                    fprintf(_out, "\033[48;5;%dm", cr_col_codes[_col_num - 9][2]);
                    break;

               case 4:
                    fprintf(_out, "\033[07;04;%dm", 30 + _col_num - 8);
                    break;

               default:
                    fprintf(stderr, "%s: invalid color brightness ! (col = %p : (%d, %d))\n",
                            G.prgname, col, col->col_num, col->intensity);
                    exit(1);
               }
          }
          else {
               /* No color
                  ~~~~~~~~ */
          }
     }
     else {
          /* Normal video
             ~~~~~~~~~~~~ */
          switch (col->intensity) {

          case 1:
               fprintf(_out, "\033[02;%dm", 30 + _col_num);    // Half-bright
               break;

          case 2:
               fprintf(_out, "\033[%dm", 30 + _col_num);       // Normal
               break;

          case 3:
               fprintf(_out, "\033[01;%dm", 30 + _col_num);    // Bold
               break;

          case 4:
               fprintf(_out, "\033[04;%dm", 30 + _col_num);    // Underscore
               break;

          default:
               fprintf(stderr, "%s: invalid color brightness ! (col = %p : (%d, %d))\n",
                       G.prgname, col, col->col_num, col->intensity);
               exit(1);
          }
     }
}

/******************************************************************************

                         CR_END_COLOR

******************************************************************************/
void cr_end_color(struct cr_color *col)
{
     FILE                     *_out;

     if (col)  _out = col->out;
     else      _out = stdout;

     if (col && col->col_num != CR_NO_COLOR) {
          fprintf(_out, "\033[0m");
     }
}

/******************************************************************************

                         CR_INIT_DESC

******************************************************************************/
void cr_init_desc(void)
{
     struct cr_col_desc       *_desc;

     G.length       = 0;

     for (_desc = G.desc; _desc < (&G.desc[sizeof(G.desc) / sizeof(G.desc[0])]);
          _desc++) {
          _desc->col          = NULL;
          _desc->used         = FALSE;
          _desc->marker       = 0;
     }
}

/******************************************************************************

                         CR_SAME_COLORS

******************************************************************************/
inline bool cr_same_colors(struct cr_color *col1, struct cr_color *col2)
{
     return (col1->col_num   == col2->col_num)
         && (col1->intensity == col2->intensity);
}

/******************************************************************************

                         CR_DISP_LINE

******************************************************************************/
void cr_disp_line(void)
{
     int                  _i, _c;
     struct cr_col_desc  *_desc;

     for (_i = 0, _desc = G.desc; _i < G.length; _i++, _desc++) {
          _c        = G.line[_i];
          if (_c == '\n' || (_c == 0 && G.newline)) {
               if (G.curr_col) {
                    cr_end_color(G.curr_col);
                    putc('\n', G.curr_col->out);
                    G.curr_col     = NULL;
               }
               else {
                    putc('\n', stdout);
               }
          }
          else if (_desc->used) {
               /* Character is in color
                  ~~~~~~~~~~~~~~~~~~~~~ */
               if (G.curr_col == NULL) {
                    /* Previous character was not in color
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//X
                    cr_start_color(_desc->col);
                    putc(_c, _desc->col->out);
                    G.curr_col     = _desc->col;
               }
               else {
                    /* Previous character was in color
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    if (cr_same_colors(_desc-> col, G.curr_col)) {
                         /* No color change
                            ~~~~~~~~~~~~~~~ */
                         putc(_c, G.curr_col->out);
                    }
                    else {
                         /* Color change
                            ~~~~~~~~~~~~ */
                         cr_end_color(G.curr_col);
//X
                         cr_start_color(_desc->col);
                         putc(_c, _desc->col->out);
                         G.curr_col     = _desc->col;
                    }
               }
          }
          else {
               /* Character is not in color
                  ~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (G.curr_col) {
                    /* Previous character was in color
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    cr_end_color(G.curr_col);
                    putc(_c, G.curr_col->out);
                    G.curr_col     = NULL;
               }
               else {
                    /* Previous character was not in color
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    putc(_c, stdout);
               }
          }
     }

#if 0
     if (G.newline) {
          if (G.curr_col) {
               cr_end_color(G.curr_col);
          }
     }
#endif

     _desc--;
     if (_desc->used) {
          cr_end_color(G.curr_col);
     }

     G.curr_col     = NULL;
}

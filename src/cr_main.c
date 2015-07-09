/* ============================================================================
 * Copyright (C) 2015 Martial Bornet
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
 *   Auteur       :     Martial BORNET (MB) - 3 Janvier 2015
 *
 *   Description  :     Programme de colorisation  de chaines
 *
 *   Fichier      :     cr_main.c
 *
 *   @(#)  cr_main.c  1.35  15/07/09  MB  
 *
 *   Liste des fonctions de ce fichier :
 *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   - cr_read_config_file
 *   - main
 *   - cr_usage
 *   - cr_init_list
 *   - cr_init_col_names
 *   - cr_set_color
 *   - cr_read_input
 *   - cr_start_color
 *   - cr_end_color
 *   - cr_init_desc
 *   - cr_set_desc
 *   - cr_disp_line
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

#include "cr_epri.h"

#define   X      if (G.debug) fprintf(stderr, "%s(%d)\n", __FILE__, __LINE__);

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
void cr_read_config_file(bool *already_done)
{
     char                     *_home, _cfg_file[1024];

     if (*already_done == TRUE) {
          return;
     }

     if ((_home = getenv("HOME")) == 0) {
          fprintf(stderr, "%s: HOME variable undefined !\n", G.prgname);
          exit(1);
     }

     sprintf(_cfg_file, "%s/%s", _home, CR_CONFIG_FILENAME);
     if (access(_cfg_file, 0) != 0) {
          fprintf(stderr, "%s: config file \"%s\" does not exist !\n",
                  G.prgname, _cfg_file);
          exit(1);
     }

     if ((yyin = fopen(_cfg_file, "r")) == NULL) {
          fprintf(stderr, "%s: cannot open \"%s\" !\n",
                  G.prgname, _cfg_file);
          perror("fopen");
          exit(1);
     }

     yylex();

     cr_lists2argv(&G.configs);

     *already_done  = TRUE;
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

                         CR_NEEDS_ARG

******************************************************************************/
bool cr_needs_arg(char opt, struct cr_args *args)
{
     int                  _i;

//printf("needs_args(%c, %s) ...\n", opt, args->opts);
     for (_i = 0; args->opts[_i] != 0; _i++) {
          if (args->opts[_i] != opt) continue;
          if (args->opts[_i + 1] == ':') {
//               printf("   => YES\n");
               return TRUE;
          }
          else {
               break;
          }
     }

//     printf("   => NO\n");
     return FALSE;
}

/******************************************************************************

                              CR_GET_CONFIG

******************************************************************************/
struct cr_config *cr_get_config(char *config_name, struct cr_args *args)
{
     struct cr_config         *_config;

     for (_config = args->configs->extract; _config != 0;
          _config = _config->next) {
          if (!strcmp(config_name, _config->name)) {
//               fprintf(stderr, "Config found !\n");
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

                              CR_GETOPT

******************************************************************************/
int cr_getopt(struct cr_args *args)
{
     /* Les pointeurs sont toujours positionnes sur
      * l'argument / option a traiter
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     char                 _c, *_arg, *_config_name, _next_char;
     struct cr_ptrs      *_ptrs, *_new_ptrs;
     struct cr_config    *_config;

     for ( ; ; ) {
          if (!(_ptrs = args->curr_ptrs)) {
               /* Plus d'arguments a traiter
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//fprintf(stderr, "NO MORE ARGS.\n");
               return -1;
          }
//X
          if (*(_ptrs->curr_argv) == 0) {
               /* Plus d'argument pour ce niveau
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//fprintf(stderr, "No more args for this level\n");
               args->curr_ptrs     = _ptrs->prev;
               free(_ptrs);
			continue;
          }

//fprintf(stderr, "Current arg = %p \"%s\"\n", _ptrs->curr_arg, _ptrs->curr_arg);

//fprintf(stderr, "curr_idx = %d\n", _ptrs->curr_idx);
          if (_ptrs->curr_idx == 0) {
//X
               /* Traitement d'un nouvel argument
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (_ptrs->curr_arg[0] != '-') {
                    fprintf(stderr, "%s: argument with no associated option (\"%s\") ! \n",
                            G.prgname, _ptrs->curr_arg);
                    cr_dump_args(args);
                    exit(1);
               }
               else {
                    /* Suite du traitement d'un argument
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _ptrs->curr_idx++;
                    if (_ptrs->curr_arg[1] != '-') {
                         /* Suite des options a une lettre
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         break;
                    }
                    else {
                         /* Nom de configuration
                            ~~~~~~~~~~~~~~~~~~~~ */
//X
                         _config_name        = _ptrs->curr_arg + 2;
//fprintf(stderr, "CONFIG : \"%s\"\n", _config_name);
					_ptrs->curr_argv++;
//X
					_ptrs->curr_arg	= *_ptrs->curr_argv;
//X
//fprintf(stderr, "Apres incrementation pointeur CONFIG :\n");
//fprintf(stderr, "Current arg = %p \"%s\"\n", _ptrs->curr_arg, _ptrs->curr_arg);

                         _config             = cr_get_config(_config_name, args);
                         if (_config == 0) {
                              fprintf(stderr, "%s: undefined configuration (%s) !\n",
                                      G.prgname, _config_name);
                              exit(1);
                         }

//X
                         /* Recherche de boucle recursive
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
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
                         _new_ptrs->config        = _config;          // Utilite ???
                         args->curr_ptrs          = _new_ptrs;

//X
                         continue;
                    }
               }
          }
          else {
//X
               break;
          }
     }
//X

     /* Options
        ~~~~~~~ */
//X
     _c        = _ptrs->curr_arg[_ptrs->curr_idx];
//X
//fprintf(stderr, "OPTION : %c\n", _c);
     _ptrs->curr_idx++;
     _next_char     = _ptrs->curr_arg[_ptrs->curr_idx];
     if (cr_needs_arg(_c, args)) {
//fprintf(stderr, "  argument needed.\n");
          _arg      = *(_ptrs->curr_argv + 1);
          if (_arg[0] == '-' || _next_char != 0) {
               fprintf(stderr, "%s: missing argument for \"-%c\" !\n",
                       G.prgname, _c);
               cr_dump_args(args);
               exit(1);
          }
//X

          args->optarg      = _arg;
//fprintf(stderr, "  OPTARG = \"%s\"\n", args->optarg);
          _ptrs->curr_argv++;
          _ptrs->curr_arg     = *_ptrs->curr_argv;
     }
     else {
//fprintf(stderr, "  no argument needed.\n");
          args->optarg      = 0;
     }

//X
     if (_next_char == 0) {
//fprintf(stderr, "  no more 1 letter option\n");
//X
//fprintf(stderr, "Current arg = %p \"%s\"\n", _ptrs->curr_arg, _ptrs->curr_arg);
          if (*(_ptrs->curr_argv + 1) == 0) {
               /* Plus d'argument pour ce niveau
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//fprintf(stderr, "No more args for this level\n");
               args->curr_ptrs     = _ptrs->prev;
               free(_ptrs);
          }
          else {
               _ptrs->curr_argv++;
               _ptrs->curr_arg     = *_ptrs->curr_argv;
               _ptrs->curr_idx     = 0;
//fprintf(stderr, "  Next arg  = %p \"%s\"\n", _ptrs->curr_arg, _ptrs->curr_arg);
          }
     }
//X
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

                              MAIN

******************************************************************************/
int main(int argc, char *argv[])
{
     int                       _opt, _i;
     struct cr_color          *_col;
     struct cr_args           *_args;

     G.prgname      = argv[0];
     G.out          = stdout;

     if (argc == 1) {
          cr_usage(FALSE);
     }

     cr_init_list();
     cr_init_col_names();
     G.intensity    = CR_DEFLT_INTENSITY;

     /* Scan des arguments a la recherche d'une configuration (--...)
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_i = 1; _i < argc; _i++) {
          if (argv[_i][0] == '-' &&  argv[_i][1] == '-' &&  argv[_i][2] != 0) {
               cr_read_config_file(&G.config_file_read);
               break;
          }
     }

     /* Analyse des arguments
        ~~~~~~~~~~~~~~~~~~~~~ */
     _args               = cr_set_args(argc, argv,
                                       "hHuVvEr:g:y:b:m:c:w:R:G:Y:B:M:C:W:Ddei1234",
                                       &G.configs);
     while ((_opt = cr_getopt(_args)) != -1) {
          switch (_opt) {
          case 'h':
               cr_usage(FALSE);
               break;

          case 'H':
               cr_read_config_file(&G.config_file_read);
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

          case 'r':
               cr_set_color(CR_RED, _args->optarg);
               break;

          case 'g':
               cr_set_color(CR_GREEN, _args->optarg);
               break;

          case 'y':
               cr_set_color(CR_YELLOW, _args->optarg);
               break;

          case 'b':
               cr_set_color(CR_BLUE, _args->optarg);
               break;

          case 'm':
               cr_set_color(CR_MAGENTA, _args->optarg);
               break;

          case 'c':
               cr_set_color(CR_CYAN, _args->optarg);
               break;

          case 'w':
               cr_set_color(CR_WHITE, _args->optarg);
               break;

          case 'R':
               cr_set_color(CR_RED_REV, _args->optarg);
               break;

          case 'G':
               cr_set_color(CR_GREEN_REV, _args->optarg);
               break;

          case 'Y':
               cr_set_color(CR_YELLOW_REV, _args->optarg);
               break;

          case 'B':
               cr_set_color(CR_BLUE_REV, _args->optarg);
               break;

          case 'M':
               cr_set_color(CR_MAGENTA_REV, _args->optarg);
               break;

          case 'C':
               cr_set_color(CR_CYAN_REV, _args->optarg);
               break;

          case 'W':
               cr_set_color(CR_WHITE_REV, _args->optarg);
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
               G.verbose      = TRUE;
               break;

          case 'V':
               fprintf(stderr, "%s: version %s\n", G.prgname, "1.35");
               exit(1);
               break;

          case '1':
          case '2':
          case '3':
          case '4':
               G.intensity    = _opt - '0';
               break;

          default:
               fprintf(stderr, "%s: unknown option '%c' !\n", G.prgname, _opt);
               cr_usage(FALSE);
               break;
          }
     }

     if (G.disp_regex) {
          for (_i = 0; _i < G.idx_list; _i++) {
               _col           = &G.color_RE[G.list[_i]];
               printf("[%2d] ", _i + 1);
               cr_start_color(NULL, _col->col_num);
               printf("%s\n", _col->RE.regex);
               cr_end_color(NULL);
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
     fprintf(stderr, "%s: version %s\n", G.prgname, "1.35");
     fprintf(stderr, "Usage: %s [-h|-H|-eidD1234][-E][-rgybmcwRGYBMCW|--config_name] regexp ...\n",
             G.prgname);
     fprintf(stderr, "  -h : help\n");
     fprintf(stderr, "  -H : help + configuration names\n");
     fprintf(stderr, "  -V : version\n");
     fprintf(stderr, "  -v : verbose\n");
     fprintf(stderr, "  -u : do not bufferize output on stdout\n");
     fprintf(stderr, "  -e : extended regular expressions\n");
     fprintf(stderr, "  -i : ignore case\n");
     fprintf(stderr, "  -E : print on stderr\n");
     fprintf(stderr, "  -r : red\n");
     fprintf(stderr, "  -g : green\n");
     fprintf(stderr, "  -y : yellow\n");
     fprintf(stderr, "  -b : blue\n");
     fprintf(stderr, "  -m : magenta\n");
     fprintf(stderr, "  -c : cyan\n");
     fprintf(stderr, "  -w : white\n");
     fprintf(stderr, "  -R : red     (reverse video)\n");
     fprintf(stderr, "  -G : green   (reverse video)\n");
     fprintf(stderr, "  -Y : yellow  (reverse video)\n");
     fprintf(stderr, "  -B : blue    (reverse video)\n");
     fprintf(stderr, "  -M : magenta (reverse video)\n");
     fprintf(stderr, "  -C : cyan    (reverse video)\n");
     fprintf(stderr, "  -W : white   (reverse video)\n");
     fprintf(stderr, "  -d : debug\n");
     fprintf(stderr, "  -D : display regular expressions\n");
     fprintf(stderr, "  -1 : color brightness (half-bright)\n");
     fprintf(stderr, "  -2 : color brightness (normal : default)\n");
     fprintf(stderr, "  -3 : color brightness (bright)\n");
     fprintf(stderr, "  -4 : color brightness (underscore)\n");

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
          fprintf(stderr, "    --%s\n", _config->name);
          if (G.verbose) {
               cr_display_args(_config);
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

                         CR_INIT_COL_NAMES

******************************************************************************/
void cr_init_col_names(void)
{
     CR_INIT_COL(0);
     CR_INIT_COL(1);
     CR_INIT_COL(2);
     CR_INIT_COL(3);
     CR_INIT_COL(4);
     CR_INIT_COL(5);
     CR_INIT_COL(6);
     CR_INIT_COL(7);
     CR_INIT_COL(8);
     CR_INIT_COL(9);
     CR_INIT_COL(10);
     CR_INIT_COL(11);
     CR_INIT_COL(12);
     CR_INIT_COL(13);
     CR_INIT_COL(14);
     CR_INIT_COL(15);
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

                         CR_SET_COLOR

******************************************************************************/
void cr_set_color(int color, char *regexp)
{
     int                       _idx;
     struct cr_color          *_col;

     _idx           = CR_COLOR_IDX(color);

     _col           = &G.color_RE[_idx];

     if (_col->used) {
          /* Couleur deja specifiee
             ~~~~~~~~~~~~~~~~~~~~~~ */
          fprintf(stderr, "%s: duplicated color (%s)\n", G.prgname, _col->col_name);
          exit(1);
     }
     G.list[G.idx_list++]     = _idx;

     _col->used               = TRUE;
     _col->col_num            = color;
     _col->RE.cflags          = G.cflags;
     _col->RE.regex           = regexp;
     _col->out                = G.out;
     G.out                    = stdout;

     if (regcomp(&_col->RE.reg, regexp, _col->RE.cflags) != 0) {
          fprintf(stderr, "%s: regcomp error for \"%s\" !\n", G.prgname, regexp);
          exit(1);
     }
}

/******************************************************************************

                         CR_FREE_RE

******************************************************************************/
void cr_free_RE(void)
{
     int                       _i;
     regex_t                  *_reg;

     for (_i = 0; _i < CR_NB_COLORS; _i++) {
          _reg      = &G.color_RE[_i].RE.reg;
          if (_reg) {
               regfree(_reg);
          }
     }
}
/******************************************************************************

                         CR_READ_INPUT

******************************************************************************/
void cr_read_input(void)
{
     int                       _i, _j, _n, _s = 0, _e = 0, _off, _idx_last;
     struct cr_color          *_col;
     size_t                    _nmatch;
     regmatch_t                _pmatch[CR_SIZE + 1];
     int                       _eflags = 0;

     char                      _debug_str[CR_SIZE + 1];

     _nmatch        = sizeof(_pmatch) / sizeof(_pmatch[0]);

     for (_n = sizeof(G.line); fgets(G.line, _n, stdin) != 0; _n = sizeof(G.line)) {
          /* Reinitialisation des descripteurs de couleurs
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
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

          for (_i = 0; _i < G.idx_list; _i++) {
               _col           = &G.color_RE[G.list[_i]];

               for (_off = 0, _eflags = 0;
                    regexec(&_col->RE.reg, G.line + _off, _nmatch, _pmatch,
                    _eflags) == 0; _off += _e + 1, _eflags = REG_NOTBOL) {

                    if (G.debug) {
                         fprintf(stderr, "Match for [%s] // [%s]\n",
                                 G.line + _off, _col->RE.regex);
                         fprintf(stderr, "LINE : [%s] :\n", G.line + _off);
                    }

                    for (_j = 0; _pmatch[_j].rm_so != -1; _j++) {
                         if (_j == 0 && _pmatch[1].rm_so != -1) {
                              continue;
                         }

                         _s   = _pmatch[_j].rm_so;
                         _e   = _pmatch[_j].rm_eo - 1;

                         if (G.debug) {
                              strncpy(_debug_str, G.line + _off + _s, _e - _s + 1);
                              _debug_str[_e -_s + 1]   = 0;
                              fprintf(stderr, "OFFSET = %3d : %3d => %3d [%s] [%s]\n",
                                      _off, _s, _e, _col->RE.regex, _debug_str);
                         }

                         cr_set_desc(_col, _off, _s, _e);
                    }

                    /* Pour traiter les chaines vides
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    if (_e < 0) {
                         fprintf(stderr, "%s: warning : empty match !\n", G.prgname);
                         exit(1);
                    }
               }
               if (G.debug) {
                    fprintf(stderr, "NO MATCH for [%s] // [%s]\n", G.line + _off,
                            _col->RE.regex);
               }
          }

          cr_disp_line();
     }

     exit(1);
}

/******************************************************************************

                         CR_START_COLOR

******************************************************************************/
void cr_start_color(struct cr_color *col, int color)
{
     FILE                     *_out;

     if (col)  _out = col->out;
     else      _out = stdout;

     if (color > CR_WHITE) {
          /* Reverse video
             ~~~~~~~~~~~~~ */
          switch (G.intensity) {

          case 1:
               fprintf(_out, "\033[07;02;%dm", 30 + color - 8);
               break;

          case 2:
               fprintf(_out, "\033[07;%dm", 30 + color - 8);
               break;

          case 3:
               fprintf(_out, "\033[07;01;%dm", 30 + color - 8);
               break;

          case 4:
               fprintf(_out, "\033[07;04;%dm", 30 + color - 8);
               break;

          default:
               fprintf(stderr, "%s: invalid color brightness !\n", G.prgname);
               exit(1);
          }
     }
     else {
          /* Normal video
             ~~~~~~~~~~~~ */
          switch (G.intensity) {

          case 1:
               fprintf(_out, "\033[02;%dm", 30 + color);    // Half-bright
               break;

          case 2:
               fprintf(_out, "\033[%dm", 30 + color);       // Normal
               break;

          case 3:
               fprintf(_out, "\033[01;%dm", 30 + color);    // Bold
               break;

          case 4:
               fprintf(_out, "\033[04;%dm", 30 + color);    // Underscore
               break;

          default:
               fprintf(stderr, "%s: invalid color brightness !\n", G.prgname);
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

     fprintf(_out, "\033[0m");
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
     }
}

/******************************************************************************

                         CR_SET_DESC

******************************************************************************/
void cr_set_desc(struct cr_color *col, int offset, int s, int e)
{
     int                  _i;
     struct cr_col_desc  *_desc;

     for (_i = s, _desc = &G.desc[offset + s]; _i <= e; _i++, _desc++) {
          if (!_desc->used) {
               _desc->used    = TRUE;
               _desc->col     = col;
          }
     }
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
                    putc('\n', G.curr_col->out);
                    cr_end_color(G.curr_col);
               }
               else {
                    putc('\n', stdout);
               }
          }
          else if (_desc->used) {
               /* Le caractere est en couleur
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (G.curr_col == NULL) {
                    /* Le caractere precedent n'etait pas en couleur
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    cr_start_color(_desc->col, _desc->col->col_num);
                    putc(_c, _desc->col->out);
               }
               else {
                    /* Le caractere precedent etait en couleur
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    if (_desc-> col == G.curr_col) {
                         /* Pas de changement de couleur
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         putc(_c, G.curr_col->out);
                    }
                    else {
                         /* Changement de couleur
                            ~~~~~~~~~~~~~~~~~~~~~ */
                         cr_end_color(G.curr_col);
                         cr_start_color(_desc->col, _desc->col->col_num);
                         putc(_c, _desc->col->out);
                    }
               }
          }
          else {
               /* Le caractere n'est pas en couleur
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (G.curr_col == NULL) {
                    /* Le caractere precedent n'etait pas en couleur
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    putc(_c, stdout);
               }
               else {
                    /* Le caractere precedent etait en couleur
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    cr_end_color(G.curr_col);
                    putc(_c, G.curr_col->out);
               }
          }
          G.curr_col     = _desc->col;
          if (_c == '\n') {
               G.curr_col     = NULL;
          }
     }

     if (G.newline) {
          if (G.curr_col) {
               cr_end_color(G.curr_col);
          }
     }

     _desc--;
     if (_desc->used) {
          cr_end_color(G.curr_col);
     }
}

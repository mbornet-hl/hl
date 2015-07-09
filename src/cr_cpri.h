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
 *   (C) Copyright Martial Bornet, 2015.
 *
 *   Auteur       :     Martial BORNET (MB) - 3 Janvier 2015
 *
 *   Description  :     Constantes privees
 *
 *   Fichier      :     cr_cpri.h
 *
 *   @(#)  cr_cpri.h  1.15  15/07/08  MB  
 *
 * ============================================================================
 */

#if ! defined(CR_CPRI_H)
# define CR_CPRI_H

#include <stdio.h>
#include <sys/types.h>
#include <regex.h>

#define   CR_CONFIG_FILENAME            ".hl.cfg"

#define   bool                          int
#define   FALSE                         (0)
#define   TRUE                          (1)

#define   CR_NB_COLORS                  (16)

#define   CR_DEFLT_INTENSITY            (2)

/* Numeros des couleurs
 * ~~~~~~~~~~~~~~~~~~~~ */
#define   CR_BLACK                      (0)
#define   CR_RED                        (1)
#define   CR_GREEN                      (2)
#define   CR_YELLOW                     (3)
#define   CR_BLUE                       (4)
#define   CR_MAGENTA                    (5)
#define   CR_CYAN                       (6)
#define   CR_WHITE                      (7)
#define   CR_BLACK_REV                  (8)
#define   CR_RED_REV                    (9)
#define   CR_GREEN_REV                  (10)
#define   CR_YELLOW_REV                 (11)
#define   CR_BLUE_REV                   (12)
#define   CR_MAGENTA_REV                (13)
#define   CR_CYAN_REV                   (14)
#define   CR_WHITE_REV                  (15)

#define   CR_C0                         "black"
#define   CR_C1                         "red"
#define   CR_C2                         "green"
#define   CR_C3                         "yellow"
#define   CR_C4                         "blue"
#define   CR_C5                         "magenta"
#define   CR_C6                         "cyan"
#define   CR_C7                         "white"
#define   CR_C8                         "black reverse"
#define   CR_C9                         "red reverse"
#define   CR_C10                        "green reverse"
#define   CR_C11                        "yellow reverse"
#define   CR_C12                        "blue reverse"
#define   CR_C13                        "magenta reverse"
#define   CR_C14                        "cyan reverse"
#define   CR_C15                        "white reverse"

#define   CR_INIT_COL(n)                { G.color_RE[CR_COLOR_IDX(n)].col_name = CR_C ## n; \
                                          G.color_RE[CR_COLOR_IDX(n)].col_num  = (n % 8); }

/* Indice de tableau d'une couleur
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_COLOR_IDX(color)           ((color))

/* Taille d'une ligne, nombre d'intervalles differents
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_SIZE                       (1024)

//#define   CR_DISP_LEX(...)			fprintf(stderr, __VA_ARGS__)
#define   CR_DISP_LEX(...)			

#define   CR_NEW(name)                                                     \
struct cr_##name *cr_new_##name(void)                                      \
{                                                                          \
     struct cr_##name              *_##name;                               \
                                                                           \
     if ((_##name = (struct cr_##name *) malloc(sizeof(*_##name))) == 0) { \
          fprintf(stderr, cr_err_malloc, G.prgname);                       \
          exit(1);                                                         \
     }                                                                     \
     bzero(_##name, sizeof(*_##name));                                     \
                                                                           \
     return _##name;                                                       \
}
#define   CR_DECL_NEW(name)             struct cr_##name *cr_new_##name(void)

#if ! defined(yyin)
//#define   yyin                          CR_in
#endif
#if ! defined(yylex)
#define   yylex                         CR_lex
#endif

/* Structures
   ~~~~~~~~~~ */
struct cr_RE {
     regex_t                             reg;
     char                               *regex;
     int                                 cflags;
};

struct cr_color {
     bool                                used;
     char                               *col_name;
     int                                 col_num;
     struct cr_RE                        RE;
     int                                 linux_code;
     FILE                               *out;
};

struct cr_col_desc {
     bool                                used;
     struct cr_color                    *col;
};

struct cr_arg {
     char                               *value;
     struct cr_arg                      *next;
};

struct cr_config {
     char                               *name;
     bool                                visited;
     struct cr_config                   *next;
     struct cr_arg                      *extract,
                                        *insert;
     int                                 argc;
     char                              **argv;
};

struct cr_configs {
     struct cr_config                   *extract,
                                        *insert;
};

struct cr_ptrs {
     int                                 argc;
     char                              **argv;
     char                              **curr_argv;
     char                               *curr_arg;
     int                                 curr_idx;
     char                               *next_arg;
     struct cr_ptrs                     *prev;
     struct cr_config                   *config;
};

struct cr_args {
     char                               *opts;
     char                               *optarg;
     struct cr_ptrs                     *curr_ptrs;
     struct cr_configs                  *configs;
};

struct cr_global {
     char                               *prgname;

     struct cr_color                     color_RE[CR_NB_COLORS];
     struct cr_color                    *curr_col;
     int                                 cflags;
     int                                 list[CR_NB_COLORS];
     int                                 idx_list;
     char                                line[CR_SIZE + 1];
     struct cr_col_desc                  desc[CR_SIZE + 1];
     int                                 length;
     bool                                debug,
                                         verbose,
                                         disp_regex,
								 config_file_read;
     FILE                               *out;
     bool                                newline;
     int                                 intensity;
     struct cr_configs                   configs;
};

#endif    /* CR_CPRI_H */

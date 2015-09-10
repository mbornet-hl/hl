/* ============================================================================
 * Copyright (C) 2015, Martial Bornet
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
 *   Author       :     Martial BORNET (MB) - 3rd of January, 2015
 *
 *   Description  :     Private constants
 *
 *   File         :     cr_cpri.h
 *
 *   @(#)  [MB] cr_cpri.h Version 1.24 du 15/09/10 -  
 *
 * ============================================================================
 */

#if ! defined(CR_CPRI_H)
# define CR_CPRI_H

#include <stdio.h>
#include <sys/types.h>
#include <regex.h>

#define   CR_CONFIG_FILENAME            ".hl.cfg"
#define   CR_DEFLT_CONFIG_FILE          "/etc/default/hl"

#define   bool                          int
#define   FALSE                         (0)
#define   TRUE                          (1)

#define   CR_NB_COLORS                  (16)

#define   CR_DEFLT_INTENSITY            (2)

/* Color numbers
 * ~~~~~~~~~~~~~ */
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

/* Line size, number of different intervals
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_SIZE                       (1024)

#define   CR_DISP_LEX(...)              if (G.disp_lex) fprintf(stderr, __VA_ARGS__)
#define   CR_DEBUG(...)                 if (G.debug) {                                              \
                                             fprintf(stderr, "%-15s (%4d) ", __func__, __LINE__);   \
                                             fprintf(stderr, __VA_ARGS__);                          \
                                        }

/* Macros to define and declare a "new" function
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
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

#if defined(CDD)
#define   yylex                         CR_lex
#define   yyin                          CR_in
#endif

/* Structures
   ~~~~~~~~~~ */
struct cr_color {
     int                                 col_num;
     int                                 intensity;
     int                                 linux_code;
     FILE                               *out;
};

struct cr_re_desc {
     regex_t                             reg[2];
     char                               *regex[2];
     int                                 cflags;
	int							 curr_level;
     struct cr_color                     col;
     struct cr_re_desc                  *next;
};

struct cr_col_desc {
     bool                                used;
     struct cr_color                    *col;
	int							 marker;
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
                                         disp_lex,
                                         config_file_read;
     FILE                               *out;
     bool                                newline;
	bool							 begin_specified,
								 end_specified;
     int                                 intensity;
     struct cr_configs                   configs;
     struct cr_re_desc                  *extract_RE,
                                        *insert_RE,
								*last_RE;
	int							 last_color;
};

#endif    /* CR_CPRI_H */

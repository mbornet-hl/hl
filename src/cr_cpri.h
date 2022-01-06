/* ============================================================================
 * Copyright (C) 2015-2021, Martial Bornet
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
 *   Description  :     Private constants
 *
 *   File         :     cr_cpri.h
 *
 *   @(#)  [MB] cr_cpri.h Version 1.53 du 22/01/06 -  
 *
 * Sources from the original hl command are available on :
 * https://github.com/mbornet-hl/hl
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
#define   CR_ENV_DEFLT                  "HL_DEFAULT"
#define   CR_ENV_DEFLT_ALTERNATE_1      "HL_A1"
#define   CR_ENV_DEFLT_ALTERNATE_2      "HL_A2"
#define   CR_ENV_CONF                   "HL_CONF"
#define   CR_ENV_CONF_GLOB              "HL_CONF_GLOB"

#define   CR_DEFLT_COLOR                "3Y"
#define   CR_DEFLT_ALT_INTENSITY_1      (2)
#define   CR_DEFLT_ALT_COLOR_1          'B'
#define   CR_DEFLT_ALT_INTENSITY_2      (1)
#define   CR_DEFLT_ALT_COLOR_2          'n'

#define   CR_DEFLT_ALT_REGEXP           "^(.*)$"
#define   CR_DEFLT_CONF_GLOB            "hl_*.cfg:hl.cfg:.hl_*.cfg:.hl.cfg"

#define   CR_CONF_LIST_ALL              (1)
#define   CR_CONF_SEARCH_BY_REGEXP      (2)

#define   bool                          int
#if ! defined(FALSE)
#define   FALSE                         (0)
#endif

#if ! defined(TRUE)
#define   TRUE                          (1)
#endif

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
#define   CR_NO_COLOR                   (16)

/* Strings describing elements for
 * alternate options
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_SELECTOR_STRING            "0123456789"
#define   CR_INT_STRING                 "0123456789"
#define   CR_INTENSITY_STRING           "12345"
#define   CR_COLORS_STRING              "rgybmcwnRGYBMCW"
#define   CR_OP_STRING                  "+-*/"
#define   CR_BASE_STRING                "adox"
 
/* Line size, number of different intervals
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_SIZE                       (65536)

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

#define   CR_SYNTAX_OPT(x)              fprintf(stderr, cr_err_syntax_opt, G.prgname, x, __func__, __FILE__, __LINE__)

/* Parser states for the decoding of option -A
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_STATE_INITIAL              (1)
#define   CR_STATE_W_PARAM              (2)
#define   CR_STATE_W_BASE               (3)
#define   CR_STATE_W_SEPARATOR_2        (4)
#define   CR_STATE_W_SELECTOR_ID        (5)
#define   CR_STATE_W_SEPARATOR          (6)
#define   CR_STATE_W_INTENSITY          (7)
#define   CR_STATE_W_COLOR              (8)
#define   CR_STATE_W_END                (9)

/* Formatting parameters
   ~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_SZ_CFG_FILE                (60)

/* Bases for seq control
   ~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_BASE_DEC                   (0x1001)
#define   CR_BASE_HEX                   (0x1002)
#define   CR_BASE_OCT                   (0x1003)
#define   CR_BASE_ASCII                 (0x1004)

/* Operations for seq control
   ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_OP_ADD                     (0x2001)
#define   CR_OP_SUB                     (0x2002)
#define   CR_OP_MUL                     (0x2003)
#define   CR_OP_DIV                     (0x2004)

/* Structures
   ~~~~~~~~~~ */
struct cr_color {
     int                                 col_num;
     int                                 intensity;
     FILE                               *out;
};

struct cr_re_desc {
     regex_t                             reg[2];
     char                               *regex[2];
     bool                                begin_is_end,
                                         inside_zone;
     int                                 cflags;
     int                                 curr_level;
     struct cr_color                     col;
     struct cr_re_desc                  *next;
     int                                 max_sub;
     struct cr_color                   **alt_cols;               /* Array for alternate colors */
     int                                 alt_idx;                /* Current alternate index    */
     int                                 idx_regex_select;       /* Index of selector regex    */
     char                               *matching_str;           /* String matching the        */
                                                                 /* selector regex             */
     bool                                alternate,              /* Alternate                  */
                                         sequential;             /* Sequential                 */
     bool                                change_on_diff;         /* Change on difference       */
     bool                                change_on_bad_next;     /* Change on bad next value   */
     long                                val;                    /* Last value for seq control */
     long                                param;                  /* Parameter for seq control  */
     int                                 op;                     /* Operation for seq control  */
     int                                 base;                   /* Base for seq control       */
};

struct cr_col_desc {
     bool                                used;
     struct cr_color                    *col;
     int                                 marker;
};

struct cr_arg {
     char                               *value;
     struct cr_arg                      *next;
};

struct cr_config {
     char                               *name;
     bool                                visited;
     char                               *config_file;
     struct cr_config                   *next;
     struct cr_arg                      *extract,
                                        *insert;
     int                                 argc;
     char                              **argv;
     int                                 nb_opts;
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
     int                                 special_opt;
};

struct cr_global {
     char                               *prgname;

     char                               *selector_string,
                                        *color_string,
                                        *intensity_string,
                                        *int_string,
                                        *op_string,
                                        *base_string;
     char                               *cfg_filename;
     struct cr_color                     color_RE[CR_NB_COLORS];
     struct cr_color                    *curr_col;
     struct cr_color                    *deflt_alt_col_1,
                                        *deflt_alt_col_2;
     int                                 cflags;
     int                                 list[CR_NB_COLORS];
     int                                 idx_list;
     char                                line[CR_SIZE + 1];
     struct cr_col_desc                  desc[CR_SIZE + 1];
     int                                 length;
     bool                                debug,
                                         verbose,
                                         disp_nb_opts,
                                         disp_regex,
                                         disp_lex,
                                         config_file_read;
     bool                                consistency;            /* Consistent numbering of    */
                                                                 /* sub regex between -A/-I    */
                                                                 /* and -s                     */
     FILE                               *out,
                                        *usage_out;
     bool                                newline;
     bool                                begin_specified,
                                         end_specified;
     int                                 intensity;
     struct cr_configs                   configs;
     struct cr_re_desc                  *extract_RE,
                                        *insert_RE,
                                        *last_RE;
     int                                 last_color;
};

#endif    /* CR_CPRI_H */

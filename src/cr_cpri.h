/* ============================================================================
 * Copyright (C) 2015-2022, Martial Bornet
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
 *   (C) Copyright Martial Bornet, 2015-2022.
 *
 *   Author       :     Martial BORNET (MB) - 3rd of January, 2015
 *
 *   Description  :     Private constants
 *
 *   File         :     cr_cpri.h
 *
 *   @(#)  [MB] cr_cpri.h Version 1.67 du 22/04/03 -  
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

/* Sum of the N first numbers
   ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_SUM(N)                     (((N) * (N + 1)) / 2)

/* Index of elements of thresholds
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_IDX(i, j)                  (CR_SUM((i)) - (i) + (j) - 1)

/* Default configuration files
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_CONFIG_FILENAME            ".hl.cfg"
#define   CR_DEFLT_CONFIG_FILE          "/etc/default/hl"
#define   CR_DEFLT_CONF                 "~/" CR_CONFIG_FILENAME ";" CR_DEFLT_CONFIG_FILE

/* Environment variable for default color
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_ENV_DEFLT                  "HL_DEFAULT"

/* Environment variables for configurations
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_ENV_CONF                   "HL_CONF"
#define   CR_ENV_CONF_GLOB              "HL_CONF_GLOB"

/* Environment variables for alternate options
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_ENV_ALTERNATE_1            "HL_A1"
#define   CR_ENV_ALTERNATE_2            "HL_A2"

/* Environment variables for days of the week
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_ENV_DOW_SPEC               "HL_DOW_SPEC"
#define   CR_ENV_DOW_RE                 "HL_DOW_REGEX"
#define   CR_ENV_DOW_SUNDAY             "HL_SUNDAY"
#define   CR_ENV_DOW_MONDAY             "HL_MONDAY"
#define   CR_ENV_DOW_TUESDAY            "HL_TUESDAY"
#define   CR_ENV_DOW_WEDNESDAY          "HL_WEDNESDAY"
#define   CR_ENV_DOW_THURSDAY           "HL_THURSDAY"
#define   CR_ENV_DOW_FRIDAY             "HL_FRIDAY"
#define   CR_ENV_DOW_SATURDAY           "HL_SATURDAY"

/* Environment variables for thresholds colors
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_ENV_T_2_1                  "HL_T_2_1"
#define   CR_ENV_T_2_2                  "HL_T_2_2"

#define   CR_ENV_T_3_1                  "HL_T_3_1"
#define   CR_ENV_T_3_2                  "HL_T_3_2"
#define   CR_ENV_T_3_3                  "HL_T_3_3"

#define   CR_ENV_T_4_1                  "HL_T_4_1"
#define   CR_ENV_T_4_2                  "HL_T_4_2"
#define   CR_ENV_T_4_3                  "HL_T_4_3"
#define   CR_ENV_T_4_4                  "HL_T_4_4"

#define   CR_ENV_T_5_1                  "HL_T_5_1"
#define   CR_ENV_T_5_2                  "HL_T_5_2"
#define   CR_ENV_T_5_3                  "HL_T_5_3"
#define   CR_ENV_T_5_4                  "HL_T_5_4"
#define   CR_ENV_T_5_5                  "HL_T_5_5"

#define   CR_ENV_T_6_1                  "HL_T_6_1"
#define   CR_ENV_T_6_2                  "HL_T_6_2"
#define   CR_ENV_T_6_3                  "HL_T_6_3"
#define   CR_ENV_T_6_4                  "HL_T_6_4"
#define   CR_ENV_T_6_5                  "HL_T_6_5"
#define   CR_ENV_T_6_6                  "HL_T_6_6"

#define   CR_ENV_T_7_1                  "HL_T_7_1"
#define   CR_ENV_T_7_2                  "HL_T_7_2"
#define   CR_ENV_T_7_3                  "HL_T_7_3"
#define   CR_ENV_T_7_4                  "HL_T_7_4"
#define   CR_ENV_T_7_5                  "HL_T_7_5"
#define   CR_ENV_T_7_6                  "HL_T_7_6"
#define   CR_ENV_T_7_7                  "HL_T_7_7"

#define   CR_ENV_T_8_1                  "HL_T_8_1"
#define   CR_ENV_T_8_2                  "HL_T_8_2"
#define   CR_ENV_T_8_3                  "HL_T_8_3"
#define   CR_ENV_T_8_4                  "HL_T_8_4"
#define   CR_ENV_T_8_5                  "HL_T_8_5"
#define   CR_ENV_T_8_6                  "HL_T_8_6"
#define   CR_ENV_T_8_7                  "HL_T_8_7"
#define   CR_ENV_T_8_8                  "HL_T_8_8"

#define   CR_ENV_T_9_1                  "HL_T_9_1"
#define   CR_ENV_T_9_2                  "HL_T_9_2"
#define   CR_ENV_T_9_3                  "HL_T_9_3"
#define   CR_ENV_T_9_4                  "HL_T_9_4"
#define   CR_ENV_T_9_5                  "HL_T_9_5"
#define   CR_ENV_T_9_6                  "HL_T_9_6"
#define   CR_ENV_T_9_7                  "HL_T_9_7"
#define   CR_ENV_T_9_8                  "HL_T_9_8"
#define   CR_ENV_T_9_9                  "HL_T_9_9"

#define   CR_ENV_T_10_1                 "HL_T_10_1"
#define   CR_ENV_T_10_2                 "HL_T_10_2"
#define   CR_ENV_T_10_3                 "HL_T_10_3"
#define   CR_ENV_T_10_4                 "HL_T_10_4"
#define   CR_ENV_T_10_5                 "HL_T_10_5"
#define   CR_ENV_T_10_6                 "HL_T_10_6"
#define   CR_ENV_T_10_7                 "HL_T_10_7"
#define   CR_ENV_T_10_8                 "HL_T_10_8"
#define   CR_ENV_T_10_9                 "HL_T_10_9"
#define   CR_ENV_T_10_10                "HL_T_10_10"

/* Environment variables for thresholds values
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_ENV_T_V_1                  "HL_T_V_1"
#define   CR_ENV_T_V_2                  "HL_T_V_2"
#define   CR_ENV_T_V_3                  "HL_T_V_3"
#define   CR_ENV_T_V_4                  "HL_T_V_4"
#define   CR_ENV_T_V_5                  "HL_T_V_5"
#define   CR_ENV_T_V_6                  "HL_T_V_6"
#define   CR_ENV_T_V_7                  "HL_T_V_7"
#define   CR_ENV_T_V_8                  "HL_T_V_8"
#define   CR_ENV_T_V_9                  "HL_T_V_9"
#define   CR_ENV_T_V_10                 "HL_T_V_10"

/* Default values for environment variables
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_DEFLT_ALT_REGEXP           "^(.*)$"
#define   CR_DEFLT_CONF_GLOB            "hl_*.cfg:hl.cfg:.hl_*.cfg:.hl.cfg"


#define   CR_DEFLT_DOW_SPEC             "Y2m3d4"
#define   CR_DEFLT_DOW_RE               "(([0-9]{4})-([0-9]{2})-([0-9]{2}))"
#define   CR_DEFLT_DOW_POS_Y            (2)
#define   CR_DEFLT_DOW_POS_m            (3)
#define   CR_DEFLT_DOW_POS_d            (4)

#define   CR_DEFLT_THRES_RE             "([0-9]+)"

#define   CR_DEFLT_THRES_V1             (  0)
#define   CR_DEFLT_THRES_V2             ( 10)
#define   CR_DEFLT_THRES_V3             ( 20)
#define   CR_DEFLT_THRES_V4             ( 30)
#define   CR_DEFLT_THRES_V5             ( 40)
#define   CR_DEFLT_THRES_V6             ( 50)
#define   CR_DEFLT_THRES_V7             ( 60)
#define   CR_DEFLT_THRES_V8             ( 70)
#define   CR_DEFLT_THRES_V9             ( 80)
#define   CR_DEFLT_THRES_V10            (100)


/* Default color
   ~~~~~~~~~~~~~ */
//#define   CR_DEFLT_COLOR                "3Y"
#define   CR_DEFLT_COL_INTENSITY        (3)
#define   CR_DEFLT_COL_COLOR            'Y'

/* Default values for 'alternate' variables
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_DEFLT_ALT_INTENSITY_1      (2)
#define   CR_DEFLT_ALT_COLOR_1          'B'
#define   CR_DEFLT_ALT_INTENSITY_2      (3)
#define   CR_DEFLT_ALT_COLOR_2          'c'

/* Default values for DOW variables
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_DEFLT_DOW_SUNDAY_INTENS    (3)
#define   CR_DEFLT_DOW_SUNDAY_COLOR     'R'

#define   CR_DEFLT_DOW_MONDAY_INTENS    (2)
#define   CR_DEFLT_DOW_MONDAY_COLOR     'b'

#define   CR_DEFLT_DOW_TUESDAY_INTENS   (2)
#define   CR_DEFLT_DOW_TUESDAY_COLOR    'c'

#define   CR_DEFLT_DOW_WEDNESDAY_INTENS (2)
#define   CR_DEFLT_DOW_WEDNESDAY_COLOR  'g'

#define   CR_DEFLT_DOW_THURSDAY_INTENS  (3)
#define   CR_DEFLT_DOW_THURSDAY_COLOR   'g'

#define   CR_DEFLT_DOW_FRIDAY_INTENS    (3)
#define   CR_DEFLT_DOW_FRIDAY_COLOR     'y'

#define   CR_DEFLT_DOW_SATURDAY_INTENS  (3)
#define   CR_DEFLT_DOW_SATURDAY_COLOR   'r'

/* Default values for threshold variables
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* 2 ranges */
#define   CR_DEFLT_T_2_1_INTENS         (3)
#define   CR_DEFLT_T_2_1_COLOR          'g'

#define   CR_DEFLT_T_2_2_INTENS         (3)
#define   CR_DEFLT_T_2_2_COLOR          'r'


/* 3 ranges */
#define   CR_DEFLT_T_3_1_INTENS         (3)
#define   CR_DEFLT_T_3_1_COLOR          'g'

#define   CR_DEFLT_T_3_2_INTENS         (3)
#define   CR_DEFLT_T_3_2_COLOR          'y'

#define   CR_DEFLT_T_3_3_INTENS         (3)
#define   CR_DEFLT_T_3_3_COLOR          'r'


/* 4 ranges */
#define   CR_DEFLT_T_4_1_INTENS         (3)
#define   CR_DEFLT_T_4_1_COLOR          'b'

#define   CR_DEFLT_T_4_2_INTENS         (3)
#define   CR_DEFLT_T_4_2_COLOR          'g'

#define   CR_DEFLT_T_4_3_INTENS         (3)
#define   CR_DEFLT_T_4_3_COLOR          'y'

#define   CR_DEFLT_T_4_4_INTENS         (3)
#define   CR_DEFLT_T_4_4_COLOR          'r'


/* 5 ranges */
#define   CR_DEFLT_T_5_1_INTENS         (3)
#define   CR_DEFLT_T_5_1_COLOR          'b'

#define   CR_DEFLT_T_5_2_INTENS         (3)
#define   CR_DEFLT_T_5_2_COLOR          'g'

#define   CR_DEFLT_T_5_3_INTENS         (3)
#define   CR_DEFLT_T_5_3_COLOR          'y'

#define   CR_DEFLT_T_5_4_INTENS         (2)
#define   CR_DEFLT_T_5_4_COLOR          'r'

#define   CR_DEFLT_T_5_5_INTENS         (3)
#define   CR_DEFLT_T_5_5_COLOR          'r'


/* 6 ranges */
#define   CR_DEFLT_T_6_1_INTENS         (3)
#define   CR_DEFLT_T_6_1_COLOR          'b'

#define   CR_DEFLT_T_6_2_INTENS         (3)
#define   CR_DEFLT_T_6_2_COLOR          'c'

#define   CR_DEFLT_T_6_3_INTENS         (3)
#define   CR_DEFLT_T_6_3_COLOR          'g'

#define   CR_DEFLT_T_6_4_INTENS         (3)
#define   CR_DEFLT_T_6_4_COLOR          'y'

#define   CR_DEFLT_T_6_5_INTENS         (3)
#define   CR_DEFLT_T_6_5_COLOR          'r'

#define   CR_DEFLT_T_6_6_INTENS         (3)
#define   CR_DEFLT_T_6_6_COLOR          'w'


/* 7 ranges */
#define   CR_DEFLT_T_7_1_INTENS         (3)
#define   CR_DEFLT_T_7_1_COLOR          'b'

#define   CR_DEFLT_T_7_2_INTENS         (3)
#define   CR_DEFLT_T_7_2_COLOR          'c'

#define   CR_DEFLT_T_7_3_INTENS         (3)
#define   CR_DEFLT_T_7_3_COLOR          'g'

#define   CR_DEFLT_T_7_4_INTENS         (3)
#define   CR_DEFLT_T_7_4_COLOR          'y'

#define   CR_DEFLT_T_7_5_INTENS         (3)
#define   CR_DEFLT_T_7_5_COLOR          'r'

#define   CR_DEFLT_T_7_6_INTENS         (3)
#define   CR_DEFLT_T_7_6_COLOR          'm'

#define   CR_DEFLT_T_7_7_INTENS         (3)
#define   CR_DEFLT_T_7_7_COLOR          'w'


/* 8 ranges */
#define   CR_DEFLT_T_8_1_INTENS         (2)
#define   CR_DEFLT_T_8_1_COLOR          'b'

#define   CR_DEFLT_T_8_2_INTENS         (3)
#define   CR_DEFLT_T_8_2_COLOR          'b'

#define   CR_DEFLT_T_8_3_INTENS         (3)
#define   CR_DEFLT_T_8_3_COLOR          'c'

#define   CR_DEFLT_T_8_4_INTENS         (3)
#define   CR_DEFLT_T_8_4_COLOR          'g'

#define   CR_DEFLT_T_8_5_INTENS         (3)
#define   CR_DEFLT_T_8_5_COLOR          'y'

#define   CR_DEFLT_T_8_6_INTENS         (3)
#define   CR_DEFLT_T_8_6_COLOR          'r'

#define   CR_DEFLT_T_8_7_INTENS         (3)
#define   CR_DEFLT_T_8_7_COLOR          'm'

#define   CR_DEFLT_T_8_8_INTENS         (3)
#define   CR_DEFLT_T_8_8_COLOR          'w'


/* 9 ranges */
#define   CR_DEFLT_T_9_1_INTENS         (2)
#define   CR_DEFLT_T_9_1_COLOR          'b'

#define   CR_DEFLT_T_9_2_INTENS         (3)
#define   CR_DEFLT_T_9_2_COLOR          'b'

#define   CR_DEFLT_T_9_3_INTENS         (3)
#define   CR_DEFLT_T_9_3_COLOR          'c'

#define   CR_DEFLT_T_9_4_INTENS         (2)
#define   CR_DEFLT_T_9_4_COLOR          'g'

#define   CR_DEFLT_T_9_5_INTENS         (3)
#define   CR_DEFLT_T_9_5_COLOR          'g'

#define   CR_DEFLT_T_9_6_INTENS         (3)
#define   CR_DEFLT_T_9_6_COLOR          'y'

#define   CR_DEFLT_T_9_7_INTENS         (3)
#define   CR_DEFLT_T_9_7_COLOR          'r'

#define   CR_DEFLT_T_9_8_INTENS         (3)
#define   CR_DEFLT_T_9_8_COLOR          'm'

#define   CR_DEFLT_T_9_9_INTENS         (3)
#define   CR_DEFLT_T_9_9_COLOR          'w'


/* 10 ranges */
#define   CR_DEFLT_T_10_1_INTENS        (2)
#define   CR_DEFLT_T_10_1_COLOR         'b'

#define   CR_DEFLT_T_10_2_INTENS        (3)
#define   CR_DEFLT_T_10_2_COLOR         'b'

#define   CR_DEFLT_T_10_3_INTENS        (3)
#define   CR_DEFLT_T_10_3_COLOR         'c'

#define   CR_DEFLT_T_10_4_INTENS        (2)
#define   CR_DEFLT_T_10_4_COLOR         'g'

#define   CR_DEFLT_T_10_5_INTENS        (3)
#define   CR_DEFLT_T_10_5_COLOR         'g'

#define   CR_DEFLT_T_10_6_INTENS        (3)
#define   CR_DEFLT_T_10_6_COLOR         'y'

#define   CR_DEFLT_T_10_7_INTENS        (2)
#define   CR_DEFLT_T_10_7_COLOR         'y'

#define   CR_DEFLT_T_10_8_INTENS        (3)
#define   CR_DEFLT_T_10_8_COLOR         'r'

#define   CR_DEFLT_T_10_9_INTENS        (3)
#define   CR_DEFLT_T_10_9_COLOR         'm'

#define   CR_DEFLT_T_10_10_INTENS       (3)
#define   CR_DEFLT_T_10_10_COLOR        'w'


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

#define   CR_STR_COLORS                 ".rgybmcw.RGYBMCWn"

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

#define   CR_DISP_LEX(...)              if (G.disp_lex) fprintf(G.debug_out, __VA_ARGS__)
#define   CR_DEBUG(...)                 if (G.debug) {                                                   \
                                             fprintf(G.debug_out, "%-20s (%4d) ", __func__, __LINE__);   \
                                             fprintf(G.debug_out, __VA_ARGS__);                          \
                                        }
#define   CR_DEBUG2(...)                if (G.debug >= 2) {                                              \
                                             fprintf(G.debug_out, "%-20s (%4d) ", __func__, __LINE__);   \
                                             fprintf(G.debug_out, __VA_ARGS__);                          \
                                        }

#define   CR_ENTERING                   CR_DEBUG("Entering %s()\n",  __func__)
#define   CR_LEAVING                    CR_DEBUG("Leaving %s()\n\n", __func__)

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

/* Parser states for the decoding of option -A, -I, -J
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CR_STATE_INITIAL              ( 1)
#define   CR_STATE_W_PARAM              ( 2)
#define   CR_STATE_W_BASE               ( 3)
#define   CR_STATE_W_SEPARATOR_2        ( 4)
#define   CR_STATE_W_SELECTOR_ID        ( 5)
#define   CR_STATE_W_SEPARATOR          ( 6)
#define   CR_STATE_W_INTENSITY          ( 7)
#define   CR_STATE_W_INTENSITY2         ( 8)
#define   CR_STATE_W_COLOR              ( 9)
#define   CR_STATE_W_NUM                (10)
#define   CR_STATE_W_DMY                (11)
#define   CR_STATE_W_DMY2               (12)
#define   CR_STATE_W_EQUAL              (13)
#define   CR_STATE_W_COMMA              (14)
#define   CR_STATE_W_SIGN               (15)
#define   CR_STATE_W_DIGIT              (16)
#define   CR_STATE_W_COLUMN             (17)
#define   CR_STATE_W_COMMA2             (18)
#define   CR_STATE_W_NO_COLOR           (19)
#define   CR_STATE_W_END                (20)
#define   CR_STATE_W_OPTION             (21)
#define   CR_STATE_W_CONFIG             (22)
#define   CR_STATE_W_NEXT_CHAR          (23)
#define   CR_STATE_W_NEXT_ARG           (24)
#define   CR_STATE_W_RAW_CHAR           (25)
#define   CR_STATE_FINAL                (26)
#define   CR_STATE_ERR_FOLLOWING        (27)
#define   CR_STATE_ERR_MISSING_ARG      (28)

#define   CR_UNINITIALIZED              (-1)
#define   CR_MAX_DAYS                   ( 7)
#define   CR_MAX_THRESHOLDS             (10)

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

/* Exit codes
   ~~~~~~~~~~ */
#define   CR_EXIT_OK                    (  0)
#define   CR_EXIT_ERR_SYNTAX            (  1)
#define   CR_EXIT_ERR_MALLOC            (  2)
#define   CR_EXIT_ERR_OPEN              (  3)
#define   CR_EXIT_ERR_GLOB              (  4)
#define   CR_EXIT_ERR_UNDEFINED_VAR     (  5)
#define   CR_EXIT_ERR_REGCOMP           (  6)
#define   CR_EXIT_ERR_DUP_VALUE         (  7)
#define   CR_EXIT_ERR_UNKNOWN_CONF      (  8)
#define   CR_EXIT_ERR_CONF_LOOP         (  9)
#define   CR_EXIT_ERR_SIGNAL            ( 10)
#define   CR_EXIT_ERR_VERSION           ( 11)
#define   CR_EXIT_ERR_MARKER            ( 12)
#define   CR_EXIT_ERR_USAGE             ( 13)

#define   CR_EXIT_ERR_SEGV              (125)
#define   CR_EXIT_ERR_INTERNAL          (126)

/* Structures
   ~~~~~~~~~~ */
struct cr_color {
     int                                 col_num;
     int                                 intensity;
     FILE                               *out;
};

struct cr_RE_dow_desc {
     struct cr_color                   **cols;                   /* Array for DOW colors       */
     int                                 idx;                    /* Current DOW index          */
     bool                                used;                   /* Colorize day of week       */
     int                                 year_RE_num;            /* Number of sub-RE for year  */
     int                                 month_RE_num;           /* Number of sub-RE for month */
     int                                 day_RE_num;             /* Number of sub-RE for day   */
};

struct cr_RE_threshold_desc {
     struct cr_color                   **cols;                   /* Array for threshold colors */
     int                                 idx;                    /* Current threshold index    */
     bool                                used;                   /* Colorize thresholds        */
     double                              threshold[CR_MAX_THRESHOLDS];
                                                                 /* Thresholds array           */
     int                                 nb_thresholds;          /* Number of thresholds       */
     int                                 base;                   /* decimal or hexadecimal     */
};

/* Regexp descriptor
   ~~~~~~~~~~~~~~~~~ */
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
     struct cr_RE_dow_desc               dow;                    /* DOW descriptor             */
     struct cr_RE_threshold_desc         threshold;              /* Thresholds descriptor      */
     int                                 idx_regex_select;       /* Index of selector regex    */
     char                               *matching_str;           /* String matching the        */
                                                                 /* selector regex             */
     bool                                alternate,              /* Alternate                  */
                                         sequential,             /* Sequential                 */
                                         ind_color;              /* Indirect color             */
     bool                                change_on_diff;         /* Change on difference       */
     bool                                change_on_bad_next;     /* Change on bad next value   */
     long                                val;                    /* Last value for seq control */
     long                                param;                  /* Parameter for seq control  */
     int                                 op;                     /* Operation for seq control  */
     int                                 base;                   /* Base for seq control       */
};

/* Color descriptor
   ~~~~~~~~~~~~~~~~ */
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

struct cr_args {
     int                                 argc;
     char                              **argv;
     char                              **argp;         // Pointer to the address
                                                       // of the current argument
     int                                 idx;          // Index of the current 
                                                       // character
     char                               *next_arg;
     struct cr_args                     *prev;
     struct cr_config                   *config;
     int                                 level;
//     char                              **p,
     char                              **regexp;
};
typedef struct cr_args                   cr_args;

struct cr_root_args {
     char                               *opts;
     char                               *optarg;
     cr_args                            *args;
     struct cr_configs                  *configs;
     int                                 state;
};
typedef struct cr_root_args              cr_root_args;

/* Descriptor for  string copy, one character at a time
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
struct cr_str {
     char                               *s;
     char                               *e;
};

/* Environment variable descriptor for configuration
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
struct cr_env_var_conf {
     char                              **var,
                                        *name,
                                        *deflt_value,
                                        *var_value;
};

/* Environment variable descriptor for colors
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
struct cr_env_var_desc {
     struct cr_color                   **color_desc;
     bool                                skip_line;
     char                               *name;
     char                                deflt_intens,
                                         deflt_color;
     int                                 intensity;
     char                                color;
};

/* Environment variable descriptor for thresholds
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
struct cr_env_var_thres {
     char                               *name;
     bool                                skip_line;
     double                             *value;
     double                              deflt_value;
     bool                                used;
};

/* Global structure descriptor
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
struct cr_global {
     char                               *prgname;

     char                               *selector_string,
                                        *color_string,
                                        *intensity_string,
                                        *int_string,
                                        *op_string,
                                        *base_string;
     char                               *cfg_filename;
     struct cr_color                     color_RE[CR_NB_COLORS],
                                        *curr_col,
                                        *deflt_color,
                                        *deflt_alt_col_1,
                                        *deflt_alt_col_2,
                                        *deflt_dow[7],
                                        *deflt_t[CR_SUM(10)];
     double                              deflt_t_v[CR_MAX_THRESHOLDS];
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
                                        *debug_out,
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

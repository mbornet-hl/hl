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
 *   Description  :     Private global variables
 *
 *   File         :     cr_gpri.c
 *
 *	@(#)	[MB] cr_gpri.c	Version 1.21 du 22/01/20 - 
 *
 * Sources from the original hl command are available on :
 * https://github.com/mbornet-hl/hl
 * ============================================================================
 */

#include "cr_epri.h"

struct cr_global                                  G         = { 0 };

/* Error messages
   ~~~~~~~~~~~~~~ */
char                *cr_err_malloc      = "%s: cannot allocate memory !\n",
                    *cr_err_syntax      = "%s: syntax error !\n",
                    *cr_err_syntax_opt  = "%s: syntax error on '%c' ! [%s() - %s(%d)]\n";

int                  cr_col_codes[8][3] = {
     {  88,    160,      196  },
     {  28,     34,       46  },
     { 100,    142,      226  },
     {  18,     26,       38  },
     {  90,     91,      134  },
     {  61,     62,       81  },
     { 245,    252,      255  },
     {   0,      0,        0  }
};

char                *cr_best_fg[8][3] = {
     { "1;37", "1;37",   "1;37"    },
     { "1;37", "1;37",   "2;31"    },
     { "1;37", "2;30",   "2;30"    },
     { "1;37", "1;37",   "2;30"    },
     { "1;37", "1;37",   "1;37"    },
     { "1;37", "1;37",   "2;30"    },
     { "1;37", "2;30",   "2;30"    },
     {      0,      0,        0    }
};

/* Environment variables list
   ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
struct cr_env_var_desc                            cr_env_vars[] = {
     { &G.deflt_alt_col_1,          FALSE,
       CR_ENV_ALTERNATE_1,          CR_DEFLT_ALT_INTENSITY_1,          CR_DEFLT_ALT_COLOR_1,           0,   0    },
     { &G.deflt_alt_col_2,          FALSE,
       CR_ENV_ALTERNATE_2,          CR_DEFLT_ALT_INTENSITY_2,          CR_DEFLT_ALT_COLOR_2,           0,   0    },

     { &G.deflt_dow[0],             TRUE,
       CR_ENV_DOW_SUNDAY,           CR_DEFLT_DOW_SUNDAY_INTENS,        CR_DEFLT_DOW_SUNDAY_COLOR,      0,   0    },
     { &G.deflt_dow[1],             FALSE,
       CR_ENV_DOW_MONDAY,           CR_DEFLT_DOW_MONDAY_INTENS,        CR_DEFLT_DOW_MONDAY_COLOR,      0,   0    },
     { &G.deflt_dow[2],             FALSE,
       CR_ENV_DOW_TUESDAY,          CR_DEFLT_DOW_TUESDAY_INTENS,       CR_DEFLT_DOW_TUESDAY_COLOR,     0,   0    },
     { &G.deflt_dow[3],             FALSE,
       CR_ENV_DOW_WEDNESDAY,        CR_DEFLT_DOW_WEDNESDAY_INTENS,     CR_DEFLT_DOW_WEDNESDAY_COLOR,   0,   0    },
     { &G.deflt_dow[4],             FALSE,
       CR_ENV_DOW_THURSDAY,         CR_DEFLT_DOW_THURSDAY_INTENS,      CR_DEFLT_DOW_THURSDAY_COLOR,    0,   0    },
     { &G.deflt_dow[5],             FALSE,
       CR_ENV_DOW_FRIDAY,           CR_DEFLT_DOW_FRIDAY_INTENS,        CR_DEFLT_DOW_FRIDAY_COLOR,      0,   0    },
     { &G.deflt_dow[6],             FALSE,
       CR_ENV_DOW_SATURDAY,         CR_DEFLT_DOW_SATURDAY_INTENS,      CR_DEFLT_DOW_SATURDAY_COLOR,    0,   0    },

     { &G.deflt_t[CR_IDX(2, 1)],    TRUE,
       CR_ENV_T_2_1,                CR_DEFLT_T_2_1_INTENS,             CR_DEFLT_T_2_1_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(2, 2)],    FALSE,
       CR_ENV_T_2_2,                CR_DEFLT_T_2_2_INTENS,             CR_DEFLT_T_2_2_COLOR,           0,   0    }, 

     { &G.deflt_t[CR_IDX(3, 1)],    TRUE,
       CR_ENV_T_3_1,                CR_DEFLT_T_3_1_INTENS,             CR_DEFLT_T_3_1_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(3, 2)],    FALSE,
       CR_ENV_T_3_2,                CR_DEFLT_T_3_2_INTENS,             CR_DEFLT_T_3_2_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(3, 3)],    FALSE,
       CR_ENV_T_3_3,                CR_DEFLT_T_3_3_INTENS,             CR_DEFLT_T_3_3_COLOR,           0,   0    }, 

     { &G.deflt_t[CR_IDX(4, 1)],    TRUE,
       CR_ENV_T_4_1,                CR_DEFLT_T_4_1_INTENS,             CR_DEFLT_T_4_1_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(4, 2)],    FALSE,
       CR_ENV_T_4_2,                CR_DEFLT_T_4_2_INTENS,             CR_DEFLT_T_4_2_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(4, 3)],    FALSE,
       CR_ENV_T_4_3,                CR_DEFLT_T_4_3_INTENS,             CR_DEFLT_T_4_3_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(4, 4)],    FALSE,
       CR_ENV_T_4_4,                CR_DEFLT_T_4_4_INTENS,             CR_DEFLT_T_4_4_COLOR,           0,   0    }, 

     { &G.deflt_t[CR_IDX(5, 1)],    TRUE,
       CR_ENV_T_5_1,                CR_DEFLT_T_5_1_INTENS,             CR_DEFLT_T_5_1_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(5, 2)],    FALSE,
       CR_ENV_T_5_2,                CR_DEFLT_T_5_2_INTENS,             CR_DEFLT_T_5_2_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(5, 3)],    FALSE,
       CR_ENV_T_5_3,                CR_DEFLT_T_5_3_INTENS,             CR_DEFLT_T_5_3_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(5, 4)],    FALSE,
       CR_ENV_T_5_4,                CR_DEFLT_T_5_4_INTENS,             CR_DEFLT_T_5_4_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(5, 5)],    FALSE,
       CR_ENV_T_5_5,                CR_DEFLT_T_5_5_INTENS,             CR_DEFLT_T_5_5_COLOR,           0,   0    }, 

     { &G.deflt_t[CR_IDX(6, 1)],    TRUE,
       CR_ENV_T_6_1,                CR_DEFLT_T_6_1_INTENS,             CR_DEFLT_T_6_1_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(6, 2)],    FALSE,
       CR_ENV_T_6_2,                CR_DEFLT_T_6_2_INTENS,             CR_DEFLT_T_6_2_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(6, 3)],    FALSE,
       CR_ENV_T_6_3,                CR_DEFLT_T_6_3_INTENS,             CR_DEFLT_T_6_3_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(6, 4)],    FALSE,
       CR_ENV_T_6_4,                CR_DEFLT_T_6_4_INTENS,             CR_DEFLT_T_6_4_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(6, 5)],    FALSE,
       CR_ENV_T_6_5,                CR_DEFLT_T_6_5_INTENS,             CR_DEFLT_T_6_5_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(6, 6)],    FALSE,
       CR_ENV_T_6_6,                CR_DEFLT_T_6_6_INTENS,             CR_DEFLT_T_6_6_COLOR,           0,   0    }, 

     { &G.deflt_t[CR_IDX(7, 1)],    TRUE,
       CR_ENV_T_7_1,                CR_DEFLT_T_7_1_INTENS,             CR_DEFLT_T_7_1_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(7, 2)],    FALSE,
       CR_ENV_T_7_2,                CR_DEFLT_T_7_2_INTENS,             CR_DEFLT_T_7_2_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(7, 3)],    FALSE,
       CR_ENV_T_7_3,                CR_DEFLT_T_7_3_INTENS,             CR_DEFLT_T_7_3_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(7, 4)],    FALSE,
       CR_ENV_T_7_4,                CR_DEFLT_T_7_4_INTENS,             CR_DEFLT_T_7_4_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(7, 5)],    FALSE,
       CR_ENV_T_7_5,                CR_DEFLT_T_7_5_INTENS,             CR_DEFLT_T_7_5_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(7, 6)],    FALSE,
       CR_ENV_T_7_6,                CR_DEFLT_T_7_6_INTENS,             CR_DEFLT_T_7_6_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(7, 7)],    FALSE,
       CR_ENV_T_7_7,                CR_DEFLT_T_7_7_INTENS,             CR_DEFLT_T_7_7_COLOR,           0,   0    }, 

     { &G.deflt_t[CR_IDX(8, 1)],    TRUE,
       CR_ENV_T_8_1,                CR_DEFLT_T_8_1_INTENS,             CR_DEFLT_T_8_1_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(8, 2)],    FALSE,
       CR_ENV_T_8_2,                CR_DEFLT_T_8_2_INTENS,             CR_DEFLT_T_8_2_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(8, 3)],    FALSE,
       CR_ENV_T_8_3,                CR_DEFLT_T_8_3_INTENS,             CR_DEFLT_T_8_3_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(8, 4)],    FALSE,
       CR_ENV_T_8_4,                CR_DEFLT_T_8_4_INTENS,             CR_DEFLT_T_8_4_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(8, 5)],    FALSE,
       CR_ENV_T_8_5,                CR_DEFLT_T_8_5_INTENS,             CR_DEFLT_T_8_5_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(8, 6)],    FALSE,
       CR_ENV_T_8_6,                CR_DEFLT_T_8_6_INTENS,             CR_DEFLT_T_8_6_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(8, 7)],    FALSE,
       CR_ENV_T_8_7,                CR_DEFLT_T_8_7_INTENS,             CR_DEFLT_T_8_7_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(8, 8)],    FALSE,
       CR_ENV_T_8_8,                CR_DEFLT_T_8_8_INTENS,             CR_DEFLT_T_8_8_COLOR,           0,   0    }, 

     { &G.deflt_t[CR_IDX(9, 1)],    TRUE,
       CR_ENV_T_9_1,                CR_DEFLT_T_9_1_INTENS,             CR_DEFLT_T_9_1_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(9, 2)],    FALSE,
       CR_ENV_T_9_2,                CR_DEFLT_T_9_2_INTENS,             CR_DEFLT_T_9_2_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(9, 3)],    FALSE,
       CR_ENV_T_9_3,                CR_DEFLT_T_9_3_INTENS,             CR_DEFLT_T_9_3_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(9, 4)],    FALSE,
       CR_ENV_T_9_4,                CR_DEFLT_T_9_4_INTENS,             CR_DEFLT_T_9_4_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(9, 5)],    FALSE,
       CR_ENV_T_9_5,                CR_DEFLT_T_9_5_INTENS,             CR_DEFLT_T_9_5_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(9, 6)],    FALSE,
       CR_ENV_T_9_6,                CR_DEFLT_T_9_6_INTENS,             CR_DEFLT_T_9_6_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(9, 7)],    FALSE,
       CR_ENV_T_9_7,                CR_DEFLT_T_9_7_INTENS,             CR_DEFLT_T_9_7_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(9, 8)],    FALSE,
       CR_ENV_T_9_8,                CR_DEFLT_T_9_8_INTENS,             CR_DEFLT_T_9_8_COLOR,           0,   0    }, 
     { &G.deflt_t[CR_IDX(9, 9)],    FALSE,
       CR_ENV_T_9_9,                CR_DEFLT_T_9_9_INTENS,             CR_DEFLT_T_9_9_COLOR,           0,   0    }, 

     { &G.deflt_t[CR_IDX(10, 1)],    TRUE,
       CR_ENV_T_10_1,               CR_DEFLT_T_10_1_INTENS,            CR_DEFLT_T_10_1_COLOR,          0,   0    }, 
     { &G.deflt_t[CR_IDX(10, 2)],   FALSE,
       CR_ENV_T_10_2,               CR_DEFLT_T_10_2_INTENS,            CR_DEFLT_T_10_2_COLOR,          0,   0    }, 
     { &G.deflt_t[CR_IDX(10, 3)],   FALSE,
       CR_ENV_T_10_3,               CR_DEFLT_T_10_3_INTENS,            CR_DEFLT_T_10_3_COLOR,          0,   0    }, 
     { &G.deflt_t[CR_IDX(10, 4)],   FALSE,
       CR_ENV_T_10_4,               CR_DEFLT_T_10_4_INTENS,            CR_DEFLT_T_10_4_COLOR,          0,   0    }, 
     { &G.deflt_t[CR_IDX(10, 5)],   FALSE,
       CR_ENV_T_10_5,               CR_DEFLT_T_10_5_INTENS,            CR_DEFLT_T_10_5_COLOR,          0,   0    }, 
     { &G.deflt_t[CR_IDX(10, 6)],   FALSE,
       CR_ENV_T_10_6,               CR_DEFLT_T_10_6_INTENS,            CR_DEFLT_T_10_6_COLOR,          0,   0    }, 
     { &G.deflt_t[CR_IDX(10, 7)],   FALSE,
       CR_ENV_T_10_7,               CR_DEFLT_T_10_7_INTENS,            CR_DEFLT_T_10_7_COLOR,          0,   0    }, 
     { &G.deflt_t[CR_IDX(10, 8)],   FALSE,
       CR_ENV_T_10_8,               CR_DEFLT_T_10_8_INTENS,            CR_DEFLT_T_10_8_COLOR,          0,   0    }, 
     { &G.deflt_t[CR_IDX(10, 9)],   FALSE,
       CR_ENV_T_10_9,               CR_DEFLT_T_10_9_INTENS,            CR_DEFLT_T_10_9_COLOR,          0,   0    }, 
     { &G.deflt_t[CR_IDX(10, 10)],  FALSE,
       CR_ENV_T_10_10,              CR_DEFLT_T_10_10_INTENS,           CR_DEFLT_T_10_10_COLOR,         0,   0    }, 
     { NULL,                        0,                                 0,                              0,   0    }
};

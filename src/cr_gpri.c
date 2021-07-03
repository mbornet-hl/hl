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
 *   (C) Copyright Martial Bornet, 2015-2019.
 *
 *   Author       :     Martial BORNET (MB) - 3rd of January, 2015
 *
 *   Description  :     Private global variables
 *
 *   File         :     cr_gpri.c
 *
 *   @(#)  [MB] cr_gpri.c Version 1.16 du 21/07/03 - 
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


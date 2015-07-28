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
 *   Description  :     Externes privees
 *
 *   Fichier      :     cr_epri.h
 *
 *   @(#)  [MB] cr_epri.h Version 1.15 du 15/07/28 - 
 *
 * ============================================================================
 */

#if ! defined(CR_EPRI_H)
# define CR_EPRI_H

#include "cr_cpri.h"

extern struct cr_global                                G;

extern FILE                                            *yyin;

/* Error messages
   ~~~~~~~~~~~~~~ */
extern char                                            *cr_err_malloc;

extern int                                              cr_col_codes[8][3];
extern char                                            *cr_best_fg[8][3];

/* Fonctions
 * ~~~~~~~~~ */
void                                     cr_usage(bool);
void                                     cr_display_args(struct cr_config *);
void                                     cr_display_config(void);
void                                     cr_init_list(void);
void                                     cr_init_col_names(void);
CR_DECL_NEW(config);
CR_DECL_NEW(arg);
void                                     cr_add_config(struct cr_config *);
void                                     cr_add_arg(struct cr_arg *);
void                                     cr_set_color(int, char *);
void                                     cr_free_RE(void);
void                                     cr_read_input(void);
void                                     cr_disp_color(int, char *);
void                                     cr_start_color(struct cr_color *);
void                                     cr_end_color(struct cr_color *);
void                                     cr_init_desc(void);
void                                     cr_set_desc(struct cr_color *, int, int, int);
void                                     cr_disp_line(void);

int                                      yylex(void);

#endif    /* CR_EPRI_H */

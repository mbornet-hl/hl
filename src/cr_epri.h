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
 *   (C) Copyright Martial Bornet, 2015-2022
 *
 *   Author       :     Martial BORNET (MB) - 3rd of January, 2015
 *
 *   Description  :     Private externals
 *
 *   File         :     cr_epri.h
 *
 *   @(#)  [MB] cr_epri.h Version 1.34 du 22/08/15 - 
 *
 * Sources from the original hl command are available on :
 * https://github.com/mbornet-hl/hl
 *
 * ============================================================================
 */

#if ! defined(CR_EPRI_H)
# define CR_EPRI_H

#include "cr_cpri.h"

extern struct cr_global                                G;

extern FILE                                            *yyin;

extern struct cr_month                                  cr_months[CR_NB_MONTHS];

/* Error messages
   ~~~~~~~~~~~~~~ */
extern char                                            *cr_err_malloc,
                                                       *cr_err_syntax,
                                                       *cr_err_syntax_opt;

extern int                                              cr_col_codes[8][3];
extern char                                            *cr_best_fg[8][3];
extern struct cr_env_var_conf                           cr_env_vars_cfg[];
extern struct cr_env_var_desc                           cr_env_vars[];
extern struct cr_env_var_thres                          cr_env_thres[];

extern char                                            *cr_env_dow_spec,
                                                       *cr_env_dow_RE,

                                                       *cr_env_thres_RE,

                                                       *cr_env_time_RE_Y,
                                                       *cr_env_time_RE_m,
                                                       *cr_env_time_RE_d,
                                                       *cr_env_time_RE_H,
                                                       *cr_env_time_RE_M,
                                                       *cr_env_time_RE_S,
                                                       *cr_env_time_RE_u,
                                                       *cr_env_time_RE_n,

                                                       *cr_env_time_spec_Y,
                                                       *cr_env_time_spec_m,
                                                       *cr_env_time_spec_d,
                                                       *cr_env_time_spec_H,
                                                       *cr_env_time_spec_M,
                                                       *cr_env_time_spec_S,
                                                       *cr_env_time_spec_u,
                                                       *cr_env_time_spec_n;

extern char                        				*cr_time_period_labels[];

/* Functions
 * ~~~~~~~~~ */
void                                     cr_usage(bool);
void                                     cr_display_args(struct cr_config *);
void                                     cr_init_list(void);
void                                     cr_init_col_names(void);
CR_DECL_NEW(config);
CR_DECL_NEW(arg);
void                                     cr_add_config(struct cr_config *);
void                                     cr_add_arg(struct cr_arg *, bool);
void                                     cr_set_color(int, char *);
void                                     cr_free_RE(void);
void                                     cr_read_input(void);
void                                     cr_disp_color(int, char *);
void                                     cr_start_color(struct cr_color *);
void                                     cr_end_color(struct cr_color *);
void                                     cr_init_desc(void);
void                                     cr_disp_line(void);
struct cr_color                         *cr_get_deflt_alt_col(char *, int, int);

int                                      yylex(void);

#endif    /* CR_EPRI_H */

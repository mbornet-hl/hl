/* ============================================================================
 * Copyright (C) 2015-2023, Martial Bornet
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
 *   (C) Copyright Martial Bornet, 2015-2025.
 *
 *   Author       :     Martial BORNET (MB) - 3rd of January, 2015
 *
 *   Description  :     String colorization
 *
 *   File         :     cr_main.c
 *
 *   @(#)  [MB] cr_main.c Version 1.174 du 25/05/08 - 
 *
 * Sources from the original hl command are available on :
 * https://github.com/mbornet-hl/hl
 *
 */

#define   HL_BACKTRACE

/* includes {{{ */
#if defined(HL_BACKTRACE)
#define _GNU_SOURCE
#endif    /* HL_BACKTRACE */

#if ! defined(__APPLE__)
#define _POSIX_C_SOURCE 199309L
#endif    /* __APPLE__ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <glob.h>
#include <sys/time.h>
#include <time.h>
#include <locale.h>
#include <stdint.h>
#include <inttypes.h>
#if defined(HL_BACKTRACE)
#include <signal.h>
#include <execinfo.h>
#endif    /* HL_BACKTRACE */

#include "cr_epri.h"
/* includes }}} */

/* macros {{{ */
#define   X0     fprintf(G.debug_out, "%s(%d)\n", __FILE__, __LINE__);
#define   X      if (G.debug) fprintf(G.debug_out, "%-20s (%4d)\n", __FILE__, __LINE__);
#define   SC     if (G.debug) fprintf(G.debug_out, "==> %s(%d) Start color : [%c]\n", __FILE__, __LINE__, _c);
#define   NC     if (G.debug) fprintf(G.debug_out, "==> %s(%d) No color    : [%c]\n", __FILE__, __LINE__, _c);
#define   EC     if (G.debug) fprintf(G.debug_out, "==> %s(%d) End color\n", __FILE__, __LINE__);

#define   inline /* empty : for compilers that do not know the inline directive */
/* macros }}} */
#define   NB_MATCH  12

/* cr_error_malloc() {{{ */
/******************************************************************************

                         CR_ERROR_MALLOC

******************************************************************************/
void cr_error_malloc()
{
     fprintf(stderr, cr_err_malloc, G.prgname);
}

/* cr_error_malloc() }}} */
/* cr_init_tm() {{{ */

/******************************************************************************

                         CR_INIT_TM

******************************************************************************/
void cr_init_tm(struct tm *tm)
{
     bzero(tm, sizeof(*tm));
     tm->tm_mday              = 1;
}

/* cr_new_tm() }}} */
/* cr_new_tm() {{{ */

/******************************************************************************

                         CR_NEW_TM

******************************************************************************/
struct tm *cr_new_tm()
{
     struct tm                *_tm;

     if ((_tm = malloc(sizeof(*_tm))) == NULL) {
          cr_error_malloc();
          exit(CR_EXIT_ERR_MALLOC);
     }

     cr_init_tm(_tm);

     return _tm;
}

/* cr_new_tm() }}} */
/* cr_disp_tm() {{{ */

/******************************************************************************

                         CR_DISP_TM

******************************************************************************/
void cr_disp_tm(struct tm *tm)
{
     printf("tm->tm_year   = %4d (%d)\n",    tm->tm_year + 1900, tm->tm_year);
     printf("tm->tm_mon    =   %02d (%d)\n", tm->tm_mon + 1, tm->tm_mon);
     printf("tm->tm_mday   =   %02d\n",      tm->tm_mday);
     printf("tm->tm_hour   =   %02d\n",      tm->tm_hour);
     printf("tm->tm_min    =   %02d\n",      tm->tm_min);
     printf("tm->tm_sec    =   %02d\n",      tm->tm_sec);
     printf("tm->tm_wday   = %3d\n",         tm->tm_wday);
     printf("tm->tm_yday   = %3d\n",         tm->tm_yday);
     printf("tm->tm_isdst  = %3d\n",         tm->tm_isdst);
}

/* cr_disp_tm() }}} */
/* cr_disp_time_desc() {{{ */

/******************************************************************************

                         CR_DISP_TIME_DESC

******************************************************************************/
void cr_disp_time_desc(cr_time_desc *time_desc)
{
     printf("nsec_epoch    = %19ld\n",       time_desc->nsec_epoch);
     printf("tspec.tv_sec  = %10ld\n",       time_desc->tspec.tv_sec);
     printf("tspec.tv_nsec =           %09ld\n",
                                             time_desc->tspec.tv_nsec);
     printf("tm->tm_year   = %4d (%d)\n",    time_desc->tm->tm_year + 1900, time_desc->tm->tm_year);
     printf("tm->tm_mon    =   %02d (%d)\n", time_desc->tm->tm_mon + 1, time_desc->tm->tm_mon);
     printf("tm->tm_mday   =   %02d\n",      time_desc->tm->tm_mday);
     printf("tm->tm_hour   =   %02d\n",      time_desc->tm->tm_hour);
     printf("tm->tm_min    =   %02d\n",      time_desc->tm->tm_min);
     printf("tm->tm_sec    =   %02d\n",      time_desc->tm->tm_sec);
     printf("tm->tm_wday   = %3d\n",         time_desc->tm->tm_wday);
     printf("tm->tm_yday   = %3d\n",         time_desc->tm->tm_yday);
     printf("tm->tm_isdst  = %3d\n",         time_desc->tm->tm_isdst);
     printf("%4d-%02d-%02d %02d:%02d:%02d.%09ld\n",
            time_desc->tm->tm_year + 1900, time_desc->tm->tm_mon + 1, time_desc->tm->tm_mday,
            time_desc->tm->tm_hour, time_desc->tm->tm_min, time_desc->tm->tm_sec,
            time_desc->tspec.tv_nsec);
}

/* cr_disp_time_desc() }}} */
/* cr_str_color() {{{ */

/******************************************************************************

                              CR_STR_COLOR

******************************************************************************/
char *cr_str_color(int intensity, int color_num)
{
     char                 _str[4];

     _str[0]             = (intensity % 10) + '0';
     _str[1]             = CR_STR_COLORS[color_num];
     _str[2]             = '\0';

     return strdup(_str);
}

/* cr_str_color() }}} */
/* cr_tm_copy() {{{ */

/******************************************************************************

                         CR_TM_COPY

******************************************************************************/
void cr_tm_copy(struct tm *tm_dst, struct tm *tm_src)
{
     tm_dst->tm_year          = tm_src->tm_year;
     tm_dst->tm_mon           = tm_src->tm_mon;
     tm_dst->tm_yday          = tm_src->tm_yday;
     tm_dst->tm_mday          = tm_src->tm_mday;
     tm_dst->tm_wday          = tm_src->tm_wday;
     tm_dst->tm_hour          = tm_src->tm_hour;
     tm_dst->tm_min           = tm_src->tm_min;
     tm_dst->tm_sec           = tm_src->tm_sec;
     tm_dst->tm_isdst         = tm_src->tm_isdst;
}

/* cr_tm_copy() }}} */
/* cr_time_desc_copy() {{{ */

/******************************************************************************

                         CR_TIME_DESC_COPY

******************************************************************************/
void cr_time_desc_copy(cr_time_desc *dst, cr_time_desc *src)
{
     dst->time                = src->time;
     dst->nsec                = src->nsec;
     dst->nsec_epoch          = src->nsec_epoch;

     dst->tspec.tv_sec        = src->tspec.tv_sec;
     dst->tspec.tv_nsec       = src->tspec.tv_nsec;

     cr_tm_copy(dst->tm, src->tm);
}

/* cr_time_desc_copy() }}} */
/* cr_month_cmp() {{{ */

/******************************************************************************

                         CR_MONTH_CMP

******************************************************************************/
int cr_month_cmp(cr_month *m1, cr_month *m2)
{
     return strcmp(m1->name, m2->name);
}

/* cr_month_cmp() }}} */
/* cr_get_month_num() {{{ */

/******************************************************************************

                         CR_GET_MONTH_NUM

******************************************************************************/
int cr_get_month_num(char *name)
{
     static cr_month               *_last    = NULL;
     cr_month                       _searched;
     int                            _num;

     if (_last != NULL && !strcmp(name, _last->name)) {
          _num                = _last->num;
     }
     else {
          _searched.name = name;
          _last          = bsearch(&_searched, cr_months, CR_NB_MONTHS, sizeof(_searched),
                                   (int (*)(const void *, const void *)) cr_month_cmp);
          if (_last != NULL) {
               _num                = _last->num;
          }
          else {
               _num                = 0;
          }
     }

     return _num;
}

/* cr_get_month_num() }}} */
/* cr_init_months_names() {{{ */

/******************************************************************************

                         CR_INIT_MONTHS_NAMES

******************************************************************************/
void cr_init_months_names()
{
     struct tm                      _dates;
     char                          *_locale, *_mon[CR_NB_MONTHS], _tmp[32];
     int                            _i;

     if ((_locale = getenv("LC_ALL")) != NULL) {
          CR_DEBUG("locale = \"%s\"\n", _locale);

          if (setlocale(LC_ALL, _locale) == NULL) {
               fprintf(stderr, "%s: setlocale() error !\n", G.prgname);
          }
     }

     bzero(&_dates, sizeof(_dates));
     _dates.tm_year      = G.time_desc.tm->tm_year;
     _dates.tm_mday      = 1;

     for (_i = 0; _i < CR_NB_MONTHS; _i++) {
          _dates.tm_mon                 = _i;
          strftime(_tmp, 10, "%b", &_dates);
          cr_months[_i].num             = _i + 1;
          cr_months[_i].name            = strdup(_tmp);
     }

     qsort(cr_months, sizeof(cr_months) / sizeof(cr_months[0]), sizeof(cr_months[0]),
           (int (*)(const void *, const void *)) cr_month_cmp);
}

/* cr_init_months_names() }}} */
/* cr_init_start_time() {{{ */

/******************************************************************************

                              CR_INIT_START_TIME

******************************************************************************/
void cr_init_start_time()
{
     struct tm           *_tm;

     time_t               _sec;
     long int             _ns;

#if 0
     /* Get clock resolution
        ~~~~~~~~~~~~~~~~~~~~ */
     if (clock_getres(CLOCK_REALTIME, &G.time_desc.tspec) < 0) {
          perror("clock_getres");
          exit(CR_EXIT_ERR_CLOCK_GETTIME);
     }
     CR_DEBUG("Clock res : sec = %ld ns = %ld\n",
              G.time_desc.tspec.tv_sec, G.time_desc.tspec.tv_nsec);
#endif    /* 0 */

     /* Get current clock value
        ~~~~~~~~~~~~~~~~~~~~~~~ */
     if (clock_gettime(CLOCK_REALTIME, &G.time_desc.tspec) < 0) {
          perror("clock_gettime");
          exit(CR_EXIT_ERR_CLOCK_GETTIME);
     }
     _sec                     = G.time_desc.tspec.tv_sec;
     _ns                      = G.time_desc.tspec.tv_nsec;

     G.time_desc.nsec_epoch   = (uint64_t) _sec * CR_10e9 + (uint64_t) _ns;
     G.time_desc.time         = G.time_desc.tspec.tv_sec;
     G.time_desc.tm           = localtime(&G.time_desc.tspec.tv_sec);

     CR_DEBUG("Current time: %ld nanoseconds since the Epoch\n", G.time_desc.nsec_epoch);
     if (G.debug) {
          cr_disp_time_desc(&G.time_desc);
     }
}

/* cr_init_start_time() }}} */
/* cr_set_bounds() {{{ */

/******************************************************************************

                         CR_SET_BOUNDS

******************************************************************************/
void cr_set_bounds(cr_RE_time_desc *RE_t, long period)
{
     int                       _i;
     long                      _delta;
     struct tm                *_tm;

     _delta                        = RE_t->coeff * period;

     for (_i = 0; _i < CR_MAX_PERIODS; _i++) {
          RE_t->lower_bounds[_i].tm               = cr_new_tm();
          RE_t->lower_bounds[_i].nsec_epoch       = ((RE_t->ref_time.tspec.tv_sec - ((_i+1) * _delta)) * CR_10e9)
                                                  + RE_t->ref_time.tspec.tv_nsec;
          RE_t->lower_bounds[_i].tspec.tv_nsec    = RE_t->lower_bounds[_i].nsec_epoch % CR_10e9;
          RE_t->lower_bounds[_i].tspec.tv_sec     = RE_t->lower_bounds[_i].nsec_epoch / CR_10e9;
          _tm                                     = localtime(&RE_t->lower_bounds[_i].tspec.tv_sec);
          cr_tm_copy(RE_t->lower_bounds[_i].tm, _tm);

          RE_t->upper_bounds[_i].tm               = cr_new_tm();
          RE_t->upper_bounds[_i].nsec_epoch       = ((RE_t->ref_time.tspec.tv_sec + ((_i + 1) * _delta)) * CR_10e9)
                                                  + RE_t->ref_time.tspec.tv_nsec;
          RE_t->upper_bounds[_i].tspec.tv_nsec    = RE_t->upper_bounds[_i].nsec_epoch % CR_10e9;
          RE_t->upper_bounds[_i].tspec.tv_sec     = RE_t->upper_bounds[_i].nsec_epoch / CR_10e9;
          _tm                                     = localtime(&RE_t->upper_bounds[_i].tspec.tv_sec);
          cr_tm_copy(RE_t->upper_bounds[_i].tm, _tm);

          CR_DEBUG("lower_bounds[%d].nsec_epoch    = %19ld\n",  _i, RE_t->lower_bounds[_i].nsec_epoch);
          CR_DEBUG("lower_bounds[%d].tspec.tv_nsec = %19ld\n",  _i, RE_t->lower_bounds[_i].tspec.tv_nsec);

          CR_DEBUG("upper_bounds[%d].nsec_epoch    = %19ld\n",  _i, RE_t->upper_bounds[_i].nsec_epoch);
          CR_DEBUG("upper_bounds[%d].tspec.tv_nsec = %19ld\n",  _i, RE_t->upper_bounds[_i].tspec.tv_nsec);
     }
}

/* cr_set_bounds() }}} */
/* cr_init_bounds() {{{ */

/******************************************************************************

                         CR_INIT_BOUNDS

******************************************************************************/
void cr_init_bounds(cr_RE_time_desc *RE_t)
{
     int                       _i, _delta_y, _delta_m;
     long                      _delta;
     struct tm                *_tm;

     CR_ENTERING;

CR_DEBUG("period_type = %d coeff = %d\n", RE_t->period_type, RE_t->coeff);
     if (G.debug) {
// X
//           cr_disp_time_desc(&RE_t->ref_time);
     }

     switch (RE_t->period_type) {

     case CR_PERIOD_YEAR:
          for (_i = 0; _i < CR_MAX_PERIODS; _i++) {
X
CR_DEBUG("_i = %d\n", _i);
               RE_t->lower_bounds[_i].tm               = cr_new_tm();
               cr_tm_copy(RE_t->lower_bounds[_i].tm, RE_t->ref_time.tm);
//               RE_t->lower_bounds[_i].tm->tm_year     -= (_i * RE_t->coeff);
               RE_t->lower_bounds[_i].tm->tm_year     -= ((_i+1) * RE_t->coeff);
CR_DEBUG("==========> ref_time.tm.tm_year = %d\n", RE_t->ref_time.tm->tm_year + 1900);
               RE_t->lower_bounds[_i].tm->tm_isdst     = -1;
               RE_t->lower_bounds[_i].tspec.tv_sec     = mktime(RE_t->lower_bounds[_i].tm);
               RE_t->lower_bounds[_i].nsec_epoch       = (RE_t->lower_bounds[_i].tspec.tv_sec * CR_10e9)
                                                       + RE_t->ref_time.tspec.tv_nsec;
               RE_t->lower_bounds[_i].tspec.tv_nsec    = RE_t->lower_bounds[_i].nsec_epoch % CR_10e9;

               RE_t->upper_bounds[_i].tm               = cr_new_tm();
               cr_tm_copy(RE_t->upper_bounds[_i].tm, RE_t->ref_time.tm);
               RE_t->upper_bounds[_i].tm->tm_year     += ((_i+1) * RE_t->coeff);
               RE_t->upper_bounds[_i].tm->tm_isdst     = -1;
               RE_t->upper_bounds[_i].tspec.tv_sec     = mktime(RE_t->upper_bounds[_i].tm);
               RE_t->upper_bounds[_i].nsec_epoch       = (RE_t->upper_bounds[_i].tspec.tv_sec * CR_10e9)
                                                       + RE_t->ref_time.tspec.tv_nsec;
               RE_t->upper_bounds[_i].tspec.tv_nsec    = RE_t->upper_bounds[_i].nsec_epoch % CR_10e9;

               CR_DEBUG("lower_bounds[%d].nsec_epoch    = %19ld\n",  _i, RE_t->lower_bounds[_i].nsec_epoch);
               CR_DEBUG("lower_bounds[%d].tspec.tv_nsec = %19ld\n",  _i, RE_t->lower_bounds[_i].tspec.tv_nsec);

               CR_DEBUG("upper_bounds[%d].nsec_epoch    = %19ld\n",  _i, RE_t->upper_bounds[_i].nsec_epoch);
               CR_DEBUG("upper_bounds[%d].tspec.tv_nsec = %19ld\n",  _i, RE_t->upper_bounds[_i].tspec.tv_nsec);

               if (G.debug) {
X
                    cr_disp_time_desc(&RE_t->lower_bounds[_i]);
X
                    cr_disp_time_desc(&RE_t->upper_bounds[_i]);
               }
          }
          break;

     case CR_PERIOD_MONTH:
          for (_i = 0; _i < CR_MAX_PERIODS; _i++) {
X
CR_DEBUG("_i = %d\n", _i);
               _delta_m                 = ((_i+1) * RE_t->coeff) % 12;
               _delta_y                 = ((_i+1) * RE_t->coeff) / 12;

               RE_t->lower_bounds[_i].tm               = cr_new_tm();
               cr_tm_copy(RE_t->lower_bounds[_i].tm, RE_t->ref_time.tm);

               if (RE_t->ref_time.tm->tm_mon < _delta_m) {
                    RE_t->lower_bounds[_i].tm->tm_mon      += 12 - _delta_m;
                    RE_t->lower_bounds[_i].tm->tm_year     -= 1  + _delta_y;
               }
               else {
                    RE_t->lower_bounds[_i].tm->tm_mon      -= _delta_m;
                    RE_t->lower_bounds[_i].tm->tm_year     -= _delta_y;
               }
CR_DEBUG("==========> ref_time.tm.tm_year = %d\n", RE_t->ref_time.tm->tm_year + 1900);
               RE_t->lower_bounds[_i].tm->tm_isdst     = -1;
               RE_t->lower_bounds[_i].tspec.tv_sec     = mktime(RE_t->lower_bounds[_i].tm);
               RE_t->lower_bounds[_i].nsec_epoch       = (RE_t->lower_bounds[_i].tspec.tv_sec * CR_10e9)
                                                       + RE_t->ref_time.tspec.tv_nsec;
               RE_t->lower_bounds[_i].tspec.tv_nsec    = RE_t->lower_bounds[_i].nsec_epoch % CR_10e9;

               _delta_m                 = ((_i + 1) * RE_t->coeff) % 12;
               _delta_y                 = ((_i + 1) * RE_t->coeff) / 12;

               RE_t->upper_bounds[_i].tm               = cr_new_tm();
               cr_tm_copy(RE_t->upper_bounds[_i].tm, RE_t->ref_time.tm);

               if (RE_t->ref_time.tm->tm_mon + _delta_m >= 12) {
                    RE_t->upper_bounds[_i].tm->tm_mon      += _delta_m - 12;
                    RE_t->upper_bounds[_i].tm->tm_year     += _delta_y + 1;
               }
               else {
                    RE_t->upper_bounds[_i].tm->tm_mon      += _delta_m;
                    RE_t->upper_bounds[_i].tm->tm_year     += _delta_y;
               }
               RE_t->upper_bounds[_i].tm->tm_isdst     = -1;
               RE_t->upper_bounds[_i].tspec.tv_sec     = mktime(RE_t->upper_bounds[_i].tm);
               RE_t->upper_bounds[_i].nsec_epoch       = (RE_t->upper_bounds[_i].tspec.tv_sec * CR_10e9)
                                                       + RE_t->ref_time.tspec.tv_nsec;
               RE_t->upper_bounds[_i].tspec.tv_nsec    = RE_t->upper_bounds[_i].nsec_epoch % CR_10e9;

               CR_DEBUG("lower_bounds[%d].nsec_epoch    = %19ld\n",  _i, RE_t->lower_bounds[_i].nsec_epoch);
               CR_DEBUG("lower_bounds[%d].tspec.tv_nsec = %19ld\n",  _i, RE_t->lower_bounds[_i].tspec.tv_nsec);

               CR_DEBUG("upper_bounds[%d].nsec_epoch    = %19ld\n",  _i, RE_t->upper_bounds[_i].nsec_epoch);
               CR_DEBUG("upper_bounds[%d].tspec.tv_nsec = %19ld\n",  _i, RE_t->upper_bounds[_i].tspec.tv_nsec);

               if (G.debug) {
X
                    cr_disp_time_desc(&RE_t->lower_bounds[_i]);
X
                    cr_disp_time_desc(&RE_t->upper_bounds[_i]);
               }
          }
          break;

     case CR_PERIOD_WEEK:
          cr_set_bounds(RE_t, CR_PERIOD_w);
          break;

     case CR_PERIOD_DAY:
          cr_set_bounds(RE_t, CR_PERIOD_d);
          break;

     case CR_PERIOD_HOUR:
          cr_set_bounds(RE_t, CR_PERIOD_H);
          break;

     case CR_PERIOD_MIN:
          cr_set_bounds(RE_t, CR_PERIOD_M);
          break;

     case CR_PERIOD_SEC:
          cr_set_bounds(RE_t, CR_PERIOD_S);
          break;

     case CR_PERIOD_USEC:
          for (_i = 0; _i < CR_MAX_PERIODS; _i++) {
               RE_t->lower_bounds[_i].tm               = cr_new_tm();
               RE_t->lower_bounds[_i].nsec_epoch       = (RE_t->ref_time.tspec.tv_sec * CR_10e9)
                                                       + RE_t->ref_time.tspec.tv_nsec - ((_i+1) * 1000 * RE_t->coeff);
               RE_t->lower_bounds[_i].tspec.tv_nsec    = RE_t->lower_bounds[_i].nsec_epoch % CR_10e9;
               RE_t->lower_bounds[_i].tspec.tv_sec     = RE_t->lower_bounds[_i].nsec_epoch / CR_10e9;

               _tm                                     = localtime(&RE_t->lower_bounds[_i].tspec.tv_sec);
               cr_tm_copy(RE_t->lower_bounds[_i].tm, _tm);

               RE_t->upper_bounds[_i].tm               = cr_new_tm();
               RE_t->upper_bounds[_i].nsec_epoch       = (RE_t->ref_time.tspec.tv_sec * CR_10e9)
                                                       + RE_t->ref_time.tspec.tv_nsec + ((_i+1) * 1000 * RE_t->coeff);
               RE_t->upper_bounds[_i].tspec.tv_nsec    = RE_t->upper_bounds[_i].nsec_epoch % CR_10e9;
               RE_t->upper_bounds[_i].tspec.tv_sec     = RE_t->upper_bounds[_i].nsec_epoch / CR_10e9;

               _tm                                     = localtime(&RE_t->upper_bounds[_i].tspec.tv_sec);
               cr_tm_copy(RE_t->upper_bounds[_i].tm, _tm);

               CR_DEBUG("lower_bounds[%d].nsec_epoch    = %19ld\n",  _i, RE_t->lower_bounds[_i].nsec_epoch);
               CR_DEBUG("lower_bounds[%d].tspec.tv_nsec = %19ld\n",  _i, RE_t->lower_bounds[_i].tspec.tv_nsec);

               CR_DEBUG("upper_bounds[%d].nsec_epoch    = %19ld\n",  _i, RE_t->upper_bounds[_i].nsec_epoch);
               CR_DEBUG("upper_bounds[%d].tspec.tv_nsec = %19ld\n",  _i, RE_t->upper_bounds[_i].tspec.tv_nsec);
          }
          break;

     case CR_PERIOD_NSEC:
X
          for (_i = 0; _i < CR_MAX_PERIODS; _i++) {
               RE_t->lower_bounds[_i].tm               = cr_new_tm();
               RE_t->lower_bounds[_i].nsec_epoch       = (RE_t->ref_time.tspec.tv_sec * CR_10e9)
                                                       + RE_t->ref_time.tspec.tv_nsec - ((_i+1) * RE_t->coeff);
               RE_t->lower_bounds[_i].tspec.tv_nsec    = RE_t->lower_bounds[_i].nsec_epoch % CR_10e9;
               RE_t->lower_bounds[_i].tspec.tv_sec     = RE_t->lower_bounds[_i].nsec_epoch / CR_10e9;

               _tm                                     = localtime(&RE_t->lower_bounds[_i].tspec.tv_sec);
               cr_tm_copy(RE_t->lower_bounds[_i].tm, _tm);

               RE_t->upper_bounds[_i].tm               = cr_new_tm();
               RE_t->upper_bounds[_i].nsec_epoch       = (RE_t->ref_time.tspec.tv_sec * CR_10e9)
                                                       + RE_t->ref_time.tspec.tv_nsec + ((_i+1) * RE_t->coeff);
               RE_t->upper_bounds[_i].tspec.tv_nsec    = RE_t->upper_bounds[_i].nsec_epoch % CR_10e9;
               RE_t->upper_bounds[_i].tspec.tv_sec     = RE_t->upper_bounds[_i].nsec_epoch / CR_10e9;

               _tm                                     = localtime(&RE_t->upper_bounds[_i].tspec.tv_sec);
               cr_tm_copy(RE_t->upper_bounds[_i].tm, _tm);

               CR_DEBUG("lower_bounds[%d].nsec_epoch    = %19ld\n",  _i, RE_t->lower_bounds[_i].nsec_epoch);
               CR_DEBUG("lower_bounds[%d].tspec.tv_nsec = %19ld\n",  _i, RE_t->lower_bounds[_i].tspec.tv_nsec);

               CR_DEBUG("upper_bounds[%d].nsec_epoch    = %19ld\n",  _i, RE_t->upper_bounds[_i].nsec_epoch);
               CR_DEBUG("upper_bounds[%d].tspec.tv_nsec = %19ld\n",  _i, RE_t->upper_bounds[_i].tspec.tv_nsec);
          }
          break;

     default:
X
          fprintf(stderr, "%s(%d)\n", __func__, __LINE__);
          fprintf(stderr, "%s: internal error, period_type = %d\n",
                  G.prgname, RE_t->period_type);
          exit(CR_EXIT_ERR_INTERNAL);
     }
// fprintf(G.out, "\n");
     CR_LEAVING;
}

/* cr_init_bounds() }}} */
/* cr_ctime() {{{ */

/******************************************************************************

                         CR_CTIME

******************************************************************************/
char * cr_ctime(long long nsec, char *buf)
{
     time_t                    _time;

     _time                    = nsec / CR_10e9;
     strcpy(buf, ctime(&_time));
     buf[strlen(buf) - 1]     = '\0';

     return buf;
}

/* cr_ctime() }}} */
/* cr_get_period_idx() {{{ */

/******************************************************************************

                         CR_GET_PERIOD_IDX

******************************************************************************/
int cr_get_period_idx(cr_RE_time_desc *RE_t, cr_time_desc *time_desc)
{
     int                  _i;
     char                 _buf1[64], _buf2[64];

     CR_ENTERING;

     if (G.debug) {
          CR_DEBUG("RE_t->ref_time :\n");
          cr_disp_time_desc(&RE_t->ref_time);
          printf("\n");

          CR_DEBUG("time_desc :\n");
          cr_disp_time_desc(time_desc);
          printf("\n");
     }

     for (_i = 0; _i < CR_MAX_PERIODS; _i++) {
          CR_DEBUG("%s() : _i = %2d\n", __func__, _i);

          CR_DEBUG("Lower : %ld <> %ld --- %s <> %s\n",
                   time_desc->nsec_epoch, RE_t->lower_bounds[_i].nsec_epoch,
                   cr_ctime(time_desc->nsec_epoch, _buf1),
                   cr_ctime(RE_t->lower_bounds[_i].nsec_epoch, _buf2));

          CR_DEBUG("Upper : %ld <> %ld --- %s <> %s\n",
                   time_desc->nsec_epoch, RE_t->upper_bounds[_i].nsec_epoch,
                   cr_ctime(time_desc->nsec_epoch, _buf1),
                   cr_ctime(RE_t->upper_bounds[_i].nsec_epoch, _buf2));

          if ((time_desc->nsec_epoch > RE_t->lower_bounds[_i].nsec_epoch)
          &&  (time_desc->nsec_epoch < RE_t->upper_bounds[_i].nsec_epoch)) {
               break;
          }
     }

     if (_i >= CR_LAST_PERIOD_IDX) {
          _i                       = CR_LAST_PERIOD_IDX;
     }


     CR_DEBUG("==========> Period IDX = %d\n", _i);

     CR_LEAVING;
     return _i;
}

/* cr_get_period_idx() }}} */
/* cr_init_ref_time() {{{ */

/******************************************************************************

                    CR_INIT_REF_TIME

******************************************************************************/
bool cr_init_ref_time(char *ref_time, cr_re_desc *re_desc)
{
     char                      *_regex, _errbuf[256], _tmp_str[10240];
     size_t                    _nmatch  = NB_MATCH;
     regmatch_t                _pmatch[NB_MATCH];
     regex_t                   _reg;
     int                       _error, _eflags = 0, _i, _j, _s = 0, _e = 0, _max_sub = 8,
                               _year, _month, _week, _day, _hour, _min, _sec, _usec, _nsec,
                               _retcode;
     long                      _value;
     struct tm                *_tm;
     time_t                    _time;
     cr_RE_time_desc          *_time_desc;

     _time_desc               = &re_desc->time;
     _regex                   = re_desc->regex[0];
     _tm                      = re_desc->time.ref_time.tm;

#if 0
     if (G.debug) {
X
          CR_DEBUG("_time_desc->ref_time :\n");
          cr_disp_time_desc(&_time_desc->ref_time);
     }
#endif    /* 0 */

     /* Compile regex
        ~~~~~~~~~~~~~ */
     if ((_error = regcomp(&_reg, _regex, re_desc->cflags| REG_EXTENDED)) != 0) {
          (void) regerror(_error, &_reg, _errbuf, sizeof(_errbuf));
          fprintf(stderr, "%s: regcomp error for \"%s\" : %s\n",
                  G.prgname, _regex, _errbuf);
          exit(1);
     }

//     CR_DEBUG("ref_time = [%s]\n", ref_time);

     /* Execute regex
        ~~~~~~~~~~~~~ */
     if ((_retcode = regexec(&_reg, ref_time, _nmatch, _pmatch, _eflags)) == 0) {
          //          printf("  MATCH FOR [%s] // [%s]\n", ref_time, _regex);

          /* Search for the date elements : loop on substrings
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          for (_j = 0; _j <= _max_sub; _j++) {
               CR_DEBUG("_j = %d sub_RE_num = %d\n", _j, _time_desc->sub_RE_num.num[CR_TIME_IDX_n]);

               if (_j == 0 && _pmatch[1].rm_so != -1) {
                    continue;
               }

               _s                       = _pmatch[_j].rm_so;
               _e                       = _pmatch[_j].rm_eo - 1;

               strncpy(_tmp_str, ref_time + _s, _e - _s + 1);
               _tmp_str[_e -_s + 1]     = 0;
               _value                   = atol(_tmp_str);

//               printf("    [sub-exp %2d] OFFSET = %3d : %3d -> %3d match = [%s]\n",
//                            _j, _e - _s + 1, _s, _e, _tmp_str);
//               printf("    [sub-exp %2d] DATE ELT : value = %ld\n", _j, _value);

               if (_j == _time_desc->sub_RE_num.num[CR_TIME_IDX_Y]) {
                    _year          = atoi(_tmp_str);
                    _tm->tm_year   = _year - 1900;
                    CR_DEBUG("Year  found (RE_num = %d) : %4d\n",
                             _time_desc->sub_RE_num.num[CR_TIME_IDX_Y], _year);
               }
               else if (_j == _time_desc->sub_RE_num.num[CR_TIME_IDX_m]) {
                    _month         = atoi(_tmp_str);
                    _tm->tm_mon    = _month - 1;
                    CR_DEBUG("Month found (RE_num = %d) : %4d\n",
                             _time_desc->sub_RE_num.num[CR_TIME_IDX_m], _month);
               }
               else if (_j == _time_desc->sub_RE_num.num[CR_TIME_IDX_mn]) {
                    _month         = cr_get_month_num(_tmp_str);
                    _tm->tm_mon    = _month - 1;
                    CR_DEBUG("Month name found (RE_num = %d) : %4d\n",
                             _time_desc->sub_RE_num.num[CR_TIME_IDX_mn], _month);
               }
               else if (_j == _time_desc->sub_RE_num.num[CR_TIME_IDX_d]) {
                    _day           = atoi(_tmp_str);
                    _tm->tm_mday   = _day;
                    CR_DEBUG("Day   found (RE_num = %d) : %4d\n",
                             _time_desc->sub_RE_num.num[CR_TIME_IDX_d], _day);
               }
               else if (_j == _time_desc->sub_RE_num.num[CR_TIME_IDX_H]) {
                    _hour          = atoi(_tmp_str);
                    _tm->tm_hour   = _hour;
                    CR_DEBUG("Hour  found (RE_num = %d) : %4d\n",
                             _time_desc->sub_RE_num.num[CR_TIME_IDX_H], _hour);
               }
               else if (_j == _time_desc->sub_RE_num.num[CR_TIME_IDX_M]) {
                    _min           = atoi(_tmp_str);
                    _tm->tm_min    = _min;
                    CR_DEBUG("Min   found (RE_num = %d) : %4d\n",
                             _time_desc->sub_RE_num.num[CR_TIME_IDX_M], _min);
               }
               else if (_j == _time_desc->sub_RE_num.num[CR_TIME_IDX_S]) {
                    _sec           = atoi(_tmp_str);
                    _tm->tm_sec    = _sec;
                    CR_DEBUG("Sec   found (RE_num = %d) : %4d\n",
                             _time_desc->sub_RE_num.num[CR_TIME_IDX_S], _sec);
               }
               else if (_j == _time_desc->sub_RE_num.num[CR_TIME_IDX_u]) {
                    _usec          = atoi(_tmp_str);
                    re_desc->time.ref_time.tspec.tv_nsec  = _usec * 1000;
                    CR_DEBUG("µsec  found (RE_num = %d) : %4d\n",
                             _time_desc->sub_RE_num.num[CR_TIME_IDX_u], _usec);
               }
               else if (_j == _time_desc->sub_RE_num.num[CR_TIME_IDX_n]) {
                    _nsec          = atoi(_tmp_str);
                    re_desc->time.ref_time.tspec.tv_nsec  = _nsec;
                    CR_DEBUG("nsec  found (RE_num = %d) : %4d\n",
                             _time_desc->sub_RE_num.num[CR_TIME_IDX_n], _nsec);
               }
               else {
                    CR_DEBUG("_j = %d sub_RE_num = %d\n",
                             _j, _time_desc->sub_RE_num.num[CR_TIME_IDX_n]);
               }

#if 0
X
cr_disp_tm(_tm);
X
cr_disp_tm(_time_desc->ref_time.tm);
#endif    /* 0 */
          }

          if (_year == 0) {
X
               _time_desc->ref_time.tm->tm_year       = G.time_desc.tm->tm_year;
               _year                                   = _time_desc->ref_time.tm->tm_year + 1900;
               CR_DEBUG("Year  found (RE_num = %d) : %4d\n",
                         _time_desc->sub_RE_num.num[CR_TIME_IDX_Y], _year);
          }

#if 0
X
cr_disp_tm(_time_desc->ref_time.tm);
#endif    /* 0 */
          /* Convert ASCII date to calendar  time
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          _time_desc->ref_time.tm->tm_isdst      = -1;
          if ((_time_desc->ref_time.tspec.tv_sec = mktime(_time_desc->ref_time.tm)) == -1) {
               perror("mktime");
               exit(CR_EXIT_ERR_MKTIME);
          }
     }
     else {
          fprintf(stderr, "_retcode = %d\n", _retcode);
     }
     _time_desc->ref_time.tm->tm_isdst      = G.time_desc.tm->tm_isdst;
#if 0
X
cr_disp_tm(_time_desc->ref_time.tm);
#endif    /* 0 */

     _time_desc->ref_time.nsec_epoch         = (_time_desc->ref_time.tspec.tv_sec * CR_10e9)
                                             +  _time_desc->ref_time.tspec.tv_nsec;

     if (G.debug) {
          CR_DEBUG("Reference time :\n");
          cr_disp_time_desc(&_time_desc->ref_time);
     }

     return (_retcode == 0);
}

/* cr_init_ref_time() }}} */
/* cr_char_to_str() {{{ */

/******************************************************************************

                              CR_CHAR_TO_STR

******************************************************************************/
char *cr_char_to_str(unsigned char c)
{
     static char               _buf[16];

     if (c != 0 && c < 0x80) {
          sprintf(_buf, "%c", c);
     }
     else {
          sprintf(_buf, "0x%02X", c);
     }

     return _buf;
}

/* cr_char_to_str() }}} */
/* cr_error_syntax() {{{ */

/******************************************************************************

                         CR_ERROR_SYNTAX

******************************************************************************/
void cr_error_syntax(cr_root_args *root_args)
{
     char                 _c, *_p, *_option;
     cr_args             *_args;

     _args               = root_args->args;
     if (_args->idx > 0) {
          _args->idx--;
          if (*(_args->argp) != NULL) {
               _c                  = (*_args->argp)[_args->idx];
               _p                  = &(*_args->argp)[_args->idx];
          }
          *_p                 = 0;
     }
     else {
          _args->argp--;
          _args->idx          = 0;
     }
     _option             = &(*_args->argp)[0];
X

     fprintf(stderr, "%s: syntax error on '%s' after \"%s\"\n",
            G.prgname, cr_char_to_str(_c), _option);
     exit(CR_EXIT_ERR_SYNTAX);
}

/* cr_error_syntax() }}} */
/* cr_error_invalid_color() {{{ */

/******************************************************************************

                         CR_ERROR_INVALID_COLOR

******************************************************************************/
void cr_error_invalid_color(char c)
{
     fprintf(stderr, "%s: invalid color specifier (%s) !\n",
             G.prgname, cr_char_to_str(c));
}

/* cr_error_invalid_color() }}} */
/* cr_state_to_str() {{{ */

/******************************************************************************

                         CR_STATE_TO_STR

******************************************************************************/
char *cr_state_to_str(int state)
{
     char                     *_str;

     switch (state) {

     case CR_STATE_INITIAL :
          _str                     = "INITIAL";
          break;

     case CR_STATE_W_PARAM :
          _str                     = "W_PARAM";
          break;

     case CR_STATE_W_BASE :
          _str                     = "W_BASE";
          break;

     case CR_STATE_W_SEPARATOR_2 :
          _str                     = "W_SEPARATOR_2";
          break;

     case CR_STATE_W_SELECTOR_ID :
          _str                     = "W_SELECTOR_ID";
          break;

     case CR_STATE_W_SEPARATOR :
          _str                     = "W_SEPARATOR";
          break;

     case CR_STATE_W_INTENSITY :
          _str                     = "W_INTENSITY";
          break;

     case CR_STATE_W_INTENSITY_2 :
          _str                     = "W_INTENSITY_2";
          break;

     case CR_STATE_W_COLOR :
          _str                     = "W_COLOR";
          break;

     case CR_STATE_W_NUM :
          _str                     = "W_NUM";
          break;

     case CR_STATE_W_DMY :
          _str                     = "W_DMY";
          break;

     case CR_STATE_W_DMY_2 :
          _str                     = "W_DMY_2";
          break;

     case CR_STATE_W_EQUAL :
          _str                     = "W_EQUAL";
          break;

     case CR_STATE_W_COMMA :
          _str                     = "W_COMMA";
          break;

     case CR_STATE_W_SIGN :
          _str                     = "W_SIGN";
          break;

     case CR_STATE_W_DIGIT :
          _str                     = "W_DIGIT";
          break;

     case CR_STATE_W_COLUMN :
          _str                     = "W_COLUMN";
          break;

     case CR_STATE_W_COMMA_2 :
          _str                     = "W_COMMA_2";
          break;

     case CR_STATE_W_END :
          _str                     = "W_END";
          break;

     case CR_STATE_W_OPTION :
          _str                     = "W_OPTION";
          break;

     case CR_STATE_W_CONFIG :
          _str                     = "W_CONFIG";
          break;

     case CR_STATE_W_NEXT_CHAR :
          _str                     = "W_NEXT_CHAR";
          break;

     case CR_STATE_W_NEXT_ARG :
          _str                     = "W_NEXT_ARG";
          break;

     case CR_STATE_W_RAW_CHAR :
          _str                     = "W_RAW_CHAR";
          break;

     case CR_STATE_W_REFERENCE :
          _str                     = "W_REFERENCE";
          break;

     case CR_STATE_W_SEPARATOR_3 :
          _str                     = "W_SEPARATOR_3";
          break;

// XXX Replace with W_NUM
     case CR_STATE_W_INCREMENT :
          _str                     = "W_INCREMENT";
          break;

     case CR_STATE_W_FMT_SPEC :
          _str                     = "W_FMT_SPEC";
          break;

     case CR_STATE_M_FMT_SPEC :
          _str                     = "M_FMT_SPEC";
          break;

     case CR_STATE_W_FMT_POS :
          _str                     = "W_FMT_POS";
          break;

     case CR_STATE_W_ZERO :
          _str                     = "W_ZERO";
          break;

     case CR_STATE_ERR_SYNTAX_ERROR :
          _str                     = "SYNTAX_ERROR";
          break;

     case CR_STATE_FINAL:
          _str                     = "FINAL";
          break;

     case CR_STATE_W_OPT_NUM :
          _str                     = "W_OPT_NUM";
          break;

     case CR_STATE_W_SELECTOR :
          _str                     = "W_SELECTOR";
          break;

     case CR_STATE_W_COLOR_2 :
          _str                     = "W_COLOR_2";
          break;

     case CR_STATE_W_END_2 :
          _str                     = "W_END_2";
          break;

     default:
          _str                     = "<UNKNOWN>";
          break;
     }

     return _str;
}

/* cr_state_to_str() }}} */
/* cr_transition() {{{ */

/******************************************************************************

                         CR_TRANSITION

******************************************************************************/
void cr_transition(char c, int *ref_state, int new_state)
{
     CR_DEBUG("State %-16s '%s' => state %s\n", 
              cr_state_to_str(*ref_state), cr_char_to_str(c),
              cr_state_to_str(new_state));

     *ref_state               = new_state;
}

/* cr_transition() }}} */
/* cr_list2argv() {{{ */

/******************************************************************************

                              CR_LIST2ARGV

******************************************************************************/
void cr_list2argv(cr_config *config)
{
     /* Convert a list of cr_arg elements into an array argv,
     * and free the initial list (must not be called twice
      * on the same config !!!).
     * argv[0] is unused (skipped), elements are copied 
     * from argv[1]
       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     char                     **_argv;
     cr_arg                    *_arg, *_old_arg;
     int                        _size;

//   CR_ENTERING;

     _size          = sizeof(char **) * (config->argc + 2);

     config->argv   = (char **) malloc(_size);
     if (config->argv == 0) {
          cr_error_malloc();
          exit(CR_EXIT_ERR_MALLOC);
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

//   CR_LEAVING;
}

/* cr_list2argv() }}} */
/* cr_lists2argv() {{{ */

/******************************************************************************

                              CR_LISTS2ARGV

******************************************************************************/
void cr_lists2argv(cr_configs *configs)
{
     cr_config                *_config;

     for (_config = configs->extract; _config != 0; _config = _config->next) {
          cr_list2argv(_config);
     }
}

/* cr_lists2argv() }}} */
/* cr_glob_to_regexp() {{{ */

/******************************************************************************

                              CR_GLOB_TO_REGEXP

******************************************************************************/
char *cr_glob_to_regexp(char *glob_expr)
{
     char                     *_regexp, *_r, *_g;
     int                       _len, _size;

     _len                     = strlen(glob_expr);
     _size                    = (_len * 2) + 3;

     if ((_regexp = malloc(_size)) == NULL) {
          cr_error_malloc();
          exit(CR_EXIT_ERR_MALLOC);
     }

     _r                       = _regexp;
     *_r++                    = '^';

     for (_g = glob_expr; *_g != 0; _g++) {
          switch (*_g) {
          
          case '*':
               *_r++               = '.';
               *_r++               = '*';
               break;

          case '?':
               *_r++               = '.';
               break;

          default:
               *_r++               = *_g;
               break;
          }
     }

     *_r++                    = '$';
     *_r++                    = '\0';

     return _regexp;
}

/* cr_glob_to_regexp() }}} */
/* cr_read_config_file() {{{ */

/******************************************************************************

                              CR_READ_CONFIG_FILE

******************************************************************************/
void cr_read_config_file(char *cfg_file)
{
     struct stat               _st_buf;

     CR_DEBUG("%s(%s) ...\n", __func__, cfg_file);

     if (access(cfg_file, 0) != 0) {
          CR_DEBUG2("%s: config file \"%s\" does not exist !\n",
                    G.prgname, cfg_file);
          return;
     }

     if (stat(cfg_file, &_st_buf) < 0) {
          CR_DEBUG2("%s: cannot stat \"%s\" !\n", G.prgname, cfg_file);
          return;
     }

     if ((_st_buf.st_mode & S_IFMT) != S_IFREG) {
          fprintf(stderr, "%s: %s() : \"%s\" is not a regular file !\n",
                  G.prgname, __func__, cfg_file);
     }
     else {
          if ((yyin = fopen(cfg_file, "r")) == NULL) {
               fprintf(stderr, "%s: cannot open \"%s\" !\n",
                       G.prgname, cfg_file);
               perror("fopen");
               exit(CR_EXIT_ERR_OPEN);
          }

          G.cfg_filename           = strdup(cfg_file);
          yylex();
     }
}

/* cr_read_config_file() }}} */
/* cr_search_config() {{{ */

/******************************************************************************

                              CR_SEARCH_CONFIG

******************************************************************************/
void cr_search_config(char *dir)
{
     int                       _flags = 0, _size, _i;
     glob_t                    _globbuf;
     char                     *_var_conf_glob, *_val_conf_glob, *_val, *_pattern,
                              *_s, *_saveptr = 0;

     _var_conf_glob           = CR_ENV_CONF_GLOB;
     if ((_val_conf_glob = getenv(_var_conf_glob)) == 0) {
          _val_conf_glob      = CR_DEFLT_CONF_GLOB;
     }

     _size                    = strlen(dir) + strlen(_val_conf_glob);
     if ((_pattern = malloc(_size)) == 0) {
          cr_error_malloc();
          exit(CR_EXIT_ERR_MALLOC);
     }

     for (_val = strdup(_val_conf_glob); (_s = strtok_r(_val, ":", &_saveptr)) != 0; _val = 0) {
          sprintf(_pattern, "%s/%s", dir, _s);

          switch (glob(_pattern, _flags, NULL, &_globbuf)) {  
       
          case GLOB_NOSPACE:  
               fprintf(stderr, "Not enough memory !\n");  
               exit(CR_EXIT_ERR_GLOB);  
               break;  
       
          case GLOB_ABORTED:  
               fprintf(stderr, "Read error !\n");  
               exit(CR_EXIT_ERR_GLOB);  
               break;  
       
          case GLOB_NOMATCH:  
//               fprintf(stderr, "No match !\n");  
               break; 
       
          default:  
//fprintf(stderr, "OK\n");  
               for (_i = 0; _globbuf.gl_pathv[_i] != NULL; _i++) { 
                    cr_read_config_file(_globbuf.gl_pathv[_i]);
               } 
          }
     }
}

/* cr_search_config() }}} */
/* cr_read_config_files() {{{ */

/******************************************************************************

                              CR_READ_CONFIG_FILES

******************************************************************************/
void cr_read_config_files(void)
{
     int                       _size;
     char                     *_home, *_cfg_file;
     char                     *_var_conf = CR_ENV_CONF,
                              *_val_conf, *_val, *_s, *_saveptr = 0;
     struct stat               _st_buf;

     if (G.config_file_read) {
          return;
     }

     if ((_val_conf = getenv(_var_conf)) == 0) {
          if ((_home = getenv("HOME")) == 0) {
               fprintf(stderr, "%s: HOME variable undefined !\n", G.prgname);
               exit(CR_EXIT_ERR_UNDEFINED_VAR);
          }

          _size     = strlen(_home) + 1 + sizeof(CR_CONFIG_FILENAME);
          if ((_cfg_file = malloc(_size)) == NULL) {
               cr_error_malloc();
               exit(CR_EXIT_ERR_MALLOC);
          }

          sprintf(_cfg_file, "%s/%s", _home, CR_CONFIG_FILENAME);

          cr_read_config_file(_cfg_file);
          cr_read_config_file(CR_DEFLT_CONFIG_FILE);
     }
     else {
//fprintf(G.debug_out, "%s (%d) : _val_conf = [%s]\n", __func__, __LINE__, _val_conf);
          for (_val = strdup(_val_conf); (_s = strtok_r(_val, ":", &_saveptr)) != 0; _val = 0) {
//fprintf(G.debug_out, "%s (%d) : _s = [%s]\n", __func__, __LINE__, _s);
               if (stat(_s, &_st_buf) < 0) {
                    fprintf(stderr, "Cannot stat \"%s\"\n", _s);
               }
               else {
//fprintf(G.debug_out, "%s (%d) : CONFIG FILE : [%s]\n", __func__, __LINE__, _s);
                    switch (_st_buf.st_mode & S_IFMT) {

                    case S_IFDIR:
//                         fprintf(G.debug_out, "%s (%d) : %s : directory\n", __func__, __LINE__,  _s);
                         cr_search_config(_s);
                         break;

                    case S_IFREG:
//                         fprintf(G.debug_out, "%s (%d) : %s : file\n", __func__, __LINE__,  _s);
                         cr_read_config_file(_s);
                         break;

                    default:
//                         fprintf(G.debug_out, "%s (%d) : %s : IGNORED\n", __func__, __LINE__,  _s);
                         break;
                    }
               }
          }
     }

     cr_lists2argv(&G.configs);

     G.config_file_read  = TRUE;
}

/* cr_read_config_files() }}} */
/* cr_new_config() {{{ */

/******************************************************************************

                         CR_NEW_CONFIG

******************************************************************************/
CR_NEW(config)

/* cr_new_config() }}} */
/* cr_new_arg() {{{ */

/******************************************************************************

                         CR_NEW_ARG

******************************************************************************/
CR_NEW(arg)

/* cr_new_arg() }}} */
/* cr_new_args() {{{ */

/******************************************************************************

                         CR_NEW_ARGS

******************************************************************************/
CR_NEW(args)

/* cr_new_args() }}} */
/* cr_new_root_args() {{{ */

/******************************************************************************

                         CR_NEW_ROOT_ARGS

******************************************************************************/
CR_NEW(root_args)

/* cr_new_root_args() }}} */
/* cr_new_re_desc() {{{ */

/******************************************************************************

                         CR_NEW_RE_DESC

******************************************************************************/
cr_re_desc * cr_new_re_desc()
{
     cr_re_desc                    *_re;

     if ((_re = malloc(sizeof(*_re))) == NULL) {
          cr_error_malloc();
          exit(CR_EXIT_ERR_MALLOC);
     }

     bzero(_re, sizeof(*_re));

     _re->time.ref_time.tm         = cr_new_tm();
#if 0
X
cr_disp_time_desc(&_re->time.ref_time);
#endif    /* 0 */

     return _re;
}


/* cr_new_re_desc() }}} */
/* cr_new_color() {{{ */

/******************************************************************************

                         CR_NEW_COLOR

******************************************************************************/
CR_NEW(color)

/* cr_new_color() }}} */
/* cr_create_color() {{{ */

/******************************************************************************

                         CR_CREATE_COLOR

******************************************************************************/
cr_color *cr_create_color(int color, int intensity)
{
     // XXX : for Alternate option
     cr_color            *_color;

     _color              = cr_new_color();
     _color->col_num     = color;
     _color->intensity   = intensity;
     _color->out         = stdout;      // XXX : TO CHANGE !!!

     return _color;
}

/* cr_create_color() }}} */
/* cr_dump_color() {{{ */

/******************************************************************************

                         CR_DUMP_COLOR

******************************************************************************/
void cr_dump_color(struct cr_color *col)
{
     fprintf(G.debug_out, "COLOR %p : color = %3d), intensity = %d\n",
            col, col->col_num, col->intensity);
}

/* cr_dump_color() }}} */
/* cr_needs_arg() {{{ */

/******************************************************************************

                         CR_NEEDS_ARG

******************************************************************************/
bool cr_needs_arg(char opt, cr_root_args *root_args)
{
     int                  _i;

     for (_i = 0; root_args->opts[_i] != 0; _i++) {
          if (root_args->opts[_i] != opt) continue;
          if (root_args->opts[_i + 1] == '!') {
               return TRUE;
          }
          else {
               break;
          }
     }

     return FALSE;
}

/* cr_needs_arg() }}} */
/* cr_special_opt() {{{ */

/******************************************************************************

                         CR_SPECIAL_OPT

******************************************************************************/
bool cr_special_opt(char opt, cr_root_args *root_args)
{
     // for Alternate option
     int                  _i;

     for (_i = 0; root_args->opts[_i] != 0; _i++) {
          if (root_args->opts[_i] != opt) continue;
          if (root_args->opts[_i + 1] == '{') {
               return TRUE;
          }
          else {
               break;
          }
     }

     return FALSE;
}

/* cr_special_opt() }}} */
/* cr_get_config() {{{ */

/******************************************************************************

                              CR_GET_CONFIG

******************************************************************************/
struct cr_config *cr_get_config(char *config_name, cr_root_args *root_args)
{
     struct cr_config         *_config;

     if (!G.config_file_read) {
          cr_read_config_files();
     }

     for (_config = root_args->configs->extract; _config != 0;
          _config = _config->next) {
          if (!strcmp(config_name, _config->name)) {
               break;
          }
     }

     return _config;
}

/* cr_get_config() }}} */
/* cr_dump_argv() {{{ */

/******************************************************************************

                              CR_DUMP_ARGV

******************************************************************************/
void cr_dump_argv(char **argv)
{
     char                     **_p;

     CR_ENTERING;

     for (_p = argv; *_p != 0 || (_p == argv); _p++) {
          fprintf(G.debug_out, "ARGV[%3d] = \"%s\"\n", (int) (_p - argv), *_p);
     }

     CR_LEAVING;
}

/* cr_dump_argv() }}} */
/* cr_dump_config() {{{ */

/******************************************************************************

                              CR_DUMP_CONFIG

******************************************************************************/
void cr_dump_config(struct cr_config *config)
{
     fprintf(G.debug_out, "CONFIG (%s) :\n", config->name);
     fprintf(G.debug_out, "    visited     = %s\n",     config->visited ? "TRUE" : "FALSE");
     fprintf(G.debug_out, "    config_file = \"%s\"\n", config->config_file);
     fprintf(G.debug_out, "    next        = %p\n",     config->next);
     fprintf(G.debug_out, "    extract     = %p\n",     config->extract);
     fprintf(G.debug_out, "    insert      = %p\n",     config->insert);
     fprintf(G.debug_out, "    argc        = %3d\n",    config->argc);
     fprintf(G.debug_out, "    argv        = %p\n",     config->argv);
     cr_dump_argv(config->argv);
     fprintf(G.debug_out, "    nb_opts     = %3d\n",    config->nb_opts);
}

/* cr_dump_config() }}} */
/* cr_dump_args() {{{ */

/******************************************************************************

                              CR_DUMP_ARGS

******************************************************************************/
void cr_dump_args(cr_args *args)
{
     CR_ENTERING;

     fprintf(G.debug_out, "ARGS (%p) :\n", args);
     fprintf(G.debug_out, "    argc        = %d\n", args->argc);
     fprintf(G.debug_out, "    argv        = %p\n", args->argv);
     cr_dump_argv(args->argv);
     fprintf(G.debug_out, "    argp        = %p\n", args->argp);
     cr_dump_argv(args->argp);
     fprintf(G.debug_out, "    idx         = %d\n", args->idx);
     fprintf(G.debug_out, "    next_arg    = %p\n", args->next_arg);
     fprintf(G.debug_out, "    prev        = %p\n", args->prev);
     fprintf(G.debug_out, "    config      = %p\n", args->config);
     fprintf(G.debug_out, "    level       = %d\n", args->level);

     CR_LEAVING;
}

/* cr_dump_args() }}} */
/* cr_dump_root_args() {{{ */

/******************************************************************************

                              CR_DUMP_ROOT_ARGS

******************************************************************************/
void cr_dump_root_args(struct cr_root_args *root_args)
{
     printf("root_args :\n");
     printf("    opts      = \"%s\"\n", root_args->opts);
     printf("    optarg    = \"%s\"\n", root_args->optarg);
     printf("    args      = %p\n",     root_args->args);
     printf("    configs   = %p\n",     root_args->configs);

     cr_dump_args(root_args->args);
}

/* cr_dump_root_args() }}} */
/* cr_dump_alt_desc() {{{ */

/******************************************************************************

                              CR_DUMP_ALT_DESC

******************************************************************************/
void cr_dump_alt_desc(cr_alt_desc *alt_desc, int idx)
{
     int                       _i;
     struct cr_color          *_color;

     fprintf(G.debug_out, "alt_desc (%d): [%p]\n", idx, alt_desc);
     fprintf(G.debug_out, "    idx       = \"%2d\"\n", alt_desc->alt_idx);
     for (_i = 0; ; _i++) {
          _color              = alt_desc->alt_cols[_i];
          if (_color == NULL) {
               break;
          }
          fprintf(G.debug_out, "Color[%d][%d]  = (%d, %2d)  (%p)\n",
                   idx, _i, _color->intensity, _color->col_num, _color);
     }
     fprintf(G.debug_out, "\n");
}

/* cr_dump_alt_desc() }}} */
/* cr_is_selector() {{{ */

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

/* cr_is_selector() }}} */
/* cr_is_intensity() {{{ */

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

/* cr_is_intensity() }}} */
/* cr_is_a_color() {{{ */

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

/* cr_is_a_color() }}} */
/* cr_is_int() {{{ */

/******************************************************************************

                         CR_IS_INT

******************************************************************************/
int cr_is_int(char c)
{
     int             _retcode;

     if (strchr(G.int_string, (int) c) != NULL) {
          _retcode  = TRUE;
     }
     else {
          _retcode  = FALSE;
     }

     return _retcode;
}

/* cr_is_int() }}} */
/* cr_is_op() {{{ */

/******************************************************************************

                         CR_IS_OP

******************************************************************************/
int cr_is_op(char c)
{
     int             _retcode;

     if (strchr(G.op_string, (int) c) != NULL) {
          _retcode  = TRUE;
     }
     else {
          _retcode  = FALSE;
     }

     return _retcode;
}

/* cr_is_op() }}} */
/* cr_is_base() {{{ */

/******************************************************************************

                         CR_IS_BASE

******************************************************************************/
int cr_is_base(char c)
{
     int             _retcode;

     if (strchr(G.base_string, (int) c) != NULL) {
          _retcode  = TRUE;
     }
     else {
          _retcode  = FALSE;
     }

     return _retcode;
}

/* cr_is_base() }}} */
/* cr_op() {{{ */

/******************************************************************************

                         CR_OP

******************************************************************************/
int cr_op(char op)
{
     int             _op;

     switch (op) {

     case '+':
          _op       = CR_OP_ADD;
          break;

     case '-':
          _op       = CR_OP_SUB;
          break;

     case '*':
          _op       = CR_OP_MUL;
          break;

     case '/':
          _op       = CR_OP_DIV;
          break;
     }

     return _op;
}

/* cr_op() }}} */
/* cr_base() {{{ */

/******************************************************************************

                         CR_BASE

******************************************************************************/
int cr_base(char base)
{
     int             _base;

     switch (base) {

     case 'd':
          _base     = CR_BASE_DEC;
          break;

     case 'o':
          _base     = CR_BASE_OCT;
          break;

     case 'x':
          _base     = CR_BASE_HEX;
          break;

     case 'a':
          _base     = CR_BASE_ASCII;
          break;

     default:
          fprintf(stderr, "%s: internal error (base = 0x%08X)\n",
                  G.prgname, base);
          exit(CR_EXIT_ERR_INTERNAL);
     }

     return _base;
}

/* cr_base() }}} */
/* cr_base_to_str() {{{ */

/******************************************************************************

                         CR_BASE_TO_STR

******************************************************************************/
char *cr_base_to_str(int base)
{
     char           *_base;

     switch (base) {

     case CR_BASE_DEC:
          _base     = "decimal";
          break;

     case CR_BASE_OCT:
          _base     = "octal";
          break;

     case CR_BASE_HEX:
          _base     = "hexadecimal";
          break;

     case CR_BASE_ASCII:
          _base     = "ascii";
          break;

     default:
          _base     = "UNKNOWN";
          break;
     }

     return _base;
}

/* cr_base_to_str() }}} */
/* cr_op_to_str() {{{ */

/******************************************************************************

                         CR_OP_TO_STR

******************************************************************************/
char *cr_op_to_str(int op)
{
     char           *_op;

     switch (op) {

     case CR_OP_ADD:
          _op       = "add";
          break;

     case CR_OP_SUB:
          _op       = "sub";
          break;

     case CR_OP_MUL:
          _op       = "mul";
          break;

     case CR_OP_DIV:
          _op       = "div";
          break;

     default:
          _op       = "UNKNOWN";
          break;
     }

     return _op;
}

/* cr_op_to_str() }}} */
/* cr_decode_color() {{{ */

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
          exit(CR_EXIT_ERR_SYNTAX);
          break;
     }

     return _color;
}

/* cr_decode_color() }}} */
/* cr_dec_to_long() {{{ */

/******************************************************************************

                         CR_DEC_TO_LONG

******************************************************************************/
long cr_dec_to_long(char *str)
{
     long                 _value = 0;

     sscanf(str, "%ld", &_value);
     return _value;
}

/* cr_dec_to_long() }}} */
/* cr_hex_to_long() {{{ */

/******************************************************************************

                         CR_HEX_TO_LONG

******************************************************************************/
long cr_hex_to_long(char *str)
{
     long                 _value = 0;

     sscanf(str, "%lx", &_value);
     return _value;
}

/* cr_hex_to_long() }}} */
/* cr_oct_to_long() {{{ */

/******************************************************************************

                         CR_OCT_TO_LONG

******************************************************************************/
long cr_oct_to_long(char *str)
{
     long                 _value = 0;

     sscanf(str, "%lo", &_value);
     return _value;
}

/* cr_oct_to_long() }}} */
/* cr_ascii_to_long() {{{ */

/******************************************************************************

                         CR_ASCII_TO_LONG

******************************************************************************/
long cr_ascii_to_long(char *str)
{
     long                 _value = 0;

     _value              = str[0];

     return _value;
}

/* cr_ascii_to_long() }}} */
/* cr_convert() {{{ */

/******************************************************************************

                         CR_CONVERT

******************************************************************************/
long cr_convert(struct cr_re_desc *re, char *str)
{
     long                 _value;

     switch (re->base) {

     case CR_BASE_DEC:
          _value              = cr_dec_to_long(str);
          break;

     case CR_BASE_HEX:
          _value              = cr_hex_to_long(str);
          break;

     case CR_BASE_OCT:
          _value              = cr_oct_to_long(str);
          break;

     case CR_BASE_ASCII:
          _value              = cr_ascii_to_long(str);
          break;

     default:
          fprintf(stderr, "%s: internal error (base = 0x%08X)\n",
                  G.prgname, re->base);
          exit(CR_EXIT_ERR_INTERNAL);
     }

     return _value;
}

/* cr_convert() }}} */
/* cr_set_opt_pointers() {{{ */

/******************************************************************************

                         CR_SET_OPT_POINTERS

******************************************************************************/
void cr_set_opt_pointers(cr_root_args *root_args, char **p, char **regexp)
{
//     root_args->args->p            = p;
     root_args->args->regexp       = regexp;
}

/* cr_set_opt_pointers() }}} */
/* cr_set_opt_param() {{{ */

/******************************************************************************

                         CR_SET_OPT_PARAM

******************************************************************************/
void cr_set_opt_param(cr_root_args *root_args, char **ref_param)
{
     root_args->args->param_before_RE   = TRUE;
     root_args->args->param             = ref_param;
}

/* cr_set_opt_param() }}} */
/* cr_getopt() {{{ */

/******************************************************************************

                              CR_GETOPT

******************************************************************************/
int cr_getopt(struct cr_root_args *root_args)
{
     int                  _c = 0;
     char                 _next_c, *_config_name, *_arg, *_option;
     cr_args             *_args, *_new_args;
     struct cr_config    *_config;
     bool                 _return = FALSE;

//G.debug   = 2;
     CR_DEBUG("Entering %s()\n", __func__);

     for ( ; !_return && root_args->state != CR_STATE_FINAL; ) {
          _args               = root_args->args;
//cr_dump_args(_args);

          CR_DEBUG("\n");
          CR_DEBUG("===========================\n");
          CR_DEBUG("CONFIG       = \"%s\"\n", _args->config ? _args->config->name : "");
          CR_DEBUG("ARGP         = \"%s\"\n", *_args->argp);
          CR_DEBUG("ARGV[%2d][%2d] = \"%s\"\n", (int) (_args->argp - _args->argv),
                   _args->idx, &_args->argp[0][_args->idx]);
          CR_DEBUG("State        = \"%s\"\n", cr_state_to_str(root_args->state));

          if (*_args->argp) {
               _c                       = (*_args->argp)[_args->idx];
               CR_DEBUG("_c           = '%s'\n", cr_char_to_str(_c));
          }

          switch (root_args->state) {

          case CR_STATE_INITIAL:
               cr_transition(_c, &root_args->state, CR_STATE_W_NEXT_ARG);
               break;

          case CR_STATE_W_OPTION:
               switch (_c) {

               case '-':
                    cr_transition(_c, &root_args->state, CR_STATE_W_CONFIG);
                    _args->idx++;
                    break;

               case '\0':
                    _args->idx++;
                    cr_error_syntax(root_args);
                    break;

               default:
                    if (cr_special_opt(_c, root_args)) {
                         cr_transition(_c, &root_args->state, CR_STATE_W_RAW_CHAR);
                         _args->idx++;
                         _return                  = TRUE;
                    }
                    else if (cr_needs_arg(_c, root_args)) {
                         _arg                     = *(_args->argp + 1);
                         _next_c                  = (*_args->argp)[_args->idx + 1];
                         CR_DEBUG("_c      = '%s'\n", cr_char_to_str(_c));
                         CR_DEBUG("_next_c = '%s'\n", cr_char_to_str(_next_c));

                         if (_next_c != '\0') {
                              cr_transition(_c, &root_args->state, CR_STATE_ERR_FOLLOWING);
                         }
                         else if (_arg == NULL || _arg[0] == '-') {
                              cr_transition(_c, &root_args->state, CR_STATE_ERR_MISSING_ARG);
                         }
                         else {
                              cr_transition(_c, &root_args->state, CR_STATE_W_NEXT_ARG);
                              CR_DEBUG("OPTARG       = \"%s\"\n", _arg);
                              root_args->optarg        = _arg;
                              root_args->args->argp   += 2;
                              _args->idx               = 0;
                              _return                  = TRUE;
                         }
                    }
                    else {
                         cr_transition(_c, &root_args->state, CR_STATE_W_NEXT_CHAR);
                         _args->idx++;
                         _return                  = TRUE;
                    }
                    break;
               }
               break;

          case CR_STATE_W_CONFIG:
               cr_transition(_c, &root_args->state, CR_STATE_W_NEXT_ARG);

               _config_name             = &(*_args->argp)[2];
               CR_DEBUG("_config_name = [%s]\n", _config_name);

               _config                  = cr_get_config(_config_name, root_args);
               if (_config == 0) {
                        fprintf(stderr, "%s: undefined configuration (%s) !\n",
                            G.prgname, _config_name);
                    exit(CR_EXIT_ERR_UNKNOWN_CONF);
               }

               /* Detection of recursive loop
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (_config->visited) {
                    fprintf(stderr, "%s: configuration loop for \"%s\" !\n",
                            G.prgname, _config->name);
                    exit(CR_EXIT_ERR_CONF_LOOP);
               }
               _config->visited         = TRUE;

               /* Switch to configuration arguments and options
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _new_args                = cr_new_args();
               _new_args->prev          = _args;
               _new_args->argc          = _config->argc;
               _new_args->argv          = _config->argv;
               _new_args->argp          = _new_args->argv + 1;
               _new_args->config        = _config;
               _new_args->level         = _args->level + 1;
               root_args->args          = _new_args;
               break;

               case CR_STATE_W_RAW_CHAR:
                    switch (_c) {

                    case '\0':
                         /* End of special option string
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _arg                     = *(_args->argp + 1);

/****************************** A VALIDER *****************************************/
                         if (root_args->args->param_before_RE) {
CR_DEBUG("==========> PARAM before RE !\n");
                              if (_arg == NULL || _arg[0] == '-') {
                                   /* Following argument is not a parameter
                                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                   *root_args->args->param  = NULL;
                              }
                              else {
                                   /* Initialize param pointer
                                      ~~~~~~~~~~~~~~~~~~~~~~~~ */
                                   CR_DEBUG("PARAM        = [%s]\n", _arg);
                                   *root_args->args->param  = _arg;
                                   root_args->args->argp++;
                                   _arg                     = *(_args->argp + 1);
                              }
                         }

                         if (_arg == NULL || _arg[0] == '-') {
                              /* Following argument is not a regexp
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                              *root_args->args->regexp = NULL;
                              _args->argp++;
                         }
                         else {
                              /* Initialize regexp pointer
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~ */
CR_DEBUG("REGEXP       = [%s]\n", _arg);
                              CR_DEBUG("REGEXP       = [%s]\n", _arg);
                              *root_args->args->regexp = _arg;
                              root_args->args->argp   += 2;
                         }
/**********************************************************************************/

                         root_args->args->idx     = 0;
                         cr_transition(_c, &root_args->state, CR_STATE_W_NEXT_ARG);
                         break;

                    default:
                         _args->idx++;
                         break;
                    }

                    _return                  = TRUE;
                    break;

          case CR_STATE_W_NEXT_CHAR:
               if (cr_needs_arg(_c, root_args)) {
                    _arg                     = *(_args->argp + 1);
                    _next_c                  = (*_args->argp)[_args->idx + 1];
                    CR_DEBUG("_next_c      = '%s'\n", cr_char_to_str(_next_c));

                    if (_next_c != '\0') {
                         cr_transition(_c, &root_args->state, CR_STATE_ERR_FOLLOWING);
                    }
                    else if (_arg == NULL || _arg[0] == '-') {
                         cr_transition(_c, &root_args->state, CR_STATE_ERR_MISSING_ARG);
                    }
                    else {
                         cr_transition(_c, &root_args->state, CR_STATE_W_NEXT_ARG);
                         CR_DEBUG("OPTARG       = \"%s\"\n", _arg);
                         root_args->optarg        = _arg;
                         root_args->args->argp   += 2;
                         _args->idx               = 0;
                         _return                  = TRUE;
                    }
               }
               else if (cr_special_opt(_c, root_args)) {
                    cr_transition(_c, &root_args->state, CR_STATE_W_RAW_CHAR);
                    _return                  = TRUE;
                    _c                       = (*_args->argp)[++_args->idx];
               }
               else {
                    switch (_c) {

                    case '\0':
                         cr_transition(_c, &root_args->state, CR_STATE_W_NEXT_ARG);
                         _args->argp++;
                         root_args->args->idx     = 0;
                         break;

                    default:
                         _return                  = TRUE;
                         _args->idx++;
                         break;
                    }
               }
               break;

          case CR_STATE_W_NEXT_ARG:
               if (*_args->argp == NULL) {
                    if (_args->level == 0) {
                         /* Main level : no more argument
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         cr_transition(_c, &root_args->state, CR_STATE_FINAL);
                         _c                          = EOF;
                    }
                    else {
                         /* End of configuration arguments :
                          * resume analysis of the previous configuration
                          * (restore previous pointers and continue)
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         CR_DEBUG("RESUME TO PREVIOUS ARGUMENTS\n");
                         root_args->args               = _args->prev;
                         free(_args);
                    }
               }
               else {
                    _option             = (*_args->argp);
//                    _c                  = (*_args->argp)[_args->idx];
//                    CR_DEBUG("_c      = '%s'\n", cr_char_to_str(_c));

                    switch (_c) {

                    case '\0':
                         _c                       = EOF;
                         _return                  = TRUE;
                         break;

                    case '-':
                         cr_transition(_c, &root_args->state, CR_STATE_W_OPTION);
                         _args->idx++;
                         break;

                    default:
                         cr_transition(_c, &root_args->state, CR_STATE_W_NEXT_ARG);
                         _args->argp++;
                         root_args->args->idx     = 0;
                         break;
                    }
               }
               break;

          case CR_STATE_ERR_FOLLOWING:
               /* No option may follow this one in the current argument
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               fprintf(stderr, "%s: option '%s' followed by '%s' !\n",
                       G.prgname, cr_char_to_str(_c), cr_char_to_str(_next_c));
               fprintf(stderr, "%s: option '%s' must not be followed by another option in the same argument.\n",
                       G.prgname, cr_char_to_str(_c));
               exit(CR_EXIT_ERR_SYNTAX);
               break;

          case CR_STATE_ERR_MISSING_ARG:
               /* Missing argument for this option
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               fprintf(stderr, "%s: missing argument for \"-%s\" !\n",
                       G.prgname, cr_char_to_str(_c));
               cr_dump_root_args(root_args);
               exit(CR_EXIT_ERR_SYNTAX);
               break;

          default:
               cr_error_syntax(root_args);
               break;
          }
     }

     CR_DEBUG("[%s]\n", cr_char_to_str(_c));
     CR_DEBUG("Leaving %s()\n\n", __func__);
     return _c;
}

/* cr_getopt() }}} */
/* cr_decode_alternate() {{{ */

/******************************************************************************

                              CR_DECODE_ALTERNATE

******************************************************************************/
struct cr_re_desc *cr_decode_alternate(cr_root_args *root_args)
{
     // for Alternate option
     int                       _state, _curr_col_idx, _selector, _lg, _size;
     char                      _c, _next_c, *_regexp;
     cr_args                  *_args;
     struct cr_color         **_alt_colors;
     struct cr_re_desc        *_re;
     int                       _error, _i, _j, _trigger_num = -1;
     char                      _errbuf[256], *_p, *_option;

     CR_ENTERING;

     _re                      = cr_new_re_desc();
     _re->alternate           = TRUE;
     _re->cflags              = G.cflags;
     _re->col.col_num         = 0;
     _re->col.intensity       = 0;
     _re->col.out             = G.out;
     _re->max_sub             = 1;
     _re->alt[0].alt_idx      = 0;
     _re->alt[1].alt_idx      = 0;
     _re->curr_alt            = &_re->alt[0];
     _re->idx_regex_select    = 0;
     _re->matching_str        = NULL;
     _re->change_on_diff      = TRUE;
     G.out                    = stdout;
     _lg                      = 0;

     _curr_col_idx            = 0;
     _selector                = 0;

     _regexp                  = NULL;

     _args                    = root_args->args;

     if (_args == NULL) {
          _lg                           = 3;
     }
     else {
          if (*_args->argp != NULL) {
               _lg                      = strlen(*_args->argp);
          }
          if (_lg < 3) {
               _lg                      = 3;
          }
          _option                  = *_args->argp;
     }

     /* Allocate memory for the color descriptors
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _size                    = (_lg + 1) * sizeof(struct cr_color **);
     for (_i = 0; _i < (sizeof(_re->alt) / sizeof(_re->alt[0])); _i++) {
          if ((_alt_colors = (struct cr_color **) malloc(_size)) == NULL) {
               cr_error_malloc();
               exit(CR_EXIT_ERR_MALLOC);
          }
          _re->alt[_i].alt_cols    = _alt_colors;

          for (_j = 0; _j < (_lg + 1); _j++) {
               _alt_colors[_j]          = NULL;
          }
     }
     _alt_colors              =
     _re->curr_alt->alt_cols  = _re->alt[0].alt_cols;

     /* Decode options
        ~~~~~~~~~~~~~~ */
     for (_state = CR_STATE_INITIAL;_state != CR_STATE_FINAL; ) {
          cr_set_opt_pointers(root_args, &_p, &_regexp);
          _c             = cr_getopt(root_args);

          switch (_state) {

          case CR_STATE_INITIAL:
               switch (_c) {

               case '#' :
                    cr_transition(_c, &_state, CR_STATE_W_OPT_NUM);
                    break;

               case ',' :
                    _selector      = 0;
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
                    break;

               case '\0':
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;

               default:
                    if (cr_is_selector(_c)) {
                         _selector      = _c - '0';
                         cr_transition(_c, &_state, CR_STATE_W_SEPARATOR);
                    }
                    else {
                         cr_error_syntax(root_args);
                    }
                    break;
               }
               break;

          case CR_STATE_W_SELECTOR:
               switch (_c) {

               case ',' :
                    _selector      = 0;
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
                    break;

               case '\0':
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;

               default:
                    if (cr_is_selector(_c)) {
                         _selector      = _c - '0';
                         cr_transition(_c, &_state, CR_STATE_W_SEPARATOR);
                    }
                    else {
                         cr_error_syntax(root_args);
                    }
                    break;
               }
               break;

          case CR_STATE_W_OPT_NUM:
               if (cr_is_int(_c)) {
                    _trigger_num   = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLUMN);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COLUMN:
               if (_c == ':') {
                    cr_transition(_c, &_state, CR_STATE_W_SELECTOR);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_SEPARATOR:
               if (_c == ',') {
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
               }
               else if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_FINAL);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_INTENSITY:
               if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLOR);
               }
               else if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COLOR:
               if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else {
                    cr_error_invalid_color(_c);
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_END:
               if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLOR);
               }
               else if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else if (_c == ':') {
                    _alt_colors[_curr_col_idx++]  = NULL;
                    _alt_colors    = _re->alt[1].alt_cols;
                    _curr_col_idx  = 0;
                    _re->curr_alt  = &_re->alt[1];
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY_2);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_INTENSITY_2:
               if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLOR_2);
               }
               else if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END_2);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COLOR_2:
               if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END_2);
               }
               else {
                    cr_error_invalid_color(_c);
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_END_2:
               if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLOR_2);
               }
               else if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END_2);
               }
               else if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_FINAL);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          default:
               cr_error_syntax(root_args);
               break;
          }

          if (_regexp != NULL) {
               /* Regex initialized : get out of the loop
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

               break;
          }
     }

     _re->trigger_num              = _trigger_num;
     _alt_colors[_curr_col_idx++]  = NULL;

     for (_i = 0; _i < (sizeof(_re->alt[0].alt_cols) / sizeof(_re->alt[0].alt_cols[0])); _i++) {
          _alt_colors         = _re->alt[_i].alt_cols;

          if (_alt_colors[0] == NULL) {
               _alt_colors[0]           = G.deflt_alt_col_1;
          }
          if (_alt_colors[1] == NULL) {
               _alt_colors[1]           = G.deflt_alt_col_2;
          }
          if (_regexp == NULL) {
               _regexp                  = CR_DEFLT_ALT_REGEXP;
          }
     }

     _re->curr_alt            = &_re->alt[0];

     /* Count number of possible sub strings
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_p = _regexp; (*_p); _p++) {
          if (*_p == '(') {
               _re->max_sub++;
          }
     }

     if (!G.consistency) {
          _selector++;   // '0' => regexp number 1
     }
     _re->idx_regex_select    = _selector;
     _re->regex[0]            = _regexp;
     _re->regex[1]            = NULL;

     if (G.debug || G.verbose) {
          struct cr_color     *_color;

          CR_DEBUG("Selector  = %d\n",   _re->idx_regex_select);
          CR_DEBUG("regex[0]  = [%s]\n", _re->regex[0]);
          CR_DEBUG("regex[1]  = %p\n",   _re->regex[1]);
          for (_i = 0; _i < (sizeof(_re->alt) / sizeof(_re->alt[0])); _i++) {
               for (_j = 0; ; _j++) {
                    _color              = _re->alt[_i].alt_cols[_j];
                    if (_color == NULL) {
                         break;
                    }
                    CR_DEBUG("Color[%d][%d]  = (%d, %2d)  (%p)\n",
                             _i, _j, _color->intensity, _color->col_num, _color);
               }
               CR_DEBUG("\n");
          }
     }

     if ((_error = regcomp(&_re->reg[0], _regexp, _re->cflags)) != 0) {
          (void) regerror(_error, &_re->reg[0], _errbuf, sizeof(_errbuf));
          fprintf(stderr, "%s: regcomp error for \"%s\" : %s\n",
                  G.prgname, _regexp, _errbuf);
          exit(CR_EXIT_ERR_REGCOMP);
     }

     CR_LEAVING;
     return _re;
}

/* cr_decode_alternate() }}} */
/* cr_decode_sequential() {{{ */

/******************************************************************************

                              CR_DECODE_SEQUENTIAL

******************************************************************************/
struct cr_re_desc *cr_decode_sequential(cr_root_args *root_args)
{
     // for Sequential option
     int                       _state, _curr_col_idx, _selector, _lg, _size;
     char                      _c, _next_char, *_regexp, *_option;
     cr_args                  *_args;
     struct cr_color         **_alt_colors;
     struct cr_re_desc        *_re;
     int                       _error, _i, _idx;
     char                      _errbuf[256], *_p, _str_c[2], _param[32], *_fmt;

     CR_ENTERING;

     _re                      = cr_new_re_desc();
     _re->sequential          = TRUE;
     _re->cflags              = G.cflags;
     _re->col.col_num         = 0;
     _re->col.intensity       = 0;
     _re->col.out             = G.out;
     _re->max_sub             = 1;
     _re->alt[0].alt_idx      = 0;
     _re->idx_regex_select    = 0;
     _re->matching_str        = NULL;
     _re->change_on_diff      = TRUE;
     _re->change_on_bad_next  = TRUE;
     _re->val                 = 0;
     _re->param               = 1;
     _re->op                  = CR_OP_ADD;
     _re->base                = CR_BASE_DEC;
     G.out                    = stdout;

     _curr_col_idx            = 0;
     _selector                = 0;

     _regexp                  = NULL;

     _args                    = root_args->args;
     _option                  = *_args->argp;

     if (_args != NULL) {
          _option                  = *_args->argp;
     }
     else {
          _lg                      = strlen(*_args->argp);
          if (_lg < 3) {
               _lg                      = 3;
          }
     }

     /* Allocate memory for the color descriptors
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _size                    = (_lg + 1) * sizeof(struct cr_color **);
     if ((_alt_colors = (struct cr_color **) malloc(_size)) == NULL) {
          cr_error_malloc();
          exit(CR_EXIT_ERR_MALLOC);
     }
     _re->alt[0].alt_cols     = _alt_colors;

     for (_i = 0; _i < (_lg + 1); _i++) {
          _alt_colors[_i]          = NULL;
     }

     _idx           = 0;
     _c             = 0;

     /* Decode options
        ~~~~~~~~~~~~~~ */
     for (_state = CR_STATE_INITIAL; _state != CR_STATE_FINAL; ) {

          cr_set_opt_pointers(root_args, &_p, &_regexp);
          _c             = cr_getopt(root_args);

          CR_DEBUG("CHAR    = '%s'\n", cr_char_to_str(_c));
          CR_DEBUG("_regexp = %p\n", _regexp);

          switch (_state) {

          case CR_STATE_INITIAL:
               if (cr_is_op(_c)) {
                    _re->op        = cr_op(_c);
                    cr_transition(_c, &_state, CR_STATE_W_PARAM);
               }
               else if (cr_is_int(_c)) {
                    _param[_idx++] = _c;
                    _param[_idx]   = 0;
                    cr_transition(_c, &_state, CR_STATE_W_BASE);
               }
               else if (cr_is_base(_c)) {
                    _re->base      = cr_base(_c);
                    cr_transition(_c, &_state, CR_STATE_W_SEPARATOR_2);
               }
               else if (_c == ',') {
                    _selector      = 0;
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
               }
               else if (_c == ':') {
                    _selector      = 0;
                    cr_transition(_c, &_state, CR_STATE_W_SELECTOR_ID);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_PARAM:
               if (cr_is_int(_c)) {
                    _param[_idx++] = _c;
                    _param[_idx]   = 0;
                    cr_transition(_c, &_state, CR_STATE_W_BASE);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_BASE:
               if (cr_is_int(_c)) {
                    _param[_idx++] = _c;
                    _param[_idx]   = 0;
                    cr_transition(_c, &_state, CR_STATE_W_BASE);
               }
               else if (cr_is_base(_c)) {
                    _re->base      = cr_base(_c);
                    cr_transition(_c, &_state, CR_STATE_W_SEPARATOR_2);
               }
               else if (_c == ':') {
                    cr_transition(_c, &_state, CR_STATE_W_SELECTOR_ID);
               }
               else if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_SEPARATOR_2:
               if (_c == ':') {
                    cr_transition(_c, &_state, CR_STATE_W_SELECTOR_ID);
               }
               else if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_SELECTOR_ID:
               if (cr_is_int(_c)) {
                    _selector      = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_SEPARATOR);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_SEPARATOR:
               switch (_c) {

               case ',':
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
                    break;

               case '\0':
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;

               default:
                    cr_error_syntax(root_args);
                    break;
               }
               break;

          case CR_STATE_W_INTENSITY:
               if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLOR);
               }
               else if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COLOR:
               if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else {
                    cr_error_invalid_color(_c);
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_END:
               if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLOR);
               }
               else if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          default:
               cr_error_syntax(root_args);
               break;
          }

          if (_regexp != NULL) {
               /* Regex initialized : get out of the loop
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               break;
          }
     }

     _alt_colors[_curr_col_idx++]  = NULL;

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

     _re->idx_regex_select    = _selector;
     _re->regex[0]            = _regexp;
     _re->regex[1]            = NULL;
     if (_idx > 0) {
          if (_re->base == CR_BASE_ASCII) {
               _re->param               = cr_dec_to_long(_param);
          }
          else {
               _re->param               = cr_convert(_re, _param);
          }
     }

     if (G.debug || G.verbose) {
          struct cr_color     *_color;

          CR_DEBUG("Selector  = %d\n",   _re->idx_regex_select);
          CR_DEBUG("regex[0]  = [%s]\n", _re->regex[0]);
          CR_DEBUG("regex[1]  = %p\n",   _re->regex[1]);
          CR_DEBUG("base      = %s\n",   cr_base_to_str(_re->base));
          CR_DEBUG("operator  = %s\n",   cr_op_to_str(_re->op));
          CR_DEBUG("_param    = %s\n",   _param);

          switch (_re->base) {

          case CR_BASE_DEC:
               _fmt                = "param     = %ld\n";
               break;

          case CR_BASE_OCT:
               _fmt                = "param     = %lo\n";
               break;

          case CR_BASE_HEX:
               _fmt                = "param     = 0x%lX\n";
               break;

          case CR_BASE_ASCII:
               _fmt                = "param     = '%c'\n";
               break;

          default:
               _fmt                = "*** UNKNOWN BASE !!! ***\n";
               break;
          }
          CR_DEBUG(_fmt, _re->param);

          for (_i = 0; ; _i++) {
               _color              = _re->alt[0].alt_cols[_i];
               if (_color == NULL) {
                    break;
               }
               CR_DEBUG("Color[%d]  = (%d, %d)  (%p)\n", _i, _color->intensity, _color->col_num, _color);
          }
          CR_DEBUG("\n");
     }

     if ((_error = regcomp(&_re->reg[0], _regexp, _re->cflags)) != 0) {
          (void) regerror(_error, &_re->reg[0], _errbuf, sizeof(_errbuf));
          fprintf(stderr, "%s: regcomp error for \"%s\" : %s\n",
                  G.prgname, _regexp, _errbuf);
          exit(CR_EXIT_ERR_REGCOMP);
     }

     CR_LEAVING;
     return _re;
}

/* cr_decode_sequential() }}} */
/* cr_decode_alternate_str() {{{ */

/******************************************************************************

                              CR_DECODE_ALTERNATE_STR

******************************************************************************/
struct cr_re_desc *cr_decode_alternate_str(cr_root_args *root_args)
{
     // for Alternate option
     int                       _state, _curr_col_idx, _selector, _lg, _size;
     char                      _c, _next_c, *_regexp;
     cr_args                  *_args;
     struct cr_color         **_alt_colors;
     struct cr_re_desc        *_re;
     int                       _error, _i, _j, _trigger_num = -1;
     char                      _errbuf[256], *_p, *_option;

     CR_ENTERING;

     _re                      = cr_new_re_desc();
     _re->alternate_str       = TRUE;
     _re->cflags              = G.cflags;
     _re->col.col_num         = 0;
     _re->col.intensity       = 0;
     _re->col.out             = G.out;
     _re->max_sub             = 1;
     _re->alt[0].alt_idx      = 0;
     _re->alt[1].alt_idx      = 0;
     _re->curr_alt            = &_re->alt[0];
     _re->idx_regex_select    = 0;
     _re->matching_str        = NULL;
     _re->change_on_diff      = TRUE;
     G.out                    = stdout;
     _lg                      = 0;

     _curr_col_idx            = 0;
     _selector                = 0;

     _regexp                  = NULL;

     _args                    = root_args->args;

     if (_args == NULL) {
          _lg                           = 3;
     }
     else {
          if (*_args->argp != NULL) {
               _lg                      = strlen(*_args->argp);
          }
          if (_lg < 3) {
               _lg                      = 3;
          }
          _option                  = *_args->argp;
     }

     /* Allocate memory for the color descriptors
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _size                    = (_lg + 1) * sizeof(struct cr_color **);
     for (_i = 0; _i < (sizeof(_re->alt) / sizeof(_re->alt[0])); _i++) {
          if ((_alt_colors = (struct cr_color **) malloc(_size)) == NULL) {
               cr_error_malloc();
               exit(CR_EXIT_ERR_MALLOC);
          }
          _re->alt[_i].alt_cols    = _alt_colors;

          for (_j = 0; _j < (_lg + 1); _j++) {
               _alt_colors[_j]          = NULL;
          }
     }
     _alt_colors              =
     _re->curr_alt->alt_cols  = _re->alt[0].alt_cols;

     /* Decode options
        ~~~~~~~~~~~~~~ */
     for (_state = CR_STATE_INITIAL;_state != CR_STATE_FINAL; ) {
          cr_set_opt_pointers(root_args, &_p, &_regexp);
          _c             = cr_getopt(root_args);

          switch (_state) {

          case CR_STATE_INITIAL:
               switch (_c) {

               case '#' :
                    cr_transition(_c, &_state, CR_STATE_W_OPT_NUM);
                    break;

               case ',' :
                    _selector      = 0;
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
                    break;

               case '\0':
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;

               default:
                    if (cr_is_selector(_c)) {
                         _selector      = _c - '0';
                         cr_transition(_c, &_state, CR_STATE_W_SEPARATOR);
                    }
                    else {
                         cr_error_syntax(root_args);
                    }
                    break;
               }
               break;

          case CR_STATE_W_SELECTOR:
               switch (_c) {

               case ',' :
                    _selector      = 0;
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
                    break;

               case '\0':
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;

               default:
                    if (cr_is_selector(_c)) {
                         _selector      = _c - '0';
                         cr_transition(_c, &_state, CR_STATE_W_SEPARATOR);
                    }
                    else {
                         cr_error_syntax(root_args);
                    }
                    break;
               }
               break;

          case CR_STATE_W_OPT_NUM:
               if (cr_is_int(_c)) {
                    _trigger_num   = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLUMN);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COLUMN:
               if (_c == ':') {
                    cr_transition(_c, &_state, CR_STATE_W_SELECTOR);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_SEPARATOR:
               if (_c == ',') {
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
               }
               else if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_FINAL);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_INTENSITY:
               if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLOR);
               }
               else if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COLOR:
               if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else {
                    cr_error_invalid_color(_c);
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_END:
               if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLOR);
               }
               else if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else if (_c == ':') {
                    _alt_colors[_curr_col_idx++]  = NULL;
                    _alt_colors    = _re->alt[1].alt_cols;
                    _curr_col_idx  = 0;
                    _re->curr_alt  = &_re->alt[1];
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY_2);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_INTENSITY_2:
               if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLOR_2);
               }
               else if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END_2);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COLOR_2:
               if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END_2);
               }
               else {
                    cr_error_invalid_color(_c);
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_END_2:
               if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLOR_2);
               }
               else if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END_2);
               }
               else if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_FINAL);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          default:
               cr_error_syntax(root_args);
               break;
          }

          if (_regexp != NULL) {
               /* Regex initialized : get out of the loop
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

               break;
          }
     }

     _re->trigger_num              = _trigger_num;
     _alt_colors[_curr_col_idx++]  = NULL;

     for (_i = 0; _i < (sizeof(_re->alt[0].alt_cols) / sizeof(_re->alt[0].alt_cols[0])); _i++) {
          _alt_colors         = _re->alt[_i].alt_cols;

          if (_alt_colors[0] == NULL) {
               _alt_colors[0]           = G.deflt_alt_str_col_1;
          }
          if (_alt_colors[1] == NULL) {
               _alt_colors[1]           = G.deflt_alt_str_col_2;
          }
          if (_regexp == NULL) {
               _regexp                  = CR_DEFLT_ALT_REGEXP;
          }
     }

     _re->curr_alt            = &_re->alt[0];

     /* Count number of possible sub strings
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_p = _regexp; (*_p); _p++) {
          if (*_p == '(') {
               _re->max_sub++;
          }
     }

     if (!G.consistency) {
          _selector++;   // '0' => regexp number 1
     }
     _re->idx_regex_select    = _selector;
     _re->regex[0]            = _regexp;
     _re->regex[1]            = NULL;

     if (G.debug || G.verbose) {
          struct cr_color     *_color;

          CR_DEBUG("Selector  = %d\n",   _re->idx_regex_select);
          CR_DEBUG("regex[0]  = [%s]\n", _re->regex[0]);
          CR_DEBUG("regex[1]  = %p\n",   _re->regex[1]);
          for (_i = 0; _i < (sizeof(_re->alt) / sizeof(_re->alt[0])); _i++) {
               for (_j = 0; ; _j++) {
                    _color              = _re->alt[_i].alt_cols[_j];
                    if (_color == NULL) {
                         break;
                    }
                    CR_DEBUG("Color[%d][%d]  = (%d, %2d)  (%p)\n",
                             _i, _j, _color->intensity, _color->col_num, _color);
               }
               CR_DEBUG("\n");
          }
     }

     if ((_error = regcomp(&_re->reg[0], _regexp, _re->cflags)) != 0) {
          (void) regerror(_error, &_re->reg[0], _errbuf, sizeof(_errbuf));
          fprintf(stderr, "%s: regcomp error for \"%s\" : %s\n",
                  G.prgname, _regexp, _errbuf);
          exit(CR_EXIT_ERR_REGCOMP);
     }

     CR_LEAVING;
     return _re;
}

/* cr_decode_alternate_str() }}} */
/* cr_set_ptr() {{{ */

/******************************************************************************

                         CR_SET_PTR

******************************************************************************/
#if 0
void cr_set_ptr(int **ref_date_elt, int *date_elt_addr, char *date_type)
{
     if (*date_elt_addr != CR_UNINITIALIZED) {
          printf("%s already specified ! (previous value = %d)\n",
                 date_type, *date_elt_addr);
          exit(CR_EXIT_ERR_DUP_VALUE);
     }

     *ref_date_elt            = date_elt_addr;
}
#else
void cr_set_ptr(int **ref_date_elt, struct cr_RE_num *sub_RE_num, int idx)
{
     if (sub_RE_num->num[idx] != CR_UNINITIALIZED) {
          printf("%s already specified ! (previous value = %d)\n",
                 cr_time_period_labels[idx], sub_RE_num->num[idx]);
          exit(CR_EXIT_ERR_DUP_VALUE);
     }

     *ref_date_elt            = &sub_RE_num->num[idx];
}
#endif    /* 0 */

/* cr_set_ptr() }}} */
/* cr_pos_unspecified() {{{ */

/******************************************************************************

                         CR_POS_UNSPECIFIED

******************************************************************************/
void cr_pos_unspecified(char *date_elt_name)
{
     printf("%s position unspecified !\n", date_elt_name);
}

/* cr_pos_unspecified() }}} */
/* cr_pos_inconsistency() {{{ */

/******************************************************************************

                         CR_POS_INCONSISTENCY

******************************************************************************/
void cr_pos_inconsistency(char *date_elt_1, char *date_elt_2)
{
     if (G.verbose) {
          CR_DEBUG("\n");
     }
     fprintf(stderr, "%s: regex numbers for %s and %s are identical !\n",
             G.prgname, date_elt_1, date_elt_2);
}

/* cr_pos_inconsistency() }}} */
/* cr_disp_RE_nums() {{{ */

/******************************************************************************

                              CR_DISP_RE_NUMS

******************************************************************************/
void cr_disp_RE_nums(struct cr_RE_num *sub_RE_num)
{
     bool                 _set = FALSE;
     int                  _i;

     for (_i = CR_TIME_IDX_FIRST; _i <= CR_TIME_IDX_LAST; _i++) {
          CR_DEBUG("%-15s : RE num = %2d\n", cr_time_period_labels[_i],
                   sub_RE_num->num[_i]);
     }
}

/* cr_disp_RE_nums() }}} */
/* cr_check_time_sub_RE() {{{ */

/******************************************************************************

                         CR_CHECK_TIME_SUB_RE

******************************************************************************/
void cr_check_time_sub_RE(struct cr_RE_num *sub_RE)
{
     int                            _i, _j;

     for (_i = CR_TIME_IDX_FIRST; _i < CR_TIME_IDX_LAST; _i++) {
          if (sub_RE->num[_i] == CR_UNINITIALIZED) {
               continue;
          }
          for (_j = _i + 1; _j <= CR_TIME_IDX_LAST; _j++) {
               if (sub_RE->num[_j] == CR_UNINITIALIZED) {
                    continue;
               }
               if (sub_RE->num[_j] == sub_RE->num[_i]) {
                    cr_pos_inconsistency(cr_time_period_labels[_j],
                                         cr_time_period_labels[_i]);
                    exit(CR_EXIT_ERR_SYNTAX);
               }
          }
     }
}

/* cr_check_time_sub_RE() }}} */
/* cr_set_sub_RE_nums() {{{ */

/******************************************************************************

                         CR_SET_SUB_RE_NUMS

******************************************************************************/
void cr_set_sub_RE_nums(struct cr_RE_num *sub_RE, char *fmt_spec)
{
     char                     *_p, _c;
     int                       _state = CR_STATE_INITIAL, *_ref_date_elt;
     bool                      _get_next_char = TRUE;

     CR_DEBUG("fmt_spec = \"%s\"\n", fmt_spec);

     for (_p = fmt_spec; *_p != '\0'; ) {
          _c                       = *_p;
          _get_next_char           = TRUE;

          switch (_state) {

          case CR_STATE_INITIAL:
               if (strchr(CR_PERIOD_SPEC_CHARS, _c)) {
                    /* Switch to a new state with the same character
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _get_next_char           = FALSE;
                    cr_transition(_c, &_state, CR_STATE_M_FMT_SPEC);
               }
               else {
                    cr_transition(_c, &_state, CR_STATE_ERR_SYNTAX_ERROR);
               }
               break;

          case CR_STATE_W_FMT_SPEC:
               if (strchr(CR_PERIOD_SPEC_CHARS, _c)) {
                    /* Switch to a new state with the same character
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _get_next_char           = FALSE;
                    cr_transition(_c, &_state, CR_STATE_M_FMT_SPEC);
               }
               break;

          case CR_STATE_M_FMT_SPEC:

               switch (_c) {

               case 'Y':
                    _ref_date_elt            = &sub_RE->num[CR_TIME_IDX_Y];
                    break;

               case 'm' :
                    _ref_date_elt            = &sub_RE->num[CR_TIME_IDX_m];
                    break;

               case 'b' :
                    _ref_date_elt            = &sub_RE->num[CR_TIME_IDX_mn];
                    break;

               case 'd' :
                    _ref_date_elt            = &sub_RE->num[CR_TIME_IDX_d];
                    break;

               case 'H' :
                    _ref_date_elt            = &sub_RE->num[CR_TIME_IDX_H];
                    break;

               case 'M' :
                    _ref_date_elt            = &sub_RE->num[CR_TIME_IDX_M];
                    break;

               case 'S' :
                    _ref_date_elt            = &sub_RE->num[CR_TIME_IDX_S];
                    break;

               case 'u' :
                    _ref_date_elt            = &sub_RE->num[CR_TIME_IDX_u];
                    break;

               case 'n' :
                    _ref_date_elt            = &sub_RE->num[CR_TIME_IDX_n];
                    break;

               case '\0':
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;

               default:
                    if (isdigit(_c)) {
                         /* Switch to a new state with the same character
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _get_next_char           = FALSE;
                    }
                    else {
                         cr_transition(_c, &_state, CR_STATE_ERR_SYNTAX_ERROR);
                    }
                    break;
               }
               cr_transition(_c, &_state, CR_STATE_W_FMT_POS);
               break;

          case CR_STATE_W_FMT_POS:
               if (isdigit(_c)) {
                    *_ref_date_elt           = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_FMT_SPEC);
               }
               else {
                    cr_transition(_c, &_state, CR_STATE_ERR_SYNTAX_ERROR);
               }
               break;

          case CR_STATE_ERR_SYNTAX_ERROR:
               fprintf(stderr, "%s: syntax error in time elements specifier \"%s\"\n",
                       G.prgname, fmt_spec);
               exit(CR_EXIT_ERR_SYNTAX);
               break;

          case CR_STATE_FINAL:
               break;

          default:
               cr_transition(_c, &_state, CR_STATE_ERR_SYNTAX_ERROR);
               break;
          }

          if (_get_next_char) {
               _p++;
          }
     }
}

/* cr_set_sub_RE_nums() }}} */
/* cr_init_RE_num() {{{ */

/******************************************************************************

                              CR_INIT_RE_NUM

******************************************************************************/
void cr_init_RE_num(struct cr_RE_num *sub_RE_num)
{
     int                  _i;

     for (_i = CR_TIME_IDX_FIRST; _i <= CR_TIME_IDX_LAST; _i++) {
          sub_RE_num->num[_i]      = CR_UNINITIALIZED;
     }
}

/* cr_init_RE_num() }}} */
/* cr_is_RE_num_set() {{{ */

/******************************************************************************

                              CR_IS_RE_NUM_SET

******************************************************************************/
bool cr_is_RE_num_set(struct cr_RE_num *sub_RE_num)
{
     bool                 _set = FALSE;
     int                  _i;

     for (_i = CR_TIME_IDX_FIRST; _i <= CR_TIME_IDX_LAST; _i++) {
          CR_DEBUG("%-15s : RE num = %2d\n", cr_time_period_labels[_i],
                   sub_RE_num->num[_i]);
          _set                = _set || (sub_RE_num->num[_i] != CR_UNINITIALIZED);
     }

     return _set;
}

/* cr_is_RE_num_set() }}} */
/* cr_get_deflt_col() {{{ */

/******************************************************************************

                         CR_GET_DEFLT_COL

******************************************************************************/
struct cr_color *cr_get_deflt_col(char *env_var_name, int deflt_intensity,
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

/* cr_get_deflt_col() }}} */
/* cr_init_str() {{{ */

/******************************************************************************

                         CR_INIT_STR

******************************************************************************/
void cr_init_str(struct cr_str *S, char *s)
{
     S->s                = s;
     S->e                = s;
}

/* cr_init_str() }}} */
/* cr_init_deflt_colors() {{{ */

/******************************************************************************

                         CR_INIT_DEFLT_COLORS

******************************************************************************/
void cr_init_deflt_colors()
{
     struct cr_env_var_desc        *_pvar;
     char                          *_env_val;

     for (_pvar = cr_env_vars; _pvar->name != NULL; _pvar++) {
          *_pvar->color_desc       = cr_get_deflt_col(_pvar->name,
                                                      _pvar->deflt_intens,
                                                      _pvar->deflt_color);
     }
}

/* cr_init_deflt_colors() }}} */
#if 0
/* cr_init_deflt_thresholds() {{{ */

/******************************************************************************

                         CR_INIT_DEFLT_THRESHOLDS

******************************************************************************/
void cr_init_deflt_thresholds()
{
     struct cr_env_var_thres       *_pvar;

#if 0
     for (_pvar = cr_env_thres; _pvar->name != NULL; _pvar++) {
          cr_get_deflt_thres(_pvar);
     }
#endif    /* 0 */
}

/* cr_init_deflt_thresholds() }}} */
#endif    /* 0 */
/* cr_init_list() {{{ */

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

/* cr_init_list() }}} */
/* cr_init_desc() {{{ */

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

/* cr_init_desc() }}} */
/* cr_decode_dow() {{{ */

/******************************************************************************

                              CR_DECODE_DOW

******************************************************************************/
struct cr_re_desc *cr_decode_dow(cr_root_args *root_args)
{
     // for DOW option
     int                       _state, _curr_col_idx, _selector, _lg, _size;
     char                      _c, _next_char, *_regexp;
     cr_args                  *_args;
     struct cr_color         **_dow_colors;
     struct cr_re_desc        *_re;
     int                       _error, _i;
     char                      _errbuf[256], *_p,
                              *_possible_RE, *_possible_spec;

     char                     *_option;
     int                       _nb_sub, *_ref_date_elt, _color_count;
     struct cr_RE_num         *_sub_RE_num;

     CR_ENTERING;

     _re                      = cr_new_re_desc();
     _re->dow.used            = TRUE;
     _re->cflags              = G.cflags;
     _re->col.col_num         = 0;
     _re->col.intensity       = 0;
     _re->col.out             = G.out;
     _re->max_sub             = 1;
     _re->dow.idx             = 0;
     _re->idx_regex_select    = 0;
     _re->matching_str        = NULL;
     _re->change_on_diff      = TRUE;
     _sub_RE_num              = &_re->dow.sub_RE_num;
     G.out                    = stdout;

     _curr_col_idx            = 0;
     _selector                = 0;

     _regexp                  = NULL;
     _possible_RE             = cr_env_dow_RE;
     _possible_spec           = cr_env_dow_spec;

     cr_init_RE_num(_sub_RE_num);
     _color_count             = 0;

     _lg                      = 7;
     _args                    = root_args->args;

     if (_args != NULL) {
          _option                  = *_args->argp;
     }
     else {
          _regexp                  = cr_env_dow_RE;
          cr_transition(_c, &_state, CR_STATE_FINAL);
     }

     /* Allocate memory for the color descriptors
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _size                    = (_lg + 1) * sizeof(struct cr_color **);
     if ((_dow_colors = (struct cr_color **) malloc(_size)) == NULL) {
          cr_error_malloc();
          exit(CR_EXIT_ERR_MALLOC);
     }
     _re->dow.cols            = _dow_colors;
     _re->alt[0].alt_cols     = _dow_colors;

     for (_i = 0; _i < (_lg + 1); _i++) {
          _dow_colors[_i]          = NULL;
     }

     /* Decode options
        ~~~~~~~~~~~~~~ */
     for (_state = CR_STATE_INITIAL; _state != CR_STATE_FINAL; ) {
          cr_set_opt_pointers(root_args, &_p, &_regexp);
          _c             = cr_getopt(root_args);

          CR_DEBUG("CHAR    = '%s'\n", cr_char_to_str(_c));
          CR_DEBUG("_regexp = %p\n", _regexp);

          switch (_state) {
          
          case CR_STATE_INITIAL:
               switch (_c) {

               case 'Y' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_Y);
                    cr_transition(_c, &_state, CR_STATE_W_NUM);
                    break;

               case 'm' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_m);
                    cr_transition(_c, &_state, CR_STATE_W_NUM);
                    break;

               case 'd' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_d);
                    cr_transition(_c, &_state, CR_STATE_W_NUM);
                    break;

               case '\0':
                    /* No date format specification
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _regexp                  = cr_env_dow_RE;
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;
                    
               default:
                    cr_error_syntax(root_args);
                    break;
               }
               break;

          case CR_STATE_W_NUM:
               if (cr_is_selector(_c)) {
                    *_ref_date_elt           = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_DMY);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_DMY:
               switch (_c) {

               case 'Y' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_Y);
                    cr_transition(_c, &_state, CR_STATE_W_NUM);
                    break;

               case 'm' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_m);
                    cr_transition(_c, &_state, CR_STATE_W_NUM);
                    break;

               case 'd' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_d);
                    cr_transition(_c, &_state, CR_STATE_W_NUM);
                    break;

               case ':' :
                    cr_transition(_c, &_state, CR_STATE_W_DMY_2);
                    break;

               case ',' :
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
                    break;

               case '\0':
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;

               default:
                    cr_error_syntax(root_args);
                    break;
               }
               break;

          case CR_STATE_W_DMY_2:
               switch (_c) {

               case 'Y' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_Y);
                    cr_transition(_c, &_state, CR_STATE_W_NUM);
                    break;

               case 'm' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_m);
                    cr_transition(_c, &_state, CR_STATE_W_NUM);
                    break;

               case 'd' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_d);
                    cr_transition(_c, &_state, CR_STATE_W_NUM);
                    break;

               default:
                    cr_error_syntax(root_args);
                    break;
               }
               break;

          case CR_STATE_W_INTENSITY:
          case CR_STATE_W_INTENSITY_2:
               if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_FINAL);
               }
               else if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLOR);
               }
               else if (cr_is_a_color(_c)) {
                    _dow_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY_2);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COLOR:
               if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_FINAL);
               }
               else if (cr_is_a_color(_c)) {
                    if (_color_count++ >= CR_MAX_DAYS) {
                         fprintf(stderr, "%s: too many colors specified !\n", G.prgname);
                         cr_error_syntax(root_args);
                    }
                    _dow_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY_2);
               }
               else {
                    cr_error_invalid_color(_c);
                    cr_error_syntax(root_args);
               }
               break;

          default:
               cr_error_syntax(root_args);
               break;
          }
     }

CR_DEBUG("================> SPECIFIED REGEX = [%s]\n", _regexp);
CR_DEBUG("================> NEXT ARG        = [%s]\n", *root_args->args->argp);
CR_DEBUG("================> POSSIBLE REGEX  = [%s]\n", _possible_RE);
CR_DEBUG("================> POSSIBLE SPEC   = [%s]\n", _possible_spec);

X
     if (!cr_is_RE_num_set(_sub_RE_num)) {
          /* No date format specified : used default value
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          cr_set_sub_RE_nums(&_re->dow.sub_RE_num, _possible_spec);
     }

X
     cr_check_time_sub_RE(_sub_RE_num);

     if (_regexp == NULL) {
          _regexp                  = cr_env_dow_RE;
     }

X
     /* Count number of possible sub strings
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_p = _regexp; (*_p); _p++) {
          if (*_p == '(') {
               _re->max_sub++;
          }
     }

X
     _nb_sub                  = _re->max_sub - 1;

#if 0
     if ((_year_RE_num  > _nb_sub)
     ||  (_month_RE_num > _nb_sub)
     ||  (_day_RE_num   > _nb_sub)) {
          fprintf(stderr, "%s: not enough sub-expressions (%d)!\n",
                  G.prgname, _nb_sub);
          exit(CR_EXIT_ERR_SYNTAX);
     }
#endif

X
     if (!G.consistency) {
          _selector++;   // '0' => regexp number 1
     }
     _re->idx_regex_select    = _selector;
     _re->regex[0]            = _regexp;
     _re->regex[1]            = NULL;

X
     /* Initialize missing colors with default colors
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_i = 0; _i < CR_MAX_DAYS; _i++) {
          if (_re->dow.cols[_i] == NULL) {
               _re->dow.cols[_i]        = G.deflt_dow[_i];
          }
     }

X
     if (G.debug || G.verbose) {
          struct cr_color     *_color;

X
          CR_DEBUG("Selector  = %d\n",   _re->idx_regex_select);
          CR_DEBUG("regex[0]  = [%s]\n", _re->regex[0]);
          CR_DEBUG("regex[1]  = %p\n",   _re->regex[1]);
          for (_i = 0; ; _i++) {
               _color              = _re->dow.cols[_i];
               if (_color == NULL) {
                    break;
               }
               CR_DEBUG("Color[%d]  = \"%2s\" (%d, %d)  (%p)\n", _i,
                        cr_str_color(_color->intensity, _color->col_num),
                        _color->intensity, _color->col_num, _color);
          }
          CR_DEBUG("\n");
     }

X
     if ((_error = regcomp(&_re->reg[0], _regexp, _re->cflags)) != 0) {
          (void) regerror(_error, &_re->reg[0], _errbuf, sizeof(_errbuf));
          fprintf(stderr, "%s: regcomp error for \"%s\" : %s\n",
                  G.prgname, _regexp, _errbuf);
          exit(CR_EXIT_ERR_REGCOMP);
     }

     CR_LEAVING;
     return _re;
}

/* cr_decode_dow() }}} */
#if 0
/* cr_decode_follow() {{{ */

/******************************************************************************

                         CR_DECODE_FOLLOW

******************************************************************************/
struct cd_re_desc *cr_decode_follow(cr_root_args *root_args)
{
     /* Decode options
        ~~~~~~~~~~~~~~ */
     for (_state = CR_STATE_INITIAL; _state != CR_STATE_FINAL; ) {

          cr_set_opt_pointers(root_args, &_p, &_regexp);
          _c             = cr_getopt(root_args);

          CR_DEBUG("CHAR    = '%s'\n", cr_char_to_str(_c));
          CR_DEBUG("_regexp = %p\n", _regexp);

          switch (_state) {

          case CR_STATE_INITIAL:
               if (_c == '#') {
                    cr_transition(_c, &_state, CR_STATE_W_ID);
               }
               else if (_c == ',') {
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
               }
               else if (_c == ':') {
                    cr_transition(_c, &_state, CR_STATE_W_RE_NUM);
               }
               else if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_W_FINAL);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_ID:
               if (cr_is_id_char(_c)) {
                    cr_transition(_c, &_state, CR_STATE_W_COLUMN);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COLUMN:
               if (_c == ':') {
                    cr_transition(_c, &_state, CR_STATE_W_RE_NUM);
               }
               else if (cr_is_id_char(_c)) {
                    /* XXX CONTROLER LA TAILLE DE l'ID */
                    cr_transition(_c, &_state, CR_STATE_W_COLUMN);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_RE_NUM:
               if (cr_is_int(_c)) {
                    _selector      = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COMMA);
               }
               else if (_c == 'r') {
                    cr_transition(_c, &_state, CR_STATE_W_E_1);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COMMA:
               if (_c == ',') {
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
               }
               else if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_W_FINAL);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_INTENSITY:
               if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLOR);
               }
               else if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COLOR:
               if (cr_is_a_color(_c)) {
                    _alt_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else {
                    cr_error_invalid_color(_c);
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_E_1:
               if (_c == 'e') {
                    cr_transition(_c, &_state, CR_STATE_W_S);
               }
               else if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_S:
               if (_c == 's') {
                    cr_transition(_c, &_state, CR_STATE_W_E_2);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_E_2:
               if (_c == 'e') {
                    cr_transition(_c, &_state, CR_STATE_W_T);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_T:
               if (_c == 't') {
                    cr_transition(_c, &_state, CR_STATE_W_END);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_END:
               if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_W_FINAL);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

                default:
                    cr_error_syntax(root_args);
                    break;
                }
     }
}

/* }}} */
#endif  /* 0 */
/* cr_strcpy() {{{ */

/******************************************************************************

                         CR_STRCPY

******************************************************************************/
void cr_strcpy(struct cr_str *S, char c)
{
     *(S->e++)           = c;
     *(S->e)             = '\0';
// fprintf(G.debug_out, "S->s = [%s]\n", S->s);
}

/* cr_strcpy() }}} */
/* cr_get_str() {{{ */

/******************************************************************************

                         CR_GET_STR

******************************************************************************/
char *cr_get_str(struct cr_str *S)
{
     *(S->e)             = 0;
     return S->s;
}

/* cr_get_str() }}} */
/* cr_decode_thresholds() {{{ */

/******************************************************************************

                              CR_DECODE_THRESHOLDS

******************************************************************************/
struct cr_re_desc *cr_decode_thresholds(cr_root_args *root_args)
{
     int                       _state, _curr_col_idx, _selector, _lg, _size;
     char                      _c, _next_char, *_regexp, *_option, *_buf;
     cr_args                  *_args;
     struct cr_color         **_range_colors;
     struct cr_re_desc        *_re;
     int                       _error, _i, _idx, _nb_sub, *_ref_date_elt,
                               _thresholds_count, _base;
     char                      _errbuf[256], *_p, *_val;

     struct cr_str             _S;

     CR_ENTERING;

     _re                      = cr_new_re_desc();
     _re->threshold.used      = TRUE;
     _re->cflags              = G.cflags;
     _re->col.col_num         = 0;
     _re->col.intensity       = 0;
     _re->col.out             = G.out;
     _re->max_sub             = 1;
     _re->idx_regex_select    = 0;
     _re->matching_str        = NULL;
     _re->change_on_diff      = TRUE;
     G.out                    = stdout;

     _curr_col_idx            = 0;
     _selector                = 0;

     _regexp                  = NULL;

     _thresholds_count        = 0;
     _idx                     = 0;
     _val                     = NULL;

     _lg                      = CR_MAX_THRESHOLDS;
     _args                    = root_args->args;
     if (_args == NULL) {
          fprintf(stderr, "Incomplete option for thresholds !\n");
          exit(CR_EXIT_ERR_SYNTAX);
     }

     _option                  = *_args->argp;

     CR_DEBUG("_args          = %p\n", _args);
     CR_DEBUG("_args->argp    = %p\n", _args->argp);
     CR_DEBUG("_args->idx     = %d\n", _args->idx);
     CR_DEBUG("*_args->argp   = [%s]\n", *_args->argp);
     if ((_buf = malloc(strlen(*_args->argp))) == 0) {
          cr_error_malloc();
          exit(CR_EXIT_ERR_MALLOC);
     }

     /* Allocate memory for the color descriptors
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _size                    = (_lg + 1) * sizeof(struct cr_color **);
     if ((_range_colors = (struct cr_color **) malloc(_size)) == NULL) {
          cr_error_malloc();
          exit(CR_EXIT_ERR_MALLOC);
     }
     _re->threshold.cols      = _range_colors;
     _re->alt[0].alt_cols     = _range_colors;

     for (_i = 0; _i < (_lg + 1); _i++) {
          _range_colors[_i]          = NULL;
     }

     /* Decode options
        ~~~~~~~~~~~~~~ */
     for (_state = CR_STATE_INITIAL; _state != CR_STATE_FINAL; ) {
          CR_DEBUG("STATE   = %s\n", cr_state_to_str(root_args->state));
          CR_DEBUG("state   = %s\n", cr_state_to_str(_state));

          cr_set_opt_pointers(root_args, &_p, &_regexp);
          _c                       = cr_getopt(root_args);

          CR_DEBUG("CHAR    = '%s'\n", cr_char_to_str(_c));

          switch (_state) {
          
          case CR_STATE_INITIAL:
               switch (_c) {

               case '<' :
                    cr_transition(_c, &_state, CR_STATE_W_EQUAL);
                    break;

               case '\0':
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;

               default:
                    if (isdigit(_c)) {
                         cr_transition(_c, &_state, CR_STATE_W_BASE);
                    }
                    else {
                         cr_error_syntax(root_args);
                    }
                    break;
               }
               break;

          case CR_STATE_W_EQUAL:
               switch (_c) {

               case '=' :
                    cr_transition(_c, &_state, CR_STATE_W_NUM);
                    break;

               default:
                    cr_error_syntax(root_args);
                    break;
               }
               break;

          case CR_STATE_W_NUM:
               if (isdigit(_c)) {
                    cr_transition(_c, &_state, CR_STATE_W_BASE);
                    _selector           = _c - '0';
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_BASE:
               switch (_c) {

               case 'x' :
                    cr_transition(_c, &_state, CR_STATE_W_COMMA);
                    _base               = CR_BASE_HEX;
                    break;

               case ',' :
                    cr_transition(_c, &_state, CR_STATE_W_SIGN);
                    break;

               default:
                    cr_error_syntax(root_args);
                    break;
               }
               break;

          case CR_STATE_W_COMMA:
               switch (_c) {

               case ',' :
                    cr_transition(_c, &_state, CR_STATE_W_SIGN);
                    break;

               default:
                    cr_error_syntax(root_args);
                    break;
               }
               break;

          case CR_STATE_W_SIGN:
               switch (_c) {

               case '+' :
                    cr_transition(_c, &_state, CR_STATE_W_DIGIT);
                    cr_init_str(&_S, _buf);
                    cr_strcpy(&_S, _c);
                    break;

               case '-' :
                    cr_transition(_c, &_state, CR_STATE_W_DIGIT);
                    cr_init_str(&_S, _buf);
                    cr_strcpy(&_S, _c);
                    break;

               default:
                    if (isdigit(_c) || ((_base == CR_BASE_HEX) && isxdigit(_c))) {

                         cr_transition(_c, &_state, CR_STATE_W_COLUMN);
                         cr_init_str(&_S, _buf);
                         cr_strcpy(&_S, _c);
                    }
                    else {
                         cr_error_syntax(root_args);
                    }

                    break;
               }
               break;

          case CR_STATE_W_DIGIT:
               if (isdigit(_c) || ((_base == CR_BASE_HEX) && isxdigit(_c))) {
                    cr_transition(_c, &_state, CR_STATE_W_COLUMN);
                    cr_strcpy(&_S, _c);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COLUMN:
               CR_DEBUG("CHAR    = '%s'\n", cr_char_to_str(_c));

               switch (_c) {

               case ':' :
                    if (_thresholds_count++ >= CR_MAX_THRESHOLDS) {
                         printf("Too many thresholds specified !\n");
                         exit(CR_EXIT_ERR_SYNTAX);
                    }
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
                    _val                = cr_get_str(&_S);
CR_DEBUG("================================= val = \"%s\" idx = %d\n", _val, _idx);
                    _re->threshold.threshold[_idx++]   = strtod(_val, NULL);
CR_DEBUG("================================= VAL = %10lf idx = %d\n", _re->threshold.threshold[_idx - 1], _idx - 1);
                    break;

               case ',' :
                    if (_thresholds_count++ >= CR_MAX_THRESHOLDS) {
                         printf("Too many thresholds specified !\n");
                         exit(CR_EXIT_ERR_SYNTAX);
                    }
                    cr_transition(_c, &_state, CR_STATE_W_SIGN);
                    _val                = cr_get_str(&_S);
CR_DEBUG("================================= val = \"%s\" idx = %d\n", _val, _idx);
                    _re->threshold.threshold[_idx++]   = strtod(_val, NULL);
CR_DEBUG("================================= VAL = %10lf idx = %d\n", _re->threshold.threshold[_idx - 1], _idx - 1);
                    _curr_col_idx++;
                    break;

               case '\0':
                    if (_thresholds_count++ >= CR_MAX_THRESHOLDS) {
                         printf("Too many thresholds specified !\n");
                         exit(CR_EXIT_ERR_SYNTAX);
                    }
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    _val                = cr_get_str(&_S);
CR_DEBUG("================================= val = \"%s\" idx = %d\n", _val, _idx);
                    _re->threshold.threshold[_idx++]   = strtod(_val, NULL);
CR_DEBUG("================================= VAL = %10lf idx = %d\n", _re->threshold.threshold[_idx - 1], _idx - 1);
                    break;

               default:
                    if (isdigit(_c) || ((_base == CR_BASE_HEX) && isxdigit(_c))) {
                         cr_transition(_c, &_state, CR_STATE_W_COLUMN);
                         cr_strcpy(&_S, _c);
                    }
                    else {
                         cr_error_syntax(root_args);
                    }
                    break;
               }
CR_DEBUG("_thresholds_count = %d\n", _thresholds_count);
               break;

          case CR_STATE_W_INTENSITY:
               if (cr_is_intensity(_c)) {
                    cr_transition(_c, &_state, CR_STATE_W_COLOR);
                    G.intensity    = _c - '0';
               }
               else if (cr_is_a_color(_c)) {
                    cr_transition(_c, &_state, CR_STATE_W_COMMA_2);
                    _range_colors[_curr_col_idx]  = cr_decode_color(_c, G.intensity);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COLOR:
               if (cr_is_a_color(_c)) {
                    cr_transition(_c, &_state, CR_STATE_W_COMMA_2);
                    _range_colors[_curr_col_idx]  = cr_decode_color(_c, G.intensity);
               }
               else {
                    cr_error_invalid_color(_c);
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COMMA_2:
               switch (_c) {

               case ',' :
                    cr_transition(_c, &_state, CR_STATE_W_SIGN);
                    _curr_col_idx++;
                    break;

               case '\0':
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;

               default:
                    cr_error_syntax(root_args);
                    break;
               }
               break;

          default:
               cr_error_syntax(root_args);
               break;
          }
     }

     if (_regexp == NULL) {
          CR_DEBUG("REGEXP = NULL\n");
          _regexp        = cr_env_thres_RE;
          CR_DEBUG("regexp = [%s]\n", _regexp);
     }

     /* Count number of possible sub strings
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_p = _regexp; (*_p); _p++) {
          if (*_p == '(') {
               _re->max_sub++;
          }
     }

     _nb_sub                  = _re->max_sub - 1;

     if (_selector  > _nb_sub) {
          fprintf(stderr, "%s: not enough sub-expressions (%d)!\n",
                  G.prgname, _nb_sub);
          exit(CR_EXIT_ERR_SYNTAX);
     }

     if (_thresholds_count == 0) {
          /* No thresholds specified : set default thresholds
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          _i                                 = 0;
          _re->threshold.threshold[_i++]     = CR_DEFLT_THRES_V1;
          _re->threshold.threshold[_i++]     = CR_DEFLT_THRES_V2;
          _re->threshold.threshold[_i++]     = CR_DEFLT_THRES_V3;
          _re->threshold.threshold[_i++]     = CR_DEFLT_THRES_V4;
          _re->threshold.threshold[_i++]     = CR_DEFLT_THRES_V5;
          _re->threshold.threshold[_i++]     = CR_DEFLT_THRES_V6;

          _thresholds_count                  = _i;
     }

     if (!G.consistency) {
          _selector++;   // '0' => regexp number 1
     }
     _re->idx_regex_select         = _selector;
     _re->regex[0]                 = _regexp;
     _re->regex[1]                 = NULL;
     _re->threshold.base           = _base;
     _re->threshold.nb_thresholds  = _thresholds_count;
     CR_DEBUG("nb_thresholds = %d\n", _re->threshold.nb_thresholds);

     /* Initialize missing colors with default colors
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     CR_DEBUG2("_thresholds_count = %d\n", _thresholds_count);
     for (_i = 0; _i < CR_MAX_THRESHOLDS; _i++) {
          CR_DEBUG2("range_colors[%d] = %p\n", _i, _range_colors[_i]);
          if (_range_colors[_i] == NULL) {
               CR_DEBUG2("CR_SUM(%2d) = %3d\n", _thresholds_count, CR_SUM(_thresholds_count));
               CR_DEBUG2( "CR_IDX(%2d, %2d) = %3d\n", _thresholds_count, _i, CR_IDX(_thresholds_count, _i + 1));
               _range_colors[_i]   = G.deflt_t[CR_IDX(_thresholds_count, _i + 1)];
               CR_DEBUG("Color[%d] : using default\n", _i);
               CR_DEBUG2("_range_colors[%d] = %p\n", _i, _range_colors[_i]);
               CR_DEBUG2("THRESHOLDS USED COLORS : %2d : %d %2d\n",
                         _i, _range_colors[_i]->intensity, _range_colors[_i]->col_num);
          }
          else {
               CR_DEBUG2("_range_colors[%d] = %p\n", _i, _range_colors[_i]);
          }
     }
     _range_colors[_i]   = NULL;

     if (G.debug || G.verbose) {
          struct cr_color     *_color;

          CR_DEBUG("Selector  = %d\n",   _re->idx_regex_select);
          CR_DEBUG("regex[0]  = [%s]\n", _re->regex[0]);
          CR_DEBUG("regex[1]  = %p\n",   _re->regex[1]);

          /* DEBUG : display colors
             ~~~~~~~~~~~~~~~~~~~~~~ */
          for (_i = 0; _i < _thresholds_count; _i++) {
               CR_DEBUG2("THRESHOLDS USED COLORS : %2d : %d %2d [%10lf]\n",
                         _i, _range_colors[_i]->intensity, _range_colors[_i]->col_num,
                         _re->threshold.threshold[_i]);
          }

          CR_DEBUG2("\n");
     }

     if ((_error = regcomp(&_re->reg[0], _regexp, _re->cflags)) != 0) {
          (void) regerror(_error, &_re->reg[0], _errbuf, sizeof(_errbuf));
          fprintf(G.debug_out, "%s: regcomp error for \"%s\" : %s\n",
                  G.prgname, _regexp, _errbuf);
          exit(CR_EXIT_ERR_REGCOMP);
     }

     free(_buf);

     CR_LEAVING;
     return _re;
}

/* cr_decode_thresholds() }}} */
/* cr_decode_time() {{{ */

/******************************************************************************

                              CR_DECODE_TIME

******************************************************************************/
struct cr_re_desc *cr_decode_time(cr_root_args *root_args)
{
     // for TIME option
     int                       _state, _curr_col_idx, _selector, _lg, _size;
     char                      _c, _next_char, *_regexp, *_ref_time;
     cr_args                  *_args;
     struct cr_color         **_time_colors;
     struct cr_re_desc        *_re;
     int                       _error, _i, _incr, _period_type;
     long                      _period_length;
     char                      _errbuf[256], *_p;

     char                     *_option;
     struct cr_RE_num         *_sub_RE_num;
     int                       _nb_sub, *_ref_date_elt, _color_count,
                               _sub_RE_pos[CR_MAX_PERIODS];
     char                     *_possible_RE, *_possible_spec;
     bool                      _reference, _sub_RE_specified, _get_next_char = TRUE,
                               _start_from_0 = FALSE;
     static bool               _init_done = FALSE;

     CR_ENTERING;

     /* Get current time
        ~~~~~~~~~~~~~~~~ */
     cr_init_start_time();

     /* Initialize months management
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (_init_done == FALSE) {
          cr_init_months_names();
          _init_done               = TRUE;
     }

     _re                      = cr_new_re_desc();
     _re->time.used           = TRUE;
     _re->cflags              = G.cflags;
     _re->col.col_num         = 0;
     _re->col.intensity       = 0;
     _re->col.out             = G.out;
     _re->max_sub             = 1;
     _re->time.idx            = 0;
     _re->idx_regex_select    = 0;
     _re->matching_str        = NULL;
     _re->change_on_diff      = TRUE;
     _sub_RE_num              = &_re->time.sub_RE_num;
     G.out                    = stdout;           // XXX ??????? XXX

     _curr_col_idx            = 0;
     _selector                = 0;

     _regexp                  = NULL;
     _ref_time                = NULL;
     _reference               = FALSE;
     _possible_RE             = NULL;
     _possible_spec           = NULL;

     _incr                    = 1;
     _period_type             = CR_UNINITIALIZED;

     cr_init_RE_num(_sub_RE_num);

     _color_count             = 0;

     _lg                      = CR_MAX_PERIODS;
     _args                    = root_args->args;

     if (_args != NULL) {
          _option                  = *_args->argp;
CR_DEBUG("_option = [%s]\n", _option);
     }

     /* Allocate memory for the color descriptors
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _size                    = (_lg + 1) * sizeof(struct cr_color **);
     if ((_time_colors = (struct cr_color **) malloc(_size)) == NULL) {
          cr_error_malloc();
          exit(CR_EXIT_ERR_MALLOC);
     }
     _re->time.cols           = _time_colors;
     _re->alt[0].alt_cols     = _time_colors;

     for (_i = 0; _i < (_lg + 1); _i++) {
          _time_colors[_i]         = NULL;
     }

     /* Decode options
        ~~~~~~~~~~~~~~ */
     for (_state = CR_STATE_INITIAL; _state != CR_STATE_FINAL; ) {
          cr_set_opt_pointers(root_args, &_p, &_regexp);

#if 1
          if (_get_next_char) {
               _c                       = cr_getopt(root_args);
          }
#endif    /* 0 */

          _get_next_char           = TRUE;

          CR_DEBUG("CHAR    = '%s'\n", cr_char_to_str(_c));
          CR_DEBUG("_regexp = %p\n", _regexp);
          CR_DEBUG("_regexp = %s\n", _regexp == 0 ? "nil" : _regexp);
          CR_DEBUG("State   = %s\n", cr_state_to_str(_state));

          switch (_state) {
          
          case CR_STATE_INITIAL:
               switch (_c) {

               case 'Y' :
                    _period_type             = CR_PERIOD_YEAR;
                    _period_length           = CR_PERIOD_Y;
                    _possible_RE             = cr_env_time_RE_Y;
                    _possible_spec           = cr_env_time_spec_Y;
                    cr_transition(_c, &_state, CR_STATE_W_ZERO);
                    break;

               case 'm' :
                    _period_type             = CR_PERIOD_MONTH;
                    _period_length           = CR_PERIOD_m;
                    _possible_RE             = cr_env_time_RE_m;
                    _possible_spec           = cr_env_time_spec_m;
                    cr_transition(_c, &_state, CR_STATE_W_ZERO);
                    break;

               case 'w' :
                    _period_type             = CR_PERIOD_WEEK;
                    _period_length           = CR_PERIOD_w;
                    _possible_RE             = cr_env_time_RE_d; // XXX TODO : 7 days !!!
                    _possible_spec           = cr_env_time_spec_d;
                    cr_transition(_c, &_state, CR_STATE_W_ZERO);
                    break;

               case 'd' :
                    _period_type             = CR_PERIOD_DAY;
                    _period_length           = CR_PERIOD_d;
                    _possible_RE             = cr_env_time_RE_d;
                    _possible_spec           = cr_env_time_spec_d;
                    cr_transition(_c, &_state, CR_STATE_W_ZERO);
                    break;

               case 'H' :
                    _period_type             = CR_PERIOD_HOUR;
                    _period_length           = CR_PERIOD_H;
                    _possible_RE             = cr_env_time_RE_H;
                    _possible_spec           = cr_env_time_spec_H;
                    cr_transition(_c, &_state, CR_STATE_W_ZERO);
                    break;

               case 'M' :
                    _period_type             = CR_PERIOD_MIN;
                    _period_length           = CR_PERIOD_M;
                    _possible_RE             = cr_env_time_RE_M;
                    _possible_spec           = cr_env_time_spec_M;
                    cr_transition(_c, &_state, CR_STATE_W_ZERO);
                    break;

               case 'S' :
                    _period_type             = CR_PERIOD_SEC;
                    _period_length           = CR_PERIOD_S;
                    _possible_RE             = cr_env_time_RE_S;
                    _possible_spec           = cr_env_time_spec_S;
                    cr_transition(_c, &_state, CR_STATE_W_ZERO);
                    break;

               case 'u' :
                    _period_type             = CR_PERIOD_USEC;
                    _period_length           = CR_PERIOD_u;
                    _possible_RE             = cr_env_time_RE_u;
                    _possible_spec           = cr_env_time_spec_u;
                    cr_transition(_c, &_state, CR_STATE_W_ZERO);
                    break;

               case 'n' :
                    _period_type             = CR_PERIOD_NSEC;
                    _period_length           = CR_PERIOD_n;
                    _possible_RE             = cr_env_time_RE_n;
                    _possible_spec           = cr_env_time_spec_n;
                    cr_transition(_c, &_state, CR_STATE_W_ZERO);
                    break;

               default:
X
                    /* No time period specification
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    cr_error_syntax(root_args);
                    break;
               }
               break;

          case CR_STATE_W_ZERO:
               switch (_c) {

               case '0':
                    _start_from_0            = TRUE;
                    cr_transition(_c, &_state, CR_STATE_W_REFERENCE);
                    break;

               case 'R':
                    _reference               = TRUE;
                    cr_set_opt_param(root_args, &_ref_time);
                    cr_transition(_c, &_state, CR_STATE_W_SEPARATOR);
                    break;

               case ':':
                    _reference               = FALSE;
                    cr_transition(_c, &_state, CR_STATE_W_INCREMENT);
                    break;

               case '\0':
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;

               default:
                    cr_error_syntax(root_args);
                    break;
               }
               break;

          case CR_STATE_W_REFERENCE:
               switch (_c) {

               case 'R':
                    _reference               = TRUE;
                    cr_set_opt_param(root_args, &_ref_time);
                    cr_transition(_c, &_state, CR_STATE_W_SEPARATOR);
                    break;

               case ':':
                    _reference               = FALSE;
                    cr_transition(_c, &_state, CR_STATE_W_INCREMENT);
                    break;

               case '\0':
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;

               default:
                    cr_error_syntax(root_args);
                    break;
               }
               break;

          case CR_STATE_W_SEPARATOR:
               switch (_c) {

               case ':':
                    cr_transition(_c, &_state, CR_STATE_W_INCREMENT);
                    break;

               case '\0':
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;

               default:
                    cr_error_syntax(root_args);
                    break;
               }
               break;

          case CR_STATE_W_INCREMENT:
               if (isdigit(_c)) {
                    _incr                    = (_c - '0');
                    cr_transition(_c, &_state, CR_STATE_W_SEPARATOR_2);
                    break;
               }
               else if (strchr(CR_PERIOD_SPEC_CHARS, _c)) {
                    /* Switch to a new state with the same character
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _get_next_char           = FALSE;
                    cr_transition(_c, &_state, CR_STATE_M_FMT_SPEC);
               }
               else if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_FINAL);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_SEPARATOR_2:
               if (isdigit(_c)) {
                    _incr                    *= 10;
                    _incr                    += (_c - '0');
                    cr_transition(_c, &_state, CR_STATE_W_SEPARATOR_2);
                    break;
               }
               else if (_c == ':') {
                    cr_transition(_c, &_state, CR_STATE_W_FMT_SPEC);
               }
               else if (_c == ',') {
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
               }
               else if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_FINAL);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_FMT_SPEC:
CR_DEBUG("state_W_FMT_SPEC\n");
               if (strchr(CR_PERIOD_SPEC_CHARS, _c)) {
                    /* Switch to a new state with the same character
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _get_next_char           = FALSE;
                    cr_transition(_c, &_state, CR_STATE_M_FMT_SPEC);
               }
               break;

          case CR_STATE_M_FMT_SPEC:
CR_DEBUG("state_M_FMT_SPEC\n");
CR_DEBUG("_c = 0x%02X\n", _c);
               switch (_c) {

               case 'Y':
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_Y);
                    break;

               case 'm' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_m);
                    break;

               case 'b' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_mn);
                    break;

               case 'd' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_d);
                    break;

               case 'H' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_H);
                    break;

               case 'M' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_M);
                    break;

               case 'S' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_S);
                    break;

               case 'u' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_u);
                    break;

               case 'n' :
                    cr_set_ptr(&_ref_date_elt, _sub_RE_num, CR_TIME_IDX_n);
                    break;

               case '\0':
                    /* No date format specification
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
CR_DEBUG("==> STATE_FINAL\n");
                    _regexp                  = _possible_RE;
                    cr_transition(_c, &_state, CR_STATE_FINAL);
                    break;

               default:
                    cr_error_syntax(root_args);
                    break;
               }
               if (_state != CR_STATE_FINAL) {
                    cr_transition(_c, &_state, CR_STATE_W_FMT_POS);
               }
               break;

          case CR_STATE_W_FMT_POS:
               if (isdigit(_c)) {
                    *_ref_date_elt           = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_SEPARATOR_3);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_SEPARATOR_3:
CR_DEBUG("========================================================================\n");
               if (isdigit(_c)) {
                    *_ref_date_elt           *= 10;
                    *_ref_date_elt           += _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_SEPARATOR_3);
               }
               else if (_c == ',') {
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
               }
               else if (strchr(CR_PERIOD_SPEC_CHARS, _c)) {
                    /* Switch to a new state with the same character
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _get_next_char           = FALSE;
                    cr_transition(_c, &_state, CR_STATE_M_FMT_SPEC);
               }
               else if (_c == '\0') {
CR_DEBUG("======================================================================== NUL\n");
                    cr_transition(_c, &_state, CR_STATE_FINAL);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_INTENSITY:
               if (_c == '\0') {
                    cr_transition(_c, &_state, CR_STATE_FINAL);
               }
               else if (cr_is_intensity(_c)) {
                    G.intensity    = _c - '0';
                    cr_transition(_c, &_state, CR_STATE_W_COLOR);
               }
               else if (cr_is_a_color(_c)) {
                    _time_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
               }
               else {
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_W_COLOR:
               if (_c == '\0') {
                    cr_error_syntax(root_args);
               }
               else if (cr_is_a_color(_c)) {
                    if (_color_count++ >= CR_MAX_PERIODS) {
                         fprintf(stderr, "%s: too many colors specified !\n", G.prgname);
                         cr_error_syntax(root_args);
                    }
                    _time_colors[_curr_col_idx++]  = cr_decode_color(_c, G.intensity);
                    cr_transition(_c, &_state, CR_STATE_W_INTENSITY);
               }
               else {
                    cr_error_invalid_color(_c);
                    cr_error_syntax(root_args);
               }
               break;

          case CR_STATE_FINAL:
CR_DEBUG("==================> STATE FINAL\n");
               break;

          default:
               cr_error_syntax(root_args);
               break;
          }
     }

CR_DEBUG("================> SPECIFIED REGEX = [%s]\n", _regexp);
CR_DEBUG("================> NEXT ARG        = [%s]\n", *root_args->args->argp);
CR_DEBUG("================> POSSIBLE REGEX  = [%s]\n", _possible_RE);
CR_DEBUG("================> POSSIBLE SPEC   = [%s]\n", _possible_spec);

     if (!cr_is_RE_num_set(_sub_RE_num)) {
X
          /* No date format specified : used default value
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          cr_set_sub_RE_nums(_sub_RE_num, _possible_spec);
     }
X
// cr_disp_RE_nums(_sub_RE_num);

X
     cr_check_time_sub_RE(_sub_RE_num);

#if 0
     _sub_RE_specified        = FALSE;
     for (_i = 0; _i < sizeof(_sub_RE_pos)/sizeof(_sub_RE_pos[0]); _i++) {
          _sub_RE_pos[_i]          = 0;
     }
#endif    /* 0 */

     if (_regexp == NULL) {
          _regexp                  = _possible_RE;
     }
     _re->time.start_from_0   = _start_from_0;
     _re->time.period_type    = _period_type;
     _re->time.period_length  = _period_length * _incr;
     _re->time.coeff          = _incr;

     if (_reference) {
          /* Reference specified
             ~~~~~~~~~~~~~~~~~~~ */
     }
     else {
          /* Reference unspecified : use current time
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          cr_time_desc_copy(&_re->time.ref_time, &G.time_desc);
     }

     /* Copy time reference into the descriptor
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _re->time.ref_time.tm        = cr_new_tm();

     if (_regexp == NULL) {
          _regexp                  = _possible_RE;
     }

     /* Count number of possible sub strings
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_p = _regexp; (*_p); _p++) {
          if (*_p == '(') {
               _re->max_sub++;
          }
     }

     _nb_sub                  = _re->max_sub - 1;

#if 0
     if ((_year_RE_num  > _nb_sub)
     ||  (_month_RE_num > _nb_sub)
     ||  (_day_RE_num   > _nb_sub)
     ||  (_hour_RE_num  > _nb_sub)
     ||  (_min_RE_num   > _nb_sub)
     ||  (_sec_RE_num   > _nb_sub)
     ||  (_usec_RE_num  > _nb_sub)
     ||  (_nsec_RE_num  > _nb_sub)) {
          fprintf(stderr, "%s: not enough sub-expressions (%d)!\n",
                  G.prgname, _nb_sub);
          exit(CR_EXIT_ERR_SYNTAX);
     }
#endif

     if (!G.consistency) {
          _selector++;   // '0' => regexp number 1
     }
     _re->idx_regex_select    = _selector;
     _re->regex[0]            = _regexp;
     _re->regex[1]            = NULL;

     /* Initialize missing colors with default colors
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_i = 0; _i < CR_MAX_PERIODS; _i++) {
          if (_time_colors[_i] == NULL) {
               _time_colors[_i]         = G.deflt_time[_i];
          }
          else {
               CR_DEBUG2("_time_colors[%d] = %p\n", _i, _time_colors[_i]);
          }
     }
     _time_colors[_i]   = NULL;

     if (G.debug || G.verbose) {
          struct cr_color     *_color;

          CR_DEBUG("Selector  = %d\n",   _re->idx_regex_select);
          CR_DEBUG("regex[0]  = [%s]\n", _re->regex[0]);
          CR_DEBUG("regex[1]  = %p\n",   _re->regex[1]);
          for (_i = 0; ; _i++) {
               _color              = _re->time.cols[_i];
               if (_color == NULL) {
                    break;
               }
               CR_DEBUG("Color[%d]  = (%d%c)\n", _i, _color->intensity, CR_STR_COLORS[_color->col_num]);
          }
          CR_DEBUG("\n");
     }

X
CR_DEBUG("================> USED REGEX      = [%s]\n", _regexp);
     /* Compile regular expression
        ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if ((_error = regcomp(&_re->reg[0], _regexp, _re->cflags)) != 0) {
          (void) regerror(_error, &_re->reg[0], _errbuf, sizeof(_errbuf));
          fprintf(stderr, "%s: regcomp error for \"%s\" : %s\n",
                  G.prgname, _regexp, _errbuf);
          exit(CR_EXIT_ERR_REGCOMP);
     }

X
     if (_reference) {
X
          if (_ref_time == NULL) {
X
               fprintf(stderr, "%s: reference time missing !\n", G.prgname);
               exit(CR_EXIT_ERR_USAGE);
          }
          else {
               /* Convert ASCII reference time
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
X
               CR_DEBUG("================> REFERENCE TIME = [%s]\n", _ref_time);
               if (cr_init_ref_time(_ref_time, _re) != TRUE) {
X
                    fprintf(stderr, "%s: error while analyzing reference time !\n", G.prgname);
                    exit(CR_EXIT_ERR_REGEXEC);
               }
          }
     }
     else {
          CR_DEBUG("================> NO REFERENCE SPECIFIED : using current time\n");
          cr_time_desc_copy(&_re->time.ref_time, &G.time_desc);
     }

// X
// cr_disp_time_desc(&_re->time.ref_time);
     cr_init_bounds(&_re->time);

     CR_LEAVING;
     return _re;
}

/* cr_decode_time() }}} */
/* cr_decode_ind_color() {{{ */

/******************************************************************************

                              CR_DECODE_IND_COLOR

******************************************************************************/
struct cr_re_desc *cr_decode_ind_color(cr_root_args *rootargs)
{
     /* TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
     return NULL;        /* XXX */
}

/* cr_decode_ind_color() }}} */
/* cr_set_args() {{{ */

/******************************************************************************

                              CR_SET_ARGS

******************************************************************************/
struct cr_root_args *cr_set_args(int argc, char **argv, char *opts,
                            struct cr_configs *configs)
{
     cr_root_args             *_root_args;
     cr_args                  *_args;

     _root_args               = cr_new_root_args();
     _args                    = cr_new_args();
     _root_args->state        = CR_STATE_INITIAL;

     _args->argc              = argc;
     _args->argv              = argv;
     _args->argp              = argv + 1;    // Skip argv[0]
     _args->idx               = 0;
     _args->next_arg          = NULL;
     _args->prev              = NULL;
     _args->config            = NULL;

     _root_args->opts         = opts;
     _root_args->args         = _args;
     _root_args->configs      = configs;

     return _root_args;
}

/* cr_set_args() }}} */
/* cr_add_to_list() {{{ */

/******************************************************************************

                         CR_ADD_TO_LIST

******************************************************************************/
void cr_add_to_list(struct cr_re_desc *re)
{
CR_DEBUG("############## ADD TO LIST ###############\n");
X
     if (G.extract_RE == 0) {
X
          G.extract_RE   =
          G.insert_RE    = re;
     }
     else {
X
          G.insert_RE->next   = re;
          G.insert_RE         = re;
     }
X
}

/* cr_add_to_list() }}} */
/* cr_clear_marker_flags() {{{ */

/******************************************************************************

                         CR_CLEAR_MARKER_FLAGS

******************************************************************************/
inline void cr_clear_marker_flags(void)
{
     G.begin_specified        = FALSE;
     G.end_specified          = FALSE;
}

/* cr_clear_marker_flags() }}} */
/* cr_add_regexp() {{{ */

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
               exit(CR_EXIT_ERR_REGCOMP);
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
                    exit(CR_EXIT_ERR_REGCOMP);
               }
          }

          cr_clear_marker_flags();
          G.last_RE                = 0;
          G.last_color             = 0;
     }
}

/* cr_add_regexp() }}} */
/* cr_init_env_vars() {{{ */

/******************************************************************************

                         CR_INIT_ENV_VARS

******************************************************************************/
void cr_init_env_vars(struct cr_env_var_conf *vconfs, struct cr_env_var_desc *vars,
                      struct cr_env_var_thres *thres)
{
     struct cr_env_var_conf        *_pconf;
     struct cr_env_var_desc        *_pvar;
     struct cr_env_var_thres       *_pthres;
     char                           _deflt[4], *_undefined, *_msg, *_undef_double,
                                   *_env_val;

     for (_pconf = vconfs; _pconf->name != NULL; _pconf++) {
          if ((*(_pconf->var) = getenv(_pconf->name)) == NULL) {
               *(_pconf->var)                = _pconf->deflt_value;
               _pconf->deflt                 = TRUE;
          }
          else {
               _pconf->deflt                 = FALSE;
          }
     }

#if 0
     for (_pvar = vars; _pvar->name != NULL; _pvar++) {
          if (_pvar->skip_line) {
               fprintf(G.usage_out, "\n");
          }
          if ((_env_val = getenv(_pvar->name)) == NULL) {
               sprintf(_deflt, "%d%c", _pvar->deflt_intens, _pvar->deflt_color);
               fprintf(G.usage_out, _undefined, _pvar->name, _deflt);
          }
          else {
               fprintf(G.usage_out, _msg, _pvar->name, _env_val);
          }
     }

     for (_pthres = thres; _pthres->name != NULL; _pthres++) {
          if (_pthres->skip_line) {
               fprintf(G.usage_out, "\n");
          }
          if ((_env_val = getenv(_pthres->name)) == NULL) {
               fprintf(G.usage_out, _undef_double, _pthres->name, _pthres->deflt_value);
          }
          else {
               fprintf(G.usage_out, _msg, _pthres->name, _env_val);
          }
     }
#endif    /* 0 */
}

/* cr_init_env_vars() }}} */
/* cr_disp_env_vars() {{{ */

/******************************************************************************

                         CR_DISP_ENV_VARS

******************************************************************************/
void cr_disp_env_vars(struct cr_env_var_conf *vconfs, struct cr_env_var_desc *vars,
                      struct cr_env_var_thres *thres)
{
     struct cr_env_var_conf        *_pconf;
     struct cr_env_var_desc        *_pvar;
     struct cr_env_var_thres       *_pthres;
     char                           _deflt[4], *_undefined, *_msg, *_undef_double,
                                   *_env_val;
     int                            _sz = 15;

     _undefined          = "Environment variable %-*s is undefined. Default value = \"%s\".\n";
     _undef_double       = "Environment variable %-*s is undefined. Default value = %15.6g\n";
     _msg                = "Environment variable %-*s =  \"%s\"\n";

     for (_pconf = vconfs; _pconf->name != NULL; _pconf++) {
          if ((_env_val = getenv(_pconf->name)) == NULL) {
               fprintf(G.usage_out, _undefined, _sz, _pconf->name, _pconf->deflt_value);
          }
          else {
               fprintf(G.usage_out, _msg, _sz, _pconf->name, *_pconf->var);
          }
     }

     for (_pvar = vars; _pvar->name != NULL; _pvar++) {
          if (_pvar->skip_line) {
               fprintf(G.usage_out, "\n");
          }
          if ((_env_val = getenv(_pvar->name)) == NULL) {
               _deflt[0]      = _pvar->deflt_intens + '0';
               _deflt[1]      = _pvar->deflt_color;
               _deflt[2]      = '\0';
               fprintf(G.usage_out, _undefined, _sz, _pvar->name, _deflt);
          }
          else {
               fprintf(G.usage_out, _msg, _sz, _pvar->name, _env_val);
          }
     }

     for (_pthres = thres; _pthres->name != NULL; _pthres++) {
          if (_pthres->skip_line) {
               fprintf(G.usage_out, "\n");
          }
          if ((_env_val = getenv(_pthres->name)) == NULL) {
               fprintf(G.usage_out, _undef_double, _sz, _pthres->name, _pthres->deflt_value);
          }
          else {
               fprintf(G.usage_out, _msg, _sz, _pthres->name, _env_val);
          }
     }
}

/* cr_disp_env_vars() }}} */
/* cr_disp_regex() {{{ */

/******************************************************************************

                         CR_DISP_REGEX

******************************************************************************/
void cr_disp_regex()
{
     int                       _i, _j;
     struct cr_re_desc        *_re;

// fprintf(G.debug_out, "======================================== DISP_REGEX ==============================\n");
     for (_i = 0, _re = G.extract_RE; _re != NULL; _re = _re->next) {
          if (_re->alt[0].alt_cols == NULL) {
               printf("[%2d  ] ", ++_i);
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
                    _color              = _re->alt[0].alt_cols[_j];
                    if (_color == NULL) {
                         break;
                    }

                    printf("[%2d.%d] ", _i, _j);
                    cr_start_color(_re->alt[0].alt_cols[_j]);
                    printf("%s", _re->regex[0]);
                    cr_end_color(_re->alt[0].alt_cols[_j]);
                    printf("\n");
               }
          }
     }
// fprintf(G.debug_out, "==================================================================================\n");
}

/* cr_disp_regex() }}} */
/* print_trace() {{{ */

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

     fprintf(stderr, "%s: received signal %d.\n", __func__, signum);

     _max_addr      = sizeof(_array)/sizeof(_array[0]);
     fprintf(stderr, "Max addresses number = %u\n", (unsigned int) _max_addr);

     _num_addr      = backtrace(_array, _max_addr);
     fprintf(stderr, "size = %u\n", (unsigned int) _num_addr);

     _strings       = backtrace_symbols(_array, _num_addr);
     fprintf(stderr, "%u currently active functions :\n", (unsigned int) _num_addr);

     for (_i = 0; _i < _num_addr; _i++) {
          fprintf(stderr, "===> %s\n", _strings[_i]);
     }

     free(_strings);

     exit(CR_EXIT_ERR_SEGV);
}
#endif    /* HL_BACKTRACE */

/* print_trace() }}} */
/* cr_display_config() {{{ */

/******************************************************************************

                         CR_DISPLAY_CONFIG

******************************************************************************/
void cr_display_config(int search_mode, char *re)
{
     struct cr_config         *_config;
     char                      _c, _next_char, *_regexp;
     struct cr_re_desc        *_re;
     int                       _error, _i, _so, _eo;
     char                      _errbuf[256], *_p;
     size_t                    _nmatch  = NB_MATCH;
     regmatch_t                _pmatch[NB_MATCH];
     regex_t                   _reg;

     switch (search_mode) {

     case CR_CONF_LIST_ALL:
          fprintf(G.usage_out, "  Configurations :\n");
          break;

     case CR_CONF_SEARCH_BY_REGEXP:
          if ((_error = regcomp(&_reg, re, G.cflags)) != 0) {
               (void) regerror(_error, &_reg, _errbuf, sizeof(_errbuf));
               fprintf(stderr, "%s: regcomp error for \"%s\" : %s\n",
                       G.prgname, _regexp, _errbuf);
               exit(CR_EXIT_ERR_REGCOMP);
          }
          break;
     }

     for (_config = G.configs.extract; _config != 0; _config = _config->next) {
          switch (G.verbose) {

          case 0:
               switch (search_mode) {

               case CR_CONF_LIST_ALL:
                    fprintf(G.usage_out, "    --%s\n", _config->name);
                    break;

               case CR_CONF_SEARCH_BY_REGEXP:
                    if ((_error = regexec(&_reg, _config->name, _nmatch, _pmatch, 0)) == 0) {
                         for (_i = 0; _i < _nmatch; _i++) {
                              _so       = _pmatch[_i].rm_so;
                              _eo       = _pmatch[_i].rm_eo;
                              if (_so != -1) {
                                   fprintf(G.usage_out, "%-*s : %s\n",
                                           CR_SZ_CFG_FILE, _config->config_file, _config->name);
                              }
                         }
                    }
                    else {
                         _error    = regerror(_error, &_reg, _errbuf, sizeof(_errbuf));
                    }
                    break;
               }
               break;

          case 1:
               switch (search_mode) {

               case CR_CONF_LIST_ALL:
                    if (!G.disp_nb_opts) {
                         fprintf(G.usage_out, "%-*s : %s\n",
                                 CR_SZ_CFG_FILE, _config->config_file, _config->name);
                    }
                    else {
                         fprintf(G.usage_out, "%-*s : %3d %s\n",
                                 CR_SZ_CFG_FILE, _config->config_file,
                                 _config->nb_opts, _config->name);
                    }
                    break;

               case CR_CONF_SEARCH_BY_REGEXP:
                    if ((_error = regexec(&_reg, _config->name, _nmatch, _pmatch, 0)) == 0) {
                         for (_i = 0; _i < _nmatch; _i++) {
                              _so       = _pmatch[_i].rm_so;
                              _eo       = _pmatch[_i].rm_eo;
                              if (_so != -1) {
                                   fprintf(G.usage_out, "# %s\n%-*s :\n",
                                           _config->config_file, CR_SZ_CFG_FILE,
                                           _config->name);
                              }
                         }
                         cr_display_args(_config);
                    }
                    else {
                         _error    = regerror(_error, &_reg, _errbuf, sizeof(_errbuf));
                    }
                    break;

               }
               break;

          default:
               switch (search_mode) {

               case CR_CONF_LIST_ALL:
                    if (!G.disp_nb_opts) {
                         fprintf(G.usage_out, "%-*s : %s\n",
                                 CR_SZ_CFG_FILE, _config->config_file, _config->name);
                    }
                    else {
                         fprintf(G.usage_out, "%-*s : %3d %s\n",
                                 CR_SZ_CFG_FILE, _config->config_file,
                                 _config->nb_opts, _config->name);
                    }
                    cr_display_args(_config);
                    break;

               case CR_CONF_SEARCH_BY_REGEXP:
                    if ((_error = regexec(&_reg, _config->name, _nmatch, _pmatch, 0)) == 0) {
                         for (_i = 0; _i < _nmatch; _i++) {
                              _so       = _pmatch[_i].rm_so;
                              _eo       = _pmatch[_i].rm_eo;
                              if (_so != -1) {
                                   fprintf(G.usage_out, "# %s\n%-*s :\n",
                                           _config->config_file, CR_SZ_CFG_FILE,
                                           _config->name);
                              }
                         }
                         cr_display_args(_config);
                    }
                    else {
                         _error    = regerror(_error, &_reg, _errbuf, sizeof(_errbuf));
                    }
                    break;

               }
               break;
          }
     }

     if (re) {
          regfree(&_reg);
     }
}

/* cr_display_config() }}} */
/* cr_set_alternate_links() {{{ */

/******************************************************************************

                         CR_SET_ALTERNATE_LINKS

******************************************************************************/
void cr_set_alternate_links()
{
     struct cr_re_desc        *_re, **_alt_RE_array, **_ref_re;
     int                       _count = 0, _i = 0;
     bool                      _error = FALSE;

     /* Count alternate options
        ~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_re = G.extract_RE; _re != NULL; _re = _re->next) {
          if (_re->alternate) {
               _count++;
          }
     }

     /* Allocate temporary array for alternate RE
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if ((_alt_RE_array = (cr_re_desc **) malloc((_count + 1) * sizeof(cr_re_desc **))) == 0) {
          /* Memory allocation error
             ~~~~~~~~~~~~~~~~~~~~~~~ */
          cr_error_malloc();
          exit(CR_EXIT_ERR_MALLOC);
     }

     /* Store alternate RE addresses in the array
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_re = G.extract_RE; _re != NULL; _re = _re->next) {
          if (_re->alternate) {
               _alt_RE_array[_i++]      = _re;
          }
     }
     _alt_RE_array[_i++]      = NULL;

     /* Initialize links
        ~~~~~~~~~~~~~~~~ */
     for (_ref_re = _alt_RE_array; *_ref_re != NULL; _ref_re++) {
          _re                      = *_ref_re;
          if (_re->trigger_num >= 0) {
               if (_re->trigger_num <= _count) {
                    _re->triggering_opt      = _alt_RE_array[_re->trigger_num - 1];
               }
               else {
                    fprintf(stderr, "%s: invalid number of triggering option (%d) in option %ld\n",
                            G.prgname, _re->trigger_num, (_ref_re - _alt_RE_array) + 1);
                    _error         = TRUE;
               }
          }
     }

     if (G.debug) {
          for (_ref_re = _alt_RE_array; *_ref_re != NULL; _ref_re++) {
               _re                      = *_ref_re;
               if (_re->alternate) {
                    printf("re [%p] : alternate, ", _re);
                    if (_re->trigger_num < 0) {
                         printf("NOT triggered\n");
                    }
                    else {
                         printf("triggered by %d [%p]\n",
                                _re->trigger_num, _re->triggering_opt);
                    }
               }
          }
     }

     if (_error) {
          exit(CR_EXIT_ERR_INVALID_REFERENCE);
     }
}

/* cr_set_alternate_links() }}} */
/* main() {{{ */

/******************************************************************************

                              MAIN

******************************************************************************/
int main(int argc, char *argv[])
{
     int                       _opt, _i, _j, _lg, _argc;
     cr_root_args             *_root_args;
     struct cr_re_desc        *_re;
     char                     *_env_var_name, *_env_deflt, _deflt_color_opt[4],
                              **_argv, *_argv_deflt[4];

#if ! defined(__APPLE__)
#if defined(HL_BACKTRACE)
     sighandler_t              _previous_handler;
     
     if ((_previous_handler = signal(SIGSEGV, print_trace)) == SIG_ERR) {
          fprintf(stderr, "%s: signal() returned an error !\n", argv[0]);
          exit(CR_EXIT_ERR_SIGNAL);
     }

#endif    /* HL_BACKTRACE */
#endif    /* ! __APPLE__ */

     G.prgname           = argv[0];
     G.selector_string   = CR_SELECTOR_STRING;
     G.int_string        = CR_INT_STRING;
     G.intensity_string  = CR_INTENSITY_STRING;
     G.color_string      = CR_COLORS_STRING;
     G.op_string         = CR_OP_STRING;
     G.base_string       = CR_BASE_STRING;
     G.out               = stdout;
     G.usage_out         = stderr;
     G.debug_out         = stderr;

     cr_init_env_vars(cr_env_vars_cfg, cr_env_vars, cr_env_thres);
     cr_init_deflt_colors();
     cr_init_list();

     switch (argc) {

     case 1:
          cr_usage(FALSE);
          break;

     case 2:
          if (argv[1][0] != '-') {
               _i                       = 0;
               _deflt_color_opt[_i++]   = '-';
               _deflt_color_opt[_i++]   = G.deflt_color->intensity + '0';
               _deflt_color_opt[_i++]   = CR_STR_COLORS[G.deflt_color->col_num];
               _deflt_color_opt[_i++]   = '\0';

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

     G.intensity    = CR_DEFLT_INTENSITY;

     cr_clear_marker_flags();

     /* Decoding of arguments
        ~~~~~~~~~~~~~~~~~~~~~ */
     _root_args          = cr_set_args(_argc, _argv,
                                       "oOhHuVvEr!g!y!b!m!c!w!R!G!Y!B!M!C!W!n!DLdei12345%.!NA{I{s{J{K{T{t{a{P!p!x",
                                       &G.configs);
     while ((_opt = cr_getopt(_root_args)) != -1) {
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
               G.debug++;
               break;

          case 'D' :
               G.disp_regex   = TRUE;
               break;

          case 'L' :
               G.disp_lex     = TRUE;
               break;

          case 'r':
               cr_add_regexp(CR_RED, _root_args->optarg);
               break;

          case 'g':
               cr_add_regexp(CR_GREEN, _root_args->optarg);
               break;

          case 'y':
               cr_add_regexp(CR_YELLOW, _root_args->optarg);
               break;

          case 'b':
               cr_add_regexp(CR_BLUE, _root_args->optarg);
               break;

          case 'm':
               cr_add_regexp(CR_MAGENTA, _root_args->optarg);
               break;

          case 'c':
               cr_add_regexp(CR_CYAN, _root_args->optarg);
               break;

          case 'w':
               cr_add_regexp(CR_WHITE, _root_args->optarg);
               break;

          case 'R':
               cr_add_regexp(CR_RED_REV, _root_args->optarg);
               break;

          case 'G':
               cr_add_regexp(CR_GREEN_REV, _root_args->optarg);
               break;

          case 'Y':
               cr_add_regexp(CR_YELLOW_REV, _root_args->optarg);
               break;

          case 'B':
               cr_add_regexp(CR_BLUE_REV, _root_args->optarg);
               break;

          case 'M':
               cr_add_regexp(CR_MAGENTA_REV, _root_args->optarg);
               break;

          case 'C':
               cr_add_regexp(CR_CYAN_REV, _root_args->optarg);
               break;

          case 'W':
               cr_add_regexp(CR_WHITE_REV, _root_args->optarg);
               break;

          case 'n':
               cr_add_regexp(CR_NO_COLOR, _root_args->optarg);
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
               fprintf(stderr, "%s: version %s\n", G.prgname, "1.174");
               exit(CR_EXIT_ERR_VERSION);
               break;

          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
               G.intensity    = _opt - '0';
               break;

          case '%':
               /* Begin marker
                  ~~~~~~~~~~~~ */
               if (G.begin_specified) {
                    fprintf(stderr, "%s: begin marker without end marker !\n",
                            G.prgname);
                    exit(CR_EXIT_ERR_MARKER);
               }
               G.begin_specified    = TRUE;
               break;

          case '.':
               /* End marker
                  ~~~~~~~~~~ */
               if (!G.begin_specified) {
                    fprintf(stderr, "%s: end marker without begin marker !\n",
                            G.prgname);
                    exit(CR_EXIT_ERR_MARKER);
               }
               G.end_specified      = TRUE;
               cr_add_regexp(G.last_color, _root_args->optarg);
               break;

          case 'N':
               /* By default (i.e. without specification of -N), sub-expression (1)
                * in : (...(1)...) is numbered 1 in option -A and -I, which is
                * inconsistent with the numbering in option -s where the same
                * sub-expression is numbered 2.
                * To make -A/-I and -s consistent, the -N option has been added so that
                * the sub-expression is numbered identically in all these options.
                */
               G.consistency  = TRUE;
               break;

          case 'A':
               G.cflags  |= REG_EXTENDED;
               _re                 = cr_decode_alternate(_root_args);
               _re->change_on_diff = TRUE;
               cr_add_to_list(_re);
               break;

          case 'I':
               G.cflags  |= REG_EXTENDED;
               _re                 = cr_decode_alternate(_root_args);
               _re->change_on_diff = FALSE;
               cr_add_to_list(_re);
               break;

          case 's':
               G.cflags  |= REG_EXTENDED;
               _re                 = cr_decode_sequential(_root_args);
               _re->change_on_bad_next = TRUE;
               cr_add_to_list(_re);
               break;

          case 'J':
               G.cflags  |= REG_EXTENDED;
               _re                 = cr_decode_dow(_root_args);
               _re->dow.used       = TRUE;
               cr_add_to_list(_re);
               break;

          case 'T':
               G.cflags  |= REG_EXTENDED;
               _re                 = cr_decode_thresholds(_root_args);
               _re->threshold.used = TRUE;
               cr_add_to_list(_re);
               break;

          case 't':
               G.cflags  |= REG_EXTENDED;
// CR_DEBUG("regexp before cr_decode_time() call :\n");
// cr_disp_regex();
               _re                 = cr_decode_time(_root_args);
// CR_DEBUG("_re->alt[0].alt_cols = %p\n", _re->alt[0].alt_cols);
               _re->time.used = TRUE;
               cr_add_to_list(_re);
// CR_DEBUG("regexp after cr_decode_time() call : [%s]\n", _re->regex[0]);
// cr_disp_regex();
               break;

          case 'a':
               G.cflags  |= REG_EXTENDED;
               _re                 = cr_decode_alternate_str(_root_args);
               _re->alternate_col  = TRUE;
               cr_add_to_list(_re);
               break;

          case 'K':
               G.cflags  |= REG_EXTENDED;
               _re                 = cr_decode_ind_color(_root_args);
               _re->ind_color      = TRUE;
               cr_add_to_list(_re);
               break;

          case 'o':
               G.usage_out    = stdout;
               break;

          case 'O':
               G.debug_out    = stdout;
               break;

          case 'p':
               cr_read_config_files();
               cr_display_config(CR_CONF_SEARCH_BY_REGEXP,
                                 cr_glob_to_regexp(_root_args->optarg));
               exit(CR_EXIT_OK);
               break;

          case 'P':
               cr_read_config_files();
               cr_display_config(CR_CONF_SEARCH_BY_REGEXP, _root_args->optarg);
               exit(CR_EXIT_OK);
               break;

          case 'x':
               G.disp_nb_opts = TRUE;
               break;

          default:
X
               fprintf(stderr, "%s: unknown option '%s' !\n", G.prgname, cr_char_to_str(_opt));
               cr_usage(FALSE);
               break;
          }
     }

     cr_set_alternate_links();

     if (G.disp_regex) {
          cr_disp_regex();
     }

X
     if (G.disp_nb_opts) {
          /* Only display configuration pathname, options count and config name
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          G.verbose      = 1;
          cr_read_config_files();
          cr_display_config(CR_CONF_LIST_ALL, NULL);
X
          exit(CR_EXIT_OK);
     }
X

     cr_read_input();
X

     cr_free_RE();
X
     return CR_EXIT_OK;
}

/* main() }}} */
/* cr_usage() {{{ */

/******************************************************************************

                         CR_USAGE

******************************************************************************/
void cr_usage(bool disp_config)
{
     char                     *_env_var,           *_env_val,
                              *_env_var_conf,      *_env_val_conf,
                              *_env_var_conf_glob, *_env_val_conf_glob,
                              *_msg,               *_undefined,
                               _deflt_alt_1[4],     _deflt_alt_2[4],
                               _deflt_conf[128];

     fprintf(G.usage_out, "%s: version %s\n", G.prgname, "1.174");
     fprintf(G.usage_out, "Usage: %s [-oO][-h|-H|-V|-[[%%.]eiuvdDEL12345][-[rgybmcwRGYBMCWnAIsNpPxJTt] regexp ...][--config_name ...] ]\n",
             G.prgname);
     fprintf(G.usage_out, "  -o  : usage will be displayed on stdout (default = stderr)\n");
     fprintf(G.usage_out, "  -O  : debug messages will be displayed on stdout (default = stderr)\n");
     fprintf(G.usage_out, "  -h  : help\n");
     fprintf(G.usage_out, "  -H  : help + configuration names\n");
     fprintf(G.usage_out, "  -V  : version\n");
     fprintf(G.usage_out, "  -v  : verbose\n");
     fprintf(G.usage_out, "  -u  : do not bufferize output on stdout\n");
     fprintf(G.usage_out, "  -e  : extended regular expressions\n");
     fprintf(G.usage_out, "  -i  : ignore case\n");
     fprintf(G.usage_out, "  -E  : print on stderr\n");
     fprintf(G.usage_out, "  -r  : red\n");
     fprintf(G.usage_out, "  -g  : green\n");
     fprintf(G.usage_out, "  -y  : yellow\n");
     fprintf(G.usage_out, "  -b  : blue\n");
     fprintf(G.usage_out, "  -m  : magenta\n");
     fprintf(G.usage_out, "  -c  : cyan\n");
     fprintf(G.usage_out, "  -w  : white\n");
     fprintf(G.usage_out, "  -R  : red     (reverse video)\n");
     fprintf(G.usage_out, "  -G  : green   (reverse video)\n");
     fprintf(G.usage_out, "  -Y  : yellow  (reverse video)\n");
     fprintf(G.usage_out, "  -B  : blue    (reverse video)\n");
     fprintf(G.usage_out, "  -M  : magenta (reverse video)\n");
     fprintf(G.usage_out, "  -C  : cyan    (reverse video)\n");
     fprintf(G.usage_out, "  -W  : white   (reverse video)\n");
     fprintf(G.usage_out, "  -n  : never colorize\n");
     fprintf(G.usage_out, "  -%%c : specifies the beginning of a range colorized in color 'c'\n");
     fprintf(G.usage_out, "  -.  : specifies the end of the previous range\n");
     fprintf(G.usage_out, "  -d  : debug\n");
     fprintf(G.usage_out, "  -D  : display regular expressions\n");
     fprintf(G.usage_out, "  -L  : lex debug\n");
     fprintf(G.usage_out, "  -1  : color brightness (half-bright)\n");
     fprintf(G.usage_out, "  -2  : color brightness (normal : default)\n");
     fprintf(G.usage_out, "  -3  : color brightness (bright)\n");
     fprintf(G.usage_out, "  -4  : color brightness (underscore)\n");
     fprintf(G.usage_out, "  -5  : color brightness (blink, on some terminals)\n");
     fprintf(G.usage_out, "  -A  : alternate colors when string matched by selection regex changes\n");
     fprintf(G.usage_out, "  -I  : alternate colors when string matched by selection regex does not change\n");
     fprintf(G.usage_out, "        Syntax for alternate options : -{A|I}[[s],c1c2...cn]\n");
     fprintf(G.usage_out, "         where s is a number from 0 to 9 indicating the selection regexp number,\n");
     fprintf(G.usage_out, "         and c1, c2, ... cn are color specifiers to use\n");
     if (G.verbose) {
          fprintf(G.usage_out, "        Example : -A2,2By  '(^([^ ]+ [0-9]{2} ..:..):..)'\n");
          fprintf(G.usage_out, "        Example : -A1 '(.*([12][0-9]{3}[-/][0-9]{2}[-/][0-9]{2})[   ]+.*)'\n");
          fprintf(G.usage_out, "        Example : -I1 '^([^:]*:[^:]*:([^:]*)[:]*.*)'\n");
     }
     fprintf(G.usage_out, "        Alternate colors implies extended regular expressions (-e)\n");
     fprintf(G.usage_out, "        Syntax for the use of a 2nd colorset triggered by another option :\n");
     fprintf(G.usage_out, "         -{A|I}#a:s,c11c12...c1n:c21c22...c2p\n");
     fprintf(G.usage_out, "         where s is a number from 0 to 9 indicating the selection regexp number,\n");
     fprintf(G.usage_out, "         a is the number of the option that triggers the use of the 2nd colorset,\n");
     fprintf(G.usage_out, "         c11c12...c1n are the colors of the 1st colorset, and\n");
     fprintf(G.usage_out, "         c21c22...c2p are the colors of the 2nd colorset\n");
     if (G.verbose) {
          fprintf(G.usage_out, "        Example : -A0,2B3c '^[^ ]+ +([^ ]+) ' -A#1:0,2G3g:3r2R '^([^ ]+) '\n");
     }
     fprintf(G.usage_out, "  -a  : alternate colors at each match on a line\n");
     fprintf(G.usage_out, "        Syntax for alternate options : -a[[s],c1c2...cn]\n");
     fprintf(G.usage_out, "         where s is a number from 0 to 9 indicating the selection regexp number,\n");
     fprintf(G.usage_out, "         and c1, c2, ... cn are color specifiers to use\n");
     if (G.verbose) {
          fprintf(G.usage_out, "        Example : -a2,2By  '(^([^ ]+ [0-9]{2} ..:..):..)'\n");
          fprintf(G.usage_out, "        Example : -a1 '(.*([12][0-9]{3}[-/][0-9]{2}[-/][0-9]{2})[   ]+.*)'\n");
     }
     fprintf(G.usage_out, "        Alternate colors implies extended regular expressions (-e)\n");
     fprintf(G.usage_out, "  -s  : alternate colors when the string matched by the selection regex is the image\n");
     fprintf(G.usage_out, "        by a simple function (+, -, * or /) of the value of the previous matching string\n");
     fprintf(G.usage_out, "        Syntax for sequential control option : -s[[-+*/]p[%s]:][n],c1c2...cn]\n",
             G.base_string);
     fprintf(G.usage_out, "         where p is a positive integer (parameter),\n");
     fprintf(G.usage_out, "         n is a number from 0 to 9 indicating the selection regexp number,\n");
     fprintf(G.usage_out, "         and c1, c2, ... cn are color specifiers to use\n");
     fprintf(G.usage_out, "           d : decimal (default)\n");
     fprintf(G.usage_out, "           o : octal\n");
     fprintf(G.usage_out, "           x : hexadecimal\n");
     fprintf(G.usage_out, "           a : ascii (first character of the matching string)\n");
     fprintf(G.usage_out, "        Alternate colors implies extended regular expressions (-e)\n");
     if (G.verbose) {
          fprintf(G.usage_out, "        Example : -s1:2,3b3r  '^(#[   ]*define[      ]+[^      ]+[  ]+\\(([   ]*-?[0-9]+)\\))'\n");
          fprintf(G.usage_out, "        Example : -s-5:2,1G1B  '^(Countdown[    ]*:[      ]*([0-9]{2}))'\\(([      ]*-?[0-9]+)\\))'\n");
          fprintf(G.usage_out, "        Example : -s*2x:2,3g3m  '^(#[      ]*define[      ]+[^      ]+[  ]+0x([0-9a-fA-F]+))'\\(([     ]*-?[0-9]+)\\))'\n");
     }
     fprintf(G.usage_out, "  -J  : Colorize date according to the day of the week\n");
     fprintf(G.usage_out, "        Syntax for day of week option : -J[Ys1ms2ds3[,c1c2...c7]]\n");
     fprintf(G.usage_out, "         where :\n");
     fprintf(G.usage_out, "           s1 : number of the sub-regex for the year\n");
     fprintf(G.usage_out, "           s2 : number of the sub-regex for the month\n");
     fprintf(G.usage_out, "           s3 : number of the sub-regex for the day of the month\n");
     fprintf(G.usage_out, "        and c1c2...c7 are the optional color specifiers for Sunday to Saturday\n");
     if (G.verbose) {
          fprintf(G.usage_out, "        Example : -JY2m3d4,3R1b1g2b2g3b3r '^(.* ([0-9]{4})-([0-9]{2})-([0-9]{2}))'\n");
          fprintf(G.usage_out, "        Example : -Jm2:d3:Y4,3R1g1g1g1g1g3R '^(.* ([0-9]{2})/([0-9]{2})/([0-9]{4}))'\n");
          fprintf(G.usage_out, "        Example : -JY2:m3:d4 '^(.* ([0-9]{4})-([0-9]{2})-([0-9]{2}))'\n");
     }
     fprintf(G.usage_out, "  -T  : Colorize string according to specified thresholds values\n");
     fprintf(G.usage_out, "        Syntax for thresholds option : -Ts[x],s1[:c1],s2[:c2]...s10[:c10]\n");
     fprintf(G.usage_out, "         where :\n");
     fprintf(G.usage_out, "           s is a number from 0 to 9 indicating the selection regexp number,\n");
     fprintf(G.usage_out, "           x indicates that thresholds are in hexadecimal\n");
     fprintf(G.usage_out, "           si : value of the i-th threshold for 1 <= i <= 10\n");
     fprintf(G.usage_out, "           c1 : color of the i-th range     for 1 <= i <= 10\n");
     if (G.verbose) {
          fprintf(G.usage_out, "        Example : -T1,0,10,50,70,95,100  '(([0-9]+)%% .*)'\n");
          fprintf(G.usage_out, "        Example : -T1,0:2b,10:2g,50:2y,70:3y,95:3r,100:3R  '(([0-9]+)%% .*)'\n");
     }
     fprintf(G.usage_out, "  -t  : Colorize string according to time periods\n");
     fprintf(G.usage_out, "        Syntax for time periods option : -tp[0][R][:num][:spec][,c1c2...c10]\n");
     fprintf(G.usage_out, "         where :\n");
     fprintf(G.usage_out, "           p is a time period specifier in [YmwdHMSun]\n");
     if (G.verbose) {
          fprintf(G.usage_out, "             with the following meaning :\n");
          fprintf(G.usage_out, "               Y : year\n");
          fprintf(G.usage_out, "               m : month\n");
          fprintf(G.usage_out, "               w : week\n");
          fprintf(G.usage_out, "               d : day\n");
          fprintf(G.usage_out, "               H : hour\n");
          fprintf(G.usage_out, "               M : minute\n");
          fprintf(G.usage_out, "               S : second\n");
          fprintf(G.usage_out, "               u : micro-second\n");
          fprintf(G.usage_out, "               n : nano-second\n");
     }
     fprintf(G.usage_out, "           0 tells that the date must be framed at the beginning of the period\n");
     fprintf(G.usage_out, "           R is an optional flag telling to use an optional time reference\n");
     fprintf(G.usage_out, "             instead of the current time. The optional time reference must be\n");
     fprintf(G.usage_out, "             specified before the regex argument\n");
     fprintf(G.usage_out, "           num is an optional number of time periods (default is 1)\n");
     fprintf(G.usage_out, "           spec is a string specifying the position of date elements,\n");
     fprintf(G.usage_out, "             composed of letters in [YmbdHMSus], each one followed by the\n");
     fprintf(G.usage_out, "             number of the sub-regex it is associated to,\n");
     if (G.verbose) {
          fprintf(G.usage_out, "             with the following meaning :\n");
          fprintf(G.usage_out, "               Y : year\n");
          fprintf(G.usage_out, "               m : month\n");
          fprintf(G.usage_out, "               b : abbreviated month name\n");
          fprintf(G.usage_out, "               w : week\n");
          fprintf(G.usage_out, "               d : day\n");
          fprintf(G.usage_out, "               H : hour\n");
          fprintf(G.usage_out, "               M : minute\n");
          fprintf(G.usage_out, "               S : second\n");
          fprintf(G.usage_out, "               u : micro-second\n");
          fprintf(G.usage_out, "               n : nano-second\n");
     }
     fprintf(G.usage_out, "         and c1c2...c7 are the optional color specifiers for the time periods\n");
     fprintf(G.usage_out, "  -N  : consistent numbering of sub-expressions in -A/-I and -s\n");
     fprintf(G.usage_out, "  -p  : display configuration(s) matching glob-like expression (pattern)\n");
     fprintf(G.usage_out, "  -P  : display configuration(s) matching regexp\n");
     fprintf(G.usage_out, "  -x  : display options count for each config (with -vH options)\n");

     if (G.verbose) {
          fprintf(G.usage_out, "Buffer size = %d Ko\n", CR_SIZE / 1024);
     }

     if (G.verbose) {
          cr_disp_env_vars(cr_env_vars_cfg, cr_env_vars, cr_env_thres);
     }

     if (disp_config) {
          cr_display_config(CR_CONF_LIST_ALL, NULL);
     }
     exit(CR_EXIT_ERR_USAGE);
}

/* cr_usage() }}} */
/* cr_display_args_list() {{{ */

/******************************************************************************

                         CR_DISPLAY_ARGS_LIST

******************************************************************************/
void cr_display_args_list(struct cr_config *config)
{
     struct cr_arg            *_arg;

     for (_arg = config->extract; _arg != 0; _arg = _arg->next) {
          fprintf(G.debug_out, "      %s\n", _arg->value);
     }
     fprintf(G.debug_out, "\n");
}

/* cr_display_args_list() }}} */
/* cr_display_args() {{{ */

/******************************************************************************

                         CR_DISPLAY_ARGS

******************************************************************************/
void cr_display_args(struct cr_config *config)
{
     char                    **_argv;
     int                       _opt_displayed = FALSE, _opt_len;

#define   OPT_WIDTH           (5)

     for (_argv = config->argv + 1; *_argv != 0; _argv++) {
          if ((*_argv)[0] == '-') {
               if (_opt_displayed) {
                    fprintf(G.usage_out, "\n");
               }

               if ((*_argv)[1] == '-') {
                    /* --XXXXXXXXX : config name
                       ~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    fprintf(G.usage_out, "      %s\n", *_argv);
                    _opt_displayed           = FALSE;
               }
               else {
                    /* -XXXXXXXX : option
                       ~~~~~~~~~~~~~~~~~~ */
                    fprintf(G.usage_out, "      %s", *_argv);
                    _opt_len                 = strlen(*_argv);
                    _opt_displayed           = TRUE;
               }
          }
          else {
               /* Argument
                  ~~~~~~~~ */
               fprintf(G.usage_out, "%*s '%s'\n",
                       _opt_len < OPT_WIDTH ? OPT_WIDTH - _opt_len : 1, "",
                       *_argv);
               _opt_displayed           = FALSE;
          }
     }

     if (_opt_displayed) {
          fprintf(G.usage_out, "\n");
     }
     fprintf(G.usage_out, "\n");
}

/* cr_display_args() }}} */
/* cr_add_config() {{{ */

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

/* cr_add_config() }}} */
/* cr_add_arg() {{{ */

/******************************************************************************

                         CR_ADD_ARG

******************************************************************************/
void cr_add_arg(struct cr_arg *arg, bool is_option)
{
     struct cr_config         *_config;

     assert(G.configs.insert != 0);

     _config   = G.configs.insert;
     if(_config->insert == 0) {
          _config->insert          =
          _config->extract         = arg;
     }
     else {
          _config->insert->next    = arg;
          _config->insert          = arg;
     }

     _config->argc++;

     if (is_option) {
          _config->nb_opts++;
     }
}

/* cr_add_arg() }}} */
/* cr_free_re() {{{ */

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

/* cr_free_re() }}} */
/* cr_marker2color() {{{ */

/******************************************************************************

                         CR_MARKER2COLOR

******************************************************************************/
void cr_marker2color( struct cr_re_desc *re)
{
     int                  _i = 0, _curr_level;
     struct cr_col_desc  *_desc;

    _curr_level          = re->curr_level;
//fprintf(G.debug_out, "[%2d] >>> curr_level : %d\n", _i, _curr_level);
     for (_i = 0, _desc = G.desc; _i < G.length; _i++, _desc++) {
          switch (_desc->marker) {
          case  1:
               _curr_level++;
//fprintf(G.debug_out, "[%2d] +++ curr_level : %d\n", _i, _curr_level);
               break;

          case -1:
               if (_curr_level > 0) {
                    _curr_level--;
               }
//fprintf(G.debug_out, "[%2d] --- curr_level : %d\n", _i, _curr_level);
               break;

          case  0:
//fprintf(G.debug_out, "[%2d] === curr_level : %d\n", _i, _curr_level);
               break;

          default:
               fprintf(stderr, "%s: internal error\n", G.prgname);
               exit(CR_EXIT_ERR_INTERNAL);
               break;
          }
          if (!_desc->used) {
               if (_curr_level > 0) {
//                  fprintf(G.debug_out, "==> [%2d] [%c] Color set. Col num = %d\n",
//                          _i, G.line[_i], re->col.col_num);
                    _desc->used    = TRUE;
                    _desc->col     = &re->col;
               }
               else {
//                  fprintf(G.debug_out, "==> [%2d] [%c] NO COLOR.\n", _i, G.line[_i]);
               }
          }
          else {
//             fprintf(G.debug_out, "==> [%2d] [%c] Already colorized.\n", _i, G.line[_i]);
          }
     }

     re->curr_level = _curr_level;
//fprintf(G.debug_out, "[%2d] <<< curr_level : %d\n", _i, _curr_level);
}

/* cr_marker2color() }}} */
/* cr_set_desc() {{{ */

/******************************************************************************

                         CR_SET_DESC

******************************************************************************/
void cr_set_desc(struct cr_re_desc *re, int offset, int s, int e, int marker)
{
     int                  _i;
     struct cr_col_desc  *_desc;

//fprintf(G.debug_out, "SET_DESC : s = %3d  e = %3d  marker = %d\n", s, e, marker);
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
               fprintf(stderr, "%s: internal error, marker = %d\n",
                       G.prgname, marker);
               exit(CR_EXIT_ERR_INTERNAL);
          }

          _desc->marker  = marker;
     }
}

/* cr_set_desc() }}} */
/* cr_set_desc_alt() {{{ */

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

/* cr_set_desc_alt() }}} */
/* cr_set_desc_dow() {{{ */

/******************************************************************************

                         CR_SET_DESC_DOW

******************************************************************************/
void cr_set_desc_dow(struct cr_re_desc *re, int offset, int s, int e, struct cr_color *col)
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

/* cr_set_desc_dow() }}} */
/* cr_next_val() {{{ */

/******************************************************************************

                         CR_NEXT_VAL

******************************************************************************/
long cr_next_val(struct cr_re_desc *re)
{
     long                 _result;

     switch (re->op) {

     case CR_OP_ADD:
          _result             = re->val + re->param;
          break;

     case CR_OP_SUB:
          _result             = re->val - re->param;
          break;

     case CR_OP_MUL:
          _result             = re->val * re->param;
          break;

     case CR_OP_DIV:
          _result             = re->val / re->param;
          break;

     default:
          fprintf(stderr, "%s: internal error (operator = 0x%08X)\n",
                  G.prgname, re->op);
          exit(CR_EXIT_ERR_INTERNAL);
     }

     return _result;
}

/* cr_next_val() }}} */
/* cr_day_of_the_week() {{{ */

/******************************************************************************

                         CR_DAY_OF_THE_WEEK

******************************************************************************/
/*
 *   Computes the day of the week from YYYY, mm, dd
 *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *   From https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
 *
 *   1 <= month <= 12,
 *   year > 1752 (in the U.K.)
 */
int cr_day_of_the_week(struct cr_time_desc *t)
{
     static int          _t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
     int                  _y, _m, _d;

     _y                  = t->tm->tm_year;
     _m                  = t->tm->tm_mon;
     _d                  = t->tm->tm_mday;

     CR_DEBUG("DAY OF THE WEEK : YEAR = %4d Month = %2d Day = %2d\n",
               _y, _m, _d);

     if (_m < 3 ) {
          _y                  -= 1;
     }

     return (_y + _y/4 - _y/100 + _y/400 + _t[_m - 1] + _d) % 7;
}

/* cr_day_of_the_week() }}} */
/* cr_reset_tm() {{{ */

/******************************************************************************

                              CR_RESET_TM

******************************************************************************/
void cr_reset_tm(struct tm *tm)
{
     tm->tm_year         = 0;
     tm->tm_mon          = 0;
     tm->tm_mday         = 0;
     tm->tm_hour         = 0;
     tm->tm_min          = 0;
     tm->tm_sec          = 0;

     tm->tm_wday         = 0;
     tm->tm_yday         = 0;
     tm->tm_isdst        = 0;
}

/* cr_reset_tm() }}} */
/* cr_reset_time_desc() {{{ */

/******************************************************************************

                              CR_RESET_TIME_DESC

******************************************************************************/
void cr_reset_time_desc(struct cr_time_desc *t)
{
     bzero((char *) t, sizeof(*t));
     t->tm                    = cr_new_tm();
}

/* cr_reset_time_desc() }}} */
/* cr_read_input() {{{ */

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

     char                      _tmp_str[CR_SIZE + 1];
     char                      _matching_str[CR_SIZE + 1];
     bool                      _change;
     long                      _new_val, _expected;
     int                       _dow;
     struct cr_time_desc       _t;

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

          CR_DEBUG("LINE (length = %3d) : [%s]\n", G.length, G.line);

          /* Loop on regexp
             ~~~~~~~~~~~~~~ */
          for (_re = G.extract_RE; _re != NULL; _re = _re->next) {
               /* Loop on BEGIN / END regexp
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               for (_i = 0; _i < 2; _i++) {
// X
                    if (_re->alternate) {
                         CR_DEBUG("ALTERNATE COLORS ...\n");

                         /* Search for multiple matches on the line
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _search_no     = 1;
                         for (_off = 0, _eflags = 0;
                              _off < G.length &&
                              regexec(&_re->reg[_i], G.line + _off, _nmatch, _pmatch,
                              _eflags) == 0; _off += _e + 1, _eflags = REG_NOTBOL, _search_no++) {

                              CR_DEBUG("  Search %3d : MATCH FOR [%s] // [%s] _i = %d\n",
                                       _search_no, G.line + _off, _re->regex[_i], _i);

                              _change        = FALSE;
                              if ((_search_no == 1) && (_re->idx_regex_select < _re->max_sub)) {
                                   /* Check if the string matching the selection regexp has changed
                                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                   _j   = _re->idx_regex_select;
                                   CR_DEBUG("Search %3d : Selection regexp number = %d\n", _search_no, _j);

                                   _s   = _pmatch[_j].rm_so;
                                   _e   = _pmatch[_j].rm_eo - 1;

                                   if (G.debug) {
                                        strncpy(_tmp_str, G.line + _off + _s, _e - _s + 1);
                                        _tmp_str[_e -_s + 1]   = 0;
                                        fprintf(G.debug_out, "    OFFSET = %3d : %3d => %3d [%s] [%s] _j = %d\n",
                                                _off, _s, _e, _re->regex[_i], _tmp_str, _j);
                                   }

                                   strncpy(_matching_str, G.line + _off + _s, _e - _s + 1);
                                   _matching_str[_e -_s + 1]   = 0;
                                   CR_DEBUG("String matching the selection regexp : [%s]\n", _matching_str);

                                   if (_re->matching_str == NULL) {
                                        CR_DEBUG("NO MATCHING STRING STORED YET : storing [%s]\n", _matching_str);

                                        /* No previous match for the selection regexp
                                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                        _re->matching_str   = strdup(_matching_str);
                                   }
                                   else {
                                        CR_DEBUG("Comparing [%s] and [%s] ...\n", _re->matching_str, _matching_str);

                                        if (!strcmp(_re->matching_str, _matching_str)) {
                                             /* Current match is identical to the previous match
                                                ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                             CR_DEBUG("IDENTICAL STRINGS\n");

                                             if (!_re->change_on_diff) {
                                                  CR_DEBUG("CHANGE !!!\n");
                                                  _change   = TRUE;
                                             }
                                             else {
                                                  CR_DEBUG("No change !!!\n");
                                             }
                                        }
                                        else {
                                             /* Current match differs from the previous match
                                                ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                             CR_DEBUG("DIFFERENT STRINGS\n");
                                             free(_re->matching_str);
                                             _re->matching_str   = strdup(_matching_str);

                                             if (_re->change_on_diff) {
                                                  CR_DEBUG("CHANGE !!!\n");
                                                  _change   = TRUE;
                                             }
                                             else {
                                                  CR_DEBUG("No change !!!\n");
                                             }
                                        }
                                   }
                              }

                              _re->change    = _change;
                              if (_change) {
                                   if (_re->triggering_opt)  {
                                        /* Triggering option exists
                                           ~~~~~~~~~~~~~~~~~~~~~~~~ */
                                        if (_re->triggering_opt->change) {
                                             /* Use first colors set
                                                ~~~~~~~~~~~~~~~~~~~~ */
                                             _re->curr_alt       = &_re->alt[0];

                                             /* Reset index of 2nd colors set
                                                ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                             _re->alt[1].alt_idx = -1;
                                        }
                                        else {
                                             /* Use second colors set
                                                ~~~~~~~~~~~~~~~~~~~~~ */
                                             _re->curr_alt       = &_re->alt[1];
                                        }
                                   }

                                   /* Color change : select next color from current set
                                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                   _re->curr_alt->alt_idx++;
                                   if (_re->curr_alt->alt_cols[_re->curr_alt->alt_idx] == NULL) {
                                        _re->curr_alt->alt_idx      = 0;
                                   }
                              }

                              /* Loop on substrings
                                 ~~~~~~~~~~~~~~~~~~ */
                              for (_j = 0; _j < _re->max_sub; _j++) {
                                   CR_DEBUG("%s %s(%d) : _j = %d\n", __func__, __FILE__, __LINE__, _j);

                                   if (_j == 0 && _pmatch[1].rm_so != -1) {
                                        continue;
                                   }

                                   _s   = _pmatch[_j].rm_so;
                                   _e   = _pmatch[_j].rm_eo - 1;

                                   if (G.debug) {
                                        strncpy(_tmp_str,
                                                G.line + _off + _s, _e - _s + 1);
                                        _tmp_str[_e -_s + 1]   = 0;
                                        fprintf(G.debug_out,
                                                "    OFFSET = %3d : %3d => %3d [%s] [%s] _j = %d\n",
                                                _off, _s, _e, _re->regex[_i], _tmp_str, _j);
                                   }

                                   if (_s >= 0) {
                                        cr_set_desc_alt(_re, _off, _s, _e,
                                                        _re->curr_alt->alt_cols[_re->curr_alt->alt_idx]);

                                        if (G.debug) {
                                             cr_dump_color(_re->curr_alt->alt_cols[_re->curr_alt->alt_idx]);
                                             fprintf(G.debug_out, "    offset = %d, [%d => %d], col = %d\n",
                                                     _off, _s, _e,
                                                     _re->curr_alt->alt_cols[_re->curr_alt->alt_idx]->col_num);
//                                             fprintf(G.debug_out, "\n");
                                        }
                                   }
                              }

                              CR_DEBUG("***** CHANGE COLOR ***** : alt_idx = %d\n",
                                       _re->curr_alt->alt_idx);

                              /* To handle empty strings
                                 ~~~~~~~~~~~~~~~~~~~~~~~ */
                              if (_e < 0) {
                                   _e   = 0;
                              }
                         }
                         break;    // Only 1 regex for alternate
                    }
                    if (_re->alternate_str) {
                         CR_DEBUG("ALTERNATE STRING COLORS ...\n");

                         /* Search for multiple matches on the line
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _search_no     = 1;
                         for (_off = 0, _eflags = 0;
                              _off < G.length &&
                              regexec(&_re->reg[_i], G.line + _off, _nmatch, _pmatch,
                              _eflags) == 0; _off += _e + 1, _eflags = REG_NOTBOL, _search_no++) {

                              CR_DEBUG("  Search %3d : MATCH FOR [%s] // [%s] _i = %d\n",
                                       _search_no, G.line + _off, _re->regex[_i], _i);

                              if ((_search_no == 1) && (_re->idx_regex_select < _re->max_sub)) {
                                   /* Get the string matched by the selection
                                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                   _j   = _re->idx_regex_select;
                                   CR_DEBUG("Search %3d : Selection regexp number = %d\n", _search_no, _j);

                                   _s   = _pmatch[_j].rm_so;
                                   _e   = _pmatch[_j].rm_eo - 1;

                                   if (G.debug) {
                                        strncpy(_tmp_str, G.line + _off + _s, _e - _s + 1);
                                        _tmp_str[_e -_s + 1]   = 0;
                                        fprintf(G.debug_out, "    OFFSET = %3d : %3d => %3d [%s] [%s] _j = %d\n",
                                                _off, _s, _e, _re->regex[_i], _tmp_str, _j);
                                   }

                                   strncpy(_matching_str, G.line + _off + _s, _e - _s + 1);
                                   _matching_str[_e -_s + 1]   = 0;
                                   CR_DEBUG("String matching the selection regexp : [%s]\n", _matching_str);

                              }

                              /* Loop on substrings
                                 ~~~~~~~~~~~~~~~~~~ */
                              for (_j = 0; _j < _re->max_sub; _j++) {
                                   CR_DEBUG("%s %s(%d) : _j = %d\n", __func__, __FILE__, __LINE__, _j);

                                   if (_j == 0 && _pmatch[1].rm_so != -1) {
                                        continue;
                                   }

                                   _s   = _pmatch[_j].rm_so;
                                   _e   = _pmatch[_j].rm_eo - 1;

                                   if (G.debug) {
                                        strncpy(_tmp_str,
                                                G.line + _off + _s, _e - _s + 1);
                                        _tmp_str[_e -_s + 1]   = 0;
                                        fprintf(G.debug_out,
                                                "    OFFSET = %3d : %3d => %3d [%s] [%s] _j = %d\n",
                                                _off, _s, _e, _re->regex[_i], _tmp_str, _j);
                                   }

                                   if (_s >= 0) {
                                        cr_set_desc_alt(_re, _off, _s, _e,
                                                        _re->curr_alt->alt_cols[_re->curr_alt->alt_idx]);

                                        if (G.debug) {
                                             cr_dump_color(_re->curr_alt->alt_cols[_re->curr_alt->alt_idx]);
                                             fprintf(G.debug_out, "    offset = %d, [%d => %d], col = %d\n",
                                                     _off, _s, _e,
                                                     _re->curr_alt->alt_cols[_re->curr_alt->alt_idx]->col_num);
//                                             fprintf(G.debug_out, "\n");
                                        }
                                   }
                              }

                              /* Color change : select next color from current set
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                              _re->curr_alt->alt_idx++;
                              if (_re->curr_alt->alt_cols[_re->curr_alt->alt_idx] == NULL) {
                                   _re->curr_alt->alt_idx      = 0;
                              }

                              CR_DEBUG("***** CHANGE COLOR ***** : alt_idx = %d\n",
                                       _re->curr_alt->alt_idx);

                              /* To handle empty strings
                                 ~~~~~~~~~~~~~~~~~~~~~~~ */
                              if (_e < 0) {
                                   _e   = 0;
                              }
                         }
                         break;    // Only 1 regex for alternate
                    }
                    else if (_re->sequential) {
                         CR_DEBUG("CHECK SEQUENTIALITY ...\n");

                         /* Search for multiple matches on the line
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _search_no     = 1;
                         for (_off = 0, _eflags = 0;
                              _off < G.length &&
                              regexec(&_re->reg[_i], G.line + _off, _nmatch, _pmatch,
                              _eflags) == 0; _off += _e + 1, _eflags = REG_NOTBOL, _search_no++) {

                              CR_DEBUG("  Search %3d : MATCH FOR [%s] // [%s] _i = %d\n",
                                       _search_no, G.line + _off, _re->regex[_i], _i);

                              _change        = FALSE;
                              if ((_search_no == 1) && (_re->idx_regex_select < _re->max_sub)) {
                                   /* Check if the string matching the selection regexp has changed
                                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                   _j   = _re->idx_regex_select;
                                   CR_DEBUG("Search %3d : Selection regexp number = %d\n", _search_no, _j);

                                   _s   = _pmatch[_j].rm_so;
                                   _e   = _pmatch[_j].rm_eo - 1;

                                   if (G.debug) {
                                        strncpy(_tmp_str, G.line + _off + _s, _e - _s + 1);
                                        _tmp_str[_e -_s + 1]   = 0;
                                        fprintf(G.debug_out, "    OFFSET = %3d : %3d => %3d [%s] [%s] _j = %d\n",
                                                _off, _s, _e, _re->regex[_i], _tmp_str, _j);
                                   }

                                   strncpy(_matching_str, G.line + _off + _s, _e - _s + 1);
                                   _matching_str[_e -_s + 1]   = 0;
                                   CR_DEBUG("String matching the selection regexp : [%s]\n", _matching_str);

                                   if (_re->matching_str == NULL) {
                                        CR_DEBUG("NO MATCHING STRING STORED YET : storing [%s]\n", _matching_str);

                                        /* No previous match for the selection regexp
                                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                        _re->matching_str   = strdup(_matching_str);
                                        _re->val            = cr_convert(_re, _matching_str);
                                   }
                                   else {
                                        CR_DEBUG("Comparing [%s] and [%s] ...\n", _re->matching_str, _matching_str);

                                        _new_val            = cr_convert(_re, _matching_str);
                                        _expected           = cr_next_val(_re);

                                        if (_new_val == _expected) {
                                             /* Current match is the expected value
                                                ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                             CR_DEBUG("EXPECTED VALUE\nNo change !!!\n");
                                        }
                                        else {
                                             /* Current match differs from the previous match
                                                ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                             CR_DEBUG("UNEXPECTED VALUE\nCHANGE !!!\n");
                                             _change   = TRUE;
                                        }

                                        free(_re->matching_str);
                                        _re->matching_str   = strdup(_matching_str);
                                        _re->val            = _new_val;
                                   }
                              }

                              if (_change) {
                                   /* Color change : select next color
                                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                   _re->alt[0].alt_idx++;
                                   if (_re->alt[0].alt_cols[_re->alt[0].alt_idx] == NULL) {
                                        _re->alt[0].alt_idx   = 0;
                                   }

                                   CR_DEBUG("***** CHANGE COLOR ***** : alt_idx = %d\n",
                                            _re->alt[0].alt_idx);
                              }

                              /* Loop on substrings
                                 ~~~~~~~~~~~~~~~~~~ */
                              for (_j = 0; _j < _re->max_sub; _j++) {
                                   CR_DEBUG("%s %s(%d) : _j = %d\n", __func__, __FILE__, __LINE__, _j);

                                   if (_j == 0 && _pmatch[1].rm_so != -1) {
                                        continue;
                                   }

                                   _s   = _pmatch[_j].rm_so;
                                   _e   = _pmatch[_j].rm_eo - 1;

                                   if (G.debug) {
                                        strncpy(_tmp_str,
                                                G.line + _off + _s, _e - _s + 1);
                                        _tmp_str[_e -_s + 1]   = 0;
                                        fprintf(G.debug_out,
                                                "    OFFSET = %3d : %3d => %3d [%s] [%s] _j = %d\n",
                                                _off, _s, _e, _re->regex[_i], _tmp_str, _j);
                                   }

                                   if (_s >= 0) {
                                        cr_set_desc_alt(_re, _off, _s, _e,
                                                        _re->alt[0].alt_cols[_re->alt[0].alt_idx]);

                                        if (G.debug) {
                                             cr_dump_color(_re->alt[0].alt_cols[_re->alt[0].alt_idx]);
                                             fprintf(G.debug_out, "    offset = %d, [%d => %d], col = %d\n",
                                                     _off, _s, _e,
                                                     _re->alt[0].alt_cols[_re->alt[0].alt_idx]->col_num);
                                        }
                                   }
                              }

                              /* To handle empty strings
                                 ~~~~~~~~~~~~~~~~~~~~~~~ */
                              if (_e < 0) {
                                   _e   = 0;
                              }
                         }
                         break;    // Only 1 regex for sequential
                    }
                    else if (_re->dow.used) {
                         CR_DEBUG("DOW COLORS ...\n");

                         /* Search for multiple matches on the line
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _search_no     = 1;
                         for (_off = 0, _eflags = 0;
                              _off < G.length &&
                              regexec(&_re->reg[_i], G.line + _off, _nmatch, _pmatch,
                              _eflags) == 0; _off += _e + 1, _eflags = REG_NOTBOL, _search_no++) {

                              CR_DEBUG("  Search %3d : MATCH FOR [%s] // [%s] _i = %d\n",
                                       _search_no, G.line + _off, _re->regex[_i], _i);

                              /* Reset date elements
                                 ~~~~~~~~~~~~~~~~~~~ */
                              cr_reset_time_desc(&_t);
                              _dow                = CR_UNINITIALIZED;

                              /* Search for the date : loop on substrings
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                              for (_j = 0; _j < _re->max_sub; _j++) {
                                   CR_DEBUG("%s %s(%d) : _j = %d\n", __func__, __FILE__, __LINE__, _j);

                                   if (_j == 0 && _pmatch[1].rm_so != -1) {
                                        continue;
                                   }

                                   _s   = _pmatch[_j].rm_so;
                                   _e   = _pmatch[_j].rm_eo - 1;

                                   strncpy(_tmp_str, G.line + _off + _s, _e - _s + 1);
                                   _tmp_str[_e -_s + 1]   = 0;

                                   CR_DEBUG("    OFFSET = %3d : %3d => %3d [%s] [%s] _j = %d\n",
                                                _off, _s, _e, _re->regex[_i], _tmp_str, _j);


                                   if (_j == _re->dow.sub_RE_num.num[CR_TIME_IDX_Y]) {
                                        _t.tm->tm_year = atoi(_tmp_str);
                                        CR_DEBUG("Year  found (RE_num = %d) : %4d\n",
                                                 _re->dow.sub_RE_num.num[CR_TIME_IDX_Y], _t.tm->tm_year);
                                   }
                                   else if (_j == _re->dow.sub_RE_num.num[CR_TIME_IDX_m]) {
                                        _t.tm->tm_mon  = atoi(_tmp_str);
                                        CR_DEBUG("Month found (RE_num = %d) : %4d\n",
                                                 _re->dow.sub_RE_num.num[CR_TIME_IDX_m], _t.tm->tm_mon);
                                   }
                                   if (_j == _re->dow.sub_RE_num.num[CR_TIME_IDX_d]) {
                                        _t.tm->tm_mday = atoi(_tmp_str);
                                        CR_DEBUG("Day   found (RE_num = %d) : %4d\n",
                                                 _re->dow.sub_RE_num.num[CR_TIME_IDX_d], _t.tm->tm_mday);
                                   }

                                   if (_t.tm->tm_year != 0 && _t.tm->tm_mon != 0 && _t.tm->tm_mday != 0) {
//                                        _dow           = cr_day_of_the_week(_year, _month, _day);
                                        _dow           = cr_day_of_the_week(&_t);
                                        CR_DEBUG("Day of week = %d (%4d-%02d-%02d)\n",
                                                 _dow, _t.tm->tm_year, _t.tm->tm_mon, _t.tm->tm_mday);
                                   }

                                   if (G.debug) {
X
                                        cr_disp_time_desc(&_t);
                                   }
                              }

                              /* Colorize : loop on substrings
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                              if (_dow != CR_UNINITIALIZED) {
                                   for (_j = 0; _j < _re->max_sub; _j++) {
                                        CR_DEBUG("%s %s(%d) : _j = %d\n", __func__, __FILE__, __LINE__, _j);

                                        if (_j == 0 && _pmatch[1].rm_so != -1) {
                                             continue;
                                        }

                                        _s   = _pmatch[_j].rm_so;
                                        _e   = _pmatch[_j].rm_eo - 1;

                                        strncpy(_tmp_str, G.line + _off + _s, _e - _s + 1);
                                        _tmp_str[_e -_s + 1]   = 0;

                                        CR_DEBUG("    OFFSET = %3d : %3d => %3d [%s] [%s] _j = %d\n",
                                                     _off, _s, _e, _re->regex[_i], _tmp_str, _j);

                                        if (_s >= 0) {
                                             cr_set_desc_dow(_re, _off, _s, _e, _re->dow.cols[_dow]);

                                             if (G.debug) {
                                                  cr_dump_color(_re->dow.cols[_re->dow.idx]);
                                                  CR_DEBUG("    offset = %d, [%d => %d], col = %d\n",
                                                          _off, _s, _e, _re->dow.cols[_re->dow.idx]->col_num);
                                                  CR_DEBUG("\n");
                                             }
                                        }
                                   }
                              }

                              /* To handle empty strings
                                 ~~~~~~~~~~~~~~~~~~~~~~~ */
                              if (_e < 0) {
                                   _e   = 0;
                              }
                         }
                         break;    // Only 1 regex for DOW
                    }
                    else if (_re->threshold.used) {
                         double                    _value;
                         int                       _t, _range_idx;

                         CR_DEBUG("THRESHOLDS COLORS ...\n");

                         /* Search for multiple matches on the line
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _search_no     = 1;
                         for (_off = 0, _eflags = 0;
                              _off < G.length &&
                              regexec(&_re->reg[_i], G.line + _off, _nmatch, _pmatch,
                              _eflags) == 0; _off += _e + 1, _eflags = REG_NOTBOL, _search_no++) {

                              CR_DEBUG("  Search %3d : MATCH FOR [%s] // [%s] _i = %d\n",
                                           _search_no, G.line + _off, _re->regex[_i], _i);
                              CR_DEBUG("  Search %3d :  LINE : [%s] :\n", _search_no, G.line + _off);

                              _range_idx          = 0;

                              /* Search for the range : loop on substrings
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                              for (_j = 0; _j < _re->max_sub; _j++) {
                                   CR_DEBUG("%s %s(%d) : _j = %d\n", __func__, __FILE__, __LINE__, _j);

                                   if (_j == 0 && _pmatch[1].rm_so != -1) {
                                        continue;
                                   }

                                   _s                       = _pmatch[_j].rm_so;
                                   _e                       = _pmatch[_j].rm_eo - 1;

                                   strncpy(_tmp_str, G.line + _off + _s, _e - _s + 1);
                                   _tmp_str[_e -_s + 1]     = 0;
                                   _value                   = atof(_tmp_str);

                                   CR_DEBUG("    OFFSET = %3d : %3d => %3d [%s] [%s] _j = %d\n",
                                                _off, _s, _e, _re->regex[_i], _tmp_str, _j);
                                   CR_DEBUG("    THRESHOLDS : value = %g\n", _value);

                                   /* Search for the right color
                                      ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                   _range_idx               = 0;
                                   for (_t = _re->threshold.nb_thresholds - 1; _t >= 0; _t--) {
                                        if (_value >= _re->threshold.threshold[_t]) {
                                             CR_DEBUG("    Threshold : value = %g threshold[%d] = %g color found ! num = %d\n",
                                                      _value, _t, _re->threshold.threshold[_t], _t);
                                             _range_idx               = _t;
                                             break;
                                        }
                                   }
                              }

                              CR_DEBUG("     THRESHOLDS : _range_idx = %d\n", _range_idx);

                              /* Colorize : loop on substrings
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//                              if (_dow != CR_UNINITIALIZED) {
                                   for (_j = 0; _j < _re->max_sub; _j++) {
                                        CR_DEBUG("%s %s(%d) : _j = %d\n", __func__, __FILE__, __LINE__, _j);

                                        if (_j == 0 && _pmatch[1].rm_so != -1) {
                                             continue;
                                        }

                                        _s   = _pmatch[_j].rm_so;
                                        _e   = _pmatch[_j].rm_eo - 1;

                                        if (G.debug) {
                                             strncpy(_tmp_str,
                                                     G.line + _off + _s, _e - _s + 1);
                                             _tmp_str[_e -_s + 1]   = 0;
                                             fprintf(G.debug_out,
                                                     "    OFFSET = %3d : %3d => %3d [%s] [%s] _j = %d\n",
                                                     _off, _s, _e, _re->regex[_i], _tmp_str, _j);
                                        }

                                        strncpy(_tmp_str, G.line + _off + _s, _e - _s + 1);
                                        _tmp_str[_e -_s + 1]   = 0;

                                        if (_s >= 0) {
                                             cr_set_desc_dow(_re, _off, _s, _e, _re->threshold.cols[_range_idx]);

                                             if (G.debug) {
                                                  cr_dump_color(_re->threshold.cols[_range_idx]);
                                                  fprintf(G.debug_out, "    offset = %d, [%d => %d], col = %d\n",
                                                          _off, _s, _e, _re->threshold.cols[_range_idx]->col_num);
//                                                fprintf(G.debug_out, "\n");
                                             }
                                        }
                                   }
//                              }

                              /* To handle empty strings
                                 ~~~~~~~~~~~~~~~~~~~~~~~ */
                              if (_e < 0) {
                                   _e   = 0;
                              }
                         }

                         CR_DEBUG("END OF THRESHOLDS COLORS\n");
                         break;    // Only 1 regex for thresholds
                    }
                    else if (_re->time.used) {
                         double                    _value;
                         int                       _period_idx;
                         struct cr_time_desc       _t;

                         CR_DEBUG("=========================> TIME PERIOD COLORS ...\n");

                         /* Search for multiple matches on the line
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _search_no     = 1;
                         for (_off = 0, _eflags = 0;
                              _off < G.length &&
                              regexec(&_re->reg[_i], G.line + _off, _nmatch, _pmatch,
                              _eflags) == 0; _off += _e + 1, _eflags = REG_NOTBOL, _search_no++) {

                              CR_DEBUG("  Search %3d : MATCH FOR [%s] // [%s] _i = %d\n",
                                           _search_no, G.line + _off, _re->regex[_i], _i);
                              CR_DEBUG("  Search %3d :  LINE : [%s] :\n", _search_no, G.line + _off);

                              /* Reset date elements
                                 ~~~~~~~~~~~~~~~~~~~ */
                              cr_reset_time_desc(&_t);
                              _period_idx         = 0;

                              /* Search for the date : loop on substrings
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                              for (_j = 0; _j < _re->max_sub; _j++) {
                                   CR_DEBUG("%s %s(%d) : _j = %d\n", __func__, __FILE__, __LINE__, _j);

                                   if (_j == 0 && _pmatch[1].rm_so != -1) {
                                        continue;
                                   }

                                   _s                       = _pmatch[_j].rm_so;
                                   _e                       = _pmatch[_j].rm_eo - 1;

                                   strncpy(_tmp_str, G.line + _off + _s, _e - _s + 1);
                                   _tmp_str[_e -_s + 1]     = 0;
                                   _value                   = atol(_tmp_str);

                                   CR_DEBUG("    [sub-exp %2d] OFFSET = %3d : %3d -> %3d regex = [%s] match = [%s]\n",
                                                _j, _off, _s, _e, _re->regex[_i], _tmp_str);
                                   CR_DEBUG("    [sub-exp %2d] DATE ELT : value = %g\n", _j, _value);
                                   CR_DEBUG("\n");
#if 0
X
cr_disp_time_desc(&_re->time.ref_time);
X
cr_disp_time_desc(&_t);
#endif    /* 0 */

                                   if (_j == _re->time.sub_RE_num.num[CR_TIME_IDX_Y]) {
                                        _t.tm->tm_year = atoi(_tmp_str) - 1900;
                                        CR_DEBUG("Year  found (RE_num = %d) : %4d\n",
                                                 _re->time.sub_RE_num.num[CR_TIME_IDX_Y], _t.tm->tm_year);
                                   }
                                   else if (_j == _re->time.sub_RE_num.num[CR_TIME_IDX_m]) {
                                        _t.tm->tm_mon  = atoi(_tmp_str) - 1;
                                        CR_DEBUG("Month found (RE_num = %d) :   %02d\n",
                                                  _re->time.sub_RE_num.num[CR_TIME_IDX_m], _t.tm->tm_mon);
                                   }
                                   else if (_j == _re->time.sub_RE_num.num[CR_TIME_IDX_mn]) {
                                        _t.tm->tm_mon  = cr_get_month_num(_tmp_str) - 1;
                                        CR_DEBUG("Month found (RE_num = %d) :   %02d\n",
                                                 _re->time.sub_RE_num.num[CR_TIME_IDX_mn], _t.tm->tm_mon);
                                   }
                                   else if (_j == _re->time.sub_RE_num.num[CR_TIME_IDX_d]) {
                                        _t.tm->tm_mday = atoi(_tmp_str);
                                        CR_DEBUG("Day   found (RE_num = %d) :   %2d\n",
                                                 _re->time.sub_RE_num.num[CR_TIME_IDX_d], _t.tm->tm_mday);
                                   }
                                   else if (_j == _re->time.sub_RE_num.num[CR_TIME_IDX_H]) {
                                        _t.tm->tm_hour = atoi(_tmp_str);
                                        CR_DEBUG("Hour  found (RE_num = %d) :   %02d\n",
                                                 _re->time.sub_RE_num.num[CR_TIME_IDX_H], _t.tm->tm_hour);
                                   }
                                   else if (_j == _re->time.sub_RE_num.num[CR_TIME_IDX_M]) {
                                        _t.tm->tm_min  = atoi(_tmp_str);
                                        CR_DEBUG("Min   found (RE_num = %d) :   %02d\n",
                                                 _re->time.sub_RE_num.num[CR_TIME_IDX_M], _t.tm->tm_min);
                                   }
                                   else if (_j == _re->time.sub_RE_num.num[CR_TIME_IDX_S]) {
                                        _t.tm->tm_sec  = atoi(_tmp_str);
                                        CR_DEBUG("Sec   found (RE_num = %d) :   %02d\n",
                                                 _re->time.sub_RE_num.num[CR_TIME_IDX_S], _t.tm->tm_sec);
                                   }
                                   else if (_j == _re->time.sub_RE_num.num[CR_TIME_IDX_u]) {
                                        _t.nsec        = atoi(_tmp_str) * 1000;
                                        CR_DEBUG("µsec  found (RE_num = %d) : %06ld\n",
                                                 _re->time.sub_RE_num.num[CR_TIME_IDX_u], _t.nsec);
                                   }
                                   else if (_j == _re->time.sub_RE_num.num[CR_TIME_IDX_n]) {
                                        _t.nsec          = atoi(_tmp_str);
                                        CR_DEBUG("nsec  found (RE_num = %d) : %09ld\n",
                                                 _re->time.sub_RE_num.num[CR_TIME_IDX_n], _t.nsec);
                                   }
                              }

                              if (G.debug) {
X
                                   cr_disp_time_desc(&_t);
                              }

                              if (_t.tm->tm_year == 0) {
                                   _t.tm->tm_year = G.time_desc.tm->tm_year;
                                   CR_DEBUG("Year  found (RE_num = %d) : %4d\n",
                                            _re->time.sub_RE_num.num[CR_TIME_IDX_Y], _t.tm->tm_year);
                              }

                              _t.tm->tm_isdst          = -1;

                              /* Convert ASCII date to calendar  time
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                              if ((_t.tspec.tv_sec = mktime(_t.tm)) == -1) {
                                   perror("mktime");
                                   exit(CR_EXIT_ERR_MKTIME);
                              }

                              _t.tm->tm_isdst          = G.time_desc.tm->tm_isdst;

                              /* Search for the right color
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
X
                              _t.tspec.tv_nsec         = _t.nsec;
                              _t.nsec_epoch            = ((_t.tspec.tv_sec * CR_10e9)
                                                       +   _t.tspec.tv_nsec);

                              CR_DEBUG("TIME SINCE THE EPOCH : %s\n", ctime(&_t.tspec.tv_sec));
                              CR_DEBUG("TIME SINCE THE EPOCH : %19ld nano-seconds since the Epoch\n",
                                       _t.nsec_epoch);
                              CR_DEBUG("     TIME PERIODS : _idx = ...\n");

                              _period_idx              = cr_get_period_idx(&_re->time, &_t);

                              /* Colorize : loop on substrings
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                              for (_j = 0; _j < _re->max_sub; _j++) {
                                   CR_DEBUG("%s %s(%d) : _j = %d\n", __func__, __FILE__, __LINE__, _j);

                                   if (_j == 0 && _pmatch[1].rm_so != -1) {
                                        continue;
                                   }

                                   _s   = _pmatch[_j].rm_so;
                                   _e   = _pmatch[_j].rm_eo - 1;

                                   if (G.debug) {
                                        strncpy(_tmp_str,
                                                G.line + _off + _s, _e - _s + 1);
                                        _tmp_str[_e -_s + 1]   = 0;
                                        fprintf(G.debug_out,
                                                "    OFFSET = %3d : %3d => %3d [%s] [%s] _j = %d\n",
                                                _off, _s, _e, _re->regex[_i], _tmp_str, _j);
                                   }

                                   strncpy(_tmp_str, G.line + _off + _s, _e - _s + 1);
                                   _tmp_str[_e -_s + 1]   = 0;

                                   if (_s >= 0) {
                                        cr_set_desc_dow(_re, _off, _s, _e, _re->time.cols[_period_idx]);

                                        if (G.debug) {
                                             cr_dump_color(_re->time.cols[_period_idx]);
                                             fprintf(G.debug_out, "    offset = %d, [%d => %d], col = %d\n",
                                                     _off, _s, _e, _re->time.cols[_period_idx]->col_num);
                                        }
                                   }
                              }

                              /* To handle empty strings
                                 ~~~~~~~~~~~~~~~~~~~~~~~ */
                              if (_e < 0) {
                                   _e   = 0;
                              }
                         }
CR_DEBUG("_i = %d _off = %d\n", _i, _off);

                         CR_DEBUG("END OF TIME PERIODS COLORS\n");
CR_DEBUG("=========================> END OF TIME PERIODS COLORS\n");
                         break;
                    }
                    else {
                         if (_re->regex[_i]) {
                              if (_i == 1 && _re->begin_is_end) {
                                   break;
                              }
CR_DEBUG("G.line = [%s]\n", G.line);
CR_DEBUG("regex = [%s]\n", _re->regex[_i]);

                              /* Search for multiple matches on the line
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                              for (_off = 0, _eflags = 0;
                                   _off < G.length &&
                                   regexec(&_re->reg[_i], G.line + _off, _nmatch, _pmatch,
                                   _eflags) == 0; _off += _e + 1, _eflags = REG_NOTBOL) {

                                   if (G.debug) {
                                        fprintf(G.debug_out, "  MATCH FOR    [%s] : [%s]\n",
                                                _re->regex[_i], G.line + _off);
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
                                             strncpy(_tmp_str,
                                                     G.line + _off + _s, _e - _s + 1);
                                             _tmp_str[_e -_s + 1]   = 0;
                                             fprintf(G.debug_out,
                                                     "    Offset = %3d [%3d => %3d] marker = %2d [%s]\n",
                                                     _off, _s, _e, _marker, _tmp_str);
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

#if 0
                                        if (G.debug) {
                                             fprintf(G.debug_out, "    offset = %3d :  %3d => %4d, marker = %d\n",
                                                     _off, _s, _e, _marker);
//                                             fprintf(G.debug_out, "\n");
                                        }
#endif
                                   }

                                   /* To handle empty strings
                                      ~~~~~~~~~~~~~~~~~~~~~~~ */
                                   if (_e < 0) {
                                        _e   = 0;
                                   }
                              }

                              CR_DEBUG("  No match for [%s]\n", _re->regex[_i]);
                         }
                    }
               }

               if (_re->regex[1]) {
                    cr_marker2color(_re);
               }

          }

          CR_DEBUG("\n");

          cr_disp_line();
     }
}

/* cr_read_input() }}} */
/* cr_start_color() {{{ */

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

               case 5:
                    fprintf(_out, "\033[07;05;%dm", 30 + _col_num - 8);
                    break;

               default:
                    fprintf(stderr, "%s: invalid color brightness ! (col = %p : (%d, %d))\n",
                            G.prgname, col, col->col_num, col->intensity);
                    exit(CR_EXIT_ERR_SYNTAX);
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

          case 5:
               fprintf(_out, "\033[05;%dm", 30 + _col_num);    // Blink
               break;

          default:
               fprintf(stderr, "%s: invalid color brightness ! (col = %p : (%d, %d))\n",
                       G.prgname, col, col->col_num, col->intensity);
               exit(CR_EXIT_ERR_SYNTAX);
          }
     }
}

/* cr_start_color() }}} */
/* cr_end_color() {{{ */

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

/* cr_end_color() }}} */
/* cr_same_colors() {{{ */

/******************************************************************************

                         CR_SAME_COLORS

******************************************************************************/
inline bool cr_same_colors(struct cr_color *col1, struct cr_color *col2)
{
     return (col1->col_num   == col2->col_num)
         && (col1->intensity == col2->intensity);
}

/* cr_same_colors() }}} */
/* cr_disp_line() {{{ */

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

/* cr_disp_line() }}} */

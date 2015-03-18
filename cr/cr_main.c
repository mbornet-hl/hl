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
 *	Auteur       :     Martial BORNET (MB) - 3 Janvier 2015
 *
 *	Description  :     Programme de colorisation  de chaines
 *
 *	Fichier      :     cr_main.c
 *
 *	@(#)	cr_main.c	1.17	15/03/18	MB	
 *
 *	Liste des fonctions de ce fichier :
 *	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *	- main
 *	- cr_usage
 *	- cr_init_list
 *	- cr_init_col_names
 *	- cr_set_color
 *	- cr_read_input
 *	- cr_start_color
 *	- cr_end_color
 *	- cr_init_desc
 *	- cr_set_desc
 *	- cr_disp_line
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "cr_epri.h"

#define	X			 fprintf(stderr, "%s(%d)\n", __FILE__, __LINE__);

/******************************************************************************

						MAIN

******************************************************************************/
int main(int argc, char *argv[])
{
	int					 _opt, _i;
	struct cr_color		*_col;

	G.prgname		= argv[0];
	G.out		= stdout;

	if (argc == 1) {
		cr_usage();
	}

	cr_init_list();
	cr_init_col_names();

	/* Analyse des arguments
	   ~~~~~~~~~~~~~~~~~~~~~ */
	while ((_opt = getopt(argc, argv, "huvEr:g:y:b:m:c:w:DdeiR")) != -1) {
		switch (_opt) {
		case 'h':
			cr_usage();
			break;

		case	'E':
			G.out		= stderr;
			break;

		case 'd' :
			G.debug		= TRUE;
			break;

		case	'D' :
			G.disp_regex	= TRUE;
			break;

		case	'r':
			cr_set_color(CR_RED, optarg);
			break;

		case	'g':
			cr_set_color(CR_GREEN, optarg);
			break;

		case	'y':
			cr_set_color(CR_YELLOW, optarg);
			break;

		case 'b':
			cr_set_color(CR_BLUE, optarg);
			break;

		case	'm':
			cr_set_color(CR_MAGENTA, optarg);
			break;

		case	'c':
			cr_set_color(CR_CYAN, optarg);
			break;

		case	'w':
			cr_set_color(CR_WHITE, optarg);
			break;

		case 'e':
			G.cflags	|= REG_EXTENDED;
			break;

		case 'i':
			G.cflags	|= REG_ICASE;
			break;

		case 'R':
			G.reverse	 = TRUE;
			break;

		case	'u':
			setvbuf(stdout, (char *) 0, _IONBF, 0);
			break;

		case	'v':
			fprintf(stderr, "%s: version %s\n", G.prgname, "1.17");
			exit(1);
			break;

		default:
			fprintf(stderr, "%s: unknown option '%c' !\n", G.prgname, _opt);
			cr_usage();
			break;
		}
	}
	G.cflags	|= REG_NEWLINE;

	if (G.disp_regex) {
		for (_i = 0; _i < G.idx_list; _i++) {
			_col			= &G.color_RE[G.list[_i]];
			printf("[%2d] ", _i + 1);
			cr_start_color(NULL, _col->col_num);
			printf("%s\n", _col->RE.regex);
			cr_end_color(NULL);
		}
	}

	cr_read_input();
	return 0;
}

/******************************************************************************

					CR_USAGE

******************************************************************************/
void cr_usage(void)
{
	fprintf(stderr, "Usage: %s [-h|-eidDR][-E][-rgybmcw] regexp\n", G.prgname);
	fprintf(stderr, "  -h : help\n");
	fprintf(stderr, "  -v : version\n");
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
	fprintf(stderr, "  -R : reverse video\n");
	fprintf(stderr, "  -d : debug\n");
	fprintf(stderr, "  -D : display regular expressions\n");
	exit(1);
}

/******************************************************************************

					CR_INIT_LIST

******************************************************************************/
void cr_init_list(void)
{
	int					 _i;

	for (_i = 0; _i < CR_NB_COLORS; _i++) {
		G.list[_i]	= -1;
	}
	G.idx_list	= 0;
}

/******************************************************************************

					CR_INIT_COL_NAMES

******************************************************************************/
void cr_init_col_names(void)
{
	CR_INIT_COL(1);
	CR_INIT_COL(2);
	CR_INIT_COL(3);
	CR_INIT_COL(4);
	CR_INIT_COL(5);
	CR_INIT_COL(6);
	CR_INIT_COL(7);
}

/******************************************************************************

					CR_SET_COLOR

******************************************************************************/
void cr_set_color(int color, char *regexp)
{
	int					 _idx;
	struct cr_color		*_col;

	_idx			= CR_COLOR_IDX(color);

	_col			= &G.color_RE[_idx];

	if (_col->used) {
		/* Couleur deja specifiee
		   ~~~~~~~~~~~~~~~~~~~~~~ */
		fprintf(stderr, "%s: duplicated color (%s)\n", G.prgname, _col->col_name);
		exit(1);
	}
	G.list[G.idx_list++]	= _idx;

	_col->used			= TRUE;
	_col->col_num			= color;
	_col->RE.cflags		= G.cflags;
	_col->RE.regex			= regexp;
	_col->out				= G.out;
	G.out				= stdout;

	if (regcomp(&_col->RE.reg, regexp, _col->RE.cflags) != 0) {
		fprintf(stderr, "%s: regcomp error for \"%s\" !\n", G.prgname, regexp);
		exit(1);
	}
}

/******************************************************************************

					CR_READ_INPUT

******************************************************************************/
void cr_read_input(void)
{
	int					 _i, _j, _n, _s = 0, _e = 0, _off;
	struct cr_color		*_col;
	size_t				 _nmatch;
	regmatch_t			 _pmatch[CR_SIZE + 1];
	int					 _eflags = 0;

	char					 _debug_str[CR_SIZE + 1];

	_nmatch		= sizeof(_pmatch) / sizeof(_pmatch[0]);

	for (_n = sizeof(G.line); fgets(G.line, _n, stdin) != 0; _n = sizeof(G.line)) {
		/* Reinitialisation des descripteurs de couleurs
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		cr_init_desc();

		G.length		= strlen(G.line);

		for (_i = 0; _i < G.idx_list; _i++) {
			_col			= &G.color_RE[G.list[_i]];

			for (_off = 0;  regexec(&_col->RE.reg, G.line + _off, _nmatch, _pmatch, _eflags) == 0; _off += _e + 1) {
				if (G.debug) {
					fprintf(stderr, "LINE : [%s] :\n", G.line + _off);
				}

				for (_j = 0; _pmatch[_j].rm_so != -1; _j++) {
					_s	= _pmatch[_j].rm_so;
					_e	= _pmatch[_j].rm_eo - 1;

					if (G.debug) {
						strncpy(_debug_str, G.line + _off + _s, _e - _s + 1);
						_debug_str[_e -_s + 1]	= 0;
						fprintf(stderr, "OFFSET = %3d : %3d => %3d [%s] [%s]\n", _off, _s, _e, _col->RE.regex, _debug_str);
					}

					cr_set_desc(_col, _off, _s, _e);
				}

				/* Pour traiter les chaines vides
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				if (_e < 0) {
					fprintf(stderr, "%s: warning : empty match !\n", G.prgname);
					exit(1);
//					break;
				}
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
	FILE					*_out;

	if (col)	_out = col->out;
	else		_out = stdout;

	if (G.reverse) {
		fprintf(_out, "\033[01;30m\033[01;%dm", 40 + color);
	}
	else {
		fprintf(_out, "\033[01;%dm", 30 + color);
	}
}

/******************************************************************************

					CR_END_COLOR

******************************************************************************/
void cr_end_color(struct cr_color *col)
{
	FILE					*_out;

	if (col)	_out = col->out;
	else		_out = stdout;

	fprintf(_out, "\033[0m");
}

/******************************************************************************

					CR_INIT_DESC

******************************************************************************/
void cr_init_desc(void)
{
	struct cr_col_desc		*_desc;

	G.length		= 0;

	for (_desc = G.desc; _desc < (&G.desc[sizeof(G.desc) / sizeof(G.desc[0])]);  _desc++) {
		_desc->col		= NULL;
		_desc->used		= FALSE;
	}
}

/******************************************************************************

					CR_SET_DESC

******************************************************************************/
void cr_set_desc(struct cr_color *col, int offset, int s, int e)
{
	int			 	 _i;
	struct cr_col_desc	*_desc;

	for (_i = s, _desc = &G.desc[offset + s]; _i <= e; _i++, _desc++) {
		if (!_desc->used) {
			_desc->used	= TRUE;
			_desc->col	= col;
		}
	}
}

/******************************************************************************

					CR_DISP_LINE

******************************************************************************/
void cr_disp_line(void)
{
	int			 	 _i;
	struct cr_col_desc	*_desc;

	for (_i = 0, _desc = G.desc; _i < G.length; _i++, _desc++) {
		if (G.line[_i] == '\n') {
			if (G.curr_col) {
				cr_end_color(G.curr_col);
				putc(G.line[_i], G.curr_col->out);
			}
			else {
				putc(G.line[_i], stdout);
			}
		}
		else if (_desc->used) {
			/* Le caractere est en couleur
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			if (G.curr_col == NULL) {
				/* Le caractere precedent n'etait pas en couleur
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				cr_start_color(_desc->col, _desc->col->col_num);
				putc(G.line[_i], _desc->col->out);
			}
			else {
				/* Le caractere precedent etait en couleur
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				if (_desc-> col == G.curr_col) {
					/* Pas de changement de couleur
					   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
					putc(G.line[_i], G.curr_col->out);
				}
				else {
					/* Changement de couleur
					   ~~~~~~~~~~~~~~~~~~~~~ */
					cr_end_color(G.curr_col);
					cr_start_color(_desc->col, _desc->col->col_num);
					putc(G.line[_i], _desc->col->out);
				}
			}
		}
		else {
			/* Le caractere n'est pas en couleur
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			if (G.curr_col == NULL) {
				/* Le caractere precedent n'etait pas en couleur
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				putc(G.line[_i], stdout);
			}
			else {
				/* Le caractere precedent etait en couleur
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				cr_end_color(G.curr_col);
				putc(G.line[_i], G.curr_col->out);
			}
		}
		G.curr_col	= _desc->col;
		if (G.line[_i] == '\n') {
			G.curr_col	= NULL;
		}
	}
	_desc--;
	if (_desc->used) {
		cr_end_color(G.curr_col);
	}
}

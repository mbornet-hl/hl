%{
/* ============================================================================
 *   (C) Copyright Martial Bornet, 2015.
 *
 *   Author       :     Martial BORNET (MB) - 17th of June, 2015
 *
 *   Description  :     Lexical analyzer
 *
 *   File         :     cr_lex.l
 *
 *   @(#)  cr_lex.l  1.11  21/06/29  MB  
 *
 * ============================================================================
 */

#include	<stdio.h>
#include  <string.h>
#include  "cr_epri.h"

int					fileno(FILE *);

%}

comment				([ \t]*#.*\n)
config                   ([a-zA-Z][-._a-zA-Z0-9_]*)
options                  (-[a-zA-Z0-9%.][-+*\/a-zA-Z0-9%.,:]*)
args                     ('.*')
space                    ([ \t]+)
call_config              (--[a-zA-Z][-.a-zA-Z0-9_]*)
sep                      (:)

%s                       CONFIG ARGS

%%

^{comment}               {
						yytext[strlen(yytext) - 1]	= 0;
                              CR_DISP_LEX("COMMENT : [%s]\n", yytext);
                         }

{config}                 {
                              struct cr_config         *_config;

                              BEGIN CONFIG;
                              CR_DISP_LEX("CONFIG  : %s\n", yytext);

                              _config        		= cr_new_config();
                              _config->name  		= strdup(yytext);
						_config->config_file	= G.cfg_filename;

                              cr_add_config(_config);
                         }

<CONFIG>{sep}$           {
                              BEGIN ARGS;
                              CR_DISP_LEX("SEP\n");
                         }

<CONFIG>{sep}{space}$    {
                              BEGIN ARGS;
                              CR_DISP_LEX("Spaces after delimiter !\n");
                         }

<ARGS>{call_config}      {
                              struct cr_arg            *_arg;

                              CR_DISP_LEX("CFG     : [%s]\n", yytext);
                              _arg           = cr_new_arg();
                              _arg->value    = strdup(yytext);

                              cr_add_arg(_arg);
                         }

<ARGS>{options}          {
                              struct cr_arg            *_arg;

                              CR_DISP_LEX("OPTIONS : [%s]\n", yytext);
                              _arg           = cr_new_arg();
                              _arg->value    = strdup(yytext);

                              cr_add_arg(_arg);
                         }

<ARGS>{args}             {
                              struct cr_arg            *_arg;
                              int                       _l;

                              CR_DISP_LEX("ARGS    : [%s]\n", yytext);
                              _l             = strlen(yytext);
                              yytext[_l - 1] = 0;
                              _arg           = cr_new_arg();
                              _arg->value    = strdup(yytext + 1);

                              cr_add_arg(_arg);
                         }

<ARGS>^[ \t]*\n          {
                              BEGIN 0;
                         }

.|\n                     ;

%%

#if 0
/******************************************************************************

                         MAIN

******************************************************************************/
int main()
{
     yylex();
     return 0;
}
#endif

/******************************************************************************

                         YYWRAP

******************************************************************************/
int yywrap()
{
     return 1;
}

/******************************************************************************

                         YYERROR

******************************************************************************/
void yyerror(char *message)
{
     fprintf(stderr, "%s\n", message);
}

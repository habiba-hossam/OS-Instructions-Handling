
/*
 *
 * CS-413 Spring 98
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 */

%{

#include <string.h>
#include "y.tab.h"
#include <stdio.h>

%}

%%

\n 	{
		return NEWLINE;
	}

[ \t] 	{
		/* Discard spaces and tabs */
     	}	

">" 	{
		return GREAT;
	}
"&" {
	return ANDPERCENT;
}	
">>" {
	return CONCATENATE;
}
"<" {
	return ADDINPUT;
}
"|" {
	return PIPE;
}
"exit" {
	yylval.exit= strdup(yytext);
	return EXIT;
}
"cd" {
	yylval.cd = strdup(yytext);
	return CHANGEDIR;
}
"&>>" {
	return ERRORAPPEND;
}
"&>" {
	return ERRORREPLACE;
}
"echo" {
	yylval.echo = strdup(yytext);
	return ECHO;
}
[-+]*[A-Za-z/0-9*\.][*A-Za-z0-9/\.]*|\.\.[\/\.\.]* {
		/* Assume that file names have only alpha chars */
		yylval.string_val = strdup(yytext);
		return WORD;
	}
.	{
		/* Invalid character in input */
		return NOTOKEN;
	}



%%
	

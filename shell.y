
/*
 * CS-413 Spring 98
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * you must extend it to understand the complete shell grammar
 *
 */

%token	<string_val> WORD
%token	<cd>  CHANGEDIR
%token	<exit>  EXIT
%token	<echo>  ECHO


%token 	NOTOKEN GREAT NEWLINE ANDPERCENT CONCATENATE ADDINPUT PIPE BACKWARD ERRORREPLACE ERRORAPPEND

%union	{
		char   *string_val;
		char *cd;
		char *exit;
		char *echo;
	}

%{
extern "C" 
{
	int yylex();
	void yyerror (char const *s);
}
#define yylex yylex
#include <stdio.h>
#include "command.h"
%}

%%

goal:	
	commands
	;

commands: 
	command
	| commands command 
	;

command: simple_command;

simple_command:	
	command_and_args iomodifier_opt NEWLINE{
		printf("   Yacc: Execute command\n");
		Command::_currentCommand.execute();
	}
	|command_and_args iomodifier_opt PIPE {
		Command::_currentCommand._pipeEntered = 1;
		//Command::_currentCommand.execute();
	}
	| NEWLINE 
	| error NEWLINE { yyerrok; }
	;

command_and_args:
	command_word arg_list {
		Command::_currentCommand.
			insertSimpleCommand( Command::_currentSimpleCommand );
	}
	;

arg_list:
	arg_list argument
	| /* can be empty */
	;

   
argument:
	WORD {
            printf("   Yacc: insert argument \"%s\"\n", $1);
	        Command::_currentSimpleCommand->insertArgument( $1 );
	}
	|CHANGEDIR {
		printf("   Yacc: insert argument \"%s\"\n", $1);
	        Command::_currentSimpleCommand->insertArgument( $1 );

	}
	|EXIT {
		printf("   Yacc: insert argument \"%s\"\n", $1);
	    Command::_currentSimpleCommand->insertArgument( $1 );
	}
	
	; 

command_word:

	WORD {
           printf("   Yacc: insert command \"%s\"\n", $1);
	       Command::_currentSimpleCommand = new SimpleCommand();
	       Command::_currentSimpleCommand->insertArgument( $1 );
	}
	|CHANGEDIR 
	{
		 printf("   Yacc: insert command \"%s\"\n", $1);
	       Command::_currentSimpleCommand = new SimpleCommand();
	       Command::_currentSimpleCommand->insertArgument( $1 );
		   Command::_currentCommand._changeDir = 1;
	}|
	|ECHO{
		printf("   Yacc: insert command \"%s\"\n", $1);
	       Command::_currentSimpleCommand = new SimpleCommand();
	       Command::_currentSimpleCommand->insertArgument( $1);
		   Command::_currentCommand._echoing = 1;
	}

	| EXIT NEWLINE {
		printf("\n\tGood bye!!\n\n");
		exit(2);
	}
	;

iomodifier_opt:
	GREAT WORD {
		printf("   Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = $2;
	}
	| /* can be empty */ 
	|ANDPERCENT {
		printf("   Yacc: insert output \"%s\"\n","habiba backGrounf");
		Command::_currentCommand._background = 1;

	}

	|CONCATENATE WORD {
		printf("   Yacc: insert output \"%s\"\n","klkm backGrounf");
		Command::_currentCommand._outFile = $2;
		Command::_currentCommand._concat = 1;

	}	
	
	|ADDINPUT WORD {
		printf("   Yacc: insert input \"%s\"\n","klkm backGrounf");
		Command::_currentCommand._inputFile = $2;
	}

	|ADDINPUT WORD GREAT WORD {
		printf("   Yacc: insert input \"%s\"\n","klkm backGrounf");
		Command::_currentCommand._inputFile = $2;
		Command::_currentCommand._outFile = $4;
	}
	| ERRORAPPEND WORD {
		printf("   Yacc: insert output \"%s\"\n","klkm backGrounf");
		Command::_currentCommand._errFile = $2;
		Command::_currentCommand._concat = 1;
	}
	| ERRORREPLACE WORD {
		printf("   Yacc: insert output \"%s\"\n","klkm backGrounf");
		Command::_currentCommand._errFile = $2;
	}
	;
%%

void
yyerror(const char * s)
{
	fprintf(stderr,"%s", s);
}

#if 0
main()
{
	yyparse();
}
#endif

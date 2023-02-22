
/*
 * CS354: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 */
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include "command.h"
int ID;





SimpleCommand::SimpleCommand()
{
	// Creat available space for 5 arguments
	_numberOfAvailableArguments = 5;
	_numberOfArguments = 0;
	_arguments = (char **) malloc( _numberOfAvailableArguments * sizeof( char * ) );
}

void
SimpleCommand::insertArgument( char * argument )
{
	if ( _numberOfAvailableArguments == _numberOfArguments  + 1 ) {
		// Double the available space
		_numberOfAvailableArguments *= 2;
		_arguments = (char **) realloc( _arguments,
				  _numberOfAvailableArguments * sizeof( char * ) );
	}
	
	_arguments[ _numberOfArguments ] = argument;

	// Add NULL argument at the end
	_arguments[ _numberOfArguments + 1] = NULL;
	
	_numberOfArguments++;
}

Command::Command()
{
	// Create available space for one simple command
	_numberOfAvailableSimpleCommands = 1;
	_simpleCommands = (SimpleCommand **)
		malloc( _numberOfSimpleCommands * sizeof( SimpleCommand * ) );

	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_background = 0;
	_concat=0;
	_changeDir=0;
	_pipeEntered=0;
	_echoing =0;
}

void
Command::insertSimpleCommand( SimpleCommand * simpleCommand )
{
	if ( _numberOfAvailableSimpleCommands == _numberOfSimpleCommands ) {
		_numberOfAvailableSimpleCommands *= 2;
		_simpleCommands = (SimpleCommand **) realloc( _simpleCommands,
			 _numberOfAvailableSimpleCommands * sizeof( SimpleCommand * ) );
	}
	
	_simpleCommands[ _numberOfSimpleCommands ] = simpleCommand;
	_numberOfSimpleCommands++;
}

void
Command:: clear()
{
	for ( int i = 0; i < _numberOfSimpleCommands; i++ ) {
		for ( int j = 0; j < _simpleCommands[ i ]->_numberOfArguments; j ++ ) {
			free ( _simpleCommands[ i ]->_arguments[ j ] );
		}
		
		free ( _simpleCommands[ i ]->_arguments );
		free ( _simpleCommands[ i ] );
	}

	if ( _outFile ) {
		free( _outFile );
	}

	if ( _inputFile ) {
		free( _inputFile );
	}

	if ( _errFile ) {
		free( _errFile );
	}


	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_background = 0;
	_concat=0;
	_changeDir=0;
	_pipeEntered=0;
	_echoing=0;
}

void
Command::print()
{
	printf("\n\n");
	printf("              COMMAND TABLE                \n");
	printf("\n");
	printf("  #   Simple Commands\n");
	printf("  --- ----------------------------------------------------------\n");
	for ( int i = 0; i < _numberOfSimpleCommands; i++ ) {
		printf("  %-3d ", i );
		for ( int j = 0; j < _simpleCommands[i]->_numberOfArguments; j++ ) {
			printf("\"%s\" \t", _simpleCommands[i]->_arguments[ j ] );
		}
		printf("\n");
	}

	printf( "\n\n" );
	printf( "  Output       Input        Error        Background\n" );
	printf( "  ------------ ------------ ------------ ------------\n" );
	printf( "  %-12s %-12s %-12s %-12s\n", _outFile?_outFile:"default",
		_inputFile?_inputFile:"default", _errFile?_errFile:"default",
		_background?"YES":"NO");
	printf( "\n\n" );
	
}
void
errorHandling(char * str){
int fd = open("logfile", O_WRONLY); // 'fd' stands for 'file descriptor
close(2); // close stderr
dup(fd); // duplicate fd so that it's copy will be the lowest available value -- 2
}

void 
Command::readWrite(char*args[]){
	int defaultin  = dup( 0 );
	int defaultout = dup( 1 );
	int defaulterr = dup( 2 );
	int outfd,infd;

	if(_inputFile){
		infd=open(_inputFile,O_RDONLY);
		printf("%d",infd);
	if ( infd < 0 ) {
		printf( "No such file or directory" );
		return;
	}
		dup2(infd,0);
	}

	if(_outFile){
	if (_concat==1)
		 outfd=open(_outFile,O_APPEND|O_CREAT|O_WRONLY,0777);
	else{
		 outfd=creat(_outFile,0777);
	}
	dup2(outfd,1);
	}	
	
	int pid = fork();
	if ( pid == -1 ) {
		perror( "ls: fork\n");
		exit( 2 );
	}
	

	if (pid == 0) {
		
     	execvp(args[0], args);
		perror( "ls: exec ls");
		exit( 2 );
	}
	waitpid( pid, 0, 0 );

	dup2(defaultin,0);
	dup2(defaultout,1);
}
 
using namespace std;
void file_to_file(char* original, char* copy){
	string line;
	ifstream ini_file{
		original
	}; 
	ofstream out_file{ copy };
	if (ini_file && out_file) {
		getline(ini_file, line);
		out_file << line;
		while (getline(ini_file, line)) {
			out_file <<"\n"<< line;
		}
	}
	else {
		printf("Cannot read File");
	}
	ini_file.close();
	out_file.close();
}

void 
Command:: pipeCommands(){
	int defaultin  = dup( 0 );
	int defaultout = dup( 1 );
	int defaulterr = dup( 2 );
	char pipe_arg[50][50];
	for (int i = 0; i < _numberOfSimpleCommands; i++)
	{
		strcpy(pipe_arg[i] ,_simpleCommands[i]->_arguments[0]);
	}
	int temp_out;
	int temp_in;
	int infd,outfd;
	for (int i = 0; i < _numberOfSimpleCommands; i++)
	{
	 temp_out = open("tempp_out",O_TRUNC|O_RDWR|O_CREAT);
	 temp_in = open("tempp_in",O_RDWR|O_CREAT);
	 if(i==0){
		if(_inputFile){
		infd = open(_inputFile,O_RDONLY);
	if ( infd < 0 ) {
		printf( "No such file or directory" );
		return;
	}
		dup2(infd,0);
	}
		}
		else
		{
		dup2(temp_in,0);
		}
	if (i == _numberOfSimpleCommands -1)
	{
			if(_outFile)
		{
			if (_concat==1)
			 	{
					outfd=open(_outFile,O_APPEND|O_CREAT|O_WRONLY,0777);
			 	}
			else{
		 			outfd=creat(_outFile,0777);
			
				}	
		dup2(outfd,1);
		}
		else{
			dup2(defaultout,1);
			}
		dup2(temp_in,0);

	}else
	{		
		dup2(temp_out,1);
		dup2(temp_in,0);
	}
	int pid1 = fork();
	if ( pid1 == -1) {
		perror( "ls: fork\n");
		exit( 2 );
	}
	if (pid1 == 0) {	
		waitpid( pid1, 0, 0 );
        execvp(pipe_arg[i], _simpleCommands[i]->_arguments);
		errorHandling( "ls: exec ls");
		exit(2);
	}
	waitpid( pid1, 0, 0 );
	file_to_file("tempp_out","tempp_in");
	close(temp_out); 
	close(temp_in); 	
	}
	close(infd);
	close(outfd);
	dup2(defaultin,0);
	dup2(defaultout,1);
	dup2(defaulterr,2);
}

void
Command::executeAnyCommand(char*args[]){
	int pid = fork();
	   if ( pid == -1 ) {
		perror( "ls: fork\n");
		exit( 2 );
	}

	if (pid == 0) {	
        execvp(args[0], args) ;
		errorHandling( "ls: exec ls");
		exit( 2 );
	}
	ID = waitpid( pid, 0, 0 );

}

#include <bits/stdc++.h>
using namespace std;
#include <bits/stdc++.h>
using namespace std;


bool strmatch(char* str, char pattern[], int n, int m)
{

	if (m == 0)
		return (n == 0);


	bool lookup[n + 1][m + 1];

	memset(lookup, false, sizeof(lookup));

	lookup[0][0] = true;

	for (int j = 1; j <= m; j++)
		if (pattern[j - 1] == '*')
			lookup[0][j] = lookup[0][j - 1];

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (pattern[j - 1] == '*')
				lookup[i][j]
					= lookup[i][j - 1] || lookup[i - 1][j];
			else if (pattern[j - 1] == '?'
					|| str[i - 1] == pattern[j - 1])
				lookup[i][j] = lookup[i - 1][j - 1];

			else
				lookup[i][j] = false;
		}
	}

	return lookup[n][m];
}

void wildcarding(char *pattern,char*args){

	int defualtout= dup(1);
	int defualtin = dup(0);
	int n= strlen(pattern);
    int outfd=creat("outd",0777);
 	dup2(outfd,1);
	int pid1 = fork();
	if ( pid1 == -1) {
		perror( "ls: fork\n");
		exit( 2 );
	}
	if (pid1 == 0) {	
		execlp("ls", "ls", args , (char *) 0);
		errorHandling( "ls: exec ls");
		exit(2);
	}
	waitpid( pid1, 0, 0 );
	dup2(defualtout,1);
	close(outfd);
	fstream newfile;
   newfile.open("outd",ios::in); 
   if (newfile.is_open()){   
      string tp;
      while(getline(newfile, tp)){ 
	  if (strmatch(tp.data(), pattern, strlen(tp.data()),
				strlen(pattern)))
		{
         cout << tp << "\t"; 
		 }
	
   }
      newfile.close(); 
   }

}
void
Command::execute()
{

	
	// Don't do anything if there are no simple commands
	if ( _numberOfSimpleCommands == 0 ) {
		prompt();
		return;
	}
	print();
	int defaulterr;
	if(_errFile){
		int fd;
	defaulterr = dup( 2 ); 
	if(_concat)
		 fd=open(_errFile,O_APPEND|O_CREAT|O_WRONLY,0777);
	else
		 fd=creat(_errFile,0777);
	close(2); // close stderr
	dup(fd);
	}
	if (_echoing ==1)
	{
		strmatch(_simpleCommands[0]->_arguments[1].data() ,"/tmp/*" ,strlen(_simpleCommands[0]->arguments[1]), strlen("/tmp/*") );
	if (!strcmp(_simpleCommands[0]->_arguments[1] ,"/tmp/*") || !strcmp(_simpleCommands[0]->_arguments[1] ,"/dev/*"))
		 {
			printf("yes");
			char buff[6];
			memcpy(buff,&_simpleCommands[0]->_arguments[1][0],5);
			buff[5]='\0';
			wildcarding("*",buff);
		  } else
		wildcarding(_simpleCommands[0]->_arguments[1],nullptr);	
	}
else{
	if(_pipeEntered==1){

		pipeCommands();
	}
	
else{
		
	char * args[_simpleCommands[_numberOfSimpleCommands-1]->_numberOfArguments];
 
		for ( int j = 0; j < _simpleCommands[_numberOfSimpleCommands-1]->_numberOfArguments; j++ ) {
			args[j]=_simpleCommands[_numberOfSimpleCommands-1]->_arguments[ j ] ;
		}
		args[_simpleCommands[_numberOfSimpleCommands-1]->_numberOfArguments]=nullptr;
	if (_changeDir==1){
		int x = chdir(args[1]);
		if (x==-1){
			printf("No such directory redirecting to home");
			chdir("/home");
		}
	}
	else{
	if (_outFile != 0 || _inputFile!=0){
		readWrite(args);
	}
	else {
	executeAnyCommand(args);
	}
	}
}}
	if(_background==0)
	{
		clear();
		prompt();
	}
	else {
		_background=0;
	}
dup2(defaulterr,2);
close(defaulterr);
}


// Shell implementation

void
Command::prompt()
{
	printf("\nmyshell~");
	printf(" %s >>  ", get_current_dir_name());
	fflush(stdout);
}

Command Command::_currentCommand;
SimpleCommand * Command::_currentSimpleCommand;

int yyparse(void);

void intHandler(int dummy) {
		Command::_currentCommand.prompt();
}
FILE* fp;
void terminates(int x){
	char result[500];
	sprintf(result, "%d", ID);
	time_t rawtime;
	struct tm * timeinfo;
    time(&rawtime );
    timeinfo = localtime ( &rawtime );
    strcat(result, "  ");
    strcat(result, asctime (timeinfo));
    fprintf(fp,result);
}

int 
main()
{
	fp = fopen("log.log","w");
	signal(SIGINT, intHandler);
	signal(SIGCHLD, terminates);
	Command::_currentCommand.prompt();
	yyparse();
	fclose(fp);
	return 0;
}


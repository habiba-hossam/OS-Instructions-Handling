
#ifndef command_h
#define command_h

// Command Data Structure
struct SimpleCommand {
	// Available space for arguments currently preallocated
	int _numberOfAvailableArguments;

	// Number of arguments
	int _numberOfArguments;
	char ** _arguments;
	
	SimpleCommand();
	void insertArgument( char * argument );
};

struct Command {
	int _numberOfAvailableSimpleCommands;
	int _numberOfSimpleCommands;
	SimpleCommand ** _simpleCommands;
	char * _outFile;
	char * _inputFile;
	char * _errFile;
	int _background;
	int _concat;
	int _pipeEntered;
	int _changeDir;
	int _echoing;


	void prompt();
	void print();
	void execute();
	void clear();
	
	
	Command();
	void copying(char*[]);
	void viewing(char*[]);
	void executeAnyCommand(char*[]);
	void readWrite(char*[]);
	void insertSimpleCommand( SimpleCommand * simpleCommand );
	void pipeCommands();
	

	static Command _currentCommand;
	static SimpleCommand *_currentSimpleCommand;
};

#endif

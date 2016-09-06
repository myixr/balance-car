#include <modbus/modbus-rtu.h>
#include <modbus/modbus-version.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdio.h>

struct Cmd{
	int cmd;
	const char *name;
};

#define CMD_VERSION 1
struct Cmd cmds[] = {
	{ CMD_VERSION, "version" },
	{ 0, NULL },
};

static void printUsage( void );
static int doCommand( int cmd, int argc, char *argv[] );

typedef struct GlobalArgs{
	const char *serial_port;

	int reserved;
}global_args_t;

static struct option version_long_opts[] = {
	{ NULL, 0, NULL, 0 }
};

static struct option read_input_opts[] = {
	{ "serialport", required_argument, NULL, 's' },
	{ NULL, 0, NULL, 0 }
};

int main( int argc, char *argv[] ){
	(void)argc; (void)argv;

	/* Command Line Format: cmd + args */
	if( argc < 2 ){
		printUsage( );
		return -1;
	}

	const char *cmd_string = argv[1];
	int cmd = -1;
	const struct Cmd *pcmd = cmds;
	while( pcmd->name != NULL ){
		if( strcmp( cmd_string, pcmd->name ) == 0 ){
			cmd = pcmd->cmd;
			break;
		}
	}

	if( cmd == - 1 ){
		printUsage( );
		return -1;
	}

	return doCommand( cmd, argc-1, argv+1 );
}

void printUsage( void ){
	printf( "TODO\n" );
}

void doVersionCmd( void ){
	printf( "libmodbus version: %s\n", LIBMODBUS_VERSION_STRING );
}

int doCommand( int cmd, int argc, char *argv[] ){
	int opt;
	switch( cmd ){
	case CMD_VERSION:
		opt = getopt_long( argc, argv, NULL, version_long_opts, NULL );
		while( opt != -1 ){
			switch( opt ){
			default:
				printUsage();
				return -1;

			}
		}
		doVersionCmd();
	}
	return 0;
}


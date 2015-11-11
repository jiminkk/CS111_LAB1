// UCLA CS 111 Lab 1 main program

#include <errno.h>
#include <error.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "command.h"

static char const *program_name;
static char const *script_name;

static void usage (void)
{
	error (1, 0, "usage: %s [-pt] SCRIPT-FILE", program_name);
}

static int get_next_byte (void *stream)
{
	return getc (stream);
}

int execute_parallelism (command_stream_t stream);

int main (int argc, char **argv)
{
	int opt;
	int command_number = 1;
	int print_tree = 0;
	int time_travel = 0;

	int print_v = 0;
	int print_x = 0;

	program_name = argv[0];

	for (;;)
	switch (getopt (argc, argv, "ptvx"))
	{
		case 'v':
			print_v = 1;
			break;
		case 'x':
			print_x = 1;
			break;
		case 'p': print_tree = 1; break;
		case 't': 
			time_travel = 1;
			break;
		default: usage (); break;
		case -1: goto options_exhausted;
	}
	
	options_exhausted:;

	// There must be exactly one file argument.
	if (optind != argc - 1)
	usage ();

	script_name = argv[optind];
	const char * src_file = script_name;
	
	FILE *script_stream = fopen(script_name, "r");
	if (! script_stream)
		error (1, errno, "%s: cannot open", script_name);
	
	command_stream_t command_stream = make_command_stream(get_next_byte, script_stream);
	command_t last_command = NULL;
	command_t command;

	if (time_travel)
	{
		return execute_parallelism(command_stream);
	}
	else if(print_v)
	{
		char v_shellscript[255];
		strcpy(v_shellscript, "sh -v ");
		int input;
		printf("Type 1 to go through every command or 2 to normal -v (print out script)\n");
		scanf("%d", &input);

		int count=0;
		if(input == 1)
		{
			while((command = read_command_stream(command_stream)))
			{
				char enter;
				count++;
				printf("\n");
				print_vcommand(command);
				printf("\nKeep going? (y/n)\n");
				scanf("%s", &enter);
				if(enter=='n')
					break;
			}
		}
		else if(input == 2)
		{
			//printf("script name: %s\n", src_file);
			strcat(v_shellscript, src_file);
			printf("Now running: %s\n", v_shellscript);
			system(v_shellscript);		// runs shell script with debug option -v
		}
	}
	// -x option
	else if(print_x)
	{
		char x_shellscript[255];
		strcpy(x_shellscript, "sh -x ");
		int input;
		printf("Type 1 to only show outputs on command line or type 2 to execute the normal -x debugging option to see all commands.\n");
		scanf("%d", &input);
		if(input == 1)
		{
			printf("%s\n", "Showing you outputs...");
			while ((command = read_command_stream(command_stream)))
				execute_command(command, time_travel);
		}
		else if(input == 2)
		{
			//printf("script name: %s\n", src_file);
			strcat(x_shellscript, src_file);
			printf("Now running: %s\n", x_shellscript);
			system(x_shellscript);		// runs shell script with debug option -v
		}
	}
	//print tree option
	else
	{
		while ((command = read_command_stream(command_stream)))
		{
			if (print_tree)
			{
				printf("# %d\n", command_number++);
				print_command(command);
			}
			else
			{
				last_command = command;
				execute_command(command, time_travel);
			}
		}
	}
	return print_tree || !last_command ? 0 : command_status(last_command);
}



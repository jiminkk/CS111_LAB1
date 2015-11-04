// UCLA CS 111 Lab 1 main program

#include <errno.h>
#include <error.h>
#include <getopt.h>
#include <stdio.h>

#include "command.h"
#include "command-internals.h"

static char const *program_name;
static char const *script_name;

static void
usage (void)
{
  error (1, 0, "usage: %s [-pt] SCRIPT-FILE", program_name);
}

static int
get_next_byte (void *stream)
{
  return getc (stream);
}

int
main (int argc, char **argv)
{
  int opt;
  int command_number = 1;
  int print_tree = 0;
  int time_travel = 0;
  program_name = argv[0];

  for (;;)
    switch (getopt (argc, argv, "pt"))
      {
      case 'p': print_tree = 1; break;
      case 't': time_travel = 1; break;
      default: usage (); break;
      case -1: goto options_exhausted;
      }
 options_exhausted:;

  // There must be exactly one file argument.
  if (optind != argc - 1)
    usage ();

  script_name = argv[optind];
  FILE *script_stream = fopen (script_name, "r");
  if (! script_stream)
    error (1, errno, "%s: cannot open", script_name);
  command_stream_t command_stream =
    make_command_stream (get_next_byte, script_stream);

	
	
  command_t last_command = NULL;
  command_t command;
  int count = 0;
  command_t command1;
  command_t command2;
  
  while ((command = read_command_stream (command_stream)))
    {
		if(command->input!=NULL)
			printf("it has input or output\n");
		if(count==0)
			command1 = command;
		if(count==1)
			command2 = command;
		count++;
		//printf("Stuck in main!");
		if (time_travel)
		{
			printf("inside time_travel main\n");
			if(count>1){
				printf("count>1\n");
				linked_files * file1 = get_linked_files(command1);
				linked_files * file2 = get_linked_files(command2);
				int output = check_dependency(file1, file2);
				printf("output: %d\n", output);
			}
		}
      /*if (print_tree)
	{
	  printf ("# %d\n", command_number++);
	  print_command (command);
	}
      else
	{
	  last_command = command;
	  execute_command (command, time_travel);
	}*/
    }

  return print_tree || !last_command ? 0 : command_status (last_command);
}

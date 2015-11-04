// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"
#include "alloc.h"
#include <unistd.h>
#include <error.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

int
command_status (command_t c)
{
  return c->status;
}

void
execute_command (command_t c, int time_travel)
{
  int read = 0;
  int write = 1;
  int mypipe[2];
  pid_t child;
	
  switch (c->type)
  {
    
    case SIMPLE_COMMAND:
      //base case for all recursion
      child = fork();

      //child process
      if(child == 0)
      {
        //handle redirects here
        if(c->input)  // <
        {
          int input_file;

          //do we need more parameters here?
		  input_file = open(c->input, O_RDONLY|O_CREAT, S_IRUSR|S_IRGRP|S_IROTH);
          if(input_file == -1)
            error(3, 0, "Could not open input file");
          if(dup2(input_file, read) == -1)
            error(3, 0, "Read file descriptor for input redirect not found");

          close(input_file);
        }

        if(c->output) // >
        {
          int output_file;
			printf("output character: %d\n", *(c->output));
			output_file = open(c->output, O_WRONLY|O_CREAT, S_IWUSR|S_IWGRP|S_IWOTH);
          //do we need more parameters here?
          if(output_file == -1)
            error(3, 0, "Could not write to file");
          if(dup2(output_file, write) == -1)
            error(3, 0, "Write file descriptor for output redirect not found");

          close(output_file);
        }
        //check if correct
        if(execvp(c->u.word[0], c->u.word) == -1)
          error(3, 0, "Cannot execute command");
      }
      //parent process
      else if(child > 0)
      {
        int status;
        waitpid(child, &status, 0); //wait on child process
        c->status = status; //waitpid returns status of child process
      }
      else
        error(3, 0, "Failed to create child process");
      break;

    case SEQUENCE_COMMAND:
      //parallelized
      //should we have to check for dependencies for lab 1b?
      if(time_travel && !(check_dependency(get_linked_files(c->u.command[0]), get_linked_files(c->u.command[1]))))
      {
        child = fork();
        //newly created child process
        if(child == 0)
        {
          execute_command(c->u.command[0], time_travel);
          exit(0);
        }
        // parent process
        if(child > 0)
        {
          execute_command(c->u.command[1], time_travel);
          exit(0);
        }
        else
          error(3, 0, "Child process not created.");
        waitpid(child, NULL, 0); //wait for child to finish.
      }
      else
      {
        execute_command(c->u.command[0], time_travel);
        execute_command(c->u.command[1], time_travel);
      }
      c->status = c->u.command[1]->status;
      break;

    case AND_COMMAND:
      execute_command(c->u.command[0], time_travel);
      c->status = c->u.command[0]->status;
      
      //Given that the first command has succeeded exeucte the second one
      if (!c->status)
      {
        execute_command(c->u.command[1], time_travel);
        c->status = c->u.command[1]->status;
      }
      break;

    case OR_COMMAND:
      execute_command(c->u.command[0], time_travel);
      c->status = c->u.command[0]->status;
      
      //execute only if failure occurs on the first command
      if (c->status)
      {
        execute_command(c->u.command[1], time_travel);
        c->status = c->u.command[1]->status;
      }
      break;

    case PIPE_COMMAND:
      //pipe(mypipe);
    if (pipe(mypipe) == -1)
		error(3, 0, "Cannot create pipe.");
		child = fork();

      if (child == 0) //child process
      {
        close(mypipe[read]);

        ///NOTE There may be a logic error depending on order of closing and executing commands
        
        //Check for error with duplication
        if (dup2(mypipe[write], write)!=-1)
        {
          execute_command(c->u.command[0], time_travel);   //execute_switch(c->u.command[0]);
          c->status = c->u.command[0]->status;
          close(mypipe[write]);
          exit(0);
        }
        else
        {
          error(3, 0, "Failure to duplicate write file descriptor");
        }
      }

      else if (child > 0) //parent process
      {
    int child_status;
        waitpid(child, &child_status, 0);
        
        close(mypipe[write]);
        
        if (dup2(mypipe[read],read) !=-1)
        { 
          execute_command(c->u.command[1], time_travel);
          c->status = c->u.command[1]->status;
          close(mypipe[read]);
          //exit(0);
        }
        else
          error(3, 0, "failure to read from file line 171");
      }
      else
          error(3, 0, "Failed to create child process");
      break;
      
    case SUBSHELL_COMMAND:
      execute_command(c->u.subshell_command, time_travel);
      c->status = c->u.subshell_command->status;
      break;
    default:
      error(3,0,"What command are you using? its not valid");
    break;
  }
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
   
   //Assuming we need to run a switch statement to run each command 
   
   //Maybe put the psuedo code here for the operator it parses stuff?
  return ;
}

int execute_parallelism(command_stream_t command_stream)
{
	while(command_stream !=NULL)
	{
		command_stream_t queue = NULL;
		command_stream_t queue_curr = NULL;
		command_stream_t prev = NULL;
		command_stream_t curr = command_stream;
		int runnable = 0;
		while (curr != NULL)
		{
			if (queue == NULL)
			{
				queue = curr;
				queue_curr = curr;
				command_stream = curr->next;
				curr = curr->next;
				runnable = 1;
			}
			else 
			{
				int dependents = 0;
				command_stream_t temp_stream = queue;
				while (temp_stream != NULL)
				{
					if (check_dependency(curr->depends, temp_stream->depends))
					{
						dependents = 1;
						break;
					}
					temp_stream = temp_stream->next;
				}
				if (dependents == 0)
				{
					if (prev == NULL) 
					{
						queue_curr->next = curr;
						queue_curr = curr;
						command_stream = curr->next;
						curr = curr->next;
						
					}
					else
					{
						queue_curr->next = curr;
						queue_curr = curr;
						prev->next = curr->next;
						curr = curr->next;
					}
					runnable++;
					
				}
				else
				{
					prev = curr;
					curr = curr->next;
				}
				
			}
			queue_curr->next = NULL;
		}
		pid_t* chillin = checked_malloc(runnable * sizeof(pid_t));
		int i =0;
	
		if (queue != NULL)
		{
			command_t command;
			curr = queue;
			while (curr)
			{
				pid_t child = fork();
				if (child == 0)
				{
					execute_command(curr->comm, 1);
					exit(0);
				}
				
				else if (child > 0)
					chillin[i] = child;
				else
					error(3, 0, "cannot create child process.");
				i++;
				curr = curr->next;
			}
			int waiting;
			do 
			{
				waiting = 0;
				int j;
				for (j = 0; j < runnable; j++)
				{
					if (chillin[j] > 0)
					{
						if (waitpid(chillin[j], NULL, 0) != 0)
							chillin[j] = 0;
						else 
							waiting = 1;
					}
					sleep (0);
				}
			} while (waiting == 1);
		}
		free(chillin);
		curr = queue;
		prev = NULL;
		while (curr)
		{
			free_command(curr->comm); // need to implement
			free(curr->comm);
			
			linked_files * lfcurr = curr->depends;
			linked_files * lfprev = NULL;
			while (lfcurr)
			{
				lfprev = lfcurr;
				lfcurr = lfcurr->next;
				free(lfprev);
			}
			free(curr->depends);
			prev = curr;
			curr = curr->next;
			
			//Implement free later
		}
	}
	return 0;
}	
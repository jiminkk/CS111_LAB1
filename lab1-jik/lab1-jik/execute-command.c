// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"
#include <unistd.h>
#include <error.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

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
  switch (c->type)
  {
    int mypipe[2];
    pid_t child;
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
          if((input_file = open(c->input, O_RDONLY)) == -1)
            error(3, 0, "Could not open input file");
          if(dup2(input_file, read) == -1)
            error(3, 0, "Read file descriptor for input redirect not found");

          close(input_file);
        }

        if(c->output) // >
        {
          int output_file;

          //do we need more parameters here?
          if((output_file = open(c->output, O_WRONLY|O_CREAT|O_TRUNC)) == -1)
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
      if(time_travel)
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
      pipe(mypipe);
      child = fork();

      if (child == 0) //child process
      {
        close(mypipe[read]);

        ///NOTE There may be a logic error depending on order of closing and executing commands
        
        //Check for error with duplication
        if (dup2(mypipe[write], write))
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
        
        if (dup2(mypipe[read],read))
        { 
          execute_command(c->u.command[1], time_travel);
          c->status = c->u.command[1]->status;
          close(mypipe[read]);
          exit(0);
        }
        else
          error(3, 0, "ailure to duplicate read file descriptor");
      }
      else
          error(3, 0, "Failed to create child process");
      break;
      
    case SUBSHELL_COMMAND:
      execute_command(c->u.command[0], time_travel);
      c->status = c->u.command[1]->status;
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
  error (1, 0, "command execution not yet implemented");
  return ;
}

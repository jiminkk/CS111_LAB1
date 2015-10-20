// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"
#include <unistd.h>
#include <error.h>

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
    int fd[2];
    pid_t child;
    case SIMPLE_COMMAND:
      //TO DO
    case SEQUENCE_COMMAND:
      //TO DO

      execute_command(c->command[0], time_travel);
      execute_command(c->command[1], time_travel);
      c->status = c->command[1]->status;
      break;
    case AND_COMMAND:
      execute_command(c->command[0], time_travel);
      c->status = c->command[0]->status;
      
      //Given that the first command has succeeded exeucte the second one
      if (!c->status)
      {
        execute_command(c->command[1], time_travel);
        c->status = c->command[1]->status;
      }
      break;
    case OR_COMMAND:
      execute_command(c->command[0], time_travel);
      c->status = c->command[0]->status;
      
      //execute only if failure occurs on the first command
      if (c->status)
      {
        execute_command(c->command[1], time_travel);
        c->status = c->command[1]->status;
      }
      break;
    case PIPE_COMMAND:
      int child_status;
      pid_t first_pid, second_pid, return_pid;
      int mypipe[2];
      pipe(mypipe); //assuming its function that must be defined later 
      first_pid = fork();
      if (first_pid == 0) //child process
      {
        close(mypipe[0];
        
        
        ///NOTE There may be a logic error depending on order of closing and executing commands
        
        
        //Chek for error with pipe
        if (dup2(mypipe[write], write);
        {
          close(mypipe[write]);
        } 
        else
        {
          error(3, 0, "Failure to close mypipe[write]");
        }
        execute_command(c->u.command[read])   //execute_switch(c->u.command[0]);
      }
      else if (first_pid > 0)
      {
        
        waitpid(first_pid, &child_status, 0);
        
        close(mypipe[write]);
        
        if (dup2(mypipe[read],read)
        { 
          close(mypipe[read],read);
        }
        else
          error(3, 0, "Failure to close mypipe[read]");
          
        execute_command(c->command[1], time_travel);
        c->status = c->command[1]->status;
        close(mypipe[read]);
      }
      else
          error(3, 0, "Failed to create child process");
      break;
      
    case SUBSHELL_COMMAND:
      execute_command(c->ucommand[0], time_travel);
      c->status = c->u.command[1]->status;
      break;
    default:
      error(3,0,"What command are you using? its not valid");
  }
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
   
   //Assuming we need to run a switch statement to run each command 
   
   //Maybe put the psuedo code here for the operator it parses stuff?
  error (1, 0, "command execution not yet implemented");
}

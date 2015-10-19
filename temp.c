#include "alloc.h"
#include <stdlib.h>
#include <stdbool.h> //allows support of bool type
#include <ctype.h> //used for isalphanum()
#include <string.h> // required for strchr() 
#include <stdio.h> // required for EOF


typedef struct command * command_t;
typedef struct stack stack;
typedef struct stack * stack_t;
typedef struct node node;
typedef struct node * node_t;

enum command_type
  {
    AND_COMMAND,         // A && B
    SEQUENCE_COMMAND,    // A ; B
    OR_COMMAND,          // A || B
    PIPE_COMMAND,        // A | B
    SIMPLE_COMMAND,      // a simple command
    SUBSHELL_COMMAND,    // ( A )
  };

// Data associated with a command.
struct command
{
  enum command_type type;

  // Exit status, or -1 if not known (e.g., because it has not exited yet).
  int status;

  // I/O redirections, or 0 if none.
  char *input;
  char *output;

  union
  {
    // for AND_COMMAND, SEQUENCE_COMMAND, OR_COMMAND, PIPE_COMMAND:
    struct command *command[2];

    // for SIMPLE_COMMAND:
    char **word;

    // for SUBSHELL_COMMAND:
    struct command *subshell_command;
  } u;
};


struct node
{
	command_t val;
	node_t next;
};

struct stack{
	node_t top;
	int num_cmds;
};


stack_t make_stack()
{
  printf("Making a stack\n");

  stack_t tmp = (stack_t)checked_malloc(sizeof(stack));
  tmp->top = NULL;
  tmp->num_cmds = 0;

  return tmp;
}

//PUSH
void push(stack_t p, command_t add)
{
  p->num_cmds++;

  //if no head node
  if(p->top==NULL)
  {
    node_t head_node = (node_t)checked_malloc(sizeof(node));
    p->top = head_node;
    head_node->val = add;
    head_node->next = NULL;
    return;
  }
  //else
  node_t next_node = (node_t)checked_malloc(sizeof(node));
  next_node->next = p->top;
  next_node->val = add;
  p->top = next_node;
  return;
}

//POP the command_t value of top node.
command_t pop(stack_t p)
{
  if (p->top != NULL)
  { 
    p->num_cmds--;
    command_t p_comm = p->top->val;

    //temp top node of stack will be freed.
    node_t top_node = p->top;
    p->top->next = p->top;
    free(top_node);

    return p_comm;
  }
  return NULL;
}

int main()
{

	stack_t stack1 = make_stack();
	stack_t stack2 = make_stack();
	command_t comm = (command_t)checked_malloc(struct command);

	stack1.push(stack2, comm);
	printf(stack1 );
	stack1.pop();

	return 0;
}








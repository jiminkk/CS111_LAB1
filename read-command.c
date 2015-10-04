// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"

#include <error.h>
#include <stdlib.h>
#include <stdbool.h> //allows support of bool type
#include <ctype.h> //used for isalphanum()
#include <string.h> // required for strchr() 
#include <stdio.h> // required for EOF

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */

enum token_trait
{
  WORD,
  SEMICOLON,
  AND,
  OR,
  SUBSHELL,
  HEAD,
  PIPELINE,
  LEFT,
  RIGHT,
  START
};

typedef struct token token_t;
typedef struct linked_list * linked_list_t;
typedef struct linked_tokens linked_tokens_t;
//typedef struct linked_tokens linked_tokens;
typedef struct stack * stack_t;
typedef struct stack stack;
typedef struct node node;
typedef struct node * node_t;

linked_tokens_t* make_linked_tokens(char* p_input, int input_size);

struct node {
	command_t val;
	node_t next;
};

struct stack{
	node_t top;
	int num_cmds;
};

stack_t make_stack(){
	stack_t tmp = checked_malloc(sizeof(stack));
	tmp->top = NULL;
	tmp->num_cmds = 0;
	return tmp;
}

struct linked_list{
	node_t head;
};

//This is specifically made for a linked list of tokens 
struct token
{
  char *value;
  enum token_trait trait;
  token_t* next;
};

token_t * make_token(enum token_trait type, char *value)
{ 
  token_t * product = checked_malloc(sizeof(token_t));
  product->trait = type;
  product->value = value;
  product->next = NULL;
  return product;
}
//This struct will hold tokens that will all be used to parse one command tree through 

struct linked_tokens
{
  token_t* head;
  linked_tokens_t* next;
};

//Constructor
linked_tokens_t* makeLinkedTokens(){
	linked_tokens_t* temp = checked_malloc(sizeof(linked_tokens_t));
	temp->head = NULL;
	temp->next = NULL;
	return temp;
}

//Constructor should be made to create linked_tokens 
//Destructor as well
// 
struct commeand_stream{
  //A collection of all the command objects 
  //linked_list* commands; //to be implemented later
  //Probably want to have a linked_tokens struct instead 
  command_stream_t* next; //pointer to next command stream
  command_t comm; // value read this value from read command 
};



 //STACK FUNCTIONALITY IMPLEMENTED HERE
void push(void *p, command_t add)
{ 
  stack_t item = (stack_t) p;
  
  item->num_cmds++;
  
  node_t next_node = (node_t)checked_malloc(sizeof(node_t));
  
  next_node->val = add;
  next_node->next=NULL;
  if(item->top==NULL){
	item->top=next_node;
	return;
  }
  next_node->next = item->top;
  item->top = next_node;
} 

command_t pop(void *p)
{
  stack * item = (stack*) p;
  if (item->top != NULL)
  { 
    item->num_cmds--;
    command_t info = item->top->val;
    item->top->next = item->top;
    return info;
  }
  return NULL;
  
}

command_t peek(void *p)
 {
  stack * item = (stack*)  (p); //sketch syntax may be incorrect 
  return item->top->val;
 }
 
 bool higherPrecedence(void * p, command_t * current)
{
	//TO DO IMPLEMENT THIS LATER TO CORRECTLY DETERMINE PRECEDENCE
}

bool isEmpty(void *p)
{
	stack * item = (stack*) p;
	return item->num_cmds==0;
}

bool isValidWord(char c)
{ 
  if(isalnum(c) || strchr("!&+,-./:@^_", c) != NULL)
    return true;
  return false; 
}


 
command_stream_t
make_command_stream (int (*get_next_byte) (void *),
         void *get_next_byte_argument)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
   
   //Place to process the command stream or just intializes the linked lists
   
   //FIRST GET INPUT FROM CONSOLE 
   //Taken from SROT13 CS 35L
  char next_arg;
  int count_char = 0;
  
  //Intializes the array of chars for all of the command input by console
  int maxbuffspace = 1024;
  char *input = (char*)checked_malloc(sizeof(char) * maxbuffspace);
  int pos = 0; //Keep track of position of the input array.
  do 
  {
    //Get the next byte 
    next_arg = (*get_next_byte)(get_next_byte_argument);
    
    //Get the whole string of command 
    //Evaluating 
    //HOW TO PARSE THE COMMAND 
    //The algorithm is for the -p option only applicable once you have parsed and have your commands
     
    //Execute the command is for LATER 
    //Note down the operation redirectors 
    //Look up tokens and make up specific types of string 
    //
    
    //TO IGNORE THE USELESS COMMENTS ITERATE UNTIL END OF COMMENT MARKER
    if (next_arg =='#')
    {
      do
      {
        next_arg = get_next_byte(get_next_byte_argument);
      }
      while(next_arg != '\n' || next_arg != EOF || next_arg != -1);
    }
    
    if(next_arg !=-1 && next_arg != EOF)
    {
      count_char++;
      input[pos++] = next_arg;
      if (pos == maxbuffspace)
      {
        input = (char*)checked_realloc(input, maxbuffspace * 2);
        maxbuffspace *= 2;
      //printf("sucessful realloc\n");
      }
      
    }
    
    
  }
  while (next_arg != -1); 
  
  linked_tokens_t * start = make_linked_tokens(&input, count_char);
  //At this point entire input from console has been taken 
  //What we should be doing at this point is to tokenize the input stream and prepare to parse them
  //Call on split trees 
  //Split trees should utilize tokens to be efficient and easier to implement 
  //After splitting the trees linking must occur within to create the command stream linked list
  //return and it is complete! 
  
  
  
  
  error (1, 0, "command reading not yet implemented");
  return 0;
}

//It appears 

linked_tokens_t* make_linked_tokens(char* p_input, int input_size)
{
  token_t * start_token = make_token(HEAD, NULL);
  token_t * current_token = start_token;
  
  linked_tokens_t * start_linked_tokens = makeLinkedTokens();
  linked_tokens_t * current_linked_tokens = start_linked_tokens;
  current_linked_tokens->head = current_token;
  
  // a|bd
  int index =0;
  char ch = *p_input;
  int parenCount = 0; //Used to count for nested parenthesis and detected incorrect numbers of parenthesis 
  // int token_line = 1;   //For debugging purposes. token_line = line in input

  while (index < input_size)
  {
    ch = *p_input;

    //In the case of a parenthesis AKA SUBSHELL
    if(ch=='(')
    {
      //(ab) ->subshell token
      //a|b -> a is left | is pipeline b is right
      //
      parenCount++;   //should be parenCount == 1 here.
      
      //let's allocate memory for the subshell (more than one char)
      size_t count_subshell=0;
      size_t size_subshell = 128;
      char* subshell = (char*)checked_malloc(size_subshell);

      //until we reach the end of the nested parentheses.
      while(parenCount > 0)
      {
        index++;
        p_input++;

        // subshell_line = token_line;

        //if the index is outside the bounds.
        if(index == input_size)
        {
          error(2, 0, "you need to close your parenthesis");
          return NULL;
        }

        //within subshell, more than 2 newlines is OK.
        //if either newline or tab or empty space, consume all whitespace
        if(*p_input == '\n')
        {
          while(p_input[1]==' ' || p_input[1]=='\n' || p_input[1]=='\t') //WHITESPACE && TAB CASE
          {
            p_input++;
            index++;
          }

          //newlines are special -- it can be substituted for semicolon.
          *p_input = ';';
        }
        else if(*p_input == '(')
          parenCount++;
        else if(*p_input == ')')
        {
          parenCount--;
          //if this closes all the subshells
          if(parenCount==0)
          {
            p_input++;
            index++;
            break;      //break out if subshell is closed.
          }
        }

        //push current char into subshell buffer if subshell is not closed
        subshell[count_subshell] = *p_input;
        count_subshell++;

        // In case we need to expand the subshell buffer
        if(count_subshell == size_subshell)
        {
          size_subshell += size_subshell;
          subshell = checked_grow_alloc(subshell, &size_subshell);
        }
      }
      //CHECK FOR NESTED SUBSHELL COMMANDS

      //create subshell token here
      current_token->next = make_token(SUBSHELL, subshell);
      current_token = current_token->next;
    }

    //In the case of closed parenthesis 
    else if(ch==')')
    {
      error(2, 0, "closed parenthesis found without leading parenthesis");
      return NULL;
    }

    //Beginning of a case of & and &&
    else if(ch=='&') 
    {
      p_input++;
      index++;
      if (*p_input == '&')
      {
        token_t * tok = make_token(AND, NULL);
        current_token->next = tok;
        current_token = current_token->next;
      }
      else
      {
        error(2,0, "Single & is illegal and not recognized");
        return NULL;
      }
    }

    //PIPE CASE
    else if(ch=='|') 
    { //a|b
      //CHECK FOR || CASE
      p_input++;
      index++;
      //OR
      if (*p_input == '|')
      {
        token_t * tok = make_token(OR, NULL);
        current_token->next = tok;
        current_token = current_token->next;
      }
      //PIPELINE
      else
      {
        token_t * tok = make_token(PIPELINE, NULL);
        current_token->next = tok;
        current_token = current_token->next;
      }
    }

    //WHITESPACE && TAB CASE
    else if(ch==' ' || ch=='\t') 
    {
      index++; //a_b
      p_input++;
    }

    //NEWLINE CASE
    else if(ch=='\n')
    {
      //IF WORD OR SUBSHELL BEFORE NEWLINE, it's a separate command.
      if(current_token->trait == WORD || current_token->trait == SUBSHELL)
      {
        if(current_token != HEAD)
        {
          current_linked_tokens->next = checked_malloc(sizeof(linked_tokens_t));
          current_linked_tokens = current_linked_tokens->next;
          current_linked_tokens->head = make_token(HEAD, NULL);
          current_token = current_linked_tokens->head;
        }
      }
      else if(current_token->trait == LEFT || current_token->trait == RIGHT)
      {
        error(2, 0, "You cannot have left or right redirects.");
        return NULL;
      }

      p_input++;
      index++;
    }

    //SEMICOLON CASE
    else if(ch==';')
    {
      token_t * tok = make_token(SEMICOLON, NULL);
      current_token->next = tok;
      current_token = current_token->next;
      //Have to consider incrementing JASMINE
    }

    //RIGHT DIRECT CASE
    else if(ch=='>')  
    {
      token_t * tok = make_token(RIGHT, NULL);
      current_token->next = tok;
      current_token = current_token->next;
    }

    //LEFT DIRECT
    else if(ch=='<')  
    {
      index++;
      p_input++;
      token_t * tok = make_token(LEFT, NULL);
      current_token->next = tok;
      current_token = current_token->next;
    }

    //VALID WORD CASE
    else if(isValidWord(ch))
    {
      //OVERALL IDEA
      //**ATTEMPT TO FIND THE ENTIRE WORD AND THEN PASS THAT AS A TOKEN
      int word_space = 8; //size of a char
      int word_index =0;
      char * word = checked_malloc(sizeof(word_space)); //stores entire word
      do
      {
        ch = *p_input;
        if (!isValidWord(ch))
          {
            error(2, 0, "Error unrecognized character detected");
            return NULL;
          }
        if (word_index == word_space)
        {
          word_space *=8;
          word = checked_realloc(word, word_space);
		  word[word_index] = ch;
		  p_input++;
          index++;
		}
	   }
      while(index < input_size && isValidWord(ch) && ch != EOF);
      
      token_t * tok = make_token(WORD, word);
      current_token->next = tok;
      current_token = current_token->next;
    }

    //FOR ANY OTHER CHARACTER THAT IS UNRECOGNIZED
    else
    {
      error(2, 0, "Unrecognized characters.");
      return NULL;
    }
  }
  return start_linked_tokens;
}
command_stream_t splitTrees(char * input)
{
  //The passed in buffer arg holds the entire string input from the console 
  //Iterate through the buffer until a character indicates the start of a large string 
    //Have a start index at teh beginning of the buffer and iterate until double '\n' or EOF 
    //Open parenthesis must mean you keep going until you see a closesd parenthesis pair (keep check
    //for extra parenthesis pairs that may exist in the middle 
    //Operator detection (; && || |), it will search for the operand 
  //Once you reach the end of the command tree hand the starting location to another function which 
  //creates the tree 
  //Add the command tree to the stream 
  
  
}

bool make_new_branch(stack * ops, stack * operands)
{
	if(operands->num_cmds <2)
		return false;
		
	//Use one operator and two oeprands 
	command_t operator = pop(ops);
	command_t second_command = pop(operands);
	command_t first_command = pop(operands);

	
	//Make the new command with its left and right parts
	command_t new_command = checked_malloc(sizeof(struct command));
	new_command->u.command[0] = first_command;
	new_command->u.command[1] = second_command;
	new_command->type = operator->type;
	
	//Add this new command tothe operand stack
	push(operands, new_command);
	return true;
}

//Organizes it into proper command_trees
command_t make_command(token_t * head)
{
	stack_t * operands = make_stack();
	stack_t * ops = make_stack();
	token_t * current_tok = head;
	command_t curr_cmd;
	command_t prev_cmd = NULL;
	do 
	{
		//Check for redirect operators if there are not redirectors then make a new command  
		if(current_tok->trait != LEFT && current_tok->trait != RIGHT)
		{
			curr_cmd = checked_malloc(sizeof(struct command)); //is this syntax ok?
		}
		
		switch (current_tok->trait)
		{
			case SUBSHELL:
				//Make another tree with the subshell command by calling on make_command...
				curr_cmd->u.subshell_command = make_command(make_linked_tokens(current_tok->value, strlen(current_tok->value))->head);
				
				//Add this operand to the operand stack
				push(operands, curr_cmd);
				break;
			case AND:
				//Pretty sure you just push the thing onto the stack 
				curr_cmd->type = AND_COMMAND;
				if (isEmpty(ops))
					push(ops, curr_cmd);
				else
				{
					if(higherPrecedence(ops, curr_cmd))
					{
						push(ops, curr_cmd);
						break;
					}
					else 
					{
						//HOW to detect parenthesis?
						/*while (peek(ops)-> && !higherPrecedence(ops, curr_cmd))
						{
							
						}*/
						
					}
				}
				push(ops, curr_cmd);
				break;
			case PIPELINE:
				//Follows same behavior of the &&
				curr_cmd->type = PIPE_COMMAND;
				push(ops, curr_cmd);
				break;
			case OR:
				//Follows same behavior as |
				curr_cmd->type = OR_COMMAND;
				push(ops,curr_cmd);
				break;
			case SEMICOLON:
			case WORD:
			case LEFT:
			case RIGHT:
			default:
				break;
		}
		current_tok = current_tok->next;
	}
	while(current_tok != NULL && current_tok->next != NULL);
	
	return curr_cmd; //placeholder
	
	
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  
  //Most likely to have the processing of the command tree?
  error (1, 0, "command reading not yet implemented");
  return 0;
}

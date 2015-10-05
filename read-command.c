// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h"
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
command_t make_command(token_t * head);

struct node {
	command_t val;
	node_t next;
};

struct stack{
	node_t top;
	int num_cmds;
};

struct linked_tokens
{
  token_t* head;
  linked_tokens_t* next;
};

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



stack_t make_stack(){
	printf("Making a stack");
	stack_t tmp = checked_malloc(sizeof(struct stack));
	tmp->top = NULL;
	tmp->num_cmds = 0;
	return tmp;
}

//dunno about this one
void free_stack(stack_t this_stack){
	free(this_stack);
	return;
}

token_t * make_token(enum token_trait type, char *value)
{ 
  token_t * product = checked_malloc(sizeof(token_t));
  product->trait = type;
  product->value = value;
  product->next = NULL;
  return product;
}
//This struct will hold tokens that will all be used to parse one command tree through 

void free_linked_tokens(linked_tokens_t* linkedTokens)
{
	linked_tokens_t* current = linkedTokens;
	linked_tokens_t* previous;
	
	while(current != NULL)
	{
		token_t* curr_token = current->head;
		token_t* prev_token;
		
		while(curr_token != NULL)
		{
			prev_token = curr_token;
			curr_token = prev_token->next;
			free(prev_token);
		}
		
		previous = current;
		current = current->next;
		free(current);
	}
	return;
}



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
struct command_stream{
  //A collection of all the command objects 
  //linked_list* commands; //to be implemented later
  //Probably want to have a linked_tokens struct instead 
  command_stream_t next; //pointer to next command stream
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

int stack_size(stack* p)
{
	return p->num_cmds;
}

command_t peek(void *p)
 {
  stack * item = (stack*)  (p); //sketch syntax may be incorrect 
  return item->top->val;
 }
 
 //Current Operator must be higher than top of stack
 bool higherPrecedence(void * p, command_t current)
{
	stack_t ops =(stack_t) p;
	
	switch(current->type)
	{
		case PIPE_COMMAND:
			return true;
		case AND_COMMAND:
		case OR_COMMAND: 
			return (peek(ops)->type == AND_COMMAND || peek(ops)->type == OR_COMMAND || peek(ops)->type == SEQUENCE_COMMAND);
		default:
			return false;
	}
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
  command_stream_t head = NULL;
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
		printf("Line 226");
      }
      while(next_arg != '\n' && next_arg != EOF && next_arg != -1);
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
    
    //printf("Line 244");
  }
  while (next_arg != -1); 
  
  int i;
  /*for(i=0; i<pos; i++)
  {
	printf(input[i]);
  }*/
  linked_tokens_t * start = make_linked_tokens(input, count_char);
  
  
  //MAKING ALL THE TRESS AND LINKING ALL THE TREES AT THIS POINT
  
	linked_tokens_t * token_stream = start;
	command_stream_t stream_head = NULL;
	command_stream_t curr_stream = NULL;
	command_stream_t prev_stream = NULL;
	
	while(stream_head != NULL && stream_head->next != NULL)
	{
		token_t* current_tok = token_stream->head->next;
		command_t current_cmd = make_command(current_tok);
		
		curr_stream = checked_malloc(sizeof(struct command_stream));
		curr_stream->comm = current_cmd;
		
		if (!stream_head)
		{
			stream_head = curr_stream;
			prev_stream = stream_head;
		}
		else
		{
			prev_stream->next = curr_stream;
			prev_stream = curr_stream;
		}
		
		curr_stream = curr_stream->next;
		
	}

  //At this point entire input from console has been taken 
  //What we should be doing at this point is to tokenize the input stream and prepare to parse them
  //Call on split trees 
  //Split trees should utilize tokens to be efficient and easier to implement 
  //After splitting the trees linking must occur within to create the command stream linked list
  //return and it is complete! 
  
   
  //Need to make actual command stream
  //head = make_command(start);
  
  
  
  //error (1, 0, "command reading not yet implemented");
  return stream_head;
}

/*command_stream_t make_command_forest(linked_tokens_t * token_stream)
{
	linked_tokens_t * = token_stream;
	command_stream_t stream_head = NULL;
	command_stream_t curr_stream = NULL;
	command_stream_t prev_stream = NULL;
	
	while(stream_head != NULL && stream_head->next != NULL)
	{
		token_t* current_tok = token_stream->head->next;
		command_t current_cmd = make_command(current_tok);
		
		current_tree = check_malloc(sizeof(struct command_stream));
		current_tree->
		prev_stream = curr_stream;
		curr_stream->next = curr_stream;
		
		
	}
	return tree_head;
}*/

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
	printf("line 317");
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
	  printf("Line 96");
      //until we reach the end of the nested parentheses.
      while(parenCount > 0)
      {
        index++;
        p_input++;

        // subshell_line = token_line;
		printf("Line 338");
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
			printf("line 317");
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
        if(current_token->trait != HEAD)
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
		printf("Line 486");
		printf("character is:%c", ch);
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
/*command_stream_t splitTrees(char * input)
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
  
  
}*/
//(a|(a|(a|b)))
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
//(a|(b|(c|d)))
//Organizes it into proper command_trees
command_t make_command(token_t * head)
{
	stack_t operands = make_stack();
	stack_t ops = make_stack();
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
				if(!isEmpty(ops) && !higherPrecedence(ops, curr_cmd))
				{
					bool branch_success = make_new_branch(ops, operands);
					if(!branch_success)
						{
							error(2,0, "Error making child branch");
							return NULL;
						}
				}
				push(ops, curr_cmd);
				break;
			case PIPELINE:
				//Follows same behavior of the &&
				curr_cmd->type = PIPE_COMMAND;
				if(!isEmpty(ops) && !higherPrecedence(ops, curr_cmd))
				{
					bool branch_success = make_new_branch(ops, operands);
					if(!branch_success)
						{
							error(2,0, "Error making child branch");
							return NULL;
						}
				}
				push(ops, curr_cmd);
				break;
			case OR:
				//Follows same behavior as |
				curr_cmd->type = OR_COMMAND;
				if(!isEmpty(ops) && !higherPrecedence(ops, curr_cmd))
				{
					bool branch_success = make_new_branch(ops, operands);
					if(!branch_success)
						{
							error(2,0, "Error making child branch");
							return NULL;
						}
				}
				push(ops,curr_cmd);
				break;
			case SEMICOLON:
				curr_cmd->type = SEQUENCE_COMMAND;
				if(!isEmpty(ops) && !higherPrecedence(ops, curr_cmd))
				{
					bool branch_success = make_new_branch(ops, operands);
					if(!branch_success)
						{
							error(2,0, "Error making child branch");
							return NULL;
						}
				}
				push(ops, curr_cmd);
				break;
			case WORD:
				curr_cmd->type = SIMPLE_COMMAND;
				
				size_t num_ofWords = 1;
				token_t * it = current_tok;
				//curr_cmd->u.word[0] = &(current_tok->value); 
				//loop through a possible string of multiple words
				while (it->next != NULL && it->next->trait == WORD)
				{
					num_ofWords++;
					it = it->next;
					printf("Line 632");
				}
				
				//ALLOCATE ENOUGH SPACE FOR ALL THE WORDS 
				curr_cmd->u.word = checked_malloc(num_ofWords * sizeof(char*));
				
				//Iterate through tokens again and assign words to curr_cmd->u.word
				it = current_tok;
				int u_index = 0;
				while (it->next != NULL && it->next->trait == WORD)
				{
					curr_cmd->u.word[u_index] = it->value;
					u_index++;
					it = it->next;
					printf("line 681");
				}
				push(ops, curr_cmd);
				break;
			case LEFT:
				if ((prev_cmd==NULL) || (prev_cmd->type != SIMPLE_COMMAND && prev_cmd->type != SUBSHELL_COMMAND))
					{
						error(2, 0 ,"Syntax error no input to redirect or non simple/subshell command was there");
						return NULL;
					}
				if (current_tok->trait != WORD)
				{
					error(2, 0, "Redirect MUST be followed by a WORD");
					return NULL;
				}
				
				//CHECK IF PREV_CMD HAS NO OUTPUT OR INPUT or can you?
				if (prev_cmd->input != NULL)
				{
					error(2, 0, "Previous command cannot have an input.");
					return NULL;
				}
				if (prev_cmd->output != NULL)
				{
					error(2, 0, "Previous command cannot have an output.");
					return NULL;
				}
				prev_cmd->input = current_tok->value;				
				break;
			case RIGHT:
				if ((prev_cmd==NULL) || (prev_cmd->type != SIMPLE_COMMAND && prev_cmd->type != SUBSHELL_COMMAND))
					{
						error(2, 0 ,"Syntax error no input to redirect");
						return NULL;
					}
				if (current_tok->trait !=WORD)
				{
					error(2, 0, "Redirect MUST be followed by a WORD");
					return NULL;
				}
				//CHECK IF PREV_CMD HAS NO OUTPUT or CAN YOU HAVE REDIRECTS UPON REDIRECTS
				if (prev_cmd->output != NULL)
				{
					error(2, 0, "Previous command already has some output.");
					return NULL;
				}
				prev_cmd->output = current_tok->value;
				break;
			default:
				break;
		}
		prev_cmd = curr_cmd;
		current_tok = current_tok->next;
		printf("Line 698");
	}
	while(current_tok != NULL && current_tok->next != NULL);
	
	//WE NEED TO EMPTY THE THE STACK AND COMPLETE THE TREE WITH WHATS LEFT 
	bool branch_success = true;
	command_t root;
	while(stack_size(ops)>0)
	{
		printf("Line 707");
		branch_success = make_new_branch(ops, operands);
		if (!branch_success)
		{
			error(2,0,"Error making child branch");
			return NULL;
		}
		
	}
	
	if(stack_size(operands) != 1)
	{
		error(2, 0, "Unable to make a complete command tree root is missing");
		return NULL;
	}
	
	root = pop(operands);
	
	//Do we need deconstructors? for the two stacks 
	return root; //placeholder
	
	
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  
  //Most likely to have the processing of the command tree?
  error (1, 0, "command reading not yet implemented");
  return 0;
}

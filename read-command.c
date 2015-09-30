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
	SIMPLE_COMMAND,
	PIPELINE,
	LEFT,
	RIGHT,
	START
	
}

typedef struct token token_t;

//This is specifically made for a linked list of tokens 
struct token
{
	char *value;
	enum token_trait trait;
	*token next;
}   

token_t * make_token(enum token_trait type, char *value)
{	
	token * product = checked_malloc(sizeof(token));
	product->trait = type;
	product->value = *value;
	product->next = NULL:
	return product;
}
//This struct will hold tokens that will all be used to parse one command tree through 
typedef struct linked_tokens linked_tokens_t
{
	*token head;
	*token next;
}


//Constructor should be made to create linked_tokens 
//Destructor as well
// 
struct commeand_stream{
	//A collection of all the command objects 
	linked_list* commands; //to be implemented later
	//Probably want to have a linked_tokens struct instead 
	command_stream next; //pointer to next command stream
	command_t comm; // value read this value from read command 
	
};

 //STACK FUNCTIONALITY IMPLEMENTED HERE
void push(void *p, command_t add)
{	
	stack * item = (stack*) p;
	item->num_cmds++;
	
	node* next_node = (node*)checked_malloc(sizeof(node));
	next_node->val = add;
	
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
	int count_char 0;
	
	//Intializes the array of chars for all of the command input by console
	int maxbuffspace = 1024;
	char *input = (char*)checked_malloc(sizeof(char) * maxbuffspace);
	int pos = 0; //Keep track of position of the input array.
	do 
	{
		//Get the next byte 
		next_arg = *(get_next_byte)(get_next_byte_argument);
		
		//Get the whole string of command 
		//Evaluating 
		//HOW TO PARSE THE COMMAND 
		//The algorithm is for the -p option only applicable once you have parsed and have your commands
		ready 
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
	
	linked_tokens * start = make_linked_tokens(&input, count_char);
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

linked_tokens * make_linked_tokens(char* p_input, int input_size)
{
	token_t * start_token = make_token(HEAD, NULL);
	token_t * curret_token_t = start_token;
	
	linked_tokens * start_linked_tokens = checked_malloc(sizeof(linked_tokens));
	lniked_tokens * current_linked_tokens = start_linked_tokens;
	current_linked_tokens->head = current_token_t;
	
	// a|bd
	int index =0;
	char ch = *p_input;
	int parenCount = 0; //Used to count for nested parenthesis and detected incorrect numbers of parenthesis 
	while (index < input_size)
	{
		ch = *p_input;
		//In the case of a parenthesis AKA SUBSHELL
		if(ch=='(')
		{
			//(ab) ->subshell token
			//a|b -> a is left | is pipeline b is right
			//
			parenCount++
			
			
			//CHECK FOR NESTED SUBSHELL COMMANDS
		}
		else if(ch==')')//In the case of closed parenthesis 
		{
			error(2, 0, "closed parenthesis found without leading parenthesis");
			return NULL;
		}
		else if(ch=='&') //Beginning of a case of & and &&
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
		else if(ch=='|') //PIPE CASE
		{ //a|b
			//CHECK FOR || CASE
			p_input++;
			index++;
			if (*p_input == '|')
			{
				token_t * tok = make_token(OR, NULL);
				current_token->next = tok;
				current_token = current_token->next;
			}
			else
			{
				token_t * tok = make_token(PIPELINE, NULL);
				current_token->next = tok;
				current_token = current_token->next;
			}
		}
		else if(ch==' ' || ch=='\t') //WHITESPACE && TAB CASE
		{
			index++; //a_b
			p_input++;
		}
		else if(ch=='\n')//NEWLINE CASE
		{
		}
		else if(ch==';')
		{
			token_t * tok = make_token(SEMICOLON, NULL);
			current_token->next = tok;
			current_token = current_token->next;
			//Have to consider incrementing JASMINE
		}
		else if(ch=='>')	//RIGHT DIRECT
		{
			token_t * tok = make_token(RIGHT, NULL);
			current_token->next = tok;
			current_token = current_token->next;
		}
		else if(ch=='<')	//LEFT DIRECT
		{
			index++;
			p_input++;
			token_t * tok = make_token(LEFT, NULL);
			current_token->next = tok;
			current_token = current_token->next;
		}
		else if(isValidWord(ch))
		{
			//OVERALL IDEA
			//**ATTEMPT TO FIND THE ENTIRE WORD AND THEN PASS THAT AS A TOKEN
			int word_space = 8; //size of a char
			int word_index =0;
			char * word = checked_malloc(sizeof(word_size)); //stores entire word
			do
				{
					ch = *p_input;
					if (!isValidWord(ch))
						{
							error(2, 0, "Error unrecognized character detected");
							return NULL:
						}
					if (word_index == word_space)
					{
						word_space *=8;
						word = checked_realloc(word, word_space);
					word[word_index] = ch;
					p_input++;
					index++;
				}
			while(index < input_size &&isValidWord(ch) && ch != EOF);
			
			token_t * tok = make_token(WORD, word);
			current_token->next = tok;
			current_token = current_token->next;
			
		}
	}
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

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  
  //Most likely to have the processing of the command tree?
  error (1, 0, "command reading not yet implemented");
  return 0;
}

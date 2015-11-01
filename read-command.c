// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h"
#include <error.h>
#include <stdlib.h>
#include <stdbool.h> //allows support of bool type
#include <ctype.h> //used for isalphanum()
#include <string.h> // required for strchr(), strcmp()
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

typedef struct linked_files linked_files;
typedef struct linked_tokens linked_tokens_t;
//typedef struct linked_tokens linked_tokens;
typedef struct stack * stack_t;
typedef struct stack stack;
typedef struct node node;
typedef struct node * node_t;
int make_branch_count = 0;

linked_tokens_t* make_linked_tokens(char* p_input, int input_size);
command_t make_command(token_t * head);

struct node {
  command_t val;
  node_t next;
};



struct linked_tokens
{
  token_t* head;
  linked_tokens_t* next;
};

struct linked_files{
   linked_files * next;
   char* file;
};

//This is specifically made for a linked list of tokens 
struct token
{
  char *value; //ERROR LIES HERE WITH THE TOKEN AND THE VALUE ASK SCOTT
  enum token_trait trait;
  token_t* next;
  int token_line;
};




token_t * make_token(enum token_trait type, char *value, int line)
{ 
  token_t * product = checked_malloc(sizeof(token_t));
  product->trait = type;
  product->value = value;
  product->next = NULL;
  product->token_line = line;
  return product;
}
//This struct will hold tokens that will all be used to parse one command tree through 

void free_linked_tokens(linked_tokens_t* linkedTokens)
{
  linked_tokens_t* current = linkedTokens;
  linked_tokens_t* previous;
  
  while(current != NULL)
  {
    //printf("line 106");
    token_t* curr_token = current->head;
    token_t* prev_token;
    
    while(curr_token != NULL)
    {
      prev_token = curr_token;
      curr_token = prev_token->next;
      free(prev_token);
      //printf("line 116");
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
  command_stream_t next; //pointer to next command stream
  command_t comm; // value read this value from read command 
  //get depends here
};


command_stream_t new_command_stream()
{
  command_stream_t p = checked_malloc(sizeof(struct command_stream));
  p->next = NULL;
  p->comm = NULL;
  return p;
}
//STACK FUNCTIONALITY IMPLEMENTED HERE
 
 struct stack 
 {
  int num_cmds;
  int cmd_index;
  command_t cmds[8];
 };
 
stack_t make_stack()
 {
  stack_t p = checked_malloc(sizeof(struct stack));
  p->num_cmds = 0;
  p->cmd_index = 0;
  int i =0;
  for (i; i<8; i++)
    p->cmds[i] = NULL;
  return p;
 }
 
 command_t peek(stack_t p)
 {
  
  return p->cmds[p->cmd_index - 1];
 }
 
 void push(stack_t p, command_t add)
 {
  //printf("ENTERTING PUSH\n");
  p->num_cmds++;
  p->cmds[p->cmd_index] = add;
  p->cmd_index++;
  //if(add->type==SIMPLE_COMMAND)
    //printf("Value of pushed word is:%s", add->u.word[0]);
 }
 
 command_t pop(stack_t p)
 {
  p->cmd_index--;
  command_t target = p->cmds[p->cmd_index];
  p->num_cmds--;
  
  return target;
 }
 
 void print_stack(stack_t p)
 {
  int i = p->cmd_index-1;
  while (i >=0)
  {
    /*switch (p->cmds[i]->type)
    {
      case SIMPLE_COMMAND:
        printf("Simple Command word is %s", p->cmds[i]->u);
        break;
      case AND_COMMAND:
      case OR_COMMAND:
      case PIPE_COMMAND:
      case SEQUENCE_COMMAND:
        break;
      default:
        break;
    }*/
    //printf("%d value is %s\n", i, p->cmds[i]->u.word[0]);
    i--;
  }
 }
 
 int stack_size(stack_t p)
 {
  return p->num_cmds;
 }
 //Current Operator must be higher than top of stack
 bool higherPrecedence(void * p, command_t current)
{
  stack_t ops =(stack_t) p;
  
  switch(current->type)
  {
    case PIPE_COMMAND:
      return peek(ops)->type == PIPE_COMMAND;
    case AND_COMMAND:
    case OR_COMMAND: 
      return (peek(ops)->type == AND_COMMAND || peek(ops)->type == OR_COMMAND || peek(ops)->type == PIPE_COMMAND);
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
  if(isalnum(c) || strchr("!+,-./:@^_", c) != NULL)
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
    //printf("Line 226");
      }
      while(next_arg != '\n' && next_arg != EOF && next_arg != -1);
    }
    
    if(next_arg !=-1 && next_arg != EOF)
    {
      count_char++;
      input[pos] = next_arg;
      if (pos == maxbuffspace)
      {
        input = (char*)checked_realloc(input, maxbuffspace * 2);
        maxbuffspace *= 2;
      //printf("sucessful realloc\n");
      }
    pos++;
      
    }
    
    //printf("Line 244");
  }
  while (next_arg != -1); 
  
  int i;
  /*for (i=0; i<pos; i++)
  {
  printf("%c",input[i]);
  }*/
    
  //OUTPUT NOW WORKS
  linked_tokens_t * start = make_linked_tokens(input, count_char);
  //printf("Concluded make_linked_tokens \n");
  
  if (start == NULL)
  {
  error(4, 0, "Error during tokenization");
  exit(1);
  }
  //MAKING ALL THE TRESS AND LINKING ALL THE TREES AT THIS POINT
  
  linked_tokens_t * token_stream = start;
  command_stream_t stream_head = NULL;//new_command_stream();
  command_stream_t curr_stream = NULL;//new_command_stream();
  command_stream_t prev_stream = NULL;//new_command_stream();
  
  //printf("\n\nEntered Branch Making portion of make_command_stream \n");
  while(token_stream != NULL && token_stream->head->next != NULL) //&& token_stream->next != NULL)
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
    
    token_stream = token_stream->next;
    
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
  if(stream_head ==NULL)
  {
  error(2,0,"STREAM_HEAD IS NULL");
  exit(1);
  }
  //printf("Branches made: %i \n", make_branch_count);
  return stream_head;
}

linked_tokens_t* make_linked_tokens(char* p_input, int input_size)
{
  token_t * start_token = make_token(HEAD, NULL, -1);
  token_t * current_token = start_token;
  
  linked_tokens_t * start_linked_tokens = makeLinkedTokens();
  linked_tokens_t * current_linked_tokens = start_linked_tokens;
  current_linked_tokens->head = current_token;
  
  // a|bd
  int index =0;
  char ch = *p_input;
  int parenCount = 0; //Used to count for nested parenthesis and detected incorrect numbers of parenthesis 
  int token_line = 1;   //For debugging purposes. token_line = line in input

  while (index < input_size)
  {
    ch = *p_input;
  //printf("line 317 \n");
    //In the case of a parenthesis AKA SUBSHELL
    if(ch=='(')
    {
    //printf("In the subshell case");
      //(ab) ->subshell token
      //a|b -> a is left | is pipeline b is right
      //
      parenCount++;   //should be parenCount == 1 here.
      int subshell_line = token_line;
      //let's allocate memory for the subshell (more than one char)
      size_t count_subshell=0;
      size_t size_subshell = 128;
      char* subshell = (char*)checked_malloc(size_subshell);
    //printf("Line 96 \n");
      //until we reach the end of the nested parentheses.
      while(parenCount > 0)
      {
        index++;
        p_input++;

        // subshell_line = token_line;
    //printf("Line 338 \n");
        //if the index is outside the bounds.
        if(index == input_size)
        {
          error(2, 0, "Line %d: you need to close your parenthesis", subshell_line);
          return NULL;
        }

        //within subshell, more than 2 newlines is OK.
        //if either newline or tab or empty space, consume all whitespace
        if(*p_input == '\n')
        {
          while(p_input[1]==' ' || p_input[1]=='\n' || p_input[1]=='\t') //WHITESPACE && TAB CASE
          {
      if(p_input[1]=='\n')
        token_line++;
            p_input++;
            index++;
      //printf("line 317 \n");
          }

          //newlines are special -- it can be substituted for semicolon.
          *p_input = ';';
      token_line++;
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
      current_token->next = make_token(SUBSHELL, subshell, subshell_line);
      current_token = current_token->next;
    }

    //In the case of closed parenthesis 
    else if(ch==')')
    {
      error(2, 0, "Line %d: closed parenthesis found without leading parenthesis", token_line);
      return NULL;
    }

    //Beginning of a case of & and &&
    else if(ch=='&') 
    {
    //printf("In the & case \n"); 
      p_input++;
      index++;
      if (*p_input == '&')
      {
        token_t * tok = make_token(AND, NULL, token_line);
        current_token->next = tok;
        current_token = current_token->next;
    p_input++;
    index++;
      }
      else
      {
        error(2,0, "Line %d: Single & is illegal and not recognized", token_line);
        return NULL;
      }
    }

    //PIPE CASE
    else if(ch=='|') 
    { 
    //printf("In the pipe case \n");
    //a|b
      //CHECK FOR || CASE
      p_input++;
      index++;
      //OR
      if (*p_input == '|')
      {
    //printf("Actually it was an OR\n");
        token_t * tok = make_token(OR, NULL, token_line);
        current_token->next = tok;
        current_token = current_token->next;
    p_input++;
    index++;
      }
      //PIPELINE
      else
      {
        token_t * tok = make_token(PIPELINE, NULL, token_line);
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
    //printf("in the newline case \n");
    token_line++;
      //IF WORD OR SUBSHELL BEFORE NEWLINE, it's a separate command.
      if(current_token->trait == WORD || current_token->trait == SUBSHELL)
      {
        if(current_token->trait != HEAD)
        {
          current_linked_tokens->next = checked_malloc(sizeof(linked_tokens_t));
          current_linked_tokens = current_linked_tokens->next;
          current_linked_tokens->head = make_token(HEAD, NULL, -1);
          current_token = current_linked_tokens->head;
        }
      }
      else if(current_token->trait == LEFT || current_token->trait == RIGHT)
      {
        error(2, 0, "Line %d: You cannot have left or right redirects.", token_line);
        return NULL;
      }

      p_input++;
      index++;
    }

    //SEMICOLON CASE
    else if(ch==';')
    {
    //printf("In the semicolon case \n");
      token_t * tok = make_token(SEMICOLON, NULL, token_line);
      current_token->next = tok;
      current_token = current_token->next;
    p_input++;
    index++;
      //Have to consider incrementing JASMINE
    }

    //RIGHT DIRECT CASE
    else if(ch=='>')  
    {
    //printf("In right redirect case \n");
      token_t * tok = make_token(RIGHT, NULL, token_line);
      current_token->next = tok;
      current_token = current_token->next;
    index++;
    p_input++;
    }

    //LEFT DIRECT
    else if(ch=='<')  
    {
    //printf("In the left redirect case \n");
      index++;
      p_input++;
      token_t * tok = make_token(LEFT, NULL, token_line);
      current_token->next = tok;
      current_token = current_token->next;
    }

    //VALID WORD CASE
    else if (isValidWord(ch))
    {
    //printf("Currently presently valid word case \n");
      //OVERALL IDEA
      //**ATTEMPT TO FIND THE ENTIRE WORD AND THEN PASS THAT AS A TOKEN
      int word_space = 8; //size of a char
      int word_index =0; 
      char * word = checked_malloc(sizeof(word_space+1)); //stores entire word
    
      do
      {
    //printf("character is:%c \n", ch);
    word[word_index] = ch;
    word_index++;
    
        //ch = *p_input; //p_input never changes! thus its infinite
        if (word_index == word_space)
        {
          word_space *=8;
          word = checked_realloc(word, word_space);
    }
    p_input++;
    index++;
    ch = *p_input;
    //printf("Line 486 \n");
    
     }
      while(index < input_size && isValidWord(ch) && ch != EOF);
    
      //word = checked_realloc(word, (word_index+2) * sizeof(char));
    word[word_index+1] = '\0';
    //printf("Final word is %s \n", word);
      token_t * tok = make_token(WORD, word, token_line);
      current_token->next = tok;
      current_token = current_token->next;
    }
  //FOR ANY OTHER CHARACTER THAT IS UNRECOGNIZED
    else
    {
    printf("Character not recognized is: %i", *p_input);
      error(2, 0, "Line %d: Unrecognized characters.", token_line);
    
      return NULL;
    }
  }
  return start_linked_tokens;
}

bool make_new_branch(stack * ops, stack * operands)
{
  
  //printf("Entered make_new_branch \n");
  
  if(stack_size(operands) <2)
    return false;
    
  //Use one operator and two oeprands 
  command_t operator = pop(ops);
  command_t second_command = pop(operands);
  command_t first_command = pop(operands);

  operator->u.command[0] = first_command;
  operator->u.command[1] = second_command;
  push(operands, operator);
  make_branch_count++;
  return true;
}
//(a|(b|(c|d)))
//Organizes it into proper command_trees
command_t make_command(token_t * head)
{
  //printf("Entered make_command \n");
  stack_t operands = make_stack();
  stack_t ops = make_stack();
  token_t * current_tok = head;
  command_t curr_cmd;
  command_t prev_cmd = NULL;
  int token_count = 0;
  int token_line = current_tok->token_line;
  
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
        curr_cmd->type = SUBSHELL_COMMAND;
        //Make another tree with the subshell command by calling on make_command...
        curr_cmd->u.subshell_command = make_command(make_linked_tokens(current_tok->value, strlen(current_tok->value))->head);
        
        //Add this operand to the operand stack
        push(operands, curr_cmd);
        break;
      case AND:
        //Pretty sure you just push the thing onto the stack 
        curr_cmd->type = AND_COMMAND;
        if (!isEmpty(ops) && higherPrecedence(ops, curr_cmd)) //if was replaced 
        {
          if (!make_new_branch(ops, operands))
          {
            error(2,0, "Line %d: Error making child branch", token_line);
              exit(1);
          }
          /*
          if(!branch_success)
            {
              
            }*/
        }
        push(ops, curr_cmd);
        //printf("Pushing AND\n");
        break;
      case PIPELINE:
        //Follows same behavior of the &&
        curr_cmd->type = PIPE_COMMAND;
        if (!isEmpty(ops) && higherPrecedence(ops, curr_cmd))
        {
          bool branch_success = make_new_branch(ops, operands);
          if(!branch_success)
            {
              error(2,0, "Line %d: Error making child branch Line 713", token_line);
              exit(1);
            }
        }
        push(ops, curr_cmd);
        //printf("Pushing PIPE\n");
        break;
      case OR:
        //Follows same behavior as |
        //printf("encountered OR Command\n");
        curr_cmd->type = OR_COMMAND;
        if (!isEmpty(ops) && higherPrecedence(ops, curr_cmd))
        {
          bool branch_success = make_new_branch(ops, operands);
          if(!branch_success)
            {
              error(2,0, "Line %d: Error making child branch Line 727", token_line);
              exit(1);
            }
        }
        
        push(ops,curr_cmd);
        //printf("Pushing OR\n");
        break;
      case SEMICOLON:
        curr_cmd->type = SEQUENCE_COMMAND;
        if (!isEmpty(ops) && higherPrecedence(ops, curr_cmd))
        {
          bool branch_success = make_new_branch(ops, operands);
          if(!branch_success)
            {
              error(2,0, "Line %d: Error making child branch Line 740", token_line);
              exit(1);
            }
        }
        push(ops, curr_cmd);
        break;
      case WORD:
        curr_cmd->type = SIMPLE_COMMAND;
        //printf("encountered word token\n");
        
        //COUNT NUMBER OF WORDS THAT MAY FOLLOW
        int num_ofWords = 1;
        token_t * it = current_tok;
        while (it->next != NULL && it->next->trait == WORD)
        {
          num_ofWords++;
          it = it->next;
        }
        //printf("%i words were detected\n", num_ofWords);
        
        
        //ALLOCATE ENOUGH SPACE FOR ALL THE WORDS 
        curr_cmd->u.word = checked_malloc((num_ofWords+1) * sizeof(char*));
        
        //Iterate through tokens again and assign words to curr_cmd->u.word
        int u_index =0;
        curr_cmd->u.word[0] = current_tok->value;
        //printf("0 word/letter is:%s\n", curr_cmd->u.word[0]);
        
        
        for (u_index = 1; u_index<num_ofWords; u_index++)
        {
          token_count++;
          current_tok = current_tok->next;
          //printf("%d word/letter is %s\n", u_index, current_tok->value);
          curr_cmd->u.word[u_index] = current_tok->value;
        }
        
        curr_cmd->u.word[u_index]=NULL;
        int j =0;
        //printf("Complete token is:");
        while (curr_cmd->u.word[j] != NULL)
          {
            //printf("%s ", curr_cmd->u.word[j]);
            j++;
          }
        //printf("\n\n");
        push(operands, curr_cmd);//KEEP THIS ORIGINAL CODE
        break;
      case LEFT:
        if ((prev_cmd==NULL) || (prev_cmd->type != SIMPLE_COMMAND && prev_cmd->type != SUBSHELL_COMMAND))
          {
            error(2, 0 ,"Line %d: Syntax error no input to redirect or non simple/subshell command was there", token_line);
            exit(1);
          }
        if (current_tok->next->trait != WORD)
        {
          error(2, 0, "Line %d: Redirect MUST be followed by a WORD", token_line);
          exit(1);
        }
        
        //CHECK IF PREV_CMD HAS NO OUTPUT OR INPUT or can you?
        if (prev_cmd->input != NULL)
        {
          error(2, 0, "Line %d: Previous command cannot have an input.", token_line);
          exit(1);
        }
        if (prev_cmd->output != NULL)
        {
          error(2, 0, "Line %d: Previous command cannot have an output.", token_line);
          exit(1);
        }
        current_tok = current_tok->next;
        if (current_tok->trait == WORD)
          prev_cmd->input = current_tok->value;       
        break;
      case RIGHT:
        if ((prev_cmd==NULL) || (prev_cmd->type != SIMPLE_COMMAND && prev_cmd->type != SUBSHELL_COMMAND))
          {
            error(2, 0 ,"Line %d: Syntax error no input to redirect", token_line);
            exit(1);
          }
        if (current_tok->next->trait !=WORD)
        {
          error(2, 0, "Line %d: Redirect MUST be followed by a WORD", token_line);
          exit(1);
        }
        //CHECK IF PREV_CMD HAS NO OUTPUT or CAN YOU HAVE REDIRECTS UPON REDIRECTS
        if (prev_cmd->output != NULL)
        {
          error(2, 0, "Line %d: Previous command already has some output.", token_line);
          exit(1);
        }
        current_tok = current_tok->next;
        if (current_tok->trait == WORD)
          prev_cmd->output = current_tok->value;
        break;
      default:
        break;
    }
    prev_cmd = curr_cmd;
    //current_tok = current_tok->next;
    token_count++;
    //printf("Line 698 \n");
  } while(current_tok != NULL && (current_tok=current_tok->next) != NULL);
  
  
  //printf("Iterated through %i tokens\n\n\n", token_count);
  print_stack(operands);
  //WE NEED TO EMPTY THE THE STACK AND COMPLETE THE TREE WITH WHATS LEFT 
  bool branch_success = true;
  command_t root;
  /*if (stack_size(operands) ==1)
  {
    root = pop(operands);
    return root;
  }*/
  //printf("size of operator stack is %i \n", stack_size(ops));
  //printf("size of operand stack is %i \n", stack_size(operands));
  while(stack_size(ops)>0)
  {
    //printf("Line 707 \n");
    branch_success = make_new_branch(ops, operands);
    if (!branch_success)
    {
      error(2,0,"Line %d: Error making child branch Line 837", token_line);
      exit(1);
    }
    
  }
  
  if(stack_size(operands) != 1)
  {
    error(2, 0, "Line %d: Unable to make a complete command tree root is missing", token_line);
    exit(1);
  }
  if(stack_size(operands) == 1)
    root = pop(operands);
  
  //Do we need deconstructors? for the two stacks 
  return root; //placeholder
  
  
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  //command_stream_t read = s;
  if(s == NULL || s->comm == NULL)
  return NULL;
  
  command_t target = s->comm;
  
  if (s->next!= NULL)
  {
    //s->comm = s->next->comm;
    //s->next = s->next->next;
    command_stream_t next = s->next;
    s->comm = s->next->comm;
    s->next = s->next->next;
    //*s = *s->next;
    free(next);
  }
  else
    s->comm = NULL;
  
  return target;  
}

// get dependency file
// check for inputs, outputs
linked_files * get_linked_files(command_t stream)
{
	// should we initialize these as NULL
	linked_files * list;
	linked_files * temp;
	linked_files * subshell_list;
	
	switch(stream->type)
	{
		case SUBSHELL_COMMAND:
			subshell_list = get_linked_files(stream->u.subshell_command);
		case SIMPLE_COMMAND:
			if (stream->input != NULL)
			{
				temp = checked_malloc(sizeof(linked_files));
				temp->next = NULL;
				temp->file = stream->input;
				list = temp;
			}
			
			if (stream->output != NULL)
			{
				temp = checked_malloc(sizeof(linked_files));
				temp->next = NULL;
				temp->file = stream->output;
				list = temp;
				/*
				if (list == NULL)
					list = temp;
				else
					list->next = temp;
					*/ //do we need this part 
			}
			
			if(subshell_list != NULL)
			{
				temp = list;
				list = subshell_list;
				list->next = temp;
			}
			break;
			
		case AND_COMMAND:
		case OR_COMMAND:
		case PIPE_COMMAND:
		case SEQUENCE_COMMAND:
			//first command
			list = get_linked_files(stream->u.command[0]);
			if(list != NULL)
			{
				temp = list;
				while(temp!=NULL){
					temp = temp->next;
				}
				temp = get_linked_files(stream->u.command[1]);
				list->next = temp;
			}
			else
			{
				list = get_linked_files(stream->u.command[1]);
			}
			
			break;
		default:
			error(3, 0, "command type not valid");
			break;
	}
	return list;
}

// compare c strings of file names when looping through linked_files
int check_dependency(linked_files * file_1, linked_files * file_2)
{
	if(file_1 == NULL || file_2 == NULL)
		return 0;
	else
	{
		linked_files * f1 = file_1;
		linked_files * f2 = NULL;
		while(f1 != NULL)
		{
			f2 = file_2;
			while(f2 != NULL)
			{
				if(strcmp(f1->file, f2->file)==0)
					return 1;
				f2 = f2->next;
			}
			f1 = f1->next;
		}
	}
	return 0; //if not dependent
}










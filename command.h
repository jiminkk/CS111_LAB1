// UCLA CS 111 Lab 1 command interface

typedef struct command *
command_t;
typedef struct command_stream *command_stream_t;
typedef struct linked_files linked_files;

struct linked_files{
   linked_files * next;
   char* file;
};
struct command_stream{
  command_stream_t next; //pointer to next command stream
  command_t comm; // value read this value from read command 
  linked_files * depends; //get depends here
};
//LINKED LIST AND STACK IMPLEMENTATION HERE
//LINKED LIST WILL BE USED IN THE IMPLEMENTATION 
//struct linked_list;
//struct stack;

/*
//Put command into the stack
void push(stack * p, command_t extra);
//allocate memory from sizeof(command)

//Put remove top item from stack and put it into result
command_t pop(void *p);
//free the memory from sizeof(command)


//Only access the data and put it into result
command_t peek(void *p);
//no need for memory 

//void* make_stack();
//Create space in memory 

//Maybe we need a destructor?
*/

void free_command (command_t cmd);

/* Create a command stream from LABEL, GETBYTE, and ARG.  A reader of
   the command stream will invoke GETBYTE (ARG) to get the next byte.
   GETBYTE will return the next input byte, or a negative number
   (setting errno) on failure.  */
command_stream_t make_command_stream (int (*getbyte) (void *), void *arg);

/* Read a command from STREAM; return it, or NULL on EOF.  If there is
   an error, report the error and exit instead of returning.  */
command_t read_command_stream (command_stream_t stream);

/* Print a command to stdout, for debugging.  */
void print_command (command_t);

void print_vcommand (command_t);

/* Execute a command.  Use "time travel" if the integer flag is
   nonzero.  */
void execute_command (command_t, int);

/* Return the exit status of a command, which must have previously been executed.
   Wait for the command, if it is not already finished.  */
int command_status (command_t);

linked_files * get_linked_files(command_t stream);

int check_dependency(linked_files * file_1, linked_files * file_2);

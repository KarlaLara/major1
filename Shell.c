/*
Mario Mendiola Jr, Terrence Jackson
Major Assignment 1
CSCE 3600
10/25/2017
*/
#include <sys/wait.h>
#include <fcntl.h> //(TJ)For file opening system calls
#include <sys/types.h> //(TJ)For file opening system calls
#include <sys/stat.h> //(TJ)For file opening system calls
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LSH_RL_BUFSIZE 128 //Associated with lsh_read_line
#define LSH_TOK_BUFSIZE 64 //Associated with lsh_split_line
#define LSH_TOK_DELIM " \t\r\n\a; " //Associated with lsh_split_line


/*####################### PROBLEMS #############################
-Everytime we try to do anything with the input redirectors, they give use an error saying the file
isnt in the directory. (Example: ls > text.txt) The files do exist some we dont know why it's saying that.
Nothing sure if is a problem with the line parsing, the why things are being read in oor if its cause
we actually need to program the redirection and dont know how.

-The program pasres the line into however many things are typed in with the "lsh_split_line" function, but
the args part only has 3 argument and never puts in anymore? args[2] is the highest it will go

-The Aurgument are staying in the buffer instead of emptying out when the user types in some
thing after the first set of commands.


*/

//pipe();
//pipe();

// (TJ) ############################# LOOPS AND PRINTS TO CHECK THINGS ##############################
/*

// Check to see what is in the args[] 
int i;
	// This is to check and see what variable are making it into this function
for (i = 0; i < num_builtins(); i++)
{
	printf("This Args %d : %s\n",i , args[i]); //(TJ) Added this to check args inputs
	//if ( strcmp( args[0], str[i]) == 0 )
	//{
		//(*func[i]) (args);
	//	return ( *func[i]) (args);
	//}
}





// Prints to see what is tokenized
//printf("This Token %d : %s\n",position , tokens[position]); //(TJ) Added this to check stuff 





for (i = 0; i < num_builtins(); i++)
{
	printf("This Args %d : %s\n",i , args[i]); //(TJ) Added this to check args inputs
}
	

*/


//###################### SHELL LOOPING FUNCTIONS ###############################
//Will launch the shell
int lsh_launch(char **args)
{
	
	pid_t pid, wpid, twopid;
	int status, i;
	pid = fork(); // (TJ)Forking for the first child process
	twopid = fork(); //(TJ) This is my test for the second child
	int fd1[2]; // (TJ) The array for the pipe 1
	int fd2[2]; // (TJ) The array for the pipe 2
	
	//stdin = 0, stdout = 1, stderr = 2
	pipe(fd1); // The actually making of the pipes. Puts file descriptors with current process 
	pipe(fd2);
	
	
	if (fork() == 0) //(TJ)Fork the first child process to hopefully get a pipe 
	{
		//(TJ)Stdout side of first child process. Connects to first pipe
		dup2(fd1[1], 1); //Take the STDOUT and connect it to the write end '[1]' on the first pipe
		
		//(TJ)Stdin side of first child process. Connects to second pipe
		//dup2(fd2[0], 0); // Take the STDIN and connects its to the read end '[0] on second pipe
		
		
	
		printf("\nYou made it to the fork in first child process! \n"); //(TJ) Added this to check args inputs
		
		
		// Child process
		if (execvp(args[0], args) == -1) 
		{
			execvp(args[0], args);  
			perror("lsh");
			printf("IN CHILD PROCESS \n"); //(TJ) Checkin to see where error is
		}
		exit(EXIT_FAILURE);
		
		
		close(fd1[0]); //Close the read end
		close(fd1[1]); //Close the write end
		close(fd2[0]); //Close the read end
		close(fd2[1]); //Close the write end					
		
		
		execvp(args[0],args);
			
	// (TJ) The child process failed from soem reason
	} else if (pid < 0) 
	{ 
		perror("lsh");
		printf("IN MAKING OF CHILD \n"); //Checkin to see where error is
			
	// (TJ)Parent Process
	} else { 
	//Wait for the first child process to finsih before making another(might still need so leaving comment to remind)
		printf("\nYou made it to the fork in first parent process! \n"); //(TJ) Added this to check args inputs
		
		//trying to make parent wait fo pid to finish
		//do
		//{
			
		//(TJ)fork the second child to execute 2nd command
		if(twopid == 0)
		{

			
			//(TJ)Stdin side of second child process. Connects to first pipe
			dup2(fd1[0],0);
			
			//(TJ)Stdout side of first child process. Connects to second pipe
			//dup2(fd2[1],1);
			
			// Child process
			if (execvp(args[0], args) == -1) 
			{
				execvp(args[0], args);  
				perror("lsh");
				printf("IN CHILD PROCESS \n"); //(TJ) Checkin to see where error is
			}
			exit(EXIT_FAILURE);
			
			
			
			close(fd1[0]); //Close the read end
			close(fd1[1]); //Close the write end
			close(fd2[0]); //Close the read end
			close(fd2[1]); //Close the write end	
			
			//run commad ? (loops endlessly around here atm i think)
			//prints twice cause 
			execvp(*args, args);
			
			// (TJ) The child process failed from soem reason
		} else if (pid < 0) 
		{ 
			perror("lsh");
			printf("IN MAKING OF CHILD \n"); // Checkin to see where error is
			
	// (TJ)Parent Process
		} else 
		{
			// trying to parent wait for twopid to finish
			//do
			//{
			printf("\nYou made it to the fork in second child process! \n");
			//waitpid(twopid, &status, WUNTRACED); 
			//} while (!WIFEXITED(status) && !WIFSIGNALED(status));


			//this is the wait for the child processes (two were made)
			for (i = 0; i < 2; i++)
			{
					wait(&status);
			}

			
		}
		// Loop for other parent
		//waitpid(pid, &status, WUNTRACED); 
		//} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		
		
		close(fd1[0]); //Close the read end
		close(fd1[1]); //Close the write end
		close(fd2[0]); //Close the read end
		close(fd2[1]); //Close the write end	
		

		
	}
	/* 
	if (pid == 0) 
	{
	// Child process
	if (execvp(args[0], args) == -1) 
	{
		execvp(args[0], args);  
		perror("lsh");
		printf("IN CHILD PROCESS \n"); //(TJ) Checkin to see where error is
	}
	exit(EXIT_FAILURE);
	} else if (pid < 0) {
	// Error forking
	perror("lsh");
	printf("IN MAKING OF CHILD \n"); //(TJ) Checkin to see where error is
	} else {
		// Parent process
		do 
		{
			printf("This is parent process! Process ID : %d \n\n", getpid()); //(TJ) Added this to check stuff 
			waitpid(pid, &status, 0);	//(TJ) Shouldnt need to do anything other than call the wait, right?
			//wpid = waitpid(pid, &status, WUNTRACED); 
		//} while (!WIFEXITED(status) && !WIFSIGNALED(status)); // exit the loop if the child processed normally
		} while (!WIFEXITED(pid) && !WIFSIGNALED(pid)); //(TJ) just... trying to past in child process to see what happens
	} */

  //return 1;
  return pid; //(TJ) Return the child process to the parent
}


//########################## COMMAND FUNCTIONS ##############################
//Will clear the screen when the program gets ran
int lsh_clrscr( char **args ) //Clears the screen
{
	system("@cls||clear");
	return 1;
}

int lsh_exit( char **args )
{
	return 0; // just closes the shell
}

int lsh_cd ( char **args )
{
	if ( args[1] == NULL ) // If there is no directory after cd then error
	{
		fprintf( stderr, "lsh: ERROR");
	}
	else
	{
		if (chdir( args[1] ) != 0) // changes directory of whatever was put into the arg as long as arg isn't zero
		{
			perror("lsh");
			printf("IN CD \n"); //(TJ) Checkin to see where error is
		}
	}
	return 1;
}

//(TJ)Using the '<' as a call to look for what is better redirected  
int lsh_output( char **args )
{
	int infp, outfp; //(TJ)The input and output files... descriptor variables?
	int i; //(TJ) For the 'for' loop
	int redir; // (TJ) tells you where your redirect operator is
	
	
	/*
		
	O_RDONLY	: Open the file so that it is read only.
	O_WRONLY	: Open the file so that it is write only.
	O_RDWR : Open the file so that it can be read from and written to.
	use >> : O_APPEND: Append new information to the end of the file.
	O_TRUNC	: Initially clear all data from the file.
	O_CREAT	: If the file does not exist, create it. If the O_CREAT option is used, then you must include the third parameter.
	O_EXCL : Combined with the O_CREAT option, it ensures that the caller must create the file. If the file already exists, the call will fail.
	
	*/
	
	
	//(TJ) Loops as many times as things tokened in cmd line
	for (i = 0; i < num_builtins(); i++)
	{
		//(TJ)Checks to see if ">" comes up and if so, Do the input redirect
		if ( args[i] == ">" ) 
		{
			//(TJ) Opening the input/output files that are associated with the the redirection
			//(TJ) ########## Look above to see what the 'O_..." varibles do ###########
			infp = open(args[i--], O_RDONLY); // looks at thing infront of ">" (GOING TO HAVE A PROBLEM WHEN IT COMES TO "cat text.txt > text2.txt" )
			outfp = open(args[i++], O_WRONLY | O_TRUNC | O_CREAT );	// Looks at thing behind the ">"
			
			// (TJ)Replaces the Standard input "0", with the input file "infp"
			dup2(infp, 0);
	
			// (TJ) Replaces the Standard output with the output file "outfp"
			dup2(outfp, 1);
			
			//(TJ) If the args[0] is a command, run the command; 
			//execvp(args[0], args);
			
		}
		
	}
	
	
	close(infp); // Close the input file
	close(outfp); // Close the output files	

	/*
	KARLA'S PART, NOT FINISHED
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdbool.h>
//#include <sys/types.h>
//#include <unistd.h>
//#include <string.h>

//test, this is coming from part 1

//char *line = "ls << out.txt";

//char **tokens = malloc(64 * sizeof(char*));

// set up pipes

	pid_t pid;
	pid = fork();
	//makes 2 pipes, each has two fds
	int fd1[2];
	pipe(fd1);


//check if line has < > symbols

	//declare variables
	bool check = false, append = false;
	int i, j, size, counter;
	char token, type;
	size = 3;
	counter = size;
	//loop through tokens until find tokens or ends
	while(check != true || counter > 0){
		for(i = 0; i < size; i++){
			for(j = 0; j < size; j++){
				printf("%c ", tokens[i][j]);
				//if token is found
				if(tokens[i][j] == '<' || '>'){
					check = true;
					type = tokens[i][j];
					//if double token therefore append symbol
					if(tokens[i][j+1] == tokens[i][j]){
						append = true;

					}
				}
			}
		}
	counter--;
	}
//for testing
printf("bool1: %d bool2: %d", check, append);

//if so, see if it is input, output, in-append or out-append
	//we can execute because there's redirection
	if(check == true){
		if(type == '<'){
			if(append == true){
				//execute <<
				if(pid == 0){
					//dup2(infile, 0);
				}
			}
			else{
				//execute <
				if(pid == 0){
					dup2(fd1[0], 0);
					execvp(tokens[0], tokens);
				}
			}
		}
		else if(type == '>'){
			if(append == true){
				//execute >>
				if(pid == 0){
					//dup
				}
			}
			else{
				// execute >
				if(pid == 0){
					dup2(fd1[1], 1);
					execvp(tokens[0], tokens);
				}
			}
		}
	}

	//double redirection
	//dup2(fd1[0],

	
	*/
	
	
}

//
int lsh_input( char **args )
{
	
	
	
}

//######################### COMMAND HELPER FUNCTIONS ########################
// An array of functions, so a bunch of functions in a array
int (*func[]) (char **) = 
{
	// Each function being called and put into the array
	&lsh_exit,
	&lsh_clrscr,
	&lsh_cd,
	&lsh_output
};

// Array of strings of each command label
char *str[] = 
{
	"exit",
	"clear",
	"cd",
	">"
};

int num_builtins()
{
	return sizeof(str) / sizeof( char * );
}

//##################### COMMAND LINE PARSING FUNCTIONS #########################
/**
   brief Read a line of input from stdin.
   return The line from stdin.
*/
char *lsh_read_line(void)
{
 /* int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize); //Allocate memory to read in the line 
  int c;

  if (!buffer) { //checks if the allocation was correct
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getline();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
  */
  /**********************************************************************************************/
  /*****************************	 DOES THE SAME SHIT THE OTHER CODE DID JUST SHORTER *************/
  
	char *line = NULL;
	ssize_t bufsize = 0;
	getline(&line, &bufsize, stdin);
	return line;
}

/**
   brief Split a line into tokens (very naively).
   param line The line.
   return Null-terminated array of tokens.
 */
char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
	//printf("This Token %d : %s\n",position , tokens[position]); //(TJ) Added this to check stuff 
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}


// This function will execute whatever commands were put in
int lsh_execute( char **args )
{	
	int i;

	//args = argv;
	//int num=0;

	if (args[0] == NULL)
	{
		// No command was entered
		return 1;
	}

	// For loop cycles through each argument. Saves us from having to use a big switch statement
	for (i = 0; i < num_builtins(); i++)
	{
		if ( strcmp( args[0], str[i]) == 0 ) // (TJ) changed from ( == 0) / 
		{
			{
				return ( *func[i]) (args);
				//num = i; // (TJ) using to see if i could pass in specific function calls
			}	 
		}
	
		return lsh_launch( args );
	}
}




//brief Loop getting input and executing it.
void lsh_loop(void) 
{
	int i;	//(TJ) Added for the arg checking
	char *line;
	char **args;
	int status;
	
	
	
	
	//(TJ) IMPORTANT NOTE : with pipes, '0'is the read end and '1' is the write end (EX: fd1[0] = read, fd1[1] = write)

  do {
    printf(">> ");
    line = lsh_read_line(); //Call the read line function for the shell
    args = lsh_split_line(line); //Split line for the args
    status = lsh_execute(args); //status of the shell, and executes the function (Returns a process i think?)
	
	//(TJ) Tested the output redir code here


	
	
	//printf("\n");
	/*for (i = 0; i < num_builtins(); i++)
	{
		//printf("This Args %d : %s\n",i , args[i]); //(TJ) Added this to check args inputs
	}
	printf("\n");
	*/
	
    free(line);
    free(args);

  } while ( status );
}


//############################### MAIN #####################################
int main(int argc, char **argv) 
{ 
		//system("@cls||clear"); //(tj) remove this later
  		// Run command loop.
 		lsh_loop();
  // Perform any shutdown/cleanup.
	
  
  
  
  return EXIT_SUCCESS;
}

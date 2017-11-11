/*
Mario Mendiola Jr, Terrence Jackson, Makayla Zacarias, Karla Lara
Major Assignment 1
CSCE 3600
10/25/2017
*/


#include <sys/wait.h>
#include <fcntl.h> 	//(TJ)For file opening system calls
#include <sys/types.h>  //(TJ)For file opening system calls
#include <sys/stat.h> 	//(TJ)For file opening system calls
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LSH_RL_BUFSIZE 512 //Associated with lsh_read_line (was 128)
#define LSH_TOK_BUFSIZE 128 //Associated with lsh_split_line (was 64)
#define LSH_TOK_DELIM " \t\r\n\a; " //Associated with lsh_split_line

// (TJ)FIND "IMPORTANT" IN THE FILE TO SEE THINGS I FELT WERE INPORTANT


/*(TJ) ####################### PROBLEMS #############################
-Everytime we try to do anything with the input redirectors, they give use an error saying the file
isnt in the directory. (Example: ls > text.txt) The files do exist some we dont know why it's saying that.
Nothing sure if is a problem with the line parsing, the why things are being read in oor if its cause
we actually need to program the redirection and dont know how.

-The program pasres the line into however many things are typed in with the "lsh_split_line" function, but
the args part only has 3 argument and never puts in anymore? args[2] is the highest it will go

-The Aurgument are staying in the buffer instead of emptying out when the user types in some
thing after the first set of commands.

-REFER TO LINE 472 FOR SOMETHING I NEED TO KNOW

-Is there a problem freeing the 'args' and 'line' in the loop? Are we sure we are done using them before they are free
 
 (IMPORTANT) When i changed some things, the 'num_builtins()' function was making me seg fault cause of 
 incorrect size of array i believe.
*/


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

//(TJ)Using this process for output redir. Not working i dont think
int lsh_output_process( char **args )
{
	pid_t pid;
	int status, i;
	pid = fork(); // (TJ)Forking for the first child process
	
	
	if (pid == 0) 
	{
	// Child process
		lsh_output( args ); // (TJ) figured i could run the output redir in function in teh child
		if (execvp(args[0], args) == -1) 
		{
			execvp(args[0], args);  
			perror("lsh");
			//printf("IN CHILD PROCESS \n"); //(TJ) Checkin to see where error is
		}
		exit(EXIT_FAILURE);
		
		//printf("This is child process! Process ID : %d \n", getpid());
	} else if (pid < 0) {
		// Error forking
		perror("lsh");
		//printf("IN MAKING OF CHILD \n"); //(TJ) Checkin to see where error is
	} else {
		// Parent process
		do 
		{
			//printf("This is parent process! Process ID : %d \n", getpid()); //(TJ) Added this to check stuff 
			waitpid(pid, &status, WUNTRACED);	//(TJ) Shouldnt need to do anything other than call the wait, right?
		} while (!WIFEXITED(status) && !WIFSIGNALED(status)); // exit the loop if the child processed normally

	} 

  //return 1;
  return pid; //(TJ) Return the child process to the parent
}

//(TJ)Using this process for some redir testing. Not working i dont think
int lsh_launch_redir(char **args)
{
	
	pid_t pid, tpid;
	int status,i;

  
	pid = fork(); // (TJ)Forking for the first child process
	if (pid == 0) 
	{
	// Child process

		if (execvp(args[0], args) == -1) 
		{
			execvp(args[0], args);  
			perror("lsh");
			//printf("IN CHILD PROCESS \n"); //(TJ) Checkin to see where error is
		}
	exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// Error forking
		perror("lsh");
		//printf("IN MAKING OF CHILD \n"); //(TJ) Checkin to see where error is
	} else {
		// Parent process
		do 
		{
			//printf("This is parent process! Process ID : %d \n\n", getpid()); //(TJ) Added this to check stuff 
			waitpid(pid, &status, WUNTRACED);	//(TJ) Shouldnt need to do anything other than call the wait, right?
		} while (!WIFEXITED(status) && !WIFSIGNALED(status)); // exit the loop if the child processed normally
		
		
		
		
		
			tpid = fork();
			if (tpid == 0) 
			{
			// Child process

			
				for (i = 0; i < 3; i++)
				{
					if (strcmp(args[i], ">") == 0 )
					{
						//return ( *func[3]) (args);
						//lsh_output_process( args );
						printf("WE MADE IT IN THE REDIR OUTPUT!\n");
						lsh_output( args );
					}
				}	
			
			
			
			
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
					waitpid(tpid, &status, WUNTRACED);	//(TJ) Shouldnt need to do anything other than call the wait, right?
				} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		
				//not sure where id call this function for the 
				/* for (i = 0; i < 3; i++)
				{
					if (strcmp(args[i], ">") == 0 )
					{
						//return ( *func[3]) (args);
						//lsh_output_process( args );
						printf("WE MADE IT IN THE REDIR OUTPUT!\n");
						lsh_output( args );
					}
				}		 */		
		
		
		
		
			}
		
		
		
		
	} 

  return 1;
  //return pid; //(TJ) this is actually the parent process ID
}



//Will launch the shell
int lsh_launch(char **args)
{
	
	pid_t pid, tpid;
	int status,i;

	
	/*
	
	
	//(TJ)Some piping code im messing with  
	
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
	
		//printf("\nYou made it to the fork in first child process! \n"); //(TJ) Added this to check args inputs
		
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
		//printf("\nYou made it to the fork in first parent process! \n"); //(TJ) Added this to check args inputs
		
		//trying to make parent wait fo pid to finish
		//do
		//{
			
		//(TJ)fork the second child to execute 2nd command
		if(twopid == 0)
		{

			//printf("\nYou made it to the fork in second child process! \n");
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
			//printf("\nYou made it to the fork in second parent process! \n");
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

  
  
  
	pid = fork(); // (TJ)Forking for the first child process
	if (pid == 0) 
	{
	// Child process

		if (execvp(args[0], args) == -1) 
		{
			execvp(args[0], args);  
			perror("lsh");
		}
	exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// Error forking
		perror("lsh");
	} else {
		// Parent process
		do 
		{
			//printf("This is parent process! Process ID : %d \n", getpid()); //(TJ) Added this to check stuff 
			waitpid(pid, &status, WUNTRACED);	//(TJ) Shouldnt need to do anything other than call the wait, right?
		} while (!WIFEXITED(status) && !WIFSIGNALED(status)); // exit the loop if the child processed normally
		
		
		
		// 2nd fork incase for another function to run 
		/*
			tpid = fork();
			if (tpid == 0) 
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
					printf("This is parent process! Process ID : %d \n", getpid()); //(TJ) Added this to check stuff 
					waitpid(tpid, &status, WUNTRACED);	//(TJ) Shouldnt need to do anything other than call the wait, right?
				} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			
		
			}
			
			*/
		
		
		
		
	} 

  return 1;
  //return pid; //(TJ) this is actually the parent process ID
}


//########################## COMMAND FUNCTIONS ################################
//Will clear the screen when the program gets ran
int lsh_clrscr( char **args ) //Clears the screen
{
	system("@cls||clear");
	return 1;
}


int lsh_exit( char **args )
{
	printf("Now Exiting For Shell\n");
	return 0; // just closes the shell
}


int lsh_cd ( char **args )
{
	//lsh_launch(args); //(TJ) Pretty sure this is what is running the process 
	if ( args[1] == NULL ) // If there is no directory after cd then error
	{
		chdir(getenv("HOME"));
	} // change to home directory if no specific directory is given
	else
	{
		if (chdir( args[1] ) != 0) // changes directory of whatever was put into the arg as long as arg isn't zero
		{
			perror("lsh");
		}
	}

	return 1;
}


//(TJ)Using the '<' as a call to look for what is better redirected  
int lsh_output( char **args )
{
	int outfp;// infp; //(TJ)The input and output files... descriptor variables?
	int i; //(TJ) For the 'for' loop
	//int redir; // (TJ) tells you where your redirect operator is
	
	
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
	for (i = 0; i < 3; i++)
	{
		//(TJ)Checks to see if ">" comes up and if so, Do the input redirect
		if (strcmp(args[i], ">") == 0 ) 
		{
			//(TJ) Opening the input/output files that are associated with the the redirection
			//infp = open(args[i--], O_RDONLY); // looks at thing infront of ">" (GOING TO HAVE A PROBLEM WHEN IT COMES TO "cat text.txt > text2.txt" )
			outfp = open(args[i++], O_WRONLY | O_TRUNC | O_CREAT );	// Looks at thing behind the ">"
			
			// (TJ)Replaces the Standard input "0", with the input file "infp"
			//dup2(infp, 0);
	
			// (TJ) Replaces the Standard output with the output file "outfp"
			dup2(outfp, 1);
			
		}
		
	}
	
	
	//close(infp); // Close the input file
	close(outfp); // Close the output files	
	
	//(TJ) Just something to return
	return 1;
}

//
int lsh_input( char **args )
{
int infp, outfp; //input gile
	int i; //iterator
	
	//Loops as many times as things tokened in cmd line
	for (i = 0; i < 3; i++)
	{
		//Checks to see if ">" comes up and if so, Do the input redirect
		if (strcmp(args[i], ">") == 0 ) 
		{
			//Opening the input file
			infp = open(args[i--], O_RDONLY); // looks at thing infront of ">" (GOING TO HAVE A PROBLEM WHEN IT COMES TO "cat text.txt > text2.txt" )
			
			// (TJ)Replaces the Standard input "0", with the input file "infp"
			dup2(infp, 0);

		}
		
	}
	
	
	close(infp); // Close the input file
	
	return 1;

}

int lsh_path( char **args )
{
	int i,j, found = 0, position=0;
	char *path[100]; // aray to hold path list
	char *path1 = getenv("PATH");// path1 equal to current path
	path[0] = path1; // first slot is always the actual PATH location



	if (args[1] == NULL)
	{
		for (j = 0; j < 100; j++) //go through the entire path array 
		{
			printf("PATH : %s\n", path[j]); //print the current path list  (TJ) changed from 
		}
	}

	else
	{
		
		//if (args[1] == "+")
		if(strcmp(args[1], "+") == 0 ) 
		{
			for (j = 0; j < 100; j++)
			{
				if (path[j] == 0)
				{
					path[j] == args[2]; // if the next slot in array is empty, fill it with the path user wants to add, return for further commands
					return;
				}
				else
				{
					lsh_path(args); // if the slot is already full, go back through the function until it finds a free slot to put the argument in. 
				}
			}
		}
		
		//else if (args[1] == "-")
		else if (strcmp(args[1], "-") == 0)
		{
			for (i = 0; i < 100; i++)
			{
				if(path[i] == args[2])
				{
					int found = 1;
					int position = 1;// checks every position in the array and if it finds the word it says it is found and breaks
					break;
				}
			}
			
			if (found == 1)
			{
				for (i = position; i < 100; i++)
				{
					path[i] = path[i+1]; //shifts the array down one after deletion 
				}
			}
		}
	}

}



//######################### COMMAND HELPER FUNCTIONS ########################
// An array of functions, so a bunch of functions in a array
int (*func[]) (char **) = 
{
	// Each function being called and put into the array
	&lsh_exit,
	&lsh_clrscr,
	&lsh_cd,
	&lsh_output,
	&lsh_path
};


// Array of strings of each command label
char *str[] = 
{
	"exit", //	0
	"clear",//	1
	"cd",	//	2
	"output",//	3
	"path"//	4
};


int num_builtins()
{
	//printf("This Args Size from num bulitins %d \n", sizeof(sizeof(str)/sizeof( char * )));
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
	ssize_t bufsize = 0; //changed from ssize_t. not sure if it was a typo (IMPORTANT)
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
  int i;
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
//	printf("This is number of positions :  %d\n",position ); //(TJ) Added this to check stuff 
    position++;
    
	
	if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
	
	
	//printf("This token %d : %s \n",position , tokens[position]); //(TJ) Added this to check args inputs
    token = strtok(NULL, LSH_TOK_DELIM);

  }
  
  
  tokens[position] = NULL;
  return tokens;
}


// This function will execute whatever commands were put in
int lsh_execute( char **args )
{	
	int i,j;
	int status; // (TJ)Honestly only have this here to make things work snytaxtically
	pid_t exit_pid, pid ; //(TJ)fork for the exit process
	
	
	if (args[0] == NULL)
	{
		// No command was entered so keep running
		return 1;
	}
	
	// For loop cycles through each argument. Saves us from having to use a big switch statement
	for (i = 0; i < 3; i++) //(TJ)num_builtins() was where the 3 was
	{	
		if ( strcmp( args[i], "exit") == 0 )
		{
			//{
				
				
				exit_pid = fork(); //(TJ) fork the process to make exit parent		
				if( exit_pid == 0 )
				{
					//(TJ) Child process for the exit command to be done AFTER parent is done

					
					// (TJ) Trying to make this process handle cases that have exit in the command line
					// Basically run whatever command first before exiting
					for(j = 0; j < 3; j++)
					{
						//(TJ) If you find a out redirections, call lsh_output
						if (strcmp(args[j], ">") == 0 )
						{
							//return ( *func[3]) (args);
							lsh_output( args );
						}
						//(TJ) If you find a out redirections, call lsh_input
						else if (strcmp(args[j], "<") == 0 )
						{
							lsh_input( args );
						}
						//(TJ) If you find a out redirections, call lsh_append
						else if (strcmp(args[j], ">>") == 0 )
						{
							lsh_launch( args );
						}
						else if (strcmp(args[j], "cd") == 0 )
						{
							return ( *func[2]) (args); //(TJ)hardcoded func[] for cd function
						}
						else if (strcmp(args[j], "clear") == 0 )
						{
							return ( *func[1]) (args);//(TJ)hardcoded func[] for clear function
						}
						else
						{
							
							printf("Args ' %d | %s ' wasnt a redirect\n", j, args[j]);
						}
					}		

					// make a new process for a command that might have run after exit
					printf("This Launch In Exit Child\n");
					lsh_launch( args );
					
				} else if (exit_pid < 0) 
				{
					// Error forking
					perror("lsh");
				
				} else //(TJ)else for parent process
				{ 

					// (TJ)Runs the exit once we are done (I think you need to run this in the parent)
					do 
					{
						
						waitpid(exit_pid, &status, WUNTRACED);	//(TJ) Shouldnt need to do anything other than call the wait, right?
					} while (!WIFEXITED(status) && !WIFSIGNALED(status));
				}
		
				printf("The Return after the 'exit' parents\n");
				return ( *func[i]) (args);
			//}	 
		
		


		
		// (TJ) These are the else for when not looking at the exit
		}
		else if (strcmp(args[i], ">") == 0 )
		{
			//return ( *func[3]) (args);
			lsh_launch( args );
		}
		//(TJ) If you find a out redirections, call lsh_input
		else if (strcmp(args[i], "<") == 0 )
		{
			lsh_launch( args );
		}
		//(TJ) If you find a out redirections, call lsh_append
		else if (strcmp(args[i], ">>") == 0 )
		{
			lsh_launch( args );
		}
		else if (strcmp(args[i], "cd") == 0 )
		{
			return ( *func[2]) (args); //(TJ)hardcoded func[] for cd function
			lsh_launch( args );
		}
		else if (strcmp(args[i], "clear") == 0 )
		{
			return ( *func[1]) (args); //(TJ)hardcoded func[] for clear function
		}
		else if (strcmp(args[i], "path") == 0 )
		{
			return ( *func[4]) (args); //(TJ)hardcoded func[] for clear function
		}
		else {
			//If there is not command thats one of ours, do there commands
			//printf("This Launch If None Our Commands Typed\n");
			lsh_launch( args );
		
		}
		
		
		//return lsh_launch( args ); // (TJ) is this is trying to start a new child process for whatever the next command is? IMPORTANT
		return 1;
	}
//}
	
	
	//(TJ) First attempt at making some statements for forking processes
	/*
	
	printf("outside for loop > ");
 //(TJ)This is the over arching for loop looking for an exit command ()
	for(i = 0; i < num_builtins(); i++)
	{
		printf("i : %d | arg : %s" , i, args[i]);
		if(strcmp(args[i], "exit") == 0 )
		{
			// (TJ) Fork the process so it can wait and be last	
			
			exit_pid = fork(); //(TJ)fork for the exit process
			if( exit_pid == 0 )
			{
				//(TJ) Child process for the exit command to be done AFTER parent is done
				execvp("exit", args);
				
			} else if (exit_pid < 0) {
				// Error forking
				perror("lsh");
			} else {
				//(TJ) Will need to look at piping for these.... processes
				
				//(TJ) Inner loop to see if there is a files redirection 
				for(j = 0; j < num_builtins(); j++)
				{
					//(TJ) If you find a out redirections, call lsh_output
					if (strcmp(args[j], ">") == 0 )
					{
						lsh_output( args );
					}
					//(TJ) If you find a out redirections, call lsh_input
					else if (strcmp(args[j], "<") == 0 )
					{
						
					}
					//(TJ) If you find a out redirections, call lsh_append
					else if (strcmp(args[j], ">>") == 0 )
					{
						
					}
					else{
						printf("Args ' %d | %s ' wasnt a redirect", j, args[j]);
					}
				}
				
				
				// (TJ)Runs the exit once we are done (I think you need to run this in the parent)
					do 
					{
						//printf("This is parent process! Process ID : %d | pid : %d \n\n", getpid(), pid); //(TJ) Added this to check stuff 
						waitpid(exit_pid, &status, WUNTRACED);	//(TJ) Shouldnt need to do anything other than call the wait, right?
					} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			}
			
			
		}
		
	}
	
	*/

}
//brief Loop getting input and executing it.
void lsh_loop(void) 
{
	int i;	//(TJ) Added for loop that is checking other loop for exit function
	
	char *line;
	char **args;
	int status;
	


	
	
	//(TJ) IMPORTANT NOTE : with pipes, '0' is the read end and '1' is the write end (EX: fd1[0] = read, fd1[1] = write)
    
	

  do {
	printf("Prompt> "); // Changed cause the output is like this on the examples
    line = lsh_read_line(); //Call the read line function for the shell
    args = lsh_split_line(line); //Split line for the args
	
	
 	//status of the shell, and executes the function (Returns a process i think?)  
	//printf("Pre Execute > \n");	
	status = lsh_execute(args); //Changed from 'status = lsh_execute(args);'
	//printf("Post Execute > \n");


	
    free(line); //Is there a problem with freeing this here?
    free(args);
  } while ( status );

}


//############################### MAIN #####################################
int main(int argc, char **argv) 
{ 

  		// Run command loop.
 		lsh_loop();
  // Perform any shutdown/cleanup.
	
  

  
  return EXIT_SUCCESS;
}

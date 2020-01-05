# LinuxShell
About: This program Creates a Shell with a Linux enviroment.
Program Walkthrough:
First we print a prompt letting the know that we are in our new shell. Then we readCommand() method call. The readCommand(char *buffer) method calls fgets() which blocks the process until the user enters input into the command prompt, and then loads the line of characters into a buffer variable “commandLine”. 
parseCommand & Command_t structure
Next well call parseCommand(commandLine, &command). Command is variable representing a initialized structure named Command_t, that will hold the “name” of our command (which for ls will be “ls”), the number of arguments given to the command (the structure of linux command lines goes, command argument1 argument2 … argumenti), and *argv[] holding all the arguments  (*argv[] is a pointer to an array of characters, so each time it is incremented it points to a new char[] representing a new string). The command variable that represents a structure is being passed by reference to the parseCommand method so it can be modified. Parse command initialized the arrays to hold the arguments with” cmd->argv[argc] = (char *)malloc(MAX_ARG_LEN);”  This creates space for the arguments to go. Then the input string given by the user is tokenized around what we #defined as WHITESPACE  " \t\n”  . The whole string is tokenized the first string placed into the string array is the command, and then argument one, and then argument2, and so on. So, argv[0] now contains the “name” of the command. Now we return to the main() function where we do command.name = lookupPath(command.argv, pathv);
lookupPath-char *lookupPath(char **argv, char *dir[])
LookupPath has the job of taking the command name from argv[0] and finding its path. The first thing it does is check that argv[0] is not already a direct path to a command by checking if argv[0] string starts with a “\”. The second argument passed to lookupPath is pathv. pathv is an array of string that contains the parts of the paths delineated by “:” . The path is obtained from the linux operating system throught the “PATH” environment variable, using the cmd getenv(). Lookup path takes all these paths and then cats /arg[0](command name) onto the end. A while loop checks all the possible paths until access(pathv+/+arg[0]) say’s, “yes this is the path to this variable. Once the Path is found we return to the main function. In the main function we create a child process and run the command and arguments with execv(commandName,pathToCommand) and does the operations. The parent process loops and does it all again!

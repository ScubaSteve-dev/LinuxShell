#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINE_LEN	80
#define MAX_ARGS	64
#define MAX_ARG_LEN	16
#define MAX_PATHS	64
#define MAX_PATH_LEN	96
#define WHITESPACE	" \t\n"

#ifndef NULL
#define NULL -1
#endif


struct command_t
{
	char *name;
	int argc;
	char *argv[MAX_ARGS];
};

char *lookupPath(char **, char*[]);
int parseCommand(char *, struct command_t *);
int parsePath(char *[]);
void printPrompt();
void readCommand(char *);

int main()
{
	char commandLine[LINE_LEN];
	struct command_t command;
	//TODO shell initialization
	char *pathv[MAX_PATHS];
	parsePath(pathv);
	while (1)
	{
		printPrompt();

		//read command and parse it
		readCommand(commandLine);
		parseCommand(commandLine, &command);

		command.name = lookupPath(command.argv, pathv);
		if (command.name == NULL)
		{
			fprintf(stderr, "Command cannot be null.\n");
			continue;
		}

		int f = fork();
		if (f == 0)//TODO create child and execute the command
		{
			execv(command.name, command.argv);
			perror("execv");
			exit(0);
		}
		else if (f < 0)
		{
			fprintf(stderr, "error creating child process\n");
		}
		else //wait for child to terminate
		{
			wait(NULL);
		}
	}
	return 0; //shell terminates
}

int parseCommand(char *cLine, struct command_t *cmd)
{
	int argc;
	/*Initialization*/
	argc = 0;
	cmd->argv[argc] = (char *)malloc(MAX_ARG_LEN);
	/* Fill argv[] */
	char* token = strtok(cLine, WHITESPACE);
	while (token != NULL)
	{
		cmd->argv[argc] = (char *)malloc(MAX_ARG_LEN);
		cmd->argv[argc] = token;
		argc++;
		token = strtok(NULL, WHITESPACE);
	}

	/* Set the commandname and argc*/
	cmd->argc = argc - 1;
	while (argc < MAX_ARGS)
		cmd->argv[argc++] = NULL;
	return 1;
}

void printPrompt()
{
	char promptString[1024];
	getcwd(promptString, sizeof(promptString));
	printf("[%s]$", promptString);
}

void readCommand(char *buffer)
{
	fgets(buffer, LINE_LEN, stdin);
}

int parsePath(char *dirs[])//TODO
{
	char *pathEnvVar;
	char *thePath;

	int i;
	for (i = 0; i < MAX_ARGS; i++)
		dirs[i] = NULL;
	pathEnvVar = getenv("PATH");
	//printf("%s\n", pathEnvVar);
	thePath = (char *)malloc(strlen(pathEnvVar) + 1);
	strcpy(thePath, pathEnvVar);

	//TODO Loop to parse thePath. looking for ':' delimiter between path names.

	char *token = strtok(thePath, ":");
	i = 0;
	while (token != NULL)
	{
		//printf("%s\n", token);
		dirs[i++] = token;
		token = strtok(NULL, ":");
	}

	return -1;
}

char *lookupPath(char **argv, char *dir[])//TODO
{
	if (*argv[0] == '/')//check if file name is already an absolute path name
	{
		return argv[0];
	}

	int i = 0;
	//use access() to see if the file is in a dir
	for (i = 0; i < MAX_PATHS; i++)
	{
		if (dir[i] != NULL)
		{
			char* temp = (char *)malloc(strlen(dir[i]) + 1);
			strcpy(temp, dir[i]);;
			//printf("%s\n", temp);	
			if (strlen(temp) < MAX_PATH_LEN)
			{
				if (access(strcat(strcat(temp, "/"), argv[0]), X_OK) == 0)
				{
					return temp;
				}
			}
		}
	}

	//File name not found
	fprintf(stderr, "%s: command not found\n", argv[0]);
	return NULL;
}
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported
// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

// shell  start
void ShellStart(){
    clear();
    printf("Welcome to our Linux Shell :)");
    char* username = getenv("USER");
    sleep(1);
    clear();
}

//get input
int GetInput(char* str){
	char* buf;
    //FILE *f = fopen("history.txt", "w");

	buf = readline("\n>> ");
	if (strlen(buf) != 0) {
        //fprintf(f, "%s\n", buf);
		add_history(buf);
        
		strcpy(str, buf);

		return 0;
        //fclose(f);
	} 
    else {
		return 1;
	}
}

// print Directory
void printDirectory(){
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("\nDir: %s", cwd);
}

// Function where the system command is executed
void execArgs(char** parsed){
	// Forking a child
	pid_t pid = fork();

	if (pid == 0) {
		if (execvp(parsed[0], parsed) < 0) {
			printf("\nWHAT IS THIS?!");
		}
		return;
	} else if (pid == -1) {
			printf("\nFailed forking child..");
		exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
}

// Help command 
void openHelp(){
	puts("\n***WELCOME TO MY SHELL HELP***"
		"\nList of Commands supported:"
		"\n>cd"
		"\n>ls"
		"\n>exit"
		"\n>rw"
		);

	return;
}

int firstPart(char* dir){
	FILE *fp;
	char word[1000];
	fp = fopen(dir, "r");
	if (fp == NULL) {
    	printf("The file address if wreong!\n");
} 	else {
    	while (!feof(fp)) {
			fscanf(fp,"%s%*[^\n]",word);
        	printf("word read is: %s\n", word);
        	strcpy(word,""); //is this correct?
    }
}
	fclose(fp);
	return 0;
}

// Function to execute builtin commands
int ownCmdHandler(char** parsed){

	int NoOfOwnCmds = 4, i, switchOwnArg = 0;
	char* ListOfOwnCmds[NoOfOwnCmds];
	char* username;

	ListOfOwnCmds[0] = "exit";
	ListOfOwnCmds[1] = "cd";
	ListOfOwnCmds[2] = "help";
	ListOfOwnCmds[3] = "fp"; //first part in each line 

	for (i = 0; i < NoOfOwnCmds; i++) {
		if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
			switchOwnArg = i + 1;
			break;
		}
	}
	switch (switchOwnArg) {
	case 1:
		printf("\nGoodbye\n");
		exit(0);
	case 2:
		chdir(parsed[1]);
		return 1;
	case 3:
		openHelp();
		return 1;
	case 4:
		firstPart(parsed[1]);
		return 1;
	default:
		break;
	}

	return 0;
}

// function for parsing command words
void parseSpace(char* str, char** parsed){
	int i;

	for (i = 0; i < MAXLIST; i++) {
		parsed[i] = strsep(&str, " ");

		if (parsed[i] == NULL)
			break;
		if (strlen(parsed[i]) == 0)
			i--;
	}
}

int processString(char* str, char** parsed){
	parseSpace(str, parsed);
	if (ownCmdHandler(parsed))
		return 0;
	else
		return 1;
}

int main(){
    FILE *fptr;
    //char fname[20]="history.txt";
    //fptr=fopen(fname,"w");	

	char inputString[MAXCOM], *parsedArgs[MAXLIST];
	int execFlag = 0;
	ShellStart();

	while (1) {
		// print shell line
		printDirectory();
		// take input
		if (GetInput(inputString)){
            //fprintf(fptr,"%s",GetInput(inputString));
			continue;
            
        }
        //fclose(fptr); 

		// process
		execFlag = processString(inputString,parsedArgs);

		// execute
		if (execFlag == 1)
			execArgs(parsedArgs);
	}
	return 0;
}

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

int GetInput(char* str){
	char* buf;
	FILE *f;
	f = fopen("history.txt", "w");

	buf = readline("\n>> ");
	if (strlen(buf) != 0) {		
        fprintf(f, "%s\n", buf);
		add_history(buf);
		strcpy(str, buf);

		return 0;
	} 
    else {
		return 1;
	}
}

void printDirectory(){
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("\nDir: %s", cwd);
}

// Function where the system command is executed
void execArgs(char** input){
	// Forking a child
	pid_t pid = fork();

	if (pid == 0) {
		if (execvp(input[0], input) < 0) {
			printf("\nWHAT IS THIS?!");
		}
		return;
	} else if (pid == -1) {
			printf("\nFailed forking child.");
		exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
}

// Help command 
void HelpMenu(){
	puts("\n***WELCOME TO MY SHELL HELP***\n"
		"\nList of Commands :\n"
		"\n>cd ->  change directory"
		"\n>ls -> long listing format"
		"\n>exit -> exit from shell"
		"\n>fp -> show first word in each line"
		"\nmrs -> show most repeat string"
		"\nds -> delete white spaces"
		"\ncl -> counting lines of file"
		"\nsuc -> show uncomment lines"
		"\nftl -> show first ten lines"
		);

	return;
}

int firstPart(char* dir){
	FILE *fp;
	char str[1000];
	fp = fopen(dir, "r");
	if (fp == NULL) {
    	fprintf(stderr, "Unable to open file\n" );
		return 0;
} 	else {
    	while (!feof(fp)) {
			fscanf(fp,"%s%*[^\n]",str);
        	printf("%s\n", str);
        	strcpy(str,"");
    }
}
	fclose(fp);
	return 0;
}

int MostRepeatString(char *dir){
    FILE *f;  
    char ch, *line;  
    size_t len = 0, read;  
    char words[1000][1000], word[20];  
    int i = 0, j, k, maxCount = 0, count;  
      
    f = fopen(dir,"r");  
    if (f == NULL){  
        fprintf(stderr, "Unable to open file\n" );
        return 0;
    }  
    //Since, C doesn't provide in-built function,   
    //following code will split content of file into words  
    while ((read = getline(&line, &len, f)) != -1) {  
          
        for(k=0; line[k]!='\0'; k++){  
            //Here, i represents row and j represents column of two-dimensional array words   
            if(line[k] != ' ' && line[k] != '\n' && line[k] != ',' && line[k] != '.' ){  
                words[i][j++] = (line[k]);  
            }  
            else{  
                words[i][j] = '\0';  
                //Increment row count to store new word  
                i++;  
                //Set column count to 0  
                j = 0;  
            }  
        }  
    }  
      
    int length = i;  
      
    //Determine the most repeated word in a file  
    for(i = 0; i < length; i++){  
        count = 1;  
        //Count each word in the file and store it in variable count  
        for(j = i+1; j < length; j++){  
            if(strcmp(words[i], words[j]) == 0 && (strcmp(words[i]," ") != 0)){  
                count++;  
            }   
        }  
        //If maxCount is less than count then store value of count in maxCount   
        //and corresponding word to variable word  
        if(count > maxCount){  
            maxCount = count;  
            strcpy(word, words[i]);  
        }  
    }  
      
    printf("%s\n", word);  
    fclose(f);  
      
    return 0;  
}

int DelSpace(char *dir){
    FILE * f;
	int a;

	f = fopen (dir,"r");
	if (f == NULL) {
    	fprintf(stderr, "Unable to open file\n" );
}
	if (f){
		do {
			a = fgetc (f);
			if ( isgraph(a) ) putchar (a);
		} while (a != EOF);
		fclose (f);
	}
	printf("\n");
	return 0;
}

int CountLine(char *dir){
    FILE *f;
    int count = 1;  // Line counter
    char c;  // store a char from file
    f = fopen(dir, "r");
 
    if (f == NULL){
        fprintf(stderr, "Unable to open file\n" );
        return 0;
    }
 
    // Extract characters from file and store in character c
    for (c = getc(f); c != EOF; c = getc(f))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
 
    // Close the file
    printf("The file %s has %d lines\n ", dir, count);
    fclose(f); 
    return 0;
}

int ShowUnComment(char *dir){
	FILE *f;
	f = fopen(dir, "r");
	if(f == NULL){
		fprintf(stderr, "Unable to open file\n" );
		return 0;
	}
	int cc;
	while((cc = getc(f)) != EOF){
		if(cc == '#'){
			while(cc != '\n'){
				cc = getc(f);
			}
		printf("\n");
		}
		else{
			printf("%c",cc);
		}
	}
}

int FirstTenLine(char *dir){
	FILE *f;
    char content;
    int max = 0;

    // Open file
    f = fopen(dir, "r");
    if (f  == NULL){
        fprintf( stderr, "Unable to open file\n" );
        return 0;
}
    // Read the first 10 lines from file
    content  = fgetc(f);
    while (content != EOF && max < 10){
        if(content == '\n'){
            max++;
        }
        printf ("%c", content);
        content = fgetc(f);
    }
 
    fclose(f);
    return 0;
}

// Function to execute builtin commands
int ownCmdHandler(char** parsed){

	int NoOfCmds = 9, i, switchOwnArg = 0;
	char* ListOfCmds[NoOfCmds];
	char* username;

	ListOfCmds[0] = "bye";
	ListOfCmds[1] = "cd";
	ListOfCmds[2] = "help";
	ListOfCmds[3] = "fp"; //first part in each line 
    ListOfCmds[4] = "mrs"; //most repeat string
    ListOfCmds[5] = "ds"; //delete spaces
    ListOfCmds[6] = "cl"; //count line
	ListOfCmds[7] = "suc"; //show uncomment txt
	ListOfCmds[8] = "ftl"; //first ten line 

	for (i = 0; i < NoOfCmds; i++) {
		if (strcmp(parsed[0], ListOfCmds[i]) == 0) {
			switchOwnArg = i + 1;
			break;
		}
	}

	pid_t pid = fork();
	if (pid == 0){
	switch (switchOwnArg) {
	case 1: 
		exit(3); //bye
	case 2: 
		exit(4); //cd
	case 3: 
		HelpMenu();
		exit(1); //help
	case 4:
		firstPart(parsed[1]); //fp
		exit(1);
    case 5:
        MostRepeatString(parsed[1]); //mrs
		exit(1);
    case 6:
        DelSpace(parsed[1]); //ds
		exit(1);
    case 7:
        CountLine(parsed[1]); //cl
		exit(1);
	case 8:
		ShowUnComment(parsed[1]); //suc
		exit(1);
	case 9:
		FirstTenLine(parsed[1]); //ftl
		exit(1);
	
	default:
		break;
	}

	exit(0);
}
else{
	int status;
	waitpid(pid, &status, 0);
	int swReturn = WEXITSTATUS(status);

	if(swReturn == 3){ // for bye 
		printf("\nBye Bye\n");
		exit(0);
		}
	if(swReturn == 4){
		chdir(parsed[1]); 
	}
	return swReturn;
	}
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

void ShellStart(){
    printf("\033[H\033[J"); // clear shell
    printf("Welcome to our Linux Shell :)\n");
    char* username = getenv("USER");
    sleep(1);
}

void sigintHandler(int sig_num){ // for don't work ctrl + c
    signal(SIGINT, sigintHandler);
    printf("\n Cannot be terminated using Ctrl+C \n");
    fflush(stdout);
}

int main(){
	char inputString[MAXCOM], *parsedArgs[MAXLIST];
	int execFlag = 0;
	signal(SIGINT, sigintHandler);
	ShellStart();
	while (1) {
		printDirectory();
		if (GetInput(inputString)){
			continue;            
        }
		execFlag = processString(inputString,parsedArgs);

		if (execFlag == 1)
			execArgs(parsedArgs);
	}
	return 0;
}

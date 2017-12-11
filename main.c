#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <time.h>
#include <string.h>
#include <direct.h>
#include <Windows.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>

#define SIZE 15
//new line on rmdir and mkdir

char *token[5];//maximum number of tokens in a command is 5
int tokenCount = 0;//

//this function prints the current directory
void printcwd(char *buffer){
    if( (buffer = _getcwd( NULL, 0 )) == NULL )// Get the current working directory:
        perror( "_getcwd error" );
    else{
        printf( "%s>", buffer);
        free(buffer);
    }
}
//this functions counts string length
int getStringCount(char *string) {
	int i = 0;
	while(1) {
		if(string[i] != '\0') {
			i++;
		} else {
			break;
		}
	}

	return i;
}
//this function prints an error statement
void notFound() {
	printf("The system cannot find the path specified.\n");
}
//this function prints an invalid statement
void printNotValid(){
    printf("'%s' is not recognized as an external or internal command,\noperable program or batch file.\n", token[0]);
}
//this function does the type command
void doHelp(){
    printf("For more information on a specific command, type HELP command-name\n");
    printf("CD\tDisplays the name of or changes the current directory.\n");
    printf("CHDIR\tChanges the current directory.\n");
    printf("CLS\tClears the screen.\n");
    printf("CMD\tStarts a new instance of the command interpreter.\n");
    printf("COLOR\tSets the default console foreground and background colors.\n");
    printf("COPY\tCopies one or more files to another location.\n");
    printf("DATE\tDisplays or sets the date.\n");
    printf("DEL\tDeletes one or more files.\n");
    printf("DIR\tDisplays a list of files and subdirectories in a directory.\n");
    printf("HELP\tProvides Help information for Windows commands.\n");
    printf("MD\tCreates a directory.\n");
    printf("MKDIR\tCreates a directory.\n");
    printf("MOVE\tMoves one or more files from one directory to another directory.\n");
    printf("RENAME\tRenames a file or files.\n");
    printf("RD\tRemoves a directory.\n");
    printf("RMDIR\tRemoves a directory.\n");
    printf("TIME\tDisplays or sets the system time.\n");
    printf("TYPE\tDisplays the contents of a text file.\n");

}
//this function does the type command
void doType(){
    FILE *file;//a pointer to a file
    int filechar;//variable holder for a character

    file = fopen(token[1], "r");//opens the file for reading

    //error handling
    if (file == NULL){
        printNotValid();
        return ;
    }
    else{
        //prints out character by character if file is not null
        while ((filechar = fgetc(file)) != EOF){
            printf("%c", filechar);
        }
        fclose(file);//closes file
    }
    printf("\n");
    return ;
}
//this function does the time command
int doTime(){
    char newTime[SIZE];//variable holder for the new time
    char currentTime[100];//variable holder for the current time
    time_t now = time (0);//system time
    char *timeArray[SIZE];//variable holder for getting time elements
    int i = 0, hour, min, sec, msec;

    strftime (currentTime, 100, "%H:%M:%S.000", localtime (&now)); //gets current time
    printf ("The current time is: %s\n", currentTime);

    printf("Enter the new time: (hh:min:sec.msec) ");//gets new time input from user
    gets(newTime);

    //tokenizes newTime per element
    timeArray[i] = strtok(newTime,".:");
    while(timeArray[i]!=NULL){
       timeArray[++i] = strtok(NULL,".:");
    }
    if(i < 3){
        printf("A required privilege is not held by the client.\n");
        return 0;
    }
    //converts string element of newTime to integer
    hour = atoi(timeArray[0]);
    min = atoi(timeArray[1]);
    sec = atoi(timeArray[2]);
    msec = atoi(timeArray[3]);

    SYSTEMTIME str_t;
	GetSystemTime(&str_t);

	struct tm str_time;

    //sets time elements
    str_time.tm_hour = hour;
    str_time.tm_min = min;
    str_time.tm_sec = sec;
    str_time.tm_isdst = msec;

    mktime(&str_time);//sets the new time
    //printf("New time is: %d:%d:%d.%d\n", str_time.tm_hour, str_time.tm_min, str_time.tm_sec, str_time.tm_isdst);
    printf("A required privilege is not held by the client.\n");
    return 1;
}
//this function does the rmdir command
int doRmdir(){
    int i = 1;

    if(token[1] == NULL){
        printNotValid();
        return 0;
    }

    //removes directory if found
	while(token[i] != NULL) {
		if(rmdir(token[i]) == -1) {
			notFound();//else prints an error statement
		}
		i++;
	}
	return 1;
}
//this function does the rename command
int doRename(){
    int result;


    if(token[1] == NULL || token[2] == NULL){
        printNotValid();
        return 0;
    }
    result = rename(token[1], token[2]);//renames token[1] file name to token[2] file name

    //error handling
    if(result != 0) {
         notFound();
    }
    return 1;
}
//this function does the mkdir command
void doMkdir(){
    int i = 1;

    while(token[i] != NULL) {
        if(mkdir(token[i]) == -1) {
            printf("A subdirectory or file %s already exists.\n", token[i]);
        }
        i++;
    }
	return ;
}
//this function does the dir command
void doDir(DIR *dp, char *opendis){
    struct dirent *dir;
    dp = opendir(opendis);
    struct stat attr;
    char time[20];
    char size[10];
    char *newsize;
    int length;
    int commas;
    struct tm * timeinfo;
    int files = 0;
    int dirs = 0;
    char *buffer = "";

    printf("Volume in drive C is OS\n");
    printf("Volume ");
    system("wmic path win32_physicalmedia get SerialNumber");
    printf("Directory of ");
    printcwd(buffer);

    if (dp) {
        printf("\n\n");
        while ((dir = readdir(dp)) != NULL) {
            stat(dir->d_name, &attr);
            timeinfo = localtime (&attr.st_mtime);
            strftime(time, sizeof(time), "%m/%d/%Y %I:%M %p", timeinfo);
            printf("%20s", time);
            if (S_ISDIR(attr.st_mode)) {
                printf("%7s", "<DIR>");
                printf("%15s", " ");
                dirs++;
            }
            else {
                printf("%10s", " ");
                itoa((unsigned)attr.st_size, size, 10);
                length = getStringCount(size);
                if(length % 3 == 0) {
                	commas = (length / 3) - 1;
                } else {
                	int mod = length % 3;
                	commas = (length - mod) / 3;
                }
                length = commas + getStringCount(size);

                newsize = (char *)malloc(length);

                for(int i = getStringCount(size)-1; i >= 0; i--) {
                	if(((getStringCount(size)-i)%3 == 0) && i != 0) {
                		newsize[i + commas] = size[i];
                		newsize[i + --commas] = ',';
                	} else {
                		newsize[i + commas] = size[i];
                	}
                }
                newsize[length] = '\0';

                printf("%10s", newsize);
                printf("%2s", " ");
                files++;
            }
            printf("%s\n", dir->d_name);
        }
        closedir(dp);
    }
    else {
    	printf("File not found.\n");
    }

}
//this function does the delete command
int doDelete(){

    int i = 1;
	char cwd[1024];

    if(token[1] == NULL){
        printNotValid();
        return 0;
    }
	while(token[i] != NULL) {
		if(remove(token[i]) == -1) {
			getcwd(cwd, sizeof(cwd));
			if(i == 1) {
				notFound();
				return 0;
			}
		}
		i++;
	}

    return 1;
}
int doDelete2(){

    if(remove(token[1]) == -1) {
        notFound();//else prints an error statement
        return 0;
    }

    return 1;
}
//this function does the date command
int doDate(){
    char newdate[SIZE];//variable to hold the new date from input
    char *dateElements[SIZE];//variable to hold date elements
    int i = 0, year, month, day, go = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("The current date is: %d/%d/%d\n", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);

    while(go == 0){
        printf("Enter the new date: (mm-dd-yy) ");
        gets(newdate);

        //tokenize date
        dateElements[i] = strtok(newdate,"-/");
        while(dateElements[i]!=NULL){
           dateElements[++i] = strtok(NULL,"-/");
        }

        if(i < 2){
            printf("The system cannot accept the time entered.\n");
        }
        else
            go = 1;
    }

    //converts string date elements to integer
    month = atoi(dateElements[0]);
    day = atoi(dateElements[1]);
    year = atoi(dateElements[2]);

    SYSTEMTIME str_t;
	GetSystemTime(&str_t);

    struct tm str_time;

    //sets the date elements
    str_time.tm_year = year-1900;
    str_time.tm_mon = month-1;
    str_time.tm_mday = day;

    mktime(&str_time);
    //printf("New Date is: %d/%d/%d\n", str_time.tm_mon+1, str_time.tm_mday, str_time.tm_year+1900);
    printf("A required privilege is not held by the client.\n");
    return 1;
}
//this function changes the color of the shell
void doColor(){
    char color[50]="";

    if(token[1] == NULL){
        printNotValid();
        return ;
    }

    strcat(color, token[0]);
    strcat(color, " ");
    strcat(color, token[1]);
    printf("%s", color);

    system(color);
    return ;
}
//this function does the copy command
int doCopy(){
    //copy C:\Users\Munic\Desktop\text.txt C:\Users\Munic\Documents\codes\C\Shell\text.txt
    //copy text.txt text2.txt

    FILE *file1, *file2;//variables for file pointers
    char fileChar;//character holder for file
    int filesCopied = 1;

    file1 = fopen(token[1], "r");//open first file for reading

    //error handling
    if(token[1] == NULL || token[2] == NULL){
        printNotValid();
        return 0;
    }
    else{
        file2 = fopen(token[2], "w");

        //writes character by character
        while (1) {
            fileChar = getc(file1);
            if (fileChar == EOF){
                break;
            }
            else{
                fputc(fileChar, file2);
            }
        }

        if(strcmp(token[0],"copy") == 0)
            printf("\t %d file(s) copied.\n", filesCopied);
    }

    fclose(file1);
    fclose(file2);

    return 1;
}
//this function does the rename command
void doMove(){

    int filesMoved = 1;

    if(token[1] == NULL || token [2] == NULL){
        notFound();
    }
    else{
         if(doCopy() == 1 && doDelete2() == 1)
            printf("\t %d file(s) moved.\n", filesMoved);
        else
            notFound();
    }

}
//this function does the cmd command
void doCmd(){
    DWORD dwVersion = 0;
    DWORD dwMajorVersion = 0;
    DWORD dwMinorVersion = 0;
    DWORD dwBuild = 0;

    dwVersion = GetVersion();

    // Get the Windows version.
    dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

    // Get the build number.
    if (dwVersion < 0x80000000)
        dwBuild = (DWORD)(HIWORD(dwVersion));

    printf("Microsoft Windows [Version %ld.%ld.%ld]\n(c) 2017 Microsoft Corporation. All rights reserved.\n",
                dwMajorVersion,
                dwMinorVersion,
                dwBuild);
}
//this function does the cls command
void doCLS(){
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD coordScreen = { 0, 0 };    // home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

   if( !GetConsoleScreenBufferInfo( hConsole, &csbi )){// Get the number of character cells in the current buffer.
      return ;
   }

   dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

   if( !FillConsoleOutputCharacter(hConsole,(TCHAR)' ', dwConSize,coordScreen,&cCharsWritten )){
      return ;
   }

   if( !GetConsoleScreenBufferInfo( hConsole, &csbi )){
      return ;
   }

   if( !FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten )){
   }

   SetConsoleCursorPosition( hConsole, coordScreen );

   return ;
}
//this function does the chdir command
void doChdir(){
    char *buffer = "";
    char directory[1024];

    //if there is no other token after chdir and there is a ".." after
    if(token[1] == NULL){
        if(strlen(token[0]) == 7){
            if(token[0][5] == '.' && token[0][6] == '.'){
                chdir("..");
            }
        }
    }

    //if there is another token after chdir, change to that directory
    else if(token[1] != NULL){
        strcpy(directory, "./");
        strcpy(directory, token[1]);
        if(token[1][0] == '.' && token[1][1] == '.'){
            chdir(directory);
        }
        else if(chdir(directory) == -1) {
			printNotValid();
		}
    }
    printcwd(buffer);

    return ;
}
//this function does the cd command
void doCD(){
    char *buffer = "";
    char directory[1024];

    //if there is no other token after cd and there is a ".." after
    if(token[1] == NULL){
        if(strlen(token[0]) == 4){
            if(token[0][2] == '.' && token[0][3] == '.'){
                chdir("..");
            }
        }
    }

    //if there is no other token after cd and there is a ".." after
    else if(token[1] != NULL){
        strcpy(directory, "./");
        strcpy(directory, token[1]);
        if(token[1][0] == '.' && token[1][1] == '.'){
            chdir(directory);
        }
        else if(chdir(directory) == -1) {
			notFound();
		}
    }
    printcwd(buffer);

    return ;
}
//this function evaluates the input command
void evaluateToken(char *command){
    //the series of if statements evaluates the input command to which function to execute
    if(strcmp(command, "cd") == 0 || strcmp(command, "cd..") == 0){
        doCD();
        printf("\n");
    }
    else if(strcmp(command, "chdir") == 0 || strcmp(command, "chdir..") == 0){
        doChdir();
        printf("\n");
    }
    else if(strcmp(command, "cls") == 0){
        doCLS();
    }
    else if(strcmp(command, "cmd") == 0){
        doCmd();
    }
    else if(strcmp(command, "copy") == 0){
        doCopy();
    }
    else if(strcmp(command, "color") == 0){
        doColor();
    }
    else if(strcmp(command, "date") == 0){
        doDate();
    }
    else if(strcmp(command, "del") == 0){
        doDelete();
    }
    else if(strcmp(command, "dir") == 0){
        char cwd[1024];
		DIR *d;
		int i = 1;
		if(token[1] == NULL) {
//			printf(getcwd(cwd, sizeof(cwd)));
			d = opendir(getcwd(cwd, sizeof(cwd)));
			doDir(d, ".");
		} else {
			while(token[i] != NULL){
				d = opendir(token[i]);
				if(d == NULL) {
					printf("File not found\n");
				} else {
					printf("\nDirectory of %s\n", token[i]);
					doDir(d, token[i]);
				}
				i++;
			}
		}

    }
    else if(strcmp(command, "mkdir") == 0 || strcmp(command, "md") == 0){
        doMkdir();
    }
    else if(strcmp(command, "move") == 0){
        doMove();
    }
    else if(strcmp(command, "rename") == 0 || strcmp(command, "ren") == 0){
        doRename();
    }
    else if(strcmp(command, "rmdir") == 0 || strcmp(command, "rd") == 0){
        doRmdir();
    }
    else if(strcmp(command, "time") == 0){
        doTime();
    }
    else if(strcmp(command, "type") == 0){
        doType();
    }
    else if(strcmp(command, "help") == 0){
        doHelp();
    }
    else if(strcmp(command, "exit") == 0){
        exit(0);
    }
    else{
        printNotValid();
    }
    return ;
}
//this function tokenizes the input
void tokenize(char *command){
    int i = 0;

    //tokenize the input
    token[i] = strtok(command," \t");
    while(token[i]!=NULL){
       token[++i] = strtok(NULL," \t");
       tokenCount++;//counts the number of tokens
    }

    return ;
}
//this function converts the input to lowercase
void tolow(char *command){
    int i = 0;

    while( command[i] ) {
        command[i] = (tolower(command[i]));//converts to lower case
        i++;
    }
}
//this function handles input functions
void inputFunctions(){
    char command[1024];
    gets(command);//gets the input
    tolow(command);//converts to lowercase
    tokenize(command);//tokenizes the input
    evaluateToken(token[0]);//evaluates the command stored in token[0]
    tokenCount = 0;//token count resets to zero every iteration
}
//this function accepts input while not 'exit'
void start(){
    doCmd();
    char *currentDirectory;

    while(1){
        if( (currentDirectory = _getcwd( NULL, 0 )) == NULL )
            perror( "_getcwd error" );
        else{

            printf( "\n%s>", currentDirectory);
            free(currentDirectory);
        }
        inputFunctions();
    }
}

int main(){
    token[0] = "";//initializes command to empty string
    start();//call starts function
    return(0);
}

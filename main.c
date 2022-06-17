#include <stdio.h>
#include "string.h"
#include <unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <time.h>
#include <memory.h>
#include <errno.h>

typedef struct{
    char *name;
    char* time;
} record;

void color(char *color){
    if (strcmp(color, "red") == 0) {
        printf("\033[0;31m");
    }else if (strcmp(color, "blue") == 0){
        printf("\033[0;34m");
    }else if (strcmp(color, "green") == 0) {
        printf("\033[0;32m");
    } else {
        printf("unsupported theme\n");
    }
}

void print(char **args){
    int i = 1;
    while(args[i] != NULL){
        printf("%s ",args[i]);
        i++;
    }
    printf("\n");
}

record log_add(char* command){
    record newNode;
    char *temp = strdup(command);
    newNode.name = temp;
    time_t t;
    t = time(NULL);
    char *temp2 = strdup(asctime(localtime(&t)));
    newNode.time = temp2;
    return newNode;
}

void printIndivisualLog(record log){
    printf("%s", log.time);
    printf("%s 0\n", log.name);
}

void log_print(record *array, int counter){
    for(int i=0; i < counter; i++){
        printIndivisualLog(array[i]);
    }
}

void cshell(){

    char* args[200];          // Argument array.
    char userInput[200];     // User input.
    int pid;                 // Process ID for fork().
    int logCounter = 0;
    record log[200];

    do {
        int i = 0;               // Counter.
        // Take user Input and parse it
        printf("cshell$");
        fgets(userInput, sizeof(userInput), stdin);
        userInput[strlen(userInput)-1] = '\0';

        args[0] = strtok(userInput, " ");

        while(args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        // the programmer wants to exit the application
        if(strcmp(args[0], "exit") == 0) {
            printf("Bye!\n");
            exit(0);
        }

        pid = fork();

        // Parent process will wait for child to execute.
        // Child process will execute the command given in userInput.
        if(pid > 0) {
            // Parent //
            wait( (int *) 0 );
        } else {
            if (strcmp(args[0], "log") == 0) {
                log_print(log, logCounter);
            } else if (strcmp(args[0], "print") == 0){
                print(args);
            }else if(strcmp(args[0], "theme") == 0) {
                color(args[1]);
            } else if(execvp(args[0], args) < 0) {
                 printf("Missing keyword or command, or permission problem\n");
             }
            exit(0);
        }
        log[logCounter++] = log_add(args[0]);
    } while(1);

}

int main(int argc, char** argv) {



    cshell();
//    exit(0);

    return 0;
}

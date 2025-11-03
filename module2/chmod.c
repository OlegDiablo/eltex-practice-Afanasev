#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void printMask(mode_t mask){

    printf((mask & S_IRUSR) ? "r" : "-");
    printf((mask & S_IWUSR) ? "w" : "-");
    printf((mask & S_IXUSR) ? "x" : "-");
    printf((mask & S_IRGRP) ? "r" : "-");
    printf((mask & S_IWGRP) ? "w" : "-");
    printf((mask & S_IXGRP) ? "x" : "-");
    printf((mask & S_IROTH) ? "r" : "-");
    printf((mask & S_IWOTH) ? "w" : "-");
    printf((mask & S_IXOTH) ? "x" : "-");
    printf("\n");

}

void printBitMask(mode_t mask){

    printf((mask & S_IRUSR) ? "1" : "0");
    printf((mask & S_IWUSR) ? "1" : "0");
    printf((mask & S_IXUSR) ? "1" : "0");
    printf("-");
    printf((mask & S_IRGRP) ? "1" : "0");
    printf((mask & S_IWGRP) ? "1" : "0");
    printf((mask & S_IXGRP) ? "1" : "0");
    printf("-");
    printf((mask & S_IROTH) ? "1" : "0");
    printf((mask & S_IWOTH) ? "1" : "0");
    printf((mask & S_IXOTH) ? "1" : "0");
    printf("\n");

}


mode_t checkFile(char* filename){
 
    struct stat file_info;

    if(stat(filename, &file_info) == 0) {
        return file_info.st_mode & 0777;
    }

    return (mode_t)-1;

}


mode_t makeMask(char* owner, char* mode){

    mode_t owner_mask = 0;
    mode_t mode_mask = 0;
    int i = 0;
    while(owner[i] != '\0') {

        switch (owner[i]) {

            case 'u':

                owner_mask = owner_mask | S_IRWXU;               
                break;
            
            case 'g':

                owner_mask = owner_mask | S_IRWXG;
                break;

            case 'o':

                owner_mask = owner_mask | S_IRWXO;
                break;

            default:
                break;
        }

        i++;

    }

    i = 0;

    while(mode[i] != '\0') {

        switch (mode[i]) {

            case 'r':
                
                mode_mask = mode_mask | S_IRUSR | S_IRGRP | S_IROTH;
                break;
            
            case 'w':

                mode_mask = mode_mask | S_IWUSR | S_IWGRP | S_IWOTH;
                break;

            case 'x':

                mode_mask = mode_mask | S_IXUSR | S_IXGRP | S_IXOTH;
                break;

            default:
                break;
        }

        i++;

    }

    return mode_mask & owner_mask;

}


mode_t operProc(char oper, mode_t mask, mode_t file_mask){

    switch (oper) {

    case '+':

        return mask | file_mask;
    
    case '-':

        return ~ mask & file_mask;
    

    case '=':

        return mask;
    
    default:
        break;
    }

    return file_mask;

}


int inputParsing(char* input, int counter, char* owner, char* oper, char* mode, char* filename){

    int i = 0;
    while(input[i] != '+' && input[i] != '-' && input[i] != '=') {

        if(input[i] == 'u' || input[i] == 'g' || input[i] == 'o'){
            owner[i] = input[i];
            i++;
            owner = realloc(owner, sizeof(char)*(i+1));
        }
        else {
            return 1;
        }
        
    }
    
    owner[i+1] = '\0';

    if(input[i] == '+' || input[i] == '-' || input[i] == '=') {
        oper[0] = input[i];
        i++;
    }
    else {
        return 1;
    }

    
    for(int j = 0; input[i] != ' '; j++) {

        if(input[i] == 'r' || input[i] == 'w' || input[i] == 'x') {
            mode[j] = input[i];
            i++;
            mode = realloc(mode, sizeof(char)*(j+1));
        }
        else {
            return 1;
        }

    }

    mode[i+1] = '\0';
    i++;
    counter -= i;
    int filename_lenth = 0;
    for(int j = 0; j < counter; j++) {

        filename[j] = input[i];
        i++;
        filename = realloc(filename, sizeof(char)*(j+1));
        filename_lenth = j;
        
    }
    filename[filename_lenth+1] = '\0'; 

    return 0;
}


int main(){

    while(1) {

        char* input = calloc(1 ,sizeof(char));
        char symbol;
        int counter = 0;
        printf("Input command: ");

        while(scanf("%c", &symbol) == 1 && symbol != '\n'){
            
            input[counter] = symbol;
            counter++;
            input = realloc(input, sizeof(char) * (counter+1));

        }

        char* owner = calloc(1, sizeof(char));
        char oper = ' ';
        char* mode = calloc(1, sizeof(char));
        char* filename = calloc(1, sizeof(char));

        int error = inputParsing(input, counter, owner, &oper, mode, filename);
        if(error == 1) {
            printf("Incorrect input\n");
            continue;
        }

        mode_t file_mask = checkFile(filename);
        if(file_mask == (mode_t)-1) {
            printf("No such file in directory\n");
        }
        printf("filemask of %s: ", filename);
        printMask(file_mask);

        mode_t mask = makeMask(owner, mode);
        mode_t new_mask = operProc(oper, mask, file_mask);
        printf("New mask: %03o\n", new_mask);
        printMask(new_mask);
        printBitMask(new_mask);
        
    }

    return 0;
}
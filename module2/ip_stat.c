#include <stdio.h>



void inputParsing(char* input, int counter, char* octets, int amount){

    int i = 0;
    while(input[i] != '/') {

        

    }

}


int main(){

    while(1) {
       
        char* input = calloc(1, sizeof(char));
        char symbol;
        int counter = 0;
        printf("Input command: ");

        while(scanf("%c", &symbol) == 1 && symbol != '\n'){
            
            input[counter] = symbol;
            counter++;
            input = realloc(input, sizeof(char) * (counter+1));

        }
    }
    return 0;
}
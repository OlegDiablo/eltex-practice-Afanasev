#include <stdio.h>

int charToInt(char data[3]){

    int result = 0;
    for(int i = 0; i < 1; i++){

        result = atoi(data);

    }

    return result;
}

int main(){

    char data[3] = {'1', '2', '3'};

    printf("%d", charToInt(data));

}
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int error = 0;

float addition(float a, float b){

    return a + b;

}


float multiplication(float a, float b){

    return a * b;

}


float division(float a, float b){

    if (b !=(float)0) {
        return a / b;
    }   
    else{
        error = 1;
        return a;
    }
    

}


float subtraction(float a, float b){

    return a - b;

}


int isValidInput(const char *input, int counter) {
    for (int i = 0; i < counter; i++) {
        
        if(input[0] == '?'){
            return 2;
        }

        else if(isdigit(input[i])) {
            continue;
        }
        
        else if(input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '\n') {
            continue;
        }

        else {
            return 0; 
        }
    }
    return 1; 
}


float charToInt(char* data){

    int result = atoi(data);

    return (float)result;
}


void shiftArr(float* values, char* operators, int oper_size, int num){

    for(int i = num; i < oper_size; i++){

        operators[i] = operators[i+1];

    }

    for(int i = num + 1; i < oper_size + 1; i++){

        values[i] = values[i+1];
        
    }

}


char* prioritiesDist(const char* operators, int oper_size){

    char* priority = calloc(1, sizeof(char));
    for(int i = 0; i < oper_size; i++){
        
        if(operators[i] == '*' || operators[i] == '/'){
            priority[i] = '2';
        }
        else {
            priority[i] = '1';
        }
        
    }
    
    return priority;

}


int inputParsing(char* input, int counter, char* operators, float* values){

    int arr_size = 0, values_size = 0, oper_size = 0;; 
    char* num = calloc(1, sizeof(char));

    for(int i = 0; i < counter; i++){

        num = realloc(num, sizeof(char) * (arr_size+1));

        if (input[i] != '*' && input[i] != '/' && input[i] != '+' && input[i] != '-'){
            
            
            num[arr_size] = input[i];
            arr_size++;
        }
        else{

            float res = charToInt(num);
            free(num);
            num = calloc(1, sizeof(char));

            values_size++;
            arr_size = 0;
            values = realloc(values, sizeof(float)*values_size);
            values[values_size-1] = res;

            oper_size++;
            operators = realloc(operators, sizeof(char)*oper_size);
            operators[oper_size-1] = input[i];

        }

    }

    float res = charToInt(num);
    printf("values_size=%d\n", values_size);
    values = realloc(values, sizeof(float)*(values_size+2));
    values[values_size] = res;

    for(int i = 0; i < oper_size + 1; i++){

        printf("%c ", operators[i]);

    }
    
    printf("\n");
    
    for(int i = 0; i < values_size + 1; i++){

        printf("%f ", values[i]);

    }

    printf("\n");
    printf("pointer in inputParsing is %p\n", values);

    return oper_size;

}


int operating(float* values, char* operators, int oper_size, int num){
    float res;
    
    printf("operating with %f and %f\n", values[num], values[num+1]);

    switch (operators[num])
    {
        case '*':

            res = multiplication(values[num], values[num+1]);
            values[num] = res;

            shiftArr(values, operators, oper_size, num);

            oper_size--;


            break;
    
        case '/':

            res = division(values[num], values[num+1]);
            values[num] = res;

            shiftArr(values, operators, oper_size, num);

            oper_size--;

            break;

        case '+':

            res = addition(values[num], values[num+1]);
            values[num] = res;

            shiftArr(values, operators, oper_size, num);

            oper_size--;

            break;

        case '-':

            res = subtraction(values[num], values[num+1]);
            values[num] = res;

            shiftArr(values, operators, oper_size, num);

            oper_size--;

            break;

        default:

            break;

    }

    printf("result is %f\n",res);

    return oper_size;

}


void calculation(float* values, char* operators, char* priority, int oper_size){

    for(int i = 0; i < oper_size; i++){
       
        if (priority[i] == '2'){
            printf("operating with %f and %f\n", values[i], values[i+1]);
            oper_size = operating(values, operators, oper_size, i);
            for(int j = i; j < oper_size; j++){

                priority[j] = priority[j+1];

            }

        }

    }

    while(oper_size != 0){

        oper_size = operating(values, operators, oper_size, 0);
        for(int i = 0; i < oper_size; i++){

            priority[i] = priority[i+1];

        }     

    }

}


int main(){

    while(1){

        char* expression = calloc(1, sizeof(char));
        printf("Input an expression:\n");

        int counter = 0, oper_size = 0;
        char input;
        float* values = calloc(1, sizeof(float));
        char* operators = calloc(1, sizeof(char));

        while(scanf("%c", &input) == 1 && input != '\n'){
            
            expression[counter] = input;
            counter++;
            expression = realloc(expression, sizeof(char) * (counter+1));
        }

        int check = isValidInput(expression, counter);

        if (check == 0){

            printf("Incorrect input\n");
            continue;

        }

        else if(check == 2){

            return 0;

        } 

        else {

            oper_size = inputParsing(expression, counter, operators, values);
            printf("pointer in main is %p\n", values);
            for(int i = 0; i < 6; i++){
                printf("%f ", values[i]);
            }
            char* priority = prioritiesDist(operators, oper_size);

            printf("\n");
            calculation(values, operators, priority, oper_size);
            if (error == 1){

                printf("Division by zero is not allowed\n");
                error = 0;
                continue;
            }

            else {

                printf("result of expression: %f\n", values[0]);
                free(values);
                free(operators);
                continue;

            }
        }

    }
    return 0;
}
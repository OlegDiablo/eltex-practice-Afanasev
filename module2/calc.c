#include <stdio.h>
#include <stdarg.h>

int mul(int n, ...) {

    int result = 1;
    va_list counter;
    va_start(counter, n);
    for (int i = 0; i < n; i++) {

        result *= va_arg(counter, int); 
    
    }
    return result;
}

int div(int n, ...) {

    int result = 1;
    int arr[n];
    va_list counter;
    va_start(counter, n);
    for (int i = 0; i < n; i++) {

        arr[i] = va_arg(counter, int); 
        
    }

    result = arr[0];

    for (int i = 0; i < n - 1; i++) {
        
        if (arr[i] > arr[i+1] || arr[i+1] == 0) {

            result /= arr[i+1];

        } 
        else {

            printf("Incorrect input");
            return 89765;

        }

    }

    return result;
}

int add(int n, ...) {

    int result = 0;
    va_list counter;
    va_start(counter, n);
    for (int i = 0; i < n; i++) {

        result += va_arg(counter, int); 
    
    }
    return result;
}

int sub(int n, ...) {

    int result = 0;
    int arr[n];
    va_list counter;
    va_start(counter, n);

    for (int i = 0; i < n; i++) {

        arr[i] = va_arg(counter, int); 
        
    }

    result = arr[0];

    for (int i = 1; i < n; i++) {

        result -= arr[i];
    
    }

    return result;
}

int main(){

    char operation = ' ', test = ' ';
    while (1) {
        int flag = 0;
        
        printf("Insert an operation: ");
        scanf("%c", operation);
        if (scanf("%c", test) != 1) {
            while (scanf("%c", test) != 1 || test == '\n') {
                scanf("%c", test);
            }
            flag = 1;
        }  
        if (flag == 1) {
            printf("Incorrect input\n");
            continue;
        }

        switch (operation) {

            case '+': 

                continue;

            case '-':

                printf("Result is %d", sub(4, 200, 100, 50, 60, 40));
                break;

            case '*':

                continue;

            case '/':

                continue;

            case '=':
            
                return 0;

            default:

            printf("Incorrect input");
            continue;

        }
    }

}






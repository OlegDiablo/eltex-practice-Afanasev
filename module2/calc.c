#include <stdio.h>
#include <stdarg.h>

int error = 0;

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

    for (int i = 1; i < n; i++) {
        
        if (result > arr[i] && arr[i] != 0) {

            result /= arr[i];

        } 
        else {

            error = 1;
            return result;

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
        
        printf("Insert an operation: ");
        scanf("%c", &operation);
        scanf("%c", &test);
        if ( test != '\n') {

            while ((test = getchar()) != '\n' && test != EOF);
            printf("Incorrect input\n");
            continue;
        }  

        switch (operation) {

            case '+': 

                printf("Result is %d\n", add(3, 200, 100, 50, 60, 40));
                printf("Result is %d\n", add(4, 200, 100, 50, 60, 40));
                printf("Result is %d\n", add(5, 200, 100, 50, 60, 40));
                break;

            case '-':

                printf("Result is %d\n", sub(3, 200, 100, 50, 60, 40));
                printf("Result is %d\n", sub(4, 200, 100, 50, 60, 40));
                printf("Result is %d\n", sub(5, 200, 100, 50, 60, 40));
                break;

            case '*':

                printf("Result is %d\n", mul(3, 2, 10, 5, 6, 4));
                printf("Result is %d\n", mul(4, 2, 10, 5, 6, 4));
                printf("Result is %d\n", mul(5, 2, 10, 5, 6, 4));
                break;

            case '/':

                int res = div(4, 200, 0, 40, 50);
                if (error == 1) {
                    
                    printf("Incorrect input\n");
                    error = 0;

                } 
                else {

                    printf("Result is %d\n", res);
                    error = 0;
                    
                }
                break;

            case '=':
            
                return 0;

            default:

            printf("Incorrect input\n");
            continue;

        }
    }

}






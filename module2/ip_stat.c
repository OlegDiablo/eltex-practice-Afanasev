#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>

void printBinary(uint32_t num) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i % 8 == 0 && i != 0) 
            printf(" ");
    }
    printf("\n");
}


uint32_t binaryIp(int octets[4]) {
    return (uint32_t)(octets[0]) << 24 | (uint32_t)(octets[1]) << 16 | (uint32_t)(octets[2]) << 8 | (uint32_t)(octets[3]);
}


uint32_t binaryMask(int mask) {

    uint32_t binary_mask = 0;
    uint32_t tmp = 1;
    int shift = 32 - mask;
    for(int i = 31; i >= shift; i--) {

        tmp = tmp << i;
        binary_mask = binary_mask | tmp;
        tmp = 1;

    }

    return binary_mask; 
}


uint32_t defNet(uint32_t ip, uint32_t mask) {
    return ip & mask;
}

float statistics(uint32_t subnet, uint32_t mask, int amount) {

    int res = 0;
    for(int i = 0; i < amount; i++) {

        int ip[4] = {rand()%256, rand()%256, rand()%256, rand()%256};
        uint32_t binary_ip = binaryIp(ip);
        if((binary_ip & mask) == subnet) {
            res++;
        }

    }

    return (float)res/(float)amount*100;
}


int inputParsing(char* input, int counter, int* octets, int* mask, int* amount){

    int i = 0, j = 0, k = 0;;
    char* octet = calloc(1, sizeof(char));

    while(input[i] != '/') {

        if(input[i] != '.' && isdigit(input[i])) {
            octet = realloc(octet, sizeof(char)*(j+1));
            octet[j] = input[i];
            i++;
            j++;

        }
        else if(input[i] == '.') {
            j = 0;
            i++;
            octets[k] = atoi(octet);
            free(octet);
            octet = calloc(1, sizeof(char));
            k++;
        }
        else {
            return -1;
        }

    }

    octets[k] = atoi(octet);
    free(octet);

    char* mask_char = calloc(1, sizeof(char));
    i++;
    j = 0;

    while(input[i] != ' ') {
        
        mask_char = realloc(mask_char, sizeof(char)*(j+1));
        mask_char[j] = input[i];
        j++;
        i++; 

    }

    *mask = atoi(mask_char);
    free(mask_char);

    counter -= i;
    char* amount_char = calloc(1, sizeof(char));

    for(j = 0; j < counter; j++) {

        amount_char = realloc(amount_char, sizeof(char)*(j+1));
        amount_char[j] = input[i];
        i++;

    }

    *amount = atoi(amount_char);
    free(amount_char);

    return 0;
}


int main(){

    srand(time(NULL));

    while(1) {

        char* input = calloc(1, sizeof(char));
        char symbol;
        int counter = 0;
        printf("Input command: ");

        while(scanf("%c", &symbol) == 1 && symbol != '\n'){

            input = realloc(input, sizeof(char) * (counter+1));
            input[counter] = symbol;
            counter++;

        }

        int octets[4];
        int mask, amount;
        int err = inputParsing(input, counter, octets, &mask, &amount);
        if(err == -1) {
            printf("Incorrect IP\n");
            continue;
        }

        if(mask > 32 && mask != 0) {
            printf("Incorrect mask\n");
            continue;
        }

        printf("mask=%d\n", mask);
        printf("amount=%d\n", amount);

        uint32_t test_ip = binaryIp(octets);

        uint32_t binary_mask = binaryMask(mask);

        uint32_t subnet = defNet(test_ip, binary_mask); 
        printf("Result: %.1f\n", statistics(subnet, binary_mask, amount)); 
    
    }

    return 0;
}
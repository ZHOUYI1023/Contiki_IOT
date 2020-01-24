#include<stdio.h>
#include"print_format.h"

void print_float(char *str, float n){
	
    uint16_t d1 = (uint16_t) n;
    uint16_t d2 = (n - d1) * 1000;
	if (str== NULL)
		printf(" %u.%02u ", d1, d2);
	else{
		printf("%s", str);
    	printf(" %u.%02u ", d1, d2);
		printf("\n");
	}
}


void print_array(char* str, float *data, uint8_t size){
	int i = 0;
	printf("%s(", str);	
    for(i = 0 ; i <size; i++)
        print_float(NULL, data[i]);
    printf(")\n");
}

void print_char_array(char* str, char *data, uint8_t size){
	int i = 0;
	printf("%s( ", str);	
    for(i = 0 ; i <size; i++)
        printf("%c ", data[i]);
    printf(")\n");


}

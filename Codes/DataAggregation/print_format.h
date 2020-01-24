/*
 * Author: Yi Zhou <zhouyi1023@tju.edu.cn> 
 * Date: 2019/11
 */

#ifndef PRINT_FORMAT_H
#define PRINT_FORMAT_H

/*print a float number*/
void print_float(char *str, float n);

/*print a array*/
void print_array(char* str, float *data, uint8_t size);

/*print a char array with space between each char*/
void print_char_array(char* str, char *data, uint8_t size);

#endif

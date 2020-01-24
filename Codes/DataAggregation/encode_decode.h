/*
 * Author: Yi Zhou <zhouyi1023@tju.edu.cn> 
 * Date: 2019/11
 */

#ifndef ENCODE_DECODE_H
#define ENCODE_DECODE_H

#include <stdio.h>
#include "main.h"

/*generate binary code*/
int generate_code(float *data, uint8_t size, uint16_t *code);

/*decode the data*/
void decode(float *data, uint8_t size, float *data_decoded, uint16_t code);

/*encode the data*/
void encode(float *data, uint8_t size, float *data_encode, uint16_t code);

#endif

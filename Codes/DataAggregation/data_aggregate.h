/*
 * Author: Yi Zhou <zhouyi1023@tju.edu.cn> 
 * Date: 2019/11
 */

#ifndef DATA_AGGREGATE_H
#define DATA_AGGREGATE_H

#include<stdio.h>
#include "main.h"

/*calculate the mean and standard deviation*/
void variance(float *data, uint8_t size, struct distribution *dist);

/*calculate the bin size*/
uint8_t get_bin_size(struct distribution *dist);

/*aggregate the data*/
void aggregation(float *data, uint8_t data_size, uint8_t bin_size, float* data_new);

#endif

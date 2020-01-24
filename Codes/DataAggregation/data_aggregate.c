#include <math.h>
#include "data_aggregate.h"


void variance(float *data, uint8_t size, struct distribution *dist){
	uint8_t i = 0;
    for(i = 0; i < size; i++){
        dist->mean += data[i];
    }
    dist->mean /= size;//mean

    for(i = 0; i < size; i++){
        dist->std += (data[i] - dist->mean) * (data[i] - dist->mean) ;
    }
    dist->std  = sqrtf(dist->std / size);//standard deviation
}

uint8_t get_bin_size(struct distribution *dist){
    // calculate bin size
	uint8_t bin_size;
    if(dist->std < THRESHOLD_LOW)
        bin_size = 4;
    else if(dist->std < THRESHOLD_HIGH)
        bin_size = 2;
    else
        bin_size = 1;
	return bin_size;
}

void aggregation(float *data, uint8_t data_size, uint8_t bin_size, float* data_new) {
    float sum_temp = 0;
	uint8_t i = 0, j = 0;
    for(i = 0; i < data_size; i += bin_size) {
        sum_temp = 0;
        for (j = i; j < i + bin_size; j++) {
            sum_temp += data[j];
        }
        data_new[i / bin_size] = sum_temp / bin_size;// is two u8 ok here?
    }
}


#include <stdio.h>
#include <math.h>
#define  THRESHOLD_LOW 4
#define  THRESHOLD_HIGH 6

float std(float *data, int size){
    float sum = 0, mean = 0;
    float stdValue = 0;
    for(int i = 0; i < size; i++){
        sum += data[i];
    }
    mean = sum/size;

    for(int i = 0; i < size; i++){
        stdValue += (data[i] - mean) * (data[i] - mean) ;
    }
    stdValue = sqrt(stdValue / size);
    return stdValue;
}


void aggregation(float *data, int data_size, int bin_size, float* data_new) {
    float sum_temp = 0;
    for(int i = 0; i < data_size; i += bin_size) {
        sum_temp = 0;
        for (int j = i; j < i + bin_size; j++) {
            sum_temp += data[j];
        }
        data_new[i / bin_size] = sum_temp / bin_size;
    }
}

int main() {
    float buffer[] = {1,2,3,4,5,6,7,8,9,10,11,12};
    //TODO: wait process event buffer full
    int bin_size = 0;
    float stdValue = 0;
    // calculate standard value
    stdValue = std(buffer, sizeof(buffer)/ sizeof(buffer[0]));
    // calculate bin size
    if(stdValue < THRESHOLD_LOW)
        bin_size = 4;
    else if(stdValue < THRESHOLD_HIGH)
        bin_size = 2;
    else
        bin_size = 1;
    float buffer_new[sizeof(buffer)/ sizeof(buffer[0])/bin_size];
    // aggregation
    aggregation(buffer, sizeof(buffer)/ sizeof(buffer[0]), bin_size, buffer_new);
    // format printing
    int i = 0;
    printf("B = (");
    for( i = 0 ; i < sizeof(buffer)/ sizeof(buffer[0]) - 1; i++)
        printf("%.1f, ", buffer[i]);
    printf("%.1f)\n", buffer[i]);
    printf("StdDev = %.2f\n", stdValue);
    printf("Number of values to aggregate = %d\n", bin_size);
    printf("X = (");
    for( i = 0 ; i < sizeof(buffer)/ sizeof(buffer[0])/bin_size-1; i++)
        printf("%.1f, ", buffer_new[i]);
    printf("%.1f)\n", buffer_new[i]);
    //TODO: Hoffman Coding
    return 0;
}
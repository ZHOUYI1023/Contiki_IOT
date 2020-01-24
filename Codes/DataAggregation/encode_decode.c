#include <math.h>
#include <encode_decode.h>


#define getbit(x, y) ((x) >> (y)&1)
#define reversebit(x, y) x^=(1<<y)


int generate_code(float *data, uint8_t size, uint16_t *code) {
    float diff1;
    uint8_t i = 0;
    uint8_t size_reduced = size;
    for (i = 1; i < size - 1; i++) {
        diff1 = data[i] - data[i-1];
		//diff2 = data[i+1] - data[i];
        if (abs(diff1) <= DIFF_THRESHOLD){
            reversebit(*code, i);// set 0 if the same data
            size_reduced -= 1;// size minus 1
        }
    }
    return size_reduced;
}


void decode(float *data, uint8_t size, float *data_decoded, uint16_t code) {
    uint8_t ind = 0, i = 0;
    for (i = 0; i < size; i++) {
        if(getbit(code, i) == 1){
            data_decoded[i] = data[ind];
            ind += 1;
        }
        else{
            data_decoded[i] = data_decoded[i-1];//same as the previous data
        }
    }
}


void encode(float *data, uint8_t size, float *data_encode, uint16_t code){
    uint8_t ind = 0, i = 0;
    for (i = 0; i < size; i++){
        if(getbit(code, i) == 1){
            data_encode[ind] = data[i];
            ind += 1;
        }
    }
}

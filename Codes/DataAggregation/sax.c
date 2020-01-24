#include <math.h>
#include "sax.h"
#include "sax_code.h"

static const char* foo = "abcdefghik";


void sax(float *data, uint8_t size, struct distribution *dist, char *sax_data){
	int i = 0, j =0;
	float data_normal;
	// normalization
    for (i = 0; i < size; i++){
        data_normal = (data[i] - dist->mean)/dist->std;
		for (j = 0; j < ALPHABET_SIZE-1; j++){
			if (saxcode[ALPHABET_SIZE-1][j+sizeof(saxcode[0])/sizeof(saxcode[0][0])-ALPHABET_SIZE+1] - data_normal >= 0)
				break;
		}
		sax_data[i] = foo[j];
    }
}

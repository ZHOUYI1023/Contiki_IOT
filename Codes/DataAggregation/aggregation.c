#include "contiki.h"
#include <stdio.h>
#include "etimer.h"
#include "dev/light-sensor.h"
#include <stdio.h>
#include "main.h"
#include "print_format.h"
#include "data_aggregate.h"
#include "sax.h"
#include "encode_decode.h"

PROCESS(sensor_reading_process, "Sensor Reading Process");
AUTOSTART_PROCESSES(&sensor_reading_process);

int manipulate(float *buffer, int data_size) {
    uint8_t bin_size = 0;
	struct distribution dist = {0, 0};
    // calculate standard value
    variance(buffer, data_size, &dist);
    // calculate bin size
  	bin_size = get_bin_size(&dist);
    float buffer_new[data_size/bin_size];
    // aggregation
    aggregation(buffer, data_size, bin_size, buffer_new);
	// SAX
	char sax_data[data_size];
	sax(buffer, data_size, &dist, sax_data);
	// encoding and decoding
	uint16_t code = 0b111111111111;
    uint8_t size_reduced;
    size_reduced = generate_code(buffer, data_size, &code);
    float data_encoded[size_reduced];
    float data_decoded[data_size];
    encode(buffer, data_size, data_encoded, code);
    decode(data_encoded, data_size,data_decoded,code);
    // format printing
	print_array("B = ", buffer, data_size);
	print_float("StdDev = ", dist.std);
    printf("Number of values to aggregate = %d\n", bin_size);
	print_array("X = ", buffer_new, (uint8_t)data_size/bin_size);
	
	printf("Reducde size after encoding is = %d\n", size_reduced);
	print_array("Encoded Data = ", data_encoded, size_reduced);
	print_array("Decoded Data = ", data_decoded, data_size);
	
	printf("Alphabet size for SAX is %d\n", ALPHABET_SIZE);
	print_char_array("SAX representation = ", sax_data, data_size);
    return 0;
	
}

PROCESS_THREAD(sensor_reading_process, ev, data){
	static struct etimer timer;
	static uint8_t ind;
	float light_raw;
	static float buffer[BUFFER_SIZE];
	PROCESS_BEGIN();
	SENSORS_ACTIVATE(light_sensor);
	etimer_set(&timer, CLOCK_CONF_SECOND);
	while(1){
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
		light_raw = light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC);
		buffer[ind] = 0.625 * 1e6 * 1000 * 1.5 *  light_raw / 4096 / 100000;
		ind += 1;
		if(ind == 12){
			ind = 0;		
			manipulate(buffer, sizeof(buffer)/ sizeof(buffer[0]));
		}
		etimer_reset(&timer);
	}
	PROCESS_END();
}

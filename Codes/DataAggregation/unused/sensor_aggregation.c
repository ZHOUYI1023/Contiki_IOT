#include "contiki.h"
#include <stdio.h>
#include "etimer.h"
#include "dev/light-sensor.h"
#include "print_float.h"

#define  THRESHOLD_LOW 2
#define  THRESHOLD_HIGH 10
#define  BUFFER_SIZE 12


PROCESS(sensor_reading_process, "Sensor Reading Process");

struct buffer{
	float *p;
	int size;
	int index;
}


float variance(struct buffer *data){
    float sum = 0, mean = 0, var =0;
	float * num;
	num = data->p;
	data -> index = 0;
    while (data->index < data->size){
		print_float(num[data->index]);
     	sum += num[data->index];
  	}
    mean = sum / data->size;
	
	data -> index = 0;
    while (data->index < data->size){
		print_float(num[data->index]);
     	var += (num[data->index] - mean) * (num[data->index] - mean) ;
  	}
    var = var / data->size;
    return var;
}

/*
void aggregation(list_t *data, int bin) {
    float sum_temp = 0;
    for(int i = 0; i < data_size; i += bin_size) {
        sum_temp = 0;
        for (int j = i; j < i + bin_size; j++) {
            sum_temp += data[j];
        }
        data_new[i / bin_size] = sum_temp / bin_size;
    }
}

*/

AUTOSTART_PROCESSES(&sensor_reading_process);

PROCESS_THREAD(sensor_reading_process, ev, data){
	static struct etimer timer;
	static struct buffer sensor_buffer;
	static float light_read[BUFFER_SIZE];
	sensor_buffer = {&light_read, BUFFER_SIZE, 0};                                                                                                                                                                                                                                                   
	float light_raw;
	int bin_size;	
	float var;
	PROCESS_BEGIN();
	SENSORS_ACTIVATE(light_sensor);
	etimer_set(&timer, CLOCK_CONF_SECOND);
	while(1){
		sensor_buffer.index = 0;
		while (sensor_buffer.index < sensor_buffer.size){
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
			light_raw = light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC);
			light_read[sensor_buffer.index] = 0.625 * 1e6 * 1000 * 1.5 *  light_raw / 4096 / 100000;
			sensor_buffer.index++;
		}
		var = variance(&sensor_buffer);
	
			
		/*
		if (list_length(buffer_list) == 12){
			printf("buffer full\n");
			var = variance(&buffer_list);
			print_float(var);
			if(var < THRESHOLD_LOW)  bin_size = 4;
    		else if(var < THRESHOLD_HIGH)  bin_size = 2;
    		else  bin_size = 1;
			//aggregate(*buffer_list);
		}
*/
		
		etimer_reset(&timer);
	}
	PROCESS_END();
}


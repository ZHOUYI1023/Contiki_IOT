#include "contiki.h"
#include <stdio.h>
#include "etimer.h"
#include "dev/light-sensor.h"
#include "print_float.h"
#include "lib/list.h"

#define  THRESHOLD_LOW 2
#define  THRESHOLD_HIGH 10

PROCESS(sensor_reading_process, "Sensor Reading Process");


struct buffer {
  struct buffer *next;
  int number;
};

LIST(buffer_list);

float variance(list_t *data){
	struct buffer *s;
    float sum = 0, mean = 0, var =0;
    for(s = list_head(*data);s != NULL;s = list_item_next(s)) {
		print_float(s->number);
     	sum += s->number;
  	}
    mean = sum / list_length(*data);

	for(s = list_head(*data);s != NULL;s = list_item_next(s)) {
     	var += (s->number - mean) * (s->number - mean) ;
  	}
	
    var = var / list_length(*data);
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
	float light_read;
	float light_raw;
	int bin_size;	
	float var;
	PROCESS_BEGIN();
	list_init(buffer_list);
	SENSORS_ACTIVATE(light_sensor);
	etimer_set(&timer, CLOCK_CONF_SECOND);
	while(1){
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
		light_raw = light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC);
		light_raw = 0.625 * 1e6 * 1000 * 1.5 *  light_raw / 4096 / 100000;
		list_add(buffer_list, light_raw);
		printf("%d\n",list_length(buffer_list));
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

















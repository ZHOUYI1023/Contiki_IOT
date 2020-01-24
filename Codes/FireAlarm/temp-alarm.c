#include "contiki.h"
#include <stdio.h>
#include "etimer.h"
#include "dev/leds.h"
#include "dev/sht11-sensor.h"
#include "dev/light-sensor.h"
#include "dev/battery-sensor.h"
#include "dev/button-sensor.c"
#include "powertrace.h"
#include "net/rime.h"
#include "rimeaddr.h"
#include "print_float.h"

#define TEMP_DEVIATION_THRESHOLD 1
#define LIGHT_DEVIATION_THRESHOLD 50
#define CALIBRATION_COUNTS 10
#define CALIBRATION_TIME_INTERVAL CLOCK_CONF_SECOND
#define DETECTION_TIME_INTERVAL (10 * CLOCK_CONF_SECOND)
#define NOTIFICATION_TIME_INTERAVL (CLOCK_CONF_SECOND / 4)
#define POWER_TRACE 1
#define POWER_TRACE_TIME_INTERVAL (60 * CLOCK_CONF_SECOND)
#define BROADCAST_PORT 1234
#define SEND_INTERVAL (20 * CLOCK_SECOND)


PROCESS(fire_detection_process,
"Fire detection process");
PROCESS(notification_process,
"Notification process");
PROCESS(sensor_calibration_process,
"Sensor calibrarion process");
PROCESS(recalibration_process,
"Recalibration_process");

AUTOSTART_PROCESSES(&sensor_calibration_process,
        &fire_detection_process, &notification_process);

struct sensor_read {
    float temp_read;
    float light_read;
};


static process_event_t fire_detected;
static process_event_t calibration_ready;
static struct sensor_read msg;
static struct sensor_read msg_calib;

/*unicast receive function for testing*/
/*
static void recv(struct unicast_conn *c, const rimeaddr_t *from)
{
	struct sensor_read *p;
	p = (struct sensor_read *)packetbuf_dataptr();
	printf("temp received from node = %u.%u\n", from->u8[0], from->u8[1]);
	print_float(p->temp_read);
	print_float(p->light_read);
}
*/

/* unicast callback*/
static const struct unicast_callbacks unicast_callbacks = {};
static struct unicast_conn uc;

/*convertting raw sensor values into values with units*/
void convert_msg(struct sensor_read *msg, float temp_raw, float light_raw) {
    msg->temp_read = -39.6 + 0.01 * temp_raw;
    msg->light_read = 0.625 * 1e6 * 1000 * 1.5 * light_raw / 4096 / 100000;
}

/*sensor calibrtion process*/
PROCESS_THREAD(sensor_calibration_process, ev, data){
    static struct etimer timer1;

    PROCESS_BEGIN();

    SENSORS_ACTIVATE(sht11_sensor);
    SENSORS_ACTIVATE(light_sensor);
    static float temp_sum = 0;
    static float light_sum = 0;
    static int i = 0;
    process_exit(&recalibration_process);

    leds_init();

    printf("Calibrating...\n");
    etimer_set(&timer1, CALIBRATION_TIME_INTERVAL);
    while(1){
        PROCESS_WAIT_EVENT_UNTIL (etimer_expired(&timer1));
        convert_msg(&msg, sht11_sensor.value(SHT11_SENSOR_TEMP),
                light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC));
        temp_sum += msg.
        temp_read;
        light_sum += msg.
        light_read;
        i++;
        printf("%d\n",i);
        leds_toggle(LEDS_GREEN);
        if(i == CALIBRATION_COUNTS){
            i++;
            break;
        }
        etimer_reset(&timer1);
    }
    msg_calib.temp_read = temp_sum / i;
    msg_calib.light_read = light_sum / i;
    i = 0;
    temp_sum = 0;
    light_sum = 0;
    printf("Calibration Done.\n");
    printf("Environment Temperature:");
    print_float(msg_calib.temp_read);
    printf("Environment Light:");
    print_float(msg_calib.light_read);
    process_post(&fire_detection_process, calibration_ready, &msg_calib);
    process_start(&recalibration_process, NULL);

    PROCESS_END();

}

/*fire detection process*/
PROCESS_THREAD(fire_detection_process, ev, data){
    static struct etimer timer2;

    PROCESS_BEGIN();

    if(POWER_TRACE) powertrace_start(100 * CLOCK_CONF_SECOND);
    while(1){
        PROCESS_YIELD_UNTIL(ev== calibration_ready);
        leds_off(LEDS_ALL);
        leds_on(LEDS_BLUE);
        etimer_set(&timer2,DETECTION_TIME_INTERVAL);
        while(1){
            PROCESS_WAIT_EVENT_UNTIL (etimer_expired(&timer2));
            convert_msg(&msg, sht11_sensor.value(SHT11_SENSOR_TEMP),
                    light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC));
            if((msg.temp_read > msg_calib.temp_read + TEMP_DEVIATION_THRESHOLD) &
            (msg.light_read > msg_calib.light_read + LIGHT_DEVIATION_THRESHOLD)){
                printf("Fire Detected!\n");
                process_post(&notification_process, fire_detected, &msg);

                powertrace_stop();

                PROCESS_EXIT();
            }
        etimer_reset(&timer2);
        }
    }

PROCESS_END();

}

/*notification process*/
PROCESS_THREAD(notification_process, ev, data){
    static struct etimer timer3;
    rimeaddr_t addr;

    PROCESS_BEGIN();

    unicast_open(&uc, 140, &unicast_callbacks);
    while(1){
        PROCESS_YIELD_UNTIL(ev== fire_detected);
        leds_off(LEDS_ALL);
        etimer_set(&timer3, NOTIFICATION_TIME_INTERAVL);
        while(1){
            PROCESS_WAIT_EVENT_UNTIL (etimer_expired(&timer3));

            packetbuf_copyfrom(&msg, sizeof(struct sensor_read));
            addr.u8[0] = 123;
            addr.u8[1] = 123;
            //printf("my address %u.%u\n", rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);//191.114
            if(!rimeaddr_cmp(&addr, &rimeaddr_node_addr)){
                printf("sending to sink\n");
                unicast_send(&uc, &addr);
            }

            print_float(msg.temp_read);
            print_float(msg.light_read);
            convert_msg(&msg, sht11_sensor.value(SHT11_SENSOR_TEMP),
                    light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC));
            printf("Alarm!\n");
            leds_toggle(LEDS_RED);
            etimer_reset(&timer3);
        }
    }
    PROCESS_END();

}


// re-calibration when detecting button isr
PROCESS_THREAD(recalibration_process, ev, data){
    PROCESS_BEGIN();

    SENSORS_ACTIVATE(button_sensor);
    while(1){
        PROCESS_WAIT_EVENT();

        if(ev == sensors_event){
            if(data == &button_sensor){
                process_exit(&sensor_calibration_process);
                process_exit(&fire_detection_process);
                process_exit(&notification_process);
                process_start(&sensor_calibration_process, NULL);
                process_start(&fire_detection_process, NULL);
                process_start(&notification_process, NULL);
            }
        }
    }

    PROCESS_END();

}


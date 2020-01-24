#include "lib/list.h"
#include "contiki.h"
#include <stdio.h>

struct example_list_struct {
  struct example_list_struct *next;
  int number;
};

LIST(example_list);

static struct example_list_struct element1, element2;

void
example_function(void)
{
  struct example_list_struct *s;
int a = 10;
list_init(example_list);
  element1.number = 1;
  list_add(example_list, &element1);
	list_add(example_list, a);
  element2.number = 2;
  list_add(example_list, &element2);
  printf("%d\n",list_length(example_list));
  for(s = list_head(example_list);
      s != NULL;
      s = list_item_next(s)) {
    printf("List element number %d\n", s->number);
  }
}

PROCESS(sensor_reading_process, "Sensor Reading Process");

AUTOSTART_PROCESSES(&sensor_reading_process);

PROCESS_THREAD(sensor_reading_process, ev, data){
PROCESS_BEGIN();
while(1){
example_function();
}
PROCESS_END();
}

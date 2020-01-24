CONTIKI_PROJECT = aggregation#test_list#
all: $(CONTIKI_PROJECT)
PROJECT_SOURCEFILES += print_format.c 
PROJECT_SOURCEFILES += data_aggregate.c
PROJECT_SOURCEFILES += sax.c
PROJECT_SOURCEFILES += encode_decode.c
LDFLAGS+=-lm
CFLAGS+=-lm
TARGET_LIBFILES+=-lm

#UIP_CONF_IPV6=1

CONTIKI = ../..
include $(CONTIKI)/Makefile.include

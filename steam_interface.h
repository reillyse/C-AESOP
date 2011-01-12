
#include "rte/rte.h"
#include "rte/rte-publish.h"
#include "rte/rte-subscribe.h"
#include "memory/rte-mem.h"

#include <time.h>
#include <sys/time.h>
#include <stdio.h>

#define LOCATION_X 100
#define LOCATION_Y 100



proximity *my_proximity; 
latency lat;
period per;

subject my_event_subject;
coordinate my_location;
int i;

FILE *send_logger, *recv_logger;

struct timeval tv;

void my_adapt(proximity*);
void receive_my_event(event*);
int setup_steam();
int setup_event(char*,prox_type,event*);
int subscribe_to_event(char* , event*,void (*)(event*));
void send_my_event(char*,channel_id,event*);

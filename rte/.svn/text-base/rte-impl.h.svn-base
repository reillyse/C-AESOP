#ifndef _rte_impl_h_
#define _rte_impl_h_

#include <stdlib.h>

#include "rte.h"
#include "rte-publish.h"
#include "rte-subscribe.h"
#include "proximity.h"
//#include "rte-marshal.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "pthread.h"

#include <unistd.h>
#include "list.h"

typedef struct{
  channel_id channel; 
  latency lat;
  period per;
  subject sub;
  proximity *desired;
  proximity *actual;  
} event_channel;

/*
 * list to hold callbacks for all subscription
 */
typedef struct {
  int id;
  receive_event cb;
  struct list_head list;
  event_channel* sear_channel;
} channel_table;


typedef union {
  struct{
    subject sub;
    content cont;
    prox_type proximity_type;
  } event;
  struct{
    subject sub;
    content cont;
    prox_type proximity_type;
    proximity* p;
  } hull_event;
  struct{
    subject sub;
    content cont;
    prox_type proximity_type;
    proximity* p;
  } circle_event;
  
} steam_event;

void make_callbacks_for(steam_event*);

char* marshal_coordinate(coordinate);
coordinate unmarshal_coordinate(char*);

char* marshal_hull_proximity(proximity*);
char* marshal_circle_proximity(proximity*);

proximity* unmarshal_hull_proximity(char*);
proximity* unmarshal_circle_proximity(char*);

void send_local_event(steam_event*);

void free_steam_event(steam_event*);

#endif

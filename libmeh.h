#pragma once
#ifndef H_LIB_IF
#define H_LIB_IF
#include "generic_stream.h"
#include <sys/queue.h>
#include <pthread.h>
#include <sys/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "acceleration.h"
#include "swing.h"
#include "ball_contact.h"
#include "stance.h"
#include "load_sensor.h"
#include "emergency.h"

typedef struct {
  int x;
  int y;
} point;

 typedef struct {
   double a;
   double b;
   double c;
 } vector;

#define LARGEST_DATA_TYPE swing
//STREAM(float)
//STREAM(int)
//STREAM(double)

//STREAM(point)
STREAM(accel_vector)
//STREAM(vector)
//STREAM(stance)
//STREAM(load_sensor)
//STREAM(ball_contact)
//STREAM(quaternion)
STREAM(emergency)
LIST_HEAD(listhead_funct,if_list_entry) my_list_head;


//LIST_HEAD(if_listhead, if_list_entry);




 struct if_list_entry  {
  LIST_ENTRY(entry) entries;          /* List. */
   multi_event_handler *ifunc;
} ;

int do_meh_handler(multi_event_handler*);
int register_multi_event_handler(struct listhead_funct*,multi_event_handler*);
struct listhead_funct* init_if_func();
int start_multi_event_handler(struct listhead_funct*);
multi_event_handler* create_multi_event_handler(int,stream*[],stream*,void (*)(),int (*execution_policy)());
void start_meh(multi_event_handler* meh);
#endif


#include <stdlib.h>
#include "rte-mem.h"

latency* alloc_latency(){
  return (latency*)calloc(1, sizeof(latency));
}

proximity_convex_hull alloc_proximity_convex_hull(int num){
  return (proximity_convex_hull)calloc(num, sizeof(coordinate));
}

// aline - have alloc_proximity_cirlce instead of alloc_proximity
proximity* alloc_proximity(){
  return (proximity*)calloc(1, sizeof(proximity));
}

period* alloc_period(){
  return (period*)calloc(1, sizeof(period));
}

steam_event* alloc_steam_event(){
  return (steam_event*)calloc(1, sizeof(steam_event));
}

event* alloc_event(){
  return (event*)calloc(1, sizeof(event));
}

content alloc_content(int size){
  return (content)calloc(size, sizeof(content));
}

subject alloc_subject(int size){
  return (content)calloc(size, sizeof(subject));
}

char* alloc_buffer(int sz){
  return (char*)calloc(sz, sizeof(char));
}

channel_table* alloc_channel_table(){
  return (channel_table*)calloc(1, sizeof(channel_table));
}

char* alloc_proximity_type(){
  return (char*)calloc(1, PROXIMITY_TYPE_LEN);
}

event_channel* alloc_event_channel(){
  return (event_channel*)calloc(1, sizeof(event_channel));
}

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _rte_mem_h_
#define _rte_mem_h_

#include "rte-impl.h"
#include "dummy-sear-impl.h"

#define PROXIMITY_TYPE_LEN 10

subject alloc_subject(int size);
content alloc_content(int size);

// questionalble
period* alloc_period();
latency* alloc_latency();
steam_event* alloc_steam_event();
event* alloc_event();
char* alloc_proximity_type();

// proximity allocs
coordinate* alloc_coordinate();
proximity* alloc_proximity();

proximity_convex_hull alloc_proximity_convex_hull(int);

// channel allocs
event_channel* alloc_event_channel();

char* alloc_buffer(int);

channel_table* alloc_channel_table();
// free
//void free(void*);

#endif

#ifdef __cplusplus
} // extern C
#endif

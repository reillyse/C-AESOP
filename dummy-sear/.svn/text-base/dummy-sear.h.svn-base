#ifndef _dummy_sear_h_
#define _dummy_sear_h_

#include "rte-impl.h"
#include "rte-publish.h"
#include "rte-subscribe.h"

#include <stdio.h>


int init_dummy_sear_publish();
int init_dummy_sear_subscribe();

// all memory locations passed into the reserve call is owned by
// dummy_sear now and will be freed on the free call. User should not
// call free on these after calling reserve.
event_channel* reserve_channel(latency lat,
			       float requested_period,
			       proximity* requested_proximty,
			       adaptation adaptation_cb,
			       subject event_name);

event_channel* reserve_channel_for_subscribe(subject sub);

void free_channel(event_channel*);

// msg can be an event or an announcement
int send_on_channel(void* msg, event_channel* to);

int register_cb_for_recv_from_channel(receive_event cb, event_channel* from);

#endif

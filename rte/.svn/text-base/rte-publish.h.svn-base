#ifdef __cplusplus
extern "C" {
#endif

#ifndef _rte_publish_h_
#define _rte_publish_h_

#include "rte.h"
#include "proximity.h"

/*
 * Init the transport for sending events and announcements, returns
 * null if unable to get resources from SEAR or a broadcast socket.
 *
 * returns various < 0 ints for error status.  
 */
int init_rte_publish();

/*
 * These functions should be implemented and their pointers passed
 * with announcements. The respective function is called on a
 * space-elastice adaptation from SEAR.
 */
typedef void (*adaptation)(proximity*);

/*
 * announce an event, the proximity filter information is passed with
 * the announcement. This information is propagated in the network to
 * enable proximity filtering along the entire network.
 * 
 * The subject and proximity should be allocated in the shared memory
 * allocated by init function. Application allocates all data that has
 * to be passed to rte service by passing pointers into this shared
 * memory.
 *
 * latency - the max and min latencies requested by the publisher.
 *
 * period - the period of the event requested by the publisher.
 *
 * proximity passed in contains the desired coverage. On return it
 * contains the actual coverage that the lower layer were able to
 * guarantee delivery to(given the latency and the period). Future
 * adaptations are made known by invoking the adaptation callbacks
 * (circle and hull adaptation).
 *
 */
channel_id announce(subject, proximity*, latency, period, adaptation);

void unannounce(channel_id);

/*
 * returns -1 if actual < desired in the handle. The producer should
 * not be trying to send any events.
 */
int send_event(int, event*);

#endif

#ifdef __cplusplus
} // extern C
#endif

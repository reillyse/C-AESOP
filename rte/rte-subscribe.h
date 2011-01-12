#ifdef __cplusplus
extern "C" {
#endif

#ifndef _rte_subscribe_h_
#define _rte_subscribe_h_

#include "rte.h"

/*
 * Init the transport for receiving events, returns null if unable to
 * get resources from SEAR or a broadcast socket.
 *
 * returns various < 0 ints for error status.  
 */
int init_rte_subscribe();

/*
 * call backs from event reception.  An implementation whos address is
 * passed to subscribe should spawn a thread to handle the receive
 * event.
 */
typedef void (*receive_event)(event*);

/*
 * subscribe calls.
 * Content filtering handled by the application on a call back
 *
 * Do not delete/change subject after making this call; rte will not
 * copy the subject, just uses the string to locate callbacks.
 *
 */
subscription_id subscribe(subject, receive_event);

void unsubscribe(subscription_id);

#endif

#ifdef __cplusplus
}
#endif

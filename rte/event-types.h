#ifndef _event_types_h_
#define _event_types_h_

#include "proximity.h"

/*
 * subject used to identify event types. Example a subject can be
 * "weight". Thus application will be delivered all events with the
 * subject "weight".
 */ 
typedef char* subject;

/*
 * content contains all semantic and concrete information about the
 * event. Thus an event of subject "weight" from the top-left leg of a
 * couch will hold a struct describing the event. The receiving
 * application then typecasts the same into the required struct.
 *
 * The problem of providing the struct definition to applications is
 * will be considered under "event discovery". Such an event discovery
 * is not a part of the rte sub-system
 */ 
typedef char* content;

/*
 * Period captures the periodicity (in milliseconds) of an event.
 */
typedef float period;

/*
 * The latency struct is used to capture both latency bounds, and
 * figure the event class(soft, hard or non real time).
 *
 * min and max used to determine event class
 * min == max == 0 implies non-rt
 * min != 0 && max != 0  implies soft rt
 * min != 0 && max == 0  implies hard rt
 */
typedef struct{
  float min;
  float max;
} latency;

typedef struct {
  subject sub;
  content cont;
} event;

#endif

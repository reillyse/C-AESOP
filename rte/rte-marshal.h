#ifndef _rte_marshal_h_
#define _rte_marshal_h_

#include "event-types.h"
//#include <time.h>
#include "rte-impl.h"
#include "rte-mem.h"

#include <stdio.h>

/* parameters - int, double, float, string, proximities, coordinate, time */

#define DELIM "-"    //SR changed this cause  / is used in base64
#define PROX_DELIM "$"
#define COORD_DELIM ":"

char* marshal_steam_event(steam_event*);
steam_event* unmarshal_steam_event(char*);

//char* marshal_time(struct tm*);
//struct tm* unmarshal_time(char*);

char* marshal_proximity(steam_event*);
// aline - todo . implement the function below
//char* marshal_proximity(proximity*);

#endif

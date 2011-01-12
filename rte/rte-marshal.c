#include "../libmeh.h" //sean addition
#include "rte-marshal.h"

char* marshal_steam_event(steam_event* event){

  char *proximity_as_string, *data;

  //printf("marshal, subject - %s\n", event->event.sub);
  //printf("marshal, content - %s\n", event->event.cont);
  
  proximity_as_string = marshal_proximity(event);
  
  data = (char*)alloc_buffer(strlen(event->event.sub)
				   + strlen(event->event.cont)
				   + strlen(proximity_as_string)
				   + 3 * strlen(DELIM) + 1);
  
  strcat(data, event->event.sub);
  strcat(data, DELIM);
  strcat(data, event->event.cont);
  strcat(data, DELIM);
  strcat(data, proximity_as_string);
  strcat(data, DELIM);

  free(proximity_as_string);

  //printf("sizeof data = %d\n", strlen(data));

  //printf("Marshalled event - %s\n", data);
  return data;
}

/* /\* */
/*  * subject/content/hull/num_coords$lat1$lon1...$latn$lonn/ */
/*  *\/ */
steam_event* unmarshal_steam_event(char* event){

  char *subject, *content;
  steam_event* ev = (steam_event*)alloc_steam_event();
  char *saveptr;

 // printf("event - %s\n", (char*)event);
  //SR flushing the event so I know where it fudges up
  // fflush(stdout);
  ev->event.sub = (char*)strtok_r(event, DELIM, &saveptr);
  ev->event.cont = (char*)strtok_r(0, DELIM, &saveptr);
  ev->event.proximity_type = atoi(strtok_r(0, DELIM, &saveptr));

  //printf("SUBJECT --- %s\n", ev->event.sub);
  //   printf("UnMarshalling CONTENT --- %s\n", ev->event.cont);
  // fflush(stdout);
  //printf("PROX TYPE --- %d\n", ev->event.proximity_type);

  switch (ev->event.proximity_type) {
  case HULL:
    ev->hull_event.p = (proximity*)unmarshal_hull_proximity((char*)strtok_r(0, DELIM, &saveptr));
    break;
  case CIRCLE:
    ev->circle_event.p = (proximity*)unmarshal_circle_proximity((char*)strtok_r(0, DELIM, &saveptr));
    break;
  case LOCAL:
    ev->event.proximity_type = LOCAL;
  }
 
  //printf("unmarshalling ... done\n");
  return ev;
}

// should return "proximity_type/proximity_contents"
char* marshal_proximity(steam_event* ev){

  char *proximity_as_string;

  //printf("marshal_proximity - %d\n", ev->event.proximity_type);

  switch (ev->event.proximity_type) {
  case HULL: 
    proximity_as_string = marshal_hull_proximity(ev->hull_event.p);
    break;
  case CIRCLE:
    proximity_as_string = marshal_circle_proximity(ev->circle_event.p);
    break;
  case LOCAL:
    proximity_as_string = (char*)alloc_buffer((sizeof LOCAL + sizeof "/local"));
    sprintf(proximity_as_string, "%d", LOCAL);
    strcat(proximity_as_string,"/local");
  }

  //printf("From marshal_proximity --- %s\n", proximity_as_string);
  return proximity_as_string;
}

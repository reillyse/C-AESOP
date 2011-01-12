#include "proximity.h"
#include "rte-marshal.h"
#include "rte-mem.h"

char* marshal_coordinate(coordinate coord){
  char *temp, *ret;
  ret = (char*)alloc_buffer(2 * sizeof("xyz.ab") + sizeof(COORD_DELIM)); //ab.cd format
  temp = (char*)calloc(1, sizeof("xyz.ab")); 
  sprintf(ret, "%.2f", coord.lat);
  strcat(ret, COORD_DELIM);
  sprintf(temp, "%.2f", coord.lon);  
  strcat(ret, temp);
  free(temp);
  return ret;
}

coordinate unmarshal_coordinate(char* buf){
  char* temp;
  char *saveptr;
  coordinate ret;
  temp = (char*)strtok_r(buf, COORD_DELIM, &saveptr);
  ret.lat = atof(temp);
  temp = (char*)strtok_r(0, COORD_DELIM, &saveptr);
  ret.lon = atof(temp);
  return ret;
}


// returns HULL/num_cords$lat:lon$...$lat:lon (no / at end)
char* marshal_hull_proximity(proximity* hull){
  char *ret;
  char* coord;
  char temp[6];
  int i;
  ret = (char*)alloc_buffer(sizeof(prox_type) + sizeof(DELIM) + sizeof(int) +
			    hull->proximity_hull.num_coords * 2 * (sizeof("xyz.ab") + sizeof(PROX_DELIM)));
  sprintf(temp, "%d", hull->proximity_hull.type);
  strcat(ret, temp);
  strcat(ret, DELIM);
  sprintf(temp, "%d", hull->proximity_hull.num_coords);
  strcat(ret, temp);
  strcat(ret, PROX_DELIM);
  for(i = 0; i < hull->proximity_hull.num_coords; i++){
    coord = marshal_coordinate(hull->proximity_hull.hull[i]); 
    strcat(ret, coord);
    free(coord);
    strcat(ret, PROX_DELIM);
  }
  return ret;
}

// receives num_cords$lat:lon$...$lat:lon (no / at end)
proximity* unmarshal_hull_proximity(char* buf){
  proximity* ret;
  int num_coords, i;
  coordinate *next_coord;
  char* temp;
  char *saveptr;

  //printf("unmarshaling hull from %s\n", buf);

  temp = (char*)strtok_r(buf, PROX_DELIM, &saveptr);
  num_coords = atoi(temp);
  ret = (proximity*)alloc_proximity();

  ret->proximity_hull.hull = (proximity_convex_hull)alloc_proximity_convex_hull(num_coords);
  ret->proximity_hull.num_coords = num_coords;
  for(i = 0; i < num_coords; i++){
    temp = (char*)strtok_r(0, PROX_DELIM, &saveptr);
    ret->proximity_hull.hull[i]  = unmarshal_coordinate(temp);
  }
  return ret;
}



// returns CIRCLE/radius$lat:lon (no / at end)
char* marshal_circle_proximity(proximity* circle){
  char *temp, *ret;
  char* coord;
  int i;
  ret = (char*)alloc_buffer(sizeof(prox_type) + sizeof(DELIM) + sizeof("100000") +
			    2 * (sizeof("xyz.ab") + sizeof(PROX_DELIM)));
  temp = (char*)alloc_buffer(sizeof("100000")); 
  sprintf(temp, "%d", circle->proximity_circle.type);
  strcat(ret, temp);
  strcat(ret, DELIM);
  sprintf(temp, "%f", circle->proximity_circle.radius);
  strcat(ret, temp);
  strcat(ret, PROX_DELIM);
  coord = marshal_coordinate(circle->proximity_circle.center); 
  strcat(ret, coord);
  free(coord);
  return ret;
}

// returns radius$lat:lon (no / at end)
proximity* unmarshal_circle_proximity(char* buf){

  proximity* ret;
  char* temp;
  char *saveptr;

  temp = (char*)strtok_r(buf, PROX_DELIM, &saveptr);
  ret = (proximity*)alloc_proximity();
  ret->proximity_circle.radius = atoi(temp);
  temp = (char*)strtok_r(0, PROX_DELIM, &saveptr);
  ret->proximity_circle.center = unmarshal_coordinate(temp);
  return ret;
}

int is_inside(coordinate* coord, proximity* circle){
}

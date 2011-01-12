#ifndef _proximity_h_
#define _proximity_h_

#define CIRCLE 1
#define HULL 2
#define LOCAL 3

typedef int prox_type;

typedef struct{
  float lat;
  float lon;
} coordinate;

typedef coordinate* proximity_convex_hull;

typedef union{
  prox_type type;
  struct{
    prox_type type;
    int num_coords;
    proximity_convex_hull hull;
  } proximity_hull;
  struct{
    prox_type type;
    coordinate center;
    float radius;
  } proximity_circle;
} proximity;

int is_inside(coordinate*, proximity*);

#endif

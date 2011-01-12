#ifndef H_ORIENT
#define H_ORIENT
#define ORIENTATION_SUBJECT  "ORIENT"
typedef struct {
  float o1;
  float o2;
  float o3;
  float o4;
} orientation;

STREAM(orientation)
STREAM_FUNCTIONS(orientation)

#endif 

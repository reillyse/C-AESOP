#include "libmeh.h"
#include <unistd.h>
#include <pthread.h>
#include "steam_interface.h"
#include "libmeh.h"
#include "acceleration.h"
#include "orientation.h"
#include "quaternion.h"
#include "swing.h"
#include "vector.c"
#include <string.h>
 #define _REENTRANT

void parse_event_1(event *);
void parse_event_2(event *);
void parse_event_3(event *);
stream* inputs[2];
/*
  debugging function passed to helper method to allow us to print the stream of this type
 */
void print_double(double f){
   //  printf("before print \n"); fflush(stdout);
   printf("%f \n", f);
 }

/* 
  Implement a MultiEventHandler to determine when a swing has started 
  Outputs a swing event when it is detected
*/

void swing_start(orientation *orient ,accel_vector* accel, swing* result){
 
  float ideal_swing[] = {1.319f,-7.65f,-5.089f};
  float  angle =  angle_between(&ideal_swing,accel,3) *57.2957795;
  float mag_diff = magnitude(&ideal_swing,3) - magnitude(accel,3);

  if(mag_diff < 0) { mag_diff = mag_diff * -1;}
 // racket is oriented to make a swing and acceleration close to ideal swing
  if (angle < 60 &&   magnitude(accel,3) > 9 && rotation_around_Z(orient) >30 && rotation_around_Z(orient) < 150 ){   // check the orientation is ok
    strcpy(result -> message ,"START");
  }

}
//if the event is an accelerometer reading we proceed to multi-event handler
void execution_policy(){
return inputs[1]->status;
}

multi_event_handler* swing_start_meh;

int main(){
  //should be left with a few generic setup functions
  // and constructors for all the streams and the interval function constructor
  setup_steam();

 stream* output =  (stream*) create_swing_output_stream(SWING_SUBJECT);
  swing_start_meh =create_multi_event_handler(2,inputs,output,swing_start,execution_policy); 

  inputs[0] = (stream*) create_orientation_input_stream(10,ORIENTATION_SUBJECT,parse_event_1,swing_start_meh); 
  inputs[1] = (stream*) create_accel_vector_input_stream(10,ACCELERATION_SUBJECT,parse_event_2,swing_start_meh); 
  start_meh(swing_start_meh);
 
  

  while(1){
    sleep(1000);
  }
}

/* should be able to merge the parse_event into the  */
/* generic stream add_value bitsy , probably in a seperate */
/* wrapper function so I still have the add functionality  */
/* seperate.  */
/* No way to do this without hassle because I need to have knowledge of the stream in question */
/*  best bet is to use a macro to create this function but it has to be done in each create */
/*  for each new stream. */
void parse_event_1(event *ev){
  
  parse_orientation_event(ev,swing_start_meh->inputs[0]);
  
}

void parse_event_2(event *ev){
  parse_accel_vector_event(ev,swing_start_meh->inputs[1]);
}


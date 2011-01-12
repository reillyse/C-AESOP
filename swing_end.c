#include "libmeh.h"
#include <unistd.h>
#include <pthread.h>
#include "steam_interface.h"
#include "libmeh.h"
#include "acceleration.h"
#include "orientation.h"
#include "swing.h"
#include "vector.c"
#include "quaternion.h"
#include <string.h>
 #define _REENTRANT

void parse_event_1(event *);
void parse_event_2(event *);
stream* inputs[2];
multi_event_handler* swing_end_meh;

/*
  Implement a MultiEventHandler to determine when a swing has started
  Outputs a swing event when it is detected
*/

void swing_end(orientation *orient ,accel_vector* accel, swing* result){
  float ideal_swing[] = {6.86f,2.19f,3.899f};
  float  angle =  angle_between(&ideal_swing,accel,3) *57.2957795;
  float mag_diff = magnitude(&ideal_swing,3) - magnitude(accel,3);
  if(mag_diff < 0) { mag_diff = mag_diff * -1;}
	//top of racket points to the roof  
  if (angle < 20 &&   magnitude(accel,3) > 0  && rotation_around_Z(orient) >30 && rotation_around_Z(orient) < 150 ){
    strcpy(result -> message ,"END");

  }
 
}
int execution_policy(){
return inputs[1]->status;
}
int main(){
 
  setup_steam();
  stream* output =  (stream*) create_swing_output_stream(SWING_SUBJECT);
  swing_end_meh =create_multi_event_handler(2,inputs,output,swing_end,execution_policy);
  inputs[0] = (stream*) create_orientation_input_stream(10,ORIENTATION_SUBJECT,parse_event_1,swing_end_meh);
  inputs[1] = (stream*) create_accel_vector_input_stream(10,ACCELERATION_SUBJECT,parse_event_2,swing_end_meh);

  while(1){ sleep(1000);} //let my multi-event handlers take care of the program flow
}

void parse_event_1(event *ev){
  parse_orientation_event(ev,swing_end_meh->inputs[0]);
}

void parse_event_2(event *ev){
  parse_accel_vector_event(ev,swing_end_meh->inputs[1]);
}

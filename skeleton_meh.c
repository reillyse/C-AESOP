#include "steam_interface.h"
#include "libmeh.h"

void parse_event_1(event *);  //this is my callback function for STEAM need one for each input stream
void parse_event_2(event *);

stream* inputs[2];

void handler(orientation *orient ,accel_vector* accel, swing* result){

}

multi_event_handler* swing_start_meh;

int main(){

  setup_steam();

 stream* output =  (stream*) create_swing_output_stream(20,SWING_SUBJECT);
  swing_start_meh =create_multi_event_handler(2,inputs,output,swing_start,INDIVIDUAL);

  inputs[0] = (stream*) create_orientation_input_stream(10,ORIENTATION_SUBJECT,parse_event_1,GREEDY,swing_start_meh);
  inputs[1] = (stream*) create_accel_vector_input_stream(10,ACCELERATION_SUBJECT,parse_event_2,LAZY,swing_start_meh);

  while(1){
    sleep(1000);
  }
}


void parse_event_1(event *ev){

  parse_orientation_event(ev,swing_start_meh->inputs[0]);

}

void parse_event_2(event *ev){
  parse_accel_vector_event(ev,swing_start_meh->inputs[1]);
}

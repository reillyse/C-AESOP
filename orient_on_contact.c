#include "steam_interface.h"
#include "libmeh.h"
#include "ball_contact.h"
#include "orientation.c"
#include "orient_on_contact.h"

void parse_event_1(event *);  //this is my callback function for STEAM need one for each input stream
void parse_event_2(event *);

stream* inputs[2];
void detect_orient_on_contact(orientation *orient ,ball_contact* contact, orient_on_contact* result){

memcpy(result ->orient , &((orientation_input_stream *)inputs[1]) -> head -> tqh_first -> entries.tqe_next.value, sizeof(orientation);

}


multi_event_handler* detect_orient_on_contact_meh;
int execution_policy(){
retrun inputs[1] -> status;
}
int main(){

  setup_steam();

  stream* output =  (stream*) create_swing_output_stream(ORIENTATION_ON_CONTACT_SUBJECT);
  detect_orient_on_contact_meh =create_multi_event_handler(2,inputs,output,detect_orient_on_contact,execution_policy);

  inputs[0] = (stream*) create_orientation_input_stream(2,ORIENTATION_SUBJECT,parse_event_1,swing_start_meh);
  inputs[1] = (stream*) create_ball_contact_input_stream(1,BALL_CONTACT_SUBJECT,parse_event_2,swing_start_meh);

  while(1){
    sleep(1000);
  }
}


void parse_event_1(event *ev){

  parse_orientation_event(ev,detect_orient_on_contact_meh->inputs[0]);

}

void parse_event_2(event *ev){
  parse_ball_contact_event(ev,detect_orient_on_contact_meh->inputs[1]);
}

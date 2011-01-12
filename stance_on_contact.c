#include "steam_interface.h"
#include "libmeh.h"
#include "load_sensor.h"
#include "acceleration.h"
#include "swing.h"
#include "ball_contact.h"
#include "stance.h"
#include "ball_contact.h"
void parse_event_1(event *);  //this is my callback function for STEAM need one for each input stream
void parse_event_2(event *);
void parse_event_3(event *);  
void parse_event_4(event *);
void parse_event_5(event *);  

stream* inputs[4];


int execute_on_first_stream(){
	//if the first input stream had a new value return true
	return inputs[0]->status;
}
void stance_on_contact(ball_contact* contact ,accel_vector* accel,load_sensor* load_left,load_sensor* load_right,stance* result){
	//simple analysis of load 
	// wish to know if all of the players weight is on his/her lead foot
	//take the last 10 measurements and average them, compare with the other sensor, see what proportion is they are
	float left = average_last_ten(((load_sensor_input_stream*)inputs[2])-> head);
	float right = average_last_ten(((load_sensor_input_stream*)inputs[3])-> head);
	
//the sign can be used to determine which foot is the lead foot
	result -> rating = left/right;
	
}

multi_event_handler* stance_on_contact_meh;


int main(){

  setup_steam();

 stream* output =  (stream*) create_stance_output_stream(STANCE_SUBJECT);
  stance_on_contact_meh =create_multi_event_handler(2,inputs,output,stance_on_contact,execute_on_first_stream);
  inputs[0] = (stream*) create_ball_contact_input_stream(10,BALL_CONTACT_SUBJECT,parse_event_1,stance_on_contact_meh);
  inputs[1] = (stream*) create_swing_input_stream(10,SWING_SUBJECT,parse_event_3,stance_on_contact_meh);
  inputs[2] = (stream*) create_load_sensor_input_stream(10,LEFT_LOAD_SENSOR_SUBJECT,parse_event_4,stance_on_contact_meh);
  inputs[3] = (stream*) create_load_sensor_input_stream(10,RIGHT_LOAD_SENSOR_SUBJECT,parse_event_5,stance_on_contact_meh);
 	start_meh(stance_on_contact_meh);
  while(1){
    sleep(1000);
  }
}


void parse_event_1(event *ev){

  parse_ball_contact_event(ev,stance_on_contact_meh->inputs[0]);

}
void parse_event_2(event *ev){

  parse_accel_vector_event(ev,stance_on_contact_meh->inputs[1]);

}
void parse_event_3(event *ev){

  parse_swing_event(ev,stance_on_contact_meh->inputs[2]);

}

void parse_event_4(event *ev){

  parse_load_sensor_event(ev,stance_on_contact_meh->inputs[3]);

}

void parse_event_5(event *ev){

  parse_load_sensor_event(ev,stance_on_contact_meh->inputs[4]);

}

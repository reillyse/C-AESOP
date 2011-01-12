#include "steam_interface.h"
#include "libmeh.h"
#include "ball_contact.h"
#include "acceleration.h"

void parse_event_1(event *);  //this is my callback function for STEAM need one for each input stream
void parse_event_2(event *);

stream* inputs[2];

void detect_ball_contact(accel_vector* accel,swing* s,ball_contact* result){
		//we only consider acceleration in the direction that the racket is swinging,i.e perpindicular to the racket face
		float latest_value = ((accel_vector_stream*)inputs[1])-> head ->tqh_first->value.y ;
		float middle_value = ((accel_vector_stream*)inputs[1])-> head ->tqh_first->entries ->tqe_next->value.y;
		float last_value  =  ((accel_vector_stream*)inputs[1])-> head ->tqh_first->entries ->tqe_next->entries->tqe_next->value.y;

   if ((  fabsf(last_value) > 40 && fabsf (last_value - middle_value) >8 
   		&& fabsf(latest_value - middle_value) > 6)
   		 &&((last_value > middle_value && latest_value > middle_value) 
   		 || (last_value < middle_value && latest_value <  middle_value)){ 
	result-> val =1;
     } 



}

multi_event_handler* ball_contact_meh;
int execution_policy(){
		//if an acceleration caused this to execute
if(inputs[0] -> status){
	swing latest_value = ((swing_input_stream*)inputs[1])-> head->tqh_first ->value ;
	if(latest_value != NULL){
		if(strcmp(latest_value.message,"START"){
			return 1; //execute handler
			}
	}
	//now check swing status
	
}
return 0;
}
int main(){

  setup_steam();

 stream* output =  (stream*) create_swing_output_stream(BALL_CONTACT_SUBJECT);
  ball_contact_meh =create_multi_event_handler(2,inputs,output,detect_ball_contact,execution_policy);
  inputs[0] = (stream*) create_accel_vector_input_stream(10,ACCELERATION_SUBJECT,parse_event_1,ball_contact_meh);
  inputs[1] = (stream*) create_swing_input_stream(1,SWING_SUBJECT,parse_event_2,ball_contact_meh);


  while(1){
    sleep(1000);
  }
}


void parse_event_1(event *ev){

  parse_accel_vector_event(ev,ball_contact_meh->inputs[0]);

}

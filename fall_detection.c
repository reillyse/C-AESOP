#include "steam_interface.h"
#include "libmeh.h"
#include "vector.c"
#include "acceleration.h"
#include "emergency.h"
#define ACCELEROMETER_HIP "ACCELEROMETER_HIP"
#define ACCELEROMETER_WRIST "ACCELEROMETER_WRIST"
#define ACCELEROMETER_SHOE "ACCELEROMETER_SHOE"
#include <time.h>
#include <stdio.h>

void parse_event_1(event *);  //this is my callback function for STEAM need one for each input stream
void parse_event_2(event *);
void parse_event_3(event *);

int execution_policy();
stream* inputs[3];
struct tm *local;
time_t t;
void handler(accel_vector* hip, accel_vector* wrist,accel_vector* shoe, emergency* result){

  struct accel_vector_stream_entry *np;					
  struct accel_vector_listhead *head = ((accel_vector_stream*)inputs[0]) -> head;			
  for (np = head->tqh_first; np != NULL; np = np->entries.tqe_next){ 
    if(magnitude(&(np->value),3) > 15.0){
      t = time(NULL);
      local = localtime(&t);
      strcpy(result -> time ,asctime(local));   //assigns the current time to the emergency
      return;
    }
  }
  head = ((accel_vector_stream*)inputs[1]) -> head;
  for (np = head->tqh_first; np != NULL; np = np->entries.tqe_next){ 
    if(magnitude(&(np->value),3) > 15.0){
      t = time(NULL);
      local = localtime(&t);
      strcpy(result -> time ,asctime(local));   //assigns the current time to the emergency
      return;
    }
  }
  head = ((accel_vector_stream*)inputs[2]) -> head;
  for (np = head->tqh_first; np != NULL; np = np->entries.tqe_next){ 
    if(magnitude(&(np->value),3) > 15.0){
      t = time(NULL);
      local = localtime(&t);
      strcpy(result -> time ,asctime(local));   //assigns the current time to the emergency
      return;
    }
  }		
}

multi_event_handler* fall_detection_meh;

int main(){

  setup_steam();
  stream* output =  (stream*) create_emergency_output_stream(1,EMERGENCY_SUBJECT);
  fall_detection_meh =create_multi_event_handler(3,inputs,output,handler,execution_policy);
  inputs[0] = (stream*) create_accel_vector_input_stream(10,ACCELEROMETER_HIP,parse_event_1,fall_detection_meh);
  inputs[1] = (stream*) create_accel_vector_input_stream(10,ACCELEROMETER_WRIST,parse_event_2,fall_detection_meh);
  inputs[2] = (stream*) create_accel_vector_input_stream(10,ACCELEROMETER_SHOE,parse_event_3,fall_detection_meh);
  start_meh(fall_detection_meh);
  while(1){
    sleep(1000);
  }
}
int count=0;
//checks the hip accelerometer output to see if the patient has changed posture
int execution_policy(){
 
// printf("EP\n");
  if(count % 10 == 0){
    printf("EP... events received = %d\n",count);
}
    fflush(NULL); 
 count++;
 return 0;  
  struct accel_vector_stream_entry *latest;	
  struct accel_vector_stream_entry *previous;	
  float threshold = 1; //radian value for threshold for when we decide the person is lying down
  accel_vector upright = {0,-9.8,0};
  if(!inputs[0] -> status)
    return 0;     //if the hip accelerometer has not got a new reading
		
  struct accel_vector_listhead *head = ((accel_vector_stream*)inputs[0]) -> head;			
  latest = head->tqh_first ;
  previous = latest ->entries.tqe_next;
  if(previous == NULL){
    //only one value, no change in posture
    return 0;
  }
  if(angle_between(&(latest -> value) , &upright  , 3) > threshold  && 
     angle_between(&(previous -> value) , &upright  , 3) < threshold ){

    return 1;  //change in posture detected 
  }
}


void parse_event_1(event *ev){

  parse_accel_vector_event(ev,inputs[0]);

}

void parse_event_2(event *ev){
  parse_accel_vector_event(ev,inputs[1]);
}
void parse_event_3(event *ev){
  parse_accel_vector_event(ev,inputs[2]);
}

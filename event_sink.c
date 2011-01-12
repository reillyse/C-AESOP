#include <unistd.h>
#include <pthread.h>
#include "steam_interface.h"
#include "base64.h"
#include "acceleration.h"
#define _REENTRANT
#define ACCELEROMETER_HIP "ACCELEROMETER_HIP"
#define ACCELEROMETER_WRIST "ACCELEROMETER_WRIST"
#define ACCELEROMETER_SHOE "ACCELEROMETER_SHOE"
int event_count = 0; 
void receive_this_event(event* ev){
  event_count++;
   //  if (val != NULL){

   //      printf("%s \n",val->message);
   //}
   if(event_count %1000 == 0){ 
   printf("event count is %d\n",event_count);
   fflush(NULL);
   }
}
int main(){

  setup_steam();
  event* my_event = alloc_event();
  //  setup_event("FloatOutput1",LOCAL,my_event);

  subscribe_to_event(ACCELEROMETER_HIP,my_event,receive_this_event);  
  subscribe_to_event(ACCELEROMETER_WRIST,my_event,receive_this_event);
  subscribe_to_event(ACCELEROMETER_SHOE,my_event,receive_this_event);
  while(1){
    sleep(10000000);
}
}

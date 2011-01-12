#include "steam_interface.h"
#include "base64.h"
#include "libmeh.h"
#include "acceleration.h"
#define ACCELEROMETER_HIP "ACCELEROMETER_HIP"
#define ACCELEROMETER_WRIST "ACCELEROMETER_WRIST"
#define ACCELEROMETER_SHOE "ACCELEROMETER_SHOE"
int delay = 0;
int main(int argc, char *argv[]){

  if(argc < 2){
    printf("no options:\n usage= accel event_subject delay \n");
    exit(0);
}
 char*  subject1 = argv[1];
  delay = atoi(argv[2]);
  
  printf("using event type %s and delay %i",subject1,delay);
  
  setup_steam();
  // char* subject1=ACCELEROMETER_WRIST ;
  event* event1;
  accel_vector* acceleration;
  acceleration = malloc(sizeof(accel_vector));
  event1= alloc_event();  
  channel_id channel_1 = setup_event(subject1,LOCAL,event1);
  acceleration -> x = 1;
  acceleration->y = .7;
  acceleration->z = .1;
  char* str =malloc(2 * sizeof(accel_vector));
  str = encode_base64(sizeof(accel_vector),(char*)acceleration);
  strcat(str,"\0");
 int counter =0;
  while(1){


    send_my_event(str,channel_1,event1);
     //     usleep(18000);   //50hz sensor 
     usleep(delay);
 
  }
  

}

#include "steam_interface.h"
#include "base64.h"
#include "libmeh.h"
#include "orientation.h"

int main(){


  setup_steam();
  char* subject1= ORIENTATION_SUBJECT;
  event* event1;
  orientation* orient;
  orient= malloc(sizeof(orientation));
  event1= alloc_event();  
  channel_id channel_1 = setup_event(subject1,LOCAL,event1);
  orient -> o1 = 1;
  orient-> o2= .7;
  orient ->o3 = .1;
  orient -> o4 = .8;

  while(1){

    printf("%f,%f,%f\n", orient->o1,orient->o2, orient->o3,orient->o4);
    
    char* str =malloc(2 * sizeof(orientation));
    str = encode_base64(sizeof(orientation),(char*)orient);
     printf("trigger \n");
     send_my_event(strcat(str,"\0"),channel_1,event1);
     free(str);
     orient-> o1 ++;
     usleep(100000);   //200hz sensor
  }
  

}

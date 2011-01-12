#include "steam_interface.h"

void my_adapt(proximity* changed_prox){
  printf("Adaptation receieved at Local app\n");
}

void receive_my_event(event* ev){
  gettimeofday(&tv, 0);
  fprintf(recv_logger, "%s => %d.%d\n", ev->cont, tv.tv_sec, tv.tv_usec);
  printf("CALLBACK AT LOCAL APP received event \n\tsubject - %s\n\t content %s\n", ev->sub, ev->cont);
}

int setup_steam(){


  send_logger = fopen("send-times", "w+");
  recv_logger = fopen("recv-times", "w+");

  my_location.lat = LOCATION_X;
  my_location.lon = LOCATION_Y;

  if(init_rte_subscribe()<0){
    printf("init subscribe failed.. you probably need to change the default address in dummy-sear-impl.h \n");
    exit(0);
    return -1;
  }
  if(init_rte_publish() <0){
    printf("init-rte failed. we are doomed\n");
    exit(0);
    return -1;
  }
 printf("init_rte.. done\n");
}

/* returns a channel id for the channel we've announced to  */

int setup_event(char* steam_subject,prox_type proximity,event* my_ev ){
 
  channel_id c_id;
  // square
  my_proximity = alloc_proximity();
  my_proximity->proximity_hull.type = proximity;

  // non-rt latency
  lat.min = 0; lat.max = 0;

  //period is in milliseconds
  per = 1000;

  //my_event_subject = alloc_subject(100);

  
    my_ev -> sub =  alloc_subject(100);
    strcpy(my_ev->sub, steam_subject);
  printf("calling announce.. %s\n", my_ev->sub);
  if((c_id= announce(my_ev->sub, my_proximity, lat, per, my_adapt)) == 0){
    printf("Announcement went arse ways\n");
    free(my_proximity);
    return -1;
  }

  printf("announce.. done\n");
  
   printf("steam init done \n");
   free(my_proximity);
   return c_id;
}

/* returns a subscription_id for the channel we've subscribed to. 
   
   takes a pointer to an event that we want to initalise. We'll use it later to send to

*/

int subscribe_to_event(char* sub, event* ev,void (*rt)(event*)){
  char* my_event_subject = malloc(100);
  
  subscription_id sub_id;
  strcpy(my_event_subject,sub);  
  ev->sub = my_event_subject;
  printf("init rte ... done\n");
   if((sub_id = subscribe(my_event_subject, rt)) == 0){ 
     printf("subscription failed\n"); 
     return -1;
   } 
   printf("init subscribe ... done\n"); 
   free(my_event_subject);
   return sub_id;
}
void send_my_event(char* content,channel_id my_channel,event* my_ev){

  
  //fill in event
  my_ev->cont = (char*)alloc_content(200);
  strcpy(my_ev->cont, content); 
  
  //  gettimeofday(&tv, 0);
  //  fprintf(send_logger, "%s => %d.%d\n", my_ev->cont, tv.tv_sec, tv.tv_usec);
  if(send_event(my_channel, my_ev) <0){
      printf("send failed \n");
      exit(1);
    }
  free(my_ev->cont); //    probably should be freeing this up
  }

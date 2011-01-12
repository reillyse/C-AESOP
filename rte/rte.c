//#include "rte-impl.h"
#include "dummy-sear.h"
#include "rte-mem.h"
#include <mqueue.h>
int max_channel_id;
channel_table subscribe_channels;
channel_table publish_channels;
int initialised;


int init_rte_publish(){
  INIT_LIST_HEAD(&publish_channels.list);
  return init_dummy_sear_publish();
}

// question from aline - pass arg to say if local or remote will need

// another call to tell rte that the app is moving from local to
// remote.

int init_rte_subscribe(){
  INIT_LIST_HEAD(&subscribe_channels.list);
  return init_dummy_sear_subscribe();
}

void* make_announcement(event_channel*);

// Periodically broadcast announce messages to enable proximity
// filtering to work.
channel_id announce(subject sub, proximity* prox, 
	     latency lat, period per,
	     adaptation adapt){
  printf("announce:                    sub  => %s \n",sub);
  printf("announce:     proximity hull type => %i \n",prox->proximity_hull.type);
  printf("announce:                lat.min  => %i ,lat.max => %i \n" ,lat.min,lat.max);
  printf("announce:                 per     => %f \n",per);
  //  printf("announce:               adaption  => %s \n",adapt);
  fflush(stdout);
  channel_table* cb_n;
  event_channel* our_channel;
  printf("before reserve channel\n");
  if((our_channel = reserve_channel(lat, per, prox, adapt, sub)) == 0){
    printf("shit happened at channel reservation time.\n");
    exit(1);
  }
  
  cb_n = (channel_table*)alloc_channel_table();
  fflush(stdout);
  cb_n->cb = 0;
  cb_n->sear_channel = our_channel;
  max_channel_id += 1;
  cb_n->id = max_channel_id;
  list_add(&(cb_n->list), &publish_channels.list);
  printf("after list_add before make_announcment\n");
  fflush(stdout);
  make_announcement(our_channel);
  printf("returning from announce\n");
  fflush(stdout);
  return cb_n->id;
}

void* make_announcement(event_channel* our_channel){

  steam_event* announce_event =  (steam_event*)alloc_steam_event();

  announce_event->event.sub = our_channel->sub;
  announce_event->event.cont = "announcement";
  announce_event->event.proximity_type = our_channel->actual->type;

  switch (announce_event->event.proximity_type){
  case HULL:
    announce_event->hull_event.p = our_channel->actual;
    if(send_on_channel((void*)announce_event, our_channel) == -1){
      perror("send_on_channel: announce");
      pthread_exit(0);
    }
    break;
  case CIRCLE:
    announce_event->circle_event.p = our_channel->actual;
    if(send_on_channel((void*)announce_event, our_channel) == -1){
      perror("send_on_channel: announce");
      pthread_exit(0);
    }
    break;
  case LOCAL:
    announce_event->event.proximity_type = our_channel->actual->type;
    fflush(stdout);
    //send_local_event(announce_event);
      send_on_local_channel(announce_event);
  }

  //  printf("make_announcement, subject - %s\n", announce_event->event.sub);
  //printf("make_announcement, content - %s\n", announce_event->event.cont);
  //printf("make_announcement, proximity_type - %d\n", announce_event->event.proximity_type);

  free(announce_event);

}

void unannounce(channel_id ch){
  
  struct list_head *tmp;
  channel_table *n;
  
  list_for_each_entry(n, &publish_channels.list, list){
    printf("here %s\n", n->sear_channel->sub);
    if (n->id == ch && n->cb == 0){
      free_channel(n->sear_channel);
      list_del(&(n->list));
      free(n);
      return;
    }
  }
  // TODO - unreserve_channel from sear
}

int send_event(int channel_id, event* event){

  struct list_head *tmp;
  channel_table *n;
  steam_event* st_event;
  event_channel* channel;

  list_for_each_entry(n, &publish_channels.list, list){
    if (n->id == channel_id){
      channel = n->sear_channel;
      break;
    }
  }  

  st_event = (steam_event*)alloc_steam_event();
  st_event->event.sub = event->sub;
  st_event->event.cont = event->cont;
  st_event->event.proximity_type = channel->actual->type;

  switch(channel->actual->type){
  case HULL:
    st_event->hull_event.p = channel->actual;
    if(send_on_channel(st_event, channel) == -1){
      perror("send_on_channel");
      return -1;
    }
    break;
  case CIRCLE:
    st_event->circle_event.p = channel->actual;
    if(send_on_channel(st_event, channel) == -1){
      perror("send_on_channel");
      return -1;
    }
    break;
  case LOCAL:
    st_event->event.proximity_type = channel->actual->type;
    // send_local_event(st_event);
    //  printf("sending local event now!!!\n");
    send_on_local_channel(st_event);
  }
  free(st_event);
  return 0;
}

subscription_id subscribe(subject sub, receive_event cb){

  event_channel* our_channel;
  channel_table* cb_n;

  our_channel = reserve_channel_for_subscribe(sub);

  cb_n = (channel_table*)alloc_channel_table();
  cb_n->cb = cb;
  cb_n->sear_channel = our_channel;
  max_channel_id += 1;
  cb_n->id = max_channel_id;
  list_add(&(cb_n->list), &subscribe_channels.list);
  return cb_n->id;
}

void unsubscribe(subscription_id ch){

  struct list_head *tmp;
  channel_table *n;
  
  list_for_each_entry(n, &subscribe_channels.list, list){
    printf("here %s\n", n->sear_channel->sub);
    if (n->id == ch){
      free_channel(n->sear_channel);
      list_del(&(n->list));
      free(n);
      return;
    }
  }
}

void make_callbacks_for(steam_event* st_ev){
  
  struct list_head *tmp;
  channel_table *n;
  event* ev;
  
  list_for_each_entry(n, &subscribe_channels.list, list){
    if (strcmp(n->sear_channel->sub, st_ev->event.sub) == 0 && n->cb != 0){
      ev = (event*)alloc_event();
      ev->sub = st_ev->event.sub;
      ev->cont = st_ev->event.cont;      
      n->cb(ev);
      free(ev);
    }
  } 
  //printf("make callbacks for done\n");
}

void send_local_event(steam_event* st_ev){
  struct list_head *tmp;
  channel_table *n;
  event* ev;
  printf("trying to send local event \n");
  //
   int c,flags;
  mqd_t mqd;
  int file_mode =0666;
  flags  = O_RDWR ; 
  char* to_send = (char*)marshal_steam_event(st_ev);
  // printf("LOCAL CALLBACK\n");
  

  // sean - local delivery doesn't work
  // this is because this will only get delivered in the same address space.. need to use some form of 
  // IPC to deliver these messages.
  // thinking POSIX threads
  // when local processes subscribe to an event they know it is local and supply a message queue that they will be listening on
  // message queue send 

  // need to also implement a select on the other side that listens for messages and hits the callback

     
  // aline - loop only if subscribe_channels != null
  list_for_each_entry(n, &subscribe_channels.list, list){ 
    if (strcmp(n->sear_channel->sub, st_ev->event.sub) == 0 && n->cb != 0){
      
   
      //mqd =  mq_open("/myq", flags,file_mode, NULL);
      /* if (mq_send(mqd,to_send,strlen(to_send),1)==-1){ */
      /* 	perror("sending local event failed:"); */
      /* } */
      
      fflush(stdout);
      ev = (event*)alloc_event();
      ev->sub = st_ev->event.sub;
      ev->cont = st_ev->event.cont;  
      // n->cb(ev);
      
    
    }
  }
  fflush(stdout);
}

void free_steam_event(steam_event* ev){

  switch(ev->event.proximity_type){
  case HULL:
    free(ev->hull_event.p->proximity_hull.hull);
    free(ev->hull_event.p);
    break;
  case CIRCLE:
    free(ev->circle_event.p);
    break;
  }
  //  free((struct q*)ev->event.data_content);
  free(ev);
 // printf("free steam event done \n");
}

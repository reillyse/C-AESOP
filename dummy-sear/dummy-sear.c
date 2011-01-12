#include "dummy-sear-impl.h"
#include "rte-mem.h"
#include "rte-marshal.h"
#include "time.h"
#include "sys/time.h"
#include <sys/un.h>
#include <sys/ioctl.h>
#include <net/if.h>
/*
 * How this all works.
 * Broadcast on the local network for non-local messages 
 * I've added in multicast to the local interface for LOCAL events.
 * We join the group in dummy-sear-impl and send messages to this group. 
 * TTL is set to 0 for local events, this means that the message never leaves the machine.
 * 
 * Reason for this implementation is we want to lower the cost of LOCAL events i.e. don't want to waste battery power for messages
 * to be delivered locally. Would like a speed up
 * also but have found no easy way of delivering the messages to all process on the machine.
 * (can't share unix domain datagram sockets as far as I can make out!)
 * and need to register for other forms of IPC (message q etc)
 * Broadcast only delivers 1 message on the loopback interface, this is how it is defined(see steven's although not much info there)
 * multicast to this interface would be ideal
 * as its not a real device so preumably is much quicker than any of the real interfaces, and would operate 
 * in the absence of any other network devices but having trouble specifying the loopback interface, I've nearly implemented this 
 * below (see the ioctl stuff) so if you need the speed do this, think the ttl 0 solution works in much the same way though.
 */

int send_socket;
int recv_socket;
int local_send_socket;    // this socket is used to send stuff to the local loopback address
int local_recv_socket;

struct sockaddr_in sear_bcast_addr; 
struct sockaddr_in local_sear_bcast_addr;
struct sockaddr_in local_interface_addr;
int get_send_socket();
int get_recv_socket();
int get_local_send_socket(); 
void* recv_event(void*);
void* select_for_recv(void*);

FILE *logger;


int get_local_send_socket(){
 int yes = 1;

  if ((local_send_socket = socket(AF_INET, SOCK_DGRAM, 0)) <0){
    perror("local send socket");
    return -1;
  }
  
  if (setsockopt(local_send_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) <0) {
    perror("local send setsockopt");
    return -1;
  }

  /* if (setsockopt(local_send_socket, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int)) == -1) { */
  /*   perror("local send setsockopt"); */
  /*   return 0; */
  /* } */
  u_char loop =1;
    if( setsockopt(local_send_socket, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop))<0){
      perror("get local send socket loopback");
      return -1;
    }
      
    u_char ttl = 0;  //hacking ttl to 0 to stop local multicasts going out on the wire!
    if (setsockopt(local_send_socket, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0){
      perror("get local send socket, setting ttl ");
      exit(-1);
    }
    int size = sizeof(ttl);
    int response;
    if(response= getsockopt(local_send_socket, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, &size) < 0){
      perror("getting the time to live ");
    }
    printf("the ttl is coming back as %i \n", ttl);
    struct ifreq ifr;
    
    /*    ifr.ifr_name= "lo"; */

    /* if (ioctl(local_send_socket, SIOCGIFADDR, (caddr_t)&ifr, sizeof(struct ifreq))<0){ */
    /*   perror("trying to get the interface address"); */
    /*   return -1; */
    /* } */
    /* printf("the interface address is %s \n",ifr.ifr_addr); */
										
    
    
     bzero(&local_interface_addr, sizeof(local_interface_addr));
     local_interface_addr.sin_family = AF_INET;
        local_interface_addr.sin_port = htons(DUMMY_SEAR_PORT);
      inet_pton(AF_INET, LOCAL_INTERFACE_ADDR, &local_sear_bcast_addr.sin_addr);

     if( setsockopt(local_send_socket, IPPROTO_IP, IP_MULTICAST_IF,&local_interface_addr ,sizeof(local_interface_addr)) < 0){
       perror("get_local_send_socket setting local interface ");
       return -1;
     }
    
    //    setsockopt(local_send_socket, IPPROTO_IP, IP_MULTICAST_IF,INADDR_ANY ,sizeof(INADDR_ANY));
  bzero(&local_sear_bcast_addr, sizeof(local_sear_bcast_addr));
  local_sear_bcast_addr.sin_family = AF_INET;
  local_sear_bcast_addr.sin_port = htons(DUMMY_SEAR_PORT);
  inet_pton(AF_INET, MULTICAST_GROUP, &local_sear_bcast_addr.sin_addr);

  return 0; // no error
}

int get_local_recv_socket(){
  // recv 
  int yes = 1;
  int sin_size = sizeof(struct sockaddr_in);

  if ((local_recv_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    perror("recv socket");
    return 0;
  }
 
  if (setsockopt(local_recv_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("recv setsockopt");
    return 0;
  }

  if (setsockopt(local_recv_socket, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int)) == -1) {
    perror("recv setsockopt");
    return 0;
  }

  struct ip_mreq mreq;

  /* use setsockopt() to request that the kernel join a multicast group */
  mreq.imr_multiaddr.s_addr=inet_addr(MULTICAST_GROUP);
  mreq.imr_interface.s_addr=htonl(INADDR_ANY);
  if (setsockopt(local_recv_socket,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
    perror("setsockopt loca_recv_socket");
    exit(1);
  }


  bzero(&local_sear_bcast_addr, sizeof(local_sear_bcast_addr));
  local_sear_bcast_addr.sin_family = AF_INET;
  local_sear_bcast_addr.sin_port = htons(DUMMY_SEAR_PORT);
  inet_pton(AF_INET, MULTICAST_GROUP, &local_sear_bcast_addr.sin_addr);
  
  if (bind(local_recv_socket, (struct sockaddr *)&local_sear_bcast_addr, sizeof(struct sockaddr)) == -1){
    perror("bind");
    return -1;
  }
  u_char loop =1;
    if( setsockopt(local_recv_socket, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop))<0){
      perror("get local recv socket loopback");
      return 0;
    }
  return 0;
}

int get_recv_socket(){
  // recv 
  int yes = 1;
  int sin_size = sizeof(struct sockaddr_in);

  if ((recv_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    perror("recv socket");
    return 0;
  }
 
  if (setsockopt(recv_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("recv setsockopt");
    return 0;
  }

  if (setsockopt(recv_socket, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int)) == -1) {
    perror("recv setsockopt");
    return 0;
  }

  bzero(&sear_bcast_addr, sizeof(sear_bcast_addr));
  sear_bcast_addr.sin_family = AF_INET;
  sear_bcast_addr.sin_port = htons(DUMMY_SEAR_PORT);
  inet_pton(AF_INET, DUMMY_SEAR_BCAST_ADDR, &sear_bcast_addr.sin_addr);
  
  if (bind(recv_socket, (struct sockaddr *)&sear_bcast_addr, sizeof(struct sockaddr)) == -1){
    perror("bind");
    return -1;
  }
  return 0;
}

int get_send_socket(){
  
  int yes = 1;

  if ((send_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    perror("send socket");
    return 0;
  }
  
  if (setsockopt(send_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("send setsockopt");
    return 0;
  }

  if (setsockopt(send_socket, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int)) == -1) {
    perror("send setsockopt");
    return 0;
  }

  bzero(&sear_bcast_addr, sizeof(sear_bcast_addr));
  sear_bcast_addr.sin_family = AF_INET;
  sear_bcast_addr.sin_port = htons(DUMMY_SEAR_PORT);
  inet_pton(AF_INET, DUMMY_SEAR_BCAST_ADDR, &sear_bcast_addr.sin_addr);

  return 0; // no error
}



// just receive this message, do not maintain a connection back to
// the sender. Remember this is broadcast. To send a message back,
// just broadcast a response.
void* recv_event(void* fd){
  char* recv_buffer;
  steam_event *ev;
  struct sockaddr_in recv_addr; 
  int from_len;

  from_len = sizeof(struct sockaddr);
  recv_buffer = (char*)alloc_buffer(MAX_BUFFER);


  if(recvfrom((*(int*)fd), recv_buffer, MAX_BUFFER, 0, (struct sockaddr*)(&recv_addr), (socklen_t*)&from_len) == -1){
    perror("recvfrom: recv_event");
    pthread_exit(0);
  }


  ev = (steam_event*)unmarshal_steam_event(recv_buffer);
  //  fprintf(stderr, "In recv_event: subject: %s\ncontent: %s\n", ev->event.sub, ev->event.cont);
  // fflush(stderr);
  make_callbacks_for(ev);
  free_steam_event(ev);
  free(recv_buffer);
  //fprintf(stderr, "recv_event thread done\n");
  //fflush(stderr);
}

int init_dummy_sear_subscribe(){

  logger = fopen("log","a+");

	if(get_recv_socket() == -1){
    return -1;
  }

	if(get_local_recv_socket() == -1){
    return -1;
  }
  printf("got recv sockets.\n");
      
  // do select
  if(pthread_create(&select_t_id, 0, select_for_recv, 0) != 0){
    perror("pthread_create");
    return -1;
  }
  if(pthread_detach(select_t_id)){
  perror("detaching select thread to receive events:");
  return -1;
  }
  return 0;
}

int init_dummy_sear_publish(){

  printf("init_dummy_sear_publish++\n");

  if(get_local_send_socket() == -1){
    printf("can't get local_send_socket \n");
    return -1;
  }

  if (get_send_socket() == -1){
    return -1;
  }
  printf("got send socket.\n");
  return 0;
}
/*
 *  Must also check the local socket to make sure we catch local messages as well.
 *  At the moment do it all the time.
 *  TODO get add it to the recv_set and away we go
 * 
 */


void* select_for_recv(void* parameter){
  pthread_t recv_t_id;
  fd_set recv_set;
  for(;;){
    FD_ZERO(&recv_set);
    FD_SET(recv_socket, &recv_set);
    FD_SET(local_recv_socket, &recv_set);
    
    int max = local_recv_socket > recv_socket ? local_recv_socket : recv_socket;
    //    printf(" %i is biggest of %i and %i",max, local_recv_socket,recv_socket);
    if (select( max  + 1, &recv_set, 0, 0, NULL) == -1){
      perror("select");
      pthread_exit(0);
    }
    //fprintf(stderr, "select returned ... \n");
    //fflush(stderr);
    if(FD_ISSET(recv_socket, &recv_set) ){
      //      if(pthread_create(&recv_t_id, 0, recv_event, (void*)&recv_socket) != 0){
      /* 	perror("pthread_create"); */
      /* 	return 0; */
      /*       } */
      /*       pthread_detach(recv_t_id); */
      recv_event((void*)&recv_socket);
    }
     if(FD_ISSET(local_recv_socket, &recv_set) ){
      //      if(pthread_create(&recv_t_id, 0, recv_event, (void*)&recv_socket) != 0){
      /* 	perror("pthread_create"); */
      /* 	return 0; */
      /*       } */
      /*       pthread_detach(recv_t_id); */
      // printf("select returned we have a local event\n");
      recv_event((void*)&local_recv_socket);
    }
  }
}

event_channel* reserve_channel(latency lat,
			       float requested_period,
			       proximity* prox, 
			       adaptation adapt,
			       subject event_name
			       ){

  event_channel* handle = (event_channel*)alloc_event_channel();

  // using one socket solution for the moment
  handle->channel = send_socket;
  handle->lat.min = lat.min;   
  handle->lat.max = lat.max;
  handle->per = requested_period;

  handle->sub = (char*)alloc_subject(strlen(event_name) + 1);
  strcpy(handle->sub, event_name);

  // printf("sizeof of proximity being copied %d\n", sizeof(*prox));

  handle->desired = (proximity*)alloc_buffer(sizeof(*prox));
  handle->actual = (proximity*)alloc_buffer(sizeof(*prox));

  memcpy(handle->desired, prox, sizeof(*prox));
  memcpy(handle->actual, prox, sizeof(*prox));

  return handle;
}

event_channel* reserve_channel_for_subscribe(subject sub){

  event_channel* handle = (event_channel*)alloc_event_channel();

  // using one socket solution for the moment
  handle->channel = send_socket;
  handle->lat.min = 0;   
  handle->lat.max = 0;
  handle->per = 0;

  handle->sub = (char*)alloc_subject(strlen(sub) + 1);
  strcpy(handle->sub, sub);

  handle->desired = 0;
  handle->actual = 0;

  return handle;

}

// do network stuff, then free memory
void free_channel(event_channel* handle){
  free(handle->sub);
  free(handle->desired);
  free(handle->actual);
  free(handle);
}

int send_on_channel(void* msg, event_channel* to){

  char* to_send;
 // printf("send_on_channel, subject - %s\n", ((steam_event*)msg)->event.sub);
 // printf("send_on_channel, content - %s\n", ((steam_event*)msg)->event.cont);
 // printf("send_on_channel, proximity_type - %d\n", ((steam_event*)msg)->event.proximity_type);
  
  to_send = (char*)marshal_steam_event((steam_event*)msg);
  if(sendto(send_socket, to_send,
	    strlen(to_send), 0, (struct sockaddr*)&sear_bcast_addr, sizeof(struct sockaddr)) == -1){
    perror("sendto in send_on_channel\n");
    return -1;
  }
  free(to_send);
  return 0;
}

//added by sean , doesn't work not sure what to does ........
int send_on_local_channel(void* msg){
  int ttl;
  int size = sizeof(ttl);
  int response;
  if(response= getsockopt(local_send_socket, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, &size) < 0){
      perror("getting the time to live ");
    }
  //printf("the ttl is coming back as %i \n", ttl);
  // printf("the local send socket is %i \n", local_send_socket);



  char* to_send;
 // printf("send_on_channel, subject - %s\n", ((steam_event*)msg)->event.sub);
 // printf("send_on_channel, content - %s\n", ((steam_event*)msg)->event.cont);
 // printf("send_on_channel, proximity_type - %d\n", ((steam_event*)msg)->event.proximity_type);
  
  to_send = (char*)marshal_steam_event((steam_event*)msg);
  if(sendto(local_send_socket, to_send,
	    strlen(to_send), 0, (struct sockaddr*)&local_sear_bcast_addr, sizeof(struct sockaddr)) == -1){
    perror("sendto in send_on_local_channel\n");
    return -1;
  }
  free(to_send);
  return 0;
}

int register_cb_for_recv_from_channel(receive_event cb, event_channel* from){
}


#ifndef _dummy_sear_impl_h_
#define _dummy_sear_impl_h_

#include "dummy-sear.h"

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/select.h>

/*
 * Max number of channels rte is willing to provide
 */
#define MAX_CHANNELS 8

#define DUMMY_SEAR_PORT 55555

//#define DUMMY_SEAR_BCAST_ADDR "192.168.69.255"
//#define DUMMY_SEAR_BCAST_ADDR "192.168.10.255"
//#define DUMMY_SEAR_BCAST_ADDR "192.168.1.100"
//#define DUMMY_SEAR_BCAST_ADDR "134.226.36.255"
#define DUMMY_SEAR_BCAST_ADDR "192.168.10.1"
//#define DUMMY_SEAR_BCAST_ADDR "127.0.0.255"
//#define LOCAL_BCAST_ADDR "127.0.0.255"
//#define LOCAL_BCAST_ADDR "134.226.36.255"
#define MULTICAST_GROUP "225.0.0.22"
#define LOCAL_INTERFACE_ADDR  "127.0.0.1" 
#define LISTEN_BACKLOG 10
#define MAX_BUFFER 5000


pthread_t select_t_id;
pthread_t recv_t_id;

#endif

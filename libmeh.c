#include "libmeh.h"
#include "swing.h"

/*
  can't add comments to generic_stream so I'll add them here


  Need to sort out a policy for adding values, maybe make a copy and ensure that the person
  who mallocs them deletes them... might make most sense, but very ineffecient for large values.
 */

//STREAM_FUNCTIONS(float)
//STREAM_FUNCTIONS(int)
//STREAM_FUNCTIONS(double)
//STREAM_FUNCTIONS(point)
//STREAM_FUNCTIONS(quaternion)
//STREAM_FUNCTIONS(vector)
STREAM_FUNCTIONS(accel_vector)
//STREAM_FUNCTIONS(swing)
//STREAM_FUNCTIONS(ball_contact)
//STREAM_FUNCTIONS(load_sensor)
//STREAM_FUNCTIONS(stance)
STREAM_FUNCTIONS(emergency)
void start_meh(multi_event_handler* meh){
  meh->ready = 1;
}
multi_event_handler* create_multi_event_handler(int num_inputs,stream* inputs[],stream* output,void (*func)(), int (*e_policy)()){
  multi_event_handler* meh_handler = malloc(sizeof(multi_event_handler));
  meh_handler->num_inputs =num_inputs;
  meh_handler -> inputs = inputs;
  meh_handler -> output = output;
  meh_handler -> func = func;
  meh_handler -> initialised = 0;
  meh_handler -> ready = 0;
 meh_handler -> execution_policy = e_policy;
  //struct listhead_funct* l = init_if_func();
  //register_multi_event_handler(l,meh_handler);
  // start_multi_event_handler(l);
  return meh_handler;

}
/*
  Don't think I use this
 */
stream* find_stream_by_name(multi_event_handler* meh_handler,char* name){
  /* names are unique for streams. So I can just go through all the streams
     associated with this interval function and find the one with the name
     which corresponds with the provided one.

   */
  int c;
  for(c=0;c<meh_handler -> num_inputs; c++){
    if(strcmp(meh_handler->inputs[c] -> name,name)){
      return meh_handler -> inputs[c];
    }
  }

  printf("Stream with the required name not found \n");
  exit(0);




}
int register_multi_event_handler(struct listhead_funct *head_ptr,multi_event_handler *ifunc){


  // Lists are my dynamic data structure of choice

  struct if_list_entry *new = malloc(sizeof(struct if_list_entry));
  new-> ifunc = ifunc;

  LIST_INSERT_HEAD(head_ptr, new, entries);
  // queue_add(function_queue,if_func);
}

/*
  Initialises the linked list for the interval functions.
 */
struct listhead_funct* init_if_func(){
  struct listhead_funct *new_head = malloc(sizeof(struct listhead_funct));
  LIST_INIT(new_head);
  return new_head;
}


/* int meh_handler_close(){ */
/*   /\* need to go through the list and destroy the stuff I've allocated dynamically *\/ */
/* /\* and then destroy the list itself. *\/ */


/* } */


/*
  create a thread for each interval function and lets them at it
 */
int start_multi_event_handler(struct listhead_funct* list){
  //don't use this 
  /* struct if_list_entry *np; */
  /* for (np = list->lh_first; np != NULL; np = np->entries.le_next){ */
  /*   pthread_t new_thread; */
  /*   pthread_create(&new_thread,NULL,do_meh_handler,np->ifunc); */
  /* } */
  

}

/*
  iterates through an array of streams and if the status of any of them is not ready returns false otherwise returns true.
 */

int and_of_all_array(stream* s[],int length){
  int c;
  for( c = 0;c < length; c++){
    if (! s[c] -> status){

      return 0;
    }
  }
    printf("\n I've got to %d , so everybody must be ready \n",c);
    //    my_if -> inputs[0] ->get_value((stream*)  (my_if -> inputs[0])
    printf("the values are %f , %f , %f \n", s[0]->get_value(s[0]), s[1]->get_value(s[1]),s[2]->get_value(s[2]));
    return 1;
}

/*
  Checks the individual policy of the streams to see if they are OR. At the end
  we check if all of the array is AND.
 */
/*int individual_policy(stream* s[],int length){
  int c;
  for (c= 0; c< length; c++){
    if(s[c] -> timing_policy == GREEDY && s[c] -> status ){
      return 1;
    }
  }
  return and_of_all_array(s,length);   // if we've got this far its possible that all of the timing policies are AND so better check that
}
int or_of_all_array(stream* s[],int length){
  int c;
  for( c = 0;c < length; c++){
    if ( s[c] -> status){
      return 1;
    }
  }
    return 0;
}*/
/*
  Checks the input streams. If they don't all have at least one value in them we don't proceed
 */

int check_initial_values(multi_event_handler* my_if){
  int c;
  if(! (my_if -> ready)){    //check to make sure that the streams exist
    return;
  }
  for(c=0;c<my_if -> num_inputs; c++){
    if(my_if -> inputs[c] -> length ==0){
      my_if ->initialised = 0;
      return ;
    }
  }
  my_if -> initialised =1;
}
/*
  checks if the interval function has all the data it needs to execute according to the timing policy
 */
int ready_to_execute(multi_event_handler* my_if){
  if (!my_if-> initialised ){
    check_initial_values(my_if);
    if (!my_if -> initialised){
      return 0;
    }
  }
//  if (my_if -> timing_policy == GREEDY){
//    return  or_of_all_array(my_if->inputs,my_if -> num_inputs);
//  }
//  if(my_if -> timing_policy == INDIVIDUAL){
//    return individual_policy(my_if -> inputs,my_if->num_inputs);
//}
//  if(my_if -> timing_policy == LAZY){
//    return and_of_all_array(my_if -> inputs,my_if -> num_inputs);
//  }
	return my_if -> execution_policy();
	
  //printf("Error: timing policy not recognised, exiting... \n");
  //exit(0);
}
  int do_meh_handler(multi_event_handler *my_if){

      if(( ready_to_execute(my_if) ) ){
	void* v =NULL ;
	void* oldv = NULL;
	v = calloc(1,sizeof(LARGEST_DATA_TYPE)); //not ideal obviously if this is very large
	oldv= calloc(1,sizeof(LARGEST_DATA_TYPE));
	memcpy(oldv,v,sizeof(LARGEST_DATA_TYPE));
	printf("oldv is %i \n",oldv);
	
	void (*function_ptr)();
	void (*function_ptr1)(void*,void*);
	void (*function_ptr2)(void*,void*,void*);
	void (*function_ptr3)(void*,void*,void*,void*);
	void (*function_ptr4)(void*,void*,void*,void*,void*);
	void (*function_ptr5)(void*,void*,void*,void*,void*,void*);

	printf("the number of inputs is %i \n", my_if->num_inputs);
	switch (my_if -> num_inputs) {
	case 0:
	  printf("case 1 \n");
	  function_ptr= (void (*)() )my_if -> func;
	  my_if -> func();
	  break;
	case 1:
	  function_ptr1 =  ((void (*)(void*,void*) )my_if -> func);
	  function_ptr1((my_if -> inputs[0] ->get_value((stream*)  (my_if -> inputs[0]))),(void*)v );
	  break;
	case 2:   //not tested
	  function_ptr2 =  ((void (*)(void*,void*,void*) )my_if -> func);
	  function_ptr2((my_if -> inputs[0] ->get_value((stream*)  (my_if -> inputs[0]))),(my_if -> inputs[1] ->get_value((stream*)  (my_if -> inputs[1]))),(void*)v );
	  printf("about to break \n" );
	  break;

	case 3:  //not tested
	  printf("case 3 \n");
	  function_ptr3 =  ((void (*)(void*,void*,void*,void*) )my_if -> func);
	  function_ptr3((my_if -> inputs[0] ->get_value((stream*)  (my_if -> inputs[0]))),(my_if -> inputs[1] ->get_value((stream*)  (my_if -> inputs[1]))),(my_if -> inputs[2] ->get_value((stream*)  (my_if -> inputs[2]))),(void*)v );

	  break;
	case 4:
	  printf("case 4 , not tested use at your own risk \n");
	  function_ptr4 =  ((void (*)(void*,void*,void*,void*,void*) )my_if -> func);
	  function_ptr4((my_if -> inputs[0] ->get_value((stream*)  (my_if -> inputs[0]))),(my_if -> inputs[1] ->get_value((stream*)  (my_if -> inputs[1]))),(my_if -> inputs[2] ->get_value((stream*)  (my_if -> inputs[2]))),(my_if -> inputs[3] ->get_value((stream*)  (my_if -> inputs[3]))),(void*)v );


	  break;
	case 5:  //not tested
	  printf("case 5 \n");
	  function_ptr5 =  ((void (*)(void*,void*,void*,void*,void*,void*) )my_if -> func);
	  function_ptr5((my_if -> inputs[0] ->get_value((stream*)  (my_if -> inputs[0]))),(my_if -> inputs[1] ->get_value((stream*)  (my_if -> inputs[1]))),(my_if -> inputs[2] ->get_value((stream*)  (my_if -> inputs[2]))),(my_if -> inputs[3] ->get_value((stream*)  (my_if -> inputs[3]))),(my_if -> inputs[4] ->get_value((stream*)  (my_if -> inputs[4]))),(void*)v );

	  break;
	
	default:
	  printf("Too many paramters to interval function, 5 is the max supported at this time \n");
	  exit(0);
	}
	printf("new v is %i \n",v);
		if(memcmp(v,oldv,sizeof(LARGEST_DATA_TYPE))  ) {
		  printf("the contents of the memory have changed \n");
		  (my_if->output)->add_value(v,my_if->output);   //add to the output
	}
	free(v);
	free(oldv);



  }

}



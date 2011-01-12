#ifndef G_STREAM
#define G_STREAM
#include "steam_interface.h"
#include "libmeh.h"

enum e_status {WAITING,READY};     //  WAITING ==0 , READY ==1


//should probably divy this back up into input and output streams now that I've got specific output bits
typedef struct _stream stream;
typedef struct   {
  //  void (*func)(void*,void*,void*);
  void (*func)();
  int num_inputs;
  stream *output;
  stream** inputs;
  int (*execution_policy)() ;  //function pointer to the execution policy. Called for every new event
  int initialised;
  int ready;
} multi_event_handler;

 struct _stream {
   char* name;   //the name of the stream, used by STEAM
  int length;
  int max_length;
  char *event_subject;
  enum  e_status status;
   channel_id output_channel;   //should be part of an ouput stream, not used for input streams
   event* output_event;
   multi_event_handler* meh;
  int (*add_value)(void*,stream*);
   void* (*get_value)(stream*);
 } ;

#define	STREAM(STREAM_TYPE)						\
  struct								\
  STREAM_TYPE##_stream_entry {						\
    TAILQ_ENTRY(STREAM_TYPE##_stream_entry) entries;			\
    STREAM_TYPE value;							\
  };									\
  									\
  typedef struct  {							\
    stream s;								\
    struct STREAM_TYPE##_listhead *head;					\
    struct STREAM_TYPE##_stream_entry *last;				\
  } STREAM_TYPE##_stream;							\
  STREAM_TYPE##_stream*  create_##STREAM_TYPE##_input_stream(int,char*,void (*)(event*),multi_event_handler* meh); \
  STREAM_TYPE##_stream*  create_##STREAM_TYPE##_output_stream(int,char*);	\
  STREAM_TYPE##_stream* create_##STREAM_TYPE##_stream(int,char*);		\
  int add_##STREAM_TYPE##_value(void*,STREAM_TYPE##_stream*);		\
  int add_##STREAM_TYPE##_value_to_input_stream(void*,STREAM_TYPE##_stream*);	\
  int print_##STREAM_TYPE##_stream(STREAM_TYPE##_stream*,void (*print_##STREAM_TYPE)( STREAM_TYPE)); \
  int cleanup_##STREAM_TYPE##_stream(STREAM_TYPE##_stream*);		\
  STREAM_TYPE * get_##STREAM_TYPE##_value(STREAM_TYPE##_stream*);		\
  int output_##STREAM_TYPE##_val(void *, STREAM_TYPE##_stream*);		\
  STREAM_TYPE* parse_##STREAM_TYPE(event*);					\
  void parse_##STREAM_TYPE##_event(event*,STREAM_TYPE##_stream* s);			\
  TAILQ_HEAD(STREAM_TYPE##_listhead, STREAM_TYPE##_stream_entry) STREAM_TYPE##_stream_head; \

#define SETUP_STREAM(STREAM_TYPE,STREAM)\
  void parse_event_##__COUNTER__ (event *ev){\
  parse_##STREAM_TYPE##_event(ev,STREAM);\
}\

#define STREAM_FUNCTIONS(STREAM_TYPE)				\
  STREAM_TYPE##_stream*  create_##STREAM_TYPE##_stream(int length,char* name){	\
									\
  struct STREAM_TYPE##_listhead *new_head = malloc(sizeof(struct STREAM_TYPE##_listhead));\
  STREAM_TYPE##_stream *s = malloc(sizeof(STREAM_TYPE##_stream));		\
  ((stream*)s) ->max_length = length;					\
  ((stream*)s) ->length = 0;						\
  TAILQ_INIT(new_head);							\
  s -> head = new_head;							\
  ((stream*)s) -> status = WAITING;					\
  ((stream*)s) -> name= name;						\
  ((stream*)s)->get_value = get_##STREAM_TYPE##_value;			\
  return s;								\
  }									\
  STREAM_TYPE##_stream*  create_##STREAM_TYPE##_input_stream(int length,char* name,void (*cb)(event*),multi_event_handler* owner){ \
    STREAM_TYPE##_stream* s =  create_##STREAM_TYPE##_stream(length,name);	\
    event* my_input_event_s1;						\
    my_input_event_s1= alloc_event();					\
    subscribe_to_event(name,my_input_event_s1,cb);			\
    ((stream*)s) -> meh = owner;					\
  ((stream*)s)->add_value =  add_##STREAM_TYPE##_value_to_input_stream;		\
  free(my_input_event_s1);						\
    return s;								\
									\
  }									\
									\
  STREAM_TYPE##_stream*  create_##STREAM_TYPE##_output_stream(int length,char* name){	\
  STREAM_TYPE##_stream* s =  create_##STREAM_TYPE##_stream(length,name);	\
  char* subject1=((stream*)s)->name;					\
  event*  event1= alloc_event();									\
  channel_id channel_1 = setup_event(subject1,LOCAL,event1);		\
  ((stream*)s) -> output_event =event1;					\
  ((stream*)s)-> output_channel = channel_1;					\
((stream*)s) -> add_value = output_##STREAM_TYPE##_val;			\
  return s;								\
									\
  }									\
									\
  void parse_##STREAM_TYPE##_event(event* ev,STREAM_TYPE##_stream* s){			\
    STREAM_TYPE* val = parse_##STREAM_TYPE(ev);			\
    if (val != NULL){							\
      add_##STREAM_TYPE##_value_to_input_stream(val,s);					\
    }\
  }									\
									\
									\
									\
 int add_##STREAM_TYPE##_value_to_input_stream(void *i,STREAM_TYPE##_stream *s){		\
STREAM_TYPE* v_ptr = malloc(sizeof( STREAM_TYPE ));			\
 memcpy(v_ptr,i, sizeof( STREAM_TYPE ));				\
 STREAM_TYPE v = *(( STREAM_TYPE *)v_ptr);					\
 free(v_ptr);								\
 struct STREAM_TYPE##_listhead *h = s->head;				\
 struct STREAM_TYPE##_stream_entry *n1= malloc(sizeof(struct STREAM_TYPE##_stream_entry)); \
 if(n1 == NULL){							\
   return -1 ;								\
 }									\
 n1 -> value = v;							\
 TAILQ_INSERT_HEAD(h, n1, entries);					\
 ((stream*)s) -> status = READY;					\
 ((stream*)s)->length ++;						\
 if (((stream*)s)->length > ((stream*)s)->max_length){			\
   struct STREAM_TYPE##_stream_entry *last;				\
   last  = TAILQ_LAST(h,STREAM_TYPE##_listhead);				\
   TAILQ_REMOVE(h,last,entries);					\
   free(last);							\
   ((stream*)s)->length --;						\
 }									\
 do_meh_handler( ((stream *)s)-> meh);					\
 return 0;								\
 }									 \
 int add_##STREAM_TYPE##_value(void *i,STREAM_TYPE##_stream *s){		\
   STREAM_TYPE v = *(( STREAM_TYPE *)i);					\
 struct STREAM_TYPE##_listhead *h = s->head;				\
 struct STREAM_TYPE##_stream_entry *n1= malloc(sizeof(struct STREAM_TYPE##_stream_entry)); \
 if(n1 == NULL){							\
   return -1 ;								\
   printf("cannot alloc memory, exiting\n");				\
 }									\
 n1 -> value = v;							\
 TAILQ_INSERT_HEAD(h, n1, entries);					\
 ((stream*)s) -> status = READY;					\
 ((stream*)s)->length ++;						\
 if (((stream*)s)->length > ((stream*)s)->max_length){			\
   struct STREAM_TYPE##_stream_entry *last;				\
   last  = TAILQ_LAST(h,STREAM_TYPE##_listhead);				\
   TAILQ_REMOVE(h,last,entries);					\
   free(last);								\
   ((stream*)s)->length --;						\
 }									\
 return 0;								\
}									\
									\
/* cleans up all the structures and memory for a stream\
   basically cleans up all the shizzle allocated in this lib for the stream\
*/									\
									\
int cleanup_##STREAM_TYPE(STREAM_TYPE##_stream *s){				\
									\
  struct STREAM_TYPE##_stream_entry *item;				\
  while ((item = TAILQ_FIRST(s->head))) {				\
    TAILQ_REMOVE(s->head, item, entries);				\
    free(item);								\
  }									\
									\
  /* The tail queue should now be empty. */				\
  if (!TAILQ_EMPTY(s->head)){						\
    printf("tail queue is NOT empty!\n");				\
    return -1;								\
  }									\
  free(s->head);							\
  free(s);								\
  return 0;								\
}									\
									\
									\
									\
									\
									\
STREAM_TYPE* get_##STREAM_TYPE##_value(STREAM_TYPE##_stream *s){		\
									\
  struct STREAM_TYPE##_stream_entry *np;					\
  struct STREAM_TYPE##_listhead *head = s -> head;			\
  ((stream*)s)->status = WAITING;					\
  np =  head->tqh_first;						\
  return &(np->value);							\
									\
}									\
/* /\*\ */								\
/*   Prints the stream given by s, needs a function that knows how to print it\ */					\
/*  *\/\ */								\
 int print_##STREAM_TYPE##_stream(STREAM_TYPE##_stream *s,void (*print_##STREAM_TYPE)( STREAM_TYPE item)){ \
									\
									\
   printf ( ":::: outputing stream ::::: \n");				\
  struct STREAM_TYPE##_stream_entry *np;					\
  struct STREAM_TYPE##_listhead *head = s -> head;			\
  int count = 0;							\
  for (np = head->tqh_first; np != NULL; np = np->entries.tqe_next, count ++){ \
    printf(" number %i ",count);					\
    print_##STREAM_TYPE(np->value);					\
  }								\
								\
  printf(":::::::::::::::::::::::::::: \n");			\
  return 0;							\
 }								\
								\
int output_##STREAM_TYPE##_val(void *i, STREAM_TYPE##_stream *s){	\
add_##STREAM_TYPE##_value(i,s);					\
								\
 char* str = NULL;							\
  str = encode_base64(sizeof(STREAM_TYPE) ,(char*)i);		\
  printf("sending %s \n", str);					\
  printf("the size of the message is %d",strlen(str));		\
  send_my_event(str,((stream*)s)->output_channel,((stream*)s)->output_event); \
  free(str);								\
								\
}								\
STREAM_TYPE* parse_##STREAM_TYPE(event *ev){	\
  if(strcmp(ev->cont,"announcement") == 0){	\
    printf("announce received and ignored \n");	\
    return NULL;				\
  }						\
  STREAM_TYPE* val;				\
  char* dest = malloc( sizeof(STREAM_TYPE));	\
  if (!decode_base64(dest,ev->cont)){		\
    printf("base64 decode failed\n");		\
    exit(0);					\
  }						\
  val = (STREAM_TYPE*) dest;			\
  free(dest);					\
return val;					\
}
#endif

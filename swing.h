#include "generic_stream.h"
#pragma once
#ifndef H_SWING
#define H_SWING

#define SWING_SUBJECT "SWING"


// defines a swing, can have value of START or END

typedef struct {  
  char  message[20];
} swing;


STREAM(swing)


#endif

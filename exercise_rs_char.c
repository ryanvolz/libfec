#include "char.h"
#include "rs-common.h"
#include "exercise.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE_RS encode_rs_char
#define DECODE_RS decode_rs_char

void exercise_rs_char(void *p, int trials){
  struct rs *rs = (struct rs *)p;

#include "exercise_rs.h"

}

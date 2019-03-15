#include "int.h"
#include "rs-common.h"
#include "exercise.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE_RS encode_rs_int
#define DECODE_RS decode_rs_int

void exercise_rs_int(void *p, int trials){
  struct rs *rs = (struct rs *)p;

#include "exercise_rs.h"

}

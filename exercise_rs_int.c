#include "int.h"
#include "rs-common.h"
#include "exercise.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE_RS encode_rs_int
#define DECODE_RS decode_rs_int

int exercise_rs_int(void *p, int trials, int verbose){
  struct rs *rs = (struct rs *)p;
  int len = NN - PAD;

#include "exercise_rs.h"

}

int exercise_rs_bc_int(void *p, int trials, int verbose){
  struct rs *rs = (struct rs *)p;
  int len = NN - PAD;

#include "exercise_rs_bc.h"

}

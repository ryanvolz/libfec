#include "char.h"
#include "rs-common.h"
#include "exercise.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE_RS encode_rs_char
#define DECODE_RS decode_rs_char

int exercise_rs_char(void *p, int trials, int verbose){
  struct rs *rs = (struct rs *)p;
  int len = NN - PAD;

#include "exercise_rs.h"

}

int exercise_rs_bc_char(void *p, int trials, int verbose){
  struct rs *rs = (struct rs *)p;
  int len = NN - PAD;

#include "exercise_rs_bc.h"

}

/*
int exercise_rs_bc1_char(void *p, int trials, int verbose){
  struct rs *rs = (struct rs *)p;

#include "exercise_rs_bc1.h"

}
*/

#include "fixed.h"
#include "rs-common.h"
#include "exercise.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIXED
#define ENCODE_RS encode_rs_8
#define DECODE_RS decode_rs_8

int exercise_rs_8(int pad, int trials, int verbose){
  int len = NN - pad;

#include "exercise_rs.h"

}

int exercise_rs_bc_8(int pad, int trials, int verbose)
{
  int len = NN - pad;

#include "exercise_rs_bc.h"

}

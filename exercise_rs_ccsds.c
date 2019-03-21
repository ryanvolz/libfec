#include "ccsds.h"
#include "exercise.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CCSDS
#define ENCODE_RS encode_rs_ccsds
#define DECODE_RS decode_rs_ccsds

int exercise_rs_ccsds(int pad, int trials, int verbose){
  int len = NN - pad;

#include "exercise_rs.h"

}

int exercise_rs_bc_ccsds(int pad, int trials, int verbose){
  int len = NN - pad;

#include "exercise_rs_bc.h"

}

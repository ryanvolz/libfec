/* Test the Reed-Solomon codecs
 * for various block sizes and with random data and random error patterns
 *
 * Copyright 2002 Phil Karn, KA9Q
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 *
 * Modified in 2019 by Ferdinand Blomqvist to support more thorough testing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include "exercise.h"

#define VERBOSITY_LEVEL VERBOSE_NO

struct etab {
  int symsize;
  int genpoly;
  int fcs;
  int prim;
  int nroots;
  int ntrials;
} Tab[] = {
  {2, 0x7,     1,   1, 1, 100000 },
  {3, 0xb,     1,   1, 2, 100000 },
  {3, 0xb,     1,   1, 3, 100000 },
  {3, 0xb,     1,   1, 4, 100000 },
  {4, 0x13,    1,   1, 4, 100000 },
  {5, 0x25,    1,   1, 6, 10000 },
  {6, 0x43,    1,   1, 8, 10000 },
  {7, 0x89,    1,   1, 10, 1000 },
  {8, 0x11d,   1,   1, 32, 100 },
  {8, 0x187,   112,11, 32, 100 }, /* Duplicates CCSDS codec */
  {9, 0x211,   1,   1, 32, 100 },
  {10,0x409,   1,   1, 32, 100 },
  {11,0x805,   1,   1, 32, 100 },
  {12,0x1053,  1,   1, 32, 50 },
  {12,0x1053,  1,   1, 64, 50 },
  {13,0x201b,  1,   1, 32, 20 },
  {13,0x201b,  1,   1, 64, 20 },
  {14,0x4443,  1,   1, 32, 10 },
  {14,0x4443,  1,   1, 64, 10 },
  {15,0x8003,  1,   1, 32, 5 },
  {15,0x8003,  1,   1, 64, 5 },
  {16,0x1100b, 1,   1, 32, 5 },
  {0, 0, 0, 0, 0, 0},
};

#define PADS 5
double Pad[PADS] = {0, 0.25, 0.5, 0.75, 1.0};

typedef void* (*IF)(int, int, int, int, int, int);
typedef void (*FF)(void*);
typedef int (*EF)(void*, int, int);
typedef int (*E8F)(int, int, int);

static int run_exercise(struct etab *e, IF init_f, FF free_f, EF ex_uc, EF ex_bc);
static void run_exercise_8(int trials, E8F ex_uc, E8F ex_bc);

int main(){
  int i;

  srandom(time(NULL));

  printf("Testing fixed CCSDS encoder/decoder...\n");
  run_exercise_8(50, exercise_rs_8, exercise_rs_bc_8);

  printf("Testing fixed CCSDS dual representation encoder/decoder...\n");
  run_exercise_8(50, exercise_rs_ccsds, exercise_rs_bc_ccsds);

  printf("Testing general RS encoder/decoder...\n");
  for(i=0;Tab[i].symsize != 0;i++) {
    if(Tab[i].symsize <= 8){
      run_exercise(&Tab[i], init_rs_char, free_rs_char,
		  exercise_rs_char, exercise_rs_bc_char);
    }
    else {
      run_exercise(&Tab[i], init_rs_int, free_rs_int,
		  exercise_rs_int, exercise_rs_bc_int);
    }
  }
  exit(0);
}

static const char* desc_uc = "Testing up to error correction capacity...";
static const char* desc_bc = "Testing beyond error correction capacity...";
static const char* err_msg_uc = "decoding failures!";
static const char* err_msg_bc = "silent failures!";

static void helper(void* rs, int trials, EF func,
	    const char* description, const char* text)
{
  printf("%s%s", description, VERBOSITY_LEVEL ? "\n" : "");
  fflush(stdout);

  int errors = func(rs, trials, VERBOSITY_LEVEL);
  if(errors)
    printf("%s%d %s\n", VERBOSITY_LEVEL ? "" : " FAIL\n", errors, text);
  else if(!VERBOSITY_LEVEL)
    printf(" OK\n");
}

static void helper_8(int pad, int trials, E8F func,
	    const char* description, const char* text)
{
  printf("%s%s", description, VERBOSITY_LEVEL ? "\n" : "");
  fflush(stdout);

  int errors = func(pad, trials, VERBOSITY_LEVEL);
  if(errors)
    printf("%s%d %s\n", VERBOSITY_LEVEL ? "" : " FAIL\n", errors, text);
  else if(!VERBOSITY_LEVEL)
    printf(" OK\n");

}

static void run_exercise_8(int trials, E8F ex_uc, E8F ex_bc)
{
  /* max_pad = NN - NROOTS - 1 */
  int max_pad = 255 - (255 - 223) - 1;
  int prev_pad = -1;

  for(int i = 0; i < PADS; i++){
    int pad = Pad[i] * max_pad;
    if(pad == prev_pad)
      continue;

    printf("Testing (%d,%d)_256 code...\n",255-pad,223-pad);
    helper_8(pad, trials, ex_uc, desc_uc, err_msg_uc);
    helper_8(pad, trials, ex_bc, desc_bc, err_msg_bc);
  }
}

static int run_exercise(struct etab *e, IF init_f, FF free_f, EF ex_uc, EF ex_bc){
  void *rs;

  int nn = (1<<e->symsize) - 1;
  int kk = nn - e->nroots;
  int max_pad = kk - 1;
  int prev_pad = -1;

  for(int i = 0; i < PADS; i++){
    int pad = Pad[i] * max_pad;
    if(pad == prev_pad)
      continue;

    prev_pad = pad;
    printf("Testing (%d,%d)_%d code...\n",nn-pad,kk-pad,nn+1);

    rs = init_f(e->symsize,e->genpoly,e->fcs,e->prim,e->nroots,pad);
    if(rs == NULL){
      printf("init_rs_char failed!\n");
      return -1;
    }

    helper(rs, e->ntrials, ex_uc, desc_uc, err_msg_uc);
    helper(rs, e->ntrials, ex_bc, desc_bc, err_msg_bc);

    free_f(rs);
  }
  return 0;
}

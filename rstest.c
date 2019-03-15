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


struct etab {
  int symsize;
  int genpoly;
  int fcs;
  int prim;
  int nroots;
  int ntrials;
} Tab[] = {
  {2, 0x7,     1,   1, 1, 10 },
  {3, 0xb,     1,   1, 2, 10 },
  {4, 0x13,    1,   1, 4, 10 },
  {5, 0x25,    1,   1, 6, 10 },
  {6, 0x43,    1,   1, 8, 10 },
  {7, 0x89,    1,   1, 10, 10 },
  {8, 0x11d,   1,   1, 32, 10 },
  {8, 0x187,   112,11, 32, 10 }, /* Duplicates CCSDS codec */
  {9, 0x211,   1,   1, 32, 10 },
  {10,0x409,   1,   1, 32, 10 },
  {11,0x805,   1,   1, 32, 10 },
  {12,0x1053,  1,   1, 32, 5 },
  {13,0x201b,  1,   1, 32, 2 },
  {14,0x4443,  1,   1, 32, 1 },
  {15,0x8003,  1,   1, 32, 1 },
  {16,0x1100b, 1,   1, 32, 1 },
  {0, 0, 0, 0, 0},
};

int run_exercise_char(struct etab *e, int trials);
int run_exercise_int(struct etab *e, int trials);

int main(){
  int i;

  srandom(time(NULL));

  printf("Testing fixed CCSDS encoder...\n");
  exercise_rs_8(10);
  for(i=0;Tab[i].symsize != 0;i++){
    int nn,kk;

    nn = (1<<Tab[i].symsize) - 1;
    kk = nn - Tab[i].nroots;
    printf("Testing (%d,%d) code...\n",nn,kk);
    if(Tab[i].symsize <= 8)
      run_exercise_char(&Tab[i], Tab[i].ntrials);
    else
      run_exercise_int(&Tab[i], Tab[i].ntrials);
  }
  exit(0);
}

int run_exercise_char(struct etab *e, int trials){
  void *rs;

  rs = init_rs_char(e->symsize,e->genpoly,e->fcs,e->prim,e->nroots,0);
  if(rs == NULL){
    printf("init_rs_char failed!\n");
    return -1;
  }

  exercise_rs_char(rs, trials);
  free_rs_char(rs);
  return 0;
}

int run_exercise_int(struct etab *e, int trials){
  void *rs;

  rs = init_rs_int(e->symsize,e->genpoly,e->fcs,e->prim,e->nroots,0);
  if(rs == NULL){
    printf("init_rs_char failed!\n");
    return -1;
  }

  exercise_rs_int(rs, trials);
  free_rs_int(rs);
  return 0;
}

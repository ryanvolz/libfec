/* exercise.h
 * Copyright 2019, Fedinand Blomqvist
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */
#ifndef _FEC_EXERCISE_H_
#define _FEC_EXERCISE_H_

#include "fec.h"

#define VERBOSE_NO 0
#define VERBOSE_SUMMARY 1
#define VERBOSE_VERY 2

/* Convenience macros for printing arrays of data. They are used by
 * exercise_rs.h and exercise_rs_bc.h, and are unsafe to use otherwise since
 * they assume that the variable i is declared and that modifying it is not
 * having any adverse effect.
 */
#define print_vec(desc, vec, len, fmt) \
  do {\
    printf((desc)); for(i=0;i<(len);i++){ printf((fmt), (vec)[i]); } printf("\n");\
  } while(0)

#define print_vec_xor(desc, a, b, len, fmt) \
  do {\
    printf((desc)); for(i=0;i<(len);i++){ printf((fmt), (a)[i] ^ (b)[i]); } printf("\n");\
  } while(0)

int exercise_rs_8(int pad, int trials, int verbose);
int exercise_rs_ccsds(int pad, int trials, int verbose);
int exercise_rs_char(void* p, int trials, int verbose);
int exercise_rs_int(void* p, int trials, int verbose);

int exercise_rs_bc_8(int pad, int trials, int verbose);
int exercise_rs_bc_ccsds(int pad, int trials, int verbose);
int exercise_rs_bc_char(void *p, int trials, int verbose);
int exercise_rs_bc_int(void *p, int trials, int verbose);

#endif /* _FEC_EXERCISE_H_ */

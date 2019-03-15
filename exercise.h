/* exercise.h
 * Copyright 2019, Fedinand Blomqvist
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */
#ifndef _FEC_EXERCISE_H_
#define _FEC_EXERCISE_H_

#include "fec.h"

void exercise_rs_8(int trials);
void exercise_rs_ccsds(int trials);
void exercise_rs_char(void* p, int trials);
void exercise_rs_int(void* p, int trials);

#endif /* _FEC_EXERCISE_H_ */

/* The guts of the Reed-Solomon decoder tester, meant to be #included
 * into a function body with the following typedefs, macros and variables supplied
 * according to the code parameters:

 * data_t - a typedef for the data symbol
 * NROOTS - the number of roots in the RS code generator polynomial,
 *          which is the same as the number of parity symbols in a block.
            Integer variable or literal.
 * NN - the total number of symbols in a RS block. Integer variable or literal.
 * ENCODE_RS - the encoding function for the RS code.
 * DECODE_RS - the decoding function for the RS code.
 *
 * The memset(), memcmp(), memcpy(), printf() and random() functions are used.
 * The appropriate header file declaring these functions (usually <string.h>,
 * <stdio.h>, <stdlib.h>) must be included by the calling program.
 */

/* Exercise an RS codec a specified number of times using random
 * data and error patterns
 *
 * Copyright 2002 Phil Karn, KA9Q
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 *
 * Modified by Ferdinand Blomqvist in order to be included in funtion bodies.
 * This was done to reduce code duplication.
 */

#if !defined(NROOTS)
#error "NROOTS not defined"
#endif

#if !defined(NN)
#error "NN not defined"
#endif

#if !defined(ENCODE_RS)
#error "ENCODE_RS not defined"
#endif

#if !defined(DECODE_RS)
#error "DECODE_RS not defined"
#endif

{
  data_t block[len],tblock[len],rblock[len];
  int i, j;
  int errors;
  int errlocs[len];
  int derrlocs[NROOTS];
  int derrors;
  int errval,errloc;
  int erasures;
  int decodes_wrong = 0;
  int incorrect_ret_val = 0;
  int wrong_error_position = 0;
  int num_words = 0;

  for(errors=0;errors<=NROOTS/2; errors++){
    for(erasures=0;erasures<=NROOTS-2*errors; erasures++){
      for(j=0; j < trials; j++){
	/* Load block with random data and encode */
	for(i=0;i<len-NROOTS;i++)
	  block[i] = random() & NN;

#if defined(CCSDS) || defined(FIXED)
	ENCODE_RS(&block[0],&block[len-NROOTS],pad);
#else
	ENCODE_RS(rs,&block[0],&block[len-NROOTS]);
#endif

	/* Make temp copy, seed with errors */
	memcpy(tblock,block,sizeof(tblock));
	memset(errlocs,0,sizeof(errlocs));
	memset(derrlocs,0,sizeof(derrlocs));

	/* Generating random errors */
	for(i=0;i<errors;i++){
	  do {
	    errval = random() & NN;
	  } while(errval == 0); /* Error value must be nonzero */

	  do {
	    errloc = random() % len;
	  } while(errlocs[errloc] != 0); /* Must not choose the same location twice */

	  errlocs[errloc] = 1;
	  tblock[errloc] ^= errval;
	}

	int nerrors = errors;
	int nerasures = 0;
	/* Generating random erasures */
	for(i=0;i<erasures;i++){
	  do {
	    errloc = random() % len;
	  } while(errlocs[errloc] != 0); /* Must not choose the same location twice */

	  derrlocs[i] = errloc;

	  if(random() & 1) {
	    /* Erasure with the symbol intact */
	    errlocs[errloc] = 2;
	    nerasures++;
	  }
	  else {
	    /* Erasure with corrupted symbol */
	    do {
	      errval = random() & NN;
	    } while(errval == 0); /* Error value must be nonzero */

	    errlocs[errloc] = 1;
	    tblock[errloc] ^= errval;
	    nerrors++;
	  }
	}

	if(verbose >= VERBOSE_VERY)
	  memcpy(rblock,tblock,sizeof(tblock));

	/* Decode the errored block */
#if defined(CCSDS) || defined(FIXED)
	derrors = DECODE_RS(tblock,derrlocs,erasures,pad);
#else
	derrors = DECODE_RS(rs,tblock,derrlocs,erasures);
#endif

	if(derrors != nerrors){
	  incorrect_ret_val++;
	  if(verbose >= VERBOSE_VERY){
	    printf("(%d,%d)_%d:",len,len-NROOTS,NN+1);
	    printf(" decoder says %d errors, true number is %d\n",derrors,nerrors);
	  }
	}
	for(i=0;i<derrors;i++){
	  if(errlocs[derrlocs[i]] != 1){
	    wrong_error_position++;
	    if(verbose >= VERBOSE_VERY){
	      printf("(%d,%d)_%d:",len,len-NROOTS,NN+1);
	      printf(" decoder indicates error in location %d without error\n",derrlocs[i]);
	    }
	  }
	}
	if(memcmp(tblock,block,sizeof(tblock)) != 0){
	  decodes_wrong++;
	  if(verbose >= VERBOSE_VERY){
	    printf("(%d,%d)_%d:",len,len-NROOTS,NN+1);
	    print_vec(" uncorrected errors\nc:", block, len, " %02x");
	    print_vec("r:", rblock, len, " %02x");
	    print_vec_xor("e:", rblock, block, len, " %02x");
	    print_vec("x:", tblock, len, " %02x");
	    print_vec("erasurelocs:", errlocs, len, " %d");
	  }
	}
      }
      num_words += trials;
    }
  }

  if(verbose >= VERBOSE_SUMMARY){
    printf("Decodes wrong: %d / %d\n", decodes_wrong, num_words);
    printf("Wrong return value: %d / %d\n", incorrect_ret_val, num_words);
    printf("Wrong error position: %d\n", wrong_error_position);
  }

  return decodes_wrong + wrong_error_position + incorrect_ret_val;
}

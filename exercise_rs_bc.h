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
 *
 * This version of the tester tests what happens when we have uncorrectable
 * errors.  The decoder should always return a codeword or give up and report
 * that the decoding failed. In order to ensure this we test for silent
 * failures, i.e., cases where the decoder reports success but the returned
 * word is not a codeword.
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
  int non_codeword = 0;
  int num_words = 0;
  int returns_success = 0;
  int returns_fail = 0;
  int only_when_zero = 1;
  int only_with_errors = 1;
  int err_deg_lamda_zero = 0;
  int err_imp_err_pos = 0;
  int err_lambda_neq_count = 0;
  int err_not_a_codeword = 0;

  for(errors=1;errors<=NROOTS; errors++){
    erasures = NROOTS - 2*errors + 1;
    if(erasures < 0)
      erasures = 0;

    int cutoff = NROOTS <= len - errors ? NROOTS : len - errors;
    for(;erasures<=cutoff; erasures++){
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

	/* Generating random erasures */
	for(i=0;i<erasures;i++){
	  do {
	    errval = random() & NN;
	  } while(errval == 0); /* Error value must be nonzero */

	  do {
	    errloc = random() % len;
	  } while(errlocs[errloc] != 0); /* Must not choose the same location twice */

	  errlocs[errloc] = 1;
	  derrlocs[i] = errloc;
	  tblock[errloc] ^= errval;
	}

	if(verbose >= VERBOSE_VERY)
	  memcpy(rblock,tblock,sizeof(tblock));

	/* Decode the errored block */
#if defined(CCSDS) || defined(FIXED)
	derrors = DECODE_RS(tblock,derrlocs,erasures,pad);
#else
	derrors = DECODE_RS(rs,tblock,derrlocs,erasures);
#endif

	if(derrors >= 0){
	  returns_success++;
	  /* We check that the returned word is actually a codeword.  The obious
	   * way to do this would be to compute the syndrome, but we dont want
	   * to replicate that code here. However, all the codes are in
	   * systematic form, and therefore we can encode the returned word,
	   * and see whether it changes or not. */

	  data_t nblock[NN];
	  memcpy(nblock,tblock,sizeof(tblock));

#if defined(CCSDS) || defined(FIXED)
	  ENCODE_RS(nblock,&nblock[len-NROOTS],pad);
#else
	  ENCODE_RS(rs,nblock,&nblock[len-NROOTS]);
#endif
	  if(memcmp(tblock,nblock,sizeof(tblock)) != 0){
	    non_codeword++;

	    if(derrors != 0)
	      only_when_zero = 0;

	    if(erasures > 0)
	      only_with_errors = 0;

	    if(verbose >= VERBOSE_VERY) {
	      printf("(%d,%d)_%d:",len,len-NROOTS,NN+1);
	      print_vec(" silent failure\nc:", block, len, " %02x");
	      print_vec("r:", rblock, len, " %02x");
	      print_vec_xor("e:", rblock, block, len, " %02x");
	      print_vec("x:", tblock, len, " %02x");
	      print_vec("erasurelocs:", errlocs, len, " %d");
	    }
	  }
	}
	else {
	  switch(derrors) {
	    case RS_ERROR_DEG_LAMBDA_ZERO: err_deg_lamda_zero++; break;
	    case RS_ERROR_IMPOSSIBLE_ERR_POS: err_imp_err_pos++; break;
	    case RS_ERROR_DEG_LAMBDA_NEQ_COUNT: err_lambda_neq_count++; break;
	    case RS_ERROR_NOT_A_CODEWORD: err_not_a_codeword++; break;
	  }
	  returns_fail++;
	}
      }
      num_words += trials;
    }
  }

  if(verbose >= VERBOSE_SUMMARY) {
    printf("decoder gives up: %d / %d\n", returns_fail, num_words);
    printf("  deg(lamda) zero: %d / %d\n", err_deg_lamda_zero, returns_fail);
    printf("  bad err pos: %d / %d\n", err_imp_err_pos, returns_fail);
    printf("  lambda neq count: %d / %d\n", err_lambda_neq_count, returns_fail);
    printf("  not a codeword: %d / %d\n", err_not_a_codeword, returns_fail);
    printf("decoder returns success: %d / %d\n", returns_success, num_words);
    if(non_codeword) {
      printf("Fails only when decoder returns 0: %s\n",
	  only_when_zero ? "YES" : "NO");
      printf("Fails only when no erasures: %s\n",
	  only_with_errors ? "YES" : "NO");
    }
  }

  return non_codeword;
}


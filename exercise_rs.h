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
#define FLAG_ERASURE 1 /* Randomly flag 50% of errors as erasures */

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


#ifdef FIXED
#define PRINTPARM printf("(255,223):");
#elif defined(CCSDS)
#define PRINTPARM printf("CCSDS (255,223):");
#else
#define PRINTPARM printf("(%d,%d):",NN,NN-NROOTS);
#endif

{
  data_t block[NN],tblock[NN];
  int i;
  int errors;
  int errlocs[NN];
  int derrlocs[NROOTS];
  int derrors;
  int errval,errloc;
  int erasures;
  int decoder_errors = 0;

  while(trials-- != 0){
    /* Test up to the error correction capacity of the code */
    for(errors=0;errors <= NROOTS/2;errors++){

      /* Load block with random data and encode */
      for(i=0;i<NN-NROOTS;i++)
	block[i] = random() & NN;

#if defined(CCSDS) || defined(FIXED)
      ENCODE_RS(&block[0],&block[NN-NROOTS],0);
#else
      ENCODE_RS(rs,&block[0],&block[NN-NROOTS]);
#endif

      /* Make temp copy, seed with errors */
      memcpy(tblock,block,sizeof(tblock));
      memset(errlocs,0,sizeof(errlocs));
      memset(derrlocs,0,sizeof(derrlocs));
      erasures=0;
      for(i=0;i<errors;i++){
	do {
	  errval = random() & NN;
	} while(errval == 0); /* Error value must be nonzero */

	do {
	  errloc = random() % NN;
	} while(errlocs[errloc] != 0); /* Must not choose the same location twice */

	errlocs[errloc] = 1;

#if FLAG_ERASURE
	if(random() & 1) /* 50-50 chance */
	  derrlocs[erasures++] = errloc;
#endif
	tblock[errloc] ^= errval;
      }

      /* Decode the errored block */
#if defined(CCSDS) || defined(FIXED)
      derrors = DECODE_RS(tblock,derrlocs,erasures,0);
#else
      derrors = DECODE_RS(rs,tblock,derrlocs,erasures);
#endif

      if(derrors != errors){
	PRINTPARM
	printf(" decoder says %d errors, true number is %d\n",derrors,errors);
	decoder_errors++;
      }
      for(i=0;i<derrors;i++){
	if(errlocs[derrlocs[i]] == 0){
	  PRINTPARM
	  printf(" decoder indicates error in location %d without error\n",derrlocs[i]);
	  decoder_errors++;
	}
      }
      if(memcmp(tblock,block,sizeof(tblock)) != 0){
	PRINTPARM
	printf(" uncorrected errors! output ^ input:");
	decoder_errors++;
	for(i=0;i<NN;i++)
	  printf(" %02x",tblock[i] ^ block[i]);
	printf("\n");
      }
    }
  }
  //return decoder_errors;
}

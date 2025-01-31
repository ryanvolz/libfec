This is a version of Phil Karn's libfec with some Reed-Solomon decoder bug
fixes applied, see [#1](https://github.com/fblomqvi/libfec/issues/1),
[#2](https://github.com/fblomqvi/libfec/issues/2),
[#4](https://github.com/fblomqvi/libfec/issues/4). The tests for the RS encoder
and decoder are also extended to ensure correct behaviour in vider variety of
cases.

Phil's original readme is reproduced below:
```
COPYRIGHT

This package is copyright 2006 by Phil Karn, KA9Q. It may be used
under the terms of the GNU Lesser General Public License (LGPL). See
the file "lesser.txt" in this package for license details.

INTRODUCTION

This package provides a set of functions that implement several
popular forward error correction (FEC) algorithms and several low-level routines
useful in modems implemented with digital signal processing (DSP).

The following routines are provided:

1. Viterbi decoders for the following convolutional codes:

r=1/2 k=7 ("Voyager" code, now a widely used industry standard)
r=1/2 k=9 (Used on the IS-95 CDMA forward link)
r=1/6 k=15 ("Cassini" code, used by several NASA/JPL deep space missions)

2. Reed-Solomon encoders and decoders for any user-specified code.

3. Optimized encoder and decoder for the CCSDS-standard (255,223)
Reed-Solomon code, with and without the CCSDS-standard "dual basis"
symbol representation.

4. Compute dot product between a 16-bit buffer and a set of 16-bit
coefficients. This is the basic DSP primitive for digital filtering
and correlation.

4. Compute sum of squares of a buffer of 16-bit signed integers. This is
useful in DSP for finding the total energy in a signal.

5. Find peak value in a buffer of 16-bit signed integers, useful for
scaling a signal to prevent overflow.

SIMD SUPPORT

This package automatically makes use of various SIMD (Single
Instruction stream, Multiple Data stream) instruction sets, when
available: MMX, SSE and SSE2 on the IA-32 (Intel) architecture, and
Altivec on the PowerPC G4 and G5 used by Power Macintoshes.

"Altivec" is a Motorola trademark; Apple calls it "Velocity Engine",
and IBM calls it "VMX". Altivec is roughly comparable to SSE2 on the
IA-32.

Many of the SIMD versions run more than an order of
magnitude faster than their portable C versions. The available SIMD
instruction sets, if any, are determined at run time and the proper
version of each routine is automatically selected. If no SIMD
instructions are available, the portable C version is invoked by
default. On targets other than IA-32 and PPC, only the portable C
version is built.

The SIMD-assisted versions generally produce the same results as the C
versions, with a few minor exceptions. The Viterbi decoders in C have
a very slightly greater Eb/No performance due to their use of 32-bit
path metrics. On the other hand, the SIMD versions use the
"saturating" arithmetic available in these instructions to avoid the
integer wraparounds that can occur in C when argument ranges are not
properly constrained. This applies primarily to the "dotprod" (dot
product) function.

The MMX (MultiMedia eXtensions) instruction set was introduced on
later Pentium CPUs; it is also implemented on the Pentium II and most
AMD CPUs starting with the K6. SSE (SIMD Streaming Extensions) was
introduced in the Pentium III; AMD calls it "3D Now! Professional".
Intel introduced SSE2 on the Pentium 4, and it has been picked up by
later AMD CPUs. SSE support implies MMX support, while SSE2 support
implies both SSE and MMX support.

The latest IA-32 SIMD instruction set, SSE3 (also known as "Prescott
New Instructions") was introduced in early 2004 with the latest
("Prescott") revision of the Pentium 4. Relatively little was
introduced with SSE3, and this library currently makes no use of it.

See the various manual pages for details on how to use the library
routines.

Copyright 2006, Phil Karn, KA9Q
karn@ka9q.net
http://www.ka9q.net/

This software may be used under the terms of the GNU Lesser General
Public License (LGPL); see the file lesser.txt for details.

Revision history:
Version 1.0 released 29 May 2001

Version 2.0 released 3 Dec 2001:
Restructured to add support for shared libraries.

Version 2.0.1 released 8 Dec 2001:
Includes autoconf/configure script

Version 2.0.2 released 4 Feb 2002:
Add SIMD version override options
Test for lack of SSE2 mnemonic support in 'as'
Build only selected version

Version 2.0.3 released 6 Feb 2002:
Fix to parityb function in parity.h

feclib version 1.0 released November 2003
Merged SIMD-Viterbi, RS and DSP libraries
Changed SIMD Viterbi decoder to detect SSE2/SSE/MMX at runtime rather than build time

feclib version 2.0 (unreleased) Mar 2004
General speedups and cleanups
Switch from 4 to 8-bit input symbols on all Viterbi decoders
Support for Altivec on PowerPC
Support for k=15 r=1/6 Cassini/Mars Pathfinder/Mars Exploration Rover/STEREO code
Changed license to GNU Lesser General Public License (LGPL)

feclib version 2.1 June 5 2006
Added error checking, fixed alignment bug in SSE2 versions of Viterbi decoders causing segfaults

feclib version 2.1.1 June 6 2006
Fix test/benchmark time measurement on Linux
```


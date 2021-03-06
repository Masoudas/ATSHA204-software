/*
 * FIPS 180-2 SHA-224/256/384/512 implementation
 * Last update: 02/02/2007
 * Issue date:  04/30/2005
 *
 * Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

 /********************************************************************************
  *	Modified from the original source by Steve Motto, Atmel Corp.				*
  *	Changes:																	*
  *		1.  Removed all code / data which supported SHA224, SHA384, SHA512		*
  *		2.  Moved stack variables wv and v to zram (needs 288 bytes of RAM)		*
  *		3.  Moved SHA256 constants to flash segment								*
  ********************************************************************************/
  /** \file 	SHA256.c
   *  \brief 	Implements SHA256 Algorithm
   *  \author SEM
   *  \date 	Sept 16, 2009
  */
#if 0
#define TEST_VECTORS
#endif

#if 0
#define UNROLL_LOOPS /* Enable loops unrolling */
#endif

#include <string.h>
  //#include <ina90.h>

#include "sha256.h"
//#include "sramvars.h"
//#include "defines.h"

#define SHFR(x, n)    (x >> n)
#define ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define CH(x, y, z)  ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

#define SHA256_F1(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SHA256_F2(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SHA256_F3(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ SHFR(x,  3))
#define SHA256_F4(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHFR(x, 10))

#define UNPACK32(x, str)                      \
{                                             \
    *((str) + 3) = (uint8) ((x)      );       \
    *((str) + 2) = (uint8) ((x) >>  8);       \
    *((str) + 1) = (uint8) ((x) >> 16);       \
    *((str) + 0) = (uint8) ((x) >> 24);       \
}

#define PACK32(str, x)                        \
{                                             \
    *(x) =   ((uint32) *((str) + 3)      )    \
           | ((uint32) *((str) + 2) <<  8)    \
           | ((uint32) *((str) + 1) << 16)    \
           | ((uint32) *((str) + 0) << 24);   \
}


/* Macros used for loops unrolling */

#define SHA256_SCR(i)                         \
{                                             \
    w[i] =  SHA256_F4(w[i -  2]) + w[i -  7]  \
          + SHA256_F3(w[i - 15]) + w[i - 16]; \
}

#define SHA256_EXP(a, b, c, d, e, f, g, h, j)               \
{                                                           \
    t1 = wv[h] + SHA256_F2(wv[e]) + CH(wv[e], wv[f], wv[g]) \
         + sha256_k[j] + w[j];                              \
    t2 = SHA256_F1(wv[a]) + MAJ(wv[a], wv[b], wv[c]);       \
    wv[d] += t1;                                            \
    wv[h] = t1 + t2;                                        \
}

//flash uint32 sha256_h0[8] =
uint32 sha256_h0[8] = ???
{ 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };

//flash uint32 sha256_k[64] =
uint32 sha256_k[64] = ????
{ 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

/* SHA-256 functions */

// some helper macros for TPM implementation
//#define zram32  ((UINT32*) zram)
//#define eob32(buf) ((sizeof(buf)/sizeof(UINT32)))

// (Babak:) Performs the SHA-256 hash algorithm based on the current hash state of the ctx parameter. Note that the message is not taken from the ctx parameter (but the "message" parameter). Also, no padding is done since the message is presumed to exactly fill "block_nb" number of (512-bit) blocks.
void sha256_transf(sha256_ctx *ctx, const uint8 *message,
	uint32 block_nb)
{
	uint32 w[64];
	uint32 wv[8];
	uint32 h[8]; // (Babak:) Update to hold the intermediate hash state.
	//	uint32 *wv = &zram32[eob32(zram)-8];
	//	uint32 *w = &zram32[eob32(zram)-(8+64)];

	uint32 t1, t2;
	const uint8 *sub_block;
	int i;

#ifndef UNROLL_LOOPS
	int j;
#endif

	// (Babak:) Iterates over messsage blocks.
	for (i = 0; i < (int)block_nb; i++) {
		sub_block = message + (i << 6); // (Babak:) Pointer to the beginning of the i-th block.

#ifndef UNROLL_LOOPS
		// (Babak:) Copies the content of the block into the first 16 "message schedules", W_0 to W_15 (See pg. 27, item 1).
		for (j = 0; j < 16; j++) {
			PACK32(&sub_block[j << 2], &w[j]);
		}

		// (Babak:) Completes the message schedule array, W_16 to W_63 (See pg. 27, item 1).
		for (j = 16; j < 64; j++) {
			SHA256_SCR(j);
		}

		// (Babak:) Reads the last digest (from the ctx) into the temporary local array wv (corresponding to variables a, ..., h on pg. 27, item 2).
		// (Update).
		for (j = 0; j < 8; j++) {
			PACK32(&ctx->digest[j << 2], &h[j]);
			wv[j] = h[j];
		}

		// This is not applicable now:
		// for (j = 0; j < 8; j++) {
		//		wv[j] = ctx->h[j];
		// }

		// (Babak:) Performs the hash algorithm (See pg. 28, item 3).
		for (j = 0; j < 64; j++) {
			t1 = wv[7] + SHA256_F2(wv[4]) + CH(wv[4], wv[5], wv[6])
				+ sha256_k[j] + w[j];
			t2 = SHA256_F1(wv[0]) + MAJ(wv[0], wv[1], wv[2]);
			wv[7] = wv[6];
			wv[6] = wv[5];
			wv[5] = wv[4];
			wv[4] = wv[3] + t1;
			wv[3] = wv[2];
			wv[2] = wv[1];
			wv[1] = wv[0];
			wv[0] = t1 + t2;
		}

		// (Babak:) Finalizes the hash value into the ctx's array (See pg. 28, item 4).
		// (Update).
		for (j = 0; j < 8; j++) {
			h[j] += wv[j];
			UNPACK32(h[j], &ctx->digest[j << 2]);
		}

		// This is not applicable now:
		// for (j = 0; j < 8; j++) {
		//  	ctx->h[j] += wv[j];
		//}
#else
		PACK32(&sub_block[0], &w[0]); PACK32(&sub_block[4], &w[1]);
		PACK32(&sub_block[8], &w[2]); PACK32(&sub_block[12], &w[3]);
		PACK32(&sub_block[16], &w[4]); PACK32(&sub_block[20], &w[5]);
		PACK32(&sub_block[24], &w[6]); PACK32(&sub_block[28], &w[7]);
		PACK32(&sub_block[32], &w[8]); PACK32(&sub_block[36], &w[9]);
		PACK32(&sub_block[40], &w[10]); PACK32(&sub_block[44], &w[11]);
		PACK32(&sub_block[48], &w[12]); PACK32(&sub_block[52], &w[13]);
		PACK32(&sub_block[56], &w[14]); PACK32(&sub_block[60], &w[15]);

		SHA256_SCR(16); SHA256_SCR(17); SHA256_SCR(18); SHA256_SCR(19);
		SHA256_SCR(20); SHA256_SCR(21); SHA256_SCR(22); SHA256_SCR(23);
		SHA256_SCR(24); SHA256_SCR(25); SHA256_SCR(26); SHA256_SCR(27);
		SHA256_SCR(28); SHA256_SCR(29); SHA256_SCR(30); SHA256_SCR(31);
		SHA256_SCR(32); SHA256_SCR(33); SHA256_SCR(34); SHA256_SCR(35);
		SHA256_SCR(36); SHA256_SCR(37); SHA256_SCR(38); SHA256_SCR(39);
		SHA256_SCR(40); SHA256_SCR(41); SHA256_SCR(42); SHA256_SCR(43);
		SHA256_SCR(44); SHA256_SCR(45); SHA256_SCR(46); SHA256_SCR(47);
		SHA256_SCR(48); SHA256_SCR(49); SHA256_SCR(50); SHA256_SCR(51);
		SHA256_SCR(52); SHA256_SCR(53); SHA256_SCR(54); SHA256_SCR(55);
		SHA256_SCR(56); SHA256_SCR(57); SHA256_SCR(58); SHA256_SCR(59);
		SHA256_SCR(60); SHA256_SCR(61); SHA256_SCR(62); SHA256_SCR(63);

		wv[0] = ctx->h[0]; wv[1] = ctx->h[1];
		wv[2] = ctx->h[2]; wv[3] = ctx->h[3];
		wv[4] = ctx->h[4]; wv[5] = ctx->h[5];
		wv[6] = ctx->h[6]; wv[7] = ctx->h[7];

		SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 0); SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 1);
		SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 2); SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 3);
		SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 4); SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 5);
		SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 6); SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 7);
		SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 8); SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 9);
		SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 10); SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 11);
		SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 12); SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 13);
		SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 14); SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 15);
		SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 16); SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 17);
		SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 18); SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 19);
		SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 20); SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 21);
		SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 22); SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 23);
		SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 24); SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 25);
		SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 26); SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 27);
		SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 28); SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 29);
		SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 30); SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 31);
		SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 32); SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 33);
		SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 34); SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 35);
		SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 36); SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 37);
		SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 38); SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 39);
		SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 40); SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 41);
		SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 42); SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 43);
		SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 44); SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 45);
		SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 46); SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 47);
		SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 48); SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 49);
		SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 50); SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 51);
		SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 52); SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 53);
		SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 54); SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 55);
		SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 56); SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 57);
		SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 58); SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 59);
		SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 60); SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 61);
		SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 62); SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 63);

		ctx->h[0] += wv[0]; ctx->h[1] += wv[1];
		ctx->h[2] += wv[2]; ctx->h[3] += wv[3];
		ctx->h[4] += wv[4]; ctx->h[5] += wv[5];
		ctx->h[6] += wv[6]; ctx->h[7] += wv[7];
#endif /* !UNROLL_LOOPS */
	}
}

// (Babak:) Gets a message, applies the SHA25 hash algorithm and updates the given "digest" array with the results.
void sha256(const uint8 *message, uint32 len, uint8 *digest)
{
	sha256_ctx ctx;

	sha256_init(&ctx);
	sha256_update(&ctx, message, len);
	// sha256_final(&ctx, digest); // (Not applicable, since the finalization method now takes only one argument.)
	sha256_final(&ctx); // (Update.)

	// (Update).
	// The SHA256 result has to be copied to the output array.
	memcpy(digest, ctx.digest, sizeof(ctx.digest) / sizeof(uint8));
}

// (Babak:) Initializes the ctx object with the SHA256 constants.
void sha256_init(sha256_ctx *ctx)
{
#ifndef UNROLL_LOOPS
	int i;

	// (Update).
	for (i = 0; i < 8; i++) {
		UNPACK32(sha256_h0[i], &ctx->digest[i << 2]);
	}

	// This is not applicable now:
	// for (i = 0; i < 8; i++) {
	//	  ctx->h[i] = sha256_h0[i];
	// }
#else
	ctx->h[0] = sha256_h0[0]; ctx->h[1] = sha256_h0[1];
	ctx->h[2] = sha256_h0[2]; ctx->h[3] = sha256_h0[3];
	ctx->h[4] = sha256_h0[4]; ctx->h[5] = sha256_h0[5];
	ctx->h[6] = sha256_h0[6]; ctx->h[7] = sha256_h0[7];
#endif /* !UNROLL_LOOPS */

	ctx->len = 0;
	ctx->tot_len = 0;
}

// (Babak:) Gets a new message (i.e., an update) an instance of the sha256_ctx struct (ctx) that contains the 1) state of the hash algorithm, and 2) the last (512-bit) block of the last message used to generate the hash, appends the new message to the last, applies SHA256 repeatedly and updates the ctx to represent a valid state of the hash on the updated set. After the method has returned, the "block" array of the ctx holds the last incomplete block of the message that still needs to be padded before finalizing the hash algorithm.
void sha256_update(sha256_ctx *ctx, const uint8 *message,
	uint32 len)
{
	uint32 block_nb;
	uint32 new_len, rem_len, tmp_len;
	const uint8 *shifted_message;

	tmp_len = SHA256_BLOCK_SIZE - ctx->len; // (Babak:) Number of the unused bytes at the end of the ctx's (512-bit) block.
	rem_len = len < tmp_len ? len : tmp_len; // (Babak:) Number of the new bytes to be filled in the ctx's (512-bit) block.

	memcpy(&ctx->block[ctx->len], message, rem_len); // (Babak:) Filling the ctx's block with the beginning of the new message.

	if (ctx->len + len < SHA256_BLOCK_SIZE) {
		// (Babak:) The new message (i.e., the update) was small enough to be fitted in the ctx's (512-bit) block. So, there is still no need to apply the hash algorithm, because the block is not full.
		ctx->len += len;
		return;
	}

	// (Babak): At this point, the new message is ensured to be large enough to require one or more (512-bit) blocks. This means there is at least one full block and therefore repeated calls to the hash algorithm should be made to simplify the hash state (ctx) to the last incomplete block.

	new_len = len - rem_len; // (Babak:) Length of the remaining portion of the new message (out of the ctx's block).
	block_nb = new_len / SHA256_BLOCK_SIZE; // (Babak:) Number of blocks required to hold the remaining of the message.

	shifted_message = message + rem_len; // (Babak:) Pointer to the beginning of the remaining message portion.

	sha256_transf(ctx, ctx->block, 1); // (Babak:) Computes the hash for the already completed block which includes the last part of the last message (and now appended by a portion of the new message).
	sha256_transf(ctx, shifted_message, block_nb); // (Babak:) Continues to compute the hash for the rest of the new message except for the last block which is not full.

	rem_len = new_len % SHA256_BLOCK_SIZE; // (Babak:) Number of bytes of the new message that fall into the last (512-bit) block.

	memcpy(ctx->block, &shifted_message[block_nb << 6],
		rem_len); // (Babak:) Copies the final portion of the new message to the ctx's block storage.

	ctx->len = rem_len; // (Babak:) Updates the number of bytes in the ctx's block.
	ctx->tot_len += (block_nb + 1) << 6; // (Babak:) Updates the cumulative length of the message.
}

// (Babak:) Finalizes the hash algorithm. The method gets an incomplete block (ctx), adds the padding bytes, applies the hash algorithm on the completed block and copies the digest to the given array.
void sha256_final(sha256_ctx *ctx)
{
	uint32 block_nb;
	uint32 pm_len;
	uint32 len_b;

#ifndef UNROLL_LOOPS
	int i;
#endif

	// (Babak:) Applies padding on the ctx's block.
	block_nb = (1 + ((SHA256_BLOCK_SIZE - 9)
		< (ctx->len % SHA256_BLOCK_SIZE)));
	// (Babak:) Note that, the number of blocks is not always the same (1 or 2).

	len_b = (ctx->tot_len + ctx->len) << 3;
	pm_len = block_nb << 6;

	memset(ctx->block + ctx->len, 0, pm_len - ctx->len);
	ctx->block[ctx->len] = 0x80; // (Inserts the "1" bit at the end of the message.)
	UNPACK32(len_b, ctx->block + pm_len - 4);

	// (Babak:) Computes the hash on the padded block.
	sha256_transf(ctx, ctx->block, block_nb);

	// (Update).					
	// This is not applicable now:
//#ifndef UNROLL_LOOPS		
//		// (Babak:) Copies the hash result to the "digest" array.		
//		// for (i = 0; i < 8; i++) {
//		//  	UNPACK32(ctx->h[i], &digest[i << 2]);
//		// }
//#else
//	UNPACK32(ctx->h[0], &digest[0]);
//	UNPACK32(ctx->h[1], &digest[4]);
//	UNPACK32(ctx->h[2], &digest[8]);
//	UNPACK32(ctx->h[3], &digest[12]);
//	UNPACK32(ctx->h[4], &digest[16]);
//	UNPACK32(ctx->h[5], &digest[20]);
//	UNPACK32(ctx->h[6], &digest[24]);
//	UNPACK32(ctx->h[7], &digest[28]);
//#endif /* !UNROLL_LOOPS */
}

#ifdef TEST_VECTORS

/* FIPS 180-2 Validation tests */

#include <stdio.h>
#include <stdlib.h>

void test(const uint8 *vector, uint8 *digest,
	uint32 digest_size)
{
	uint8 output[2 * SHA256_DIGEST_SIZE + 1];
	int i;

	output[2 * digest_size] = '\0';

	for (i = 0; i < (int)digest_size; i++) {
		sprintf((char *)output + 2 * i, "%02x", digest[i]);
	}

	//printf("H: %s\n", output);
	if (strcmp((char *)vector, (char *)output)) {
		fprintf(stderr, "Test failed.\n");
		//      exit(EXIT_FAILURE);
		exit(-1);
	}
}

int SHA256main()
{
#if 1
	static const uint8 *vectors[3] =
	{
		/* SHA-256 */
		"ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
		"248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1",
		"cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0",
	};

	static const uint8 message1[] = "abc";
	static const uint8 message2a[] = "abcdbcdecdefdefgefghfghighijhi"
		"jkijkljklmklmnlmnomnopnopq";
	uint8 message3[100];
	uint32 message3_updates = 1000000 / sizeof(message3);

	sha256_ctx ctx;
	uint8 digest[SHA256_DIGEST_SIZE];
	uint32 i;

	memset(message3, 'a', sizeof(message3));

	//printf("SHA-2 FIPS 180-2 Validation tests\n\n");

	//printf("SHA-256 Test vectors\n");

	sha256(message1, strlen((char *)message1), digest);
	//  test(vectors[0], digest, SHA256_DIGEST_SIZE);
	sha256(message2a, strlen((char *)message2a), digest);
	//  test(vectors[1], digest, SHA256_DIGEST_SIZE);

	sha256_init(&ctx);
	for (i = 0; i < message3_updates; i++)
		sha256_update(&ctx, message3, sizeof(message3));
	sha256_final(&ctx, digest);
	//  test(vectors[2], digest, SHA256_DIGEST_SIZE);

		//printf("\n");
		//printf("All tests passed.\n");

		// debug: breakpoint target
	asm volatile("nop"::);

#endif
	return 0;
}

#endif /* TEST_VECTORS */


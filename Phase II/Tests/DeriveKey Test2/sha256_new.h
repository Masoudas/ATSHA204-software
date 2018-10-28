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
/** \file 	SHA256.h
 *  \brief 	SHA256 definitions.
 *  \author     Steven A. Fontana Sr. 
 *  \date 	September 4 2009
*/

//#ifndef SHA2_H
#define SHA2_H

#define SHA224_DIGEST_SIZE ( 224 / 8)
#define SHA256_DIGEST_SIZE ( 256 / 8)

#define SHA256_BLOCK_SIZE  ( 512 / 8)
#define SHA224_BLOCK_SIZE  SHA256_BLOCK_SIZE

/*
#ifndef SHA2_TYPES
#define SHA2_TYPES
typedef uint8 uint8;
typedef unsigned int uint32;
typedef unsigned long long uint64;
#endif
*/

#ifndef SHA2_TYPES
#define SHA2_TYPES
typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef unsigned long uint32;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// (Babak:) A struct to hold the state of the hash algorithm.
typedef struct {
    uint32 tot_len; // (Babak:) Cumulative length of the message (assuming the total message is padded). The value should not directly be used, unless the finalization method has been called.
    uint32 len; // (Babak:) Length of the message over the (512-bit) block.
    uint8 block[2 * SHA256_BLOCK_SIZE]; // (Babak:) Block storage to hold the last block of the message at any time. This is used to chain the calls to the hash algortihm.

	// (Update).
	// This is not applicable anymore:
    // uint32 h[8]; // (Babak:) Storage for hash algorithm result.

	// (Update to the original version.)
	uint8 digest[32]; // To hold the intermediate digests (H) and also the final value (after finalization).
		// Using this, there is no need to keep the "h" field.
} sha256_ctx_new;

void sha256_init_new(sha256_ctx_new * ctx);
void sha256_update_new(sha256_ctx_new *ctx, const uint8 *message, uint32 len);
void sha256_final_new(sha256_ctx_new *ctx);
void sha256_new(const uint8 *message, uint32 len, uint8 *digest);

#ifdef __cplusplus
}
#endif

//#endif /* !SHA2_H */


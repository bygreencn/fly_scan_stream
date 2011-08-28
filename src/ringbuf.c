/*
   A General High Performance Ring buffer for scanning pattern from file stream

   Copyright (c) 2011 Sui Libin <bygreencn AT gmail DOT com>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef RB_DEBUG
#	define RB_DEBUG_LOG 1
#else
# define RB_DEBUG_LOG 0
#endif

#if(RB_DEBUG_LOG)
#include <stdio.h>
void ERROR(const char * msg)
{
        printf("RB-Error: %s\n", msg);
}
#else
void ERROR(const char * msg)
{
}
#endif

#include <stdlib.h>
#include <string.h>
#include "ringbuf.h"


int
rb_create(RingBuffer **rb, int capacity, int threshold)
{
        RingBuffer *ring;

        if(rb==NULL) {
                ERROR("Not specify the ringbuf address");
                return RB_FATAL_ERROR;
        }

        if(capacity <= 1024) {
                ERROR("Capicity is too small");
                return RB_NOT_SUPPORT;
        }

        if(threshold > capacity || threshold < 0) {
                ERROR("threshold is wrong");
                return RB_NOT_SUPPORT;
        }

        ring = malloc (sizeof (RingBuffer));
        if(ring == NULL) {
                ERROR("Not enough memory");
                return RB_FATAL_ERROR;
        }
        memset (ring, 0, sizeof (RingBuffer));

        ring->capacity = capacity;
        ring->size = 0;
        ring->min_threshold = threshold;

        ring->iput = 0;
        ring->iget = 0;
        ring->buf=malloc(sizeof(char)*(ring->capacity));

        if(NULL == ring->buf) {
                ERROR("Not enough memory");
                free(ring);
                return RB_FATAL_ERROR;
        }

        *rb = ring;

        return RB_SUCCESS;
}

void
rb_destroy(RingBuffer* rb)
{
        if(!rb)
                return;
        free(rb->buf);
        free(rb);
}
int
rb_clear (struct OutRingBuffer *rb)
{
        memset(rb->buffer,0,rb->size);
        rb->size = 0;
        rb->iput=0;
        rb->iget=0;

        return RB_SUCCESS;
}

int
rb_write (RingBuffer *rb, unsigned char * buf, int len)
{
        rb->lock = 1;
		int need_write = 0;
		if (NULL == buf
			|| len<=0)
		{
			ERROR("Not write data to buffer");
			return 0;
		}

		if(rb->size == rb->capacity) // ringbuf is full
		{
			ERROR("Buffer is already full");
			return 0;
		} else {
			//TODO:			

		}
       
        b->lock = 0;
        return 0;
}
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
rb_create(RingBuffer **prb, unsigned int capacity, unsigned int threshold,bool balign, bool bshift)
{
	RingBuffer *rb;

	if(prb==NULL)
	{
		ERROR("Not specify the ringbuf address");
		return RB_FATAL_ERROR;
	}

	if(capacity < 2)
	{
		ERROR("Capicity is too small");
		return RB_NOT_SUPPORT;
	}

	if(threshold > capacity || threshold < 0)
	{
		ERROR("threshold is wrong");
		return RB_NOT_SUPPORT;
	}

	rb = malloc (sizeof (RingBuffer));
	if(rb == NULL)
	{
		ERROR("Not enough memory");
		return RB_FATAL_ERROR;
	}
	memset (rb, 0, sizeof (RingBuffer));

	rb->capacity = capacity;
	rb->size = 0;
	rb->min_threshold = threshold;
	rb->b8bytealign = balign;
	rb->bshiftalltime = bshift;

	rb->iwrite = 0;
	rb->iread = 0;
	if(true == rb->b8bytealign)
	{
		rb->prealbuf = malloc(sizeof(char)*(rb->capacity+8));  // always keep the buffer address aligned at 8.
		rb->buf = (unsigned char *)((unsigned int)rb->prealbuf&(~0x3));
		rb->buf = (rb->buf == (unsigned char *) rb->prealbuf)? rb->buf:rb->buf+8;
	}
	else
	{
		rb->buf=(unsigned char *)malloc(sizeof(char)*(rb->capacity));
		rb->prealbuf = (unsigned int *)rb->buf;
	}

	if(NULL == rb->prealbuf)
	{
		ERROR("Not enough memory");
		free(rb);
		return RB_FATAL_ERROR;
	}

	*prb = rb;

	return RB_SUCCESS;
}

void
rb_destroy(RingBuffer* rb)
{
	if(!rb)
		return;
	free(rb->prealbuf);
	free(rb);
}
int
rb_clear (RingBuffer *rb)
{
	memset(rb->buf,0,rb->capacity);
	rb->size = 0;
	rb->iwrite=0;
	rb->iread=0;
	return RB_SUCCESS;
}

int
rb_streamsize(RingBuffer * rb)
{
	return (rb->capacity-rb->iwrite);
}

int
rb_size(RingBuffer * rb)
{
	if(rb->iread<=rb->iwrite)
	{
		return (rb->capacity-rb->iwrite+rb->iread);
	}
	else
	{
		return (rb->iread-rb->iwrite);
	}
}

int
rb_capaciry(RingBuffer * rb)
{
	return rb->capacity;
}


int
rb_write (RingBuffer *rb, unsigned char * buf, unsigned int len)
{
	unsigned int need_write = 0;
	rb->lock = 1;

	if (NULL == rb
			|| NULL == buf
			|| len<=0)
	{
		ERROR("Not write data to buffer");
		return 0;
	}

	if(rb->size == rb->capacity)   // ringbuf is full
	{
		ERROR("Buffer is already full");
		return 0;
	}
	else
	{
		if(rb->bshiftalltime || rb->b8bytealign)
		{
			need_write = rb->capacity -  rb->size;
			need_write = (need_write > len)? len:need_write;
			if(rb->iread != 0)
			{
				memcpy(rb->buf, rb->buf+rb->iread, rb->size);
				rb->iread = 0;
			}
			memcpy(rb->buf+rb->size, buf, need_write);
			rb->iwrite = rb->size += need_write;
		}
		else
		{
			need_write = rb_streamsize(rb);
			if(need_write <= rb->min_threshold)
			{
				need_write = rb->capacity -  rb->size;
				need_write = (need_write > len)? len:need_write;
				if(rb->iread != 0)
				{
					memcpy(rb->buf, rb->buf+rb->iread, rb->size);
					rb->iread = 0;
				}
				memcpy(rb->buf+rb->size, buf, need_write);
				rb->iwrite = rb->size += need_write;
			}
			else
			{
				need_write = (need_write > len)? len:need_write;
				memcpy(rb->buf+rb->iwrite, buf, need_write);
				rb->size += need_write;
				rb->iwrite  += need_write;
			}
		}
	}

	rb->lock = 0;
	return 0;
}

int rb_read (RingBuffer *rb, parser_fun parser)
{
	int used = 0;
	if (NULL == rb
			|| NULL == parser)
	{
		ERROR("Not write data to buffer");
		return 0;
	}

	used = parser(rb->buf+rb->iread, rb->size);
	if(used > 0)
	{
		rb->iread -= used;
		rb->size -= used;
	}
	if(rb->bshiftalltime || rb->b8bytealign)
	{
		memcpy(rb->buf, rb->buf+rb->iread, rb->size);
		rb->iread = 0;
		rb->iwrite = rb->size ;
	}
	return used;
}
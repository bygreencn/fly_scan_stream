/*
   A General High Performance rb buffer for scanning pattern from file stream

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
#include<stdio.h>
#include "ringbuf.h"

int main(int argc,char **argv)
{
	RingBuffer* rb;
	int i;
	unsigned char buffer[128];
	printf("fly_scan_stream Sample\n\n");

	for(i=0; i<128; i++)
		buffer[i] = i;

	rb_create(&rb, 16, 5,false, false);
	for(i=0; i< 128; i++)
	{
		rb_write(rb, buffer, i);
		if(rb->size == rb->capacity)
			rb_clear(rb);
	}
	rb_destroy(rb);

	rb_create(&rb, 128, 5,true, false);
	rb_destroy(rb);

	rb_create(&rb, 128, 5,false, true);
	rb_destroy(rb);
	return 0;
};
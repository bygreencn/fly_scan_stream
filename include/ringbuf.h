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

#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#define RB_DEBUG 



#define RB_MIN_CAPACITY (1024)
#define RB_CAPACITY(v) ((v/1024

enum RB_STATUS{
	RB_SUCCESS = 0,
	RB_ERROR = -1,
	RB_NOT_SUPPORT = -2,
	RB_FATAL_ERROR = -3
};

/**
  the ringbuffer struct
*/
typedef struct _RingBuffer {
  unsigned char * buf;
  unsigned int iput;      /* index for next pos to be put */
  unsigned int iget;      /* index for next pos to be got */
  unsigned int size;        /* size of used in buffer */
  unsigned int capacity;     /* max size of buffer */
  unsigned int min_threshold; /* minimum threshold that buffer need refresh, default should be the minimum size of scan pattern */
  int lock;            /* internal lock */
  int flags;           /*< general purpose flags to mark buffer */
} RingBuffer;




/* ring buffer functions */
int rb_create(RingBuffer **, int , int );
void rb_destroy(RingBuffer*);
int rb_clear (RingBuffer *);

int rb_write (RingBuffer *, unsigned char *, int);
int rb_read (RingBuffer *, unsigned char *, int);
int rb_data_size (RingBuffer *);
#endif // end of __RINGBUF_H__
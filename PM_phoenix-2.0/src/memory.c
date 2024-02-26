/* Copyright (c) 2007-2009, Stanford University
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Stanford University nor the names of its 
*       contributors may be used to endorse or promote products derived from 
*       this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY STANFORD UNIVERSITY ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL STANFORD UNIVERSITY BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/ 

#include <assert.h>
#include <string.h>
#ifdef _SOLARIS_
#define PAGE_SIZE (8 * 1024)
#include <mtmalloc.h>
#include <sys/mman.h>
#else
#include <stdlib.h>
#define PAGE_SIZE (4 * 1024)
#endif

#define ALIGN_PAGE(ptr) (void *)((uintptr_t)(ptr) & (~(PAGE_SIZE - 1)))

#include "memory.h"

PMEMobjpool* pop;

void set_pool (PMEMobjpool* pool) {
  pop = pool;
}

void *mem_malloc (size_t size)
{
    // check for 0 size passed
    if (size == 0) {
      return NULL;
    }

    PMEMoid tmp_oid;
    pmemobj_alloc(pop, &tmp_oid, size, 0, NULL, NULL);
    void *temp = pmemobj_direct(tmp_oid);
    // void *temp = malloc (size);
    assert(temp);

    return temp;
}

void *mem_malloc_here (size_t size)
{
    // check for 0 size passed
    if (size == 0) {
      return NULL;
    }

    PMEMoid tmp_oid;
    pmemobj_alloc(pop, &tmp_oid, size, 0, NULL, NULL);
    void *temp = pmemobj_direct(tmp_oid);
    // void *temp = malloc (size);
    assert(temp);

    return temp;
}

void *mem_calloc (size_t num, size_t size)
{   
    // check for 0 size passed
    if ((num * size) == 0) {
      return NULL;
    }

    PMEMoid tmp_oid;
    pmemobj_alloc(pop, &tmp_oid, num * size, 0, NULL, NULL);
    void *temp = pmemobj_direct(tmp_oid);
    assert(temp);

    pmemobj_memset_persist(pop, temp, 0, num * size);
    // void *temp = calloc (num, size);

    return temp;
}

void *mem_realloc (void *ptr, size_t size)
{   
    // check for 0 size passed
    if (size == 0) {
      return NULL;
    }

    PMEMoid tmp_oid = pmemobj_oid(ptr);
    pmemobj_realloc(pop, &tmp_oid, size, 0);
    void *temp = pmemobj_direct(tmp_oid);
    // void *temp = realloc (ptr, size);
    assert(temp);

    return temp;
}

void *mem_memcpy (void *dest, const void *src, size_t size)
{   
    // return pmemobj_memcpy_persist(pop, dest, src, size);
    return memcpy (dest, src, size);
}

void *mem_memset (void *s, int c, size_t n)
{
    // return pmemobj_memset_persist(pop, s, c, n);
    return memset (s, c, n);
}

void mem_free (void *ptr)
{   
    PMEMoid tmp_oid = pmemobj_oid(ptr);
    pmemobj_free(&tmp_oid);
    // free (ptr);
}

void *real_malloc (size_t size)
{
    void *temp = malloc (size);
    assert(temp);

    return temp;
}

void *real_calloc (size_t num, size_t size)
{   
    void *temp = calloc (num, size);
    assert(temp);

    return temp;
}

void *real_memset (void *s, int c, size_t n)
{
    return memset (s, c, n);
}

void real_free (void *ptr)
{   
    free (ptr);
}
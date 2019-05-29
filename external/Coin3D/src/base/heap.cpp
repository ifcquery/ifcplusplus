/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*! \file heap.h */
#include <Inventor/C/base/heap.h>

#include <cassert>
#include <cstdlib>
#include <cstdio>

#include "base/dict.h"
#include "base/heapp.h"
#include "coindefs.h"

#ifndef COIN_WORKAROUND_NO_USING_STD_FUNCS
using std::realloc;
using std::malloc;
using std::free;
#endif // !COIN_WORKAROUND_NO_USING_STD_FUNCS

/* ********************************************************************** */

/*!
  \typedef int cc_heap_compare_cb(void * o1, void * o2)

  A type definition for heap compare callback function.
*/

/*! 
  \typedef struct cc_heap cc_heap

  A type definition for the cc_heap structure
*/

/* ********************************************************************** */
/* private functions */

#define HEAP_PARENT(i) (((i)+1) / 2 - 1)
#define HEAP_LEFT(i) ((i) * 2 + 1)
#define HEAP_RIGHT(i) ((i) * 2 + 2)

static void
heap_resize(cc_heap * h, unsigned int newsize)
{
  /* Never shrink the heap */
  if (h->size >= newsize)
    return;

  h->array = static_cast<void **>(realloc(h->array, newsize * sizeof(void *)));
  assert(h->array);
  h->size = newsize;
}

static void
heap_heapify(cc_heap * h, uintptr_t i)
{
  uintptr_t left = HEAP_LEFT(i);
  uintptr_t right = HEAP_RIGHT(i);
  uintptr_t largest;

  /* Check which node is larger of i and its two children; if any
   * of them is larger swap it with i and recurse down on the child
   */
  if (left < h->elements && h->compare(h->array[left], h->array[i]) > 0)
    largest = left;
  else
    largest = i;

  if (right < h->elements && h->compare(h->array[right], h->array[largest]) > 0)
    largest = right;

  if (largest != i) {
    void * tmp = h->array[largest];
    h->array[largest] = h->array[i];
    h->array[i] = tmp;

    if (h->support_remove) {
      cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(h->array[i]), reinterpret_cast<void *>(i));
      cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(h->array[largest]), reinterpret_cast<void *>(largest));
    }

    heap_heapify(h, largest);
  }
}

/* ********************************************************************** */
/* public api */

/*!

  Construct a heap. \a size is the initial array size.

  \a comparecb should return a negative value if the first element
  is less than the second, zero if they are equal and a positive value
  if the first element is greater than the second.

  \a support_remove specifies if the heap should support removal of
  elements (other than the top element) after they are added; this
  requires use of a hash table to be efficent, but as a slight runtime
  overhead will be incurred for the add and extract_top functions the
  support can be disabled if you don't need it.

*/

cc_heap *
cc_heap_construct(unsigned int size,
                  cc_heap_compare_cb * comparecb,
                  SbBool support_remove)
{
  cc_heap * h = static_cast<cc_heap *>(malloc(sizeof(cc_heap)));
  assert(h);

  h->size = size;
  h->elements = 0;
  h->array = static_cast<void **>(malloc(size * sizeof(void *)));
  assert(h->array);
  h->compare = comparecb;
  h->support_remove = support_remove;
  h->hash = NULL;
  if (support_remove) {
    h->hash = cc_dict_construct(size, 0.0f);
  }
  return h;
}

/*!
  Destruct the heap \a h.
*/
void
cc_heap_destruct(cc_heap * h)
{
  cc_heap_clear(h);
  free(h->array);
  if (h->hash) cc_dict_destruct(h->hash);
  free(h);
}

/*!
  Clear/remove all elements in the heap \a h.
*/
void cc_heap_clear(cc_heap * h)
{
  h->elements = 0;
  if (h->hash) cc_dict_clear(h->hash);
}

/*!
  Add the element \a o to the heap \a h.
*/
void
cc_heap_add(cc_heap * h, void * o)
{
  uintptr_t i;

  /* Resize the heap if it is full or the threshold is exceeded */
  if (h->elements == h->size) {
    heap_resize(h, h->size * 2);
  }

  i = h->elements++;

  /* If o is greater than its parent, swap them and check again */
  while (i > 0 && h->compare(o, h->array[HEAP_PARENT(i)]) > 0) {
    h->array[i] = h->array[HEAP_PARENT(i)];

    if (h->support_remove) {
      cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(h->array[i]), reinterpret_cast<void *>(i));
    }

    i = HEAP_PARENT(i);
  }
  h->array[i] = o;

  if (h->support_remove) {
    cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(o), reinterpret_cast<void *>(i));
  }
}

/*!
  Returns the top element from the heap \a h. If the heap is empty,
  NULL is returned.
*/
void *
cc_heap_get_top(cc_heap * h)
{
  if (h->elements == 0) return NULL;
  return h->array[0];
}

/*!
  Returns and removes the top element from the heap \a h. If the
  heap is empty, NULL is returned.
*/
void *
cc_heap_extract_top(cc_heap * h)
{
  void * top;
  if (h->elements == 0) return NULL;

  top = h->array[0];
  h->array[0] = h->array[--h->elements];

  if (h->support_remove) {
    cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(h->array[0]), reinterpret_cast<void *>(0));
    cc_dict_remove(h->hash, reinterpret_cast<uintptr_t>(top));
  }

  heap_heapify(h, 0);

  return top;
}

/*!

  Remove \a o from the heap \a h; if present TRUE is returned,
  otherwise FALSE.  Please note that the heap must have been created
  with support_remove.

*/
int
cc_heap_remove(cc_heap * h, void * o)
{
  uintptr_t i;
  void * tmp;

  if (!h->support_remove) return FALSE;

  if (!cc_dict_get(h->hash, reinterpret_cast<uintptr_t>(o), &tmp))
    return FALSE;

  i = reinterpret_cast<uintptr_t>(tmp);
  assert(i < h->elements);
  assert(h->array[i] == o);

  h->array[i] = h->array[--h->elements];
  if (h->support_remove) {
    cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(h->array[i]), reinterpret_cast<void *>(i));
  }
  heap_heapify(h, i);

  cc_dict_remove(h->hash, reinterpret_cast<uintptr_t>(o));

  return TRUE;
}

/*!
  Returns the number of elements in the heap \a h.
*/
unsigned int
cc_heap_elements(cc_heap * h)
{
  return h->elements;
}

/*!
  Returns TRUE of the heap \a h is empty; otherwise FALSE.
*/
SbBool
cc_heap_empty(cc_heap * h)
{
  return h->elements == 0 ? TRUE : FALSE;
}

#undef HEAP_LEFT
#undef HEAP_PARENT
#undef HEAP_RIGHT

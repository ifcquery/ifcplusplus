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

#ifndef SOGUI_SBGUILIST_H
#define SOGUI_SBGUILIST_H

/**************************************************************************/

// IMPORTANT NOTES:
//
//   * This file is a duplicate of Coin's lists/SbList.h file, and any
//   important fixes/improvements here should be migrated.
//
//   * This file contains definitions which should only be used during
//   library build for now. It is not yet installed for use by the
//   application programmer.

/**************************************************************************/

#include <Inventor/SbBasic.h>
#include <assert.h>
#include <stddef.h> // NULL definition

// We usually implement inline functions below the class definition,
// since we think that makes the file more readable. However, this is
// not done for this class, since Visual C++ is not too happy about
// having functions declared as inline for a template class.
// pederb, 2001-10-12

template <class Type>
class SbGuiList {
  // Older compilers aren't too happy about const declarations in the
  // class definitions, so use the enum trick described by Scott
  // Meyers in "Effective C++".
  enum { DEFAULTSIZE = 4 };

public:

  SbGuiList(const int sizehint = DEFAULTSIZE)
    : itembuffersize(DEFAULTSIZE), numitems(0), itembuffer(builtinbuffer) {
    if (sizehint > DEFAULTSIZE) this->grow(sizehint);
  }

  SbGuiList(const SbGuiList<Type> & l)
    : itembuffersize(DEFAULTSIZE), numitems(0), itembuffer(builtinbuffer) {
    this->copy(l);
  }

  ~SbGuiList() {
    if (this->itembuffer != builtinbuffer) delete[] this->itembuffer;
  }

  void copy(const SbGuiList<Type> & l) {
    if (this == &l) return;
    const int n = l.numitems;
    this->expand(n);
    for (int i = 0; i < n; i++) this->itembuffer[i] = l.itembuffer[i];
  }

  SbGuiList <Type> & operator=(const SbGuiList<Type> & l) {
    this->copy(l);
    return *this;
  }

  void fit(void) {
    const int items = this->numitems;

    if (items < this->itembuffersize) {
      Type * newitembuffer = this->builtinbuffer;
      if (items > DEFAULTSIZE) newitembuffer = new Type[items];

      if (newitembuffer != this->itembuffer) {
        for (int i = 0; i < items; i++) newitembuffer[i] = this->itembuffer[i];
      }

      if (this->itembuffer != this->builtinbuffer) delete[] this->itembuffer;
      this->itembuffer = newitembuffer;
      this->itembuffersize = items > DEFAULTSIZE ? items : DEFAULTSIZE;
    }
  }

  void append(const Type item) {
    if (this->numitems == this->itembuffersize) this->grow();
    this->itembuffer[this->numitems++] = item;
  }

  int find(const Type item) const {
    for (int i = 0; i < this->numitems; i++)
      if (this->itembuffer[i] == item) return i;
    return -1;
  }

  void insert(const Type item, const int insertbefore) {
    assert(insertbefore >= 0 && insertbefore <= this->numitems);
    if (this->numitems == this->itembuffersize) this->grow();

    for (int i = this->numitems; i > insertbefore; i--)
      this->itembuffer[i] = this->itembuffer[i-1];
    this->itembuffer[insertbefore] = item;
    this->numitems++;
  }

  void removeItem(const Type item) {
    int idx = this->find(item);
    assert(idx != -1);
    this->remove(idx);
  }

  void remove(const int index) {
    assert(index >= 0 && index < this->numitems);
    this->numitems--;
    for (int i = index; i < this->numitems; i++)
      this->itembuffer[i] = this->itembuffer[i + 1];
  }

  void removeFast(const int index) {
    assert(index >= 0 && index < this->numitems);
    this->itembuffer[index] = this->itembuffer[--this->numitems];
  }

  int getLength(void) const {
    return this->numitems;
  }

  void truncate(const int length, const int fit = 0) {
    assert(length <= this->numitems);
    this->numitems = length;
    if (fit) this->fit();
  }

  void push(const Type item) {
    this->append(item);
  }

  Type pop(void) {
    assert(this->numitems > 0);
    return this->itembuffer[--this->numitems];
  }

  const Type * getArrayPtr(const int start = 0) const {
    return &this->itembuffer[start];
  }

  Type operator[](const int index) const {
    assert(index >= 0 && index < this->numitems);
    return this->itembuffer[index];
  }

  Type & operator[](const int index) {
    assert(index >= 0 && index < this->numitems);
    return this->itembuffer[index];
  }

  int operator==(const SbGuiList<Type> & l) const {
    if (this == &l) return TRUE;
    if (this->numitems != l.numitems) return FALSE;
    for (int i = 0; i < this->numitems; i++)
      if (this->itembuffer[i] != l.itembuffer[i]) return FALSE;
    return TRUE;
  }

  int operator!=(const SbGuiList<Type> & l) const {
    return !(*this == l);
  }

protected:

  void expand(const int size) {
    this->grow(size);
    this->numitems = size;
  }

  int getArraySize(void) const {
    return this->itembuffersize;
  }

private:
  void grow(const int size = -1) {
    // Default behavior is to double array size.
    if (size == -1) this->itembuffersize <<= 1;
    else if (size <= this->itembuffersize) return;
    else { this->itembuffersize = size; }

    Type * newbuffer = new Type[this->itembuffersize];
    const int n = this->numitems;
    for (int i = 0; i < n; i++) newbuffer[i] = this->itembuffer[i];
    if (this->itembuffer != this->builtinbuffer) delete[] this->itembuffer;
    this->itembuffer = newbuffer;
  }

  int itembuffersize;
  int numitems;
  Type * itembuffer;
  Type builtinbuffer[DEFAULTSIZE];
};

#endif // !SOGUI_SBGUILIST_H

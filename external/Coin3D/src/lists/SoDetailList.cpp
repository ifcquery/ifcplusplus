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

#include <Inventor/lists/SoDetailList.h>
#include <Inventor/details/SoDetail.h>

/*!
  \class SoDetailList SoDetailList.h Inventor/lists/SoDetailList.h
  \brief The SoDetailList class is a container for pointers to SoDetail objects.
  \ingroup details

  This list class will delete the details when destructed/truncated,
  or when a detail in the list is replaced by another detail. The
  caller is responsible for allocating the details passed to the list,
  but should not deallocate them since this will be handled by the
  list.
  
  \sa SbPList
*/

/*!
  \fn SoDetailList::SoDetailList(void)

  Default constructor.
*/

/*!
  \fn SoDetailList::SoDetailList(const int sizehint)

  This constructor initializes the internal allocated size for the
  list to \a sizehint. Note that the list will still initially contain
  zero items.

  \sa SbList::SbList(const int sizehint)
*/

/*!
  \fn SoDetail * SoDetailList::operator[](const int idx) const

  Returns element at \a idx.

  Will automatically expand the size of the internal array if \a idx
  is outside the current bounds of the list. The values of any
  additional pointers are then set to \c NULL.
*/


/*!
  Copy constructor.
*/
SoDetailList::SoDetailList(const SoDetailList & l)
  : SbPList(l.getLength())
{
  this->copy(l);
}

/*!
  Destructor.
*/
SoDetailList::~SoDetailList()
{
  this->truncate(0);
}

/*!
  Overridden to delete truncated items.
*/ 
void 
SoDetailList::truncate(const int length, const int fit) 
{
  int oldlen = this->getLength();
  
  for (int i = length; i < oldlen; i++) {
    delete (*this)[i];
  }
  SbPList::truncate(length, fit);
}

/*!
  Overridden to copy items, not just pointers.
*/
void 
SoDetailList::copy(const SoDetailList & l)
{
  this->truncate(0);
  for (int i = 0; i < l.getLength(); i++) {
    this->append(l[i]->copy());
  }
}

/*!
  Overridden to delete old item.
*/
void
SoDetailList::set(const int idx, SoDetail * detail) 
{
  if (idx < this->getLength()) delete (*this)[idx];
  SbPList::operator[](idx) = (void*) detail;
}

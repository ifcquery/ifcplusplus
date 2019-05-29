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

// @configure_input@

#if SO@GUI@_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SO@GUI@_DEBUG

/**************************************************************************
 * IMPORTANT NOTICE
 * Be aware that the file So@Gui@MaterialListBuiltins.cpp is generated from
 * the file So@Gui@MaterialListBuiltins.cpp.m4, so changes has to be done in
 * the source file with the m4 macros.  The generation is done manually,
 * since it is no point in making this project depend on the user having
 * m4 installed.
 **************************************************************************/

#include <assert.h>
#include <string.h>

#include <Inventor/@Gui@/SoAnyMaterialList.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef WITH_STATIC_DEFAULTS
#include <materials/materials.h>
#endif // WITH_STATIC_DEFAULTS

include(libm4.m4)
divert(0)dnl
// *************************************************************************
define([SOANY_BUILTIN_MATERIAL_GROUPS], [13])dnl
define([SOANY_BUILTIN_MATERIAL_GROUP_SIZE], [35])dnl

#ifdef WITH_STATIC_DEFAULTS
static
const char *
changequote(,)dnl
builtin_material_names[] = {
changequote([,])dnl
define([matnum], 0)dnl
m4_foreach([material], [artdeco, autumn, glass, metal, neon, rococo, santafe,
                        sheen, silky, spring, summer, tropical, winter], [dnl
  "material",
m4_for([num], 0, m4_eval(SOANY_BUILTIN_MATERIAL_GROUP_SIZE-1),,
[dnl
    "material.num",
])dnl
define([matnum], m4_eval(matnum+1))dnl
])dnl
  NULL
}; // builtin_material_names
#endif // WITH_STATIC_DEFAULTS

// *************************************************************************

/*!
  \internal

  This method fills in the So@Gui@MaterialDirectory structure with the builtin
  material data.
*/

void
SoAnyMaterialList::setupBuiltinMaterials( // private
  So@Gui@MaterialDirectory * const index ) const
{
  assert( index != NULL );
  index->numGroups = 0;
  index->groups = NULL;
#ifdef WITH_STATIC_DEFAULTS
changequote(,)dnl
  index->flags |= SO@GUI@_BUILTIN_MATERIALS;
  index->numGroups = SOANY_BUILTIN_MATERIAL_GROUPS;
  index->groups = new So@Gui@MaterialGroup * [ SOANY_BUILTIN_MATERIAL_GROUPS ];
  for ( int i = 0; i < index->numGroups; i++ ) {
    index->groups[i] = new So@Gui@MaterialGroup;
    index->groups[i]->numMaterials = SOANY_BUILTIN_MATERIAL_GROUP_SIZE;
    index->groups[i]->materials = new So@Gui@Material * [ SOANY_BUILTIN_MATERIAL_GROUP_SIZE ];
    for ( int j = 0; j < SOANY_BUILTIN_MATERIAL_GROUP_SIZE; j++ ) {
      index->groups[i]->materials[j] = new So@Gui@Material;
    }
  }

changequote([,])dnl
define([matnum], 0)dnl
m4_foreach([material], [artdeco, autumn, glass, metal, neon, rococo, santafe,
                        sheen, silky, spring, summer, tropical, winter], [dnl
changequote(<,>)dnl
  index->groups[matnum]->name = builtin_material_names[m4_eval(matnum*(SOANY_BUILTIN_MATERIAL_GROUP_SIZE+1))];
changequote([,])dnl
m4_for([num], 0, m4_eval(SOANY_BUILTIN_MATERIAL_GROUP_SIZE-1),,
[changequote(<,>)dnl
  index->groups[matnum]->materials[num]->name = builtin_material_names[m4_eval(matnum*(SOANY_BUILTIN_MATERIAL_GROUP_SIZE+1)+num+1)];
  index->groups[matnum]->materials[num]->data = material<>num<>_iv;
changequote([,])dnl
])dnl
define([matnum], m4_eval(matnum+1))dnl
])dnl
#endif // ! WITH_STATIC_DEFAULTS
} // setupBuiltinMaterials()

// *************************************************************************

#if SO@GUI@_DEBUG
static const char * getSoAnyMaterialListBuiltinsRCSId(void) { return rcsid; }
#endif // SO@GUI@_DEBUG


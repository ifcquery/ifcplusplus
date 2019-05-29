#include <afxwin.h>

#include <assert.h>

#include <Ivf/IvfArchiveInput.h>

// *************************************************************************

CIvfArchiveInput::CIvfArchiveInput(void)
{
}

CIvfArchiveInput::CIvfArchiveInput(CArchive * archive)
{
}

CIvfArchiveInput::~CIvfArchiveInput(void)
{
}

// *************************************************************************

SbBool
CIvfArchiveInput::get(char & c) // virtual
{
  return FALSE;
}

SbBool
CIvfArchiveInput::read(char & c) // virtual
{
  return FALSE;
}

SbBool
CIvfArchiveInput::read(SbString & str) // virtual
{
  return FALSE;
}

SbBool
CIvfArchiveInput::read(SbName & name, SbBool valid) // virtual
{
  return FALSE;
}

SbBool
CIvfArchiveInput::read(int & num) // virtual
{
  return FALSE;
}

SbBool
CIvfArchiveInput::read(unsigned int & num) // virtual
{
  return FALSE;
}

SbBool
CIvfArchiveInput::read(short & num) // virtual
{
  return FALSE;
}

SbBool
CIvfArchiveInput::read(unsigned short & num) // virtual
{
  return FALSE;
}

SbBool
CIvfArchiveInput::read(float & num) // virtual
{
  return FALSE;
}

SbBool
CIvfArchiveInput::read(double & num) // virtual
{
  return FALSE;
}

SbBool
CIvfArchiveInput::readBinaryArray(int32_t * nums, int length) // virtual
{
  return FALSE;
}

SbBool
CIvfArchiveInput::readBinaryArray(float * nums, int length) // virtual
{
  return FALSE;
}

SbBool
CIvfArchiveInput::readBinaryArray(double * nums, int length) // virtual
{
  return FALSE;
}

SbBool
CIvfArchiveInput::eof(void) const // virtual
{
  return FALSE;
}

void
CIvfArchiveInput::putBack(char c) // virtual
{
}

void
CIvfArchiveInput::putBack(const char * string) // virtual
{
}

SbBool
CIvfArchiveInput::getASCIIFile(char & c) // virtual
{
  return FALSE;
}

// *************************************************************************

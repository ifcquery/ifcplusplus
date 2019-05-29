#include <afxwin.h>

#include <assert.h>

#include <Ivf/IvfArchiveOutput.h>

// *************************************************************************

CIvfArchiveOutput::CIvfArchiveOutput(void)
{
}

CIvfArchiveOutput::CIvfArchiveOutput(CArchive * archive)
{
}

CIvfArchiveOutput::~CIvfArchiveOutput(void)
{
}

// *************************************************************************

CArchive *
CIvfArchiveOutput::getArchive(void)
{
  return NULL;
}

void
CIvfArchiveOutput::closeFile(void)
{
}

void
CIvfArchiveOutput::flushFile(void)
{
}

void
CIvfArchiveOutput::write(char c)
{
}

void
CIvfArchiveOutput::write(const char * str)
{
}

void
CIvfArchiveOutput::write(const SbString & str)
{
}

void
CIvfArchiveOutput::write(const SbName & name)
{
}

void
CIvfArchiveOutput::write(int num)
{
}

void
CIvfArchiveOutput::write(unsigned int num)
{
}

void
CIvfArchiveOutput::write(short num)
{
}

void
CIvfArchiveOutput::write(unsigned short num)
{
}

void
CIvfArchiveOutput::write(float num)
{
}

void
CIvfArchiveOutput::write(double num)
{
}

void
CIvfArchiveOutput::writeBinaryArray(int32_t * nums, int len)
{
}

void
CIvfArchiveOutput::writeBinaryArray(float * nums, int len)
{
}

void
CIvfArchiveOutput::writeBinaryArray(double * nums, int len)
{
}

// *************************************************************************

#ifndef IVF_ARCHIVEOUTPUT_H
#define IVF_ARCHIVEOUTPUT_H

class CArchive;

#include <Inventor/SoOutput.h>

#include <Ivf/IvfBasic.h>

#include <IvfEnterScope.h>

class IVF_DLL_API CIvfArchiveOutput : public SoOutput
{
public:
  CIvfArchiveOutput(void);
  CIvfArchiveOutput(CArchive * archive);
  virtual ~CIvfArchiveOutput(void);

  CArchive * getArchive(void);

  virtual void closeFile(void);
  virtual void flushFile(void);

  virtual void write(char c);
  virtual void write(const char * str);
  virtual void write(const SbString & str);
  virtual void write(const SbName & name);
  virtual void write(int num);
  virtual void write(unsigned int num);
  virtual void write(short num);
  virtual void write(unsigned short num);
  virtual void write(float num);
  virtual void write(double num);
  virtual void writeBinaryArray(int32_t * nums, int len);
  virtual void writeBinaryArray(float * nums, int len);
  virtual void writeBinaryArray(double * nums, int len);

};

#include <IvfLeaveScope.h>

#endif // !IVF_ARCHIVEOUTPUT_H

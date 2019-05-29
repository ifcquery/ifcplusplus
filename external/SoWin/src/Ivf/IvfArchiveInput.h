#ifndef IVF_ARCHIVEINPUT_H
#define IVF_ARCHIVEINPUT_H

class CArchive;

#include <Inventor/SoInput.h>

#include <Ivf/IvfBasic.h>

#include <IvfEnterScope.h>

class IVF_DLL_API CIvfArchiveInput : public SoInput
{
public:
  CIvfArchiveInput(void);
  CIvfArchiveInput(CArchive * archive);
  virtual ~CIvfArchiveInput(void);

  virtual SbBool get(char & c);
  virtual SbBool read(char & c);
  virtual SbBool read(SbString & str);
  virtual SbBool read(SbName & name, SbBool valid = FALSE);
  virtual SbBool read(int & num);
  virtual SbBool read(unsigned int & num);
  virtual SbBool read(short & num);
  virtual SbBool read(unsigned short & num);
  virtual SbBool read(float & num);
  virtual SbBool read(double & num);
  virtual SbBool readBinaryArray(int32_t * nums, int length);
  virtual SbBool readBinaryArray(float * nums, int length);
  virtual SbBool readBinaryArray(double * nums, int length);
  virtual SbBool eof(void) const;
  virtual void putBack(char c);
  virtual void putBack(const char * string);
  virtual SbBool getASCIIFile(char & c);

};

#include <IvfLeaveScope.h>

#endif // !IVF_ARCHIVEINPUT_H

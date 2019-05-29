#ifndef IVF_RESOURCE_H
#define IVF_RESOURCE_H

#include <afxwin.h>

#include <Ivf/IvfBasic.h>

#include <IvfEnterScope.h>

class IVF_DLL_API IvfResource
{
public:
  static void initialize(void);

  static LPCTSTR getString(int id);
  static void freeStrings(void);
  static HGLOBAL getStringCopy(int id);
  static HMENU getMenu(int id);

public:
  static HMODULE m_hResDll;

};

#include <IvfLeaveScope.h>

#endif // !IVF_RESOURCE_H

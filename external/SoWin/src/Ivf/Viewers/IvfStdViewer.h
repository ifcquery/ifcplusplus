#ifndef IVF_STDVIEWER_H
#define IVF_STDVIEWER_H

class SoWinFullViewer;

#include <Ivf/IvfBasic.h>
#include <Ivf/IvfComponent.h>

#include <IvfEnterScope.h>

class IVF_DLL_API CIvfStdViewer : public CIvfComponent
{
  IVF_COMPONENT_BASE();

public:
  CIvfStdViewer(void);

protected:
  virtual ~CIvfStdViewer(void);

protected:
  SoWinFullViewer * m_pViewer;

};

#include <IvfLeaveScope.h>

#endif // !IVF_STDVIEWER_H

#ifndef IVF_RENDERAREA_H
#define IVF_RENDERAREA_H

class SoCamera;
class SoDirectionalLight;
class SoWinRenderArea;

#include <Ivf/IvfBasic.h>
#include <Ivf/IvfComponent.h>

#include <IvfEnterScope.h>

class IVF_DLL_API CIvfRenderArea : public CIvfComponent
{
  IVF_COMPONENT_BASE();

public:
  CIvfRenderArea(void);

  void IvfSpinText(const char * text);

protected:
  virtual ~CIvfRenderArea(void);

protected:
  SoWinRenderArea * m_pViewer;
  SoCamera * m_pCamera;
  SoDirectionalLight * m_pLight;

};

#include <IvfLeaveScope.h>

#endif // !IVF_RENDERAREA_H

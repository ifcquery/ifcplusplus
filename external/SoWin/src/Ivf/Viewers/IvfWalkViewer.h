#ifndef IVF_WALKVIEWER_H
#define IVF_WALKVIEWER_H

#include <Ivf/IvfBasic.h>
#include <Ivf/Viewers/IvfStdViewer.h>

#include <IvfEnterScope.h>

class IVF_DLL_API CIvfWalkViewer : public CIvfStdViewer {
public:
  CIvfWalkViewer(void);

  virtual void IvfCreateComponent(CWnd * window, void * createargs);
  virtual void IvfDestroyComponent(void);

protected:
  virtual ~CIvfWalkViewer(void);

};

#include <IvfLeaveScope.h>

#endif // !IVF_WALKVIEWER_H

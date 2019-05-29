#ifndef IVF_EXAMINERVIEWER_H
#define IVF_EXAMINERVIEWER_H

#include <Ivf/IvfBasic.h>
#include <Ivf/Viewers/IvfStdViewer.h>

#include <IvfEnterScope.h>

class IVF_DLL_API CIvfExaminerViewer : public CIvfStdViewer
{
public:
  CIvfExaminerViewer(void);

  virtual void IvfCreateComponent(CWnd * window, void * createargs);
  virtual void IvfDestroyComponent(void);

protected:
  virtual ~CIvfExaminerViewer(void);

};

#include <IvfLeaveScope.h>

#endif // IVF_EXAMINERVIEWER_H

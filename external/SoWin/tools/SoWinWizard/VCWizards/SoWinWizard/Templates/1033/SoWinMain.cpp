// SoWinMain.cpp : Defines the entry point for the application
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCone.h>

int main(int argc, char* argv[])
{
  HWND myWindow = SoWin::init(argv[0]);
  if (myWindow == NULL) exit(1);

  SoSeparator *root = new SoSeparator;
  root->ref();

  root->addChild(new SoCone);

  SoWinExaminerViewer* myViewer = new SoWinExaminerViewer(myWindow);
  myViewer->setSceneGraph(root);
  myViewer->setTitle("SoWin application");
  myViewer->show();

  SoWin::show(myWindow);
  SoWin::mainLoop();

  delete myViewer;
  root->unref();

  return 0;
}


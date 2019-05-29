// This example demonstrates how to set up and SoWin renderarea within
// a "stand-alone" application window.

#include <stdlib.h> // exit()
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/SoWinRenderArea.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSphere.h>

// Dummy message handler for 2nd window
LRESULT CALLBACK
WindowFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch(message) {
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hwnd, message, wParam, lParam);
  }
  return 0;
}


int
main(int argc, char ** argv)
{
  WNDCLASSEX wcl;
  wcl.cbSize = sizeof(WNDCLASSEX);
  wcl.hInstance = NULL;
  wcl.lpszClassName = "AppWindow";
  wcl.lpfnWndProc = WindowFunc;
  wcl.style = 0;
  wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
  wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcl.lpszMenuName = NULL;
  wcl.cbClsExtra = 0;
  wcl.cbWndExtra = 0;
  wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
  if (!RegisterClassEx(&wcl)) { exit(1); }
  HWND appwin = CreateWindow("AppWindow",
                             "AppWindow", // title
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             HWND_DESKTOP,
                             NULL,
                             (HINSTANCE)NULL,
                             NULL);


  SoWin::init(appwin);

  // Create a scene cointaining a sphere
  SoSeparator * root = new SoSeparator;
  root->ref(); // increment the root's reference counter

  SoPerspectiveCamera * camera = new SoPerspectiveCamera;
  SoDirectionalLight * light = new SoDirectionalLight;
  SoMaterial * material = new SoMaterial;
  SoSphere * sphere = new SoSphere;

  root->addChild(camera); // add camera node to the scene graph
  root->addChild(light); // add directional light to the scene
  root->addChild(material); // add material (with default settings)
  root->addChild(sphere); // add sphere node to the scene graph

  // Create a renderingarea which will be used to display the
  // scene graph in the window.
  SoWinRenderArea * renderarea = new SoWinRenderArea(appwin);

  // Make the camera able to see the whole scene
  camera->viewAll(root, renderarea->getViewportRegion());

  // Display the scene in our renderarea and change the title
  renderarea->setSceneGraph(root);
  renderarea->setTitle("Sphere");
  renderarea->show();

  SoWin::show(appwin);  //display the main window
  SoWin::mainLoop();  //main Coin event loop
  delete renderarea;  //free up the resources occupied by the renderarea

  root->unref();  //decrement the root's reference counter
  return 0;
}

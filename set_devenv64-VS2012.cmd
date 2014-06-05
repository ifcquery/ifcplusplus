set OSG_DIR=D:\lib\OSG\3.2
set OSG_LIBRARY_PATH=%OSG_DIR%\bin
set OSG_PLUGINS_LIB=%OSG_DIR%\lib\osgPlugins-3.2
set OSG_FILE_PATH=D:\lib\OSG\OpenSceneGraph-Data;D:\lib\OSG\OpenSceneGraph-Data\Images;D:\lib\OSG\OpenSceneGraph-Data\fonts;

set QMAKESPEC=win32-msvc2012
set QMAKE_TARGET.arch=x86_64
set QTDIR=D:\lib\Qt\5.2.0\5.2.0\msvc2012_64_opengl

PATH=%OSG_DIR%\bin;%PATH%
PATH=%OSG_DIR%\bin\osgPlugins-3.2;%PATH%
PATH=%QTDIR%\bin;%PATH%
PATH=%QTDIR_MT%\bin;%PATH%
PATH=D:\work\OpenSees\bin;%PATH%

call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x86_amd64

d:
cd work\




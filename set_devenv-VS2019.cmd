set LIB_DIR=D:\lib
set OSG_DIR=%LIB_DIR%\OSG\currentVersion
set OSG_LIBRARY_PATH=%OSG_DIR%\bin
set OSG_PLUGINS_LIB=%OSG_DIR%\lib\osgPlugins-3.6.5
set OSG_FILE_PATH=%LIB_DIR%\OSG\OpenSceneGraph-Data;%LIB_DIR%\OSG\OpenSceneGraph-Data\Images;%LIB_DIR%\OSG\OpenSceneGraph-Data\fonts;

set QMAKESPEC=win32-msvc
set QMAKE_TARGET.arch=x86_64
set QTDIR=%LIB_DIR%\Qt\5.14\5.14.1\msvc2017_64

set IFCPP_DIR=D:\work\IfcPlusPlus\

@echo off
PATH=%OSG_DIR%\bin;%PATH%
PATH=%OSG_PLUGINS_LIB%;%PATH%
PATH=%QTDIR%\bin;%PATH%
PATH=%QTDIR%\qtbase\bin;%PATH%
PATH=%QTDIR%\gnuwin32\bin;%PATH%

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
cd /D D:\work\
devenv

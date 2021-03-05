set LIB_DIR=D:\lib
set OSG_DIR=%LIB_DIR%\OSG\currentVersion
set OSG_LIBRARY_PATH=%OSG_DIR%\bin
set OSG_PLUGINS_LIB=%OSG_DIR%\lib\osgPlugins-3.6.5
set OSG_FILE_PATH=%LIB_DIR%\OSG\OpenSceneGraph-Data;%LIB_DIR%\OSG\OpenSceneGraph-Data\Images;%LIB_DIR%\OSG\OpenSceneGraph-Data\fonts;

set OCCT_DIR=%LIB_DIR%\opencascade\7.4.0\
set FREEIMAGE_DIR=%LIB_DIR%\opencascade\freeimage-3.17.0-vc14-64
set FREETYPE_DIR=%LIB_DIR%\freetype\2.10.2

set TCL_DIR=%LIB_DIR%\tcltk-86-64
set VTK_LIB_SUFFIX=9.0
set VTK_DIR=%LIB_DIR%\VTK-9.0.0
set PYTHONPATH=C:\Users\Gerold\AppData\Local\Programs\Python\Python35-32\

set wxWidgets_ROOT_DIR=%LIB_DIR%\wxWidgets-3.1.3
set wxWidgets_LIB_DIR=%LIB_DIR%\wxWidgets-3.1.3\build\lib\vc_x64_dll

set CSF_OPT_INC=%OCCT_DIR%\inc\;%FREETYPE_DIR%\include;%TCL_DIR%\include;%VTK_DIR%\include;%VTK_DIR%\build\include;
set CSF_OPT_LIB64=%FREETYPE_DIR%\build\Release;%TCL_DIR%\lib;%VTK_DIR%\lib;%FREEIMAGE_DIR%\lib;%OCCT_DIR%..\gl2ps-1.3.8-vc14-64\lib;%OCCT_DIR%..\ffmpeg-20181007-0a41a8b-win64-dev\lib
set CSF_OPT_LIB64D=%FREETYPE_DIR%\build\Debug;%TCL_DIR%\lib;%VTK_DIR%\lib;%FREEIMAGE_DIR%\lib;%OCCT_DIR%..\gl2ps-1.3.8-vc14-64\lib;%OCCT_DIR%..\ffmpeg-20181007-0a41a8b-win64-dev\lib
set CSF_ShadersDirectory=%OCCT_DIR%\src\Shaders
set DRAWDEFAULT=%OCCT_DIR%\data\occ\Ball.brep

rem set BOOST_DIR=%LIB_DIR%\boost\1_74_0\
set Boost_DIR=%LIB_DIR%\boost\1_75_0\
set Boost_ROOT=D:\lib\boost\1_75_0\
set Boost_INCLUDE_DIR=%LIB_DIR%\boost\1_74_0\

set QMAKESPEC=win32-msvc
set QMAKE_TARGET.arch=x86_64
set QTDIR=%LIB_DIR%\Qt\5.14\5.14.1\msvc2017_64

set IFCPP_DIR=D:\work\IfcPlusPlus\

set COINDIR=%IFCPP_DIR%external\Coin3D
set Coin_DIR=%IFCPP_DIR%external\Coin3D

@echo off
PATH=%OSG_DIR%\bin;%PATH%
PATH=%OSG_PLUGINS_LIB%;%PATH%
PATH=%QTDIR%\bin;%PATH%
PATH=%QTDIR%\qtbase\bin;%PATH%
PATH=%QTDIR%\gnuwin32\bin;%PATH%
PATH=%LIB_DIR%\depends22_x64\;%PATH%
PATH=C:\Program Files (x86)\WiX Toolset v3.11\bin;%PATH%
PATH=%OCCT_DIR%\win64\vc14\bin;%PATH%
PATH=%OCCT_DIR%\win64\vc14\bind;%PATH%
PATH=%OCCT_DIR%\win64\vc12\bin;%PATH%
PATH=%TBB_DIR%\bin\intel64\vc12;%PATH%
PATH=%FREEIMAGE_DIR%\bin\;%PATH%
PATH=%FREETYPE_DIR%\bin\;%PATH%
PATH=%GL2PS_DIR%\bin\;%PATH%
PATH=%TCL_DIR%\bin\;%PATH%

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64

rem cd /D D:\work\
rem cd ..
cd "C:\Program files\cmake\bin"

devenv


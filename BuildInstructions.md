# Build instructions #



## Windows ##

For VS IDE proceed as follows:

  * Install Qt (Desktop OpenGL version)
  * Download boost
  * Download OpenSceneGraph, run CMake to build it and set the paths to Qt
  * Set environment variables for boost, Qt, OSG and your development environment with a script like this: [set\_devenv64-VS2013.cmd](http://ifcplusplus.googlecode.com/svn/trunk/set_devenv64-VS2013.cmd).
  * Install Qt VS addin, configure it to use the same Qt files like the environment variable QTDIR
  * Type "devenv" to start VS.
  * Hit F7 to compile, F5 to run the debugger.

Tested with
  * Qt 5.4 (qt-opensource-windows-x86-msvc2013\_64\_opengl-5.4.0.exe)
  * OpenSceneGraph 3.2.1rc3 (http://svn.openscenegraph.org/osg/OpenSceneGraph/tags/OpenSceneGraph-3.2.1-rc3)
  * Visual Studio 2013

For other IDE's, use CMake like the Linux version below.


---

## Linux ##
CMake files and build instructions below are kindly provided by Bernd Hahnebach, bernd(at)bimstatik(dot)org


### Packagelist ###

  * Tested on Debian Jessie
  * $ apt-get install cmake subversion
  * $ apt-get install libboost-dev libopenscenegraph-dev libqt4-dev

Internal Carve of IfcPlusPlus is used.


### Build IfcPLusPlus ###
Get source code and build as follows:
  * $ svn checkout http://ifcplusplus.googlecode.com/svn/trunk/ ifcplusplus
  * $ cd ifcplusplus
  * $ mkdir build
  * $ cd build
  * $ cmake ../
  * $ make
  * $ cd Release
  * $ ./IfcPlusPlusViewer

Yeah, it runs on Linux !!!
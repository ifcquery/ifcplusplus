## Overview ##
Small and efficient Open Source C++ library for IFC (Industry Foundation Classes).

Features include
  * Class model for IFC4, generated from EXPRESS schema
  * Reader and writer for IFC STEP files
  * Reader is backward compatible to IFC 2X3
  * Smart pointers provide easy memory management
  * Very fast parallel parsing on multi-core cpu's
  * Converts IFC geometry items into a vertex-edge-face graph
  * Uses [carve](http://code.google.com/p/carve/) for fast and robust csg modeling
  * OpenSceneGraph plugin
  * Stateset caching and re-use for optimized draw traversal
  * Qt example viewer application

Developed and tested with Qt 5.2, OpenSceneGraph 3.2, VS 2012 x64.

You can find IFC files for testing at
[www.ifcwiki.org](http://www.ifcwiki.org/index.php/Examples)


---

IfcPlusPlus consists of 3 parts:
  * IfcPlusPlus - class model, reader/writer, library (requires boost)
  * GeometryConverter - OpenSceneGraph plugin, library (requires [OpenSceneGraph](http://www.openscenegraph.org))
  * BasicViewer - viewer application (requires [Qt](http://www.qtsoftware.com) and [OpenSceneGraph](http://www.openscenegraph.org))




---

## License ##
This software can be used under the OSGPL license, which means that you can use it for commercial and non-commercial purposes. The license requires you to publish only the changes that you make in IFC++, not your own source code.

## Download ##
svn checkout https://ifcplusplus.googlecode.com/svn/trunk/


---

## Build instructions ##
If you want to build the binaries by yourself, see the [build instructions page](BuildInstructions.md)



---

## Extender application ##
If you want support of (EXPRESS) schema files other than IFC4, you can use the extender application to generate the source code.

Download: [IfcExtenderInstaller\_64bit.msi](http://www.ifcplusplus.com/download/IfcExtenderInstaller_64bit.msi) Installer for Windows operating systems.


---

## IfcQuery ##
IfcQuery is an advanced viewer application based on IFC++, with additional features like WebGL export. The IfcQuery application is not open source, but can be licensed for commercial projects.
Support and development services are also available.

Download IfcQuery application
[IfcQueryInstaller\_64bit.msi](http://www.ifcquery.com/download/IfcQueryInstaller_64bit.msi) Installer for Windows operating systems

or

[IfcQuery.zip](http://www.ifcquery.com/download/IfcQuery.zip) zip file containing binaries for Windows operating systems


## WebGL IFC viewer ##
An example IFC model exported by IfcQuery can be seen here: [www.ifcquery.com](http://www.ifcquery.com)

[![](http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/ifcquery2-700.png)](http://www.ifcquery.com)


---

Example models loaded in open source viewer:

![http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-06a-700.png](http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-06a-700.png)
![http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-06-700.png](http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-06-700.png)
![http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-06wf-700.png](http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-06wf-700.png)

![http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-05-700.png](http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-05-700.png)
![http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-05wf-700.png](http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-05wf-700.png)

![http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-07-700.png](http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-07-700.png)
![http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-07wf-700.png](http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-07wf-700.png)

![http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-08-700.png](http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-08-700.png)

![http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-09-700.png](http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-09-700.png)
![http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-10-700.png](http://ifcplusplus.googlecode.com/svn/trunk/SimpleViewer/Resources/img/IfcPlusPlusViewer-10-700.png)
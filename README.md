ABOUT IFC++
=============

This repository is more or less archived.<br>
If you need support or customization for existing projects, it is still available (please contact info at ifcquery.com)<br>
For new projects, I recommend Web-ifc, since it fulfills the same purpose, plus the ability to run in WASM, see below.<br>



## Web-ifc as alternative to IFC++


As an alternative to IFC++, there is a great project called web-ifc (https://github.com/ThatOpen/engine_web-ifc). 
It does not have an object oriented approach for IFC entities, instead it has a tape reader, so the STEP file content is kept as-is, 
just with tokens inserted before each attribute, which allows positioning the read cursor to read all entities and attributes.<br>

IfcSplitAndMerge is a light weight but high performance application based on web-ifc (not IFC++): https://github.com/ifcquery/IfcSplitAndMerge





## Alternative to IFC++

the IFC library used in IfcSplitAndMerge is called web-ifc (https://github.com/ThatOpen/engine_web-ifc). It does not have an object oriented approach for IFC entities, instead it has a tape reader, so the STEP file content is kept as-is, just with tokens inserted before each attribute, which allows positioning the read cursor to read all entities and attributes.<br>

Web-ifc is so light weight, it can be directly compiled into a C++ console or GUI application, or linked as a library on Windows or Linux. It even compiles and runs efficiently as WebAssembly.<br><br>

If you want to benefit from my experience how to implement web-ifc in various applications, please contact info at ifcquery.com<br><br>


One example of a light weight application based on web-ifc is https://github.com/ifcquery/IfcSplitAndMerge


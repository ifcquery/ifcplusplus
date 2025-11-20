# ABOUT IFC++

This repository is more or less archived.<br>
If you need support or customization for existing projects, it is still available (please contact info at ifcquery.com)<br>
For new projects, I recommend Web-ifc (https://github.com/ThatOpen/engine_web-ifc), since it fulfills the same purpose, CSG operations are more stable, and it can run also in WASM.<br>



## Follow-up and alternative to IFC++

IfcSplitAndMerge is a light weight but high performance application based on web-ifc (not IFC++): https://github.com/ifcquery/IfcSplitAndMerge
<img width="2907" height="1955" alt="IfcSplitAndMerge1" src="https://github.com/user-attachments/assets/6025fbee-fbd1-4177-80cd-f55c3121aa8d" />



## Alternative to IFC++

The IFC library used in IfcSplitAndMerge is called web-ifc https://github.com/ThatOpen/engine_web-ifc. It does not have an object oriented approach for all IFC entities like IFC++. Instead it has a tape reader, so the STEP file content is kept as-is, just with tokens inserted before each attribute, which allows positioning the read cursor to read all entities and attributes with a very low memory footprint and high performance.<br>

Web-ifc is so light weight, it can be directly compiled into a C++ console or GUI application, or linked as a library on Windows or Linux. It even compiles and runs efficiently as WebAssembly.

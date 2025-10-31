ABOUT IFC++
=============

This repository is more or less archived.<br>
If you need support or customization for existing projects, it is still available (please contact info at ifcquery.com)<br>
For new projects, I recommend Web-ifc (see link below), since it fulfills the same purpose, CSG operations are more stable, and it can run also in WASM<br>



## Follow-up and alternative to IFC++

IfcSplitAndMerge is a light weight but high performance application based on web-ifc (not IFC++): https://github.com/ifcquery/IfcSplitAndMerge

<img width="2550" height="1696" alt="Screenshot 2025-09-30 104907" src="https://github.com/user-attachments/assets/eb77f3e0-e6d5-4365-a6a4-09c8894cde45" />


<img width="3839" height="2102" alt="Screenshot 2025-10-03 152408" src="https://github.com/user-attachments/assets/ee656ba8-339d-45a5-9507-baa39567f8d3" />



## Alternative to IFC++

the IFC library used in IfcSplitAndMerge is called web-ifc [github.com/ThatOpen/engine_web-ifc](https://github.com/ThatOpen/engine_web-ifc). It does not have an object oriented approach for IFC entities, instead it has a tape reader, so the STEP file content is kept as-is, just with tokens inserted before each attribute, which allows positioning the read cursor to read all entities and attributes.<br>
Web-ifc is so light weight, it can be directly compiled into a C++ console or GUI application, or linked as a library on Windows or Linux. It even compiles and runs efficiently as WebAssembly.<br><br>


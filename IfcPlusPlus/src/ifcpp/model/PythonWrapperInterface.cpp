#include <new>
#include <ifcpp/IFC4/include/IfcBuildingStorey.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/GlobalDefines.h>
#include <ifcpp/reader/ReaderSTEP.h>
#include <ifcpp/reader/ReaderUtil.h>


extern "C"
{
	// Python interface does not properly support C++ yet, so create a pure C interface.
	// If anyone knows a better solution than this ugly approach, please contribute on github.com/ifcquery/ifcpluplus
	IFCQUERY_EXPORT void* CreateInstanceOfBuildingModel(void)
	{
		// Inside the function body, C++ can be used
		return new(std::nothrow) BuildingModel();
	}

	IFCQUERY_EXPORT void DeleteInstanceOfBuildingModel(void* ptr)
	{
		delete ptr;
	}

	IFCQUERY_EXPORT void CallMember_ResetIfcModel(void* ptr)
	{

		// Note: A downside here is the lack of type safety. 
		// You could always internally(in the C++ library) save a reference to all 
		// pointers created of type MyClass and verify it is an element in that
		//structure. 
		//
		// Per comments with Andre, we should avoid throwing exceptions.  
		try
		{
			BuildingModel* ref = reinterpret_cast<BuildingModel*>(ptr);
			if (ref)
			{
				ref->resetIfcModel();
			}
		}
		catch (...)
		{
			
		}
	}


	IFCQUERY_EXPORT int test(int a, wchar_t* b)
	{
		return a + 3;
	}

	shared_ptr<BuildingModel> ifc_model;

	IFCQUERY_EXPORT const char* loadFile(wchar_t* file_path)
	{
		std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
		std::ostringstream strCout;
		

		ifc_model = shared_ptr<BuildingModel>(new BuildingModel());
		shared_ptr<ReaderSTEP> step_reader(new ReaderSTEP());

		// 2: load the model:
		step_reader->loadModelFromFile( file_path, ifc_model);

		// 3: get a flat map of all loaded IFC entities:
		const std::map<int, shared_ptr<BuildingEntity> >& map_entities = ifc_model->getMapIfcEntities();

		for (auto it : map_entities)
		{
			shared_ptr<BuildingEntity> entity = it.second;

			// check for certain type of the entity:
			shared_ptr<IfcBuildingStorey> ifc_storey = dynamic_pointer_cast<IfcBuildingStorey>(entity);
			if (ifc_storey)
			{
				// access attributes:
				if (ifc_storey->m_GlobalId)
				{
					std::wcout << L"found IfcBuildingStorey entity with GUID: " << ifc_storey->m_GlobalId->m_value << std::endl;
				}
			}
		}

		// 4: traverse tree structure of model, starting at root object (IfcProject)
		shared_ptr<IfcProject> ifc_project = ifc_model->getIfcProject();
		
		std::cout.rdbuf(strCout.rdbuf());
		return "file loaded";
	}

	IFCQUERY_EXPORT const wchar_t* getIfcProject()
	{
		shared_ptr<IfcProject> ifc_project = ifc_model->getIfcProject();
		if (ifc_project)
		{
			if (ifc_project->m_GlobalId)
			{
				return ifc_project->m_GlobalId->m_value.c_str();
				//std::string guid_str = wstring2string(ifc_project->m_GlobalId->m_value);
				//return guid_str.c_str();
			}
		}
		return L"";
	}

	IFCQUERY_EXPORT const wchar_t* getRelatedObjects(const wchar_t* guid)
	{
		const std::map<int, shared_ptr<BuildingEntity> >&  map_ifc_entities = ifc_model->getMapIfcEntities();
		//if (ifc_project)
		{
			//if (ifc_project->m_GlobalId)
			{
				//return ifc_project->m_GlobalId->m_value.c_str();
				//std::string guid_str = wstring2string(ifc_project->m_GlobalId->m_value);
				//return guid_str.c_str();
			}
		}
		return L"";
	}

}

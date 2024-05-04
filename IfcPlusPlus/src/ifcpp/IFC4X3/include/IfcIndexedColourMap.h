/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"
#include "IfcPresentationItem.h"
namespace IFC4X3
{
	class IFCQUERY_EXPORT IfcTessellatedFaceSet;
	class IFCQUERY_EXPORT IfcNormalisedRatioMeasure;
	class IFCQUERY_EXPORT IfcColourRgbList;
	class IFCQUERY_EXPORT IfcPositiveInteger;
	//ENTITY
	class IFCQUERY_EXPORT IfcIndexedColourMap : public IfcPresentationItem
	{
	public:
		IfcIndexedColourMap() = default;
		IfcIndexedColourMap( int id );
		virtual void getStepLine( std::stringstream& stream, size_t precision ) const;
		virtual void getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const;
		virtual void readStepArguments( const std::vector<std::string>& args, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound );
		virtual void setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self );
		virtual uint8_t getNumAttributes() const { return 4; }
		virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
		virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
		virtual void unlinkFromInverseCounterparts();
		virtual uint32_t classID() const { return 3570813810; }

		// IfcPresentationItem -----------------------------------------------------------

		// IfcIndexedColourMap -----------------------------------------------------------
		// attributes:
		shared_ptr<IfcTessellatedFaceSet>				m_MappedTo;
		shared_ptr<IfcNormalisedRatioMeasure>			m_Opacity;					//optional
		shared_ptr<IfcColourRgbList>					m_Colours;
		std::vector<shared_ptr<IfcPositiveInteger> >	m_ColourIndex;
	};
}

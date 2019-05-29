#include "stdafx.h"
#include "BuildingUtils.h"
#include "WindowUtils.h"
#include <ifcpp/reader/ReaderUtil.h>

MyGridProperty::MyGridProperty( shared_ptr<BuildingObject> buildingObject, const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList, bool isInverse )
	: CMFCPropertyGridProperty( strGroupName, dwData, bIsValueList ), m_buildingObject( buildingObject ), m_isInverse(isInverse)
{
	// TODO: make font always regular, disable bold font for items with sub-items
}

MyGridProperty::MyGridProperty( shared_ptr<BuildingObject> buildingObject, const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars, bool isInverse )
	: CMFCPropertyGridProperty( strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars ), m_buildingObject( buildingObject ), m_isInverse( isInverse )
{

}

CMFCPropertyGridProperty* MyGridProperty::HitTest( CPoint point, CMFCPropertyGridProperty::ClickArea* pnArea )
{
	if( pnArea != NULL )
	{
		CMFCPropertyGridProperty::ClickArea clickArea = *pnArea;
		if( clickArea == CMFCPropertyGridProperty::ClickExpandBox )
		{
			std::cout << "hit:" << point.x << "/" << point.y << " + area " << std::endl;
		}
	}
	return this;
}

void MyGridProperty::OnClickButton( CPoint point )
{
	CMFCPropertyGridProperty::OnClickButton( point );

}

void MyGridProperty::OnDrawName( CDC* pDC, CRect rect )
{
	if( m_isInverse )
	{
		pDC->SetTextColor( RGB( 160, 160, 160 ) );
	}
	else
	{
		pDC->SetTextColor( RGB( 20, 20, 220 ) );
	}
	return CMFCPropertyGridProperty::OnDrawName( pDC, rect );
}
void MyGridProperty::OnDrawValue( CDC* pDC, CRect rect )
{
	if( m_isInverse )
	{
		pDC->SetTextColor( RGB( 160, 160, 160 ) );
	}
	else
	{
		pDC->SetTextColor( RGB( 20, 20, 220 ) );
	}
	return CMFCPropertyGridProperty::OnDrawValue( pDC, rect );
}


void ExpandTreeItem( CViewTree &tree, HTREEITEM hItem, UINT nCode )
{
	HTREEITEM hChild;

	if( tree.ItemHasChildren( hItem ) )
	{
		//nCode = TVE_EXPAND;
		tree.Expand( hItem, nCode );
		hChild = tree.GetChildItem( hItem );

		while( hChild )
		{
			ExpandTreeItem( tree, hChild, nCode );
			hChild = tree.GetNextItem( hChild, TVGN_NEXT );
		}
	}
}

void addPropertyItem( std::wstring attributeName, const shared_ptr<BuildingObject>& attributeValue, MyGridProperty* entityGroup, bool addChildren, bool isInverse )
{
	if( !attributeValue )
	{
		entityGroup->AddSubItem( new MyGridProperty( attributeValue, attributeName.c_str(), (_variant_t)"-", _T( "" ), isInverse ) );
		return;
	}

	shared_ptr<IfcGloballyUniqueId> globalID = dynamic_pointer_cast<IfcGloballyUniqueId>(attributeValue);
	if( globalID )
	{
		entityGroup->AddSubItem( new MyGridProperty( attributeValue, attributeName.c_str(), (_variant_t)globalID->m_value.c_str(), _T( "" ), isInverse ) );
		return;
	}

	shared_ptr<IfcIdentifier> identifier = dynamic_pointer_cast<IfcIdentifier>(attributeValue);
	if( identifier )
	{
		entityGroup->AddSubItem( new MyGridProperty( attributeValue, attributeName.c_str(), (_variant_t)identifier->m_value.c_str(), _T( "" ), isInverse ) );
		return;
	}

	shared_ptr<IfcLabel> label = dynamic_pointer_cast<IfcLabel>(attributeValue);
	if( label )
	{
		entityGroup->AddSubItem( new MyGridProperty( attributeValue, attributeName.c_str(), (_variant_t)label->m_value.c_str(), _T( "" ), isInverse ) );
		return;
	}

	shared_ptr<IfcText> text = dynamic_pointer_cast<IfcText>(attributeValue);
	if( text )
	{
		entityGroup->AddSubItem( new MyGridProperty( attributeValue, attributeName.c_str(), (_variant_t)text->m_value.c_str(), _T( "" ), isInverse ) );
		return;
	}

	shared_ptr<IfcReal> real = dynamic_pointer_cast<IfcReal>(attributeValue);
	if( real )
	{
		std::wstring realString = std::to_wstring( real->m_value );
		entityGroup->AddSubItem( new MyGridProperty( attributeValue, attributeName.c_str(), (_variant_t)realString.c_str(), _T( "" ), isInverse ) );
		return;
	}

	shared_ptr<IfcCartesianPoint> cartesianPoint = dynamic_pointer_cast<IfcCartesianPoint>(attributeValue);
	if( cartesianPoint )
	{
		std::stringstream coordinates;

		if( cartesianPoint->m_Coordinates.size() > 1 )
		{
			if( cartesianPoint->m_Coordinates[0] && cartesianPoint->m_Coordinates[1] )
			{
				coordinates << cartesianPoint->m_Coordinates[0]->m_value << "," << cartesianPoint->m_Coordinates[1]->m_value;

				if( cartesianPoint->m_Coordinates.size() > 2 )
				{
					if( cartesianPoint->m_Coordinates[2] )
					{
						coordinates << "," << cartesianPoint->m_Coordinates[2]->m_value;
					}
				}
			}
		}
		std::string coordinatesStr = coordinates.str();
		std::wstring coordinatesWStr = s2ws( coordinatesStr );
		entityGroup->AddSubItem( new MyGridProperty( attributeValue, attributeName.c_str(), (_variant_t)coordinatesWStr.c_str(), _T( "" ), isInverse ) );
		return;
	}

	shared_ptr<IfcDirection> direction = dynamic_pointer_cast<IfcDirection>(attributeValue);
	if( direction )
	{
		std::stringstream directionRatios;

		if( direction->m_DirectionRatios.size() > 1 )
		{
			if( direction->m_DirectionRatios[0] && direction->m_DirectionRatios[1] )
			{
				directionRatios << direction->m_DirectionRatios[0]->m_value << "," << direction->m_DirectionRatios[1]->m_value;

				if( direction->m_DirectionRatios.size() > 2 )
				{
					if( direction->m_DirectionRatios[2] )
					{
						directionRatios << "," << direction->m_DirectionRatios[2]->m_value;
					}
				}
			}
		}
		std::string directionRatiosStr = directionRatios.str();
		std::wstring directionRatiosWStr = s2ws( directionRatiosStr );
		entityGroup->AddSubItem( new MyGridProperty( attributeValue, attributeName.c_str(), (_variant_t)directionRatiosWStr.c_str(), _T( "" ), isInverse ) );
		return;
	}

	shared_ptr<BuildingEntity> attributeEntity = dynamic_pointer_cast<BuildingEntity>(attributeValue);
	if( attributeEntity )
	{
		std::wstringstream wstr;
		wstr << L"#" << attributeEntity->m_entity_id << "=" << attributeEntity->className();
		std::stringstream STEPline;
		attributeEntity->getStepLine( STEPline );
		std::wstring STEPLineWstr = s2ws( STEPline.str() );

		// TODO: second or combined attribute browser like IfcQuickBrowser

		MyGridProperty* entityProperty = new MyGridProperty( attributeValue, attributeName.c_str() );// STEPLineWstr.c_str(), _T( "" ), isInverse );
		entityProperty->m_isInverse = isInverse;
		entityProperty->SetDescription( STEPLineWstr.c_str() );

		entityGroup->AddSubItem( entityProperty );

		if( addChildren )
		{
			addPropertyItems( attributeEntity, entityProperty, false, isInverse );
		}

		return;
	}

	shared_ptr<AttributeObjectVector> attributeVector = dynamic_pointer_cast<AttributeObjectVector>(attributeValue);
	if( attributeVector )
	{
		std::vector<shared_ptr<BuildingObject> >& vec = attributeVector->m_vec;

		std::wstringstream wstrValue;
		//wstr << attributeVector->className() << "(" << vec.size() << ")";
		wstrValue << attributeName.c_str() << "(" << vec.size() << ")";

		std::wstringstream wstrDescription;
		//wstr << attributeVector->className() << "(" << vec.size() << ")";
		wstrDescription << attributeName.c_str() << "[" << vec.size() << "]" ;
		//std::stringstream STEPline;
		//attributeEntity->getStepLine( STEPline );
		//std::wstring STEPLineWstr = s2ws( STEPline.str() );

		// TODO: second or combined attribute browser like IfcQuickBrowser

		MyGridProperty* vecProperty = new MyGridProperty( attributeValue, wstrValue.str().c_str() );// , wstrDescription.str().c_str(), _T( "" ), isInverse );
		//entityProperty->SetDescription( STEPLineWstr.c_str() );
		vecProperty->m_isInverse = isInverse;

		entityGroup->AddSubItem( vecProperty );

		
		for( size_t ii = 0; ii < vec.size(); ++ii )
		{
			shared_ptr<BuildingObject> attributeValue = vec[ii];

			std::wstringstream attributeVecName;
			//attributeVecName << attributeName << L"[" << ii << "]";
			attributeVecName << L"[" << ii << "]" << attributeValue->className();

			std::wstring attributeVecNameStr = attributeVecName.str();// s2ws( attributeVector->m_vec[ii].first );
			
			addPropertyItem( attributeVecNameStr, attributeValue, vecProperty, addChildren, isInverse );
		}
		return;
	}

	std::wstring attributeValueClassName = s2ws( attributeValue->className() );
	entityGroup->AddSubItem( new MyGridProperty( attributeValue, attributeName.c_str(), (_variant_t)attributeValueClassName.c_str(), _T( "" ), isInverse ) );
}

void addPropertyItems( const shared_ptr<BuildingEntity>& entity, MyGridProperty* entityGroup, bool addChildren, bool isInverse )
{
	if( !entity )
	{
		return;
	}
	std::vector<std::pair<std::string, shared_ptr<BuildingObject> > > vec_attributes;
	entity->getAttributes( vec_attributes );

	std::vector<std::pair<std::string, shared_ptr<BuildingObject> > > vec_attributes_inverse;
	entity->getAttributesInverse( vec_attributes_inverse );

	for( size_t ii = 0; ii < vec_attributes.size(); ++ii )
	{
		std::wstring attributeName = s2ws( vec_attributes[ii].first );
		shared_ptr<BuildingObject> attributeValue = vec_attributes[ii].second;
		addPropertyItem( attributeName, attributeValue, entityGroup, addChildren, false );
	}

	for( size_t ii = 0; ii < vec_attributes_inverse.size(); ++ii )
	{
		std::wstring attributeName = s2ws( vec_attributes_inverse[ii].first );
		shared_ptr<BuildingObject> attributeValue = vec_attributes_inverse[ii].second;
		addPropertyItem( attributeName, attributeValue, entityGroup, addChildren, true );
	}
}

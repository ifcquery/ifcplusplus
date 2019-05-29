#pragma once

#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcDirection.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4/include/IfcIdentifier.h>
#include <ifcpp/IFC4/include/IfcLabel.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcReal.h>
#include <ifcpp/IFC4/include/IfcText.h>
#include <ifcpp/model/AttributeObject.h>
#include "ViewTree.h"

class MyGridProperty : public CMFCPropertyGridProperty
{
public:
	MyGridProperty( shared_ptr<BuildingObject> buildingObject, const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE, bool isInverse = false );
	MyGridProperty( shared_ptr<BuildingObject> buildingObject, const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0, LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL, bool isInverse = false );

	CMFCPropertyGridProperty* HitTest( CPoint point, CMFCPropertyGridProperty::ClickArea* pnArea = NULL );
	virtual void OnClickButton( CPoint point );
	virtual void OnDrawName( CDC* pDC, CRect rect );
	virtual void OnDrawValue( CDC* pDC, CRect rect );

	shared_ptr<BuildingObject> m_buildingObject;
	bool m_isInverse = false;
};

void ExpandTreeItem( CViewTree &tree, HTREEITEM hItem, UINT nCode );

void addPropertyItems( const shared_ptr<BuildingEntity>& entity, MyGridProperty* entityGroup, bool addChildren, bool isInverse );
void addPropertyItem( std::wstring attributeName, const shared_ptr<BuildingObject>& attributeValue, MyGridProperty* entityGroup, bool addChildren, bool isInverse );


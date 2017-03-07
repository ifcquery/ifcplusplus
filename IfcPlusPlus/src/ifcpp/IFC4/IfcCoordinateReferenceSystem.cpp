/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcCoordinateOperation.h"
#include "include/IfcCoordinateReferenceSystem.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcText.h"

// ENTITY IfcCoordinateReferenceSystem 
IfcCoordinateReferenceSystem::IfcCoordinateReferenceSystem() { m_entity_enum = IFCCOORDINATEREFERENCESYSTEM; }
IfcCoordinateReferenceSystem::IfcCoordinateReferenceSystem( int id ) { m_id = id; m_entity_enum = IFCCOORDINATEREFERENCESYSTEM; }
IfcCoordinateReferenceSystem::~IfcCoordinateReferenceSystem() {}
shared_ptr<IfcPPObject> IfcCoordinateReferenceSystem::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCoordinateReferenceSystem> copy_self( new IfcCoordinateReferenceSystem() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_GeodeticDatum ) { copy_self->m_GeodeticDatum = dynamic_pointer_cast<IfcIdentifier>( m_GeodeticDatum->getDeepCopy(options) ); }
	if( m_VerticalDatum ) { copy_self->m_VerticalDatum = dynamic_pointer_cast<IfcIdentifier>( m_VerticalDatum->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCoordinateReferenceSystem::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCOORDINATEREFERENCESYSTEM" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_GeodeticDatum ) { m_GeodeticDatum->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_VerticalDatum ) { m_VerticalDatum->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcCoordinateReferenceSystem::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCoordinateReferenceSystem::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcCoordinateReferenceSystem, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	m_GeodeticDatum = IfcIdentifier::createObjectFromSTEP( args[2] );
	m_VerticalDatum = IfcIdentifier::createObjectFromSTEP( args[3] );
}
void IfcCoordinateReferenceSystem::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "GeodeticDatum", m_GeodeticDatum ) );
	vec_attributes.push_back( std::make_pair( "VerticalDatum", m_VerticalDatum ) );
}
void IfcCoordinateReferenceSystem::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_HasCoordinateOperation_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasCoordinateOperation_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasCoordinateOperation_inverse.size(); ++i )
		{
			if( !m_HasCoordinateOperation_inverse[i].expired() )
			{
				HasCoordinateOperation_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcCoordinateOperation>( m_HasCoordinateOperation_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasCoordinateOperation_inverse", HasCoordinateOperation_inverse_vec_obj ) );
	}
}
void IfcCoordinateReferenceSystem::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcCoordinateReferenceSystem::unlinkFromInverseCounterparts()
{
}

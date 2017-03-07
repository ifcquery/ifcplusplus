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
#include "include/IfcLabel.h"
#include "include/IfcRepresentation.h"
#include "include/IfcRepresentationContext.h"

// ENTITY IfcRepresentationContext 
IfcRepresentationContext::IfcRepresentationContext() { m_entity_enum = IFCREPRESENTATIONCONTEXT; }
IfcRepresentationContext::IfcRepresentationContext( int id ) { m_id = id; m_entity_enum = IFCREPRESENTATIONCONTEXT; }
IfcRepresentationContext::~IfcRepresentationContext() {}
shared_ptr<IfcPPObject> IfcRepresentationContext::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRepresentationContext> copy_self( new IfcRepresentationContext() );
	if( m_ContextIdentifier ) { copy_self->m_ContextIdentifier = dynamic_pointer_cast<IfcLabel>( m_ContextIdentifier->getDeepCopy(options) ); }
	if( m_ContextType ) { copy_self->m_ContextType = dynamic_pointer_cast<IfcLabel>( m_ContextType->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRepresentationContext::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCREPRESENTATIONCONTEXT" << "(";
	if( m_ContextIdentifier ) { m_ContextIdentifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ContextType ) { m_ContextType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcRepresentationContext::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRepresentationContext::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRepresentationContext, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_ContextIdentifier = IfcLabel::createObjectFromSTEP( args[0] );
	m_ContextType = IfcLabel::createObjectFromSTEP( args[1] );
}
void IfcRepresentationContext::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "ContextIdentifier", m_ContextIdentifier ) );
	vec_attributes.push_back( std::make_pair( "ContextType", m_ContextType ) );
}
void IfcRepresentationContext::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_RepresentationsInContext_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> RepresentationsInContext_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_RepresentationsInContext_inverse.size(); ++i )
		{
			if( !m_RepresentationsInContext_inverse[i].expired() )
			{
				RepresentationsInContext_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRepresentation>( m_RepresentationsInContext_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "RepresentationsInContext_inverse", RepresentationsInContext_inverse_vec_obj ) );
	}
}
void IfcRepresentationContext::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcRepresentationContext::unlinkFromInverseCounterparts()
{
}

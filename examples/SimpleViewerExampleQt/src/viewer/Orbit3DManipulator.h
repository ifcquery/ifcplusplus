/* -*-c++-*- IfcQuery www.ifcquery.com
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

#pragma once

#include <QObject>
#include <osg/ShapeDrawable>
#include <osg/AutoTransform>
#include <osgGA/StandardManipulator>
#include <osgViewer/Viewer>
#include <osgText/Text>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/geometry/IncludeCarveHeaders.h>
#include <ifcpp/geometry/GeomUtils.h>

class IfcPlusPlusSystem;
class IntersectionHandler;

class Orbit3DManipulator : public QObject, public osgGA::StandardManipulator
{
	Q_OBJECT
public:
	Orbit3DManipulator(IfcPlusPlusSystem* s, IntersectionHandler* ih, int flags = UPDATE_MODEL_SIZE | COMPUTE_HOME_USING_BBOX | PROCESS_MOUSE_WHEEL );
    Orbit3DManipulator( const Orbit3DManipulator& om, const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY );
	~Orbit3DManipulator();

	// inherited from osg::Object
	virtual osg::Object* cloneType() const { return new Orbit3DManipulator( m_system, m_intersection_handler ); }
    virtual osg::Object* clone(const osg::CopyOp& copyop) const { return new Orbit3DManipulator( *this, copyop ); }
    virtual bool isSameKindAs(const osg::Object* obj) const { return dynamic_cast<const Orbit3DManipulator *>(obj)!=nullptr; }
	virtual const char* libraryName() const { return ""; }
    virtual const char* className() const { return "Orbit3DManipulator"; }

	// inherited from osgGA::CameraManipulator
    virtual void setByMatrix( const osg::Matrixd& matrix );
    virtual void setByInverseMatrix( const osg::Matrixd& matrix );
    virtual osg::Matrixd getMatrix() const;
    virtual osg::Matrixd getInverseMatrix() const;
    virtual osgUtil::SceneView::FusionDistanceMode getFusionDistanceMode() const;
    virtual float getFusionDistanceValue() const;

	// inherited from osgGA::StandardManipulator
	virtual void setTransformation( const osg::Vec3d& eye, const osg::Quat& rotation );
    virtual void setTransformation( const osg::Vec3d& eye, const osg::Vec3d& lookat, const osg::Vec3d& up );
    virtual void getTransformation( osg::Vec3d& eye, osg::Quat& rotation ) const;
    virtual void getTransformation( osg::Vec3d& eye, osg::Vec3d& lookat, osg::Vec3d& up ) const;
		
	virtual bool handle(				const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool handleFrame(			const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool handleMouseMove(		const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool handleMousePush(		const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool handleMouseRelease(	const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool handleMouseWheel(		const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool handleMouseDrag(		const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
    virtual bool performMouseDeltaMovement( const float dx, const float dy );
    virtual void applyAnimationStep( const double currentProgress, const double prevProgress );
	virtual void allocAnimationData() { m_animation_data = new OrbitAnimationData(); }
	virtual void setAnimationTime( const double t );
	virtual bool performAnimationMovement( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool isAnimating() const;
	
	class OrbitAnimationData : public AnimationData
	{
    public:
		OrbitAnimationData();
        osg::Vec3d m_start_eye;
		osg::Vec3d m_start_lookat;
		osg::Vec3d m_start_up;
		osg::Vec3d m_target_eye;
		osg::Vec3d m_target_lookat;
		osg::Vec3d m_target_up;
		void start( const double startTime );
    };

	// own methods
	void initManipulator();
    void setWheelZoomFactor( double wheelZoomFactor );
    double getWheelZoomFactor() const { return m_wheel_zoom_factor; };
	bool performMovement( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
    double getMinimumDistance( bool *relativeToModelSize = nullptr ) const;
	void setMinimumDistance( const double& minimumDistance, bool relativeToModelSize = false );
	void panCamera( const float dx, const float dy, const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );//const float dx, const float dy, const float dz = 0.f );
    void zoomCamera( const float dy );
	void rotateCamera( const float dx, const float dy );
	void computeRayPointer( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	void zoomToHome( double animation_time = -1 );
	void zoomToBoundingSphere( const osg::BoundingSphere& bs, double animation_time = -1 );
	OrbitAnimationData* getOrbitAnimationData() { return m_animation_data.get(); }
	void setSelectionDisabled( bool disabled );
	
	const osg::Vec3d& getLookat() { return m_lookat; }
	const osg::Vec3d& getEye() { return m_eye; }
	const osg::Vec3d& getUp() { return m_up; }
	double getFovy() { return m_fovy; };
	const OrbitAnimationData* getAnimationData() { return m_animation_data; }
	void setAnimationData( OrbitAnimationData* );
	void setEyeLookat( const osg::Vec3d& eye, const osg::Vec3d& lookat );
	void setRotateCenter( const osg::Vec3d& center );
        
protected:
	// point snap, dimensioning
	osg::ref_ptr<osg::Switch>			m_intersect_geom_switch;
	osg::ref_ptr<osg::MatrixTransform>	m_intersect_geom_transform;
	osg::Vec3d							m_pointer_intersection;
	osg::Vec3d							m_eye;
	osg::Vec3d							m_lookat;
	osg::Vec3d							m_up;
	osg::Vec3d							m_rotate_center;
	osg::Vec2d							m_pan_start;
	osg::Vec2d							m_pan_end;
	GeomUtils::Ray						m_pan_ray;
	GeomUtils::Plane					m_pan_plane;
	osg::Vec3f							m_snapped_point;
	bool								m_has_snapped_point;
	bool								m_selection_disabled;
	double								m_fovy;
    double								m_wheel_zoom_factor;
    double								m_minimum_distance;
    static int							m_minimum_distance_flag_index;
	osg::ref_ptr<OrbitAnimationData> 	m_animation_data;
	IfcPlusPlusSystem*					m_system;
	IntersectionHandler*				m_intersection_handler;
	osg::ref_ptr<osg::Camera>			m_hud_cam;
	double								m_snap_radius;
	osg::ref_ptr<osg::AutoTransform>	m_snap_wire_cone;

signals:
	void signalEyeLookatChanged();

public slots:
	void slotModelLoadingDone();
};

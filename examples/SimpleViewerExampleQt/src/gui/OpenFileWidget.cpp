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

#pragma warning( disable: 4996 )
#include <random>
#include <sstream>
#include <QKeyEvent>
#include <QFileDialog>
#include <QFile>
#include <QPainter>
#include <QSettings>
#include <QTextStream>
#include <osgUtil/Optimizer>

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/model/BuildingGuid.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/reader/ReaderSTEP.h>
#include <ifcpp/writer/WriterSTEP.h>
#include <ifcpp/writer/WriterUtil.h>

#include "IncludeGeometryHeaders.h"
#include "IfcPlusPlusSystem.h"
#include "viewer/ViewerWidget.h"
#include "viewer/ViewController.h"
#include "viewer/Orbit3DManipulator.h"
#include "cmd/CommandManager.h"
#include "OpenFileWidget.h"

OpenFileWidget::OpenFileWidget( IfcPlusPlusSystem* sys, QWidget* parent ) 
	: m_system(sys), QWidget( parent )
{
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	QStringList keys = settings.allKeys();

	if( keys.contains( "recentFiles" ) )
	{
		m_recent_files = settings.value("recentFiles").toStringList();
	}
	
	m_combo_recent_files = new QComboBox();
	m_combo_recent_files->setMaxVisibleItems( 40 );
	connect( m_combo_recent_files, SIGNAL(currentIndexChanged(int)),	this,	SLOT( slotRecentFilesIndexChanged(int) ) );

	m_btn_load = new QPushButton( "Load" );
	m_btn_load->setStyleSheet( "min-width: 30px;" );
	connect( m_btn_load, SIGNAL(clicked()),	this,	SLOT( slotLoadRecentIfcFileClicked() ) );
	
	updateRecentFilesCombo();

	QPushButton* btn_add_file = new QPushButton( "Choose file" );
	connect( btn_add_file, SIGNAL( clicked() ), this, SLOT( slotAddOtherIfcFileClicked() ) );
	
	// write
	m_le_path_write = new QLineEdit( "IfcPlusPlus-out.ifc" );
	QPushButton* btn_set_out_path = new QPushButton( "..." );
	connect( btn_set_out_path, SIGNAL( clicked() ), this, SLOT( slotSetWritePathClicked() ) );
	

	QString path_out;
	if( settings.contains( "pathIfcFileOut" ) )
	{
		path_out = settings.value("pathIfcFileOut").toString();
	}
	m_le_path_write->setText( path_out );
	QPushButton* btn_write_file = new QPushButton( "Write ifc file" );
	connect( btn_write_file, SIGNAL( clicked() ), this, SLOT( slotWriteFileClicked() ) );

	m_txt_out = new QTextEdit();

#ifdef _DEBUG
	std::string str = "test";
	std::string str1 = encodeStepString(str);

	std::stringstream uuid_strs;
	uuid_strs << createGUID32().c_str() << std::endl;
	uuid_strs << createGUID32().c_str() << std::endl;
	uuid_strs << createBase64Uuid().data() << std::endl;

	m_txt_out->setText( uuid_strs.str().c_str() );
#endif
	
	// layout
	QHBoxLayout* combo_hbox = new QHBoxLayout();
	combo_hbox->addWidget( m_combo_recent_files, 1 );
	combo_hbox->addWidget( btn_add_file );
	combo_hbox->addWidget( m_btn_load, 0 );

	m_io_widget = new QWidget(this);
	QVBoxLayout* io_vbox = new QVBoxLayout(m_io_widget);
	io_vbox->setContentsMargins( 0, 0, 0, 0 );
	io_vbox->addWidget( new QLabel( "Read IFC file" ), 0 );
	io_vbox->addLayout( combo_hbox, 1 );
	io_vbox->addSpacing( 10 );
	io_vbox->addStretch( 1 );

	m_io_splitter = new QSplitter( Qt::Horizontal );
	m_io_splitter->addWidget( m_io_widget );
	m_io_splitter->addWidget( m_txt_out );

	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->addWidget( m_io_splitter );
	setLayout( hbox );

	if( keys.contains( "IOsplitterSizes" ) )
	{
		m_io_splitter->restoreState(settings.value("IOsplitterSizes").toByteArray());
	}
}

OpenFileWidget::~OpenFileWidget(){}

void OpenFileWidget::closeEvent( QCloseEvent* )
{
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("IOsplitterSizes", m_io_splitter->saveState());
}

void OpenFileWidget::paintEvent(QPaintEvent* event)
{
	QColor backgroundColor = palette().light().color();
	backgroundColor.setAlpha(100);
	QPainter customPainter(this);
	customPainter.fillRect(rect(), backgroundColor);
	QWidget::paintEvent(event);
}

void OpenFileWidget::messageTarget( void* ptr, shared_ptr<StatusCallback::Message> m )
{
	OpenFileWidget* myself = (OpenFileWidget*)ptr;
	if( myself )
	{
		std::lock_guard<std::mutex> lock(myself->m_mutex_messages);
		std::string reporting_function_str( m->m_reporting_function );
		std::wstringstream strs_report;
		if( reporting_function_str.size() > 0 )
		{
			strs_report << m->m_reporting_function << ", ";
		}
		strs_report << m->m_message_text.c_str();

		if( m->m_entity )
		{
			BuildingEntity* ent = dynamic_cast<BuildingEntity*>(m->m_entity);
			if( ent )
			{
				strs_report << ", IFC entity: #" << ent->m_tag << "=" << EntityFactory::getStringForClassID(m->m_entity->classID());
			}
		}
		std::wstring message_str = strs_report.str().c_str();

		if( m->m_message_type == StatusCallback::MESSAGE_TYPE_GENERAL_MESSAGE )
		{
			QString qt_str = QString::fromStdWString( message_str );
			myself->txtOut( qt_str );
		}
		else if( m->m_message_type == StatusCallback::MESSAGE_TYPE_WARNING )
		{
			QString qt_str = QString::fromStdWString( message_str );
			myself->txtOutWarning( qt_str );
		}
		else if( m->m_message_type == StatusCallback::MESSAGE_TYPE_ERROR )
		{
			QString qt_str = QString::fromStdWString( message_str );
			myself->txtOutError( qt_str );
		}
		else if( m->m_message_type == StatusCallback::MESSAGE_TYPE_PROGRESS_VALUE )
		{
			myself->progressValue( m->m_progress_value, m->m_progress_type );
		}
		else if( m->m_message_type == StatusCallback::MESSAGE_TYPE_CLEAR_MESSAGES )
		{
			myself->clearTxtOut();
		}
	}
}

void OpenFileWidget::keyPressEvent( QKeyEvent* e )
{
	if( e->text() == "c" )
	{
		m_recent_files.clear();
		QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
		settings.setValue("recentFiles",m_recent_files );
		updateRecentFilesCombo();
	}
}

void OpenFileWidget::updateRecentFilesCombo()
{
	m_combo_recent_files->blockSignals( true );
	m_combo_recent_files->clear();
	for( size_t ii = 0; ii < m_recent_files.size(); ++ii )
	{
		const QString& path_recent_file = m_recent_files.at( ii );
		QFileInfo fi( path_recent_file );
		QString file_name = fi.filePath();
		if( file_name.size() > 100 )
		{
			file_name = "..." + file_name.right(97);
		}
		m_combo_recent_files->addItem( file_name );
	}
	m_combo_recent_files->blockSignals( false );
}

void OpenFileWidget::slotRecentFilesIndexChanged(int idx)
{
	progressValue( 0, "" );
	m_btn_load->setFocus();
}

// Function to create a shader from source
osg::ref_ptr<osg::Shader> createShader(osg::Shader::Type type, const std::string& source) {
	osg::ref_ptr<osg::Shader> shader = new osg::Shader(type);
	shader->setShaderSource(source);
	return shader;
}

// Function to create a program and attach shaders
osg::ref_ptr<osg::Program> createShaderProgram() {
	osg::ref_ptr<osg::Program> program = new osg::Program;

	// Example vertex shader source
	std::string vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 inPosition;
        out vec3 fragPosition;

        uniform mat4 modelMatrix;
        uniform mat4 viewMatrix;
        uniform mat4 projectionMatrix;

        void main() {
            fragPosition = vec3(modelMatrix * vec4(inPosition, 1.0));
            gl_Position = projectionMatrix * viewMatrix * vec4(fragPosition, 1.0);
        }
    )";

	// Example geometry shader source
	std::string geometryShaderSource = R"(
        #version 330 core
        layout(triangles) in;
        layout(triangle_strip, max_vertices = 3) out;

        in vec3 fragPosition[];
        out vec3 normal;

        void main() {
            vec3 edge1 = fragPosition[1] - fragPosition[0];
            vec3 edge2 = fragPosition[2] - fragPosition[0];
            normal = normalize(cross(edge1, edge2));

            for (int i = 0; i < 3; ++i) {
                gl_Position = gl_in[i].gl_Position;
                EmitVertex();
            }
            EndPrimitive();
        }
    )";

	// Example fragment shader source
	std::string fragmentShaderSource = R"(
        #version 330 core
        in vec3 normal;
        out vec4 fragColor;

        uniform vec3 lightDirection;

        void main() {
            float brightness = max(dot(normalize(normal), normalize(lightDirection)), 0.0);
            fragColor = vec4(vec3(brightness), 1.0);
        }
    )";

	// Attach shaders
	program->addShader(createShader(osg::Shader::VERTEX, vertexShaderSource));
	program->addShader(createShader(osg::Shader::GEOMETRY, geometryShaderSource));
	program->addShader(createShader(osg::Shader::FRAGMENT, fragmentShaderSource));

	return program;
}


void OpenFileWidget::loadIfcFile( QString& path_in )
{
	// redirect message callbacks
	m_system->m_ifc_model = shared_ptr<BuildingModel>(new BuildingModel());
	m_system->m_geometry_converter = make_shared<GeometryConverter>(m_system->m_ifc_model, m_system->m_geom_settings);
	m_system->m_geometry_converter->setMessageCallBack(std::bind(&OpenFileWidget::messageTarget, this, std::placeholders::_1));

	m_stopSignals = false;
	txtOut( QString( "loading file: " ) + path_in );

	clock_t millisecs = clock();
	m_system->notifyModelCleared();
	m_txt_out->clear();
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));

	if( !QFile::exists(path_in) )
	{
		txtOutError( QString("file ") + path_in + QString(" does not exist\n") );

		// remove all non-existing files from recent files combo
		for( int i=0; i<m_recent_files.size(); )
		{
			const QString& recent_file = m_recent_files[i];
			if( !QFile::exists(recent_file) )
			{
				m_recent_files.takeAt( i );
			}
			else
			{
				++i;
			}
		}
		settings.setValue("recentFiles",m_recent_files );
		updateRecentFilesCombo();
		m_stopSignals = true;
		return;
	}
	else
	{
		// move to top of recent files list
		int i = m_recent_files.indexOf( path_in );
		if( i > 0 )
		{
			QString current_path = m_recent_files.takeAt( i );
			m_recent_files.insert( 0, current_path );
			m_recent_files.removeDuplicates();
			settings.setValue("recentFiles",m_recent_files );
			updateRecentFilesCombo();
		}
		else
		{
			m_recent_files.insert( 0, path_in );
			m_recent_files.removeDuplicates();
			settings.setValue("recentFiles",m_recent_files );
			updateRecentFilesCombo();
		}
	}

	try
	{
		std::string path_str = path_in.toStdString();
		if (path_str.length() == 0)
		{
			m_stopSignals = true;
			return;
		}
		setEnabled(false);

		// first remove previously loaded geometry from scenegraph
		osg::ref_ptr<osg::Group> modelNode = m_system->getViewController()->getModelNode();
		SceneGraphUtils::clearAllChildNodes(modelNode);
		m_system->clearSelection();

		// reset the IFC model
		shared_ptr<GeometryConverter> geometry_converter = m_system->m_geometry_converter;
		geometry_converter->clearMessagesCallback();
		geometry_converter->resetModel();
		geometry_converter->getGeomSettings()->setNumVerticesPerCircle(16);
		geometry_converter->getGeomSettings()->setMinNumVerticesPerArc(4);
		std::stringstream err;

		// load file to IFC model
		shared_ptr<ReaderSTEP> step_reader(new ReaderSTEP());
		step_reader->setMessageCallBack(std::bind(&OpenFileWidget::messageTarget, this, std::placeholders::_1));
		step_reader->loadModelFromFile(path_str, geometry_converter->getBuildingModel());

		// convert IFC geometric representations into Carve geometry
		geometry_converter->setCsgEps(1.5e-08);
		geometry_converter->convertGeometry();

		// convert Carve geometry to OSG
		shared_ptr<ConverterOSG> converter_osg(new ConverterOSG(geometry_converter->getGeomSettings()));
		converter_osg->setMessageTarget(geometry_converter.get());
		converter_osg->convertToOSG(geometry_converter->getShapeInputData(), modelNode);

		if (modelNode)
		{

			// Create the shader program
			osg::ref_ptr<osg::Program> program = createShaderProgram();

			// Create a StateSet for the geode
			osg::ref_ptr<osg::StateSet> stateSet = modelNode->getOrCreateStateSet();

			// Attach the shader program to the StateSet
			stateSet->setAttributeAndModes(program, osg::StateAttribute::ON);

			// Set a uniform for light direction in the fragment shader
			osg::Vec3 lightDirection(0.0f, 0.0f, 1.0f);  // Example light direction
			stateSet->addUniform(new osg::Uniform("lightDirection", lightDirection));

			bool optimize = true;
			if (optimize)
			{
				osgUtil::Optimizer opt;
				opt.optimize(modelNode);
			}

			// if model bounding sphere is far from origin, move to origin
			const osg::BoundingSphere& bsphere = modelNode->getBound();
			if (bsphere.center().length() > 10000)
			{
				if (bsphere.center().length() / bsphere.radius() > 100)
				{
					osg::MatrixTransform* mt = new osg::MatrixTransform();
					mt->setMatrix(osg::Matrix::translate(-bsphere.center() * 0.98));

					int num_children = modelNode->getNumChildren();
					for (int i = 0; i < num_children; ++i)
					{
						osg::Node* node = modelNode->getChild(i);
						if (!node)
						{
							continue;
						}
						mt->addChild(node);
					}
					SceneGraphUtils::removeChildren(modelNode);
					modelNode->addChild(mt);
				}
			}
		}

		geometry_converter->clearIfcRepresentationsInModel( true, true, false );
		geometry_converter->clearInputCache();
	}
	catch( BuildingException& e )
	{
		txtOutError( e.what() );
	}
	catch(std::exception& e)
	{
		txtOutError( e.what() );
	}

	int time_diff = (clock() - millisecs)/(double)CLOCKS_PER_SEC;
	int num_entities = m_system->getIfcModel()->getMapIfcEntities().size();
	txtOut( tr("File loaded: ") + QString::number(num_entities) + " entities in " + QString::number( round(time_diff*10)*0.1 ) + " sec."  );

	m_system->notifyModelLoadingDone();
	progressValue( 1.0, "" );
	setEnabled(true);
}

void OpenFileWidget::txtOut( QString txt )
{
	QString basecol = palette().text().color().name();
	m_txt_out->append( "<div style=\"color:" + basecol + ";\">" + txt.replace( "\n", "<br/>" ) + "</div><br/>" );
}

void OpenFileWidget::txtOutWarning( QString txt )
{
	m_txt_out->append( "<div style=\"color:#dca103;\">Warning: " + txt.replace( "\n", "<br/>" ) + "</div><br/>" );
}

void OpenFileWidget::txtOutError( QString txt )
{
	m_txt_out->append( "<div style=\"color:red;\">Error: " + txt.replace( "\n", "<br/>" ) + "</div><br/>" );
}

void OpenFileWidget::progressValue(double progress_value_in, const std::string& progress_type)
{
	if (m_stopSignals)
	{
		return;
	}
	emit(signalProgressValue(progress_value_in, progress_type));
}

void OpenFileWidget::clearTxtOut()
{
	m_txt_out->clear();
}

void OpenFileWidget::slotLoadRecentIfcFileClicked()
{
	QPushButton* btn_load = (QPushButton*)sender();
	if( !btn_load )
	{
		return;
	}
	
	int row = m_combo_recent_files->currentIndex();
	if( row < 0 || row >= m_combo_recent_files->count() )
	{
		return;
	}
	m_stopSignals = false;
	m_io_widget->setDisabled( true );
	if( row < m_recent_files.size() )
	{
		QString file_name = m_recent_files.at( row );
		loadIfcFile( file_name );
	}

	QCoreApplication::removePostedEvents(this);
	progressValue(1.0, "");
	m_stopSignals = true;
	emit(signalClearSignalQueue());

	m_io_widget->setDisabled(false);
}

void OpenFileWidget::slotClearRecentIfcFiles()
{
	m_recent_files.clear();
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("recentFiles",m_recent_files );
	updateRecentFilesCombo();
}

void OpenFileWidget::slotAddOtherIfcFileClicked()
{
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	QStringList keys = settings.allKeys();
	QString default_dir = "";
	if( keys.contains( "recentFiles" ) )
	{
		default_dir = settings.value( "defaultDir" ).toString();
	}
	QString selectedFilter = "IFC files (*.ifc *.ifczip)";
	QString selected_file = QFileDialog::getOpenFileName(this, "Choose IFC file", default_dir, selectedFilter);
	
	if( !selected_file.isEmpty() )
	{
		QDir current_dir;
		settings.setValue( "defaultDir", current_dir.absoluteFilePath(selected_file) );
		loadIfcFile( selected_file );
	}
}

void OpenFileWidget::slotSetWritePathClicked()
{
	QString selectedFilter;
	QString fileName = QFileDialog::getSaveFileName(this, "IfcPlusPlus - choose path to write ifc file", m_le_path_write->text(), "All Files (*);;IFC file (*.ifc)", &selectedFilter, QFileDialog::DontUseNativeDialog);
	if( !fileName.isEmpty() )
	{
		m_le_path_write->setText(fileName);
	}
}

void OpenFileWidget::slotWriteFileClicked()
{
	QString path = m_le_path_write->text();
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("pathIfcFileOut", path );

	txtOut( "writing file: " + path );
	int millisecs = clock();

	std::string path_std = path.toStdString();
	try
	{
		if (path_std.length() == 0)
		{
			return;
		}

		shared_ptr<GeometryConverter> geom_converter = m_system->getGeometryConverter();
		geom_converter->setGeomSettings( m_system->getGeometrySettings() );
		shared_ptr<BuildingModel>& model = geom_converter->getBuildingModel();
		std::string applicationName = "IfcPlusPlus";
		model->initFileHeader(path_std, applicationName);
		std::stringstream stream;
		shared_ptr<WriterSTEP> writer_step(new WriterSTEP());
		writer_step->writeModelToStream(stream, model);

		QFile file_out(QString::fromStdString(path_std.c_str()));
		if (!file_out.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			return;
		}

		QTextStream file_out_stream(&file_out);
		file_out_stream << stream.str().c_str();
		file_out.close();

	}
	catch( std::exception& e )
	{
		txtOutWarning( "couldn't write file " + path + e.what() );
	}
	
	int time_diff = clock() - millisecs;
	txtOut( "file written (" + QString::number( time_diff*0.001 ) + " sec)" );
	progressValue( 1.0, "" );
}

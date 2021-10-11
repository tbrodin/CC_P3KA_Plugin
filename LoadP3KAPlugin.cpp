#include <QtGui>

#include "LoadP3KAPlugin.h"

// Default constructor:
//	- pass the Qt resource path to the info.json file (from <yourPluginName>.qrc file) 
//  - constructor should mainly be used to initialize actions and other members
LoadP3KAPlugin::LoadP3KAPlugin( QObject *parent )
	: QObject( parent )
	, ccStdPluginInterface( ":/CC/plugin/LoadP3KAPlugin/info.json" )
	, m_action( nullptr )
{
}

// This method should enable or disable your plugin actions
// depending on the currently selected entities ('selectedEntities').
void LoadP3KAPlugin::onNewSelection( const ccHObject::Container &selectedEntities )
{
	if ( m_action == nullptr )
	{
		return;
	}
	
}

// This method returns all the 'actions' your plugin can perform.
// getActions() will be called only once, when plugin is loaded.
QList<QAction *> LoadP3KAPlugin::getActions()
{
	// default action (if it has not been already created, this is the moment to do it)
	if ( !m_action )
	{
		// Here we use the default plugin name, description, and icon,
		// but each action should have its own.
		m_action = new QAction( getName(), this );
		m_action->setToolTip( getDescription() );
		m_action->setIcon( getIcon() );
		
		// Connect appropriate signal
		connect(m_action, &QAction::triggered, this, &LoadP3KAPlugin::performActionLoadDialog);

	}

	return { m_action };
}


void LoadP3KAPlugin::readP3KAResult(QString path)
{
	QFile myFile(path);
	bool opened = myFile.open(QIODevice::ReadOnly);
	if (!opened)
	{
		ccLog::Error(QStringLiteral("Could not load plugin resources: %1").arg(path));
		return;
	}
	QByteArray json = myFile.readAll();
	QJsonParseError	jsonError;
	auto json_doc = QJsonDocument::fromJson(json, &jsonError);

	if (jsonError.error != QJsonParseError::NoError)
	{
		ccLog::Error(QStringLiteral("Could not parse plugin info: %1").arg(jsonError.errorString()));
		return;
	}

	if (!json_doc.isObject()) {
		qDebug() << "JSON is not an object.";
		exit(3);
	}

	QJsonObject rootObj = json_doc.object();
	QString name,type,data;
	QString camera_type = "Camera", point_type = "Point", plane_type = "Plane", point_uncert_type = "Point Uncertainty";

	QStringList books = rootObj.keys();
	for (auto book : books)
	{
		QJsonObject obj = rootObj.value(book).toObject();
		QString keys;
		name = obj.value("name").toString();
		type = obj.value("type").toString();
		data = obj.value("data").toString();
		QStringList data_str = data.split(QRegExp("[\r\n]"));

		if (type == camera_type)
		{	
			ccGLMatrix trans = ccGLMatrix();
			ccCameraSensor* camera = new ccCameraSensor();
			camera->setName(name);
			//camera->apply // TODO: convert data to correct format and then transform camera
			m_app->addToDB(camera);
		} 
		else if (type == point_type)
		{
			CCVector3 coord = CCVector3(data_str[0].toFloat(), data_str[1].toFloat(), data_str[2].toFloat());
			ccSphere* point = new ccSphere(name);
			point->setRadius(PointCoordinateType(0.01));
			point->setDrawingPrecision(std::uint8_t(24));
			point->setColor(ccColor::darkBlue);
			point->showColors(true);
			point->translateGL(coord);
			m_app->addToDB(point,true);
		}
		else if (type == plane_type)
		{
			continue;
		}
		else if (type == point_uncert_type)
		{
			continue;
		}
		else {
			continue;
		}

	}

	/*
	m_app->dispToConsole("[P3KAPlugin] point: " + QString::number(x) + QString::number(y) + QString::number(z), ccMainAppInterface::STD_CONSOLE_MESSAGE);

	ccSphere* point = new ccSphere("mexico");
	point->setRadius(PointCoordinateType(0.01));
	point->setDrawingPrecision(std::uint8_t(24));
	point->setColor(ccColor::darkBlue);
	point->showColors(true);
	point->translateGL(CCVector3(x, y, z));
	m_app->addToDB(point); */
}

void LoadP3KAPlugin::performActionLoadDialog()
{
	if (m_app == nullptr)
	{
		// The application interface should have already been initialized when the plugin is loaded
		Q_ASSERT(false);

		return;
	}

	//QString path = QFileDialog::getOpenFileName(nullptr, "Load P3KA result", " ", "Files (*.json)");
	QString path = QString::fromStdString("D:/cloudcompare/p3ka_res.json");
	if (path.isEmpty() || path.isNull()) {
		m_app->dispToConsole("[P3KAPlugin] Canceled loading P3KA result", ccMainAppInterface::STD_CONSOLE_MESSAGE);
		return;
	}

	m_app->dispToConsole("[P3KAPlugin] Loaded: " + path, ccMainAppInterface::STD_CONSOLE_MESSAGE);
	readP3KAResult(path);

}


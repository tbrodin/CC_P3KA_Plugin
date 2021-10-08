#include <QtGui>

#include "LoadP3KAPlugin.h"

#include "qdialog.h"
#include "qfiledialog.h"
#include "qlayout.h"
#include "qformlayout.h" 
#include "qpushbutton.h"
#include "ccSphere.h"
#include "qlineedit.h"
#include "qlabel.h"
#include "qdialogbuttonbox.h"
#include "qstring.h"
#include "qjsondocument.h"

#include <sstream>
#include <string>
#include <fstream>

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
	
	// If you need to check for a specific type of object, you can use the methods
	// in ccHObjectCaster.h or loop and check the objects' classIDs like this:
	//
	//	for ( ccHObject *object : selectedEntities )
	//	{
	//		if ( object->getClassID() == CC_TYPES::VIEWPORT_2D_OBJECT )
	//		{
	//			// ... do something with the viewports
	//		}
	//	}
	
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

void LoadP3KAPlugin::performActionLoadDialog()
{
	if (m_app == nullptr)
	{
		// The application interface should have already been initialized when the plugin is loaded
		Q_ASSERT(false);

		return;
	}

	QString path = QFileDialog::getOpenFileName(nullptr, "Load P3KA result", " ", "Files (*.txt)");
	m_app->dispToConsole("[P3KAPlugin] Loaded: " + path, ccMainAppInterface::STD_CONSOLE_MESSAGE);

	std::ifstream source;
	source.open(path.toStdString(), std::ios_base::in);

	float x, y, z;
	for (std::string line; std::getline(source, line);)  //read stream line by line
	{
		std::istringstream in(line);      //make a stream for the line itself

		std::string type;
		in >> type;                  //and read the first whitespace-separated token

		if (type == "point")       //and check its value
		{
			in >> x >> y >> z;       //now read the whitespace-separated floats
		}
		else if (type == "camera")
		{
			in >> x >> y >> z;
		}
	}


	m_app->dispToConsole("[P3KAPlugin] point: " + QString::number(x) + QString::number(y) + QString::number(z), ccMainAppInterface::STD_CONSOLE_MESSAGE);

	ccSphere* point = new ccSphere("mexico");
	point->setRadius(PointCoordinateType(0.01));
	point->setDrawingPrecision(std::uint8_t(24));
	point->setColor(ccColor::darkBlue);
	point->showColors(true);
	point->translateGL(CCVector3(x, y, z));
	m_app->addToDB(point);



	/*
	QString path = QFileDialog::getOpenFileName(nullptr, "Load P3KA result"," ","Files (*.json)");

	appInterface->dispToConsole("[P3KAPlugin] Loaded: " + path, ccMainAppInterface::STD_CONSOLE_MESSAGE);

	QString val;
	QFile p3ka_file;
	p3ka_file.setFileName(path);
	p3ka_file.open(QIODevice::ReadOnly | QIODevice::Text);
	val = p3ka_file.readAll();
	p3ka_file.close();
	qWarning() << val;
	QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

	std::list<std::string> keys = d.object();

	QJsonObject sett2 = d.object();
	QJsonValue value = sett2.value(QString("appName"));
	qWarning() << value;
	QJsonObject item = value.toObject();
	qWarning() << tr("QJsonObject of description: ") << item;

	/* in case of string value get value and convert into string
	qWarning() << tr("QJsonObject[appName] of description: ") << item["description"];
	QJsonValue subobj = item["description"];
	qWarning() << subobj.toString();

	/* in case of array get array and convert into string
	qWarning() << tr("QJsonObject[appName] of value: ") << item["imp"];
	QJsonArray test = item["imp"].toArray();
	qWarning() << test[1].toString();

	// Show the dialog as modal

	if (rulerDlg.exec() == QDialog::Accepted) {

		// If the user didn't dismiss the dialog, do something with the fields

		rulerLength = lineEditSegmentLength->text().toInt();
		rulerWidth = lineEditSegmentWidth->text().toInt();
		rulerNoOfSegments = lineEditNrSegments->text().toInt();

		ccPlane* rulerContainer = new ccPlane("Ruler");

		int segmentOffset;

		for (int i = 0; i < rulerNoOfSegments; i++) {
			ccGenericPrimitive* segment = new ccPlane(rulerLength / rulerNoOfSegments, rulerWidth, 0, QString("ruler segment_%1").arg(i + 1));
			segmentOffset = i * rulerLength / rulerNoOfSegments;

			if (i % 2) {
				segment->setColor(ccColor::darkBlue);
			}
			else {
				segment->setColor(ccColor::white);
			}

			segment->showColors(true);
			segment->translateGL(CCVector3(segmentOffset, 0, 0));
			rulerContainer->addChild(segment);
		}

		appInterface->addToDB(rulerContainer);

	}*/
}


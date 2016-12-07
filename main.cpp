#include <QApplication>
#include <QQmlApplicationEngine>
#include "xble/xblecommunication.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    XBleCommunication *obj= new XBleCommunication();
   // obj->scan();


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("ble", obj);
     engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

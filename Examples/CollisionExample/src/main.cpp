#include "ExampleScene.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    // Enable scaling for high resolution displays
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QApplication a(argc, argv);

    
    ExampleScene* w = new ExampleScene();
    w->show();
    int ret = a.exec();
    delete w;
    qDebug() << "Objects that are not deleted: " << QSFML::Internal::LifetimeChecker::getAliveCount() << " of total: "<< QSFML::Internal::LifetimeChecker::getTotalCount();
    return ret;
}

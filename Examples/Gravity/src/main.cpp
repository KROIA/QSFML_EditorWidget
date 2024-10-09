#include "ExampleScene.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    // Qt Compatibility for high DPI displays
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QApplication a(argc, argv);
	//QSFML::Profiler::start();
    ExampleScene* w = new ExampleScene();
    w->show();
    QSFML::Scene::setProfilerOutputFileName("Gravity.prof");

    int ret = a.exec();
    delete w;
    qDebug() << "Objects that are not deleted: " << QSFML::Internal::LifetimeChecker::getAliveCount() << " of total: " << QSFML::Internal::LifetimeChecker::getTotalCount();
    return ret;
}

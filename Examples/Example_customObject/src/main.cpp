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
    ExampleScene w;
    w.show();
    return a.exec();
}

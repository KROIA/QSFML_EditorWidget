#include <iostream>

#include <QApplication>
#include <SFML/Window.hpp>
#include <easy/profiler.h>
#include "QSFML_EditorWidget.h"


using namespace std;
using namespace QSFML;
using namespace QSFML::Objects;

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    CanvasSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300, 100);
    settings.contextSettings.antialiasingLevel = 8;
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    Canvas *canvas_1 = new Canvas(nullptr, settings);

    DefaultEditor* defaultEditor = new DefaultEditor();


    canvas_1->addObject(defaultEditor);
    //m_canvas->addObject(grid);
  //  qDebug() << defaultEditor->toString().c_str();


    profiler::dumpBlocksToFile("test_profile.prof");
    return app.exec();
    return 0;
}
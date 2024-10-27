#include "ExampleScene.h"
#include <QApplication>
int quickStart(int argc, char* argv[]);

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    // Qt Compatibility for high DPI displays
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    //return quickStart(argc, argv);
    QApplication a(argc, argv);
    ExampleScene* w = new ExampleScene();
    w->show();

    int ret = a.exec();
    delete w;
    qDebug() << "Objects that are not deleted: " << QSFML::Internal::LifetimeChecker::getAliveCount() << " of total: " << QSFML::Internal::LifetimeChecker::getTotalCount();
    return ret;
}

int quickStart(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QWidget screenWidget;
	screenWidget.resize(500, 500);

    QSFML::SceneSettings settings;
    settings.timing.frameTime = 0.03; // ms -> ~60 FPS
    QSFML::Scene scene(&screenWidget, settings);	

	QSFML::Objects::DefaultEditor* editor = new QSFML::Objects::DefaultEditor("Editor", sf::Vector2f(1000, 800));
	scene.addObject(editor);

	QSFML::Objects::GameObject* object = new QSFML::Objects::GameObject("MyObject");
	object->addUpdateFunction([](QSFML::Objects::GameObject& obj)
		{
            obj.rotate(obj.getDeltaT() * 50.f);
		});
    
    object->addDrawFunction([](const QSFML::Objects::GameObject& obj, sf::RenderTarget& target, sf::RenderStates states)
        {
            sf::RectangleShape shape(sf::Vector2f(400, 400));
			shape.setPosition(-200, -200);
            shape.setFillColor(sf::Color::Red);
            target.draw(shape, states);
        });
	scene.addObject(object);

    scene.start();
    screenWidget.show();
    return a.exec();
}
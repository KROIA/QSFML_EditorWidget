#ifdef QT_ENABLED
#include <QApplication>
#endif
#include <iostream>
#include "QSFML_EditorWidget.h"
#include <iostream>
#include "tests.h"

#ifdef QT_WIDGETS_ENABLED
#include <QWidget>
#endif

// Instantiate Tests here:
// TEST_INSTANTIATE(Test_simple); // Where Test_simple is a derived class from the Test class
TEST_INSTANTIATE(TST_Pathfinder); 
//TEST_INSTANTIATE(TST_Performance); // Where TST_Performance is a derived class from the Test class
//TEST_INSTANTIATE(TST_VectorOperations);
//TEST_INSTANTIATE(TST_RenderLayer);

int main(int argc, char* argv[])
{
#ifdef QT_WIDGETS_ENABLED
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#ifdef QT_ENABLED
	QApplication app(argc, argv);
#endif


	QSFML::SceneSettings settings;

	// Background color of parts on which nothing was painted
	settings.colors.defaultBackground = sf::Color(80, 80, 80);

	// sf::ContextSettings for the render window
	settings.contextSettings.antialiasingLevel = 8;

	// QLayout margin for when the Scene has a QWidget parent
	settings.layout.margin.left = 6;
	settings.layout.margin.top = 6;
	settings.layout.margin.right = 6;
	settings.layout.margin.bottom = 6;

	settings.layout.autoAjustSize = true; // If set to true, the Scene will automatically resizes based on its parent widget.
	settings.layout.fixedSize = sf::Vector2u(1000, 700); // If autoAjustSize is false, this size will be applyed to the Scene widget

	settings.timing.frameTime = 0.01;
	settings.timing.physicsDeltaTScale = 0.1;
	settings.timing.physicsFixedDeltaT = 0.01;

	// Enable or disable the different loops
	settings.updateControlls.enableEventLoop = true;
	settings.updateControlls.enablePaintLoop = true;
	settings.updateControlls.enableUpdateLoop = true;

	// Enable or disable multithreading for the update loop
	settings.updateControlls.enableMultithreading = false;

	QWidget widget;
	QSFML::Scene* scene = new QSFML::Scene(&widget, settings);
	widget.show();
	auto defaultEditor = new QSFML::Objects::DefaultEditor();
	//defaultEditor->setColors(sf::Color(200, 200, 200), sf::Color(0, 0, 0));
	scene->addObject(defaultEditor);
	scene->start();
	return app.exec();

	Log::UI::NativeConsoleView console;
	console.show();
	QSFML::LibraryInfo::printInfo();

	std::cout << "Running "<< UnitTest::Test::getTests().size() << " tests...\n";
	UnitTest::Test::TestResults results;
	UnitTest::Test::runAllTests(results);
	UnitTest::Test::printResults(results);

#ifdef QT_WIDGETS_ENABLED
	//QWidget* widget = QSFML::LibraryInfo::createInfoWidget();
	//if (widget)
	//	widget->show();
#endif
#ifdef QT_ENABLED
	return app.exec();
#else
	return 0;
#endif
}

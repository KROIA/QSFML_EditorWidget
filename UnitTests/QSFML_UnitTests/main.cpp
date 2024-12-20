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
//   TEST_INSTANTIATE(Test_simple); // Where Test_simple is a derived class from the Test class
//TEST_INSTANTIATE(TST_Pathfinder); 
//TEST_INSTANTIATE(TST_Performance); // Where TST_Performance is a derived class from the Test class
//TEST_INSTANTIATE(TST_VectorOperations);
//TEST_INSTANTIATE(TST_RenderLayer);
//TEST_INSTANTIATE(TST_TextureManager);
//TEST_INSTANTIATE(TST_TextureAnimation);
TEST_INSTANTIATE(TST_TileMap);

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

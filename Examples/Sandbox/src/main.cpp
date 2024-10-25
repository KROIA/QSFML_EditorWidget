/*
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QTimer>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <Windows.h>

#include <iostream>

// Custom QWidget subclass that contains SFML render windows
class SFMLWidget : public QWidget {
public:
    SFMLWidget(QWidget* parent = nullptr) : QWidget(parent) {
        // Set QWidget to have no focus
        setFocusPolicy(Qt::NoFocus);

        // Create the SFML window attached to the QWidget handle
        sfmlRenderWindow.create((sf::WindowHandle)winId());

        // Initialize ImGui context
        imguiContext = ImGui::CreateContext();
        ImGui::SetCurrentContext(imguiContext);
        ImGui::SFML::Init(sfmlRenderWindow);

        // Set up a QTimer for refreshing the display at 60 FPS
        refreshTimer = new QTimer(this);
        connect(refreshTimer, &QTimer::timeout, this, &SFMLWidget::onRefresh);
        refreshTimer->start(16);  // Approximately 60 FPS (1000 ms / 60 = ~16 ms)

        setAttribute(Qt::WA_PaintOnScreen);
        setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_NoSystemBackground);
        
    }
    ~SFMLWidget()
    {
		closeEvent(nullptr);
    }

	void paintEvent(QPaintEvent*) override {
		// Do nothing here to prevent flickering
	}

    void onRefresh() {
        
        // Process SFML and ImGui events, and render content
        //sfmlRenderWindow.setActive(true);
        ImGui::SetCurrentContext(imguiContext);

        sfmlRenderWindow.setActive(true);
		sf::Vector2i mousePos = sf::Mouse::getPosition(sfmlRenderWindow);
        // Check if the mouse is over the window
		//if (mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x < sfmlRenderWindow.getSize().x && mousePos.y < sfmlRenderWindow.getSize().y) 
        {
            // Check if this SFML widget doesn't have focus, then set focus
            HWND currentFocus = GetFocus();
            if (currentFocus != (HWND)winId()) {
                SetFocus((HWND)winId());
               // std::cout << "Focus changed\n";
            }
		}

        
        sf::Event event;
        while (sfmlRenderWindow.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
        }

        // Clear the window
        sfmlRenderWindow.clear(sf::Color::Blue);
       // sfmlRenderWindow.requestFocus();
        
        if (sfmlRenderWindow.hasFocus())
            std::cout << "HasFocus\n";


        // Start ImGui frame
        ImGui::SFML::Update(sfmlRenderWindow, sf::milliseconds(16));

        // Example ImGui window
        ImGui::Begin("ImGui Window");
        ImGui::Text("Hello from ImGui!");
        ImGui::End();

        // Render ImGui
        ImGui::SFML::Render(sfmlRenderWindow);

        // Display the window
        
        sfmlRenderWindow.display();
        sfmlRenderWindow.setActive(false);
    }

    void printHandles()
    {
		HWND thisHandle = (HWND)QWidget::winId();
		HWND sfmlHandle = (HWND)sfmlRenderWindow.getSystemHandle();

		std::cout << "QWidget handle: " << thisHandle << "\t SFML handle" << sfmlHandle << "\n";
    }

    void closeEvent(QCloseEvent*) override {
		refreshTimer->stop();
		sfmlRenderWindow.close();
        ImGui::SFML::Shutdown(sfmlRenderWindow);
		ImGui::DestroyContext(imguiContext);
		imguiContext = nullptr;
    }

private:
    sf::RenderWindow sfmlRenderWindow;
    ImGuiContext* imguiContext;
    QTimer* refreshTimer;  // Timer for updating the render window
};

// Main window that contains two SFML widgets
class MainWindow : public QWidget {
public:
    MainWindow(QWidget* parent = nullptr) : QWidget(parent) 
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        setFocusPolicy(Qt::NoFocus);

        // Create two SFML widgets and add them to the layout
        sfmlWidget1 = new SFMLWidget(this);
        sfmlWidget2 = new SFMLWidget(this);

       // sfmlWidget1->show();
        layout->addWidget(sfmlWidget1);
        layout->addWidget(sfmlWidget2);
   
        setLayout(layout);


        HWND thisHandle = (HWND)QWidget::winId();
        std::cout << "MainWindow handle: " << "\t" << thisHandle << "\n";
		sfmlWidget1->printHandles();
		sfmlWidget2->printHandles();

		QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [this, thisHandle]()
            {
                HWND activeHandle = GetFocus();
                std::cout << "Active window handle: " << activeHandle;

                if(activeHandle == thisHandle)
				{
					std::cout << " (MainWindow)\n";
				}
				else
				{
					std::cout << " (Other)\n";
				}
            });
		timer->start(1000);
    }

private:
    SFMLWidget* sfmlWidget1;
    SFMLWidget* sfmlWidget2;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Create and show the main window
    MainWindow window;
    window.show();

    return app.exec();
}

*/




#include "sandbox.h"

#include <QApplication>
#include <chrono>
#include <string>
#include <vector>
#include <QDebug>


#include <QWidget>
#include <QList>

using std::string;
using QSFML::vector;

void benchmark();
void printTime(const string &msg, const std::chrono::duration<long long, std::ratio<1,1000000000>> &duration);


// Helper function to recursively disable focus for a widget and its children
void disableFocusRecursively(QWidget* widget) {
    widget->setFocusPolicy(Qt::NoFocus);  // Disable focus for this widget

    // Disable focus for all child widgets
    const QList<QWidget*> childWidgets = widget->findChildren<QWidget*>();
    for (QWidget* child : childWidgets) {
        child->setFocusPolicy(Qt::NoFocus);
    }
}
// Function to disable focus for all widgets in the application
void disableFocusForAllWidgets() {
    const QList<QWidget*> topLevelWidgets = QApplication::topLevelWidgets();

    for (QWidget* widget : topLevelWidgets) {
        // Recursively disable focus for the top-level widget and its children
        disableFocusRecursively(widget);
    }
}



int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    // Enable scaling for high resolution displays
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QApplication a(argc, argv);

    Log::UI::NativeConsoleView::createStaticInstance();
	Log::UI::NativeConsoleView::getStaticInstance()->show();

    //Log::UI::QTreeConsoleView::createStaticInstance();
    //Log::UI::QTreeConsoleView::getStaticInstance()->show();

	//Log::FilePlotter plotter("test.log");




   // EASY_PROFILER_ENABLE;
    SandBox* w = new SandBox();
    w->show();

    // Disable focus for all widgets
    //disableFocusForAllWidgets();
    //benchmark();
    int ret = a.exec();
    delete w;
    qDebug() << "Objects that are not deleted: " << QSFML::Internal::LifetimeChecker::getAliveCount() << " of total: " << QSFML::Internal::LifetimeChecker::getTotalCount();
 //   profiler::dumpBlocksToFile("test_profile.prof");
    //QSFML::Scene::startEventLoop();
    return ret;
}

void benchmark()
{

    vector<size_t> list;
    size_t count = 10000000;
    list.reserve(count);
    auto start = std::chrono::high_resolution_clock::now();
    for(size_t i=0; i<count; ++i)
    {
        list.push_back(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    printTime("InitLists",end-start);


    start = std::chrono::high_resolution_clock::now();
    for(size_t i=0; i<list.size(); ++i)
    {
        ++list[i];
    }
    end = std::chrono::high_resolution_clock::now();
    printTime("Increment 1",end-start);

    start = std::chrono::high_resolution_clock::now();
    size_t* begin = list.data();
    size_t* endPos = begin + count;
    for(size_t* it = begin; it < endPos; ++it)
    {
        (*it)++;
    }
    end = std::chrono::high_resolution_clock::now();
    printTime("Increment 2",end-start);

    start = std::chrono::high_resolution_clock::now();
    for(QSFML::vector<size_t>::iterator it = list.begin(); it != list.end(); ++it) {
        ++(*it);
    }
    end = std::chrono::high_resolution_clock::now();
    printTime("Increment 3",end-start);


    bool allOk = true;
    start = std::chrono::high_resolution_clock::now();
    for(size_t i=0; i<list.size(); ++i)
    {
        if(list[i] != i+3)
            allOk = false;
    }
    end = std::chrono::high_resolution_clock::now();
    printTime("Increment check",end-start);
    qDebug() << "Check = "<<allOk;

}

void printTime(const string &msg, const std::chrono::duration<long long, std::ratio<1,1000000000>> &duration)
{
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration); // Microsecond (as int)
   // auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration); // Milliseconds (as int)
    const float ms_fractional = static_cast<float>(us.count()) / 1000;         // Milliseconds (as float)
    qDebug() << msg.c_str() << " Duration = " << us.count() << "µs (" << ms_fractional << "ms)";
}

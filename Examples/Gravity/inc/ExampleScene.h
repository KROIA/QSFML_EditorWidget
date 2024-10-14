#pragma once

#include <QMainWindow>
#include "QSFML_EditorWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ExampleScene; }
QT_END_NAMESPACE

class ExampleScene : public QMainWindow
{
    Q_OBJECT

public:
    ExampleScene(QWidget* parent = nullptr);
    ~ExampleScene();

    private slots:
    void onScreenCapture();
private:
    void setupScene();
    void closeEvent(QCloseEvent* event) override;


    Ui::ExampleScene* ui;
	
    struct ThreadData
    {
		struct ImageData
		{
			sf::Image *image = nullptr;
			size_t index;
		};
		ThreadData()
		{

		}
        ThreadData(const ThreadData&)
        {
			
        }
        std::thread* thread = nullptr;
		std::vector<ImageData> images;
        std::mutex mutex;
        std::condition_variable condition;
        Log::LogObject log;
    };
	std::vector<ThreadData> m_threadData;

    QSFML::Scene* m_scene;
};

#pragma once

#include <QMainWindow>
#include "QSFML_EditorWidget.h"
#include <QCheckbox>

QT_BEGIN_NAMESPACE
namespace Ui { class ExampleScene; }
QT_END_NAMESPACE

class ExampleScene : public QMainWindow
{
    Q_OBJECT

public:
    ExampleScene(QWidget* parent = nullptr);
    ~ExampleScene();

private:
    void imageComponent();
    void linePainterComponent();
    void pathPainterComponent();
    void pixelPainterComponent();
    void pointPainterComponent();
    void rectPainterComponent();
    void shapeComponent();
    void textComponent();

    void setupScene();
    void closeEvent(QCloseEvent* event) override;
	void addNewObjectToScene(QSFML::Objects::GameObjectPtr obj);


    Ui::ExampleScene* ui;

    struct ObjectData
    {
		QSFML::Objects::GameObjectPtr obj = nullptr;
		QCheckBox* checkBox = nullptr;
    };

    QSFML::Scene* m_scene;

	QSFML::vector<ObjectData> m_objects;
};

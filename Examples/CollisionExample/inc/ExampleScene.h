#ifndef SANDBOX_H
#define SANDBOX_H

#include <QMainWindow>
#include "QSFML_EditorWidget.h"
#include "CollisionChecker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ExampleScene; }
QT_END_NAMESPACE

class ExampleScene : public QMainWindow
{
        Q_OBJECT

    public:
        ExampleScene(QWidget *parent = nullptr);
        ~ExampleScene();

private slots:
    void on_intersecting_radioButton_clicked();
    void on_containing_radioButton_clicked();
    void on_performanceTest_radioButton_clicked();
    void onTimerFinished();
    
    private:
        void closeEvent(QCloseEvent* event) override;
        Ui::ExampleScene *ui;

        QSFML::Scene *m_Scene;
        

        CollisionChecker* m_collisionChecker;
};
#endif // SANDBOX_H

#ifndef SANDBOX_H
#define SANDBOX_H

#include <QMainWindow>
#include "QSFML_EditorWidget.h"
#include "CollisionChecker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ExampleCanvas; }
QT_END_NAMESPACE

class ExampleCanvas : public QMainWindow
{
        Q_OBJECT

    public:
        ExampleCanvas(QWidget *parent = nullptr);
        ~ExampleCanvas();

private slots:
    void on_intersecting_radioButton_clicked();
    void on_containing_radioButton_clicked();
    void on_performanceTest_radioButton_clicked();
    void onTimerFinished();
    
    private:
        void closeEvent(QCloseEvent* event) override;
        Ui::ExampleCanvas *ui;

        QSFML::Canvas *m_canvas;

        CollisionChecker* m_collisionChecker;
};
#endif // SANDBOX_H

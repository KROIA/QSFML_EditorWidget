#ifndef SANDBOX_H
#define SANDBOX_H

#include <QMainWindow>
#include "QSFML_EditorWidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SandBox; }
QT_END_NAMESPACE

class SandBox : public QMainWindow
{
        Q_OBJECT

    public:
        SandBox(QWidget *parent = nullptr);
        ~SandBox();

    
private slots:
    void onTimerFinished();
    private:
        void closeEvent(QCloseEvent* event) override;
        Ui::SandBox *ui;

        QSFML::Scene *m_Scene_1;
        QSFML::Scene *m_Scene_2;

        QSFML::Components::PointPainter* m_pointPainter;

       

};
#endif // SANDBOX_H

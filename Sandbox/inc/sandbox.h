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

    private:
        Ui::SandBox *ui;

        QSFML::Canvas *m_canvas_1;
        QSFML::Canvas *m_canvas_2;
};
#endif // SANDBOX_H

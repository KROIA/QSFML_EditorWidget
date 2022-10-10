#ifndef SANDBOX_H
#define SANDBOX_H

#include <QMainWindow>
#include "Canvas.h"

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

        QSFML::Canvas *m_canvas;
};
#endif // SANDBOX_H

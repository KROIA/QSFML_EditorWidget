#include "ExampleCanvas.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ExampleCanvas w;
    w.show();
    return a.exec();
}

#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

class SFML_ViewWidget : public QObject
{
        Q_OBJECT

    public:
        SFML_ViewWidget();
        ~SFML_ViewWidget();

    private slots:
        void test_case1();

};

SFML_ViewWidget::SFML_ViewWidget()
{

}

SFML_ViewWidget::~SFML_ViewWidget()
{

}

void SFML_ViewWidget::test_case1()
{

}

QTEST_MAIN(SFML_ViewWidget)

#include "tst_sfml_viewwidget.moc"

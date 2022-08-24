#include "Headers/Views/MainWindow.hpp"
#include<QTranslator>
#include <QApplication>

int main( int argc, char *argv[] )
{
    QApplication application( argc, argv );

    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load("diplom_" + locale);

    application.installTranslator(&translator);

    MainWindow window;
    window.show();

    return application.exec();

}

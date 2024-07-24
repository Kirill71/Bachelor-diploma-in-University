#include "Views/MainWindow.hpp"
#include<QTranslator>
#include <QApplication>

int main( int argc, char *argv[] )
{
    QApplication application( argc, argv );

    const auto locale = QLocale::system().name();
    QTranslator translator;
    if (translator.load("diploma_" + locale))

    QApplication::installTranslator(&translator);

    MainWindow window;
    window.show();

    return QApplication::exec();

}

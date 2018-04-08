#include "Headers/Views/MainWindow.hpp"

#include <QApplication>
#include <QDir>

int main( int argc, char *argv[] )
{
    QApplication application( argc, argv );

    MainWindow window;

    window.show();

    return application.exec();

}

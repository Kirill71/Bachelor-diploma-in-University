#include "MainWindow.hpp"

#include <QApplication>
#include <QDir>

int main( int argc, char *argv[] )
{
    QApplication a( argc, argv );

    const QString inputFilePath{ QDir::currentPath() + "/Input_value_viol.inf" };
    const QString statFilePath{ QDir::currentPath() + "/Stat.inf" };

    MainWindow w(
             nullptr
         ,   inputFilePath.toStdString()
         ,   statFilePath.toStdString()
    );

    w.show();

    return a.exec();

}

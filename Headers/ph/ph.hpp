/*
    2018

    Author : Kirill Zharenkov, Aldec-KTC Team.

    Date: 01.11.2017

    Purpose: Pre-compiled header for this project.

    All right reserved (c).

*/

#ifndef PH_HPP_
#define PH_HPP_

// STL Headers
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <fstream>
#include <iostream>
#include <initializer_list>
#include <tuple>
#include <map>

#ifdef _WIN32
  #define _USE_MATH_DEFINES
#endif
#include <math.h>

// Boost headers
#include<boost/noncopyable.hpp>
#include<boost/lexical_cast.hpp>


// Qt Headers
#include <QChart>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegend>
#include <QList>
#include <QUrl>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>

//User defined headers
#include "Headers/Model/Setup.hpp"


/*---------------------------------------------------------------------------*/

#endif // ! _PH_HPP_

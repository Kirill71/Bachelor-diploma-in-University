#include "Headers/Model/PhisicalModel.hpp"

/*---------------------------------------------------------------------------*/

PhisicalModel::PhisicalModel(
        const std::string & _inputFilePath
    ,   const std::string & _inputStatFilePath
 )
    :   m_log (  makeLogImpl( _inputFilePath, _inputStatFilePath ) )
{
} // PhisicalModel::PhisicalModel

/*---------------------------------------------------------------------------*/

PhisicalModel::~PhisicalModel()
{
    m_log.reset();

} // PhisicalModel::~PhisicalModel

/*---------------------------------------------------------------------------*/

PhisicalModel::ChartData PhisicalModel::calculatePhisicalModel()
{
    using VectorInt = std::vector < int >;

    auto log = [ this ]( Log::FileType _type = Log::FileType::Scattering )-> auto&
    {
        return m_log->log( _type );

    };

    size_t N, M, L, Itr;            // размерность массивов

    double    resonanceFreq,   // Резонансная частота
                radiusNorm,			// нормированный радиус пузырька
                radiusBubbleMin,    // R_min < radiusBubble
                radiusBubbleMax,    // radiusBubble > R_max
                h,				 // указатель глубины
                eta,                // teta > 0  // не понятно, название этой переменной
                deltaRadius,				 // дельта прирашения по радиусу - не понятно, название этой переменной
                dd;				 // не понятно, название этой переменной

    double alpha, err;

    log( Log::FileType::Input ) >> N;
    log( Log::FileType::Input ) >> M;
    log( Log::FileType::Input ) >> L;
    log( Log::FileType::Input ) >> radiusBubbleMin >> radiusBubbleMax;
    log( Log::FileType::Input ) >> h;
    log( Log::FileType::Input ) >> eta;
    log( Log::FileType::Input ) >> Itr;
    log( Log::FileType::Input ) >> err;

    // in the new method
    Table table;
    while( !log( Log::FileType::Stat ).eof() )
    {
        double first, second;
        log( Log::FileType::Stat ) >> first >> second;
        table.push_back( std::make_pair( first, second ) );
    };

    table.erase( --table.end() );

    Vector F_exp( L );  // массив частот эксперимента
    Vector Aph( Itr );  // массив для поиска альфы, далее будет замена на оптимизирующий алгоритм
    Vector delta_al( Itr );  // массив критериев отбора

    Matrix A( M , M );  // матрица для СЛАУ
    Vector B( M );  // вектор правой части
    Vector mv_f( L );  // то, что мы должны приблизить к решению

    Matrix Umn( M + 1, N + 1 );  // матрица базисных функций
    Matrix Imn( M + 1, N + 1 );  // матрица интеграла от б.ф
    Matrix Kmn_fr( L, N + 1 );  // матрица ядра подинтегральной функции
    Vector Lm_r( M + 1 );  // полиномы
    Matrix Im_N( L, M + 1 );  // матрица интегрирования

          /*  массивы для посчетов количества пузырьков */
    Vector Nm( N );
    Vector Nm_s( M );
    VectorInt INm( N );
    Vector n_r( N );

    log() << "N=" << N << " M=" << M << std::endl;
    log() << "R_min=" << radiusBubbleMin << "м" << " R_max=" << radiusBubbleMax << "м" << std::endl;
    log() << "h=" << h << "м" << std::endl;
    log() << "eta = " << eta << std::endl;
    log() << "Число инераций: " << Itr << " Ошибка точности вычислений: " << err << std::endl;
    log() << "\n" << std::endl;

    log() << "   Прямая задача\n" << std::endl;
    log() << "   Частоты контроля\n" << std::endl;

    for ( size_t i = 0; i < L; i++)
    {
        log( Log::FileType::Input ) >> F_exp[i];       //  частоты исследования
        log() << F_exp[i] << std::endl;
    }

    log() << "   Регуляризирующие значения альфа\n" << std::endl;
    for (size_t i = 0; i < Itr; ++i)
    {
        log( Log::FileType::Input ) >> Aph[i];
        log() << Aph[i] << std::endl;
    }
    radiusNorm = radiusBubbleMin / radiusBubbleMax;

    deltaRadius = (1.0 - radiusNorm) / (N - 1);

     // in the new method. mb too.
    Vector radiusCollection( N );
    for (size_t i = 0; i < N; ++i )
        radiusCollection[ i ] = radiusNorm + deltaRadius * i;

    log() << std::endl << std::endl;

    //   dd = 3.26*sqrt(1.0+0.098*h);   // for gas
    //dd = 3.15*sqrt( 1.0 + 0.1*h );         // for methane
    dd = 10.6276*(1.0+0.00985*h);

    double a,f,b;

    a = 1.2;
    b = 7.5;

    for ( size_t i = 0; i < N; ++i )
    {
        for (size_t j=0; j<M; ++j )
            Umn[ j ][ i ] =
                exp( -b *( j + 1 ) * radiusCollection[ i ] ) * pow( radiusCollection[ i ] , a );
    }

  // Вычисление ядра


    b = 0.32;
    double squareRadius( 0.0 );
    for ( size_t k = 0; k < L; ++k )
    {
        f = F_exp[k]; // не понятно название этой переменной

        for ( size_t i = 0; i < N; ++i )
        {
            squareRadius =  radiusCollection[ i ] * radiusCollection[ i ];
            resonanceFreq = dd/(radiusBubbleMax*radiusBubbleMax* squareRadius );

            // The Medwin formula - the backscattering cross-section of a gas-filled

            Kmn_fr[k][i] = squareRadius / ((resonanceFreq / (f * f) - 1.0)*(resonanceFreq / (f * f) - 1.0 ) + b * b );
        }
    }

     // Прямая задача
    double ser = 0.0;    // Остаточный член суммирования

    for ( size_t k = 0; k < L; ++k )
    {
        for (  size_t j = 0; j < M; ++j )
        {
            Imn[j][0] = 0.0;
            for ( size_t i = 0; i<N; ++i )
            {
                ser = 0.5*deltaRadius*(Kmn_fr[k][i] * Umn[j][i] + Kmn_fr[k][i + 1] * Umn[j][i + 1]);
                Imn[j][i + 1] = Imn[j][i] + ser;
            }
            Im_N[k][j] = Imn[j][N];
        }
    }

    double n_0 = 120.0; // не понятно название этой переменной
    double summ = 0.0;
       std::for_each(
            table.cbegin()
         ,  table.cend()
         ,  [ &summ ]( auto it )
            {
                summ += it.second;
            }
      );
    eta = 2.7; /* ( 1 / table.size() ) * summ; // 1/n*summ( y[i])*/

    summ = 0.0;
    std::for_each(
        table.cbegin()
     ,  table.cend()
     ,  [ &summ, eta ]( auto it )
        {
            summ += pow( it.second - eta, 2 );
        }
    );
    a = 15.0; /*sqrt( (1 / table.size() - 1) * summ );*/

    for ( size_t i = 0; i<N; ++i )
    {
        Nm[i] = n_0*pow(radiusCollection[i], eta / 2.0)*exp(-radiusCollection[i]*a / 2.0);
        log() << radiusCollection[ i ] << "  " << Nm[i] << std::endl;
    }
  // Конец загрузки и подготовки массивов для СЛАУ
  /*
  Вместо примера 2 нужно будет читать исходные данные, рисовать гистограмму, определять функцию распределения
  и по ней находить Nm[i]
  */

    for ( size_t k = 0; k < L; ++k )
    {
        mv_f[k] = 0.0;
        for (size_t i=0; i<N; ++i )
            mv_f[ k ] += 0.5 * deltaRadius * ( Kmn_fr[ k ][ i ] * Nm[ i ] + Kmn_fr[ k ][ i+1 ] * Nm[ i+1 ] );

        log() << "\nКоэффициент объемного рассеяния на частоте " << F_exp[k] << " = " << mv_f[k] << "\n" << std::endl;
    }

    log() << "\n" << std::endl;

    Vector mv_fal( L ); // не понятно название этой переменной

    log() << "\n   Обратная задача\n" << std::endl;


  // Формируем матрицу системы и свободный вектор
  //   для СЛАУ (метод регуляризации Лаврентьева)

    for ( size_t j = 0; j < M; ++j )
    {
        B[j] = 0.0;
        for ( size_t i = 0; i < M; ++i )
        {
            A[j][i] = 0.0;
            for ( size_t k = 0; k < L; ++k )
                A[j][i] += Im_N[k][i] * Im_N[k][j] / L;
        }

        for ( size_t k = 0; k < L; ++k )
        {
            B[j] += mv_f[k] * Im_N[k][j];
        }
    }

    double D_min_alph = 1.0; // не понятно название этой переменной

    for ( size_t iter = 0; iter < Itr; ++iter ) // Iteration
    {
        alpha = Aph[iter];
        log() << " Alpha : = " << alpha << std::endl;

         for ( size_t i = 0; i < M; ++i )
            A[i][i] += alpha;

         log() << "\n Исходная матрица [A]: " << std::endl;

         for (size_t i = 0; i < M ; ++i, std::cout << std::endl )
         {
            for (size_t j = 0; j < M; ++j)
            {
                log() << A[i][j] << " ";
                log() << std::endl;
            }

            log() << "\n" << B[i] << "\n";
            log() << std::endl;
         }

      //  Решение СЛАУ

        Gauss resolver(A, B);
        auto X = resolver.getRoots();

      // Анализ результатов
        log() << "       Решение СЛАУ\n" << std::endl;

        if ( X.empty() )
          log() << "Система несовместна";
        else
        {
            for (size_t i = 0; i < M; ++i )
            {
                log() << "X[" << i + 1 << "]" << " = " << X[ i ];
                if  ( fabs(X[ i ] ) > err )
                {
                    if ( X[ i ] > 0 )
                        log() << " + " << X[ i ] << "A[" << i + 1 << "]";
                     else
                        log() << " - " << -X[ i ] << "A[" << i + 1 << "]";
                 }
             }
             log() << std::endl;
       }

       for ( size_t k = 0; k < L; ++k )
       {
            mv_fal[ k ] = 0.0;

            for ( size_t i = 0; i < M; ++i)
            {
                Nm_s[i] = X[i];
                mv_fal[k] += Nm_s[i] * Im_N[k][i];
            }
            log() << "\nКоэффициент объемного рассеяния обратной задачи - " << mv_fal[k] << std::endl;
        }

        double	SZ = 0.0, SCH = 0.0; // не понятно название этой переменной

        for ( size_t i = 0; i < L; ++i )
        {
            SZ += mv_f[ i ] * mv_f[ i ];
            SCH += ( mv_f[ i ] - mv_fal[ i ] ) * ( mv_f[ i ] - mv_fal[ i ] );
        }

        delta_al[iter] = SCH / SZ;

        if ( delta_al[ iter ] <= D_min_alph )
        {
            log( Log::FileType::Chart ) << "\n Alpha = " << alpha << " Delta-alpha = " << delta_al[iter] << "\n" << std::endl;
            for ( size_t k = 0; k < N; ++k )
            {
                for ( size_t i = 0; i < M; ++i )
                    n_r[k] += Nm_s[i] * Umn[i][k];
            }
        }
    }

  for ( size_t i = 0; i<N; ++i)
    log( Log::FileType::Chart ) << radiusCollection[ i ] << " " << n_r[ i ] << std::endl;

  return { table, radiusCollection, Nm, n_r };

} // PhisicalModel::calculatePhisicalModel

/*---------------------------------------------------------------------------*/

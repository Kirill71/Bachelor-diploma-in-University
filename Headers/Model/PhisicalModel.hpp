/*
	2018

	Author : Kirill Zharenkov, Aldec-KTC Team.

	Date creation: 20.01.2018

	Purpose: class for representation phisical model.

	All right reserved (c).

*/

/*---------------------------------------------------------------------------*/

#ifndef _PHISICAL_MODEL_HPP_
#define _PHISICAL_MODEL_HPP_

/*---------------------------------------------------------------------------*/

#include"Headers\ph\ph.hpp"

/*---------------------------------------------------------------------------*/

class PhisicalModel
{

/*---------------------------------------------------------------------------*/

	using FileImpl = std::fstream;

	using FileImplPtr = std::unique_ptr< FileImpl >;

/*---------------------------------------------------------------------------*/

	const std::string ScatteringFilePath = "Scattering.dat";

	const std::string ChartFilePath = "Chart.dat";

/*---------------------------------------------------------------------------*/

	 const double averageSpeedSoundInSeaWater = 1.522E5;

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

	PhisicalModel() = default;

	 explicit PhisicalModel( const std::string& _filePath );

	 ~PhisicalModel();

/*---------------------------------------------------------------------------*/

public:

/*---------------------------------------------------------------------------*/

	 void calculatePhisicalModel();

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

	double  beta( double _scalar ) const noexcept;

	double resonanceFrequency( double _radius, double _d ) const noexcept;


private:

/*---------------------------------------------------------------------------*/

	auto makeFileImpl( 
			const std::string& _filePath
		,	FileImpl::openmode _mode = FileImpl::in 
	) const noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

	FileImplPtr m_inputFile;

	FileImplPtr m_outputScatteringFile;
	FileImplPtr m_outputGraphicsFile;
/*---------------------------------------------------------------------------*/

}; // class PhisicalModel

 /*---------------------------------------------------------------------------*/


inline double 
PhisicalModel::beta( double _scalar ) const noexcept
{
	return _scalar * _scalar;

} // PhisicalModel::beta

/*---------------------------------------------------------------------------*/

inline double 
PhisicalModel::resonanceFrequency( double _radius, double _d ) const noexcept
{
	return _radius / _d;

} // PhisicalModel::resonanceFrequency

/*---------------------------------------------------------------------------*/

inline auto
PhisicalModel::makeFileImpl(
		const std::string& _filePath
	,	FileImpl::openmode _mode 
) const noexcept
{
	return std::make_unique< FileImpl >( _filePath, _mode );

} // PhisicalModel::makeFileImpl


/*---------------------------------------------------------------------------*/

#endif // ! _PHISICAL_MODEL_HPP

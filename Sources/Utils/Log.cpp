#include "Headers/Utils/Log.hpp"

/*-----------------------------------------------------------------*/

Log::Log(
        const std::string& _inputFilePath
    ,   const std::string& _inputStatFilePath
)
  : m_inputFile( makeFileImpl( _inputFilePath ) )
  , m_inputStatFile( makeFileImpl( _inputStatFilePath ) )
  , m_scatteringFile( makeFileImpl( ScatteringFilePath, FileImpl::out ) )
  , m_chartFile( makeFileImpl( ChartFilePath, FileImpl::out ) )
{
    if ( !m_inputFile->is_open() )
        throw std::runtime_error( "File with input data non opened" );

    if ( !m_inputStatFile->is_open() )
        throw std::runtime_error( "File with input statistic data non opened" );


} // Log::Log

/*-----------------------------------------------------------------*/

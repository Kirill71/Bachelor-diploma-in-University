/*
    2018

    Author : Kirill Zharenkov, Aldec-KTC Team.

    Date creation: 24.03.2018

    Purpose: class for representation working with phisical files.

    All right reserved (c).

*/

/*---------------------------------------------------------------------------*/

#ifndef LOG_HPP_
#define LOG_HPP_

/*---------------------------------------------------------------------------*/

#include "ph/ph.hpp"

/*---------------------------------------------------------------------------*/

class Log
    : public boost::noncopyable
{

/*---------------------------------------------------------------------------*/

    using FileImpl = std::fstream;

    using FileImplPtr = std::unique_ptr< FileImpl >;

/*---------------------------------------------------------------------------*/

    const std::string ScatteringFilePath = "Scattering.dat";

    const std::string ChartFilePath = "Chart.dat";

/*---------------------------------------------------------------------------*/

 public:

/*---------------------------------------------------------------------------*/

    enum class FileType
    {
        Stat,
        Scattering,
        Chart
    };

/*---------------------------------------------------------------------------*/

    explicit Log( const std::string& _inputStatFilePath );

    FileImpl& log( FileType _type = FileType::Scattering );

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    auto makeFileImpl(
         const std::string& _filePath
      ,  FileImpl::openmode _mode = FileImpl::in
    ) const noexcept;

/*---------------------------------------------------------------------------*/

private:

/*---------------------------------------------------------------------------*/

    FileImplPtr m_inputStatFile;

    FileImplPtr m_scatteringFile;

    FileImplPtr m_chartFile;

/*---------------------------------------------------------------------------*/

}; // class Log

/*---------------------------------------------------------------------------*/

inline Log::FileImpl&
Log::log( FileType _type )
{
    switch ( _type )
    {
        case FileType::Stat:
            return *m_inputStatFile;

        case FileType::Scattering:
            return *m_scatteringFile;

        case FileType::Chart:
            return *m_chartFile;
        default:
            return *m_inputStatFile;
    }

} // Log::log

/*---------------------------------------------------------------------------*/

inline auto
Log::makeFileImpl(
        const std::string& _filePath
    ,	FileImpl::openmode _mode
) const noexcept
{
        return std::make_unique< FileImpl >( _filePath, _mode );

} // Log::makeFileImpl

/*---------------------------------------------------------------------------*/

#endif // ! LOG_HPP_

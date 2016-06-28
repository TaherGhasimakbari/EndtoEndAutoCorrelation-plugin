#ifndef __ENDTOEND_AUTOCORRELATION_H__
#define __ENDTOEND_AUTOCORRELATION_H__

/*! \file EndtoEndAutoCorrelation.h
    \brief Declares the EndtoEndAutoCorrelation class
 */
#include <hoomd/hoomd.h>
#include </home/morsedc/tghasi/EndtoEndAutoCorrelation-plugin/util/AutoCorrArray.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <complex>

// need to declare these classes with __host__ __device__ qualifiers when building in nvcc
// HOiSTDEVICE is __host__ __device__ when included in nvcc and blank when included into the host compiler
#ifdef NVCC
#define HOSTDEVICE __host__ __device__
#else
#define HOSTDEVICE
#endif

using namespace Util;

//! Collective variable for studying phase transitions in block copolymer systems
class EndtoEndAutoCorrelation : public Analyzer
    {
    public:
        /*! Constructs the structure factor plugin
            \param sysdef The system definition
            \param chainLength
            \param endtime
            \param filename Filename for the results
            \param overwrite Whether the log file should be overwritten
         */
        EndtoEndAutoCorrelation(boost::shared_ptr<SystemDefinition> sysdef,
                               const std::string& filename,
                               const unsigned int& chainlength,
                               const unsigned int& endtime,
                               bool overwrite=false
                               );
        virtual ~EndtoEndAutoCorrelation() {}

        void analyze(unsigned int timestep);

    protected:
        std::string m_filename;                                     //!< The name of log file for CV
        std::ofstream m_file;   	                            //!< The VanHove file handle
        std::string m_delimiter;                                    //!< Record delimiter
        bool m_appending;                                           //!< Whether we are appending to the log file
        unsigned int m_chainlength;                                 //!< Stores the list of miller indices
        unsigned int m_endtime;                                     //!< Stores the last step of VanHove

        Util::DArray<Scalar3> m_data;                               //!< Stores the delta r
        Util::AutoCorrArray<Scalar3, double> m_accumulator;         //!< The accumulator that keeps CVmax data

    };

//! Export EndtoEndAutoCorrelation to python
void export_EndtoEndAutoCorrelation();

#endif // __ENDTOEND_AUTOCORRELATION_H__

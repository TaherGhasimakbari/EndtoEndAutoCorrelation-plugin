/* \file EndtoEndAutoCorrelation.cc
 * \brief Implements the EndtoEndAutoCorrelation class
 */
#include "EndtoEndAutoCorrelation.h"

#include <iomanip>
#include <stdexcept>

#include <complex>
#include <boost/python.hpp>
#include <boost/filesystem.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace boost::python;
using namespace boost::filesystem;
using namespace std;

EndtoEndAutoCorrelation::EndtoEndAutoCorrelation(boost::shared_ptr<SystemDefinition> sysdef,
                               const std::string& filename,
                               const unsigned int& chainlength,
                               const unsigned int& endtime,
                               bool overwrite)
    : Analyzer(sysdef)
    {
    m_filename = filename;
    m_chainlength = chainlength;
    m_endtime = endtime;

    m_data.allocate(m_pdata->getN()/m_chainlength);
    m_accumulator.setParam(m_pdata->getN()/m_chainlength, 1000000);
    m_accumulator.setNEnsemble(m_pdata->getN()/m_chainlength);
    m_accumulator.clear();
    }

void EndtoEndAutoCorrelation::analyze(unsigned int timestep)
    {

    ArrayHandle<Scalar4> h_postype(m_pdata->getPositions(), access_location::host, access_mode::read);

    Scalar4 postype;
    Scalar3 R_Start;
    Scalar3 R_End;
    Scalar3 Delta_R;
    
    for (unsigned int idx = 0; idx < m_pdata->getN()/m_chainlength; idx++)
        {
        postype = h_postype.data[idx];
        R_Start = make_scalar3(postype.x, postype.y, postype.z); 
        postype = h_postype.data[idx + m_chainlength -1];
        R_End = make_scalar3(postype.x, postype.y, postype.z); 
        Delta_R = R_Start - R_End;
        m_data[idx] = m_pdata->getBox().minImage(Delta_R);
        }
    
    bool is_root = true;
    //#ifdef ENABLE_MPI
    //if (m_pdata->getDomainDecomposition())
    //    is_root = m_exec_conf->isRoot();
    //#endif

    if (is_root)
        {
        //for (unsigned int idx = 0; idx < m_data.capacity(); idx++)
        m_accumulator.sample(m_data);

        // Calculate value of collective variable (sum of real parts of fourier modes)
        if (timestep == m_endtime)
            {
            std::stringstream ss;
            ss << m_filename;
            m_exec_conf->msg->notice(3) << "EndtoEndAutoCorrelation: Creating new log in file: " << ss.str().c_str() << endl;
            m_file.open(ss.str().c_str(), ios_base::out);
            m_accumulator.output(m_file);
            m_file.close();

            if (!m_file.good())
                {
                m_exec_conf->msg->error() << "EndtoEnsAutoCorrelation: Error opening file " << m_filename << endl;
                throw runtime_error("Error Initializing EndtoEndAutoCorrelation Files");
                }
            }
        } 
    }


void export_EndtoEndAutoCorrelation()
    {
    class_<EndtoEndAutoCorrelation, boost::shared_ptr<EndtoEndAutoCorrelation>, bases<Analyzer>, boost::noncopyable >
        ("EndtoEndAutoCorrelation", init< boost::shared_ptr<SystemDefinition>,
                                         const std::string&,
                                         const unsigned int,
                                         const unsigned int,
                                         bool>());
    }

#pragma once

#include <cmath>
#include <ctime>
#include <tuple>
#include <stdexcept>
#include "Time.h"

namespace SiriusFM
{
    template <typename Diffusion1D, 
              typename AProvider, 
              typename BProvider,
              typename AssetClassA,
              typename AssetClassB,
              typename PathEvaluator>

    class MCEngine1D
    {
    private:
        long const     m_MaxL;  // Max Path Length
        long const     m_MaxPM; // Max Number of Stored
        double * const m_paths; // Stored Path
        double * const m_ts;    // Timeline
    public:
        MCEngine1D (long a_MaxL, long a_MaxPM) :
            m_MaxL  (a_MaxL),
            m_MaxPM (a_MaxPM),
            m_paths (new double [m_MaxL * m_MaxPM]), 
            m_ts    (new double [m_MaxL])
        {
            if (m_MaxL <= 0 || m_MaxPM <= 0)
            {
                throw std::invalid_argument("Bad MaxL or MaxPM");
            }

            for (long l = 0; l < m_MaxL; ++l)
            {
                m_ts[l] = 0;
                long lp = l * m_MaxPM;
                for (long p = 0; p < m_MaxPM; ++p)
                {
                    m_paths[lp + p] = 0;
                }
            }
        }


        template <bool IsRN>
        inline void Simulate 
            (time_t              a_t0,          // Pricing Time 
             time_t              a_T,           // Expir. Time
             int                 a_tauMins, 
             long                a_P,           // Paths number
             bool                a_useTimerSeed,
             Diffusion1D const * a_diff,
             AProvider   const * a_rateA,
             BProvider   const * a_rateB,
             AssetClassA         a_assetA,
             AssetClassB         a_assetB,
             PathEvaluator *     a_pathEval);


        MCEngine1D (const MCEngine1D &) = delete;

        MCEngine1D & operator = (const MCEngine1D &) = delete;
        
        ~ MCEngine1D ()
        {
            delete [] m_paths;
            delete [] m_ts;
        }
    };
}

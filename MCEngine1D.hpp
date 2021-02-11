#pragma once

#include <cassert>
#include <random>
#include "MCEngine1D.h"

namespace SiriusFM
{
    template <typename Diffusion1D, 
              typename AProvider, 
              typename BProvider,
              typename AssetClassA,
              typename AssetClassB,
              typename PathEvaluator>

    template <bool IsRN>
    inline void MCEngine1D
        <Diffusion1D,
         AProvider,
         BProvider,
         AssetClassA,
         AssetClassB,
         PathEvaluator> 
        ::Simulate
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
         PathEvaluator *     a_pathEval)
    {
        // Verifications
        assert (a_diff     != nullptr                &&
                a_rateA    != nullptr                &&
                a_rateB    != nullptr                &&
                a_assetA   != AssetClassA::UNDEFINED &&
                a_assetB   != AssetClassB::UNDEFINED &&
                a_t0       <= a_T                    &&
                a_tauMins  >  0                      &&
                a_P        >  0                      &&
                a_pathEval != nullptr);

        double y0      = YearFrac(a_t0);
        time_t TSec    = a_T - a_t0;
        time_t tauSec  = a_tauMins * SEC_IN_MIN;
        long   L_segm  = (TSec % tauSec == 0) ? TSec / tauSec
                                              : TSec / tauSec + 1; 
                                                // Segments number
        double tau     = YearFracInt(tauSec);
        long   L       = L_segm + 1; // Nodes number
        long   P       = 2 * a_P;    // Antithetic variables
        

        if (L > m_MaxL)
        {
           throw std::invalid_argument("Too many steps");
        }

        std::normal_distribution N01(0.0, 1.0);
        std::mt19937_64          U(a_useTimerSeed ? time(nullptr) : 0);

        // PM: how many paths we can store in memory?
        long PM = (m_MaxL * m_MaxPM) / L;
        if (PM % 2 != 0)
        {
            --PM;
        }
        assert(PM > 0 && PM % 2 == 0);

        long PMh = PM / 2;
        
        // PI: number of outer P iterations:
        long PI = (P % PM == 0) ? P / PM
                                : P / PM + 1;

        // Now actual P = PI * PM

        double stau  = sqrt(tau);
        double tlast = (TSec % tauSec == 0) 
                       ? tau
                       : YearFracInt(TSec - (L - 1) * tauSec);
        assert(tlast <= tau && tlast >  0);
        double slast = sqrt(tlast);
        assert(slast <= stau && slast >  0);

        // Construct the Timeline:
        for (long l = 0; l < L - 1; ++l)
        {
            m_ts[l] = y0 + double(l) * tau;
        }
        m_ts[L - 1] = m_ts[L - 2] + tlast;

        // Main simulation loop:
        for (long i = 0; i < PI; ++i)
        {        
            // Generate in_memory paths:
            for (long p = 0; p < PMh; ++p)
            {
                double * path0 = m_paths + 2 * p * L;
                double * path1 = path0 + L;
            
                path0[0] = a_diff -> GetStart();
                path1[0] = a_diff -> GetStart();

                double Sp0 = a_diff -> GetStart();
                double Sp1 = a_diff -> GetStart();

                for (long l = 1; l < L; ++l)
                {
                    // Compute the Trend
                    double mu0 = 0.0;
                    double mu1 = 0.0;

                    double y = m_ts [l - 1]; // "l" is NEXT point
                
                    if (IsRN)
                    {
                        double delta_r = 
                            a_rateB -> r(a_assetB, y) - 
                            a_rateA -> r(a_assetA, y);
                        mu0 = delta_r * Sp0;
                        mu1 = delta_r * Sp1;
                    }
                    else
                    {
                        mu0 = a_diff -> mu(Sp0, y);
                        mu1 = a_diff -> mu(Sp1, y);
                    }

                    double sigma0 = a_diff -> sigma(Sp0, y); // Apply changes to
                    double sigma1 = a_diff -> sigma(Sp1, y); // all diffusions

                    // S_t = S_(t-1) + mu * tau + sigma(tau) * Z;
                    //                            -------------- N(0,1)
                    //                            ------------- dW(0, sqrt(tau))

                    double Z   = N01(U);
                    double Sn0 = 0;
                    double Sn1 = 0;

                    if (l == L - 1)
                    {
                        Sn0 = Sp0 + mu0 * tlast + sigma0 * Z * slast;
                        Sn1 = Sp1 + mu1 * tlast - sigma1 * Z * slast;

                        //y += tlast;
                    }
                    else
                    {
                        Sn0 = Sp0 + mu0 * tau + sigma0 * Z * stau;
                        Sn1 = Sp1 + mu1 * tau - sigma1 * Z * stau;

                        //y += tau;
                    }
                
                    path0[l] = Sn0;
                    path1[l] = Sn1;

                    Sp0 = Sn0;
                    Sp1 = Sn1;
                } // End of l Loop
            } // End of p Loop
            
            // Evaluate the in-memory paths:
            (* a_pathEval)(L, PM, m_paths, m_ts);

        } // End of i Loop
    }

}

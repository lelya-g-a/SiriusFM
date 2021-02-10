#pragma once

#include <cassert>
#include <random>
#include "MCEngine1D.h"
#include "Time.h"

namespace SiriusFM
{
    template <typename Diffusion1D, 
              typename AProvider, 
              typename BProvider,
              typename AssetClassA,
              typename AssetClassB>

    template <bool IsRN>
    inline void MCEngine1D
        <Diffusion1D,
         AProvider,
         BProvider,
         AssetClassA,
         AssetClassB> 
        ::Simulate
        (time_t              a_t0,     // Pricing Time 
         time_t              a_T,      // Expir. Time
         int                 a_tau_min,
         double              a_s0,     // Start
         long                a_P,      // Paths number
         Diffusion1D const * a_diff,
         AProvider   const * a_rateA,
         BProvider   const * a_rateB,
         AssetClassA         a_A,
         AssetClassB         a_B)
    {
        // Verifications
        assert (a_diff    != nullptr                &&
                a_rateA   != nullptr                &&
                a_rateB   != nullptr                &&
                a_A       != AssetClassA::UNDEFINED &&
                a_B       != AssetClassB::UNDEFINED &&
                a_t0      <= a_T                    &&
                a_tau_min >  0                      &&
                a_P       >  0);

        double y0      = YearFrac(a_t0);
        time_t T_sec   = a_T - a_t0;
        time_t tau_sec = a_tau_min * SEC_IN_MIN;
        long   L_segm  = (T_sec % tau_sec == 0) ? T_sec / tau_sec
                                                : T_sec / tau_sec + 1; 
                                                // Segments number
        double tau     = YearFrac_Int(tau_sec);
        long   L       = L_segm + 1; // Nodes number
        long   P       = 2 * a_P; // Antithetic variables
        
        if (L * P > m_MaxL * m_MaxP)
        {
           throw std::invalid_argument("Wrong paths");
        }

        std::normal_distribution N01(0.0, 1.0);
        std::mt19937_64          U;
        
        double stau  = sqrt(tau);
        double tlast = (T_sec % tau_sec == 0) 
                       ? tau
                       : YearFrac_Int(T_sec - (L-1) * tau_sec);
        assert(tlast <= tau &&
               tlast >  0);
        double slast = sqrt(tlast);
        assert(slast <= stau &&
               slast >  0);

        // Main Simulation Loop
        for (long p = 0; p < a_P; ++p)
        {
            double * path0 = m_paths + 2 * p * L;
            double * path1 = path0 + L;
            
            path0[0] = a_s0;
            path1[0] = a_s0;

            double y   = y0;
            double Sp0 = a_s0;
            double Sp1 = a_s0;

            for (long l = 1; l < L; ++l)
            {
                // Compute the Trend
                double mu0 = 0.0;
                double mu1 = 0.0;
                
                if (IsRN)
                {
                    double delta_r = 
                        a_rateB -> r(a_B, y) - a_rateA -> r(a_A, y);
                    mu0 = delta_r * Sp0;
                    mu1 = delta_r * Sp1;
                }
                else
                {
                    mu0 = a_diff -> mu(Sp0, y);
                    mu1 = a_diff -> mu(Sp1, y);
                }

                double sigma0 = a_diff -> sigma(Sp0, y);
                double sigma1 = a_diff -> sigma(Sp1, y);

                // S_t = S_(t-1) + mu * tau + sigma(tau) * Z;
                //                            -------------- N(0,1)
                //                            -------------- dW(0, sqrt(tau))

                double Z = N01(U);
                double Sn0;
                double Sn1;

                if (l == L - 1)
                {
                    Sn0 = Sp0 + mu0 * tlast + sigma0 * Z * slast;
                    Sn1 = Sp1 + mu1 * tlast - sigma1 * Z * slast;

                    y += tlast;
                }
                else
                {
                    Sn0 = Sp0 + mu0 * tau + sigma0 * Z * stau;
                    Sn1 = Sp1 + mu1 * tau - sigma1 * Z * stau;

                    y += tau;
                }
                
                path0[l] = Sn0;
                path1[l] = Sn1;

                Sp0 = Sn0;
                Sp1 = Sn1;
            } // End of l Loop
        }

        m_L = L;
        m_P = P;
    }

}

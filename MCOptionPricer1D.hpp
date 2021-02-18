#pragma once

#include "MCOptionPricer1D.h"
#include "MCEngine1D.hpp"

namespace SiriusFM
{
    template <typename Diffusion1D,
              typename AProvider,
              typename BProvider,
              typename AssetClassA,
              typename AssetClassB>
    

    double MCOptionPricer1D <Diffusion1D,
                             AProvider,
                             BProvider,
                             AssetClassA,
                             AssetClassB> 
        :: Px 
            (Option <AssetClassA, AssetClassB> const * a_option,
             time_t a_t0,      // Start Time
             int    a_tauMins, // Time step
             long   a_P)       // Paths number
    {
        assert(a_option  != nullptr &&  
               a_tauMins >  0       &&
               a_P       >  0);

        if (a_option -> IsAmerican())
        {
            throw std::invalid_argument("MC cannot price American options");
        }

        // Path Elevator:
        OPPathEval pathEval (a_option);

        // Using Simulate:
        m_mce.template Simulate <true> 
            (a_t0, a_option -> ExpirTime(), a_tauMins, a_P, m_useTimerSeed,
             m_diff, &m_irpA, &m_irpB, a_option -> AssetA(), 
             a_option -> AssetB(), &pathEval);

        // Get the price from PathEval:
        double px = pathEval.GetPx();

        // Apply the Discount Factor on B:
        px *= m_irpB.DF (a_option -> AssetB(), a_t0, a_option -> ExpirTime());
        return px;
    }
}

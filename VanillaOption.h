#pragma once

#include <cmath>
#include <cassert>
#include "Option.h"

namespace SiriusFM
{
    // Generic European Call:
    template <typename AssetClassA, typename AssetClassB>
    class EurCallOption final : public Option <AssetClassA,
                                               AssetClassB>
    {
    private:
        double const m_K; // K > 0
    public:
        EurCallOption (AssetClassA a_assetA,
                       AssetClassB a_assetB,
                       double      a_K, 
                       time_t      a_expirTime) :
            Option <AssetClassA, AssetClassB> 
                (a_assetA, a_assetB, a_expirTime, false),
            m_K(a_K)
        {
            if (m_K <= 0)
            {
                throw std::invalid_argument("Negative Strike");
            }
        }
    
        virtual double PayOff (long a_L,
                               double const * a_path,
                               double const * a_t = nullptr) const override
        {
            assert(a_L > 0 && a_path != nullptr);
            return std::max <double> (a_path[a_L - 1] - m_K, 0.0);
        }

        ~EurCallOption () override {}
    };
    
    
    // Generic European Put:
    template <typename AssetClassA, typename AssetClassB>
    class EurPutOption final : public Option <AssetClassA,
                                              AssetClassB>
    {
    private:
        double const m_K; // K > 0
    public:
        EurPutOption (AssetClassA a_assetA,
                      AssetClassB a_assetB,
                      double      a_K,
                      time_t      a_expirTime) :
            Option <AssetClassA, AssetClassB>
                (a_assetA, a_assetB, a_expirTime, false),
            m_K(a_K)
        {
            if (m_K <= 0)
            {
                throw std::invalid_argument("Negative Strike");
            }
        }
    
        virtual double PayOff (long a_L,
                               double const * a_path,
                               double const * a_t = nullptr) const override
        {
            assert(a_L > 0 && a_path != nullptr);
            return std::max <double> (m_K - a_path[a_L - 1], 0.0);
        }

        ~EurPutOption () override {}
    };

    //-----------------------------------------------------------------------//
    // Alias:
    using EurCallOptionFX = EurCallOption <CcyE, CcyE>;
    using EurPutOptionFX  = EurPutOption  <CcyE, CcyE>;
}

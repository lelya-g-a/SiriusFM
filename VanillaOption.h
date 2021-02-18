#pragma once

#include <cmath>
#include <cassert>
#include "Option.h"

namespace SiriusFM
{
    // Generic European or American (bur not Asian) Call:
    template <typename AssetClassA, typename AssetClassB>
    class CallOption final : public Option <AssetClassA, AssetClassB>
    {
    private:
        double const m_K; // Strike, K > 0
    public:
        CallOption (AssetClassA a_assetA,
                    AssetClassB a_assetB,
                    double      a_K, 
                    time_t      a_expirTime,
                    bool        a_isAmerican) :
            Option <AssetClassA, AssetClassB> 
                (a_assetA, a_assetB, a_expirTime, a_isAmerican, false),
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

        ~CallOption () override {}
    };
    
    
    // Generic European or American (but not Asian) Put:
    template <typename AssetClassA, typename AssetClassB>
    class PutOption final : public Option <AssetClassA, AssetClassB>
    {
    private:
        double const m_K; // Strike, K > 0
    public:
        PutOption (AssetClassA a_assetA,
                   AssetClassB a_assetB,
                   double      a_K,
                   time_t      a_expirTime,
                   bool        a_isAmerican) :
            Option <AssetClassA, AssetClassB>
                (a_assetA, a_assetB, a_expirTime, a_isAmerican, false),
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

        ~PutOption () override {}
    };

    //-----------------------------------------------------------------------//
    // Alias:
    using CallOptionFX = CallOption <CcyE, CcyE>;
    using PutOptionFX  = PutOption  <CcyE, CcyE>;
}

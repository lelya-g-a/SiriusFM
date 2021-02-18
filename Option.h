#pragma once

#include <ctime>
#include "IRProvider.h"

namespace SiriusFM
{
    template <typename AssetClassA, typename AssetClassB>
    class Option
    {
    private:
        // Option Underlying Inctrument: A/B
        AssetClassA const m_assetA;
        AssetClassB const m_assetB;
        time_t      const m_expirTime;
        bool        const m_isAmerican;
        bool        const m_isAsian;
    
    public:
        Option (AssetClassA a_assetA, 
                AssetClassB a_assetB,
                time_t      a_expirTime,
                bool        a_isAmerican,
                bool        a_isAsian) :
            m_assetA     (a_assetA),
            m_assetB     (a_assetB),
            m_expirTime  (a_expirTime),
            m_isAmerican (a_isAmerican),
            m_isAsian    (a_isAsian)
        {}

        AssetClassA AssetA () const
        {
            return m_assetA;
        }

        AssetClassB AssetB () const
        {
            return m_assetB;
        }

        time_t ExpirTime () const
        {
            return m_expirTime;
        }

        bool IsAmerican () const
        {
            return m_isAmerican;
        }

        bool IsAsian () const
        {
            return m_isAsian;
        }

        virtual double PayOff 
            (long a_L,
             double const * a_S,
             double const * a_t = nullptr) const = 0;

        virtual ~Option() {}
    };

    //-----------------------------------------------------------------//
    // Alias:
    using OptionFX = Option <CcyE, CcyE>;
}

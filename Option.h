#pragma once

#include <ctime>
#include "IRProvider.h"

namespace SiriusFM
{
    template <typename AssetClassA, typename AssetClassB>
    class Option
    {
    public:
        // Option Underlying Inctrument: A/B
        AssetClassA const m_assetA;
        AssetClassB const m_assetB;
        time_t      const m_expirTime;
        bool        const m_isAmerican;

        Option (AssetClassA a_assetA, 
                AssetClassB a_assetB,
                time_t      a_expirTime,
                bool a_isAmerican) :
            m_assetA     (a_assetA),
            m_assetB     (a_assetB),
            m_expirTime  (a_expirTime),
            m_isAmerican (a_isAmerican)
        {}

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

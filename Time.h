#pragma once

namespace SiriusFM
{
    constexpr double AVG_YEAR     = 365.25;
    constexpr double REF_POINT    = 1970.0;
    constexpr int    SEC_IN_DAY   = 86400;
    constexpr int    SEC_IN_MIN   = 60;
    constexpr int    MIN_IN_DAY   = 1440;
    constexpr double AVG_YEAR_SEC = AVG_YEAR * SEC_IN_DAY;

    inline double YearFrac (time_t a_t)
    {
        // Time: from secs to years
        return REF_POINT + double(a_t) / AVG_YEAR_SEC;
    }
    
    
    inline double YearFracInt (time_t a_t)
    {
        // Time difference: from secs to years, 
        return double(a_t) / AVG_YEAR_SEC;
    }
}

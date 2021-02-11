#pragma once

namespace SiriusFM
{
    constexpr double AVG_YEAR   = 365.25;
    constexpr double REF_POINT  = 1970.0;
    constexpr int    SEC_IN_DAY = 86400;
    constexpr int    SEC_IN_MIN = 60;
    constexpr int    MIN_IN_DAY = 1440;

    inline double YearFrac (time_t a_t)
    {
        // Avg year in seconds
        constexpr double SecY = AVG_YEAR * SEC_IN_DAY;
        return REF_POINT + double(a_t) / SecY;
    } 
    
    
    inline double YearFracInt (time_t a_t)
    {
        // Avg year in seconds
        constexpr double SecY = AVG_YEAR * SEC_IN_DAY;
        return double(a_t) / SecY;
    }
}

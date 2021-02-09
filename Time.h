#pragma once

#define AVG_YEAR   365.25
#define SEC_IN_DAY 86400.0
#define SEC_IN_MIN 60.0
#define MIN_IN_DAY 1440.0
#define REF_POINT  1970.0

namespace SiriusFM
{
    inline double YearFrac (time_t a_t)
    {
        // Avg year in seconds
        constexpr double SecY = AVG_YEAR * SEC_IN_DAY;
        return REF_POINT + double(a_t) / SecY;
    } 
    
    
    inline double YearFrac_Int (time_t a_t)
    {
        // Avg year in seconds
        constexpr double SecY = AVG_YEAR * SEC_IN_DAY;
        return double(a_t) / SecY;
    }
}

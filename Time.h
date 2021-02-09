#pragma once

#define AVG_YEAR    365.25
#define SEC_IN_YEAR 86400.0
#define SEC_IN_MIN  60
#define MIN_IN_YEAR 1440.0
#define REF_POINT   1970.0

namespace SiriusFM
{
    inline double YearFrac (time_t a_t)
    {
        // Avg year in seconds
        constexpr double SecY = AVG_YEAR * SEC_IN_YEAR;
        return REF_POINT + double(a_t) / SecY;
    }
}

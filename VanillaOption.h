#pragma once

#include "Option.h"

namespace SiriusFM
{
    class EurCallOption final : public Option
    {
    private:
        double const m_K; // K > 0
    public:
        EurCallOption (double a_K, int a_T_days) :
            Option(false, a_T_days),
            m_K(a_K)
        {
            if (m_K <= 0)
            {
                throw std::invalid_argument("Negative Strike");
            }
        }
    
        virtual double payoff (long a_L,
                               double const * a_t,
                               double const * a_S) const override
        {
            assert(a_L >  0       &&
                  // a_t != nullptr &&
                   a_S != nullptr);
            return std::max <double> (a_S[a_L - 1] - m_K, 0.0);
        }

        ~EurCallOption () override {}
    };
    
    
    class EurPutOption final : public Option
    {
    private:
        double const m_K; // K > 0
    public:
        EurPutOption (double a_K, int a_T_days) :
            Option(false, a_T_days),
            m_K(a_K)
        {
            if (m_K <= 0)
            {
                throw std::invalid_argument("Negative Strike");
            }
        }
    
        virtual double payoff (long a_L,
                               double const * a_t,
                               double const * a_S) const override
        {
            assert(a_L >  0       &&
                  // a_t != nullptr &&
                   a_S != nullptr);
            return std::max <double> (m_K - a_S[a_L - 1], 0.0);
        }

        ~EurPutOption () override {}
    };
}

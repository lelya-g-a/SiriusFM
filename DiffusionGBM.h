#pragma once

#include <iostream>

namespace SiriusFM
{
    class DiffusionGBM
    {
    private:
        double const m_mu;
        double const m_sigma;
        double const m_s0;
    public:
        DiffusionGBM (double a_mu, double a_sigma, double a_s0) : 
        m_mu    (a_mu),
        m_sigma (a_sigma),
        m_s0    (a_s0)
        {
            if (m_sigma <= 0)
            {
                throw std::invalid_argument("Negative sigma");
            }
            if (m_s0 <= 0)
            {
                throw std::invalid_argument("Negative start");
            }
        }

        double mu (double a_S, double a_t) const
        {
            return ((a_S < 0) ? 0 : m_mu * a_S);
        }
        
        double sigma (double a_S, double a_t) const
        {
            return ((a_S < 0) ? 0 : m_sigma * a_S);
        }

        double GetStart () const
        {
            return m_s0;
        }
    };
}

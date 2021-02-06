#pragma once

#include <iostream>

namespace SiriusFM
{
    class DiffusionGBM
    {
    private:
        double const m_mu;
        double const m_sigma;
    public:
        DiffusionGBM (double a_mu, double a_sigma) : 
        m_mu(a_mu),
        m_sigma(a_sigma)
        {
            if (m_sigma <= 0)
            {
                throw std::invalid_argument("Negative sigma");
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
    };
}

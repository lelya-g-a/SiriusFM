#pragma once

#include <cmath>
#include <iostream>

namespace SiriusFM
{
    class DiffusionCEV
    {
    private:
        double const m_mu;
        double const m_sigma;
        double const m_beta;
        double const m_s0;
    public:
        DiffusionCEV (double a_mu,   double a_sigma, 
                      double a_beta, double a_s0) : 
        m_mu    (a_mu),
        m_sigma (a_sigma),
        m_beta  (a_beta),
        m_s0    (a_s0)
        {
            if (m_sigma <= 0)
            {
                throw std::invalid_argument("Negative sigma");
            }
            if (m_beta < 0)
            {
                throw std::invalid_argument("Negative beta");
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
            return ((a_S < 0) ? 0 : m_sigma * pow(a_S, m_beta));
        }

        double GetStart () const
        {
            return m_s0;
        }
    };
}


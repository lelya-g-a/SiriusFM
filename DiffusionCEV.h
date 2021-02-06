#pragma once

#include <iostream>
#include <cmath>

namespace SiriusFM
{
    class DiffusionCEV
    {
    private:
        double const m_mu;
        double const m_sigma;
        double const m_beta;
    private:
        DiffusionCEV (double a_mu, double a_sigma, double a_beta) : 
        m_mu(a_mu),
        m_sigma(a_sigma),
        m_beta(a_beta)
        {
            if (m_sigma <= 0)
            {
                throw std::invalid_argument("Negative sigma");
            }
            if (m_beta < 0)
            {
                throw std::invalid_argument("Negative beta");
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
    };
}


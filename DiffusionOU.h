#pragma once

#include <iostream>
#include <cmath>

namespace SiriusFM
{
    class DiffusionOU
    {
    private:
        double const m_kappa;
        double const m_teta;
        double const m_sigma;
    public:
        DiffusionOU (double a_kappa, double a_teta, double a_sigma) :
        m_kappa(a_kappa),
        m_teta(a_teta),
        m_sigma(a_sigma)
        {
            if (m_sigma <= 0)
            {
                throw std::invalid_argument("Negative sigma");
            }
            if (m_kappa <= 0)
            {
                throw std::invalid_argument("Negative kappa");
            }
        }

        double mu (double a_S, double a_t) const
        {
            return ((a_S < 0) ? 0 : m_kappa * (m_teta - a_S));
        }

        double sigma (double a_S, double a_t) const
        {
            return ((a_S < 0) ? 0 : m_sigma);
        }
    };
}


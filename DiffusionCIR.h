#pragma once

#include <iostream>
#include <cmath>

namespace SiriusFM
{
    class DiffusionCIR
    {
    private:
        double const m_kappa;
        double const m_teta;
        double const m_sigma;
        double const m_s0;
    public:
        DiffusionCIR (double a_kappa, double a_teta, 
                      double a_sigma, double a_s0) :
        m_kappa (a_kappa),
        m_teta  (a_teta),
        m_sigma (a_sigma),
        m_s0    (a_s0)
        {
            if (m_sigma <= 0)
            {
                throw std::invalid_argument("Negative sigma");
            }
            if (m_kappa <= 0)
            {
                throw std::invalid_argument("Negative kappa");
            }
            if (m_teta <= 0)
            {
                throw std::invalid_argument("Negative teta");
            }
            if (m_s0 <= 0)
            {
                throw std::invalid_argument("Negative start");
            }
        }

        double mu (double a_S, double a_t) const
        {
            return ((a_S < 0) ? 0 : m_kappa * (m_teta - a_S));
        }

        double sigma (double a_S, double a_t) const
        {
            return ((a_S < 0) ? 0 : m_sigma * sqrt(a_S));
        }

        double GetStart () const
        {
            return m_s0;
        }
    };
}


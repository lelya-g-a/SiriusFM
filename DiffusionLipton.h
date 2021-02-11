#pragma once

#include <iostream>

namespace SiriusFM
{
    class DiffusionLipton
    {
    private:
        double const m_mu;
        double const m_sigma_0;
        double const m_sigma_1;
        double const m_sigma_2;
        double const m_s0;
    public:
        DiffusionLipton (double a_mu,      double a_sigma_0, 
                         double a_sigma_1, double a_sigma_2,
                         double a_s0) :
        m_mu      (a_mu),
        m_sigma_0 (a_sigma_0),
        m_sigma_1 (a_sigma_1),
        m_sigma_2 (a_sigma_2),
        m_s0      (a_s0)
        {
            if ((m_sigma_0 < 0) && (m_sigma_2 < 0))
            {
                throw std::invalid_argument("Negative sigma");
            }
            if (m_sigma_1 * m_sigma_1 - 4 * m_sigma_0 * m_sigma_2 >= 0)
            {
                throw std::invalid_argument("Wrong discriminant");
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
            return ((a_S < 0) ? 0 :
                m_sigma_0 + m_sigma_1 * a_S + m_sigma_2 * a_S * a_S);
        }

        double GetStart () const
        {
            return m_s0;
        }
    };
}


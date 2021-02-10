#pragma once

namespace SiriusFM
{
    class Option
    {
    protected:
        bool const m_isAmerican;
        int  const m_T_days;     // m_T_days > 0

        Option (bool a_isAmerican, int a_T_days) :
            m_isAmerican(a_isAmerican),
            m_T_days(a_T_days)
        {
            if (m_T_days <= 0)
            {
                throw std::invalid_argument("Negative T_days");
            }
        }
    public:
        virtual double payoff 
            (long a_L,
             double const * a_t,
             double const * a_S) const = 0;

        bool IsAmerican () const
        {
            return m_isAmerican;
        }

        virtual ~Option() {}
    };
}

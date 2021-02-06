#pragma once

#include "IRProvider.h"

namespace SiriusFM
{
    template <>
    class IRProvider <IRModeE::Const>
    {
    private:
        double m_IRs[int(CcyE::N)] = {0,};
    public:
        IRProvider(const char * a_file);
        
        double r (CcyE a_ccy, double a_t) const
        {
           return m_IRs[int(a_ccy)];
        }
    };
}

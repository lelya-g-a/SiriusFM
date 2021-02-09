#include <cstdlib>
#include <cstdio>
#include "IRProviderConst.h"
#define BUFF_SIZE 256
#define CCY_SIZE 3

namespace SiriusFM
{
    IRProvider <IRModeE::Const> :: IRProvider (char const * a_file)
    {
        if (a_file == nullptr || * a_file == '\0')
        {
            return;
        }

        char buff_num[BUFF_SIZE];
        char buff_ccy[CCY_SIZE + 1];

        FILE * file;
        if (!(file = fopen(a_file, "r")))
        {
            throw std::out_of_range("File error");
        }
        
        while (fgets(buff_ccy, CCY_SIZE + 1, file))
        {
            fgets(buff_num, BUFF_SIZE, file);
            m_IRs[int(Str2CcyE(buff_ccy))] = strtod(buff_num + 1, NULL);
        }

        fclose(file);
    }
}

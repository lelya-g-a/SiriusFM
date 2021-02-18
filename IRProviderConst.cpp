#include <cstdlib>
#include <cstdio>
#include "IRProviderConst.h"

namespace SiriusFM
{
    // Read currencies interest rates values from the file
    IRProvider <IRModeE::Const> :: IRProvider (char const * a_file)
    {    
        constexpr int BUFF_SIZE = 256;
        constexpr int CCY_SIZE  = 3;
       // Zero-out all the rates 
        for (int k = 0; k < int(CcyE::N); ++k)
        {
            m_IRs[k] = 0;
        }

        if (a_file == nullptr || * a_file == '\0')
        {
            throw std::runtime_error("Cannot open file");
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

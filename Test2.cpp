#include "DiffusionGBM.h"
#include "IRProviderConst.hpp"
#include "MCEngine1D.hpp"
#include "VanillaOption.h"

using namespace std;
using namespace SiriusFM;

int main (int argc, char * argv[])
{
    // Test #2

    if (argc != 9)
    {
        cerr << "Params: mu, sigma, s0, PC, K, T_days, tau_min, P" << endl;
        return 1;
    }

    double mu      = atof(argv[1]);
    double sigma   = atof(argv[2]);
    double s0      = atof(argv[3]);
    char * opttype =      argv[4] ;
    double K       = atof(argv[5]);
    long   T_days  = atol(argv[6]);
    int    tau_min = atoi(argv[7]); 
    long   P       = atol(argv[8]);

    // Check: sigma > 0, s0 > 0, T_days > 0, tau_min > 0, P > 0
    if (sigma   <= 0 ||
        s0      <= 0 ||
        T_days  <= 0 ||
        tau_min <= 0 ||
        P       <= 0) 
    {
        cerr << "All parametrs should be positive"; 
        return 1;
    }

    Option const * option = (!strcmp(opttype, "Call")) 
        ? static_cast <Option *> (new EurCallOption (K, T_days))
        : 
        (!strcmp(opttype, "Put"))
            ? static_cast <Option *> (new EurPutOption  (K, T_days))
            : throw std::invalid_argument("Not Put or Call");

    CcyE ccyA = CcyE::USD;
    IRProvider <IRModeE::Const> irp (nullptr);
    DiffusionGBM diff(mu, sigma, s0);
    
    MCEngine1D <decltype(diff),
                decltype(irp) ,
                decltype(irp) ,
                decltype(ccyA),
                decltype(ccyA)> mce (20'000, 20'000);

    time_t t0 = time(NULL);
    time_t T  = t0 + T_days * SEC_IN_DAY;
    double Ty = double(T_days) / AVG_YEAR;
    mce.Simulate <false>
        (t0, T, tau_min, P, &diff, &irp, &irp, ccyA, ccyA);

    auto res = mce.Get_paths();
    long L1  = get<0>(res);
    long P1  = get<1>(res);
    double const * paths = get<2>(res);
   // double const * ts    = get<3>(res);

    // E of log(ST)
    double EST  = 0.0;
    double EST2 = 0.0;
    int    NVP  = 0;   // Valid paths
    
    for (long p = 0; p < P1; ++p)
    {
        double const * path = paths + p * L1;
        double ST = path[L1 - 1];
        // In practice may be ST <= 0
        if (ST <= 0.0)
        {
            continue;
        }
        ++NVP;

        double RT = option -> payoff(L1, path);
        EST  += RT;
        EST2 += RT * RT;
    }

    assert(NVP > 1);

    EST /= double(NVP);

    double VarST = EST2 / double(NVP) - EST * EST;

    cout << "Expected Value: " << EST << endl;
    cout << "Standart Deviation: " << sqrt(VarST) << endl;
    
    return 0;
}

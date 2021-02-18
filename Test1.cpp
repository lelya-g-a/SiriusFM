#include "DiffusionGBM.h"
#include "IRProviderConst.h"
#include "MCEngine1D.hpp"

using namespace SiriusFM;


int main (int argc, char * argv[])
{
    // Test #1
    // Without Risk-Neutral Measure

    if (argc != 7)
    {
        std::cerr << "Params: mu, sigma, s0, T_days, tau_min, P" << std::endl;
        return 1;
    }

    double mu      = atof(argv[1]);
    double sigma   = atof(argv[2]);
    double s0      = atof(argv[3]);
    long   T_days  = atol(argv[4]);
    int    tau_min = atoi(argv[5]); 
    long   P       = atol(argv[6]);

    // Check: sigma > 0, s0 > 0, T_days > 0, tau_min > 0, P > 0
    if (sigma   <= 0 ||
        s0      <= 0 ||
        T_days  <= 0 ||
        tau_min <= 0 ||
        P       <= 0) 
    {
        std::cerr << "All parametrs should be positive" << std::endl; 
        return 1;
    }

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

        double RT = log(ST/s0);
        EST  += RT;
        EST2 += RT * RT;
    }

    assert(NVP > 1);

    EST /= double(NVP); // (mu - sigma^2 / 2) * T

    // Now find Variance of ST
    double VarST = (EST2 - double(NVP) * EST * EST) / double(NVP - 1);
                                                        // sigma^2 * T
    double sigma2E = VarST / Ty;
    double muE     = (EST + VarST / 2.0) / Ty;

    std::cout << "mu = " << mu;
    std::cout << ", muE = " << muE << std::endl;
    std::cout << "sigma2 = " << sigma * sigma;
    std::cout << ", sigma2E = " << sigma2E << std::endl;

    return 0;
}

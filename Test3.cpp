#include <iostream>
#include <cstring>
#include <stdexcept>
#include "DiffusionGBM.h"
#include "VanillaOption.h"
#include "IRProviderConst.h"
#include "MCOptionPricer1D.hpp"

using namespace SiriusFM;

int main(int argc, char** argv)
{
	if(argc != 9)
	{
		std::cerr << "Params: mu, sigma, S0, Call/Put, K, Tdays, tau_mins, P\n";
		return 1;
	}
	double       mu        = atof(argv[1]);
	double       sigma     = atof(argv[2]);
	double       S0        = atof(argv[3]);
	const char * OptType   =      argv[4] ;
	double       K         = atof(argv[5]);
	long         expirTime = atol(argv[6]);
	int          tau_mins  = atoi(argv[7]);
	long         P         = atol(argv[8]);

	assert(sigma     > 0 &&
		   S0        > 0 &&
		   expirTime > 0 &&
		   tau_mins  > 0 &&
		   P         > 0 &&
		   K         > 0);

	CcyE ccyA = CcyE::USD;
	CcyE ccyB = CcyE::USD;

	char const* ratesFileA = nullptr; //todo
	char const* ratesFileB = nullptr; //todo
	bool useTimerSeed      = true;    //to read from file

    DiffusionGBM diff(mu, sigma, S0);
	
	//pricer for FX
	MCOptionPricer1D <decltype(diff), 
                      IRPConst, 
                      IRPConst, 
                      CcyE, 
                      CcyE> 
		Pricer (&diff, ratesFileA, ratesFileB, useTimerSeed); 

	time_t t0 = time(NULL);
	time_t T  = t0 + SEC_IN_DAY * expirTime;

	OptionFX const* opt = nullptr;

	if(!strcmp(OptType, "Call"))
		opt = new CallOptionFX (ccyA, ccyB, K, T, false);
	else if(!strcmp(OptType, "Put"))
		opt = new PutOptionFX  (ccyA, ccyB, K, T, false);
	else
		throw std::invalid_argument("Bad OptType");

	double px = Pricer.Px(opt, t0, tau_mins, P);

    std::cout << px << std::endl;

	delete opt;
	return 0;
}

#ifndef PortfolioEvolver_Test
#define PortfolioEvolver_Test

#include <iostream>


#include <boost\test\unit_test.hpp>
#include <boost\timer.hpp>


#include "PortfolioEvolver.h"

#include <sjd\RateSource\GenerateRatesEnvironments.h>
#include <sjd\sjdTest\GenerateContracts.h>
#include <sjd\sjdTest\TestingParameters.h>


    
using namespace sjd;

class PortfolioEvolverTest 
{

public:
    
    //static boost::shared_ptr<RegCapUnderlyingFactors> getDefaultRegCapUnderlyingFactors();
    //static boost::shared_ptr<RegCapCreditParameters> getDefaultRegCreditParameters();

    //static void testStructureCreation();

    static void testPortfolioEvovler();

    
    static boost::unit_test_framework::test_suite* suite();    

};

#endif



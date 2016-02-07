#ifndef portfolio_test
#define portfolio_test

#include <iostream>

#include <math.h>
#include <boost\test\unit_test.hpp>

#include "Portfolio.h"

#include <sjd\RateSource\GenerateRatesEnvironments.h>
#include <sjd\sjdTest\GenerateContracts.h>
#include <sjd\sjdTest\TestingParameters.h>
// to use the isnan(...) function
#include <boost\math\special_functions\fpclassify.hpp>

using namespace sjd;

class PortfolioTest 
{

public:
    
    static boost::shared_ptr<RegCapUnderlyingFactors> getDefaultRegCapUnderlyingFactors();
    static boost::shared_ptr<RegCapCreditParameters> getDefaultRegCreditParameters();

    static void testStructureCreation();

    static void testPortfolioB2CEM();
    

    static boost::unit_test_framework::test_suite* suite();
    
};

#endif



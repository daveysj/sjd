#ifndef cashflowcontract_test
#define cashflowcontract_test

#include <iostream>

#include <math.h>
#include <boost\test\unit_test.hpp>

#include "Cashflow.h"

#include <sjd\RateSource\GenerateRatesEnvironments.h>
#include <sjd\sjdTest\GenerateContracts.h>
#include <sjd\sjdTest\TestingParameters.h>
// to use the isnan(...) function
#include <boost\math\special_functions\fpclassify.hpp>


#include <ql\time\daycounter.hpp>
#include <ql\time\daycounters\all.hpp>

using namespace sjd;

class CashflowTest 
{
  public:

    static void testBuySell();
    static void testVolume();
    static void testMaturityDate();
    static void testSettlementDate();

    static void testValue();
    static void testGetPayoffAfterMaturity();

    static boost::unit_test_framework::test_suite* suite();
};

#endif



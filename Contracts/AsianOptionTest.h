#ifndef asianoptioncontract_test
#define asianoptioncontract_test

#include <iostream>

#include "AsianOptionContract.h"
#include "EuropeanForwardContract.h"
#include "AsianForwardContract.h"

#include <sjd\RateSource\GenerateRatesEnvironments.h>
#include <sjd\sjdTest\TestingParameters.h>

#include <boost\test\unit_test.hpp>
#include <boost\math\special_functions\fpclassify.hpp> 

#include <ql\time\daycounter.hpp>
#include <ql\time\daycounters\all.hpp>

using namespace sjd;

class AsianOptionContactTest 
{
  public:

    static boost::shared_ptr<AsianOption> getDefaultAsianOption(Date anchorDate);
    static boost::shared_ptr<AsianOption> getAsianOptionWithMoneynessVol(Date anchorDate);


    static void testMaturityDate();
    static void testAveragingDates();
    static void testType();

    static void testValue();
    static void testGetPayoffAfterMaturity();    
    static void testPutCallParity();

    static void testEuropeanOptionWithMoneynessVol();

    static boost::unit_test_framework::test_suite* suite();
};

#endif


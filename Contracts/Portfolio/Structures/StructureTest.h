#ifndef structure_test
#define structure_test

#include <iostream>

#include <sjd\Contracts\AsianForwardContract.h>
#include <sjd\Contracts\AsianOptionContract.h>
#include <sjd\Contracts\EuropeanForwardContract.h>
#include <sjd\Contracts\EuropeanOptionContract.h>
#include <sjd\Contracts\Cashflow.h>
#include <sjd\Contracts\Portfolio\Structures\Structure.h>

#include <sjd\RateSource\GenerateRatesEnvironments.h>
#include <sjd\sjdTest\GenerateContracts.h>

#include <sjd\sjdTest\TestingParameters.h>

#include <boost\test\unit_test.hpp>
// to use the boost::math::isnan(...) function
#include <boost\math\special_functions\fpclassify.hpp>

#include <ql\time\daycounter.hpp>
#include <ql\time\daycounters\all.hpp>

using namespace sjd;

class StructureTest 
{
    
  public:
    static boost::shared_ptr<RegCapUnderlyingFactors> getDefaultRegCapUnderlyingFactors();

    static void testGradient();
    static void testIsUncappedOnTheLeft();
    static void testGetContractsWithUncappedExposureOnTheLeft();
    static void testQualifyForRegulatoryCapitalOffset();

    static void testMoveContracts();

    static void testSettlementDate();
    // Test for is capped / cap and mtm + addon
    static void testCashFlowN();
    static void testEuropeanForward();
    static void testAsianforward();
    static void testEuropeanOption();
    static void testAsianOption();
    static void testMixedStructures();
    static void testSpecialCases();

    static void testStructureCreation();
    static boost::unit_test_framework::test_suite* suite();
    
};

#endif



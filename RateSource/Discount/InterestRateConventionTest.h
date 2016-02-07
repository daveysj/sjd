#ifndef sjd_interestrateconvention_test
#define sjd_interestrateconvention_test

#include <iostream>
#include <boost\test\unit_test.hpp>
#include <ql\time\daycounters\actual365fixed.hpp>
#include <sjd\sjdTest\TestingParameters.h>
#include "InterestRateConvention.h"

class InterestRateConventionTest
{
  public:
    static void testDFToRateConversion();

    static boost::unit_test_framework::test_suite* suite();
};

#endif

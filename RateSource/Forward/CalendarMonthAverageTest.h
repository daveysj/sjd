#ifndef sjd_calendarmonthaverage_test_maths
#define sjd_calendarmonthaverage_test_maths

#include <iostream>

#include <sjd\RateSource\Forward\Stochastic\ForwardCurveCMAStochasticFwdVol.h>
#include <sjd\RateSource\Forward\Deterministic\ForwardCurveCMADeterministic.h>
#include <sjd\sjdTest\TestingParameters.h>
#include <sjd\RateSource\GenerateRatesEnvironments.h>

#include <boost\test\unit_test.hpp>
#include <ql\time\daycounter.hpp>
#include <ql\time\daycounters\actual365fixed.hpp>

class CalendarMonthAverageTest 
{
  public:
    static void testConstruction();
    static void testValues();
    static void testRoll_CMA();

    static boost::unit_test_framework::test_suite* suite();
};

#endif

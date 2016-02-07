#ifndef sjdfuturescurve_test_maths
#define sjdfuturescurve_test_maths

#include <iostream>
#include <sjd\RateSource\Forward\Deterministic\ForwardCurveDeterministic.h>
#include <sjd\RateSource\Forward\Stochastic\ForwardCurveStochasticFwdVol.h>

#include <sjd\sjdTest\TestingParameters.h>
#include <sjd\RateSource\GenerateRatesEnvironments.h>

#include <boost\test\unit_test.hpp>
#include <ql\time\daycounter.hpp>
#include <ql\time\daycounters\actual365fixed.hpp>

class ForwardCurveTest 
{
  public:
    static void testForwardCurveConstruction();


    static void testConsistencyBetweenGetMethods(); 
    static void testValues();
    static void testRoll();

    static void testForwardCurveRollsAtForwardVolConstruction();
    static void testRollStochastic();

    static boost::unit_test_framework::test_suite* suite();
};

#endif

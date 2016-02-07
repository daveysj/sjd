#ifndef sjdforwardcurvefx_test
#define sjdforwardcurvefx_test

#include <iostream>

#include <sjd\RateSource\Forward\Stochastic\ForwardCurveStochasticFX.h>
#include <sjd\RateSource\Forward\Deterministic\ForwardCurveDeterministicFX.h>
    
#include <sjd\sjdTest\TestingParameters.h>
#include <sjd\RateSource\GenerateRatesEnvironments.h>

#include <boost\test\unit_test.hpp>
#include <ql\time\daycounter.hpp>
#include <ql\time\daycounters\actual365fixed.hpp>

class ForwardCurveFXTest 
{
  public:

    static void getDefaultDatesAndDF(Date anchorDate, 
                                     boost::shared_ptr<DiscountCurve> &domesticDRS, 
                                     boost::shared_ptr<DiscountCurve> &foreignDRS,
                                     boost::shared_ptr<VolatilityCurve> &vrs);

    static void testForwardCurveConstruction();
    static void testDeterministicCurve();
    static void testStochasticCurve();

    static boost::unit_test_framework::test_suite* suite();
};

#endif

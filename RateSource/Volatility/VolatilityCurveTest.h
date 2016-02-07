#ifndef sjdvolatilitycurve_test_maths
#define sjdvolatilitycurve_test_maths

#include <iostream>
#include <boost\test\unit_test.hpp>

#include <sjd\RateSource\GenerateRatesEnvironments.h>

#include <sjd\sjdTest\TestingParameters.h>
#include <boost\math\special_functions\fpclassify.hpp> // isnan

class VolatilityCurveTest 
{
  public:

    static void testVolatilityCurveConstruction();
    static void testVolatilityCurveConsistencyBetweenVolAndSD(); 
    static void testVolatilityCurveValues();
    static void testVolatilityCurveRoll();

    static boost::unit_test_framework::test_suite* suite();    
};

#endif

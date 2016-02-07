#ifndef blackscholes_test_maths
#define blackscholes_test_maths

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/math/special_functions/fpclassify.hpp> // isnan
#include <sjd\sjdTest\TestingParameters.h>
#include "BlackScholesFormula.h"

class BlackScholesTest {
  public:

    static void testInputsAndOutputs();

    static void testPutCallParity();
    static void testGreeks();
    static void testSpecialCases();
    static void testErrorMessages();

    static boost::unit_test_framework::test_suite* suite();
};

#endif

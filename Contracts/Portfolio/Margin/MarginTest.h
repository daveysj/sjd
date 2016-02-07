#ifndef margin_test
#define margin_test

#include <iostream>

#include "Margin.h"
#include <sjd\sjdTest\TestingParameters.h>

#include <boost\test\unit_test.hpp>

using namespace sjd;

class MarginTest 
{
  public:

    static void testSimpleMargin();

    static boost::unit_test_framework::test_suite* suite();
};

#endif



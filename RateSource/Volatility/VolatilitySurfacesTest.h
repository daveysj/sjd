#ifndef sjdvolatilitysurfaces_test_maths
#define sjdvolatilitysurfaces_test_maths

#include <iostream>
#include <boost\test\unit_test.hpp>
#include <boost\math\special_functions\fpclassify.hpp> // isnan
#include "VolatilitySurfaces.h"
#include <sjd\sjdTest\TestingParameters.h>
#include <ql\time\calendar.hpp>
#include <ql\time\calendars\all.hpp>
#include <ql\time\daycounter.hpp>
#include <ql\time\daycounters\all.hpp>

#include <sjd\RateSource\GenerateRatesEnvironments.h>


class VolatilitySurfacesTest 
{
  public:
    static void testEuropeanMoneynessConstruction();
    static void testEuropeanMoneynessConsistencyBetweenVolAndSD(); 
    static void testEuropeanMoneynessRoll();

    static void testCalendarMonthMoneynessConstruction();
    static void testCalendarMonthMoneynessConsistencyBetweenVolAndSD(); 
    static void testCalendarMonthMoneynessRoll();
      
    static void testForwardDeltaVolatilitySurfaceConstruction();

    static boost::unit_test_framework::test_suite* suite();

};

#endif

#include <boost\test\unit_test.hpp>
#include <boost\timer.hpp>

#include <iostream>
#include <iomanip>

#include <sjd\maths\MathsTest.h>
#include <sjd\maths\TwoDimensionalInterpolationTest.h>

#include <sjd\OptionPricing\Black76FormulaTest.h>
#include <sjd\OptionPricing\AsianOptionFormulaTest.h>

#include <sjd\RateSource\Discount\InterestRateConventionTest.h>
#include <sjd\RateSource\Discount\DiscountCurvesTest.h>
#include <sjd\RateSource\Forward\ForwardCurveTest.h>
#include <sjd\RateSource\Forward\CalendarMonthAverageTest.h>
#include <sjd\RateSource\Forward\ForwardCurveFXTest.h>
#include <sjd\RateSource\Fixing\FixingRatesTest.h>
#include <sjd\RateSource\Volatility\VolatilityCurveTest.h>
#include <sjd\RateSource\Volatility\VolatilitySurfacesTest.h>

#include <sjd\Contracts\CashflowTest.h>
#include <sjd\Contracts\ForwardContractTest.h>
#include <sjd\Contracts\OptionContractTest.h>
#include <sjd\Contracts\AsianOptionTest.h>
#include <sjd\Contracts\Portfolio\Structures\StructureTest.h>
#include <sjd\Contracts\Portfolio\PortfolioTest.h>
#include <sjd\Contracts\Portfolio\PortfolioEvolverTest.h>

#include <sjd\Contracts\Portfolio\margin\MarginTest.h>
#include <sjd\Contracts\Portfolio\CVA\CVATest.h>
#include <sjd\Contracts\Portfolio\Insurance\PortfolioInsuranceTest.h>


#include <ql\auto_link.hpp>
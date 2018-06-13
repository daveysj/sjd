#include "InterestRateConventionTest.h"

using namespace std;
using namespace boost::unit_test_framework;
using namespace sjd;


void InterestRateConventionTest::testDFToRateConversion()
{
    BOOST_TEST_MESSAGE("Convert between rates in different convention and discount factors ...");

    boost::shared_ptr<DayCounter> dc = boost::shared_ptr<DayCounter>(new Actual365Fixed());
    Date fromDate = Date(27,Jan,2012);
    Date toDate = Date(16,Jul,2014);
    double r = 0.085;

    boost::shared_ptr<InterestRateConvention> convention = boost::shared_ptr<InterestRateConvention>
        (new InterestRateConvention(dc, Compounded, Quarterly));
    double df = convention->rateToDF(r, fromDate, toDate);
    double manualDF = pow((1 + r / Quarterly), - Quarterly * dc->yearFraction(fromDate, toDate));
    BOOST_CHECK(abs(df - manualDF) < EPSILON_FOR_DOUBLES); 
    double r2 = convention->dfToRate(df, fromDate, toDate);
    BOOST_CHECK(abs(r - r2) < EPSILON_FOR_DOUBLES); 

    convention = convention = boost::shared_ptr<InterestRateConvention>
        (new InterestRateConvention(dc, Continuous, Quarterly));
    df = convention->rateToDF(r, fromDate, toDate);
    manualDF = exp(- r * dc->yearFraction(fromDate, toDate));
    BOOST_CHECK(abs(df - manualDF) < EPSILON_FOR_DOUBLES); 
    r2 = convention->dfToRate(df, fromDate, toDate);
    BOOST_CHECK(abs(r - r2) < EPSILON_FOR_DOUBLES); 

    convention = convention = boost::shared_ptr<InterestRateConvention>
        (new InterestRateConvention(dc, Simple, Quarterly));
    df = convention->rateToDF(r, fromDate, toDate);
    manualDF = 1.0 / (1 + r * dc->yearFraction(fromDate, toDate));
    BOOST_CHECK(abs(df - manualDF) < EPSILON_FOR_DOUBLES); 
    r2 = convention->dfToRate(df, fromDate, toDate);
    BOOST_CHECK(abs(r - r2) < EPSILON_FOR_DOUBLES); 
}

test_suite* InterestRateConventionTest::suite() 
{
    test_suite* suite = BOOST_TEST_SUITE("Interest Rate Convention");

    suite->add(BOOST_TEST_CASE(&InterestRateConventionTest::testDFToRateConversion));
    return suite;
}


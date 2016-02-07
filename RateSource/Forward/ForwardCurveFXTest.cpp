#include "ForwardCurveFXTest.h"

using namespace std;
using namespace boost::unit_test_framework;
using namespace sjd;

void ForwardCurveFXTest::getDefaultDatesAndDF(Date anchorDate, 
                                              boost::shared_ptr<DiscountCurve> &domesticDRS, 
                                              boost::shared_ptr<DiscountCurve> &foreignDRS,
                                              boost::shared_ptr<VolatilityCurve> &vrs)
{
    vector<Date> domesticObservationDates, foreignObservationDates;
    vector<double> domesticDF, foreignDF;
    Calendar calendar = NullCalendar();
    Date date;
    size_t i;
    for (i = 0; i <= 12; ++i) 
    {
        date = calendar.advance(anchorDate, i, Months);
        domesticObservationDates.push_back(date);
        foreignObservationDates.push_back(date);
        double daysAhead = date - anchorDate;
        domesticDF.push_back(exp(-0.06 * daysAhead / 365.0));
        foreignDF.push_back(exp(-0.03 * daysAhead / 360.0));
    }
    ++i;
    date = calendar.advance(anchorDate, i, Months);
    foreignObservationDates.push_back(date);
    double daysAhead = date - anchorDate;
    foreignDF.push_back(exp(-0.03 * daysAhead / 360.0));

    domesticDRS = boost::shared_ptr<DiscountCurve>(
        new DiscountCurve(anchorDate, domesticObservationDates, domesticDF, LINEAR, false));
    foreignDRS = boost::shared_ptr<DiscountCurve>(
        new DiscountCurve(anchorDate, foreignObservationDates, foreignDF, LINEAR, false));
    
    vector<double> volatility = vector<double>(domesticObservationDates.size(), 0.2);
    boost::shared_ptr<DayCounter> dayCounter = boost::shared_ptr<DayCounter>(new Actual365Fixed());
    vrs = boost::shared_ptr<VolatilityCurve>(
        new VolatilityCurve(anchorDate, domesticObservationDates, volatility, dayCounter, LINEAR, false));   
}

void ForwardCurveFXTest::testForwardCurveConstruction() 
{
    BOOST_MESSAGE("Testing Construction of ForwardCurveFX, ForwardCurveStochasticFX and ForwardCurveDeterministicFX ...");
    Date anchorDate = Date(27,Jan,2012);
    Calendar calendar = NullCalendar();
    boost::shared_ptr<DiscountCurve> domesticDRS; 
    boost::shared_ptr<DiscountCurve> foreignDRS; 
    boost::shared_ptr<VolatilityCurve> vrs;
    getDefaultDatesAndDF(anchorDate, domesticDRS, foreignDRS, vrs);

    double spot = 1100.0;
    Date spotDate = calendar.advance(anchorDate, 2, Days);
    boost::shared_ptr<ForwardCurveFX> fxCurve = boost::shared_ptr<ForwardCurveFX>(
        new ForwardCurveFX(anchorDate, spot, spotDate, domesticDRS, foreignDRS));
    BOOST_REQUIRE(fxCurve->isOK());
    double manualSpotAtT0 = spot * 
                            domesticDRS->getDiscountFactor(spotDate) / 
                            foreignDRS->getDiscountFactor(spotDate);
    BOOST_CHECK(abs(fxCurve->getSpotAtT0() - manualSpotAtT0) < EPSILON_FOR_DOUBLES);
    Date fwdDate = calendar.advance(anchorDate, 173, Days);
    double manualFwd = manualSpotAtT0 * 
                       foreignDRS->getDiscountFactor(fwdDate) / 
                       domesticDRS->getDiscountFactor(fwdDate);
    BOOST_CHECK(abs(fxCurve->getForward(fwdDate) - manualFwd) < 10 * EPSILON_FOR_DOUBLES);


    boost::shared_ptr<ForwardCurveStochasticFX> stochasticFXCurve = boost::shared_ptr<ForwardCurveStochasticFX>(
        new ForwardCurveStochasticFX(anchorDate, spot, spotDate, domesticDRS, foreignDRS, vrs));
    BOOST_REQUIRE(stochasticFXCurve->isOK());
    BOOST_CHECK(abs(fxCurve->getSpotAtT0() - stochasticFXCurve->getSpotAtT0()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(stochasticFXCurve->getForward(fwdDate) - manualFwd) < EPSILON_FOR_DOUBLES);

    boost::shared_ptr<ForwardCurveDeterministicFX> fxCurveDeterminstic = boost::shared_ptr<ForwardCurveDeterministicFX>(
        new ForwardCurveDeterministicFX(anchorDate, spot, spotDate, domesticDRS, foreignDRS));
    BOOST_REQUIRE(fxCurveDeterminstic->isOK());
    BOOST_CHECK(abs(fxCurve->getSpotAtT0() - fxCurveDeterminstic->getSpotAtT0()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(fxCurveDeterminstic->getForward(fwdDate) - manualFwd) < EPSILON_FOR_DOUBLES);
}

void ForwardCurveFXTest::testDeterministicCurve()
{
    BOOST_MESSAGE("Testing rolling and bumping of ForwardCurveDeterministicFX ...");
    Date anchorDate = Date(27,Jan,2012);
    Calendar calendar = NullCalendar();
    boost::shared_ptr<DiscountCurve> domesticDRS; 
    boost::shared_ptr<DiscountCurve> foreignDRS; 
    boost::shared_ptr<VolatilityCurve> vrs;
    getDefaultDatesAndDF(anchorDate, domesticDRS, foreignDRS, vrs);

    double spot = 1100.0;
    Date spotDate = calendar.advance(anchorDate, 2, Days);
    boost::shared_ptr<ForwardCurveDeterministicFX> fxCurveDeterminstic = boost::shared_ptr<ForwardCurveDeterministicFX>(
        new ForwardCurveDeterministicFX(anchorDate, spot, domesticDRS, foreignDRS));

    double bump = 1.0;
    boost::shared_ptr<ForwardRateSource> bumpedCurve = fxCurveDeterminstic->parallelBump(bump);
    BOOST_REQUIRE(bumpedCurve->isOK());
    BOOST_CHECK(abs(bumpedCurve->getForward(anchorDate) - (fxCurveDeterminstic->getSpotAtT0() + bump)) < EPSILON_FOR_DOUBLES * 100);

    Date fwdDate = calendar.advance(anchorDate, 173, Days);
    double manualFwd = fxCurveDeterminstic->getForward(fwdDate);
    boost::shared_ptr<ForwardRateSource> rolledCurve = fxCurveDeterminstic->rollForward(fwdDate);
    BOOST_REQUIRE(rolledCurve->isOK());
    BOOST_CHECK(abs(rolledCurve->getForward(fwdDate) - manualFwd) < EPSILON_FOR_DOUBLES * 100);

}

void ForwardCurveFXTest::testStochasticCurve()
{
    BOOST_MESSAGE("Testing rolling and bumping of ForwardCurveStochasticFX ...");
    Date anchorDate = Date(27,Jan,2012);
    Calendar calendar = NullCalendar();
    boost::shared_ptr<DiscountCurve> domesticDRS; 
    boost::shared_ptr<DiscountCurve> foreignDRS; 
    boost::shared_ptr<VolatilityCurve> vrs;
    getDefaultDatesAndDF(anchorDate, domesticDRS, foreignDRS, vrs);

    double spot = 1100.0;
    Date spotDate = calendar.advance(anchorDate, 2, Days);
    boost::shared_ptr<ForwardCurveStochasticFX> stochasticFXCurve = boost::shared_ptr<ForwardCurveStochasticFX>(
        new ForwardCurveStochasticFX(anchorDate, spot, spotDate, domesticDRS, foreignDRS, vrs));

    Date fwdDate = calendar.advance(anchorDate, 173, Days);
    double normalRV = 1.64;
    boost::shared_ptr<ForwardRateSource> rolledCurve = stochasticFXCurve->rollForward(fwdDate, normalRV);
    BOOST_REQUIRE(rolledCurve->isOK());
    double manualFwd = stochasticFXCurve->getForward(fwdDate);
    double sd = vrs->getStandardDeviation(fwdDate);
    manualFwd *= exp(-sd * sd / 2.0 + normalRV * sd);
    BOOST_CHECK(abs(rolledCurve->getForward(fwdDate) - manualFwd) < EPSILON_FOR_DOUBLES * 100);

}

test_suite* ForwardCurveFXTest::suite() 
{
    test_suite* suite = BOOST_TEST_SUITE("FX Forward Curves");

    suite->add(BOOST_TEST_CASE(&ForwardCurveFXTest::testForwardCurveConstruction));      
    suite->add(BOOST_TEST_CASE(&ForwardCurveFXTest::testDeterministicCurve));
    suite->add(BOOST_TEST_CASE(&ForwardCurveFXTest::testStochasticCurve));
    
    return suite;
}


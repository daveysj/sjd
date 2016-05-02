#include "VolatilitySurfacesTest.h"

#include <boost/assign/std/vector.hpp>
using namespace boost::assign; // used to initialize vector

using namespace std;
using namespace boost::unit_test_framework;
using namespace sjd;


void VolatilitySurfacesTest::testEuropeanMoneynessConstruction()
{
    BOOST_TEST_MESSAGE("Testing VolatilitySurfaceMoneyness Construction ...");
    // initialise with nulls 
    Date anchorDate = Date(27, January, 2012); 
    boost::shared_ptr<RatesEnvironment> re;
    GenerateRates gr = GenerateRates();

    vector<Date> observationDates;
    vector<double> moneyness;
    Matrix volatility;

    boost::shared_ptr<DayCounter> dayCounter = gr.defaultDayCounter;
    boost::shared_ptr<Calendar> nullCalendar = gr.defaultCalendar;
    boost::shared_ptr<ForwardRateSource> frs = gr.getSimpleForwardCurve(dayCounter, 
                                                                        nullCalendar, 
                                                                        anchorDate, 
                                                                        10, 
                                                                        100, 
                                                                        .01, 
                                                                        vector<Date>(0));

    boost::shared_ptr<VolatilitySurfaceMoneyness> volSurface = 
        boost::shared_ptr<VolatilitySurfaceMoneyness>(
            new VolatilitySurfaceMoneyness(anchorDate, 
                                           observationDates, 
                                           moneyness, 
                                           volatility, 
                                           BICUBIC, 
                                           false));
    BOOST_CHECK(volSurface->isOK() == false); 

    // initialise with inconsistent dimensions 
    anchorDate = Date(27, January, 2012); 
    Calendar calendar = SouthAfrica();
    for (size_t i = 0; i < 12; ++i) 
    {
        observationDates.push_back(calendar.advance(anchorDate, i, Months));
    }
    for (size_t i = 0; i < 5; ++i)
    {
        moneyness.push_back(-0.5 + i * 0.25);
    }
    volatility = Matrix(moneyness.size(), observationDates.size()+1, 10);


    volSurface = 
        boost::shared_ptr<VolatilitySurfaceMoneyness>(
            new VolatilitySurfaceMoneyness(anchorDate, 
                                           observationDates, 
                                           moneyness, 
                                           volatility, 
                                           BICUBIC, 
                                           false));
    BOOST_CHECK(volSurface->isOK() == false);

    Date lastObservationDate = observationDates.back();
    Date laterDateInSameMonth = calendar.endOfMonth(lastObservationDate);

    anchorDate = Date(27, January, 2012); 
    observationDates.clear();
    for (size_t i = 0; i < 12; ++i) 
    {
        observationDates.push_back(calendar.advance(anchorDate, i, Months));
    }
    moneyness.clear();
    for (size_t i = 0; i < 5; ++i)
    {
        moneyness.push_back(-0.5 + i * 0.25);
    }
    volatility = Matrix(moneyness.size(), observationDates.size(), 10);
    volSurface = 
        boost::shared_ptr<VolatilitySurfaceMoneyness>(
            new VolatilitySurfaceMoneyness(anchorDate, 
                                           observationDates, 
                                           moneyness, 
                                           volatility, 
                                           BICUBIC, 
                                           false));
    BOOST_CHECK(volSurface->isOK() == true);
}

void VolatilitySurfacesTest::testEuropeanMoneynessConsistencyBetweenVolAndSD()
{    
    BOOST_TEST_MESSAGE("Testing VolatilitySurfaceMoneyness for consistency between stdev and vol ...");
    GenerateRates re = GenerateRates();
    boost::shared_ptr<Calendar> calendar = re.defaultCalendar;
    Date anchorDate = re.defaultAnchorDate;
    // double flatVol = re.defaultVolatiltiy;

    boost::shared_ptr<VolatilitySurfaceMoneyness> volSurface = re.getDefaultMoneynessSurface(anchorDate);

    Date d = calendar->advance(anchorDate, 3, Months);
    double vol = volSurface->getVolatility(d, 1, 1); 
    BOOST_CHECK(abs(vol - .205) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(vol - volSurface->getVolatility(d)) < EPSILON_FOR_DOUBLES);    


    DayCounter dc = Actual365Fixed();
    double yf = dc.yearFraction(anchorDate, d);
    double sd = volSurface->getStandardDeviation(d, 1.0, 1.0);
    BOOST_CHECK(abs(sd - vol * sqrt(yf)) < EPSILON_FOR_DOUBLES);    

    double moneynessAsDouble = 0.5;
    double fwd = 1.0;
    double impliedStrike = moneynessAsDouble * fwd + fwd;
    vol = volSurface->getVolatility(d, impliedStrike, fwd); 
    BOOST_CHECK(abs(vol - 0.223725) < EPSILON_FOR_DOUBLES);    
    sd = volSurface->getStandardDeviation(d, impliedStrike, fwd);
    BOOST_CHECK(abs(sd - vol * sqrt(yf)) < EPSILON_FOR_DOUBLES);   

}

void VolatilitySurfacesTest::testEuropeanMoneynessRoll()
{
    BOOST_TEST_MESSAGE("Testing VolatilitySurfaceMoneyness roll ...");

    GenerateRates re = GenerateRates();
    boost::shared_ptr<Calendar> calendar = re.defaultCalendar;
    Date anchorDate = re.defaultAnchorDate;

    boost::shared_ptr<VolatilitySurfaceMoneyness> volSurface = re.getDefaultMoneynessSurface(anchorDate);

    Date d1 = calendar->advance(anchorDate, 3, Months);
    Date d2 = calendar->advance(anchorDate, 6, Months);
    double fwd = 1.0;

    boost::shared_ptr<VolatilityRateSource> rolledSurface = volSurface->rollForward(d1);
    BOOST_REQUIRE(rolledSurface->isOK());

    double originalVol = volSurface->getVolatility(d2, fwd, fwd); 
    double rolledVol = rolledSurface->getVolatility(d2, fwd, fwd); 
    BOOST_CHECK(abs(rolledVol - originalVol) < EPSILON_FOR_DOUBLES);

    originalVol = volSurface->getVolatility(d2, fwd * 1.01, fwd); 
    rolledVol = rolledSurface->getVolatility(d2, fwd * 1.01, fwd); 
    BOOST_CHECK(abs(rolledVol - originalVol) < EPSILON_FOR_DOUBLES);


    SurfaceInterpolatorType interpolatorType = BILINEAR;
    vector<Date> surfaceDates = boost::assign::list_of(calendar->advance(anchorDate, 1, Years))
                                                      (calendar->advance(anchorDate, 2, Years))
                                                      (calendar->advance(anchorDate, 3, Years))
                                                      (calendar->advance(anchorDate, 4, Years))
                                                      (calendar->advance(anchorDate, 5, Years));
    vector<double> moneyness;
    moneyness += -.5, 0, .5;

    vector<double> v1, v2, v3;
    v1 += .17575,   .17575,     .18247,     .206225,    .234775;
    v2 += .175,     .175,       .18,        .205,       .235;
    v3 += .18825,   .18825,     .19547,     .223725,    .223725;

    vector<vector<double> > surfaceData; 
    surfaceData += v1, v2, v3;

    boost::shared_ptr<VolatilitySurfaceMoneyness> baseCurve = boost::shared_ptr<VolatilitySurfaceMoneyness>(
                                                        new VolatilitySurfaceMoneyness(anchorDate,
                                                                                       surfaceDates, 
                                                                                       moneyness, 
                                                                                       surfaceData, 
                                                                                       interpolatorType, 
                                                                                       false));
    boost::shared_ptr<VolatilityRateSource> rolledCurve = baseCurve->rollForward(surfaceDates[1]);
    BOOST_CHECK(rolledCurve->isOK());
    BOOST_CHECK(!rolledCurve->isInRange(anchorDate));

    surfaceDates.erase(surfaceDates.begin());
    v1.erase(v1.begin());
    v2.erase(v2.begin());
    v3.erase(v3.begin());
    surfaceData.clear();
    surfaceData += v1, v2, v3;
    boost::shared_ptr<VolatilitySurfaceMoneyness> manualCurve = boost::shared_ptr<VolatilitySurfaceMoneyness>(
                                                            new VolatilitySurfaceMoneyness(surfaceDates[0],
                                                                                  surfaceDates, 
                                                                                  moneyness, 
                                                                                  surfaceData, 
                                                                                  interpolatorType, 
                                                                                  false));

    BOOST_CHECK(abs(rolledCurve->getVolatility(surfaceDates[1],1,1) -  
                    manualCurve->getVolatility(surfaceDates[1],1,1)) < EPSILON_FOR_DOUBLES);
    Date anotherDate = calendar->advance(surfaceDates[0], 18, Months);
    BOOST_CHECK(abs(rolledCurve->getVolatility(anotherDate,1,1) -  
                    manualCurve->getVolatility(anotherDate,1,1)) < EPSILON_FOR_DOUBLES);
    anotherDate = calendar->advance(surfaceDates[0], 20, Months);
    BOOST_CHECK(abs(rolledCurve->getVolatility(anotherDate,.75,1) -  
                    manualCurve->getVolatility(anotherDate,.75,1)) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(rolledCurve->getVolatility(anotherDate,1.25,1) -  
                    manualCurve->getVolatility(anotherDate,1.25,1)) < EPSILON_FOR_DOUBLES);
}


//////////////////////////////////////////////////////////////////////////////////////////////////



void VolatilitySurfacesTest::testCalendarMonthMoneynessConstruction()
{
    BOOST_TEST_MESSAGE("Testing VolatilitySurfaceMoneynessCMA Construction ...");
    // initialise with nulls 
    Date anchorDate = Date(27, January, 2012); 
    boost::shared_ptr<RatesEnvironment> re;
    GenerateRates gr = GenerateRates();

    vector<Date> observationDates;
    vector<double> moneyness;
    Matrix volatility;

    boost::shared_ptr<DayCounter> dayCounter = gr.defaultDayCounter;
    boost::shared_ptr<Calendar> nullCalendar = gr.defaultCalendar;
   boost::shared_ptr<ForwardRateSource> frs = gr.getSimpleForwardCurve(dayCounter, nullCalendar, anchorDate, 10, 100, .01, vector<Date>(0));

   boost::shared_ptr<VolatilitySurfaceMoneynessCMA> volSurface = 
        boost::shared_ptr<VolatilitySurfaceMoneynessCMA>(
            new VolatilitySurfaceMoneynessCMA(anchorDate, observationDates, moneyness, volatility, BICUBIC, false));
    BOOST_CHECK(volSurface->isOK() == false);

    // initialise with inconsistent dimensions 
    anchorDate = Date(27, January, 2012); 
    Calendar calendar = SouthAfrica();
    for (size_t i = 0; i < 12; ++i) 
        observationDates.push_back(calendar.advance(anchorDate, i, Months));
    for (size_t i = 0; i < 5; ++i)
        moneyness.push_back(-0.5 + i * 0.25);
    volatility = Matrix(moneyness.size(), observationDates.size()+1, 10);
    
    volSurface = 
        boost::shared_ptr<VolatilitySurfaceMoneynessCMA>(
            new VolatilitySurfaceMoneynessCMA(anchorDate, observationDates, moneyness, volatility, BICUBIC, false));
    BOOST_CHECK(volSurface->isOK() == false);

    // Add a second date in the same observation month
    Date lastObservationDate = observationDates.back();
    Date laterDateInSameMonth = calendar.endOfMonth(lastObservationDate);
    observationDates.push_back(laterDateInSameMonth);
    volatility = Matrix(moneyness.size(), observationDates.size(), 10);
    volSurface = 
        boost::shared_ptr<VolatilitySurfaceMoneynessCMA>(
            new VolatilitySurfaceMoneynessCMA(anchorDate, observationDates, moneyness, volatility, BICUBIC, false));
    BOOST_CHECK(volSurface->isOK() == false);
    observationDates.pop_back();
    volatility = Matrix(moneyness.size(), observationDates.size(), 10);
    volSurface = 
        boost::shared_ptr<VolatilitySurfaceMoneynessCMA>(
            new VolatilitySurfaceMoneynessCMA(anchorDate, observationDates, moneyness, volatility, BICUBIC, false));
    BOOST_CHECK(volSurface->isOK() == true);

    // set anchor date after the last ovservation date
    volSurface = 
        boost::shared_ptr<VolatilitySurfaceMoneynessCMA>(
            new VolatilitySurfaceMoneynessCMA(laterDateInSameMonth, observationDates, moneyness, volatility, BICUBIC, false));
    BOOST_CHECK(volSurface->isOK() == false);


    anchorDate = Date(27, January, 2012); 
    observationDates.clear();
    for (size_t i = 0; i < 12; ++i) 
        observationDates.push_back(calendar.advance(anchorDate, i, Months));
    moneyness.clear();
    for (size_t i = 0; i < 5; ++i)
        moneyness.push_back(-0.5 + i * 0.25);    
    volatility = Matrix(moneyness.size(), observationDates.size(), 10);
    volSurface = 
        boost::shared_ptr<VolatilitySurfaceMoneynessCMA>(
            new VolatilitySurfaceMoneynessCMA(anchorDate, observationDates, moneyness, volatility, BICUBIC, false));
    BOOST_CHECK(volSurface->isOK() == true);

}

void VolatilitySurfacesTest::testCalendarMonthMoneynessConsistencyBetweenVolAndSD()
{
    BOOST_TEST_MESSAGE("Testing VolatilitySurfaceMoneynessCMA for consistency between stdev and vol ...");

    GenerateRates re = GenerateRates();
    boost::shared_ptr<Calendar> calendar = re.defaultCalendar;
    Date anchorDate = re.defaultAnchorDate;

    boost::shared_ptr<VolatilitySurfaceMoneynessCMA> volSurface = re.getDefaultCalendarMonthAverageMoneynessSurface(anchorDate);

    Date d = calendar->advance(anchorDate, 3, Months);
   double fwd = 1.0;

    double vol = volSurface->getVolatility(d, fwd, fwd); 
    BOOST_CHECK(abs(vol - .205) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(vol - volSurface->getVolatility(d)) < EPSILON_FOR_DOUBLES);    

    DayCounter dc = Actual365Fixed();
    double yf = dc.yearFraction(anchorDate, d);
    double sd = volSurface->getStandardDeviation(d, fwd, fwd);
    BOOST_CHECK(abs(sd - vol * sqrt(yf)) < EPSILON_FOR_DOUBLES);    

    double moneynessAsDouble = 0.5;
   double impliedStrike = moneynessAsDouble * fwd + fwd;

    vol = volSurface->getVolatility(d, impliedStrike, fwd); 
    BOOST_CHECK(abs(vol - 0.223725) < EPSILON_FOR_DOUBLES);    
    sd = volSurface->getStandardDeviation(d, impliedStrike, fwd);
    BOOST_CHECK(abs(sd - vol * sqrt(yf)) < EPSILON_FOR_DOUBLES);   

}

void VolatilitySurfacesTest::testCalendarMonthMoneynessRoll()
{
    BOOST_TEST_MESSAGE("Testing VolatilitySurfaceMoneynessCMA roll ...");

    GenerateRates re = GenerateRates();
    boost::shared_ptr<Calendar> calendar = re.defaultCalendar;
    Date anchorDate = re.defaultAnchorDate;

    boost::shared_ptr<VolatilitySurfaceMoneynessCMA> volSurface = re.getDefaultCalendarMonthAverageMoneynessSurface(anchorDate);

    Date d1 = calendar->advance(anchorDate, 3, Months);
    Date d2 = calendar->advance(anchorDate, 18, Months);
   double fwd = 1.0;

    boost::shared_ptr<VolatilityRateSource> rolledSurface = volSurface->rollForward(d1);
    BOOST_REQUIRE(rolledSurface->isOK());

    double originalVol = volSurface->getVolatility(d2, fwd, fwd); 
    double rolledVol = rolledSurface->getVolatility(d2, fwd, fwd); 

    BOOST_CHECK(abs(rolledVol - originalVol) < EPSILON_FOR_DOUBLES);

    originalVol = volSurface->getVolatility(d2, fwd * 1.01, fwd); 
    rolledVol = rolledSurface->getVolatility(d2, fwd * 1.01, fwd); 

    BOOST_CHECK(abs(rolledVol - originalVol) < EPSILON_FOR_DOUBLES);

    SurfaceInterpolatorType interpolatorType = BILINEAR;
    vector<Date> surfaceDates = boost::assign::list_of(calendar->advance(anchorDate, 1, Years))
                                                      (calendar->advance(anchorDate, 2, Years))
                                                      (calendar->advance(anchorDate, 3, Years))
                                                      (calendar->advance(anchorDate, 4, Years))
                                                      (calendar->advance(anchorDate, 5, Years));
    vector<double> moneyness;
    moneyness += -.5, 0, .5;

    vector<double> v1, v2, v3;
    v1 += .17575,   .17575,     .18247,     .206225,    .234775;
    v2 += .175,     .175,       .18,        .205,       .235;
    v3 += .18825,   .18825,     .19547,     .223725,    .223725;

    vector<vector<double> > surfaceData; 
    surfaceData += v1, v2, v3;

    boost::shared_ptr<VolatilitySurfaceMoneynessCMA> baseCurve = boost::shared_ptr<VolatilitySurfaceMoneynessCMA>(
                                                            new VolatilitySurfaceMoneynessCMA(anchorDate,
                                                                              surfaceDates, 
                                                                              moneyness, 
                                                                              surfaceData, 
                                                                              interpolatorType, 
                                                                              false));
    boost::shared_ptr<VolatilityRateSource> rolledCurve = baseCurve->rollForward(surfaceDates[1]);
    BOOST_CHECK(rolledCurve->isOK());
    BOOST_CHECK(!rolledCurve->isInRange(anchorDate));

    surfaceDates.erase(surfaceDates.begin());
    v1.erase(v1.begin());
    v2.erase(v2.begin());
    v3.erase(v3.begin());
    surfaceData.clear();
    surfaceData += v1, v2, v3;
    boost::shared_ptr<VolatilitySurfaceMoneynessCMA> manualCurve = boost::shared_ptr<VolatilitySurfaceMoneynessCMA>(
                                                            new VolatilitySurfaceMoneynessCMA(surfaceDates[0],
                                                                                  surfaceDates, 
                                                                                  moneyness, 
                                                                                  surfaceData, 
                                                                                  interpolatorType, 
                                                                                  false));

    BOOST_CHECK(abs(rolledCurve->getVolatility(surfaceDates[1],1,1) -  
                    manualCurve->getVolatility(surfaceDates[1],1,1)) < EPSILON_FOR_DOUBLES);
    Date anotherDate = calendar->advance(surfaceDates[0], 18, Months);
    BOOST_CHECK(abs(rolledCurve->getVolatility(anotherDate,1,1) -  
                    manualCurve->getVolatility(anotherDate,1,1)) < EPSILON_FOR_DOUBLES);
    anotherDate = calendar->advance(surfaceDates[0], 20, Months);
    BOOST_CHECK(abs(rolledCurve->getVolatility(anotherDate,.75,1) -  
                    manualCurve->getVolatility(anotherDate,.75,1)) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(rolledCurve->getVolatility(anotherDate,1.25,1) -  
                    manualCurve->getVolatility(anotherDate,1.25,1)) < EPSILON_FOR_DOUBLES);


}

//////////////////////////////////////////////////////////////////////////////////////////////////

void VolatilitySurfacesTest::testForwardDeltaVolatilitySurfaceConstruction() 
{
    BOOST_TEST_MESSAGE("Testing VolatilitySurfaceDelta Construction ...");

    Date anchorDate = Date(27,Jan,2012);
    boost::shared_ptr<RatesEnvironment> re;
    GenerateRates gr = GenerateRates();
    boost::shared_ptr<Calendar> calendar = gr.defaultCalendar;


    vector<Date> observationDates;
    observationDates.push_back(calendar->advance(anchorDate, 1, Months));
    observationDates.push_back(calendar->advance(anchorDate, 2, Months));
    observationDates.push_back(calendar->advance(anchorDate, 3, Months));
    observationDates.push_back(calendar->advance(anchorDate, 6, Months));
    observationDates.push_back(calendar->advance(anchorDate, 1, Years));
    observationDates.push_back(calendar->advance(anchorDate, 2, Years));

    vector<double> delta;
    delta += 10, 25, 50, 75, 90;

    vector<double> v1, v2, v3, v4, v5;
    v1 += .17938,   .182884,    .193908,    .219688,    .248396,    .263268; // 10 Delta put
    v2 += .17575,   .17575,     .18247,     .206225,    .234775,    .2475;
    v3 += .175,     .175,       .18,        .205,       .235,       .2475;
    v4 += .18825,   .18825,     .19547,     .223725,    .223725,    .2725;
    v5 += .20128,   .204784,    .216708,    .250288,    .287796,    .307068; // 90 Delta put


    vector<vector<double>> volatility;
    volatility += v1, v2, v3, v4, v5;

    boost::shared_ptr<DayCounter> dayCounter = boost::shared_ptr<DayCounter>(new Actual365Fixed());
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());    
   boost::shared_ptr<ForwardRateSource> frs = gr.getSimpleForwardCurve(dayCounter, nullCalendar, anchorDate, 10, 100, .01, vector<Date>(0));

   VolatilitySurfaceDelta vs(anchorDate, observationDates, delta, volatility, BICUBIC, false);
    BOOST_REQUIRE(vs.isOK()); 

    Date d1 = calendar->advance(anchorDate, 1, Years);
   double fwd = frs->getForward(d1);
   double strike1 = vs.calculateStrikeFromDelta(d1, 50, fwd); 
    BOOST_REQUIRE(vs.isInRange(d1, strike1, fwd)); 
    BOOST_CHECK(abs(vs.getVolatility(d1, strike1, fwd) - 0.235) < EPSILON_FOR_DOUBLES);
   double strike2 = vs.calculateStrikeFromDelta(d1, 10, fwd);
    BOOST_REQUIRE(vs.isInRange(d1, strike2, fwd)); 
    BOOST_CHECK(abs(vs.getVolatility(d1, strike2, fwd) - 0.248396) < 1e-6);


    dayCounter = boost::shared_ptr<DayCounter>(new Actual365Fixed());
    nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());
    Date tPlus2 = nullCalendar->advance(anchorDate, 2, Days);
    double spot = 100, naccRFR = 0.065, naccIRDiff = 0.045, vol = 0.2;    

    BOOST_CHECK(abs(vs.getVolatility(d1) - vs.getVolatility(d1,frs->getForward(d1), frs->getForward(d1))) < EPSILON_FOR_DOUBLES);

}

test_suite* VolatilitySurfacesTest::suite() 
{
    test_suite* suite = BOOST_TEST_SUITE("Volatility Surfaces");

    suite->add(BOOST_TEST_CASE(&VolatilitySurfacesTest::testEuropeanMoneynessConstruction));
    suite->add(BOOST_TEST_CASE(&VolatilitySurfacesTest::testEuropeanMoneynessConsistencyBetweenVolAndSD));
    suite->add(BOOST_TEST_CASE(&VolatilitySurfacesTest::testEuropeanMoneynessRoll));

    suite->add(BOOST_TEST_CASE(&VolatilitySurfacesTest::testCalendarMonthMoneynessConstruction));
    suite->add(BOOST_TEST_CASE(&VolatilitySurfacesTest::testCalendarMonthMoneynessConsistencyBetweenVolAndSD));
    suite->add(BOOST_TEST_CASE(&VolatilitySurfacesTest::testCalendarMonthMoneynessRoll));
        
    suite->add(BOOST_TEST_CASE(&VolatilitySurfacesTest::testForwardDeltaVolatilitySurfaceConstruction));
    
    return suite;
}


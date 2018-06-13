#include "ForwardCurveTest.h"

using namespace std;
using namespace boost::unit_test_framework;
using namespace sjd;


void ForwardCurveTest::testForwardCurveConstruction() 
{
    BOOST_TEST_MESSAGE("Testing Construction of ForwardCurveDeterministic ...");
    Date anchorDate = Date(27,Jan,2012);
    vector<Date> observationDates, incorrectObservationDates;
    vector<double> futures;
    size_t i;
    bool allowExtrapolation = false;

    // Case 1 NULL inputs
    boost::shared_ptr<ForwardCurveDeterministic> sfc = boost::shared_ptr<ForwardCurveDeterministic>(
                    new ForwardCurveDeterministic(anchorDate, incorrectObservationDates, futures, sjd::LINEAR, allowExtrapolation));
   BOOST_CHECK(sfc->isOK() == false); 


    // Case 2 Inputs dates not strictly monotonic
    Calendar calendar = NullCalendar();
    for (i = 1; i <= 12; ++i) 
    {
        Date date = calendar.advance(anchorDate, i, Months);
        observationDates.push_back(date);
        incorrectObservationDates.push_back(date);
        futures.push_back(110 - (double) i / 10.0);
    }
    incorrectObservationDates.push_back(anchorDate);
    futures.push_back(110);
    sfc = boost::shared_ptr<ForwardCurveDeterministic>(
        new ForwardCurveDeterministic(anchorDate, incorrectObservationDates, futures, sjd::LINEAR, allowExtrapolation));
    BOOST_CHECK(sfc->isOK() == false); 

    // Case 3 Input dates and times have different lengths
    futures.pop_back();
    incorrectObservationDates = observationDates;
    incorrectObservationDates.push_back(calendar.advance(anchorDate, i+1, Months));
    sfc = boost::shared_ptr<ForwardCurveDeterministic>(
        new ForwardCurveDeterministic(anchorDate, incorrectObservationDates, futures, sjd::LINEAR, allowExtrapolation));
    BOOST_CHECK(sfc->isOK() == false); 

    // Case 4 Anchor date > final curve date
    incorrectObservationDates = observationDates;
    anchorDate = calendar.advance(anchorDate, i+1, Months);
    sfc = boost::shared_ptr<ForwardCurveDeterministic>(
        new ForwardCurveDeterministic(anchorDate, incorrectObservationDates, futures, sjd::LINEAR, allowExtrapolation)); 
    BOOST_CHECK(sfc->isOK() == false); 

    //Case 5 We do not supply a point at the anchor date and we specify no extrapolation
    anchorDate = Date(27,Jan,2012);
    sfc = boost::shared_ptr<ForwardCurveDeterministic>(
        new ForwardCurveDeterministic(anchorDate, observationDates, futures, sjd::LINEAR, allowExtrapolation));
    BOOST_CHECK(sfc->isOK() == true); 
    BOOST_CHECK(sfc->getForward(anchorDate) == 109.9); 

    sfc = boost::shared_ptr<ForwardCurveDeterministic>(
        new ForwardCurveDeterministic(anchorDate, observationDates, futures, sjd::LINEAR, allowExtrapolation));
    BOOST_CHECK(sfc->isOK() == true); 
    Date d1 = calendar.advance(anchorDate, 5, Months);
    Date d2 = calendar.advance(anchorDate, 13, Months);
    Date d3 = calendar.advance(anchorDate, -1, Months);
    BOOST_CHECK(sfc->isInRange(d1) == true);
    BOOST_CHECK(sfc->isInRange(d2) == false);
    BOOST_CHECK(sfc->isInRange(d3) == false);
    BOOST_CHECK(sfc->isInRange(anchorDate) == true);

    allowExtrapolation = !allowExtrapolation;
    sfc = boost::shared_ptr<ForwardCurveDeterministic>(
        new ForwardCurveDeterministic(anchorDate, observationDates, futures, sjd::LINEAR, allowExtrapolation));
    BOOST_CHECK(sfc->isInRange(d1) == true);
    BOOST_CHECK(sfc->isInRange(d2) == true);
    BOOST_CHECK(sfc->isInRange(d3) == false);
}

void ForwardCurveTest::testForwardCurveRollsAtForwardVolConstruction() 
{
    BOOST_TEST_MESSAGE("Testing Construction of ForwardCurveStochasticFwdVol ...");

    GenerateRates gr = GenerateRates();
    Date anchorDate = gr.defaultAnchorDate;

    boost::shared_ptr<VolatilityCurve> vrs = gr.getVolatilityCurve(gr.defaultDayCounter,
                                                                   gr.defaultCalendar,
                                                                   anchorDate, 
                                                                   1, 
                                                                   0.2);

    vector<Date> observationDates;
    vector<double> forwards;
    size_t i;
    bool allowExtrapolation = false;

    Calendar calendar = NullCalendar();
    for (i = 1; i <= 24; ++i) 
    {
        Date date = calendar.advance(anchorDate, i, Months);
        observationDates.push_back(date);
        forwards.push_back(110 - (double) i / 10.0);
    }

    // Case 1: VRS does not extend to the end of the FRS
    boost::shared_ptr<ForwardCurveStochasticFwdVol> frs = boost::shared_ptr<ForwardCurveStochasticFwdVol>(new
                            ForwardCurveStochasticFwdVol(anchorDate, 
                                                          observationDates, 
                                                          forwards,                                       
                                                          LINEAR,
                                                          false,
                                                          vrs));

   BOOST_CHECK(frs->isOK() == false); 

    // Case 2: VRS has different anchor date
    vrs = gr.getVolatilityCurve(gr.defaultDayCounter,
                                gr.defaultCalendar,
                                calendar.advance(anchorDate, 1, Months), 
                                4, 
                                0.2);
    frs = boost::shared_ptr<ForwardCurveStochasticFwdVol>(new
                            ForwardCurveStochasticFwdVol(anchorDate, 
                                                          observationDates, 
                                                          forwards,                                       
                                                          LINEAR,
                                                          false,
                                                          vrs));
   BOOST_CHECK(frs->isOK() == false); 

    // Case 3, all ok
    vrs = gr.getVolatilityCurve(gr.defaultDayCounter,
                                gr.defaultCalendar,
                                anchorDate, 
                                4, 
                                0.2);
    frs = boost::shared_ptr<ForwardCurveStochasticFwdVol>(new
                            ForwardCurveStochasticFwdVol(anchorDate, 
                                                          observationDates, 
                                                          forwards,                                       
                                                          LINEAR,
                                                          false,
                                                          vrs));
   BOOST_CHECK(frs->isOK() == true); 
}


void ForwardCurveTest::testConsistencyBetweenGetMethods()
{
    BOOST_TEST_MESSAGE("Testing ForwardCurveDeterministic has consistent get methods ...");

    Calendar calendar = NullCalendar();
    Date anchorDate = Date(27,Jan,2012);
    vector<Date> observationDates;
    vector<double> futures;
    size_t i;
    for (i = 1; i <= 12; ++i) 
    {
        Date date = calendar.advance(anchorDate, i, Months);
        observationDates.push_back(date);
        futures.push_back(110 - (double) i / 10.0);
    }
    boost::shared_ptr<DayCounter> dcPtr = boost::shared_ptr<DayCounter>(new Actual365Fixed()); 
    bool allowExtrapolation = false;    
    ForwardCurveDeterministic *fcd = new ForwardCurveDeterministic(anchorDate, observationDates, futures, sjd::LINEAR, allowExtrapolation);

    Month month = October;
    Year year = 2012;
    Day day = 18;
    Date date1 = Date(day, month, year);
    double fwd1 = fcd->getForward(date1);

    vector<Date> dateVector;
    dateVector.push_back(date1);
    Date date2 = Date(day,November,year);
    dateVector.push_back(date2);
    vector<double> rates = fcd->getForward(dateVector);
    for (size_t i = 0; i < dateVector.size(); ++i) 
    {
        BOOST_CHECK(abs(rates[i]-fcd->getForward(dateVector[i])) < EPSILON_FOR_DOUBLES);
    }

    BOOST_CHECK(abs(fcd->getForwardFworwardAdditive(date1, date2) - 
                   (fcd->getForward(date2) - fcd->getForward(date1))) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(fcd->getForwardFworwardMultiplicative(date1, date2) - 
                   (fcd->getForward(date2) / fcd->getForward(date1))) < EPSILON_FOR_DOUBLES);

    Date date3 = Date(day,December,year);
    dateVector.clear();
    dateVector.push_back(date2);
    dateVector.push_back(date3);
    rates.clear();
    rates = fcd->getForwardFworwardAdditive(date1, dateVector);
    for (size_t i = 0; i < rates.size(); ++i) 
    {
        BOOST_CHECK(abs(rates[i]-fcd->getForwardFworwardAdditive(date1, dateVector[i])) < EPSILON_FOR_DOUBLES);
    }
    rates.clear();
    rates = fcd->getForwardFworwardMultiplicative(date1, dateVector);
    for (size_t i = 0; i < rates.size(); ++i) 
    {
        BOOST_CHECK(abs(rates[i]-fcd->getForwardFworwardMultiplicative(date1, dateVector[i])) < EPSILON_FOR_DOUBLES);
    }
    delete fcd;
}

void ForwardCurveTest::testValues()
{
    BOOST_TEST_MESSAGE("Testing ForwardCurveDeterministic values ...");

    Calendar calendar = NullCalendar();
    Date anchorDate = Date(27,Jan,2012);
    vector<Date> observationDates;
    vector<double> observationDatesSerialNumbers;
    vector<double> futures;
    //vector<double> time;
    boost::shared_ptr<DayCounter> dcPtr = boost::shared_ptr<DayCounter>(new Actual365Fixed()); 
    size_t i;
    for (i = 1; i <= 12; ++i) 
    {
        Date date = calendar.advance(anchorDate, i, Months);
        observationDates.push_back(date);
        observationDatesSerialNumbers.push_back(date.serialNumber());
        futures.push_back(110 - (double) i / 10.0);
    }
    bool allowExtrapolation = false;    
    ForwardCurveDeterministic *sfc = new ForwardCurveDeterministic(anchorDate, 
                                                     observationDates, 
                                                     futures, 
                                                     sjd::LINEAR, 
                                                     allowExtrapolation);

    LinearArrayInterpolator la(observationDatesSerialNumbers, futures, false);

    Date date1 = Date(13,Mar,2012);
    //double yf = dcPtr->yearFraction(anchorDate, date1);
    double interpolatedValue = la.getRate(date1.serialNumber());

    double fwd = sfc->getForward(date1);
    BOOST_CHECK(abs(fwd - (interpolatedValue)) < 10 * EPSILON_FOR_DOUBLES);


    delete sfc;
}

void ForwardCurveTest::testRoll()
{
    BOOST_TEST_MESSAGE("Testing ForwardCurveDeterministic roll ...");

    GenerateRates gr = GenerateRates();
    Date anchorDate = gr.defaultAnchorDate;

    vector<Date> observationDates;
    vector<double> forwards;
    size_t i;
    bool allowExtrapolation = false;

    Calendar calendar = NullCalendar();
    for (i = 1; i <= 24; ++i) 
    {
        Date date = calendar.advance(anchorDate, i, Months);
        observationDates.push_back(date);
        forwards.push_back(110 - (double) i / 10.0);
    }

    // Case 1: VRS does not extend to the end of the FRS
    boost::shared_ptr<ForwardCurveDeterministic> frs = boost::shared_ptr<ForwardCurveDeterministic>(new
                                ForwardCurveDeterministic(anchorDate, 
                                                          observationDates, 
                                                          forwards,                                       
                                                          LINEAR,
                                                          false));

    BOOST_CHECK(frs->isOK() == true); 
    
    Date rollDate = observationDates[0];
    vector<Date> intermedateDates;
    intermedateDates.push_back(calendar.advance(anchorDate, 10, Days));
    intermedateDates.push_back(calendar.advance(anchorDate, 20, Days));
    vector<double> rateSetForwards;
    boost::shared_ptr<ForwardRateSource> rolledCurve = frs->rollForward(rollDate, intermedateDates, rateSetForwards);
    BOOST_REQUIRE(rolledCurve->isOK() == true); 
    BOOST_REQUIRE(rateSetForwards.size() == 2);     
    BOOST_CHECK(abs(frs->getForward(observationDates[5]) - rolledCurve->getForward(observationDates[5])) < EPSILON_FOR_DOUBLES); 
    BOOST_CHECK(abs(frs->getForward(intermedateDates[0]) - rateSetForwards[0]) < EPSILON_FOR_DOUBLES); 
    BOOST_CHECK(abs(frs->getForward(intermedateDates[1]) - rateSetForwards[1]) < EPSILON_FOR_DOUBLES); 

    
    set<Date> forwardDates;
    forwardDates.insert(observationDates[0]);
    forwardDates.insert(observationDates[1]);
    frs->setForwardValuationDates(forwardDates); 

    intermedateDates.push_back(calendar.advance(observationDates[0], 10, Days));
    intermedateDates.push_back(calendar.advance(observationDates[0], 20, Days));
    vector<boost::shared_ptr<ForwardRateSource> > frss = frs->rollForward(intermedateDates, rateSetForwards);
    BOOST_REQUIRE(frss.size() == 2);
    BOOST_REQUIRE(frss[0]->isOK());
    BOOST_REQUIRE(frss[1]->isOK());
    BOOST_CHECK(abs(frs->getForward(observationDates[5]) - frss[0]->getForward(observationDates[5])) < EPSILON_FOR_DOUBLES); 
    BOOST_CHECK(abs(frs->getForward(observationDates[5]) - frss[1]->getForward(observationDates[5])) < EPSILON_FOR_DOUBLES); 
    BOOST_REQUIRE(rateSetForwards.size() == 4);
    BOOST_CHECK(abs(frs->getForward(intermedateDates[0]) - rateSetForwards[0]) < EPSILON_FOR_DOUBLES); 
    BOOST_CHECK(abs(frs->getForward(intermedateDates[1]) - rateSetForwards[1]) < EPSILON_FOR_DOUBLES); 
    BOOST_CHECK(abs(frs->getForward(intermedateDates[2]) - rateSetForwards[2]) < EPSILON_FOR_DOUBLES); 
    BOOST_CHECK(abs(frs->getForward(intermedateDates[3]) - rateSetForwards[3]) < EPSILON_FOR_DOUBLES); 
}


void ForwardCurveTest::testRollStochastic()
{
    BOOST_TEST_MESSAGE("Testing ForwardCurveStochasticFwdVol roll ...");

    GenerateRates gr = GenerateRates();
    Date anchorDate = gr.defaultAnchorDate;

    boost::shared_ptr<VolatilityCurve> vrs = gr.getVolatilityCurve(gr.defaultDayCounter,
                                                                   gr.defaultCalendar,
                                                                   anchorDate, 
                                                                   4, 
                                                                   0.2);

    vector<Date> observationDates;
    vector<double> forwards;
    size_t i;
    bool allowExtrapolation = false;

    Calendar calendar = NullCalendar();
    for (i = 1; i <= 24; ++i) 
    {
        Date date = calendar.advance(anchorDate, i, Months);
        observationDates.push_back(date);
        forwards.push_back(110 - (double) i / 10.0);
    }

    // Case 1: VRS does not extend to the end of the FRS
    boost::shared_ptr<ForwardCurveStochasticFwdVol> frs = boost::shared_ptr<ForwardCurveStochasticFwdVol>(new
                            ForwardCurveStochasticFwdVol(anchorDate, 
                                                          observationDates, 
                                                          forwards,                                       
                                                          LINEAR,
                                                          false,
                                                          vrs));

    vector<Date> intermedateDates;
    intermedateDates.push_back(calendar.advance(anchorDate, 10, Days));
    intermedateDates.push_back(calendar.advance(anchorDate, 20, Days));
    vector<double> rateSetForwards;

    BOOST_CHECK(frs->isOK() == true); 
    double normalRV = 0.1234;
    Date rollToDate = observationDates[0];
    boost::shared_ptr<ForwardRateSource> rolledCurve = frs->rollForward(rollToDate, normalRV, intermedateDates, rateSetForwards);
    BOOST_CHECK(rolledCurve->isOK() == true); 
    BOOST_REQUIRE(rateSetForwards.size() == 2);     

    double manualCheck = frs->getForward(observationDates[1]);
    double manualVol = vrs->getVolatility(observationDates[1]);
    double manualTime = (rollToDate - anchorDate) / 365.0;
    manualCheck *= exp(-manualVol * manualVol * manualTime / 2.0 + manualVol * sqrt(manualTime) * normalRV);
    BOOST_CHECK(abs(rolledCurve->getForward(observationDates[1]) - manualCheck) < EPSILON_FOR_DOUBLES * 10);

    double expectedFwd = frs->getForward(rollToDate);
    double actualFwd = rolledCurve->getForward(rollToDate);
    double factor = (actualFwd - expectedFwd) / expectedFwd;
    factor /= ((double) (observationDates[0] - anchorDate));
    double manualRateSet1 = frs->getForward(intermedateDates[0]) * (1 + factor * ((double) (intermedateDates[0] - anchorDate)));
    double manualRateSet2 = frs->getForward(intermedateDates[1]) * (1 + factor * ((double) (intermedateDates[1] - anchorDate)));
    BOOST_CHECK(abs(manualRateSet1 - rateSetForwards[0]) < EPSILON_FOR_DOUBLES); 
    BOOST_CHECK(abs(manualRateSet2 - rateSetForwards[1]) < EPSILON_FOR_DOUBLES); 


    set<Date> forwardDates;
    forwardDates.insert(observationDates[0]);
    forwardDates.insert(observationDates[1]);
    frs->setForwardValuationDates(forwardDates); 
    vector<double> normalRVs;
    normalRVs.push_back(normalRV);
    normalRVs.push_back(normalRV);
    frs->setRandomVariables(normalRVs);

    intermedateDates.push_back(calendar.advance(observationDates[0], 10, Days));
    intermedateDates.push_back(calendar.advance(observationDates[0], 20, Days));

    vector<boost::shared_ptr<ForwardRateSource> > frss = frs->rollForward(intermedateDates, rateSetForwards);
    BOOST_REQUIRE(frss.size() == 2);
    BOOST_REQUIRE(frss[0]->isOK());
    BOOST_REQUIRE(frss[1]->isOK());
    BOOST_REQUIRE(rateSetForwards.size() == 4);
    BOOST_CHECK(abs(frss[0]->getForward(observationDates[1]) - manualCheck) < EPSILON_FOR_DOUBLES * 10);

    BOOST_CHECK(abs(manualRateSet1 - rateSetForwards[0]) < EPSILON_FOR_DOUBLES); 
    BOOST_CHECK(abs(manualRateSet2 - rateSetForwards[1]) < EPSILON_FOR_DOUBLES); 
    rollToDate = observationDates[1];
    expectedFwd = frss[0]->getForward(rollToDate);
    actualFwd = frss[1]->getForward(rollToDate);
    factor = (actualFwd - expectedFwd) / expectedFwd;
    factor /= ((double) (observationDates[1] - observationDates[0]));
    double manualRateSet3 = frss[0]->getForward(intermedateDates[2]);
    manualRateSet3 *= (1 + factor * ((double) (intermedateDates[2] - observationDates[0])));
    double manualRateSet4 = frss[0]->getForward(intermedateDates[3]) * (1 + factor * ((double) (intermedateDates[3] - observationDates[0])));
    BOOST_CHECK(abs(manualRateSet3 - rateSetForwards[2]) < EPSILON_FOR_DOUBLES); 
    BOOST_CHECK(abs(manualRateSet4 - rateSetForwards[3]) < EPSILON_FOR_DOUBLES);  

}


test_suite* ForwardCurveTest::suite() 
{
    test_suite* suite = BOOST_TEST_SUITE("Forward Curves");

    suite->add(BOOST_TEST_CASE(&ForwardCurveTest::testForwardCurveConstruction));    
    suite->add(BOOST_TEST_CASE(&ForwardCurveTest::testConsistencyBetweenGetMethods));
    suite->add(BOOST_TEST_CASE(&ForwardCurveTest::testValues));
    suite->add(BOOST_TEST_CASE(&ForwardCurveTest::testRoll));

    suite->add(BOOST_TEST_CASE(&ForwardCurveTest::testForwardCurveRollsAtForwardVolConstruction));
    suite->add(BOOST_TEST_CASE(&ForwardCurveTest::testRollStochastic));    
    
    return suite;
}


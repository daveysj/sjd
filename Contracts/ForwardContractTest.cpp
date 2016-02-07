#include "ForwardContractTest.h"


using namespace std;
using namespace boost::unit_test_framework;
using namespace boost::math;
using namespace sjd;


boost::shared_ptr<EuropeanForward> ForwardContactTest::getDefaultEuropeanForward(Date anchorDate)
{
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());
    Date rateSetDate = nullCalendar->advance(anchorDate, 2, Years);
    Date settlement = nullCalendar->advance(rateSetDate, 2, Days);

    double strike = 100.0;
    double volume = 150;
    return boost::shared_ptr<EuropeanForward>(new  EuropeanForward(rateSetDate, settlement, strike, volume, BUY));
}

boost::shared_ptr<AsianForward> ForwardContactTest::getDefaultAsianForward(Date anchorDate)
{
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());
    Date rateSetDate = nullCalendar->advance(anchorDate, 2, Years);
    set<Date> averagingDates;
    Date d = Date(1, rateSetDate.month(), rateSetDate.year());
    Date nextMonth = nullCalendar->advance(d, 1, Months);
    while (d  < nextMonth) {
        averagingDates.insert(d);
        d = nullCalendar->advance(d, 1, Days);
    }
    vector<Date> averagingDatesAsVector = vector<Date>(averagingDates.begin(), averagingDates.end());
    Date settlement = nullCalendar->advance(averagingDatesAsVector.back(), 2, Days);

    double strike = 100;
    double volume = 50;

    return boost::shared_ptr<AsianForward>(new AsianForward(averagingDates, settlement, strike, volume, BUY));
}


void ForwardContactTest::testBuySell()
{
    BOOST_MESSAGE("Testing ForwardContract ...");
   
   Date anchorDate(1,Jan,2013);
    boost::shared_ptr<EuropeanForward> fc = getDefaultEuropeanForward(anchorDate);
    BuySell bs = SELL;
    fc->setBuySell(bs);
    BuySell bs2 = fc->getBuySell();
    BOOST_CHECK(bs == bs2);
}

void ForwardContactTest::testStrike()
{
    Date anchorDate(1,Jan,2013);
    boost::shared_ptr<EuropeanForward> contract = getDefaultEuropeanForward(anchorDate);
    boost::shared_ptr<DiscountCurve> dc; 
    boost::shared_ptr<ForwardCurveDeterministic> fc;
    contract->validate();
    BOOST_CHECK(contract->isOK());
    double strike = -1;
    contract->setStrike(strike);
    contract->validate();
    BOOST_CHECK(!contract->isOK());
    BOOST_CHECK(abs(contract->getStrike()-strike)<EPSILON_FOR_DOUBLES);
}

void ForwardContactTest::testVolume()
{
    Date anchorDate(1,Jan,2013);
    boost::shared_ptr<EuropeanForward> contract = getDefaultEuropeanForward(anchorDate);
    boost::shared_ptr<DiscountCurve> dc; 
    boost::shared_ptr<ForwardCurveDeterministic> fc;
    contract->validate();
    BOOST_CHECK(contract->isOK());
    double volume = -1;
    contract->setVolume(volume);
    contract->validate();
    BOOST_CHECK(contract->isOK());
    BOOST_CHECK(abs(contract->getVolume()+volume)<EPSILON_FOR_DOUBLES);
}

void ForwardContactTest::testMaturityDate()
{
    Date anchorDate(1,Jan,2013);
    boost::shared_ptr<EuropeanForward> contract = getDefaultEuropeanForward(anchorDate);
    boost::shared_ptr<DiscountCurve> dc; 
    boost::shared_ptr<ForwardCurveDeterministic> fc;
    contract->validate();
    BOOST_CHECK(contract->isOK());
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());
    Date maturityDate = nullCalendar->advance(anchorDate, 3, Years);
    contract->setMaturityDate(maturityDate);
    contract->validate();
    BOOST_CHECK(!contract->isOK());
    BOOST_CHECK(contract->getMaturityDate() == maturityDate);
}

void ForwardContactTest::testSettlementDate()
{
    Date anchorDate(1,Jan,2013);
    boost::shared_ptr<EuropeanForward> contract = getDefaultEuropeanForward(anchorDate);
    boost::shared_ptr<DiscountCurve> dc; 
    boost::shared_ptr<ForwardCurveDeterministic> fc;
    contract->validate();
    BOOST_CHECK(contract->isOK());
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());
    Date settlementDate = nullCalendar->advance(anchorDate, 1, Years);
    contract->setSettlementDate(settlementDate);
    contract->validate();
    BOOST_CHECK(!contract->isOK());
    BOOST_CHECK(contract->getSettlementDate() == settlementDate);
}


void ForwardContactTest::testValue() 
{

    boost::shared_ptr<RatesEnvironment> re;
    GenerateRates gr = GenerateRates();

    boost::shared_ptr<DayCounter> dayCounter = gr.defaultDayCounter;
    boost::shared_ptr<Calendar> nullCalendar = gr.defaultCalendar;

    Date anchorDate(1,Jan,2013);
    Date tPlus2 = nullCalendar->advance(anchorDate, 2, Days);
    Date rateSetDate = nullCalendar->advance(anchorDate, 2, Years);
    Date settlement = nullCalendar->advance(rateSetDate, 2, Days);

    vector<Date> manditoryDates;
    manditoryDates.push_back(tPlus2);
    manditoryDates.push_back(rateSetDate);
    manditoryDates.push_back(settlement);


    double spot = 100, naccRFR = 0.065, naccIRDiff = 0.045, vol = 0.2;
    
    boost::shared_ptr<DiscountCurve> dc = gr.getDiscountCurve(dayCounter, nullCalendar, anchorDate, 10, naccRFR, manditoryDates);
    boost::shared_ptr<ForwardCurveDeterministic> fc = gr.getSimpleForwardCurve(dayCounter, nullCalendar, anchorDate, 10, spot, naccIRDiff, manditoryDates);
    re = boost::shared_ptr<RatesEnvironment>(new RatesEnvironment(anchorDate));
    re->setDRS(dc);
    re->setFRS(fc);

    double strike = 100;
    double volume = 50;

    EuropeanForward contract(rateSetDate, settlement, strike, volume, BUY);
    EuropeanForward contract2(contract);
    contract2.setBuySell(SELL);

    contract.validate(re);
    BOOST_REQUIRE(contract.isOK());
    contract2.validate(re);
    BOOST_REQUIRE(contract2.isOK());

    double yte = dayCounter->yearFraction(anchorDate, rateSetDate);
    double yts = dayCounter->yearFraction(anchorDate, settlement);
    double F = fc->getForward(settlement);
    double df = dc->getDiscountFactor(settlement);

    // 1) Before maturity
    BOOST_CHECK(abs(contract.getValue(re) + contract2.getValue(re)) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(contract.getValue(re) - volume * (F-strike)* df) / volume < EPSILON_FOR_DOUBLES);


    // 3) After settlement
    Date dayAfterSettlement = nullCalendar->advance(settlement, 1, Days);
    manditoryDates.clear();
    dc = gr.getDiscountCurve(dayCounter, nullCalendar, dayAfterSettlement, 10, naccRFR);
    fc = gr.getSimpleForwardCurve(dayCounter, nullCalendar, dayAfterSettlement, 10, spot, naccIRDiff);
    re = boost::shared_ptr<RatesEnvironment>(new RatesEnvironment(dayAfterSettlement));
    re->setDRS(dc);
    re->setFRS(fc);

    BOOST_CHECK(abs(contract2.getValue(re)) < EPSILON_FOR_DOUBLES);
}

void ForwardContactTest::testGetPayoffAfterMaturity()
{
    boost::shared_ptr<DayCounter> dayCounter = boost::shared_ptr<DayCounter>(new Actual365Fixed());
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());

    Date anchorDate(1,Jan,2013);

    boost::shared_ptr<EuropeanForward> fwd = getDefaultEuropeanForward(anchorDate);

    double v1 = fwd->getVolume();
    double p1 = fwd->getPremiumAmount();
    double k1 = fwd->getStrike();
    double sign1 = (fwd->getBuySell() == BUY ? 1 : -1);

    double maturitySpot = k1+10;
    BOOST_CHECK(abs(fwd->getPayoff(maturitySpot) - (maturitySpot - k1 - p1) * v1 * sign1)< 1e-14);
}


void ForwardContactTest::testAsianAveragingDates()
{
    Date anchorDate(1,Jan,2013);
    boost::shared_ptr<AsianForward> contract = getDefaultAsianForward(anchorDate);
    boost::shared_ptr<DiscountCurve> dc; 
    boost::shared_ptr<ForwardCurveDeterministic> fc;
    boost::shared_ptr<DailyFixingCurve> fixingCurve, fixingCurve2;
    BOOST_CHECK(contract->isOK());

    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());
    Date rateSetDate = nullCalendar->advance(anchorDate, 3, Years); // after settlement
    set<Date> averagingDates;
    Date d = Date(1, rateSetDate.month(), rateSetDate.year());
    Date nextMonth = nullCalendar->advance(d, 1, Months);
    while (d  < nextMonth) 
    {
        averagingDates.insert(d);
        d = nullCalendar->advance(d, 1, Days);
    }
    contract->setAveragingDates(averagingDates);
    contract->validate();
    BOOST_CHECK(!contract->isOK());
}


void ForwardContactTest::testFixingAndForwardDates()
{
    Date anchorDate(1,Jan,2013);
    boost::shared_ptr<AsianForward> contract = getDefaultAsianForward(anchorDate);
    boost::shared_ptr<DiscountCurve> dc; 
    boost::shared_ptr<ForwardCurveDeterministic> fc;
    boost::shared_ptr<DailyFixingCurve> fixingCurve, fixingCurve2;
    BOOST_CHECK(contract->isOK());

    vector<Date> fixingDates, forwardDates, averagingDates;
    averagingDates = contract->getAveragingDates();

    size_t i = averagingDates.size() / 2;
    Date valuationDate = averagingDates[i];

    contract->getFixingAndForwardDates(valuationDate, fixingDates, forwardDates);
    BOOST_CHECK(fixingDates.size() == i+1);
    BOOST_CHECK(forwardDates.size() == averagingDates.size()-i-1);
}


void ForwardContactTest::testAsianForward() 
{

    boost::shared_ptr<RatesEnvironment> re;
    GenerateRates gr = GenerateRates();

    boost::shared_ptr<DayCounter> dayCounter = gr.defaultDayCounter;
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());

    Date anchorDate(1,Jan,2013);
    Date tPlus2 = nullCalendar->advance(anchorDate, 2, Days);

    set<Date> averagingDates;
    vector<Date> manditoryDates;
    manditoryDates.push_back(tPlus2);
    Date d = Date(1, Jan, 2014);
    Date nextMonth = Date(1, Feb, 2014);
    while (d  < nextMonth) 
    {
        averagingDates.insert(d);
        manditoryDates.push_back(d);
        d = nullCalendar->advance(d, 1, Days);
    }
    vector<Date> averagingDatesAsVector = vector<Date>(averagingDates.begin(), averagingDates.end());
    Date settlement = nullCalendar->advance(averagingDatesAsVector.back(), 2, Days);
    manditoryDates.push_back(settlement);


    double spot = 100, naccRFR = 0.065, naccIRDiff = 0.045, vol = 0.2;
    
    boost::shared_ptr<DiscountCurve> dc = gr.getDiscountCurve(dayCounter, nullCalendar, anchorDate, 10, naccRFR, manditoryDates);
    boost::shared_ptr<ForwardCurveDeterministic> fc = gr.getSimpleForwardCurve(dayCounter, nullCalendar, anchorDate, 10, spot, naccIRDiff, manditoryDates);
    boost::shared_ptr<DailyFixingCurve> fixingCurve = gr.buildFixingCurve(90.0, averagingDatesAsVector);
    re = boost::shared_ptr<RatesEnvironment>(new RatesEnvironment(anchorDate));
    re->setDRS(dc);
    re->setFRS(fc);
    re->setFixingRS(fixingCurve);


    double strike = 100;
    double volume = 50;

    AsianForward contract(averagingDates, settlement, strike, volume, BUY);
    AsianForward contract2(contract);
    contract2.setBuySell(SELL);
    
    contract.validate(re);
    BOOST_REQUIRE(contract.isOK());
    contract2.validate(re);
    BOOST_REQUIRE(contract2.isOK());


    //double yte = dayCounter->yearFraction(anchorDate, rateSetDate);
    double yts = dayCounter->yearFraction(anchorDate, settlement);
    double averageFwd = 0;
    for (size_t i = 0; i < averagingDatesAsVector.size(); ++i)
    {
        averageFwd += fc->getForward(averagingDatesAsVector[i]);
    }
    averageFwd /= averagingDatesAsVector.size();

    double df = dc->getDiscountFactor(settlement);

    // 1) Before maturity
    BOOST_CHECK(abs(contract.getValue(re) + contract2.getValue(re)) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(contract.getValue(re) - volume * (averageFwd-strike)* df) / volume < EPSILON_FOR_DOUBLES);


    // 3) After settlement
    Date dayAfterSettlement = nullCalendar->advance(settlement, 1, Days);
    manditoryDates.clear();
    dc = gr.getDiscountCurve(dayCounter, nullCalendar, dayAfterSettlement, 10, naccRFR);
    fc = gr.getSimpleForwardCurve(dayCounter, nullCalendar, dayAfterSettlement, 10, spot, naccIRDiff);
    re = boost::shared_ptr<RatesEnvironment>(new RatesEnvironment(dayAfterSettlement));
    re->setDRS(dc);
    re->setFRS(fc);
    BOOST_CHECK(abs(contract2.getValue(re)) < EPSILON_FOR_DOUBLES);
}

void ForwardContactTest::testAsianGetPayoffAfterMaturity()
{
    boost::shared_ptr<DayCounter> dayCounter = boost::shared_ptr<DayCounter>(new Actual365Fixed());
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());

    Date anchorDate(1,Jan,2013);

    boost::shared_ptr<AsianForward> fwd = getDefaultAsianForward(anchorDate);

    double v1 = fwd->getVolume();
    double p1 = fwd->getPremiumAmount();
    double k1 = fwd->getStrike();
    double sign1 = (fwd->getBuySell() == BUY ? 1 : -1);

    double maturitySpot = k1+10;
    BOOST_CHECK(abs(fwd->getPayoff(maturitySpot) - (maturitySpot - k1 - p1) * v1 * sign1)< 1e-14);
}


test_suite* ForwardContactTest::suite() 
{
    test_suite* suite = BOOST_TEST_SUITE("Forward Contracts");

    suite->add(BOOST_TEST_CASE(&ForwardContactTest::testBuySell));
    suite->add(BOOST_TEST_CASE(&ForwardContactTest::testStrike));
    suite->add(BOOST_TEST_CASE(&ForwardContactTest::testVolume));
    suite->add(BOOST_TEST_CASE(&ForwardContactTest::testMaturityDate));
    suite->add(BOOST_TEST_CASE(&ForwardContactTest::testSettlementDate));
    suite->add(BOOST_TEST_CASE(&ForwardContactTest::testValue));
    suite->add(BOOST_TEST_CASE(&ForwardContactTest::testGetPayoffAfterMaturity));


    BOOST_MESSAGE("Testing AsianForwards ...");
    suite->add(BOOST_TEST_CASE(&ForwardContactTest::testAsianAveragingDates));
    suite->add(BOOST_TEST_CASE(&ForwardContactTest::testFixingAndForwardDates));

    suite->add(BOOST_TEST_CASE(&ForwardContactTest::testAsianForward));
    suite->add(BOOST_TEST_CASE(&ForwardContactTest::testAsianGetPayoffAfterMaturity));
    
    return suite;
}


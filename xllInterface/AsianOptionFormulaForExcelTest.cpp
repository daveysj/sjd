#include "AsianOptionFormulaTest.h"

using namespace std;
using namespace boost::unit_test_framework;
using namespace boost::math;
using namespace sjd;


/*======================================================================================
AsianOptionInputParameters
=======================================================================================*/
AsianOptionInputParameters::AsianOptionInputParameters(
                        vector<double> time,
                        vector<double> fwds, 
                        double strike, 
                        double yte, 
                        double yts, 
                        vector<double> sds, 
                        double df):
time(time), fwds(fwds), sds(sds), strike(strike), yte(yte), yts(yts), df(df)
{
    numFwdsRateSet = 0;
    vector<double>::iterator it = time.begin();
    while ((it!=time.end()) && (*it <= 0)) {
        ++numFwdsRateSet;
        ++it;
    }
}


/*======================================================================================
AsianOptionTest
=======================================================================================*/
void AsianOptionTest::testErrorMessages() {
    BOOST_MESSAGE("Testing Asian Option Error Messages ...");
    BOOST_MESSAGE("... before averaging");
    testErrorMessagesWithSpecificData(getAsianInputsBeforeAveragingStarts());
    BOOST_MESSAGE("... during averaging");
    testErrorMessagesWithSpecificData(getAsianInputsDuringAveraging());
    BOOST_MESSAGE("... during averaging with high average");
    testErrorMessagesWithSpecificData(getAsianInputsJustBeforeSettlementWithHighAverage());
    BOOST_MESSAGE("... during averaging with low average");
    testErrorMessagesWithSpecificData(getAsianInputsJustBeforeSettlementWithLowAverage());
}

void AsianOptionTest::testErrorMessagesWithSpecificData(AsianOptionInputParameters ip) {

    AverageRateCall call(ip.time, ip.fwds, ip.strike, ip.yte, ip.yts, ip.sds, ip.df);
    AverageRatePut put(ip.time, ip.fwds, ip.strike, ip.yte, ip.yts, ip.sds, ip.df);
    BOOST_CHECK(call.isOK() == true);
    BOOST_CHECK(put.isOK() == true);

    // input vectors must have the correct dimension
    vector<double> incorrectTime = ip.time;
    incorrectTime.pop_back();
    put.setYearFractionsToRateSets(incorrectTime);
    BOOST_CHECK(put.isOK() == false);
    vector<double> incorrectFwds = ip.fwds;
    incorrectFwds.pop_back();
    put.setForwards(incorrectFwds);
    BOOST_CHECK(put.isOK() == false);
    vector<double> incorrectSD = ip.sds;
    incorrectSD.pop_back();
    put.setStandardDeviations(incorrectSD);
    BOOST_CHECK(put.isOK() == true);
    put.setParameters(ip.time, ip.fwds, ip.strike, ip.yte, ip.yts, ip.sds, ip.df);

    // all fwds have rateset
    incorrectTime.clear();
    size_t N = ip.time.size();
    for (size_t i = 0; i < N; ++i) {
        incorrectTime.push_back(ip.time[i] - (ip.time[N-1] + 1));
    }
    put.setYearFractionsToRateSets(incorrectTime);
    BOOST_CHECK(put.isOK() == false);
    BOOST_CHECK(put.getErrorMessage().compare("AverageRatePut->AverageRateOption->All forwards have rate set, this is not an option anymore") == 0);
    put.setYearFractionsToRateSets(ip.time);
    BOOST_CHECK(put.isOK() == true);
}

void AsianOptionTest::testGreeks()
{
    BOOST_MESSAGE("Testing Asian Option Greeks ...");
    BOOST_MESSAGE("... before averaging");
    testGreeksWithSpecificData(getAsianInputsBeforeAveragingStarts());
    BOOST_MESSAGE("... during averaging");
    testGreeksWithSpecificData(getAsianInputsDuringAveraging());
    BOOST_MESSAGE("... during averaging with high average");
    testGreeksWithSpecificData(getAsianInputsJustBeforeSettlementWithHighAverage());
    BOOST_MESSAGE("... during averaging with low average");
    testGreeksWithSpecificData(getAsianInputsJustBeforeSettlementWithLowAverage());
}

void AsianOptionTest::testGreeksWithSpecificData(AsianOptionInputParameters ip)
{
    AverageRateCall call(ip.time, ip.fwds, ip.strike, ip.yte, ip.yts, ip.sds, ip.df);
    AverageRatePut put(ip.time, ip.fwds, ip.strike, ip.yte, ip.yts, ip.sds, ip.df);

    size_t M = 0;
    vector<double>::iterator it = ip.time.begin();
    while ((it != ip.time.end()) && (*it <= 0)) {
        ++M;
        ++it;
    }

    double callPremium = call.getPremium();
    double putPremium = put.getPremium();
	BOOST_CHECK(callPremium == AverageRateCall::getPremium(ip.fwds, M, ip.strike, ip.sds, ip.df)); // class and static method are the same
	BOOST_CHECK(putPremium == AverageRatePut::getPremium(ip.fwds, M, ip.strike, ip.sds, ip.df)); // class and static method are the same
	//BOOST_CHECK(abs((putPremium - callPremium) - (X-F) * df) < 1e-14); // put call parity
    
    double absoluteChangeInFormawrd = ip.fwds[0] * 0.01;
	double callDeltaAnalytic = call.getDeltaAnalytic();
	double putDeltaAnalytic = put.getDeltaAnalytic();
    double callDeltaNumeric = call.getDeltaNumeric(absoluteChangeInFormawrd);
    double putDeltaNumeric = put.getDeltaNumeric(absoluteChangeInFormawrd);
	            
    double callGammaAnalytic = call.getGammaAnalytic();
    double putGammaAnalytic = put.getGammaAnalytic();
    double callGammaNumeric = call.getGammaNumeric(absoluteChangeInFormawrd);
    double putGammaNumeric = put.getGammaNumeric(absoluteChangeInFormawrd);
                        
    double absoluteChangeVolatility = 0.0001;
    double callVegaAnalytic = call.getVegaAnalytic();
    double putVegaAnalytic = put.getVegaAnalytic();
    double callVegaNumeric = call.getVegaNumeric(absoluteChangeVolatility);
    double putVegaNumeric = put.getVegaNumeric(absoluteChangeVolatility);

    double absoluteChangeInYearFractions = 1.0 / 365;
    double callThetaAnalytic = call.getThetaAnalytic();
    double putThetaAnalytic = put.getThetaAnalytic();
    double callThetaNumeric = call.getThetaNumeric(absoluteChangeInYearFractions);
    double putThetaNumeric = put.getThetaNumeric(absoluteChangeInYearFractions);
	    
    double absoluteChangeInNominalInterestRate = 0.0001;
    double callRhoAnalytic = call.getRhoAnalytic();
    double putRhoAnalytic = put.getRhoAnalytic();
    double callRhoNumeric = call.getRhoNumeric(absoluteChangeInNominalInterestRate);
    double putRhoNumeric = put.getRhoNumeric(absoluteChangeInNominalInterestRate);

    BlackScholesGreeks callGreeksAnalytic = call.getOptionPriceAndGreeksAnalytic();
    BlackScholesGreeks putGreeksAnalytic = put.getOptionPriceAndGreeksAnalytic();
    BlackScholesGreeks callGreeksNumeric = call.getOptionPriceAndGreeksNumeric(absoluteChangeInFormawrd, 
                                                                               absoluteChangeVolatility, 
                                                                               absoluteChangeInYearFractions, 
                                                                               absoluteChangeInNominalInterestRate);
    BlackScholesGreeks putGreeksNumeric = put.getOptionPriceAndGreeksNumeric(absoluteChangeInFormawrd, 
                                                                             absoluteChangeVolatility, 
                                                                             absoluteChangeInYearFractions, 
                                                                             absoluteChangeInNominalInterestRate);

    BOOST_CHECK(callPremium == callGreeksAnalytic.getPremium());
    BOOST_CHECK(callDeltaAnalytic == callGreeksAnalytic.getDelta());
    BOOST_CHECK(callGammaAnalytic == callGreeksAnalytic.getGamma());
    BOOST_CHECK(callVegaAnalytic == callGreeksAnalytic.getVega());
    BOOST_CHECK(callThetaAnalytic == callGreeksAnalytic.getTheta());
    BOOST_CHECK(callRhoAnalytic == callGreeksAnalytic.getRho());

    BOOST_CHECK(putPremium == putGreeksAnalytic.getPremium());
    BOOST_CHECK(putDeltaAnalytic == putGreeksAnalytic.getDelta());
    BOOST_CHECK(putGammaAnalytic == putGreeksAnalytic.getGamma());
    BOOST_CHECK(putVegaAnalytic == putGreeksAnalytic.getVega());
    BOOST_CHECK(putThetaAnalytic == putGreeksAnalytic.getTheta());
    BOOST_CHECK(putRhoAnalytic == putGreeksAnalytic.getRho());

    BOOST_CHECK(callDeltaNumeric == callGreeksNumeric.getDelta());
    BOOST_CHECK(callGammaNumeric == callGreeksNumeric.getGamma());
    BOOST_CHECK(callVegaNumeric == callGreeksNumeric.getVega());
    BOOST_CHECK(callThetaNumeric == callGreeksNumeric.getTheta());
    BOOST_CHECK(callRhoNumeric == callGreeksNumeric.getRho());

    BOOST_CHECK(putDeltaNumeric == putGreeksNumeric.getDelta());
    BOOST_CHECK(putGammaNumeric == putGreeksNumeric.getGamma());
    BOOST_CHECK(putVegaNumeric == putGreeksNumeric.getVega());
    BOOST_CHECK(putThetaNumeric == putGreeksNumeric.getTheta());
    BOOST_CHECK(putRhoNumeric == putGreeksNumeric.getRho());
}

void AsianOptionTest::testSpecialCases()
{
    BOOST_MESSAGE("Testing Asian Option Specail Cases ...");
    BOOST_MESSAGE("... Call in the money");
    AsianOptionInputParameters ip = getAsianInputsJustBeforeSettlementWithHighAverage();
    testSpecialCasesWithSpecificData(ip); 
    BOOST_MESSAGE("... Put in the money");
    ip = getAsianInputsJustBeforeSettlementWithLowAverage();
    testSpecialCasesWithSpecificData(ip); 
}

void AsianOptionTest::testSpecialCasesWithSpecificData(AsianOptionInputParameters ip)
{
    AverageRateCall call(ip.time, ip.fwds, ip.strike, ip.yte, ip.yts, ip.sds, ip.df);
    AverageRatePut put(ip.time, ip.fwds, ip.strike, ip.yte, ip.yts, ip.sds, ip.df);

    AverageRateOption *call2 = new AverageRateCall(ip.time, ip.fwds, ip.strike, ip.yte, ip.yts, ip.sds, ip.df);
    AverageRateOption *put2 = new AverageRatePut(ip.time, ip.fwds, ip.strike, ip.yte, ip.yts, ip.sds, ip.df);

    // Numeric Theta move is from pre- to post expiry
    double absoluteDt = 4.0 / 365.0;
    double callTheta = call.getThetaNumeric(absoluteDt);
    double putTheta = put.getThetaNumeric(absoluteDt);
    double call2Theta = call2->getThetaNumeric(absoluteDt);
    double put2Theta = put2->getThetaNumeric(absoluteDt);

    double bsFwd, bsStrike, bsSD, bsMoneyness;
    size_t M = ip.numFwdsRateSet;
    AverageRateOption::getEquivalentBlack76Paramters(ip.fwds, M,ip.strike, ip.sds, 
                                                     bsFwd, bsStrike, bsSD, bsMoneyness);

    BOOST_CHECK((callTheta - (call.getPremium() - (bsMoneyness))) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK((putTheta - put.getPremium()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK((call2Theta - callTheta) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK((put2Theta - putTheta) < EPSILON_FOR_DOUBLES);

    BlackScholesGreeks callGreeksNumeric = call.getOptionPriceAndGreeksNumeric(0.0001, 0.0001, absoluteDt, 0.0001);
    BlackScholesGreeks putGreeksNumeric = put.getOptionPriceAndGreeksNumeric(0.0001, 0.0001, absoluteDt, 0.0001);
    BOOST_CHECK((callTheta - callGreeksNumeric.getTheta()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK((putTheta - putGreeksNumeric.getTheta()) < EPSILON_FOR_DOUBLES);

    delete call2;
    delete put2;
}


test_suite* AsianOptionTest::suite() {
    test_suite* suite = BOOST_TEST_SUITE("Asian Option Pricing Suite");
    suite->add(BOOST_TEST_CASE(&AsianOptionTest::testErrorMessages));
    suite->add(BOOST_TEST_CASE(&AsianOptionTest::testGreeks));
    suite->add(BOOST_TEST_CASE(&AsianOptionTest::testSpecialCases));

    return suite;
}

AsianOptionInputParameters AsianOptionTest::getAsianInputsBeforeAveragingStarts()
{
    vector<double> time, fwds, sds;
    double strike, yte, yts, df; 

    double spot = 21.13;
    strike = 22.0;
    double rfr = 0.065,  yield = 0.025;
    double vol = 0.25;
    size_t i;
    for (i = 366; i < 386; ++i) // start of averaging is about a year from now for 20 days
    {
        double yearsAhead = (double) i / 365.0;
        time.push_back(yearsAhead);
        fwds.push_back(spot * exp(-(rfr - yield) * (yearsAhead + 2.0 / 365.0))); // T+2 convention for fwds
        sds.push_back(vol * sqrt(yearsAhead));
    }
    yte = (double) i / 365.0;
    yts = (double) (i+2) / 365.0;
    df = exp(-rfr * yts);

    return AsianOptionInputParameters(time, fwds, strike, yte, yts, sds, df);
}

AsianOptionInputParameters AsianOptionTest::getAsianInputsDuringAveraging()
{
    vector<double> time, fwds, sds;
    double strike, yte, yts, df; 

    double spot = 21.13;
    strike = 22.0;
    double rfr = 0.065,  yield = 0.025;
    double vol = 0.25;
    int i;
    for (i = -5; i < 15; ++ i) {
        double yearsAhead = (double) i / 365.0;
        time.push_back(yearsAhead);
        fwds.push_back(spot); // rate set values
        if (yearsAhead <= 0) {
            sds.push_back(0); // rate set values
        }
        else 
            sds.push_back(vol * sqrt(yearsAhead));
    }

    yte = (double) i / 365.0;
    yts = (double) (i+2) / 365.0;
    df = exp(-rfr * yts);
    return AsianOptionInputParameters(time, fwds, strike, yte, yts, sds, df);
}

AsianOptionInputParameters AsianOptionTest::getAsianInputsJustBeforeSettlementWithHighAverage()
{

    vector<double> time, fwds, sds;
    double strike, yte, yts, df; 

    double spot = 21.13;
    strike = 22.0;
    double rfr = 0.065,  yield = 0.025;
    double vol = 0.25;
    int i;
    for (i = -18; i < 3; ++ i) {
        double yearsAhead = (double) i / 365.0;
        time.push_back(yearsAhead);
        if (yearsAhead <= 0) {
            sds.push_back(0); 
            fwds.push_back(spot * 1.2); 
        }
        else {
            sds.push_back(vol * sqrt(yearsAhead));
            fwds.push_back(spot); 
        }
    }
    yte = (double) i / 365.0;
    yts = (double) (i+2) / 365.0;
    df = exp(-rfr * yts);
    return AsianOptionInputParameters(time, fwds, strike, yte, yts, sds, df);
}

AsianOptionInputParameters AsianOptionTest::getAsianInputsJustBeforeSettlementWithLowAverage()
{
    vector<double> time, fwds, sds;
    double strike, yte, yts, df; 

    double spot = 21.13;
    strike = 22.0;
    double rfr = 0.065,  yield = 0.025;
    double vol = 0.25;
    int i;
    for (i = -18; i < 3; ++ i) {
        double yearsAhead = (double) i / 365.0;
        time.push_back(yearsAhead);
        if (yearsAhead <= 0) {
            sds.push_back(0); 
            fwds.push_back(spot * 0.8); 
        }
        else {
            sds.push_back(vol * sqrt(yearsAhead));
            fwds.push_back(spot); 
        }
    }
    yte = (double) i / 365.0;
    yts = (double) (i+2) / 365.0;
    df = exp(-rfr * yts);
    return AsianOptionInputParameters(time, fwds, strike, yte, yts, sds, df);
}

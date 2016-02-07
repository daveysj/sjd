#include "BlackScholesFormulaTest.h"


using namespace std;
using namespace boost::unit_test_framework;
using namespace boost::math;
using namespace sjd;

void BlackScholesTest::testInputsAndOutputs() {
    BOOST_MESSAGE("Testing Black Scholes Inputs ...");

    double forward = 100, strike = 110, yearsToExpiry = 1.0, yearsToSettlement = 1.01, 
           standardDeviation = 0.2, discountFactor = 0.97;
    BlackInputs a, b;
    a = BlackInputs(forward, strike, yearsToExpiry, yearsToSettlement, standardDeviation, discountFactor);
    // first set b to something else then use the assignement operator
    b = BlackInputs(forward+1, strike+1, yearsToExpiry+0.005, yearsToSettlement+0.005, standardDeviation+0.0005, discountFactor-0.00005);
    b = a;
    BlackInputs c(b);

    BOOST_CHECK(abs(a.getForward() - b.getForward()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(a.getStrike() - c.getStrike()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(a.getYearsToExpiry() - b.getYearsToExpiry()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(a.getYearsToSettlement() - c.getYearsToSettlement()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(a.getStandardDeviation() - b.getStandardDeviation()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(a.getDiscountFactor() - c.getDiscountFactor()) < EPSILON_FOR_DOUBLES);    

    BOOST_MESSAGE("Testing Black Scholes Greeks ...");
    double premium = 0.05, delta = -0.5, gamma = 0.0255, theta = -0.0125, vega = 0.05, rho = 0.0275;
    BlackScholesGreeks d, e;
    d = BlackScholesGreeks(premium, delta, gamma, theta, vega, rho);
    e = BlackScholesGreeks(premium+0.01, delta+0.01, gamma+0.01, theta+0.01, vega+0.01, rho+0.01);
    BlackScholesGreeks* f = new BlackScholesGreeks(premium+0.01, delta+0.01, gamma+0.01, theta+0.01, vega+0.01, rho+0.01);
    e = d;
    *f = d;
    BOOST_CHECK(abs(d.getPremium() - e.getPremium()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(d.getDelta() - f->getDelta()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(d.getGamma() - e.getGamma()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(d.getTheta() - f->getTheta()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(d.getVega() - e.getVega()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(d.getRho() - f->getRho() ) < EPSILON_FOR_DOUBLES);    


    delete f;
}

void BlackScholesTest::testPutCallParity() {
    BOOST_MESSAGE("Testing Black Scholes Option Pricing ...");

    double F = 110.1;
    double X = 115.0;
    double yte = 1.5;
    double yts = 1.5;
    double vol = 0.21;
    double sd = vol * sqrt(yte); 
    double r = 0.07;
    double df = exp(-r * yts);

    EuropeanCallBlack76 call(F, X, yte, yts, sd, df);
    EuropeanPutBlack76 put(F, X, yte, yts, sd, df);

    double callPremium = call.getPremium();
    double putPremium = put.getPremium();

	BOOST_CHECK(callPremium == call.getPremium(F, X, sd, df)); // class and static method are the same
	BOOST_CHECK(putPremium == put.getPremium(F, X, sd, df)); // class and static method are the same
	BOOST_CHECK(abs((putPremium - callPremium) - (X-F) * df) < 1e-14); // put call parity

    double F1 = 11.1;
    double X1 = 11.0;
    double yte1 = 1.3;
    double yts1 = 1.3;
    double vol1 = 0.2;
    double sd1 = vol1 * sqrt(yte1); 
    double r1 = 0.065;
    double df1 = exp(-r1 * yts1);
    call.setForward(F1);
    BOOST_CHECK(call.getForward() == F1);
    call.setStrike(X1);
    BOOST_CHECK(call.getStrike() == X1);
    call.setTimeToSettlement(yts1);
    BOOST_CHECK(call.getTimeToSettlement() == yts1);
    call.setTimeToExpiry(yte1);
    BOOST_CHECK(call.getTimeToExpiry() == yte1);
    call.setStandardDeviation(sd1);
    BOOST_CHECK(call.getStandardDeviation() == sd1);
    call.setDiscountFactor(df1);
    BOOST_CHECK(call.getDiscountFactor() == df1);

    put.setParameters(F1, X1, yte1, yts1, sd1, df1);
    BOOST_CHECK(put.getForward() == F1);
    BOOST_CHECK(put.getStrike() == X1);
    BOOST_CHECK(put.getTimeToSettlement() == yts1);
    BOOST_CHECK(put.getTimeToExpiry() == yte1);
    BOOST_CHECK(put.getStandardDeviation() == sd1);
    BOOST_CHECK(put.getDiscountFactor() == df1);

	BOOST_CHECK(abs((put.getPremium() - call.getPremium()) - (X1-F1) * df1) < 1e-14); // put call parity
}

void BlackScholesTest::testGreeks() {
    BOOST_MESSAGE("Testing Black Scholes Greeks ...");

    double F = 11.1;
    double X = 11.0;
    double yte = 1.3;
    double yts = 1.3;
    double vol = 0.2;
    double sd = vol * sqrt(yte); 
    double r = 0.065;
    double df = exp(-r * yts);

    EuropeanCallBlack76 call(F, X, yte, yts, sd, df);
    EuropeanPutBlack76 put(F, X, yte, yts, sd, df);

    double callPremium = call.getPremium();
    double putPremium = put.getPremium();

    double absoluteChangeInFormawrd = F * 0.01;

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

    BOOST_CHECK(abs(callPremium - 0.968469708326806) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(callDeltaNumeric - 0.05811005233054400) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(callGammaNumeric - 0.00176339669731362) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(callVegaNumeric - 0.00045853820907349) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(callThetaNumeric - (-0.00079456871980455)) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(callRhoNumeric - (-0.00012589287886811)) < EPSILON_FOR_DOUBLES);

    BOOST_CHECK(abs(putPremium - 0.87657254278912300) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(putDeltaNumeric - (-0.04389580141628500)) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(putGammaNumeric - 0.00176339669731274) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(putVegaNumeric - 0.00045853820907349) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(putThetaNumeric - (-0.00081093542572819)) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(putRhoNumeric - (-0.00011394702384560)) < EPSILON_FOR_DOUBLES);

    BOOST_CHECK(abs(callDeltaAnalytic - 0.51561506794826200) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(callGammaAnalytic - 0.14313883086248400) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(callVegaAnalytic - 4.58539519114734000) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(callThetaAnalytic - (-0.28977217597009200)) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(callRhoAnalytic - (-1.25901062082485000)) < EPSILON_FOR_DOUBLES);

    BOOST_CHECK(abs(putDeltaAnalytic - (-0.40335658742857000)) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(putGammaAnalytic - 0.14313883086248400) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(putVegaAnalytic - 4.58539519114734000) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(putThetaAnalytic - (-0.29574549173004100)) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK(abs(putRhoAnalytic - (-1.13954430562586000)) < EPSILON_FOR_DOUBLES);
}

void BlackScholesTest::testSpecialCases() {
    BOOST_MESSAGE("Testing Black Scholes Special Cases ...");

    double F = 11.1;
    double X = 11.0;
    double yte = 1.0 / 365.0;
    double yts = yte;
    double vol = 0.2;
    double sd = vol * sqrt(yte); 
    double r = 0.065;
    double df = exp(-r * yts);

    // Numeric Theta move is from pre- to post expiry
    EuropeanCallBlack76 call(F, X, yte, yts, sd, df);
    EuropeanPutBlack76 put(F, X, yte, yts, sd, df);
    EuropeanOptionBlack76 *call2 = new EuropeanCallBlack76(F, X, yte, yts, sd, df);
    EuropeanOptionBlack76 *put2 = new EuropeanPutBlack76(F, X, yte, yts, sd, df);
    double absoluteDt = 2.0 / 365.0;
    // Case 1: call is in the money
    double callTheta = call.getThetaNumeric(absoluteDt);
    double putTheta = call.getThetaNumeric(absoluteDt);
    double call2Theta = call2->getThetaNumeric(absoluteDt);
    double put2Theta = put2->getThetaNumeric(absoluteDt);
    BOOST_CHECK((callTheta - (call.getPremium() - (call.getForward() - call.getStrike()))) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK((putTheta - put.getPremium()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK((call2Theta - (call.getPremium() - (call.getForward() - call.getStrike()))) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK((put2Theta - put.getPremium()) < EPSILON_FOR_DOUBLES);

    BlackScholesGreeks callGreeksNumeric = call.getOptionPriceAndGreeksNumeric(0.0001, 0.0001, absoluteDt, 0.0001);
    BlackScholesGreeks putGreeksNumeric = put.getOptionPriceAndGreeksNumeric(0.0001, 0.0001, absoluteDt, 0.0001);
    BOOST_CHECK((callTheta - callGreeksNumeric.getTheta()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK((putTheta - putGreeksNumeric.getTheta()) < EPSILON_FOR_DOUBLES);


    // Case 2: put is in the money
    call.setForward(X-0.1);
    put.setForward(X-0.01);
    call2->setForward(X-0.1);
    put2->setForward(X-0.01);
    callTheta = call.getThetaNumeric(absoluteDt);
    putTheta = put.getThetaNumeric(absoluteDt);
    call2Theta = call2->getThetaNumeric(absoluteDt);
    put2Theta = put2->getThetaNumeric(absoluteDt);
    BOOST_CHECK((callTheta - call.getPremium()) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK((putTheta - (put.getPremium() - (put.getStrike() - put.getForward()))) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK((call2Theta - callTheta) < EPSILON_FOR_DOUBLES);
    BOOST_CHECK((put2Theta - putTheta) < EPSILON_FOR_DOUBLES);

    callGreeksNumeric = call.getOptionPriceAndGreeksNumeric(0.0001, 0.0001, absoluteDt, 0.0001);
    putGreeksNumeric = put.getOptionPriceAndGreeksNumeric(0.0001, 0.0001, absoluteDt, 0.0001);
    BOOST_CHECK((callTheta - callGreeksNumeric.getTheta()) < EPSILON_FOR_DOUBLES);
    //cout << "putTheta: " << putTheta << ", putGreeksNumeric.getTheta(): " << putGreeksNumeric.getTheta() << endl;
    BOOST_CHECK((putTheta - putGreeksNumeric.getTheta()) < EPSILON_FOR_DOUBLES);


    delete call2;
    delete put2;
}

void BlackScholesTest::testErrorMessages() {
    BOOST_MESSAGE("Testing Black Scholes Error Messages ...");


    double F = 11.1;
    double X = 11.0;
    double yte = 1.3;
    double yts = 1.3;
    double vol = 0.2;
    double sd = vol * sqrt(yte); 
    double r = 0.065;
    double df = exp(-r * yts);

    EuropeanCallBlack76 call(F, X, yte, yts, sd, df);
    EuropeanPutBlack76 put(F, X, yte, yts, sd, df);

    call.setForward(-1.0);
    BOOST_CHECK(call.isOK() == false);
    string expectedErrorMessage = "EuropeanCallBlack76->EuropeanOptionBlack76->BlackInputs->Forward is <= 0";
    BOOST_CHECK(call.getErrorMessage().compare(expectedErrorMessage) == 0);
    call.setForward(F);

    put.setStandardDeviation(0);
    BOOST_CHECK(put.isOK() == false);
    expectedErrorMessage = "EuropeanPutBlack76->EuropeanOptionBlack76->BlackInputs->Standard Deviation is <= 0";
    BOOST_CHECK(put.getErrorMessage().compare(expectedErrorMessage) == 0);

}

test_suite* BlackScholesTest::suite() {
    test_suite* suite = BOOST_TEST_SUITE("Black Scholes Option Pricing Suite");
    suite->add(BOOST_TEST_CASE(&BlackScholesTest::testInputsAndOutputs));
    suite->add(BOOST_TEST_CASE(&BlackScholesTest::testPutCallParity));
    suite->add(BOOST_TEST_CASE(&BlackScholesTest::testGreeks));
    suite->add(BOOST_TEST_CASE(&BlackScholesTest::testSpecialCases));
    suite->add(BOOST_TEST_CASE(&BlackScholesTest::testErrorMessages));

    
    return suite;
}


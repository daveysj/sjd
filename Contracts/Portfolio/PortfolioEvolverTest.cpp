#include "PortfolioEvolverTest.h"

using namespace std;
using namespace boost::unit_test_framework;
using namespace boost::math;
using namespace sjd;

    boost::timer t;

void startTimer() 
{ 
    t.restart(); 
}

void stopTimer(string text) 
{
    double seconds = t.elapsed();
    int hours = int(seconds/3600);
    seconds -= hours * 3600;
    int minutes = int(seconds/60);
    seconds -= minutes * 60;
    int milliseconds = (int) (seconds * 1000);

    std::cout << text << " completed in ";
    if (hours > 0)
        std::cout << hours << " h ";
    if (hours > 0 || minutes > 0)
        std::cout << minutes << " m ";
    std::cout << std::fixed << std::setprecision(0)
                << milliseconds << " milliseconds" << std::endl;
}


void PortfolioEvolverTest::testPortfolioEvovler()
{
    BOOST_TEST_MESSAGE("Testing Portfolios Evolver ... time based tests excluded");
    // /*
    boost::timer t;

    GenerateRates gr = GenerateRates();
    Date anchorDate(27, Jan , 2013);
    anchorDate = gr.defaultCalendar->advance(anchorDate, 0, Months);
    boost::shared_ptr<RatesEnvironment> re = gr.getRatesEnvironmentForAsianOption(anchorDate, vector<Date>());
    if (!re->isOK())
    {
        cout << re->getErrorMessagesAsString() << endl;
    }
    boost::shared_ptr<RatesEnvironment> re2 = boost::shared_ptr<RatesEnvironment>(new RatesEnvironment(anchorDate));
    re2->setDRS(re->getDRS());
    re2->setFRS(re->getFRS());
    re2->setVRS(re->getVRS());
    int numSims = 10;
    set<Date> simdates;
    for (int i = 0; i < 60; ++i)
    {
        simdates.insert(gr.defaultCalendar->advance(anchorDate, i, Months));
    }
    SingleFactorEconomicScenarioGenerator esg2(re2);
    esg2.setSimulationDates(simdates);
    startTimer();
    for (int j = 0; j < numSims / 2; ++j) 
    {
        esg2.getRandomPath();
        esg2.getAntitheticPath();
    }
    stopTimer("Fwd Rate Source Simulation");

    re = gr.getRatesEnvironmentForAsianOption(anchorDate, vector<Date>());
    boost::shared_ptr<SingleFactorEconomicScenarioGenerator> esg = 
        boost::shared_ptr<SingleFactorEconomicScenarioGenerator>(new SingleFactorEconomicScenarioGenerator(re));
    esg->setSimulationDates(simdates);
    if (!esg->isOK())
    {
        cout << esg->getErrorMessageAsString() << endl;
    }

    startTimer();
    for (int j = 0; j < numSims / 2; ++j) 
    {
        esg->getRandomPath();
        esg->getAntitheticPath();
    }
    stopTimer("Fwd and Fixing Rate Source Simulation");

    re = gr.getRatesEnvironmentForAsianOption(anchorDate, vector<Date>());
    int yearsToSettlement = 5;
    int numberOfAveragingDates = 21;
    double strike = 65;
    double volume = 10001;
    BuySell bs = SELL;
    PutCall pc = PUT;
    double premium = 5.5;
    bool dp = true;
    boost::shared_ptr<AsianOption> ao = GenerateContracts::getDefaultAsianOption(anchorDate, 
                                                                                yearsToSettlement, 
                                                                                numberOfAveragingDates, 
                                                                                strike, 
                                                                                volume, 
                                                                                bs, 
                                                                                pc, 
                                                                                premium, 
                                                                                dp);
    ao->validate(re);
    BOOST_REQUIRE(ao->isOK() == true);
    startTimer(); 
    for (int j = 0; j < (int) (numSims * simdates.size()); ++j) 
    {
        ao->getValue(re); 
    }
    stopTimer("Option Valuation");

    vector<boost::shared_ptr<Contract>> input;
    input.push_back(ao);
    boost::shared_ptr<Portfolio> portfolio = boost::shared_ptr<Portfolio>(new Portfolio(input));

    re = gr.getRatesEnvironmentForAsianOption(anchorDate, vector<Date>());
    esg = boost::shared_ptr<SingleFactorEconomicScenarioGenerator>(new SingleFactorEconomicScenarioGenerator(re));
    boost::shared_ptr<PortfolioEvolver> pe = boost::shared_ptr<PortfolioEvolver>(new PortfolioEvolver(portfolio, esg, simdates));
    startTimer(); 
    pe->runMonteCarlo(numSims); 
    stopTimer("Portfolio Sim");


    bool useMyThreshold = true;
    double myThreshold = 100000.0;
    bool useCptyThreshold = true;
    double cptyThreshold = 100000.0;
    double minimumTransferAmount = 50000;
    double rounding = 1000;
    boost::shared_ptr<Margin> margin = boost::shared_ptr<Margin>(
        new SimpleMargin(anchorDate,
                         useMyThreshold, 
                         myThreshold,
                         useCptyThreshold, 
                         cptyThreshold,
                         minimumTransferAmount,
                         rounding));
    portfolio = boost::shared_ptr<Portfolio>(new Portfolio(input, margin));
    pe = boost::shared_ptr<PortfolioEvolver>(new PortfolioEvolver(portfolio, esg, simdates));
    startTimer(); 
    pe->runMonteCarlo(numSims);
    stopTimer("Portfolio Sim with margin");



    boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors = boost::shared_ptr<RegCapUnderlyingFactors>(
        new CommodityRegCapFactors);
    boost::shared_ptr<RegCapCreditParameters> regCapCreditParameters = boost::shared_ptr<RegCapCreditParameters>(
        new RegCapCreditParameters(1.5, 0.1));
    portfolio = boost::shared_ptr<Portfolio>(
        new PortfolioB2CEM(input,
                           regCapAssetFactors,
                           regCapCreditParameters,
                           true,
                           margin));
    pe = boost::shared_ptr<PortfolioEvolver>(new PortfolioEvolver(portfolio, esg, simdates));
    startTimer(); 
    pe->runMonteCarlo(numSims);
    stopTimer("Portfolio Sim with reg cap");
    // */
}



test_suite* PortfolioEvolverTest::suite() 
{
    
    test_suite* suite = BOOST_TEST_SUITE("PortfolioEvolver");
    suite->add(BOOST_TEST_CASE(&PortfolioEvolverTest::testPortfolioEvovler));
    //suite->add(BOOST_TEST_CASE(&PortfolioEvolverTest::testPortfolioB2CEM));

    return suite;
    
}


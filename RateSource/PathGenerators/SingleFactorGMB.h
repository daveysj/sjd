
#ifndef SINGLEFACTORGMB_INCLUDED
#define SINGLEFACTORGMB_INCLUDED

#include <math.h>

#include <set>
#include <boost/math/distributions/normal.hpp>
#include <boost/random.hpp>
#include <sjd\RateSource\RatesEnvironment.h>

#include <sjd\RateSource\Forward\ForwardRateSourceStochastic.h>
#include <sjd\RateSource\Forward\UsesInterestRateDifferential.h>
#include <sjd\RateSource\Discount\DeterministicDiscountRateSource.h>
#include <sjd\RateSource\Volatility\DeterministicVolatilityRateSource.h>
#include <sjd\RateSource\Fixing\RollingFixingRateSource.h>

#include "Path.h"

typedef boost::mt19937 RNGType;  

using namespace QuantLib;

namespace sjd 
{
    /*======================================================================================
    EconomicScenarioGenerator
    =======================================================================================*/
    class EconomicScenarioGenerator
    {
    public:
        void resetErrorMessages();
        bool isOK();
        vector<string> getErrorMessages();
        string getErrorMessageAsString();
        Date getAnchorDate();

        virtual void setBaseRates(const boost::shared_ptr<RatesEnvironment> re);
        boost::shared_ptr<RatesEnvironment> getBaseRates();

        // returns a false if there was any problem setting the dates and calculating the values used in the simulation
        virtual bool setSimulationDates(set<Date> simulationDates);
        
        virtual boost::shared_ptr<Path> getRandomPath() = 0;        
        virtual boost::shared_ptr<Path> getAntitheticPath() = 0;

    protected:

        boost::shared_ptr<sjdTools::ErrorTracking> errorTracking;

        boost::shared_ptr<RatesEnvironment> re;

        boost::shared_ptr<Path> path;
        vector<boost::shared_ptr<DiscountRateSource>> drss;
        vector<boost::shared_ptr<DiscountRateSource>> foreigndrss;
        vector<boost::shared_ptr<VolatilityRateSource>> vrss;
        vector<boost::shared_ptr<ForwardRateSource>> frss;
        vector<boost::shared_ptr<RollingFixingRateSource>> fixingrss;

        Date anchorDate;

        vector<Date> simulationDates;
        size_t numDates;
    };



    /*======================================================================================
    SingleFactorEconomicScenarioGenerator

    Has a deterministic discount and volatility rate source, a stochastic forward rate source
    and an optional fixing rate source
    =======================================================================================*/
    class SingleFactorEconomicScenarioGenerator : public EconomicScenarioGenerator
    {
    public:
        SingleFactorEconomicScenarioGenerator(const boost::shared_ptr<RatesEnvironment> re);

        virtual void setBaseRates(const boost::shared_ptr<RatesEnvironment> re);
        bool isOK();

        // returns a false if there was any problem setting the dates and calculating the values used in the simulation
        virtual bool setSimulationDates(set<Date> simulationDates);
        
        virtual void setRandomSeed(long seed);

        // 0 < confidenceInterval < 1
        virtual boost::shared_ptr<Path> getConfidenceInterval(double confidenceInterval);
        virtual boost::shared_ptr<Path> getRandomPath();        
        virtual boost::shared_ptr<Path> getAntitheticPath();
        virtual boost::shared_ptr<Path> getPathForRandomNumber(vector<double> randomNumbers);
        virtual boost::shared_ptr<Path> getPathForSpot(vector<double> spot);


    protected:

        void updateFXForwardWithRolledDiscountRateSources(size_t stepNumber);
        bool frsDependsOnInterestRateDifferential;

        boost::shared_ptr<DeterministicDiscountRateSource> drs;
        boost::shared_ptr<DeterministicDiscountRateSource> foreigndrs;
        boost::shared_ptr<DeterministicVolatilityRateSource> vrs;
        boost::shared_ptr<ForwardRateSourceStochastic> frs;
        boost::shared_ptr<RollingFixingRateSource> fixingrs;
        vector<Date> allFixingDates;
        vector<double> allFixingRates;

        long seed;
        RNGType rng;     
        boost::normal_distribution<double> nd;
        boost::variate_generator<RNGType, boost::normal_distribution<> > normalVariable; 
        vector<double> randomNumbers;

    };



}

#endif
#ifndef CONTRACTSTRESSOR_INCLUDED
#define CONTRACTSTRESSOR_INCLUDED

#include <list>
#include <algorithm>

#include "Contracts.h"
#include <sjd\RateSource\IHaveMarketRateStresses.h>
#include <sjd\RateSource\RatesEnvironment.h>
#include <sjd\RateSource\Forward\UsesInterestRateDifferential.h>
#include <sjd\RateSource\Forward\ForwardCurveFX.h>

namespace sjd 
{
    class ContractStressor
    {
    public:
        // Use this for unmargined portfolios or portfolios that do not require Holding Period calculations
        ContractStressor(boost::shared_ptr<Contract> contract, 
                         boost::shared_ptr<RatesEnvironment> rates);

        bool isOK();
        vector<string> getErrorMessages() const;
        string getErrorMessagesAsString() const;

        vector<pair<string, double> > getStressedValues();

    protected:
        vector<pair<string, boost::shared_ptr<RatesEnvironment>> > getAllRatesEnvironments();

        boost::shared_ptr<sjdTools::ErrorTracking> errorTracking;

        boost::shared_ptr<Contract> contract;
        boost::shared_ptr<RatesEnvironment> baseRates;
    };
}

#endif

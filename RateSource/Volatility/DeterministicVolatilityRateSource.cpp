#include "DeterministicVolatilityRateSource.h"

namespace sjd 
{
    bool DeterministicVolatilityRateSource::isOK()
    {
        if (!VolatilityRateSource::isOK())
        {
            return false;
        }
        if (orderedForwardDates.size() > 0)
        {
            if (!isInRange(orderedForwardDates.front()))
            {
                errorTracking->populateErrorMessage("First forward date is outside the rate source range");
                return false;
            }
            if (!isInRange(orderedForwardDates.back()))
            {
                errorTracking->populateErrorMessage("Last forward date is outside the rate source range");
                return false;
            }
        }
        return true;
    }

    vector<pair<string, boost::shared_ptr<RateSource>>> DeterministicVolatilityRateSource::getMarketRateStresses()
    {
        vector<pair<string, boost::shared_ptr<RateSource>>> marketStress;
        marketStress.push_back(make_pair("Vol + 1% ", parallelBump(0.01)));
        return marketStress;
    }

    vector<boost::shared_ptr<VolatilityRateSource> > DeterministicVolatilityRateSource::rollForward()
    {
        vector<boost::shared_ptr<VolatilityRateSource> > volatilityRateSources;
        for (size_t i = 0; i < orderedForwardDates.size(); ++i) 
        {
            volatilityRateSources.push_back(rollForward(orderedForwardDates[i]));
        }
        return volatilityRateSources;
    }

    void DeterministicVolatilityRateSource::setForwardValuationDates(set<Date> dates)
    {
        orderedForwardDates = vector<Date>(dates.begin(), dates.end());
    }
}
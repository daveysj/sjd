#include "DeterministicDiscountRateSource.h"

namespace sjd 
{
    bool DeterministicDiscountRateSource::isOK()
    {
        if (!RateSource::isOK())
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

    vector<pair<string, boost::shared_ptr<RateSource>>> DeterministicDiscountRateSource::getMarketRateStresses()
    {
        boost::shared_ptr<QuantLib::DayCounter> dayCounter = boost::shared_ptr<QuantLib::DayCounter>(
            new Actual360()); 
        boost::shared_ptr<InterestRateConvention> irc = boost::shared_ptr<InterestRateConvention>(
            new InterestRateConvention(dayCounter, Compounded, Annual));
        vector<pair<string, boost::shared_ptr<RateSource>>> marketStress;
        marketStress.push_back(make_pair("DF + 1% NACA a/360", parallelBump(0.01, irc)));
        return marketStress;
    }

    vector<boost::shared_ptr<DiscountRateSource> > DeterministicDiscountRateSource::rollForward()
    {
        vector<boost::shared_ptr<DiscountRateSource> > discountRateSources;
        for (size_t i = 0; i < orderedForwardDates.size(); ++i) 
        {
            discountRateSources.push_back(rollForward(orderedForwardDates[i]));
        }
        return discountRateSources;
    }

    void DeterministicDiscountRateSource::setForwardValuationDates(set<Date> dates)
    {
        orderedForwardDates = vector<Date>(dates.begin(), dates.end());
    }
}
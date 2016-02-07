#include "ForwardRateSourceDeterministic.h"

namespace sjd 
{
    bool ForwardRateSourceDeterministic::isOK()
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

    vector<pair<string, boost::shared_ptr<RateSource>>> ForwardRateSourceDeterministic::getMarketRateStresses()
    {
        vector<pair<string, boost::shared_ptr<RateSource>>> marketStress;
        marketStress.push_back(make_pair("Fwd + 1unit", parallelBump(1.0)));
        return marketStress;
    }


    vector<boost::shared_ptr<ForwardRateSource> > ForwardRateSourceDeterministic::rollForward()
    {
        vector<boost::shared_ptr<ForwardRateSource> > frss;
        for (size_t i = 0; i < orderedForwardDates.size(); ++i) 
        {
            frss.push_back(rollForward(orderedForwardDates[i]));
        }
        return frss;
    }

    boost::shared_ptr<ForwardRateSource> ForwardRateSourceDeterministic::rollForward(Date toDate, 
                                                                                     vector<Date> rateSetDates,
                                                                                     vector<double> &rateSetsFromAnchorToDate)
    {
        rateSetsFromAnchorToDate.clear();
        for (size_t i = 0; i < rateSetDates.size(); ++i)
        {
            rateSetsFromAnchorToDate.push_back(getForward(rateSetDates[i]));
        }
        return rollForward(toDate);
    }

    vector<boost::shared_ptr<ForwardRateSource> > ForwardRateSourceDeterministic::rollForward(
                                                                    vector<Date> rateSetDates,                
                                                                    vector<double> &rateSetsFromAnchorToFinalRollDate)
    {
        rateSetsFromAnchorToFinalRollDate.clear();
        vector<boost::shared_ptr<ForwardRateSource> > frss;
        vector<double> previousRateSets;
        vector<double> rateSets;
        for (size_t i = 0; i < orderedForwardDates.size(); ++i) 
        {
            vector<Date>::iterator it = lower_bound(rateSetDates.begin(), rateSetDates.end(), orderedForwardDates[i]);
            vector<Date> dateSubset = vector<Date>(rateSetDates.begin(), it);
            frss.push_back(rollForward(orderedForwardDates[i], dateSubset, rateSets));
            rateSetsFromAnchorToFinalRollDate.insert(rateSetsFromAnchorToFinalRollDate.end(), 
                                                     rateSets.begin() + previousRateSets.size(), 
                                                     rateSets.end());
            previousRateSets = rateSets;
        }
        return frss;
    }

    void ForwardRateSourceDeterministic::setForwardValuationDates(set<Date> dates)
    {
        orderedForwardDates = vector<Date>(dates.begin(), dates.end());
    }
}
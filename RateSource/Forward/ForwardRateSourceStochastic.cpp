#include "ForwardRateSourceStochastic.h"

namespace sjd 
{
    void ForwardRateSourceStochastic::setVolitalityRateSource(boost::shared_ptr<VolatilityRateSource> vrsInput)
    {
        vrs = vrsInput;
    }

    bool ForwardRateSourceStochastic::isOK()
    {
        if (!ForwardRateSource::isOK())
        {
            return false;
        }
        if (forwardSimulationDates.size() > 0)
        {
            if (!isInRange(forwardSimulationDates.front()))
            {
                errorTracking->populateErrorMessage("First forward date is outside the rate source range");
                return false;
            }
            if (!isInRange(forwardSimulationDates.back()))
            {
                errorTracking->populateErrorMessage("Last forward date is outside the rate source range");
                return false;
            }
        }
        if (vrs == NULL)
        {
            errorTracking->populateErrorMessage("Volatility Rate Source has not been set");
            return false;
        }
        if (!vrs->extendsTo(finalDate))
        {
            errorTracking->populateErrorMessage("Volatility Rate Source does not extend to the end of the Forward Rate Source");
        }
        if (vrs->getAnchorDate() != anchorDate)
        {
            errorTracking->populateErrorMessage("Volatility Rate Source does not have the same anchor date as the Forward Rate Source");
        }
        return !errorTracking->getHasErrors();
    }
    
    void ForwardRateSourceStochastic::setForwardValuationDates(set<Date> datesSet)
    {
        forwardSimulationDates = vector<Date>(datesSet.begin(), datesSet.end());
    }
    
    void ForwardRateSourceStochastic::setRandomVariables(vector<double> normalRVsInput)
    {
        normalRVs = normalRVsInput;
    }

    boost::shared_ptr<ForwardRateSource> ForwardRateSourceStochastic::rollForward(Date toDate, 
                                                                                  double normalRV,
                                                                                  vector<Date> rateSetDates,
                                                                                  vector<double> &rateSetsFromAnchorToDate)
    {
        boost::shared_ptr<ForwardRateSource> rolledRate = rollForward(toDate, normalRV);

        double expectedFwd = getForward(toDate);
        double simulatedFwd = rolledRate->getForward(toDate);
        double ratio = (simulatedFwd - expectedFwd) / expectedFwd;
        rateSetsFromAnchorToDate.clear();
        double denominator = (double) (toDate - anchorDate);
        for (size_t i = 0; i < rateSetDates.size(); ++i)
        {
            double adjustedFwd = getForward(rateSetDates[i]) * (1 + ratio * ((double) (rateSetDates[i] - anchorDate)) / denominator);
            rateSetsFromAnchorToDate.push_back(adjustedFwd);
        }
        return rolledRate;
    }

    vector<boost::shared_ptr<ForwardRateSource> > ForwardRateSourceStochastic::rollForward(vector<Date> rateSetDates,
                                                                                           vector<double> &rateSetsFromAnchorToFinalRollDate)
    {
        vector<boost::shared_ptr<ForwardRateSource> > frss = rollForward();
        rateSetsFromAnchorToFinalRollDate = vector<double>(rateSetDates.size(), 0);
        Date startDate = anchorDate;
        Date endDate = forwardSimulationDates[0];
        vector<Date>::iterator startIt, endIt;

        endIt = upper_bound(rateSetDates.begin(), rateSetDates.end(), endDate);
        vector<Date> rateSetSubset = vector<Date>(rateSetDates.begin(), endIt);
        double expectedFwd = getForward(endDate);
        double simulatedFwd = frss[0]->getForward(endDate);
        double ratio = (simulatedFwd - expectedFwd) / expectedFwd;
        double denominator = (double) (endDate - startDate);
        size_t index = 0;
        for (size_t j = 0; j < rateSetSubset.size(); ++j)
        {
            double adjustedFwd = getForward(rateSetSubset[j]) * (1 + ratio * ((double) (rateSetSubset[j] - startDate)) / denominator);
            rateSetsFromAnchorToFinalRollDate[index] = (adjustedFwd);
            ++index;
        }

        for (size_t i = 1; i < forwardSimulationDates.size(); ++i)
        {
            startDate = endDate;
            endDate = forwardSimulationDates[i];
            startIt = endIt;
            endIt = upper_bound(rateSetDates.begin(), rateSetDates.end(), endDate);
            rateSetSubset = vector<Date>(startIt, endIt);
            
            expectedFwd = frss[i-1]->getForward(endDate);
            simulatedFwd = frss[i]->getForward(endDate);
            ratio = (simulatedFwd - expectedFwd) / expectedFwd;
            denominator = (double) (endDate - startDate);
            for (size_t j = 0; j < rateSetSubset.size(); ++j)
            {
                double adjustedFwd = frss[i-1]->getForward(rateSetSubset[j]);
                adjustedFwd *= (1 + ratio * ((double) (rateSetSubset[j] - startDate)) / denominator);
                rateSetsFromAnchorToFinalRollDate[index] = (adjustedFwd);
                ++index;
            }
        }        
        return frss;
    }

    vector<double> ForwardRateSourceStochastic::getImpliedRandomNumbers(vector<double> spot)
    {
        vector<double> expectedForwards = vector<double>(forwardSimulationDates.size());
        vector<double> sds = vector<double>(forwardSimulationDates.size());
        vector<double> rns = vector<double>(forwardSimulationDates.size());
        size_t startIndex = 0;
        if (forwardSimulationDates.front() == anchorDate)
        {
            rns[0] = 0;
            sds[0] = 0;
            expectedForwards[0] = spot[0];
            ++startIndex;
        }
        for (size_t i = startIndex; i < forwardSimulationDates.size(); ++i)
        {
            expectedForwards[i] = getForward(forwardSimulationDates[i]);
            double fwdFwd = expectedForwards[i] / expectedForwards[i - 1];

            sds[i] = vrs->getStandardDeviation(forwardSimulationDates[i]);
            double sd = sqrt((sds[i] * sds[i] - sds[i - 1] * sds[i - 1]));
            
            rns[i] = (log((spot[i]) / (spot[i-1] * fwdFwd)) + sd * sd / 2.0) / sd;
        }
        return rns;
    }

}
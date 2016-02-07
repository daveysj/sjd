#include "ForwardCurveCMAStochasticFwdVol.h"


namespace sjd 
{
    /*======================================================================================
    ForwardCurveCMAStochasticFwdVol 
    =======================================================================================*/
    ForwardCurveCMAStochasticFwdVol::ForwardCurveCMAStochasticFwdVol(Date anchorDateInput, 
                                                                       vector<Date> observationDatesInput, 
                                                                       vector<double> forwardsInput,                                                                        
                                                                       ArrayInterpolatorType typeInput,
                                                                       bool allowExtrapolationInput,
                                                                       boost::shared_ptr<VolatilityRateSource> vrs) 
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("ForwardCurveCMAStochasticFwdVol"));

        curve = boost::shared_ptr<CalendarMonthAverageCurve>(new CalendarMonthAverageCurve(anchorDateInput, 
                                                                                           observationDatesInput, 
                                                                                           forwardsInput,
                                                                                           typeInput,
                                                                                           allowExtrapolationInput));        
        anchorDate = curve->getAnchorDate();
        allowExtrapolation = curve->getAllowsExtrapolation();
        finalDate = curve->getFinalDate();
        string tmp;
        if (curve->isOK(tmp) && 
            !allowExtrapolation && 
            (!curve->containsRateOnAnchorDate()))
        {
            curve->addPointOnAnchorDate(forwardsInput.front());
        }
        setVolitalityRateSource(vrs);
        originalObservationDates = curve->getObservationDates();
        originalObservationRates = curve->getRates();
    }

    bool ForwardCurveCMAStochasticFwdVol::isOK()
    {
        if (!ForwardRateSourceStochastic::isOK())
        {
            return false;
        }
        string errorMessage = "";
        if (!curve->isOK(errorMessage))
        {
            errorTracking->populateErrorMessage(errorMessage);
            return false;
        }
        return !errorTracking->getHasErrors();
    }

    bool ForwardCurveCMAStochasticFwdVol::isInRange(Date d) const
    {
        Date beginningOfMonth = Date(1, d.month(), d.year());
        Date beginningOfAnchorDate = Date(1, anchorDate.month(), anchorDate.year());
        if (allowExtrapolation) 
        {
            return (beginningOfMonth >= beginningOfAnchorDate);
        }
        else 
        {
            return ((beginningOfMonth >= beginningOfAnchorDate) && (beginningOfMonth <= finalDate));
        }
    }

    bool ForwardCurveCMAStochasticFwdVol::extendsTo(Date date) const
    {
        Date beginningOfMonth = Date(1, date.month(), date.year());
        return RateSource::extendsTo(beginningOfMonth);
    }

    vector<Date> ForwardCurveCMAStochasticFwdVol::getObservationDates()
    {
        return curve->getObservationDates();
    }

    vector<double> ForwardCurveCMAStochasticFwdVol::getForwards()
    {
        return curve->getObservationPoints();
    }
    void ForwardCurveCMAStochasticFwdVol::setForwards(vector<double> updatedForwards)
    {
        curve->setRates(updatedForwards);
    }

    double ForwardCurveCMAStochasticFwdVol::getForward(Date toDate) const
    {
        return curve->getRate(toDate);
    }

    vector<double> ForwardCurveCMAStochasticFwdVol::getForward(vector<QuantLib::Date> toDates) const
    {
        if (toDates.size() < 1)
        {
            return vector<double>(0);
        }
        return curve->getRate(toDates);
    }

    double ForwardCurveCMAStochasticFwdVol::getForward(Month month, Year year) const
    {
        Date beginningOfMonth = Date(1, month, year);
        return curve->getRate(beginningOfMonth);
    }


    boost::shared_ptr<ForwardRateSource> ForwardCurveCMAStochasticFwdVol::rollForward(Date toDateInput, double normalRV)
    {       
        // Why am I doing this?
        //curve->setParameters(anchorDate, originalObservationDates, originalObservationRates, curve->getType(), curve->getAllowsExtrapolation());
        vector<Date> originalCurveDates = curve->getObservationDates();
        vector<Date> rolledObservationDates;
        vector<double> rolledFwds;
        if (!isInRange(toDateInput))
        {
            rolledObservationDates.push_back(toDateInput);
            rolledFwds.push_back(1.0);
        }
        else 
        {
            double sqrtDt = sqrt((toDateInput - anchorDate) / 365.0);
            Date startOfToDate = Date(01, toDateInput.month(), toDateInput.year());
            for (size_t i = 0; i < originalCurveDates.size(); ++i) 
            {
                if (originalCurveDates[i] >= startOfToDate)
                {
                    Date d1 = originalCurveDates[i];
                    rolledObservationDates.push_back(d1);
                    double sd = vrs->getVolatility(d1) * sqrtDt;
                    rolledFwds.push_back(getForward(d1) *  exp(-sd * sd / 2 + sd * normalRV));
                }
            }
        }
        return boost::shared_ptr<ForwardCurveCMADeterministic>(
                                    new ForwardCurveCMADeterministic(toDateInput, 
                                                                        rolledObservationDates, 
                                                                        rolledFwds,
                                                                        LINEAR,
                                                                        true));        
    } 

    boost::shared_ptr<ForwardRateSource> ForwardCurveCMAStochasticFwdVol::rollForward(Date toDate, 
                                                                                      double normalRV,
                                                                                      vector<Date> rateSetDates,
                                                                                      vector<double> &rateSetsFromAnchorToDate)
    {
        return ForwardRateSourceStochastic::rollForward(toDate, normalRV, rateSetDates, rateSetsFromAnchorToDate);
    }


    void ForwardCurveCMAStochasticFwdVol::setForwardValuationDates(set<Date> datesInput)
    {
        //curve->setParameters(anchorDate, originalObservationDates, originalObservationRates, curve->getType(), curve->getAllowsExtrapolation());
        //set<Date> allObservationDatesSet = set<Date>(originalObservationDates.begin(), originalObservationDates.end());
        vector<Date> startOfSimulationMonths;
        set<Date>::iterator it;
        for (it = datesInput.begin(); it != datesInput.end(); ++it)
        {
            Date startOfMonth = Date(1, (*it).month(), (*it).year());
            startOfSimulationMonths.push_back(startOfMonth);
        //    allObservationDatesSet.insert(startOfMonth);
        }
        //vector<Date> allObservationDates = vector<Date>(allObservationDatesSet.begin(), allObservationDatesSet.end());
        //vector<double> allObservationRates = vector<double>(allObservationDates.size());
        //for (size_t i = 0; i < allObservationRates.size(); ++ i)
        //{
        //    allObservationRates = getForward(allObservationDates);
        //}
        //curve->setParameters(anchorDate, allObservationDates, allObservationRates, curve->getType(), curve->getAllowsExtrapolation());

        //vector<Date> originalCurveDates = curve->getObservationDates();
        ForwardRateSourceStochastic::setForwardValuationDates(datesInput);
        if (forwardSimulationDates[0] < anchorDate ||
            forwardSimulationDates.back() > finalDate)
        {
            forwardSimulationDates = vector<Date>();
            return;
        }
        forwardIndexes = vector<size_t>(forwardSimulationDates.size(), 0);
        frss.clear();
        for (size_t i = 0; i < forwardSimulationDates.size(); ++i)
        {
            vector<Date> dateSubset = vector<Date>();
            vector<double> ratesSubset = vector<double>();
            bool indexFound = false;
            for (size_t j = 0; j < originalObservationDates.size(); ++j) 
            {
                if (originalObservationDates[j] >= startOfSimulationMonths[i])
                {
                    if (!indexFound) 
                    {
                        forwardIndexes[i] = j;
                        indexFound = true;
                    }
                    dateSubset.push_back(originalObservationDates[j]);
                    ratesSubset.push_back(getForward(originalObservationDates[j]));
                }
            }
            frss.push_back(boost::shared_ptr<ForwardCurveCMADeterministic>(
                                                new ForwardCurveCMADeterministic(forwardSimulationDates[i], 
                                                                                    dateSubset, 
                                                                                    ratesSubset,
                                                                                    LINEAR,
                                                                                    true)));
        }

        vector<Date> rolledCurveDates = frss[0]->getObservationDates();
        vector<double> sds = vector<double>(rolledCurveDates.size(), 0);
        double sqrtdt = sqrt((forwardSimulationDates[0] - anchorDate) / 365.0);

        for (size_t j = 0; j < rolledCurveDates.size(); ++j) 
        {
            sds[j] = vrs->getVolatility(rolledCurveDates[j]) * sqrtdt;
        }
        standardDeviations.clear();
        standardDeviations.push_back(sds);
        for (size_t i = 1; i < forwardSimulationDates.size(); ++i)
        {
            sqrtdt = sqrt((forwardSimulationDates[i] - forwardSimulationDates[i-1]) / 365.0);
            rolledCurveDates = frss[i]->getObservationDates();
            sds = vector<double>(rolledCurveDates.size(), 0);
            for (size_t j = 0; j < rolledCurveDates.size(); ++j) 
            {
                sds[j] = vrs->getVolatility(rolledCurveDates[j]) * sqrtdt;
            }
            standardDeviations.push_back(sds);
        }
    }

    vector<boost::shared_ptr<ForwardRateSource> > ForwardCurveCMAStochasticFwdVol::rollForward()
    {
        vector<boost::shared_ptr<ForwardRateSource> > returnFrss = vector<boost::shared_ptr<ForwardRateSource> >(frss.size());
        vector<double> fwds = curve->getRates();
        for (size_t i = 0; i < forwardSimulationDates.size(); ++i) 
        {
            size_t startIndex = forwardIndexes[i];
            for (size_t j = startIndex; j < fwds.size(); ++j)
            {
                double sd = standardDeviations[i][j- startIndex];
                fwds[j] *= exp(-sd * sd / 2 + sd * normalRVs[i]);
            }
            frss[i]->setForwards(vector<double>(fwds.begin() + forwardIndexes[i], fwds.end()));
            returnFrss[i] = frss[i];
        }
        return returnFrss;
    }

    vector<boost::shared_ptr<ForwardRateSource> > ForwardCurveCMAStochasticFwdVol::rollForward(vector<Date> rateSetDates,
                                                            vector<double> &rateSetsFromAnchorToFinalRollDate)
    {
        return ForwardRateSourceStochastic::rollForward(rateSetDates, rateSetsFromAnchorToFinalRollDate);
    }


    vector<double> ForwardCurveCMAStochasticFwdVol::getImpliedRandomNumbers(vector<double> spot)
    {
        vector<double> expectedForwards = vector<double>(forwardSimulationDates.size());
        vector<double> tmp = normalRVs;

        size_t startIndex = 0;
        if (forwardSimulationDates.front() == anchorDate)
        {
            normalRVs[0] = 0;
            ++startIndex;
        }
        else
        {
            expectedForwards[0] = getForward(forwardSimulationDates[0]);
            normalRVs[0] = log(spot[0] / expectedForwards[0]) / standardDeviations[0][0] + standardDeviations[0][0] / 2.0;
        }

        for (size_t i = 1; i < forwardSimulationDates.size(); ++i)
        {
            rollForward();
            expectedForwards[i] = frss[i-1]->getForward(forwardSimulationDates[i]);
            normalRVs[i] = log(spot[i] / expectedForwards[i]) / standardDeviations[i][0] + standardDeviations[i][0] / 2.0;
        }
        vector<double> rns = normalRVs;
        normalRVs = tmp;
        return rns;
    }


}
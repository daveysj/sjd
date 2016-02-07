#include "ForwardCurveStochasticFwdVol.h"

namespace sjd 
{

   /*======================================================================================
   ForwardCurveStochasticFwdVol 
    =======================================================================================*/
    ForwardCurveStochasticFwdVol::ForwardCurveStochasticFwdVol(Date anchorDateInput, 
                                                                 vector<Date> observationDatesInput, 
                                                                 vector<double> forwardsInput, 
                                                                 ArrayInterpolatorType typeInput,
                                                                 bool allowExtrapolationInput,
                                                                 boost::shared_ptr<VolatilityRateSource> vrs)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("ForwardCurveStochasticFwdVol"));
        setParameters(anchorDateInput, 
                      observationDatesInput, 
                      forwardsInput, 
                      typeInput,
                      allowExtrapolationInput,
                      vrs);
    }

    void ForwardCurveStochasticFwdVol::setParameters(Date anchorDateInput, 
                                                      vector<Date> observationDatesInput, 
                                                      vector<double> forwardsInput, 
                                                      ArrayInterpolatorType typeInput,
                                                      bool allowExtrapolationInput,                                           
                                                      boost::shared_ptr<VolatilityRateSource> vrs) 
    {
        datedCurve = boost::shared_ptr<DatedCurve>(new DatedCurve(anchorDateInput, 
                                                                  observationDatesInput, 
                                                                  forwardsInput,
                                                                  typeInput,
                                                                  allowExtrapolationInput));
        anchorDate = datedCurve->getAnchorDate();
        allowExtrapolation = datedCurve->getAllowsExtrapolation();
        finalDate = datedCurve->getFinalDate();
        string tmp;
        if (datedCurve->isOK(tmp) && 
            !allowExtrapolation && 
            (!datedCurve->containsRateOnAnchorDate()))
        {
            datedCurve->addPointOnAnchorDate(forwardsInput.front());
        }
        setVolitalityRateSource(vrs);
    }
    
    ForwardCurveStochasticFwdVol::~ForwardCurveStochasticFwdVol()
    {
    }
        
    bool ForwardCurveStochasticFwdVol::isOK()
    {
        if (!ForwardRateSourceStochastic::isOK())
        {
            return false;
        }
        string errorMessage = "";
        if (!datedCurve->isOK(errorMessage))
        {
            errorTracking->populateErrorMessage(errorMessage);
            return false;
        }
        return !errorTracking->getHasErrors();
    }

    double ForwardCurveStochasticFwdVol::getForward(Date toDate) const
    {
        return datedCurve->getRate(toDate);
    }


    boost::shared_ptr<ForwardRateSource> ForwardCurveStochasticFwdVol::rollForward(Date toDate, double normalRV)
    {
        vector<Date> rolledObservationDates;
        vector<double> rolledFwds;
        if (!isInRange(toDate))
        {
            rolledObservationDates.push_back(toDate);
            rolledFwds.push_back(1.0);
        }
        else 
        {
            double sqrtDt = sqrt((toDate - anchorDate) / 365.0);
            for (size_t i = 0; i < datedCurve->getObservationDates().size(); ++i) 
            {
                if (datedCurve->getObservationDates()[i] >= toDate)
                {
                    Date d1 = datedCurve->getObservationDates()[i];
                    rolledObservationDates.push_back(d1);
                    double sd = vrs->getVolatility(d1) * sqrtDt;
                    rolledFwds.push_back(getForward(d1) *  exp(-sd * sd / 2 + sd * normalRV));
                }
            }
        }
        return boost::shared_ptr<ForwardRateSource>(new ForwardCurveDeterministic(toDate, 
                                                                                rolledObservationDates, 
                                                                                rolledFwds,
                                                                                LINEAR,
                                                                                true));        
    }

    boost::shared_ptr<ForwardRateSource> ForwardCurveStochasticFwdVol::rollForward(Date toDate, 
                                                                                      double normalRV,
                                                                                      vector<Date> rateSetDates,
                                                                                      vector<double> &rateSetsFromAnchorToDate)
    {
        return ForwardRateSourceStochastic::rollForward(toDate, normalRV, rateSetDates, rateSetsFromAnchorToDate);
    }

    void ForwardCurveStochasticFwdVol::setForwardValuationDates(set<Date> datesInput)
    {
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
            vector<Date> datesSubset = vector<Date>();
            vector<double> ratesSubset = vector<double>();
            bool indexFound = false;
            for (size_t j = 0; j < datedCurve->getObservationDates().size(); ++j) 
            {
                if (datedCurve->getObservationDates()[j] >= forwardSimulationDates[i])
                {
                    if (!indexFound) 
                    {
                        forwardIndexes[i] = j;
                        indexFound = true;
                    }
                    Date d1 = datedCurve->getObservationDates()[j];
                    datesSubset.push_back(d1);
                    ratesSubset.push_back(getForward(d1));
                }
            }
            frss.push_back(boost::shared_ptr<ForwardCurveDeterministic>(new ForwardCurveDeterministic(forwardSimulationDates[i], 
                                                                                                datesSubset, 
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

    vector<boost::shared_ptr<ForwardRateSource> > ForwardCurveStochasticFwdVol::rollForward()
    {
        vector<boost::shared_ptr<ForwardRateSource> > returnFrss = vector<boost::shared_ptr<ForwardRateSource> >(frss.size());
        vector<double> fwds = datedCurve->getRates();
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

    vector<boost::shared_ptr<ForwardRateSource> > ForwardCurveStochasticFwdVol::rollForward(vector<Date> rateSetDates,
                                                            vector<double> &rateSetsFromAnchorToFinalRollDate)
    {
        return ForwardRateSourceStochastic::rollForward(rateSetDates, rateSetsFromAnchorToFinalRollDate);
    }

}
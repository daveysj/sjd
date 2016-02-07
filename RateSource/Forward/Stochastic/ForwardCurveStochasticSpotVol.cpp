#include "ForwardCurveStochasticSpotVol.h"

namespace sjd 
{

   /*======================================================================================
   ForwardCurveStochasticSpotVol 
    =======================================================================================*/
    ForwardCurveStochasticSpotVol::ForwardCurveStochasticSpotVol(Date anchorDateInput, 
                                                                 vector<Date> observationDatesInput, 
                                                                 vector<double> forwardsInput, 
                                                                 ArrayInterpolatorType typeInput,
                                                                 bool allowExtrapolationInput,
                                                                 boost::shared_ptr<VolatilityRateSource> vrs)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("ForwardCurveStochasticSpotVol"));
        setParameters(anchorDateInput, 
                      observationDatesInput, 
                      forwardsInput, 
                      typeInput,
                      allowExtrapolationInput,
                      vrs);
    }

    void ForwardCurveStochasticSpotVol::setParameters(Date anchorDateInput, 
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
    
    ForwardCurveStochasticSpotVol::~ForwardCurveStochasticSpotVol()
    {
    }
        
    bool ForwardCurveStochasticSpotVol::isOK()
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
        Date d1, d2;
        if (vrs->hasNegativeATMFwdFwdVariances(d1, d2))
        {
            stringstream errorDescription;
            errorDescription << "Negative fwd fwd variance between " << QuantLib::io::iso_date(d1)
                             << " and " << QuantLib::io::iso_date(d2);
            errorTracking->populateErrorMessage(errorDescription.str());
        }
        return !errorTracking->getHasErrors();
    }

    double ForwardCurveStochasticSpotVol::getForward(Date toDate) const
    {
        return datedCurve->getRate(toDate);
    }


    boost::shared_ptr<ForwardRateSource> ForwardCurveStochasticSpotVol::rollForward(Date toDate, double normalRV)
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
            double sd = vrs->getStandardDeviation(toDate);
            double multiplicativeFactor = exp( - (sd * sd) / 2 + sd * normalRV);
            for (size_t i = 0; i < datedCurve->getObservationDates().size(); ++i) 
            {
                if (datedCurve->getObservationDates()[i] >= toDate)
                {
                    Date d1 = datedCurve->getObservationDates()[i];
                    rolledObservationDates.push_back(d1);
                    rolledFwds.push_back(getForward(d1) *  multiplicativeFactor);
                }
            }
        }
        return boost::shared_ptr<ForwardRateSource>(new ForwardCurveDeterministic(toDate, 
                                                                                rolledObservationDates, 
                                                                                rolledFwds,
                                                                                LINEAR,
                                                                                true));        
    }

    boost::shared_ptr<ForwardRateSource> ForwardCurveStochasticSpotVol::rollForward(Date toDate, 
                                                                                    double normalRV,
                                                                                    vector<Date> rateSetDates,
                                                                                    vector<double> &rateSetsFromAnchorToDate)
    {
        return ForwardRateSourceStochastic::rollForward(toDate, normalRV, rateSetDates, rateSetsFromAnchorToDate);
    }

    void ForwardCurveStochasticSpotVol::setForwardValuationDates(set<Date> datesInput)
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
        standardDeviations = vector<double>(forwardSimulationDates.size());
        Date d2 = forwardSimulationDates[0];
        double sd2 = vrs->getStandardDeviation(d2);
        standardDeviations[0] = sd2;
        Date d1;
        double sd1;
        boost::shared_ptr<DayCounter> dc = vrs->getDayCounter();
        for (size_t i = 1; i < standardDeviations.size(); ++i)
        {
            d1 = d2;
            sd1 = sd2;
            sd2 = vrs->getStandardDeviation(forwardSimulationDates[0]);
            standardDeviations[i] = sqrt((sd2 * sd2 - sd1 * sd1) / dc->yearFraction(d1, d2));
        }
    }

    vector<boost::shared_ptr<ForwardRateSource> > ForwardCurveStochasticSpotVol::rollForward()
    {
        vector<boost::shared_ptr<ForwardRateSource> > returnFrss = vector<boost::shared_ptr<ForwardRateSource> >(frss.size());
        vector<double> fwds = datedCurve->getRates();
        for (size_t i = 0; i < forwardSimulationDates.size(); ++i) 
        {
            size_t startIndex = forwardIndexes[i];
            double sd = standardDeviations[i];
            for (size_t j = startIndex; j < fwds.size(); ++j)
            {
                fwds[j] *= exp(-sd * sd / 2 + sd * normalRVs[i]);
            }
            frss[i]->setForwards(vector<double>(fwds.begin() + forwardIndexes[i], fwds.end()));
            returnFrss[i] = frss[i];
        }
        return returnFrss;
    }

    vector<boost::shared_ptr<ForwardRateSource> > ForwardCurveStochasticSpotVol::rollForward(vector<Date> rateSetDates,
                                                            vector<double> &rateSetsFromAnchorToFinalRollDate)
    {
        return ForwardRateSourceStochastic::rollForward(rateSetDates, rateSetsFromAnchorToFinalRollDate);
    }

}
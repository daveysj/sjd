#include "ForwardCurveDeterministic.h"

namespace sjd 
{

   /*======================================================================================
   ForwardCurveDeterministic 
    =======================================================================================*/
    ForwardCurveDeterministic::ForwardCurveDeterministic(Date anchorDateInput, 
                                                         vector<Date> observationDatesInput, 
                                                         vector<double> forwardsInput, 
                                                         ArrayInterpolatorType typeInput,
                                                         bool allowExtrapolationInput)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("ForwardCurveDeterministic"));
        setParameters(anchorDateInput, 
                      observationDatesInput, 
                      forwardsInput, 
                      typeInput,
                      allowExtrapolationInput);
    }

    void ForwardCurveDeterministic::setParameters(Date anchorDateInput, 
                                                  vector<Date> observationDatesInput, 
                                                  vector<double> forwardsInput, 
                                                  ArrayInterpolatorType typeInput,
                                                  bool allowExtrapolationInput) 
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
    }
    
    bool ForwardCurveDeterministic::isOK()
    {
        ForwardRateSourceDeterministic::isOK();
        string errorMessage = "";
        if (!datedCurve->isOK(errorMessage))
        {
            errorTracking->populateErrorMessage(errorMessage);
            return false;
        }
        return !errorTracking->getHasErrors();
    }

    double ForwardCurveDeterministic::getForward(Date toDate) const
    {
        return datedCurve->getRate(toDate);
    }

    vector<Date> ForwardCurveDeterministic::getObservationDates()
    {
        return datedCurve->getObservationDates();
    }
    vector<double> ForwardCurveDeterministic::getForwards()
    {
        return datedCurve->getRates();
    }
    void ForwardCurveDeterministic::setForwards(vector<double> updatedForwards)
    {
        datedCurve->setRates(updatedForwards);
    }

    boost::shared_ptr<ForwardRateSource> ForwardCurveDeterministic::parallelBump(double spread)
    {
        vector<Date> newObservationDates = getObservationDates();
        vector<double> bumpedForwards; 
        for (size_t i = 0; i < newObservationDates.size(); ++i)
        {
            bumpedForwards.push_back(getForward(newObservationDates[i])+spread);
        }

        return boost::shared_ptr<ForwardCurveDeterministic>(new ForwardCurveDeterministic(anchorDate, 
                                                                newObservationDates, 
                                                                bumpedForwards, 
                                                                datedCurve->getType(),
                                                                getAllowsExtrapolation()));

    }

    boost::shared_ptr<ForwardRateSource> ForwardCurveDeterministic::rollForward(Date toDate)
    {
        vector<Date> rolledObservationDates;
        vector<double> rolledForwards; 
        if (!isInRange(toDate))
        {
            rolledObservationDates.push_back(toDate);
            rolledForwards.push_back(1.0);
        }
        else
        {
            for (size_t i = 0; i < getObservationDates().size(); ++i)
            {
                if (getObservationDates()[i] >= toDate) 
                {
                    rolledObservationDates.push_back(getObservationDates()[i]);
                    rolledForwards.push_back(getForward(getObservationDates()[i]));
                }
            }
        }
        return boost::shared_ptr<ForwardCurveDeterministic>(new ForwardCurveDeterministic(toDate, 
                                                                  rolledObservationDates, 
                                                                  rolledForwards, 
                                                                  datedCurve->getType(),
                                                                  getAllowsExtrapolation()));

    }

    boost::shared_ptr<ForwardRateSource> ForwardCurveDeterministic::rollForward(Date toDate, 
                                                                                vector<Date> rateSetDates,
                                                                                vector<double> &rateSetsFromAnchorToDate)
    {
        return ForwardRateSourceDeterministic::rollForward(toDate, rateSetDates, rateSetsFromAnchorToDate);
    }

    vector<boost::shared_ptr<ForwardRateSource> > ForwardCurveDeterministic::rollForward()
    {
        return ForwardRateSourceDeterministic::rollForward();
    }

    vector<boost::shared_ptr<ForwardRateSource> > ForwardCurveDeterministic::rollForward(vector<Date> rateSetDates,
                                                            vector<double> &rateSetsFromAnchorToFinalRollDate)
    {
        return ForwardRateSourceDeterministic::rollForward(rateSetDates, rateSetsFromAnchorToFinalRollDate);
    }
}
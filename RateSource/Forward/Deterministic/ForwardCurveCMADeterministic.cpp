#include "ForwardCurveCMADeterministic.h"


namespace sjd 
{
    /*======================================================================================
    ForwardCurveCMADeterministic 
    =======================================================================================*/
    ForwardCurveCMADeterministic::ForwardCurveCMADeterministic(Date anchorDateInput, 
                                                               vector<Date> observationDatesInput, 
                                                               vector<double> forwardsInput,                                                                        
                                                               ArrayInterpolatorType typeInput,
                                                               bool allowExtrapolationInput) 
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("ForwardCurveCMADeterministic"));
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
    }
   
    bool ForwardCurveCMADeterministic::isOK()
    {
        if (!ForwardRateSourceDeterministic::isOK())
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

    bool ForwardCurveCMADeterministic::isInRange(Date d) const
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

    bool ForwardCurveCMADeterministic::extendsTo(Date d) const
    {
        Date beginningOfMonth = Date(1, d.month(), d.year());
        return RateSource::extendsTo(beginningOfMonth);
    }

    vector<Date> ForwardCurveCMADeterministic::getObservationDates()
    {
        return curve->getObservationDates();
    }

    double ForwardCurveCMADeterministic::getForward(Date toDate) const
    {
        return curve->getRate(toDate);
    }

    vector<double> ForwardCurveCMADeterministic::getForward(vector<QuantLib::Date> toDates) const
    {
        if (toDates.size() < 1)
        {
            return vector<double>(0);
        }
        return curve->getRate(toDates);
    }


    double ForwardCurveCMADeterministic::getForward(Month month, Year year) const
    {
        Date beginningOfMonth = Date(1, month, year);
        return curve->getRate(beginningOfMonth);
    }

    void ForwardCurveCMADeterministic::setForwards(vector<double> updatedForwards)
    {
        curve->setRates(updatedForwards);
    }

    boost::shared_ptr<ForwardRateSource> ForwardCurveCMADeterministic::rollForward(Date toDate)
    {
        vector<Date> originalDates = curve->getObservationDates();
        vector<Date> rolledObservationDates;
        vector<double> rolledFwds;
        if (!isInRange(toDate))
        {
            rolledObservationDates.push_back(toDate);
            rolledFwds.push_back(1.0);
        }
        else 
        {
            for (size_t i = 0; i < originalDates.size(); ++i) 
            {
                if (originalDates[i] >= toDate)
                {
                    rolledObservationDates.push_back(originalDates[i]);
                    rolledFwds.push_back(getForward(originalDates[i]));
                }
            }
        }
        return boost::shared_ptr<ForwardCurveCMADeterministic>(
                                    new ForwardCurveCMADeterministic(toDate, 
                                                                   rolledObservationDates, 
                                                                   rolledFwds,
                                                                   LINEAR,
                                                                   true));        
    } 

    boost::shared_ptr<ForwardRateSource> ForwardCurveCMADeterministic::rollForward(Date toDate, 
                                                                 vector<Date> rateSetDates,
                                                                 vector<double> &rateSetsFromAnchorToDate)
    {
        return ForwardRateSourceDeterministic::rollForward(toDate, rateSetDates, rateSetsFromAnchorToDate);
    } 

    boost::shared_ptr<ForwardRateSource> ForwardCurveCMADeterministic::parallelBump(double spread)
    {
        vector<Date> originalDates = getObservationDates();
        vector<double> rolledFwds;
        for (size_t i = 0; i < originalDates.size(); ++i) 
        {
            rolledFwds.push_back(getForward(originalDates[i]) + spread);
        }
        return boost::shared_ptr<ForwardCurveCMADeterministic>(
                                    new ForwardCurveCMADeterministic(anchorDate, 
                                                                   originalDates, 
                                                                   rolledFwds,
                                                                   LINEAR,
                                                                   true));        
    }

    vector<boost::shared_ptr<ForwardRateSource> > ForwardCurveCMADeterministic::rollForward()
    {
        return ForwardRateSourceDeterministic::rollForward();
    }

    vector<boost::shared_ptr<ForwardRateSource> > ForwardCurveCMADeterministic::rollForward(vector<Date> rateSetDates,
                                                            vector<double> &rateSetsFromAnchorToFinalRollDate)
    {
        return ForwardRateSourceDeterministic::rollForward(rateSetDates, rateSetsFromAnchorToFinalRollDate);
    }

}
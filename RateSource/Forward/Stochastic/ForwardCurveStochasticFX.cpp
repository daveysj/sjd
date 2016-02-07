#include "ForwardCurveStochasticFX.h"

namespace sjd 
{

   /*======================================================================================
   ForwardCurveStochasticFX 
    =======================================================================================*/
    ForwardCurveStochasticFX::ForwardCurveStochasticFX(Date anchorDate, 
                                                       double spotInput,
                                                       Date spotDate,
                                                       boost::shared_ptr<DiscountRateSource> domesticDRS,
                                                       boost::shared_ptr<DiscountRateSource> foreignDRS,
                                                       boost::shared_ptr<VolatilityRateSource> vrs)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("ForwardCurveStochasticFX"));
        if (!domesticDRS->extendsTo(spotDate) || (!foreignDRS->extendsTo(spotDate)))
        {
            spotAtT0 = spotInput;
        }
        else 
        {
            spotAtT0 = spotInput * 
                       domesticDRS->getDiscountFactor(spotDate) / 
                       foreignDRS->getDiscountFactor(spotDate);
        }
        setParameters(anchorDate, 
                      spotAtT0,
                      domesticDRS, 
                      foreignDRS, 
                      vrs);
    }

    ForwardCurveStochasticFX::ForwardCurveStochasticFX(Date anchorDate, 
                                                       double spotAtT0Input,
                                                       boost::shared_ptr<DiscountRateSource> domesticDRS,
                                                       boost::shared_ptr<DiscountRateSource> foreignDRS,
                                                       boost::shared_ptr<VolatilityRateSource> vrs)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("ForwardCurveStochasticFX"));
        setParameters(anchorDate, 
                      spotAtT0,
                      domesticDRS, 
                      foreignDRS, 
                      vrs);
    }

    void ForwardCurveStochasticFX::setParameters(Date anchorDateInput, 
                                                 double spotInput,
                                                 boost::shared_ptr<DiscountRateSource> domesticDRSInput,
                                                 boost::shared_ptr<DiscountRateSource> foreignDRSInput,
                                                 boost::shared_ptr<VolatilityRateSource> vrsInput) 
    {
        setSpotAtT0(spotInput);
        setRateSources(domesticDRSInput, foreignDRSInput);
        anchorDate = anchorDateInput;       
        setVolitalityRateSource(vrsInput);
    }

    void ForwardCurveStochasticFX::setSpotAtT0(double spotAtT0Input)
    {
        spotAtT0 = spotAtT0Input;
    }

    double ForwardCurveStochasticFX::getSpotAtT0()
    {
        return spotAtT0;
    }

    void ForwardCurveStochasticFX::setRateSources(boost::shared_ptr<DiscountRateSource> domesticDRSInput,
                                        boost::shared_ptr<DiscountRateSource> foreignDRSInput)
    {
        domesticDRS = domesticDRSInput;
        foreignDRS = foreignDRSInput;
        allowExtrapolation = domesticDRS->getAllowsExtrapolation() && foreignDRS->getAllowsExtrapolation();
        finalDate = min(domesticDRS->getFinalDate(), foreignDRS->getFinalDate());
    }

    
    ForwardCurveStochasticFX::~ForwardCurveStochasticFX()
    {
    }
        
    bool ForwardCurveStochasticFX::isOK()
    {
        if (!ForwardRateSourceStochastic::isOK())
        {
            return false;
        }
        if (!domesticDRS->isOK())
        {
            errorTracking->populateErrorMessage(domesticDRS->getErrorMessages());
            return false;
        }
        if (domesticDRS->getAnchorDate() != anchorDate)
        {
            errorTracking->populateErrorMessage("Domestic DRS does not have the correct anchor date");
            return false;
        }
        if (!foreignDRS->isOK())
        {
            errorTracking->populateErrorMessage(foreignDRS->getErrorMessages());
            return false;
        }
        if (foreignDRS->getAnchorDate() != anchorDate)
        {
            errorTracking->populateErrorMessage("Foreign DRS does not have the correct anchor date");
            return false;
        }
        if (vrs->getAnchorDate() != anchorDate)
        {
            errorTracking->populateErrorMessage("Volatility Rate Source does not have the correct anchor date");
            return false;
        }
        return !errorTracking->getHasErrors();
    }

    double ForwardCurveStochasticFX::getForward(Date toDate) const
    {
        return spotAtT0 * 
               foreignDRS->getDiscountFactor(toDate) / 
               domesticDRS->getDiscountFactor(toDate);
    }


    boost::shared_ptr<ForwardRateSource> ForwardCurveStochasticFX::rollForward(Date toDate, double normalRV)
    {

        double rolledSpot = getForward(toDate);
        double sd = vrs->getStandardDeviation(toDate);
        rolledSpot *= exp(-sd * sd / 2 + sd * normalRV);

        vector<Date> rolledDRSDates = getRolledDatesAheadOf(toDate);;

        vector<double> domesticDF = vector<double>(rolledDRSDates.size());
        vector<double> foreignDF = vector<double>(rolledDRSDates.size());
        double domesticScalingFactor = domesticDRS->getDiscountFactor(toDate);
        double foreignScalingFactor = foreignDRS->getDiscountFactor(toDate);

        for (size_t i = 0; i < rolledDRSDates.size(); ++i)
        {
            domesticDF[i] = domesticDRS->getDiscountFactor(rolledDRSDates[i]) / domesticScalingFactor;
            foreignDF[i] = foreignDRS->getDiscountFactor(rolledDRSDates[i]) / foreignScalingFactor;
        }
        boost::shared_ptr<DiscountRateSource> rolledDomesticDRS = boost::shared_ptr<DiscountRateSource>(
            new DiscountCurve(toDate, rolledDRSDates, domesticDF, LINEAR, allowExtrapolation));
            
        boost::shared_ptr<DiscountRateSource> rolledForeighDRS = boost::shared_ptr<DiscountRateSource>(
            new DiscountCurve(toDate, rolledDRSDates, foreignDF, LINEAR, allowExtrapolation));
            
        return boost::shared_ptr<ForwardRateSource>(new ForwardCurveFX(toDate, 
                                                                       rolledSpot, 
                                                                       rolledDomesticDRS,
                                                                       rolledForeighDRS));   
    }

    boost::shared_ptr<ForwardRateSource> ForwardCurveStochasticFX::rollForward(Date toDate, 
                                                                               double normalRV,
                                                                               vector<Date> rateSetDates,
                                                                               vector<double> &rateSetsFromAnchorToDate)
    {
        return ForwardRateSourceStochastic::rollForward(toDate, normalRV, rateSetDates, rateSetsFromAnchorToDate);
    }

    void ForwardCurveStochasticFX::setForwardValuationDates(set<Date> datesInput)
    {
        ForwardRateSourceStochastic::setForwardValuationDates(datesInput);
        if (forwardSimulationDates[0] < anchorDate ||
            forwardSimulationDates.back() > finalDate)
        {
            forwardSimulationDates = vector<Date>();
            return;
        }

        frss.clear();
        standardDeviations.clear();

        for (size_t i = 0; i < forwardSimulationDates.size(); ++i)
        {
            double rolledSpot = getForward(forwardSimulationDates[i]);
            vector<Date> rolledDRSDates = getRolledDatesAheadOf(forwardSimulationDates[i]);
            vector<double> domesticDF = vector<double>(rolledDRSDates.size());
            vector<double> foreignDF = vector<double>(rolledDRSDates.size());
            for (size_t j = 0; j < rolledDRSDates.size(); ++j)
            {
                domesticDF[j] = domesticDRS->getDiscountFactor(rolledDRSDates[j]);
                foreignDF[j] = foreignDRS->getDiscountFactor(rolledDRSDates[j]);
            }
            boost::shared_ptr<DiscountRateSource> rolledDomesticDRS = boost::shared_ptr<DiscountRateSource>(
                new DiscountCurve(forwardSimulationDates[i], rolledDRSDates, domesticDF, LINEAR, allowExtrapolation));
            
            boost::shared_ptr<DiscountRateSource> rolledForeighDRS = boost::shared_ptr<DiscountRateSource>(
                new DiscountCurve(forwardSimulationDates[i], rolledDRSDates, foreignDF, LINEAR, allowExtrapolation));

            frss.push_back(boost::shared_ptr<ForwardCurveFX>(new ForwardCurveFX(forwardSimulationDates[i], 
                                                                                   rolledSpot, 
                                                                                   rolledDomesticDRS,
                                                                                   rolledForeighDRS)));
            standardDeviations.push_back(vrs->getStandardDeviation(forwardSimulationDates[i]));
        }
    }

    vector<boost::shared_ptr<ForwardRateSource> > ForwardCurveStochasticFX::rollForward()
    {
        vector<boost::shared_ptr<ForwardRateSource> > returnFrss = vector<boost::shared_ptr<ForwardRateSource> >(frss.size());
        for (size_t i = 0; i < forwardSimulationDates.size(); ++i) 
        {
            double bumpedSpot = getForward(forwardSimulationDates[i]);
            bumpedSpot *= exp(-standardDeviations[i] * standardDeviations[i] / 2 + standardDeviations[i] * normalRVs[i]);
            frss[i]->setSpotAtT0(bumpedSpot);
            returnFrss[i] = frss[i];
        }
        return returnFrss;
    }

    vector<boost::shared_ptr<ForwardRateSource> > ForwardCurveStochasticFX::rollForward(vector<Date> rateSetDates,
                                                                                        vector<double> &rateSetsFromAnchorToFinalRollDate)
    {
        return ForwardRateSourceStochastic::rollForward(rateSetDates, rateSetsFromAnchorToFinalRollDate);
    }

    vector<Date> ForwardCurveStochasticFX::getRolledDatesAheadOf(Date date)
    {
        vector<Date> datesToUse;
        int counter = 0;
        int numberOfSingleMonths = 6;
        Date tmpDate = date;
        while (tmpDate < finalDate && counter < numberOfSingleMonths)
        {
            datesToUse.push_back(tmpDate);
            tmpDate += 30;
        }
        int numberOfQuaters = 4;
        while (tmpDate < finalDate && counter < numberOfQuaters)
        {
            datesToUse.push_back(tmpDate);
            tmpDate += 92;
        }
        while (tmpDate < finalDate)
        {
            datesToUse.push_back(tmpDate);
            tmpDate += 365;
        }
        datesToUse.push_back(finalDate);
        return datesToUse;
   }
}
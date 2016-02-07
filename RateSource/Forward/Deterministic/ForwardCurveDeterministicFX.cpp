#include "ForwardCurveDeterministicFX.h"

namespace sjd 
{

    /*======================================================================================
    ForwardCurveDeterministicFX 
    =======================================================================================*/
    ForwardCurveDeterministicFX::ForwardCurveDeterministicFX(Date anchorDate, 
                                   double spot,
                                   Date spotDate,
                                   boost::shared_ptr<DiscountRateSource> domesticDRS,
                                   boost::shared_ptr<DiscountRateSource> foreignDRS)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("ForwardCurveDeterministicFX"));
        double sT0;
        if (!domesticDRS->extendsTo(spotDate) || (!foreignDRS->extendsTo(spotDate)))
        {
            sT0 = spot;
        }
        else 
        {
            sT0 = spot * 
                  domesticDRS->getDiscountFactor(spotDate) / 
                  foreignDRS->getDiscountFactor(spotDate);
        }
        setParameters(anchorDate, sT0, domesticDRS, foreignDRS);
    }

    ForwardCurveDeterministicFX::ForwardCurveDeterministicFX(Date anchorDate, 
                                                             double spotAtT0,
                                                             boost::shared_ptr<DiscountRateSource> domesticDRS,
                                                             boost::shared_ptr<DiscountRateSource> foreignDRS)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("ForwardCurveDeterministicFX"));
        setParameters(anchorDate, spotAtT0, domesticDRS, foreignDRS);
    }

    void ForwardCurveDeterministicFX::setParameters(Date anchorDateInput, 
                                                    double spotInput,
                                                    boost::shared_ptr<DiscountRateSource> domesticDRSInput,
                                                    boost::shared_ptr<DiscountRateSource> foreignDRSInput) 
    {
        setSpotAtT0(spotInput);
        setRateSources(domesticDRSInput, foreignDRSInput);
        anchorDate = anchorDateInput;       
    }

    void ForwardCurveDeterministicFX::setSpotAtT0(double spotAtT0Input)
    {
        spotAtT0 = spotAtT0Input;
    }

    double ForwardCurveDeterministicFX::getSpotAtT0()
    {
        return spotAtT0;
    }

    void ForwardCurveDeterministicFX::setRateSources(boost::shared_ptr<DiscountRateSource> domesticDRSInput,
                                                     boost::shared_ptr<DiscountRateSource> foreignDRSInput)
    {
        domesticDRS = domesticDRSInput;
        foreignDRS = foreignDRSInput;
        allowExtrapolation = domesticDRS->getAllowsExtrapolation() && foreignDRS->getAllowsExtrapolation();
        finalDate = min(domesticDRS->getFinalDate(), foreignDRS->getFinalDate());
    }

    
    ForwardCurveDeterministicFX::~ForwardCurveDeterministicFX()
    {
    }
        
    bool ForwardCurveDeterministicFX::isOK()
    {
        if (!ForwardRateSourceDeterministic::isOK())
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
        return !errorTracking->getHasErrors();
    }

    double ForwardCurveDeterministicFX::getForward(Date toDate) const
    {
        return spotAtT0 * 
               foreignDRS->getDiscountFactor(toDate) / 
               domesticDRS->getDiscountFactor(toDate);
    }

    boost::shared_ptr<ForwardRateSource> ForwardCurveDeterministicFX::parallelBump(double spread)
    {
        return  boost::shared_ptr<ForwardCurveFX>(
            new ForwardCurveFX(anchorDate, spotAtT0 + spread, domesticDRS, foreignDRS));
    }

    boost::shared_ptr<ForwardRateSource> ForwardCurveDeterministicFX::rollForward(Date toDate)
    {
        double rolledSpot = getForward(toDate);

        vector<Date> rolledDRSDates = getRolledDatesAheadOf(toDate);
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

    vector<Date> ForwardCurveDeterministicFX::getRolledDatesAheadOf(Date date)
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
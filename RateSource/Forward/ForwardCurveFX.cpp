#include "ForwardCurveFX.h"

namespace sjd 
{

    /*======================================================================================
    ForwardCurveFX 
    =======================================================================================*/
    ForwardCurveFX::ForwardCurveFX(Date anchorDate, 
                                   double spot,
                                   Date spotDate,
                                   boost::shared_ptr<DiscountRateSource> domesticDRS,
                                   boost::shared_ptr<DiscountRateSource> foreignDRS)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("ForwardCurveFX"));
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

    ForwardCurveFX::ForwardCurveFX(Date anchorDate, 
                                   double spotAtT0,
                                   boost::shared_ptr<DiscountRateSource> domesticDRS,
                                   boost::shared_ptr<DiscountRateSource> foreignDRS)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("ForwardCurveFX"));
        setParameters(anchorDate, spotAtT0, domesticDRS, foreignDRS);
    }

    void ForwardCurveFX::setParameters(Date anchorDateInput, 
                                       double spotInput,
                                       boost::shared_ptr<DiscountRateSource> domesticDRSInput,
                                       boost::shared_ptr<DiscountRateSource> foreignDRSInput) 
    {
        setSpotAtT0(spotInput);
        setRateSources(domesticDRSInput, foreignDRSInput);
        anchorDate = anchorDateInput;       
    }

    void ForwardCurveFX::setSpotAtT0(double spotAtT0Input)
    {
        spotAtT0 = spotAtT0Input;
    }

    double ForwardCurveFX::getSpotAtT0()
    {
        return spotAtT0;
    }

    void ForwardCurveFX::setRateSources(boost::shared_ptr<DiscountRateSource> domesticDRSInput,
                                        boost::shared_ptr<DiscountRateSource> foreignDRSInput)
    {
        domesticDRS = domesticDRSInput;
        foreignDRS = foreignDRSInput;
        allowExtrapolation = domesticDRS->getAllowsExtrapolation() && foreignDRS->getAllowsExtrapolation();
        finalDate = min(domesticDRS->getFinalDate(), foreignDRS->getFinalDate());
    }

    
    ForwardCurveFX::~ForwardCurveFX()
    {
    }
        
    bool ForwardCurveFX::isOK()
    {
        if (!ForwardRateSource::isOK())
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

    double ForwardCurveFX::getForward(Date toDate) const
    {
        return spotAtT0 * 
               foreignDRS->getDiscountFactor(toDate) / 
               domesticDRS->getDiscountFactor(toDate);
    }
}
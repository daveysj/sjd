#include "RatesEnvironment.h"

namespace sjd 
{
    RatesEnvironment::RatesEnvironment(Date anchorDateInput)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("RatesEnvironment"));
        anchorDate = anchorDateInput;
        finalDate = anchorDate;
        allowExtrapolation = false;
        frsSet = false;
        vrsSet = false;
        drsSet = false;
        foreignDrsSet = false;
        fixingrsSet = false;
    }

    bool RatesEnvironment::isOK()
    {
        resetErrorMessages();
        if (frsSet == false &&
            vrsSet == false &&
            drsSet == false &&
            foreignDrsSet == false &&
            fixingrsSet == false)
        {
            errorTracking->populateErrorMessage("No rates set");
            return false;
        }
        if (drsSet)
        {
            if (drs->getAnchorDate() != anchorDate)
            {
                errorTracking->populateErrorMessage("Discount rate source does not have the correct anchor date");
                return false;
            }
            if (!drs->isOK())
            {
                errorTracking->populateErrorMessage(drs->getErrorMessages());
                return false;
            }
        }
        if (foreignDrsSet)
        {
            if (foreigndrs->getAnchorDate() != anchorDate)
            {
                errorTracking->populateErrorMessage("Foreign discount rate source does not have the correct anchor date");
                return false;
            }
            if (!foreigndrs->isOK())
            {
                errorTracking->populateErrorMessage(foreigndrs->getErrorMessages());
                return false;
            }
        }
        if (frsSet)
        {
            if (frs->getAnchorDate() != anchorDate)
            {
                errorTracking->populateErrorMessage("Forward rate source does not have the correct anchor date");
                return false;
            }
            if (!frs->isOK())
            {
                errorTracking->populateErrorMessage(frs->getErrorMessages());
                return false;
            }
        }
        if (vrsSet)
        {
            if (vrs->getAnchorDate() != anchorDate)
            {
                errorTracking->populateErrorMessage("Volatility rate source does not have the correct anchor date");
                return false;
            }
            if (!vrs->isOK())
            {
                errorTracking->populateErrorMessage(vrs->getErrorMessages());
                return false;
            }
        }
        if (fixingrsSet)
        {
            if (!fixingrs->isOK())
            {
                errorTracking->populateErrorMessage(fixingrs->getErrorMessages());
                return false;
            }
        }
        return !(errorTracking->getHasErrors());
    }

    void RatesEnvironment::setFRS(boost::shared_ptr<ForwardRateSource> frsInput)
    {
        frs = frsInput;
        frsSet = true;
        updateDataAfterSettingRate();
    }

    bool RatesEnvironment::hasFRS()
    {
        return frsSet;
    }

    boost::shared_ptr<ForwardRateSource> RatesEnvironment::getFRS()
    {
        return frs;
    }

    void RatesEnvironment::setDRS(boost::shared_ptr<DiscountRateSource> drsInput)
    {
        drs = drsInput;
        drsSet = true;
        updateDataAfterSettingRate();
    }

    bool RatesEnvironment::hasDRS()
    {
        return drsSet;
    }

    boost::shared_ptr<DiscountRateSource> RatesEnvironment::getDRS()
    {
        return drs;
    }

    void RatesEnvironment::setForeignDRS(boost::shared_ptr<DiscountRateSource> foreigndrsInput)
    {
        foreigndrs = foreigndrsInput;
        foreignDrsSet = true;
        updateDataAfterSettingRate();
    }

    bool RatesEnvironment::hasForeignDRS()
    {
        return foreignDrsSet;
    }

    boost::shared_ptr<DiscountRateSource> RatesEnvironment::getForeignDRS()
    {
        return foreigndrs;
    }

    void RatesEnvironment::setVRS(boost::shared_ptr<VolatilityRateSource> vrsInput)
    {
        vrs = vrsInput;
        vrsSet = true;
        updateDataAfterSettingRate();
    }

    bool RatesEnvironment::hasVRS()
    {
        return vrsSet;
    }

    boost::shared_ptr<VolatilityRateSource> RatesEnvironment::getVRS()
    {
        return vrs;
    }

    void RatesEnvironment::setFixingRS(boost::shared_ptr<FixingRateSource> fixingrsInput)
    {
        fixingrs = fixingrsInput;
        fixingrsSet = true;
    }

    bool RatesEnvironment::hasFixingRS()
    {
        return fixingrsSet;
    }

    boost::shared_ptr<FixingRateSource> RatesEnvironment::getFixingRS()
    {
        return fixingrs;
    }

    void RatesEnvironment::updateDataAfterSettingRate()
    {
        allowExtrapolation = true;
        finalDate == anchorDate;
        set<Date> allFinalDates;
        if (drsSet)
        {
            allFinalDates.insert(drs->getFinalDate());
            allowExtrapolation = allowExtrapolation && drs->getAllowsExtrapolation();
        }
        if (foreignDrsSet)
        {
            allFinalDates.insert(foreigndrs->getFinalDate());
            allowExtrapolation = allowExtrapolation && foreigndrs->getAllowsExtrapolation();
        }
        if (frsSet)
        {
            allFinalDates.insert(frs->getFinalDate());
            allowExtrapolation = allowExtrapolation && frs->getAllowsExtrapolation();
        }
        if (vrsSet)
        {
            allFinalDates.insert(vrs->getFinalDate());
            allowExtrapolation = allowExtrapolation && vrs->getAllowsExtrapolation();
        }
        if (allFinalDates.size() > 0)
        {
            finalDate = *allFinalDates.begin();
        }        
    }
}
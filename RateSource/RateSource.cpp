#include "RateSource.h"

namespace sjd 
{

    void RateSource::resetErrorMessages()
    {
        errorTracking->clearErrorMessages();
    }

    bool RateSource::isOK()
    {
        resetErrorMessages();
        if (finalDate < anchorDate)  
        {
            errorTracking->populateErrorMessage("Anchor date after final date");
        }
        return !errorTracking->getHasErrors();
    }

    vector<string> RateSource::getErrorMessages() const 
    {
        return errorTracking->getErrorMessages();
    }

    string RateSource::getErrorMessagesAsString() const 
    {
        return errorTracking->getErrorMessagesAsString();
    }

    QuantLib::Date RateSource::getAnchorDate() const 
    {
        return anchorDate;
    }

    QuantLib::Date RateSource::getFinalDate() const 
    {
        return finalDate;
    }

    bool RateSource::getAllowsExtrapolation() const 
    {
        return allowExtrapolation;
    }

    bool RateSource::isInRange(QuantLib::Date date) const
    {
        if (allowExtrapolation) 
        {
            return (date >= anchorDate);
        }
        return ((date >= anchorDate) && (date <= finalDate));
    }

    bool RateSource::extendsTo(QuantLib::Date date) const
    {
        if (allowExtrapolation) 
        {
            return true;
        }
        return (date <= finalDate);
    }
}

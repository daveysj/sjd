#include "FixingRateSource.h"

namespace sjd 
{

    void FixingRateSource::setDatesAndRates(vector<Date> fixingDatesInput, vector<double> fixingRatesInput)
    {
        fixingDates = fixingDatesInput;
        fixingRates = fixingRatesInput;
        if (fixingDates.size() != 0) 
        {
            firstDate = fixingDates.front();
            lastDate =  fixingDates.back();
        }
    }

    bool FixingRateSource::isOK()
    {
        errorTracking->clearErrorMessages();
        if (fixingDates.size() != fixingRates.size())
        {
            errorTracking->populateErrorMessage("Number of Dates and Rates inputs are not equal");
        }
        if (fixingDates.size() == 0) 
        {
            errorTracking->populateErrorMessage("No fixing dates entered");
        }
        else 
        {
            if (!isStrictlyIncreasing<Date>(fixingDates))
            {
                errorTracking->populateErrorMessage("Input Dates are not strictly increasing");
            }
            if (firstDate != fixingDates.front())
            {
                errorTracking->populateErrorMessage("First Fixing Date does not match the first date in the fixing vector");
            }
        }
        return !errorTracking->getHasErrors();
    }

    bool FixingRateSource::containsARateSetOn(Date date) const
    {
        vector<Date>::const_iterator low;
        low = std::lower_bound (fixingDates.begin(), fixingDates.end(), date);
        if (low == fixingDates.cend())
        {
            return false;
        }
        return *low == date;
    }

    double FixingRateSource::getRateSetOn(Date date) const
    {
        vector<Date>::const_iterator low;
        low = std::lower_bound (fixingDates.begin(), fixingDates.end(), date);
        return fixingRates[low - fixingDates.begin()];
    }

    vector<string> FixingRateSource::getErrorMessages() const 
    {
        return errorTracking->getErrorMessages();
    }

    Date FixingRateSource::getEarliestDate() const 
    {
        return firstDate;
    }

    Date FixingRateSource::getMostRecentDate() const
    {
        return lastDate;
    }

    vector<double> FixingRateSource::getRateSetOn(vector<Date> toDates) const
    {
        vector<double> rates;
        for (size_t i = 0; i < toDates.size(); ++i)
        {
            rates.push_back(getRateSetOn(toDates[i]));
        }
        return rates;
    }

}

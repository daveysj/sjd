#include "Path.h"

namespace sjd 
{
    Path::Path(vector<boost::shared_ptr<RatesEnvironment>> ratesPathInput)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("Path"));
        ratesPath = ratesPathInput;
        pathDates.clear();
        for (size_t i = 0; i < ratesPath.size(); i++)
        {
            pathDates.push_back(ratesPath[i]->getAnchorDate());
        }
    }

    void Path::resetErrorMessages()
    {
        errorTracking->clearErrorMessages();
    }

    bool Path::isOK()
    {
        resetErrorMessages();
        if (ratesPath.size() != pathDates.size())
        {
            errorTracking->populateErrorMessage("number of rates does not match the number of dates");
            return false;
        }
        for (size_t i = 0; i < ratesPath.size(); i++)
        {
            if (ratesPath[i]->getAnchorDate() != pathDates[i])
            {
                errorTracking->populateErrorMessage("Rates anchor dates do not match path dates");
                return false;
            }
            if (!ratesPath[i]->isOK()) 
            {
                errorTracking->populateErrorMessage(ratesPath[i]->getErrorMessages());
            }
        }
        return !(errorTracking->getHasErrors());
    }

    vector<string> Path::getErrorMessage()
    {
        return errorTracking->getErrorMessages();
    }

    vector<Date> Path::getPathDates() const
    {
        return pathDates;
    }

    boost::shared_ptr<RatesEnvironment> Path::getRatesEnvironment(size_t i)
    {
        return ratesPath[i];
    }

    size_t Path::getNumberOfSteps() const
    {
        return pathDates.size();
    }

    vector<double> Path::getSpotRates()
    {
        vector<double> spot = vector<double>(ratesPath.size());
        vector<boost::shared_ptr<RatesEnvironment>>::iterator it;
        size_t i = 0;
        while (it != ratesPath.end())
        {
            Date d1 = ((*it)->getAnchorDate());
            spot[i] = ((*it)->getFRS())->getForward(d1);
            ++it;
            ++i;
        }
        return spot;
    }
}
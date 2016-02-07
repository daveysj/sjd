
#ifndef PATH_INCLUDED
#define Path_INCLUDED

#include <algorithm>
#include <sjd\RateSource\RatesEnvironment.h>

using namespace QuantLib;

namespace sjd 
{

    /*======================================================================================
    Path
    =======================================================================================*/
    class Path
    {
    public:
        Path() {};

        Path(vector<boost::shared_ptr<RatesEnvironment>> ratesPath);

        void resetErrorMessages();
        bool isOK();
        vector<string> getErrorMessage();

        boost::shared_ptr<RatesEnvironment> getRatesEnvironment(size_t i);  
        vector<Date>  getPathDates() const;
        size_t getNumberOfSteps() const;

        vector<double> getSpotRates();

    protected:

        boost::shared_ptr<sjdTools::ErrorTracking> errorTracking;

        vector<boost::shared_ptr<RatesEnvironment>> ratesPath;
        vector<Date> pathDates;
    };


}

#endif
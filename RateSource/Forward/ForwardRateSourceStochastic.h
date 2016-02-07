#ifndef SJD_STOCHASTICFORWARDRATESOURCE_INCLUDED
#define SJD_STOCHASTICFORWARDRATESOURCE_INCLUDED

#include <sjd\RateSource\Forward\ForwardRateSource.h>
#include <sjd\RateSource\Volatility\VolatilityRateSource.h>

#include <boost/enable_shared_from_this.hpp>

using namespace QuantLib;

namespace sjd 
{

    /*======================================================================================
    ForwardRateSourceStochastic 

    A stochastic forward rate source is a rate source which allows a user to roll the curve 
    to a future date with *one* source of randomness
    =======================================================================================*/
    class ForwardRateSourceStochastic : public ForwardRateSource
    {
    public:
        ForwardRateSourceStochastic() {};

        virtual bool isOK();

        void setVolitalityRateSource(boost::shared_ptr<VolatilityRateSource> vrs);


        virtual boost::shared_ptr<ForwardRateSource> rollForward(Date toDate, double normalRV) = 0;
        // rateSetDates are the sorted dates between the anchorDate and the "toDate" for
        // which we may require rate sets
        virtual boost::shared_ptr<ForwardRateSource> rollForward(Date toDate, 
                                                                 double normalRV,
                                                                 vector<Date> rateSetDates,
                                                                 vector<double> &rateSetsFromAnchorToDate);


        virtual void setForwardValuationDates(set<Date> dates); 
        virtual void setRandomVariables(vector<double> normalRVs); 
        virtual vector<boost::shared_ptr<ForwardRateSource> > rollForward() = 0;
        virtual vector<boost::shared_ptr<ForwardRateSource> > rollForward(vector<Date> rateSetDates,
                                                              vector<double> &rateSetsFromAnchorToFinalRollDate) = 0;

        // default implementation for independant moves
        virtual vector<double> getImpliedRandomNumbers(vector<double> spot);

    protected:

        vector<double> normalRVs;
        vector<Date> forwardSimulationDates;
        boost::shared_ptr<VolatilityRateSource> vrs;

    };

}

#endif
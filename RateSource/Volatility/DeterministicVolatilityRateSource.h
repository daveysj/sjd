#ifndef SJD_DETERMINISTICVOLATILITYSOURCE_INCLUDED
#define SJD_DETERMINISTICVOLATILITYSOURCE_INCLUDED

#include <sjd\RateSource\Volatility\VolatilityRateSource.h>
#include <sjd\RateSource\IHaveMarketRateStresses.h>


using namespace QuantLib;

namespace sjd 
{
    /*======================================================================================
    DeterminisiticVolatilityRateSource 
    
    A deterministic volatility rate source is a rate source with market stresses which allows 
    a user to:
        - bump (arithmetically) the source by a fixed volatility at all observation points; and 
        - roll the curve to a future date
    =======================================================================================*/
    class DeterministicVolatilityRateSource : public VolatilityRateSource,
                                              public IHaveMarketRateStresses
    {
    public:
        virtual bool isOK();

        virtual vector<pair<string, boost::shared_ptr<RateSource>>> getMarketRateStresses();
        virtual boost::shared_ptr<VolatilityRateSource> parallelBump(double spread) = 0;

        // Roll forward to one date
        virtual boost::shared_ptr<VolatilityRateSource> rollForward(Date toDate) = 0;
        // Roll forward to multiple dates. This is a two-step process (set dates then roll) 
        // provide an interface that is consistent with stochastic rates where the use case 
        // is Monte-Carlo simulation (set dates once, roll many times)
        virtual void setForwardValuationDates(set<Date> dates); 
        virtual vector<boost::shared_ptr<VolatilityRateSource> > rollForward();

    protected:
        vector<Date> orderedForwardDates;
    };

}

#endif
#ifndef SJD_DETERMINISTICDISCOUNTRATESOURCE_INCLUDED
#define SJD_DETERMINISTICDISCOUNTRATESOURCE_INCLUDED

#include <sjd\RateSource\Discount\DiscountRateSource.h>
#include <ql\time\daycounters\actual360.hpp>
#include <sjd\RateSource\IHaveMarketRateStresses.h>
#include <sjd\RateSource\Discount\InterestRateConvention.h>

using namespace QuantLib;

namespace sjd 
{

    /*======================================================================================
    DeterminisiticRateSource 
    
    A deterministic discount rate source is a rate source with market stresses which allows 
    a user to:
        - bump the curve by a fixed interest rate at all observation points; and 
        - roll the curve to a future date by preserving the forward forward discount factors
    =======================================================================================*/
    class DeterministicDiscountRateSource : public DiscountRateSource, 
                                            public IHaveMarketRateStresses
    {
    public:
        virtual bool isOK();

        virtual vector<pair<string, boost::shared_ptr<RateSource>>> getMarketRateStresses();
        virtual boost::shared_ptr<DiscountRateSource> parallelBump(double spread,
                                                                   boost::shared_ptr<InterestRateConvention> irc) = 0;

        // Roll forward to one date
        virtual boost::shared_ptr<DiscountRateSource> rollForward(Date toDate) = 0;
        // Roll forward to multiple dates. This is a two-step process (set dates then roll) 
        // provide an interface that is consistent with stochastic rates where the use case 
        // is Monte-Carlo simulation (set dates once, roll many times). The "set" structure 
        // ensures the dates are ordered and contains no duplicates. The user *must* check 
        // all the dates are in an appropriate range by calling the isOK() method after 
        // setting these dates.
        virtual void setForwardValuationDates(set<Date> dates); 
        virtual vector<boost::shared_ptr<DiscountRateSource> > rollForward();

    protected:
        vector<Date> orderedForwardDates;
    };

}

#endif
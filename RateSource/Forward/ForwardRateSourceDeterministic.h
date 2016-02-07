#ifndef SJD_DETERMINISTICFORWARDRATESOURCE_INCLUDED
#define SJD_DETERMINISTICFORWARDRATESOURCE_INCLUDED

#include <sjd\RateSource\Forward\ForwardRateSource.h>
#include <sjd\RateSource\IHaveMarketRateStresses.h>


using namespace QuantLib;

namespace sjd 
{
    /*======================================================================================
    DeterminsitcForwardRateSource 
    
    A deterministic forward rate source is a rate source with market stresses which allows 
    a user to:
        - bump the curve by a fixed amount (positive or negative) at all observation points. 
            It is up to the user to ensure negative amounts do not make observation points in 
            the bumped curve negative because there are no default checks for this; and 
        - roll the curve to a future date
    =======================================================================================*/
    class ForwardRateSourceDeterministic : public ForwardRateSource,
                                           public IHaveMarketRateStresses
    {
    public:
        virtual bool isOK();

        virtual vector<pair<string, boost::shared_ptr<RateSource>>> getMarketRateStresses();
        virtual boost::shared_ptr<ForwardRateSource> parallelBump(double spread) = 0;

        // Roll forward to one date
        virtual boost::shared_ptr<ForwardRateSource> rollForward(Date toDate) = 0;
        // For rates environments that have fixing rate sources we need the ability to 
        // populate the fixing rates while rolling forward. This is achieved by passing an 
        // input vector of sorted, increasing rate set dates between the anchor date and 
        // the “rolledTo” date and getting back the “simulated” rate sets
        virtual boost::shared_ptr<ForwardRateSource> rollForward(Date toDate, 
                                                                 vector<Date> rateSetDates,
                                                                 vector<double> &rateSetsFromAnchorToDate);
        // Roll forward to multiple dates. This is a two-step process (set dates then roll) 
        // provide an interface that is consistent with stochastic rates where the use case 
        // is Monte-Carlo simulation (set dates once, roll many times). The "set" structure 
        // ensures the dates are ordered and contains no duplicates. The user *must* check 
        // all the dates are in an appropriate range by calling the isOK() method after 
        // setting these dates.
        virtual void setForwardValuationDates(set<Date> dates); 
        virtual vector<boost::shared_ptr<ForwardRateSource> > rollForward();
        // When rolling forward to multiple dates using this method, the input rate set dates 
        // should be all the rate set dates between the anchor date and the *final* date to 
        // which the curve will be rolled, i.e. the final “orderedForwardDates”
        virtual vector<boost::shared_ptr<ForwardRateSource> > rollForward(vector<Date> rateSetDates,
                                                                          vector<double> &rateSetsFromAnchorToFinalRollDate);
    protected:
        vector<Date> orderedForwardDates;
    };
}

#endif
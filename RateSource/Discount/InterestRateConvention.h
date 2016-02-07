#ifndef SJD_INTERESTRATECONVENTION_INCLUDED
#define SJD_INTERESTRATECONVENTION_INCLUDED

#include <ql\time\daycounter.hpp>
#include <ql\compounding.hpp>
#include <ql\time\frequency.hpp>
#include <boost\shared_ptr.hpp>

using namespace QuantLib;

namespace sjd 
{
   /*======================================================================================
    InterestRateConvention

    Uses Quantlib structures and encapsulates all the information necessary to use a number 
    (double) as an interest rate. The class also contains some basic logic to convert 
    interest rates into discount factors and vice-versa
    =======================================================================================*/
    class InterestRateConvention
    {
    public:
        InterestRateConvention(boost::shared_ptr<DayCounter> dayCounter, 
                               Compounding compounding, 
                               Frequency frequency);
        
        boost::shared_ptr<DayCounter> getDayCounter();
        Compounding getCompounding();
        Frequency getFrequency();

        double dfToRate(double df, Date fromDate, Date toDate);

        double rateToDF(double rate, Date fromDate, Date toDate);

    protected:
        boost::shared_ptr<DayCounter> dayCounter;
        Compounding compounding;
        Frequency frequency;
    };
}

#endif
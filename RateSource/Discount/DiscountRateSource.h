#ifndef SJD_DISCOUNTINGRATESOURCE_INCLUDED
#define SJD_DISCOUNTINGRATESOURCE_INCLUDED
#pragma once


#include <sjd\RateSource\RateSource.h>

#include <ql\compounding.hpp>
#include <ql\time\frequency.hpp>

using namespace QuantLib;
using namespace std;

namespace sjd 
{
   /*======================================================================================
   DiscountRateSource 
    
    The base class for any interest rate curve. DiscountRateSource's are used to move 
    cashflows in time (present value or future value cash flows). Typical examples include:
        zero curves, 
        discount curves, 
        stripped interest rate curves etc

    Note: When inheriting from this class it will often be the case that the user will 
        supply input data with an observation point on the anchor date *missing*. Please 
        ensure that implementing classes insure the discount factor to the anchor date is 
        1.0;
    =======================================================================================*/
    class DiscountRateSource : public RateSource
    {
        
    public:
        virtual ~DiscountRateSource() {};

        // NB Dates assumed to be good. Call isInRange(...) before calling this
        virtual double getDiscountFactor(Date toDate) const = 0;
        virtual double getDiscountFactor(Date fromDate, Date toDate) const;
        virtual vector<double> getDiscountFactor(vector<QuantLib::Date> times) const;
        virtual vector<double> getDiscountFactor(Date fromTime, vector<Date> times) const;
    };
}

#endif
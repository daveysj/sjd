#ifndef ASIANFORWARDCONTRACT_INCLUDED
#define ASIANFORWARDCONTRACT_INCLUDED
#pragma once

#include "EuropeanForwardContract.h"

using namespace QuantLib;

namespace sjd 
{

   /*======================================================================================
   AsianForward

    An Asian or average rate option with a generic averaging period
    =======================================================================================*/
    class AsianForward : public AsianContract, public ForwardContract
    {
    public:
        AsianForward(set<Date> averagingDates, 
                     Date settlement, 
                     double strike, 
                     double volume,
                     BuySell buySell);

        AsianForward(Calendar calendar, 
                     Date startOfAveraging, 
                     Date endOfAveraging, 
                     Date settlement, 
                     double strike, 
                     double volume,
                     BuySell buySell);

        AsianForward(AsianForward &forward);

        virtual AsianForward* clone();

        virtual ~AsianForward() {};

        virtual void validate();
        virtual void validate(boost::shared_ptr<RatesEnvironment> re);

        virtual void setAveragingDates(set<Date> averagingDates); 

        virtual double getValue(boost::shared_ptr<RatesEnvironment> re);
        // Overwrite to ensure fwdFwdFactors are Multiplicative to the *average* of the Averaging Rates
        virtual void getFixingAndForwardRates(Date valuationDate, 
                                              vector<double> &fixingRates, 
                                              vector<double> &forwardRates,
                                              boost::shared_ptr<RatesEnvironment> re);

        virtual string toStringRates(boost::shared_ptr<RatesEnvironment> re);

    protected:
        void constructorCommon(set<Date> averagingDates, 
                               Date settlement, 
                               double strike, double volume,
                               BuySell buySell);
    };
    
}

#endif


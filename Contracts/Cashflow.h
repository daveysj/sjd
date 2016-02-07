#ifndef CASHFLOWCONTRACT_INCLUDED
#define CASHFLOWCONTRACT_INCLUDED
#pragma once

#include "Contracts.h"
#include <sjd\maths\maths.h>

using namespace QuantLib;

namespace sjd 
{

   /*======================================================================================
   FixedCashflow

    We need to have a Contract for each individual cashflow. This is becuase when trying to
    calcuate regulatory capital we will be grouping contracts by their settlement date and 
    this would be difficult if we booked multiple cashflows in a SingleInstrumentContract
    =======================================================================================*/
    class FixedCashflow : public SingleInstrumentContract
    {
    public:
        FixedCashflow(Date cashFlowDate, double volume, BuySell buySellAmount);

        FixedCashflow(FixedCashflow &cf);

        virtual FixedCashflow* clone();

        void commonConstructor(Date cashFlowDate, 
                               double volume, 
                               BuySell buySellAmount);

        virtual ~FixedCashflow() {};

        virtual void validate();
        virtual void validate(boost::shared_ptr<RatesEnvironment> re);
        
        // Cash flows have no premium so these will set and return 0;
        virtual void setPremiumAmount(double premium);

        // These always need to be the same
        virtual void setMaturityDate(Date maturityDate);
        virtual void setSettlementDate(Date settlementDate);

        virtual double getValue(boost::shared_ptr<RatesEnvironment> re);
        virtual double getPayoff(double forSpot);

    };

}

#endif


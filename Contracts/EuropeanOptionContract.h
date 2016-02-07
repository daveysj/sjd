#ifndef OPTIONCONTRACT_INCLUDED
#define OPTIONCONTRACT_INCLUDED
#pragma once

#include <boost\math\special_functions\round.hpp>
#include <sjd/OptionPricing/Black76Formula.h>

#include "Contracts.h"


using namespace QuantLib;

namespace sjd {

   /*======================================================================================
   EuropeanOption

    The simplest Black-Scholes Option Contract used to price Black options on futures or
    Black Scholes options. This code will use the 2nd volatility dimension if the vol rate
   source is a surface.
    =======================================================================================*/
    class EuropeanOption : public SingleOptionContract 
    {
    public:
        // BuySell refers to the underlying option. Premium will have the opposite sign
        EuropeanOption(Date optionMaturity, 
                       Date optionSettlement, 
                       double strike, 
                       PutCall type, 
                       double volume,
                       Date premiumPaymentDate, 
                       double premiumAmount, 
                       BuySell buySell);        

        EuropeanOption(EuropeanOption &option);
        virtual EuropeanOption* clone();
        virtual ~EuropeanOption() {};

        virtual double getOptionValue(boost::shared_ptr<RatesEnvironment> re);

        virtual void setType(PutCall pc);

   protected:
        void constructorCommon(Date optionMaturity, 
                               Date optionSettlement, 
                               double strike, 
                               PutCall type, 
                               double volume,
                               Date premiumPaymentDate, 
                               double premiumAmount, 
                               BuySell buySell);

       boost::shared_ptr<Black76Option> option;
    };

}

#endif


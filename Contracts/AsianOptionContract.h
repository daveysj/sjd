#ifndef ASIANOPTIONCONTRACT_INCLUDED
#define ASIANOPTIONCONTRACT_INCLUDED
#pragma once

#include <boost\math\special_functions\round.hpp>
#include <sjd\OptionPricing\AsianOptionFormula.h>
#include <sjd\RateSource\Fixing\FixingRateSource.h>
#include <ql\time\calendar.hpp>

#include "EuropeanOptionContract.h"

using namespace QuantLib;

namespace sjd {

   /*======================================================================================
   AsianOption
    
    An Asian or average rate option with a generic averaging period. Coded using ATM vols
    =======================================================================================*/
    class AsianOption : public SingleOptionContract, public AsianContract
    {
    public:
        AsianOption(set<Date> averagingDates, 
                           Date optionSettlement, 
                           double strike, 
                           PutCall type, 
                           double volume,
                           Date premiumPaymentDate, 
                           double premiumAmount, 
                           BuySell buySell);

        AsianOption(Calendar calendar, 
                    Date startOfAveraging, 
                    Date endOfAveraging, 
                    Date optionSettlement, 
                    double strike, 
                    PutCall type, 
                    double volume,
                    Date premiumPaymentDate, 
                    double premiumAmount, 
                    BuySell buySell);

        AsianOption(AsianOption &option);

        virtual AsianOption* clone();

        virtual ~AsianOption() {};

        virtual void validate(); 
        virtual void validate(boost::shared_ptr<RatesEnvironment> re);

        virtual void setAveragingDates(set<Date> averagingDates); 
        virtual void setType(PutCall pc);

        //virtual double getOptionValue(Date valueDate, vector<double> sds);
        virtual double getOptionValue(boost::shared_ptr<RatesEnvironment> re);

        virtual void getRates(Date valuationDate, 
                              vector<double> &fixingRates, 
                              vector<double> &forwardRates,
                              vector<double> &sd, 
                              vector<double> &vol,
                              boost::shared_ptr<RatesEnvironment> re);

        virtual string toStringRates(boost::shared_ptr<RatesEnvironment> re);

    protected:
        void constructorCommon(set<Date> averagingDates, 
                               Date optionSettlement, 
                               double strike, 
                               PutCall type, 
                               double volume,
                               Date premiumPaymentDate, 
                               double premiumAmount, 
                               BuySell buySell);
        

       boost::shared_ptr<AverageRateOption> option;
    };
}

#endif

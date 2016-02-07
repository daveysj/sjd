#include "Cashflow.h"


namespace sjd 
{

   /*======================================================================================
   FixedCashflow

    =======================================================================================*/
    void FixedCashflow::commonConstructor(Date cashFlowDate, 
                                          double amount, 
                                          BuySell bs)
    {
      errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("FixedCashflow"));
        setMaturityDate(cashFlowDate);
        setSettlementDate(cashFlowDate);
        setVolume(abs(amount));
        setBuySell(bs);
        setPremiumPaymentDate(cashFlowDate);
        setPremiumAmount(0);
    }


    FixedCashflow::FixedCashflow(Date cashFlowDate, double amount, BuySell bs)
    {
        commonConstructor(cashFlowDate, amount, bs);
    }

    FixedCashflow::FixedCashflow(FixedCashflow &cf)
    {
        commonConstructor(cf.getMaturityDate(), cf.getVolume(), cf.getBuySell());
    }

    FixedCashflow* FixedCashflow::clone()
    {
        FixedCashflow *cf = new FixedCashflow(*this);
        return cf;
    }

    void FixedCashflow::validate()
    {
        SingleInstrumentContract::validate();
        if (getMaturityDate() != getSettlementDate())
      {
            errorTracking->populateErrorMessage("Maturity Date must equal Settlement Date");
      }
    }

    void FixedCashflow::validate(boost::shared_ptr<RatesEnvironment> re)
    {
        SingleInstrumentContract::validate(re);
    }

    void FixedCashflow::setMaturityDate(Date maturityDate)
    {
        SingleInstrumentContract::setMaturityDate(maturityDate);
        SingleInstrumentContract::setSettlementDate(maturityDate);
    }

    void FixedCashflow::setSettlementDate(Date settlementDate)
    {
        SingleInstrumentContract::setMaturityDate(settlementDate);
        SingleInstrumentContract::setSettlementDate(settlementDate);
    }

    void FixedCashflow::setPremiumAmount(double premium)
    {
        premiumAmount = 0;
    }

    double FixedCashflow::getValue(boost::shared_ptr<RatesEnvironment> re)
    {
        Date valueDate = re->getAnchorDate();
        if (valueDate > settlementDate)
        {
            return 0;
        }
        double pv = 0;
        double pvCashFlows = volume * (re->getDRS())->getDiscountFactor(settlementDate);
        return buySellAsDouble * (pvCashFlows);
    }

    double FixedCashflow::getPayoff(double forSpot)
    {
        return buySellAsDouble * volume;
    }

}


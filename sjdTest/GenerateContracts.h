#ifndef generatecontracts_test
#define generatecontracts_test

#include <iostream>

#include <sjd\Contracts\EuropeanForwardContract.h>
#include <sjd\Contracts\AsianForwardContract.h>
#include <sjd\Contracts\EuropeanOptionContract.h>
#include <sjd\Contracts\AsianOptionContract.h>
#include <sjd\Contracts\Cashflow.h>

#include <ql\time\daycounter.hpp>
#include <ql\time\daycounters\all.hpp>
#include <ql\time\calendars\all.hpp>

using namespace sjd;


class GenerateContracts {
  public:

    static boost::shared_ptr<FixedCashflow> getDefaultFixedCashflow(Date anchorDate, int yearsToSettlement, double volume, BuySell bs);

    static boost::shared_ptr<EuropeanForward> getDefaultEuropeanForward(Date anchorDate, int yearsToSettlement, double strike, double volume, BuySell bs);    
    static boost::shared_ptr<EuropeanOption> getDefaultEuropeanOption(Date anchorDate, int yearsToSettlement, double strike, double volume, BuySell bs, PutCall pc, double premium, bool dp = false);

    static boost::shared_ptr<AsianForward> getDefaultAsianForward(Date anchorDate, int yearsToSettlement, int numberOfAveragingDates, double strike, double volume, BuySell bs);
    static boost::shared_ptr<AsianOption> getDefaultAsianOption(Date anchorDate,  int yearsToSettlement, int numberOfAveragingDates, double strike, double volume, BuySell bs, PutCall pc, double premium, bool dp = false);

    static boost::shared_ptr<FixedCashflow> getDefaultFixedCashflowWithPremium(Date anchorDate, int yearsToSettlement, double volume, BuySell bs);

};

#endif


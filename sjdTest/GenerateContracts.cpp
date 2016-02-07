#include "GenerateContracts.h"


using namespace sjd;

boost::shared_ptr<FixedCashflow> GenerateContracts::getDefaultFixedCashflow(Date anchorDate, int yearsToSettlement, double volume, BuySell bs)
{
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());
    Date cashFlowDate = nullCalendar->advance(anchorDate, yearsToSettlement, Years);
    Date premiumDate = cashFlowDate;

    return boost::shared_ptr<FixedCashflow>(new  FixedCashflow(cashFlowDate, volume, bs));
}


boost::shared_ptr<EuropeanForward> GenerateContracts::getDefaultEuropeanForward(Date anchorDate, int yearsToSettlement, double strike, double volume, BuySell bs)
{
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());
    Date settlement = nullCalendar->advance(anchorDate, yearsToSettlement, Years);
    Date rateSetDate = nullCalendar->advance(settlement, -2, Days);

    return boost::shared_ptr<EuropeanForward>(new  EuropeanForward(rateSetDate, settlement, abs(strike), abs(volume), bs));
}

boost::shared_ptr<AsianForward> GenerateContracts::getDefaultAsianForward(Date anchorDate, int yearsToSettlement, int numberOfAveragingDates, double strike, double volume, BuySell bs)
{
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());
    Date settlement = nullCalendar->advance(anchorDate, yearsToSettlement, Years);
    Date d = nullCalendar->advance(settlement, -5, Days);
    set<Date> averagingDates;
    unsigned int total = (unsigned int) abs(numberOfAveragingDates);
    for (size_t i = 0; i < total; ++i) {
        averagingDates.insert(d);
        d = nullCalendar->advance(d, -1, Days);
    }
    vector<Date> averagingDatesAsVector = vector<Date>(averagingDates.begin(), averagingDates.end());

    return boost::shared_ptr<AsianForward>(new AsianForward(averagingDates, settlement, strike, volume, bs));
}

boost::shared_ptr<EuropeanOption> GenerateContracts::getDefaultEuropeanOption(Date anchorDate, int yearsToSettlement, double strike, double volume, BuySell bs, PutCall pc, double premium, bool dp)
{
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());
    Date settlement = nullCalendar->advance(anchorDate, yearsToSettlement, Years);
    Date rateSetDate = nullCalendar->advance(settlement, -2, Days);

    Date dpDate;
    if (dp == true)
        dpDate = settlement;
    else
        dpDate = anchorDate;

    return boost::shared_ptr<EuropeanOption>(new  EuropeanOption(rateSetDate, settlement, strike, pc, volume, dpDate, premium, bs));
}

boost::shared_ptr<AsianOption> GenerateContracts::getDefaultAsianOption(Date anchorDate, int yearsToSettlement, int numberOfAveragingDates, double strike, double volume, BuySell bs, PutCall pc, double premium, bool dp)
{
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());
    Date settlement = nullCalendar->advance(anchorDate, yearsToSettlement, Years);
    Date d = nullCalendar->advance(settlement, -5, Days);
    set<Date> averagingDates;
    unsigned int total = (unsigned int) abs(numberOfAveragingDates);
    for (size_t i = 0; i < total; ++i) 
    {
        averagingDates.insert(d);
        d = nullCalendar->advance(d, -1, Days);
    }
    vector<Date> averagingDatesAsVector = vector<Date>(averagingDates.begin(), averagingDates.end());

    Date dpDate;
    if (dp == true)
    {
        dpDate = settlement;
    }
    else
    {
        dpDate = anchorDate;
    }

    return boost::shared_ptr<AsianOption>(new  AsianOption(averagingDates, settlement, strike, pc, volume, dpDate, premium, bs));
}

boost::shared_ptr<FixedCashflow> GenerateContracts::getDefaultFixedCashflowWithPremium(Date anchorDate, int yearsToSettlement, double volume, BuySell bs)
{
    boost::shared_ptr<Calendar> nullCalendar = boost::shared_ptr<Calendar>(new NullCalendar());
    Date cashFlowDate = nullCalendar->advance(anchorDate, yearsToSettlement, Years); // I want this to have the same settlement as the other ontracts if I intput the same year fraction
    Date maturity = nullCalendar->advance(cashFlowDate, -2, Days);



    boost::shared_ptr<FixedCashflow> fcf;
    Date dpDate;
    dpDate = anchorDate;
    fcf = boost::shared_ptr<FixedCashflow>(new  FixedCashflow(cashFlowDate, volume, bs));
    return fcf;
}
